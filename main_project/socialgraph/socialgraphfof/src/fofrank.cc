#include "socialgraph/socialgraphfof/src/fofrank.h"

namespace xce {
namespace socialgraph {
using namespace xce;
using namespace xce::buddy;
using namespace xce::buddy::adapter;
using namespace std;

typedef BuddyByAddTimeNodeSeq::const_iterator IdTimeSeqIter;
typedef boost::unordered_map<int, BuddyByAddTimeNodeSeq> Id2IdTimeSeqMap;
typedef MyUtil::Int2IntMap::const_iterator Int2IntMapIter;
struct Formula{
  int fi_id;
  int fi_frd_num;
  int t_ufi;
  int t_fifof;
};
typedef std::vector<Formula> FormulaVec;
typedef std::vector<Formula>::const_iterator FormulaVecIter;
typedef boost::unordered_map<int, FormulaVec> Int2FormulaVecMap;
typedef boost::unordered_map<int, FormulaVec>::const_iterator Int2FormulaVecMapIter;
const int FoFRankCompute::kAvgFoFNum;
const int FoFRankCompute::kAvgFrdNum;
const int FoFRankCompute::kAvgMutFrdNum;
const int FoFRankCompute::kDispMutFrdNum;

//void FoFRankCompute::getFoFRankSeq(const int hostId, const int begin, const int limit, const FoFRankSeq& fofRankSeq) {
FoFRankSeq FoFRankCompute::getFoFRankSeq(const int hostId, const int begin, const int limit) {
  MCE_INFO("[FoFRankCompute::getFoFRankSeq] hostId: " << hostId << " begin: " << begin << " limit: " << limit);
  //RunTime fofRankTime = RunTime::create("FoFRankCompute");
  //time of adding friends between the user u and all his or her friends f: u add f
  const int u_id = hostId;
  FoFRankSeq fofRankSeq;
  fofRankSeq.reserve(kAvgFoFNum);
  try {
    BuddyByAddTimeNodeSeq uIdTimeSeq = BuddyByAddTimeCacheAdapter::instance().getFriendListAndTime(u_id, -1);
    //fofRankTime.step("[FoFRankCompute::getFoFRankSeq] BuddyByApplicantCacheAdapter getFriendListAndTime: u");
    BuddyByAddTimeNodeSeq fIdTimeSeq;
    fIdTimeSeq.reserve(kAvgFrdNum);
    IntSet uBlockSet;
    uBlockSet.insert(u_id);  //insert the host himself to block set
    //get block list of those who already applied to be the host's friends but are not friends yet
    MyUtil::IntSeq appSeq = RecommendFriendBlock::getBlockApplySeq(u_id);
    //get block list in the user's black list and the user may want to be friends with them 
    MyUtil::IntSeq blackSeq = RecommendFriendBlock::getBlockBlackSeq(u_id);
    uBlockSet.insert(appSeq.begin(), appSeq.end()); 
    uBlockSet.insert(blackSeq.begin(), blackSeq.end());
    //copy(uBlockSet.begin(), uBlockSet.end(), ostream_iterator<int>(std::cout, "\t"));

    Formula formula = {0,0,-1,-1};
    Int2FormulaVecMap fofId2FormulaVecMap;
    for(IdTimeSeqIter uIt = uIdTimeSeq.begin(); uIt != uIdTimeSeq.end(); ++uIt) {
      const int f_id = (*uIt).userId;
      uBlockSet.insert(f_id);  //insert the friend of the host u to block set
      try {
        fIdTimeSeq = BuddyByAddTimeCacheAdapter::instance().getFriendListAndTime(f_id, -1);
      } catch (...) {
        MCE_WARN("[FoFRankCompute::getFoFRankSeq] BuddyByApplicantCacheAdapter f_id: " << f_id << " getFriendListAndTime failed");
        continue;
      }
      formula.fi_id = f_id;
      formula.fi_frd_num = fIdTimeSeq.size();
      formula.t_ufi = (*uIt).time;
      for(IdTimeSeqIter fIt = fIdTimeSeq.begin(); fIt != fIdTimeSeq.end(); ++fIt) {
        const int fof_id = (*fIt).userId;
        formula.t_fifof = (*fIt).time;
        if (fofId2FormulaVecMap.empty()) {
          FormulaVec formulaVec;
          formulaVec.reserve(kAvgMutFrdNum);
          formulaVec.push_back(formula);
          fofId2FormulaVecMap.insert(make_pair(fof_id, formulaVec));
        } else {
          fofId2FormulaVecMap[fof_id].push_back(formula);
        }
      }
    }
    //fofRankTime.step("[FoFRankCompute::getFoFRankSeq] BuddyByApplicantCacheAdapter getFriendListAndTime: all fofs");
  
    FormulaVec formulaVec;
    formulaVec.reserve(kAvgMutFrdNum);
    for (Int2FormulaVecMapIter fofIt = fofId2FormulaVecMap.begin(); fofIt != fofId2FormulaVecMap.end(); ++fofIt) {
      FoFRank fofRank;
      const int fof_id = fofIt->first;
      if (uBlockSet.count(fof_id) > 0)  {  //if the fof is already the friend of the host 
        continue;
      }
      fofRank.fofId = fof_id;
      fofRank.rank = 0.0;
      formulaVec = fofIt->second;    
      fofRank.mutualFriendNumber = formulaVec.size(); 
      //fofRank.mutualFriendIdSeq.reserve(kDispMutFrdNum); //useless, most fof has only one mutual friend with the host u 
      for (FormulaVecIter fiIt = formulaVec.begin(); fiIt != formulaVec.end(); ++fiIt) {
      	const int fi_id = (*fiIt).fi_id;
        const int t_ufi = (*fiIt).t_ufi;
      	const int t_fifof = (*fiIt).t_fifof;
      	int fi_frd_num = (*fiIt).fi_frd_num;
        double time_i = getDayNum(t_ufi, t_fifof);
        //fofRankTime.step("[FoFRankCompute::getFoFRankSeq] getDayNum");
        //calculate the weight of fof to be recommended to the user u according to the formulation of "Facebook's People You may know"
        if ((fi_frd_num != 0) && (time_i != 0)) {
            fofRank.rank += pow(time_i,-0.3)/sqrt(fi_frd_num);
        } else { //if friend number of fi is 0, then ff.rank = ?
          if (time_i > 0) {
            //if (time_i < 1) {
            //  time_i = 1;
            //}
            if (fi_frd_num ==0) {
              fi_frd_num = kAvgFrdNum;
            }
            fofRank.rank += pow(time_i,-0.3)/sqrt(fi_frd_num);
          } else {
  	    CLOG(ERROR) << "fi_id " << fi_id << "fi_frd_num " << fi_frd_num << " time_i " << time_i << " t_ufi " << t_ufi << " t_fifof " << t_fifof;
          }
        }
        if (fofRank.mutualFriendIdSeq.size() < (size_t)kDispMutFrdNum) {
          fofRank.mutualFriendIdSeq.push_back(fi_id); 
        }
      } //for all mutual friends between host u and each fof
      fofRankSeq.push_back(fofRank);
      //CLOG(INFO) << "final fofRank.rank " << fofRank.rank << std::endl;
      //fofRankTime.step("[FoFRankCompute::getFoFRankSeq] for each fof");
    }
    //CLOG(INFO) << "fofRankSeq size " << fofRankSeq.size() ; 
  } catch (...) {
    MCE_WARN("[FoFRankCompute::getFoFRankSeq] BuddyByApplicantCacheAdapter u_id: " << u_id << " getFriendListAndTime failed");
    return FoFRankSeq();
  }

  if (limit < 0) {
    sort(fofRankSeq.begin(), fofRankSeq.end(), CompareRank);
    return fofRankSeq;
  }
  
  if ((size_t)begin >= fofRankSeq.size() || begin < 0) {
    return FoFRankSeq();
  } 

  if ((size_t)(begin+limit) >= fofRankSeq.size()) {
    partial_sort(fofRankSeq.begin() + begin, fofRankSeq.end(), fofRankSeq.end(), CompareRank);
    return FoFRankSeq(fofRankSeq.begin() + begin, fofRankSeq.end());
  } else {
    partial_sort(fofRankSeq.begin() + begin, fofRankSeq.begin() + begin + limit, fofRankSeq.end(), CompareRank);
    return FoFRankSeq(fofRankSeq.begin() + begin, fofRankSeq.begin() + begin + limit);
  }
}

//FoFRankSeq FoFRankCompute::getFoFRankSeq(const int hostId) {
//  RunTime fofRankTime = RunTime::create("FoFRankCompute");
//  //time of adding friends between the user u and all his or her friends f: u add f
//  FoFRankSeq fofRankSeq;
//  fofRankSeq.reserve(kAvgFoFNum);
//  const int u_id = hostId;
//  Id2IdTimeSeqMap fId2IdTimeSeqMap;
//  BuddyByAddTimeNodeSeq fofIdTimeSeq;
//  fofIdTimeSeq.reserve(kAvgFoFNum);
//  //FormulaVec formulaVec;
//  //formulaVec.reserve(kAvgMutFrdNum);
//  Int2FormulaVecMap fofId2FormulaVecMap;
//  IntVec fiIdVec;
//  fiIdVec.reserve(kAvgMutFrdNum);
//  MyUtil::Int2IntMap fiFrdNumMap;
//  try {
//    //BuddyByAddTimeCacheAdapter* Adapter = new BuddyByAddTimeCacheAdapter::instance();
//    BuddyByAddTimeNodeSeq uIdTimeSeq = BuddyByAddTimeCacheAdapter::instance().getFriendListAndTime(u_id, -1);
//    //fofRankTime.step("[FoFRankCompute::getFoFRankSeq] BuddyByApplicantCacheAdapter getFriendListAndTime: u");
//    BuddyByAddTimeNodeSeq fIdTimeSeq;
//    fIdTimeSeq.reserve(kAvgFrdNum);
//    Formula formula = {0,0,-1,-1};
//    for(IdTimeSeqIter uIt = uIdTimeSeq.begin(); uIt != uIdTimeSeq.end(); ++uIt) {
//      const int f_id = (*uIt).userId;
//      try {
//        fIdTimeSeq = BuddyByAddTimeCacheAdapter::instance().getFriendListAndTime(f_id, -1);
//      } catch (...) {
//        MCE_WARN("[FoFRankCompute::getFoFRankSeq] BuddyByApplicantCacheAdapter f_id: " << f_id << " getFriendListAndTime failed");
//        continue;
//      }
//      fId2IdTimeSeqMap.insert( make_pair(f_id, fIdTimeSeq) );
//      formula.fi_id = f_id; 
//      formula.fi_frd_num = fIdTimeSeq.size();
//      formula.t_ufi = (*uIt).time;
//      for(IdTimeSeqIter fIt = fIdTimeSeq.begin(); fIt != fIdTimeSeq.end(); ++fIt) {
//        const int fof_id = (*fIt).userId;
//        formula.t_fifof = (*fIt).time;      
//        if (!fofId2FormulaVecMap.empty()) { 
//          //Int2FormulaVecMapIter it = fofId2FormulaVecMap.find(fof_id);
//          ////FormulaVecPtr formulaVecPtr = (*it).second;
//          //FormulaVec formulaVec = it->second;
//          ////formulaVec.push_back(formula);
//          //(it->second).push_back(formula);
//          fofId2FormulaVecMap[fof_id].push_back(formula); 
//        } else {
//          FormulaVec formulaVec;
//          formulaVec.reserve(kAvgMutFrdNum);
//          formulaVec.push_back(formula);
//          fofId2FormulaVecMap.insert(make_pair(fof_id, formulaVec)); 
//        }
//        fofIdTimeSeq.push_back(*fIt);
//      }
//    }
//    //CLOG(INFO) << "fof size " << fofIdTimeSeq.size(); 
//    //fofRankTime.step("[FoFRankCompute::getFoFRankSeq] BuddyByApplicantCacheAdapter getFriendListAndTime: all fofs");
//
//    for(IdTimeSeqIter uIt = uIdTimeSeq.begin(); uIt != uIdTimeSeq.end(); ++uIt) {
//      const int f_id = (*uIt).userId;
//      Id2IdTimeSeqMap::const_iterator it = fId2IdTimeSeqMap.find(f_id);
//      fIdTimeSeq = it->second;
//      for(IdTimeSeqIter fIt = fIdTimeSeq.begin(); fIt != fIdTimeSeq.end(); ++fIt) {
//       FoFRank fofRank;
//        const int fof_id = (*fIt).userId;
//        fofRank.fofId = fof_id;
//        fofRank.rank = 0.0;
//
//        //get mutual friends fi between the user u and the user's friends of friends fof
//        fiIdVec = MutualFriend::getMutualFriendList(hostId, fof_id);
//	//fofRankTime.step("[FoFRankCompute::getFoFRankSeq] getMutualFriendList");
//        //get the friend number of each mutual friend
//        try {
//          //get the friend number of each mutual friend
//          fiFrdNumMap = xce::buddy::adapter::BuddyByIdCacheAdapter::instance().getFriendCounts(fiIdVec);
//          //fofRankTime.step("[FoFRankCompute::getFoFRankSeq] getFriendCounts(fi_frd_vec)");
//        } catch (...) {
//          MCE_WARN("[FoFRankCompute::getFoFRankSeq] BuddyByIdCacheAdapter fof_id: " << fof_id << " all mutual fis' getFriendCounts() failed");
//          continue;
//        }
//
//        for (IntVecIter fiIdIt = fiIdVec.begin(); fiIdIt != fiIdVec.end(); ++fiIdIt) {
//          //get the time of build friend relationship between the user u and friend fi: u->fi
//      	  const int fi_id = *fiIdIt;
//          int t_ufi = -1;
//          for(IdTimeSeqIter uIt = uIdTimeSeq.begin(); uIt != uIdTimeSeq.end(); ++uIt) {
//            if (fi_id == (*uIt).userId) {
//              t_ufi = (*uIt).time;
//            } else {
//            }
//          }
//
//          //get the time of build friend relationship between the friend of friend fof and the user u's friend fi: fof->fi
//          int t_fifof = -1;
//          for(IdTimeSeqIter fofIt = fofIdTimeSeq.begin(); fofIt != fofIdTimeSeq.end(); ++fofIt) {
//            if (fi_id == (*fofIt).userId) {
//              t_fifof = (*fofIt).time;
//              //CLOG(INFO) << " t_fifof " << t_fifof << std::endl;
//            } else {
//            }
//          }
//          //get the friend number of the user's friend fi
//          MyUtil::Int2IntMap::const_iterator pos = fiFrdNumMap.find(fi_id);
//          const int fi_frd_num = (*pos).second;
//          const int time_i = getDayNum(t_ufi, t_fifof);
//          //fofRankTime.step("[FoFRankCompute::getFoFRankSeq] getDayNum");
//          //calculate the weight of fof to be recommended to the user u according to the formulation of "Facebook's People You may know"
//          if ((fi_frd_num != 0) && (t_ufi != -1) && (t_fifof != -1)) {
//            fofRank.rank += pow(time_i,-0.3)/sqrt(fi_frd_num);
//          } else { //if friend number of fi is 0, then ff.rank = ?
//  	    CLOG(ERROR) << "fi_frd_num " << fi_frd_num << " t_ufi " << t_ufi << " t_fifof " << t_fifof;
//          }
//        } //for all mutual friends between host u and each fof
//        fofRankSeq.push_back(fofRank);
//        //CLOG(INFO) << "final fofRank.rank " << fofRank.rank << std::endl;
//        //fofRankTime.step("[FoFRankCompute::getFoFRankSeq] for each fof");
//       
//      } //for second-layer friend
//    } //for first-layer friend
//    
//    std::cout << "fofRankSeq size " << fofRankSeq.size() << std::endl;
//  } catch (...) {
//    MCE_WARN("[FoFRankCompute::getFoFRankSeq] BuddyByApplicantCacheAdapter u_id: " << u_id << " getFriendListAndTime failed");
//    return FoFRankSeq();
//  }
//}


//FoFRankSeq FoFRankCompute::getFoFRankSeq(const int hostId)
//{
//  RunTime fofRankTime = RunTime::create("FoFRankCompute");
//  //get friends of friends
//  const int u_id = hostId;
//  MyUtil::Int2IntSeqMap f_fof_list_map = FoF::getFoFList(hostId);
//  fofRankTime.step("[FoFRankCompute::getFoFRankSeq] getFoFList");
//  //CLOG(INFO) << " f_fof_list_map size " << f_fof_list_map.size() << std::endl;
//  try {
//    //Adapter = new BuddyByAddTimeCacheAdapter();
//    BuddyByAddTimeCacheAdapter* Adapter = new BuddyByAddTimeCacheAdapter;
//    BuddyByAddTimeNodeSeq uIdTimeSeq = Adapter->getFriendListAndTime(u_id, -1);
//    fofRankTime.step("[FoFRankCompute::getFoFRankSeq] BuddyByApplicantCacheAdapter getFriendListAndTime: u");
//
//    FoFRankSeq fofRankSeq;
//    fofRankSeq.reserve(kAvgFoFNum0);
//    IntVec fof_ids_vec;
//    fof_ids_vec.reserve(kAvgFoFNum0);
//    IntVec fiIdVec;
//    fiIdVec.reserve(kAvgMutFrdNum);
//    Int2IntMap fiFrdNumMap;
//    BuddyByAddTimeNodeSeq fofIdTimeSeq;
//    fofIdTimeSeq.reserve(kAvgFrdNum);
//    for(MyUtil::Int2IntSeqMap::const_iterator it = f_fof_list_map.begin(); it != f_fof_list_map.end(); ++it) {
//    //MyUtil::Int2IntSeqMap::iterator it = f_fof_list_map.begin();
//      FoFRank fofRank;
//      fof_ids_vec = (*it).second;
//      for (IntVecIter fof_id_it = fof_ids_vec.begin(); fof_id_it != fof_ids_vec.end(); ++fof_id_it) {
//        const int fof_id = *fof_id_it;
//        fofRank.fofId = fof_id;
//        fofRank.rank = 0.0;
//
//        //get mutual friends fi between the user u and the user's friends of friends fof
//        fiIdVec = MutualFriend::getMutualFriendList(hostId, fof_id);
//        fofRankTime.step("[FoFRankCompute::getFoFRankSeq] getMutualFriendList");
//
//        try {
//          //get the friend number of each mutual friend
//          fiFrdNumMap = xce::buddy::adapter::BuddyByIdCacheAdapter::instance().getFriendCounts(fiIdVec);
//          fofRankTime.step("[FoFRankCompute::getFoFRankSeq] getFriendCounts");
//        } catch (...) {
//          MCE_WARN("[FoFRankCompute::getFoFRankSeq] BuddyByIdCacheAdapter fof_id: " << fof_id << " all mutual fis' getFriendCounts() failed");
//          continue;
//        }
//        //adding friends time between the friends of friends fof and his or her friends: fof add f
//        try {
//          fofIdTimeSeq = Adapter->getFriendListAndTime(fof_id, -1);
//          fofRankTime.step("[FoFRankCompute::getFoFRankSeq] BuddyByApplicantCacheAdapter getFriendListAndTime: fof");
//        } catch (...) {
//          MCE_WARN("[FoFRankCompute::getFoFRankSeq] BuddyByApplicantCacheAdapter fof_id: " << fof_id << " getFriendListAndTime failed");
//          continue;
//        }
//		for (IntVecIter fiIdIt = fiIdVec.begin(); fiIdIt != fiIdVec.end(); ++fiIdIt) {
//		  //get the time of build friend relationship between the user u and friend fi: u->fi
//		  int t_ufi = -1;
//		  const int fi_id = *fiIdIt;
//		  for(IdTimeSeqIter uIt = uIdTimeSeq.begin(); uIt != uIdTimeSeq.end(); ++uIt) {
//			if (fi_id == (*uIt).userId) {
//			  t_ufi = (*uIt).time;
//			} else {
//			}
//		  }
//		  //get the time of build friend relationship between the friend of friend fof and the user u's friend fi: fof->fi
//		  int t_fifof = -1;
//		  for(IdTimeSeqIter fofIt = fofIdTimeSeq.begin(); fofIt != fofIdTimeSeq.end(); ++fofIt) {
//			if (fi_id == (*fofIt).userId) {
//			  t_fifof = (*fofIt).time;
//			} else {
//			}
//		  }
//		  //get the friend number of the user's friend fi
//		  Int2IntMapIter pos = fiFrdNumMap.find(fi_id);
//		  const int fi_frd_num = (*pos).second;
//		  //CLOG(INFO) << "fi_frd_num "<< fi_frd_num << std::endl;
//		  const int time_i = getDayNum(t_ufi, t_fifof);
//		  //CLOG(INFO) << "time_i " << time_i << std::endl;
//		  fofRankTime.step("[FoFRankCompute::getFoFRankSeq] getDayNum");
//		  //calculate the weight of fof to be recommended to the user u according to the formulation of "Facebook's People You may know"
//		  if ((fi_frd_num != 0) && (t_ufi != -1) && (t_fifof != -1)) {
//			fofRank.rank += pow(time_i,-0.3)/sqrt(fi_frd_num);
//			//CLOG(INFO) << "fofRank.rank " << fofRank.rank << std::endl;
//		  } else { //if friend number of fi is 0, then ff.rank = ?
//			CLOG(ERROR) << "fi_frd_num " << fi_frd_num << " t_ufi " << t_ufi << " t_fifof " << t_fifof;
//			continue;
//		  }
//		  fofRankTime.step("[FoFRankCompute::getFoFRankSeq] facebook formulation");
//		} //for all mutual friends between host u and each fof
//
//        //CLOG(INFO) << "final fofRank.rank " << fofRank.rank << std::endl;
//        fofRankSeq.push_back(fofRank);
//        fofRankTime.step("[FoFRankCompute::getFoFRankSeq] for each fof");
//      } //for rank of each friend's all fofs
//    } //for rank of all friends' fofs
//
//    fofRankTime.step("[FoFRankCompute::getFoFRankSeq] for each hostId");
//    return fofRankSeq;
//  } catch (...) {
//    MCE_WARN("[FoFRankCompute::getFoFRankSeq] BuddyByApplicantCacheAdapter u_id: " << u_id << " getFriendListAndTime failed");
//    return FoFRankSeq();
//  }
//}


};
};
