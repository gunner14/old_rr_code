#include <iostream>
#include <vector>
#include <boost/random.hpp>

#include "socialgraph/socialgraphfof/src/recommendfriendblock.h"
#include "socialgraph/socialgraphfof/src/fofrank.h"
#include "SocialGraphFoFAdapter.h"
//#include "socialgraph/socialgraphfof/dbutil/fofdbutil.h"

#include "item.pb.h"
#include "socialgraph/socialgraphutil/mc_adapter.h"
#include "OceCxxAdapter/src/SocialGraphRcdTripodAdapter.h"


using namespace com::xiaonei::xce;
using namespace xce::socialgraph;
using namespace MyUtil;
using namespace std;

typedef std::vector<FoFRank>::const_iterator FoFRankSeqIter;
/******************************************************************************/
//Test for FoFRank computing service 
void OutPut(const int hostId, const FoFRankSeq& fofRankSeq) {
  std::cout << "out put the result" << std::endl;
  for(FoFRankSeq::const_iterator it = fofRankSeq.begin(); it != fofRankSeq.end(); ++it) {
    FoFRank fofRank = *it;
    std::cout << "hostId: " << hostId << " fofId: " << (*it).fofId << " rank: " << (*it).rank << " mutualFriendNumber: "
              << (*it).mutualFriendNumber << " mutualFriendIdSeq: " << std::endl;
    for (IntVecIter fiIdIt = (*it).mutualFriendIdSeq.begin();  fiIdIt != (*it).mutualFriendIdSeq.end(); ++fiIdIt) {
      std::cout << (*fiIdIt) << " ";
    }
    std::cout << std::endl;
  }
}

int main(int argc, char**argv)
{
  //InitLogger("main");
  RunTime t_main = RunTime::create("main timecost");
  ConnectionPoolManager::instance().initialize();
  boost::mt19937 rng;       //凭空产生随机性，参见伪随机数发生器
  const static int kMin = 10000000;
  const static int kMax = 100000000;
  boost::uniform_smallint<> dist(kMin,kMax);     //  映射到1..6的分布，参见随机数分布
  boost::variate_generator<boost::mt19937&, boost::uniform_smallint<int> >::variate_generator die(rng, dist);  // 粘合随机性和映射
  //int i = kMin;
  FoFRankSeq fofRankSeq; 
  //while (i++ < kMax) {
  //  int hostId = die(); //uniform distributed index generated by the random generator
  //  //int hostId = i; //uniform distributed index generated by the random generator
  //  try {
  //    std::cout << "main in hostId: " << hostId << std::endl;
  //    fofRankSeq = SocialGraphFoFAdapter::instance().getFoFRankSeq(hostId, 1000); 
  //    OutPut(hostId, fofRankSeq);
  //    //fofRankSeq = xce::socialgraph::FoFRankCompute::getFoFRankSeq(hostId, 0, 1000); 
  //    std::cout << "main out" << std::endl;
  //  } catch (std::exception& e) {
  //    CLOG(INFO) << "main hostId: " << hostId << " std Exception " << e.what();
  //    continue;
  //  } catch (...) {
  //    CLOG(INFO) << "main hostId: " << hostId << " unknown Exception ";
  //    continue;
  //  }
  //}
  //int hostId;
  //hostId = 44674;
  //fofRankSeq = SocialGraphFoFAdapter::instance().getFoFRankSeq(hostId, 1000); 
  //OutPut(hostId, fofRankSeq);
  //hostId = 231815969;
  //fofRankSeq = SocialGraphFoFAdapter::instance().getFoFRankSeq(hostId, 1000); 
  //OutPut(hostId, fofRankSeq);

  //const int hostId = 256404211;
  int n = 0;
  int num = 0;
  std::cout << "Input number of hosts needed to be calculated: " << std::endl;
  cin >> n;
  int hostId = -1;
  std::vector<int> hostIds;
  while (n-- > 0) {
    std::cout << "Input the " << ++num << "-th host Id: " << std::endl;
    //const int hostId = 231815969;
    cin >> hostId;
    hostIds.push_back(hostId);
  }

  ////IntVec blockIds = RecommendFriendBlock::getBlockApplySeq(hostIds[0]);
  ////IntVec blockIds = RecommendFriendBlock::getBlockBlackSeq(hostIds[0]);
  ////IntVec blockIds = RecommendFriendBlock::getBlockSeq(hostIds[0]);
  ////for (IntVecIter it = blockIds.begin(); it != blockIds.end(); ++it) {
  ////  std::cout << *it << " "; 
  ////}

  std::cout << "Input begin: " << std::endl;
  int begin = -1;
  cin >> begin;
  std::cout << "Input limit: " << std::endl;
  int limit = -1;
  cin >> limit;
  
//FoFRankSeq fofRankSeqs = FoFRankCompute::getFoFRankSeq(hostIds[0], begin, limit);
  if (num == 1) {
    std::cout << "one host" << std::endl;
    hostId = hostIds[0];
    if (begin <= 0) {
      std::cout << "fetch all" << std::endl;
      fofRankSeq = SocialGraphFoFAdapter::instance().getFoFRankSeq(hostId, limit);
      std::cout << "fetch over" << std::endl;
      Items items;
      for (FoFRankSeqIter it = fofRankSeq.begin(); it != fofRankSeq.end(); ++it) {
	     Item* item = items.add_items();
	     item->set_id((*it).fofId);
	     item->set_field((*it).mutualFriendNumber);
	     try {
	    	item->add_messages(boost::lexical_cast<std::string>((*it).rank));
	     } catch (boost::bad_lexical_cast& e) {
	        MCE_WARN("boost::bad_lexical_cast" << e.what() << std::endl);
	     }
	     item->set_message("FOF");
	     for (IntVecIter idx = (*it).mutualFriendIdSeq.begin(); idx != (*it).mutualFriendIdSeq.end(); ++idx) {
	    	 item->add_fields(*idx);
	     }
	  }
	   
	  //std::vector<unsigned char> bin_data; 
	  //int size = items.ByteSize();
	  //bin_data.resize(size);
      //items.SerializeToArray(bin_data.data(), size);  
	  //try {
   	  //  SocialGraphRcdTripodAdapter::instance().Set(boost::lexical_cast<std::string>(hostId),
	  //  		  std::string(bin_data.begin(), bin_data.end()), 0);
	  //} catch (boost::bad_lexical_cast& e) {
	  //    MCE_WARN("boost::bad_lexical_cast" << e.what() << std::endl);
	  //}

      //std::string value;
	  //try {
	  //    SocialGraphRcdTripodAdapter::instance().Get(boost::lexical_cast<std::string>(hostId), &value);
	  //} catch (boost::bad_lexical_cast& e) {
	  //    MCE_WARN("boost::bad_lexical_cast" << e.what() << std::endl);
	  //}
      //std::vector<unsigned char> read_bin_data;
	  //if (!value.empty()) {
      //   read_bin_data = std::vector<unsigned char>(value.begin(), value.end());   
	  //} else {
	  //   MCE_WARN("SocialGraphRcdTripodAdapter::instance().Get result is null");
	  //}
      //Items read_items;
	  //int counter = 0;
	  //if (read_items.ParseFromArray(read_bin_data.data(), read_bin_data.size())) {
	  //  BOOST_FOREACH(const Item& it, items.items()) {
	  //  	++counter;
	  //  }
	  //  std::cout << "The host's rcd number is: " << num << std::endl;
	  //}  else {
	  //   MCE_WARN("SocialGraphRcdTripodAdapter::instance().Get result is null");
	  //}
    } else {
      std::cout << "fetch part" << std::endl;
      fofRankSeq = SocialGraphFoFAdapter::instance().getFoFRankSeq(hostId, begin, limit); 
      std::cout << "fetch over" << std::endl;
    }
    //OutPut(hostId, fofRankSeq);
    //FoFRankDbUtil::setFoFRankSeq(hostId, fofRankSeq);
    //RecordVec recordVec = FoFRankDbUtil::getFoFRankSeq(hostId);
  }
  //if (num > 1) { 
  //  std::cout << "mutiple hosts" << std::endl;
  //  Int2FoFRankSeqMap hostId2FoFRankSeqMap;
  //  if (begin <= 0) {
  //    std::cout << "fetch all" << " size " << hostIds.size() << std::endl;
  //    hostId2FoFRankSeqMap = SocialGraphFoFAdapter::instance().getFoFRankSeqs(hostIds, limit);
  //    std::cout << "fetch over" << std::endl;
  //    



  //    //FoFRankDbUtil::setFoFRankSeqs(hostId2FoFRankSeqMap);
  //    //std::cout << "set over" << std::endl;
  //    //Int2RecordVecMap id2VecMap = FoFRankDbUtil::getFoFRankSeqs(hostIds);
  //    //std::cout << "get over" << std::endl;
  //  } else {
  //    std::cout << "fetch all" << " size " << hostIds.size() << std::endl;
  //    hostId2FoFRankSeqMap = SocialGraphFoFAdapter::instance().getFoFRankSeqs(hostIds, begin, limit);
  //    //FoFRankDbUtil::setFoFRankSeqs(hostId2FoFRankSeqMap);
  //    //Int2RecordVecMap id2VecMap = FoFRankDbUtil::getFoFRankSeqs(hostIds);
  //    //std::cout << "fetch over" << std::endl;
  //  }
  //  for (Int2FoFRankSeqMap::iterator it = hostId2FoFRankSeqMap.begin(); it != hostId2FoFRankSeqMap.end(); ++it) {
  //    //OutPut(it->first, it->second);
  //  } 
  //}
  t_main.step("main timecost for one hostId");
}