
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "base/logging.h"
#include "base/ptime.h"
#include "base/threadpool.h"
#include "base/logrotate.h"

#include "OceSlice/slice/FriendClusterCache.h"
#include "OceCxxAdapter/src/FriendClusterCacheAdapter.h"
#include "OceCxxAdapter/src/BuddyByIdReplicaAdapter.h"
#include "OceCxxAdapter/src/BuddyFriendsAdapter.h"

#include "OceCxxAdapter/src/BuddyCoreAdapter.h"

#include "socialgraph/socialgraphutil/mc_adapter.h"
#include "socialgraph/socialgraphutil/algostuff.h"
#include "socialgraph/socialgraphproto/item.pb.h"
#include "socialgraph/socialgraphutil/dbutil.h"

#include "time.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"


namespace xce {
namespace socialgraph {

struct FriendClusterUnit_S {
  int user_id_;
  int u_weight_;
};

struct FriendCluster_S {
  std::string c_name_;
  double c_weight_;
  std::vector<FriendClusterUnit_S> units_;
};

void dumpClusterMap(std::hash_map<std::string, FriendCluster_S>* cd_map){
  std::cout << "================dump clusterMap=========================" << endl;
  std::hash_map<std::string, FriendCluster_S>::iterator it = cd_map->begin();
  for(; it != cd_map->end(); ++it) {
    std::string c_name = it->first;
    std::cout << c_name << ":\t";
    FriendCluster_S fris = it->second;
    BOOST_FOREACH(const FriendClusterUnit_S& fcu, fris.units_) {
      std::cout << fcu.user_id_ << "\t";
    }
    std::cout << endl;
  }
  std::cout << "================dump clusterMap over=====================" << endl;
}

void dumpUid2ClusterMap(std::hash_map<int, std::vector<std::string> >* uid2clustername_map) {
  std::cout << "================dump uid2clusterMap=========================" << endl;
  std::hash_map<int, std::vector<std::string> >::iterator it = uid2clustername_map->begin();
  for(; it != uid2clustername_map->end(); ++it) {
    int uid = it->first;
    std::cout << uid << ":\t";
    std::vector<std::string>  c_name_seq = it->second;
    BOOST_FOREACH(const string& c_name, c_name_seq) {
      std::cout << c_name << "\t";
    }
    std::cout << endl;
  }
  std::cout << "================dump uid2clusterMap over====================" << endl;
}



void Merge_UN_HS(std::ifstream* un_cluster_in, std::ifstream* un_uid_in, 
		     std::hash_map<std::string, FriendCluster_S>* cluster_data_map,
		     std::hash_map<int, std::vector<std::string> >* uid2clustername_map,
		     std::string tag) {

  PTIME(pt, "Merge_UN_HS", true, 0);
  
  //load clusterdata file
  std::string line; //1-1070-1997-0     1.5     305148731,3;312498668,1;743864307,1;744248339,1;
  std::getline(*un_cluster_in, line);
  do {
    std::vector<std::string> splits;
    boost::split(splits, line, boost::is_any_of("\t"));
    if (splits.size() == 3) {
      FriendCluster_S fc;
      fc.c_name_ = splits[0] + "-" + tag;
      try {
        fc.c_weight_ = boost::lexical_cast<double>(splits[1]);
      } catch(...) {   
        fc.c_weight_ = 0;
      } 
      std::vector<std::string> sub_splits;
      boost::split(sub_splits, splits[2], boost::is_any_of(";"));
      BOOST_FOREACH(const std::string& s, sub_splits) {
        std::vector<std::string> seq;
        boost::split(seq, s, boost::is_any_of(","));
        if (seq.size() == 2) {
          FriendClusterUnit_S fcu;
          try {
            fcu.user_id_ = boost::lexical_cast<int>(seq[0]);
            fcu.u_weight_ = boost::lexical_cast<int>(seq[1]);
          } catch (...) { 
            continue;
          } 
          fc.units_.push_back(fcu);
        } 
      } 
      cluster_data_map->insert(std::make_pair(fc.c_name_, fc));
    } 
    line.clear();
    std::getline(*un_cluster_in, line);
  } while(!line.empty());
  
  LOG(INFO) << "After merging, cluster_data_map->size():" << cluster_data_map->size();

//  dumpClusterMap(cluster_data_map);

  //load uid2cluster file  
  std::getline(*un_uid_in, line);
  do {
    std::vector<std::string> splits;
    boost::split(splits, line, boost::is_any_of("\t"));
    if (splits.size() == 2) {
      int uid = -1;
      std::vector<std::string> cname_seq;
      try {
        uid = boost::lexical_cast<int>(splits[0]);
        std::vector<std::string> sub_splits;
        boost::split(sub_splits, splits[1], boost::is_any_of(";"));
        BOOST_FOREACH(const std::string& s, sub_splits) {
          std::vector<std::string> seq;
          boost::split(seq, s, boost::is_any_of("|"));
          if (seq.size() == 2) {
            cname_seq.push_back(seq[0] + "-" + tag);
          }
        }
      } catch (...) {
      }

      if (uid2clustername_map->find(uid) != uid2clustername_map->end())
      {
        std::hash_map<int, std::vector<std::string> >::iterator it;
        it = uid2clustername_map->find(uid);
        std::vector<std::string> cname_seq_in_map = it->second;
        cname_seq.insert(cname_seq.begin(), cname_seq_in_map.begin(), cname_seq_in_map.end());

        uid2clustername_map->erase(it);
      }
      uid2clustername_map->insert(std::make_pair(uid, cname_seq));
    }
    line.clear();
    std::getline(*un_uid_in, line);
  } while(!line.empty());

  LOG(INFO) << "After merging, uid2clustername_map->size():" << uid2clustername_map->size();
//  dumpUid2ClusterMap(uid2clustername_map);

}



void LoadClusterData(std::ifstream* in, std::hash_map<std::string, FriendCluster_S>* cluster_data_map) {
  PTIME(pt, "LoadClusterData", true, 0);
  std::string line; //1-1070-1997-0	1.5	305148731,3;312498668,1;743864307,1;744248339,1;
  std::getline(*in, line);
  do {
    std::vector<std::string> splits;
    boost::split(splits, line, boost::is_any_of("\t"));
    if (splits.size() == 3) {
      FriendCluster_S fc;
      fc.c_name_ = splits[0];
      try {
        fc.c_weight_ = boost::lexical_cast<double>(splits[1]);
      } catch(...) {  
        fc.c_weight_ = 0;
      }
      std::vector<std::string> sub_splits;
      boost::split(sub_splits, splits[2], boost::is_any_of(";"));
      BOOST_FOREACH(const std::string& s, sub_splits) {
        std::vector<std::string> seq;
        boost::split(seq, s, boost::is_any_of(","));
        if (seq.size() == 2) {
          FriendClusterUnit_S fcu;
          try {
            fcu.user_id_ = boost::lexical_cast<int>(seq[0]);
            fcu.u_weight_ = boost::lexical_cast<int>(seq[1]);
          } catch (...) {
            continue;
          }
          fc.units_.push_back(fcu);
        }
      }
      cluster_data_map->insert(std::make_pair(fc.c_name_, fc));
    } 
    line.clear();
    std::getline(*in, line);
  } while(!line.empty());

  LOG(INFO) << "cluster_data_map->size():" << cluster_data_map->size();
}

void LoadUid2ClusterData(std::ifstream* in, std::hash_map<int, std::vector<std::string> >* uid2clustername_map) {
  PTIME(pt, "LoadUid2ClusterData", true, 0);
  std::string line; //100577418	99-1044-2005-6|11;99-1044-2005-3|11;99-1044-2005-9|11;99-1044-2005-8|11;
  std::getline(*in, line);
  do {
    std::vector<std::string> splits;
    boost::split(splits, line, boost::is_any_of("\t"));
    if (splits.size() == 2) {
      int uid = -1;
      std::vector<std::string> cname_seq;
      try {
        uid = boost::lexical_cast<int>(splits[0]);
        std::vector<std::string> sub_splits;
        boost::split(sub_splits, splits[1], boost::is_any_of(";"));
        BOOST_FOREACH(const std::string& s, sub_splits) {
          std::vector<std::string> seq;
          boost::split(seq, s, boost::is_any_of("|"));
          if (seq.size() == 2) {
            cname_seq.push_back(seq[0]);
          }
        }
      } catch (...) {

      }
      uid2clustername_map->insert(std::make_pair(uid, cname_seq));
    }
    line.clear();
    std::getline(*in, line);
  } while(!line.empty());
  LOG(INFO) << "uid2clustername_map->size():" << uid2clustername_map->size();
}




class ComputeMessage : public xce::Message {
	
 public:
	 static int64_t totalbytes_;
  ComputeMessage(MyUtil::IntSeq ur_set, 
                 std::hash_map<std::string, FriendCluster_S>* cd_map, 
                 std::hash_map<int, std::vector<std::string> >* uc_map) : user_id_set_(ur_set), cluster_data_map_(cd_map), uid2clustername_map_(uc_map) {

  }
 
  virtual ~ComputeMessage() { }

	void dumpMap(std::hash_map<int, std::set<int> > d_map) {
		std::hash_map<int, std::set<int> >::iterator ir = d_map.begin();
		for (; ir != d_map.end(); ++ir) {
			std::cout << "key:\t" << ir->first << std::endl;
			std::cout << "value:\t" ;
			BOOST_FOREACH(int id, ir->second) {
				std::cout << id << "\t";
			}
			std::cout << std::endl;
		}
	}

	void dumpSeq(std::vector<int> d_seq) {
		BOOST_FOREACH(int id, d_seq) {
			std::cout << id << "\t";
		}
		std::cout << std::endl;
	}

/*
  void dumpMap_b(std::hash_map<int, std::vector<int> > d_map) {
    std::hash_map<int, std::set<int> >iterator ir = d_map.begin();
    for (; ir != d_map.end(); ++ir) {
      std::cout << "key:\t" << ir->first << std::endl;
      std::cout << "value:\t" ;
      BOOST_FOREACH(int id, ir->second) {
        std::cout << id << "\t";
      }   
      std::cout << std::endl;
    }   
  }           
*/

  bool GetRecommend(int uid, std::set<int>* recommend_set ) {
    std::hash_map<int, std::vector<std::string> >::iterator it1 = uid2clustername_map_->find(uid);
    std::vector<std::string> c_name_seq;

		if (it1 != uid2clustername_map_->end()) {
      c_name_seq = it1->second;
    }
    if (c_name_seq.empty()) {
      LOG(INFO) << "user_id:" << uid << " cluster_name_seq's size is 0";
      return false;
    }
   
    BOOST_FOREACH(const std::string& c_name, c_name_seq) {
      std::hash_map<std::string, FriendCluster_S>::iterator it2 = cluster_data_map_->find(c_name);
      if (it2 != cluster_data_map_->end()) {
        BOOST_FOREACH(const FriendClusterUnit_S& fcu, (it2->second).units_) {
          recommend_set->insert(fcu.user_id_);
        }
      }
    }
    if (recommend_set->empty()) {
      LOG(INFO) << "user_id:" << uid << " recommend_set is 0";
    }

    return true;
  }
  

  bool GetBlockSeq(int uid, std::set<int>* block_set) {

	    MyUtil::IntSeq friend_seq;
	    try {
	      friend_seq = xce::buddy::adapter::BuddyByIdCacheAdapter::instance().getFriendListAsc(uid, 5000);
	      block_set->insert(friend_seq.begin(), friend_seq.end());
	    } catch (...) {
	      LOG(LERROR) << "BuddyByIdCacheAdapter::instance().getFriendListAsc() exception ... " << "user_id_:" << uid;
	      return false;
	    }

	    try {
	    	std::vector<int> block_seq;
	      block_seq = com::xiaonei::service::buddy::BuddyCoreAdapter::instance().getBlockSeq(uid, 0, -1);
	      block_set->insert(block_seq.begin(), block_seq.end());
	    } catch (...) {
	      LOG(LERROR) << "BuddyCoreAdapter.getBlockSeq ... exception user_id_:" << uid;
	      return false;
	    }

	    std::vector<int> sg_block_seq = DbUtil::GetInstance().GetSgBlockListByBiz(uid, "FFUCR");

	    if (!sg_block_seq.empty()) {
	      block_set->insert(sg_block_seq.begin(), sg_block_seq.end());
	    }

	    block_set->insert(uid);

	    return true;
  }


  void GetBlocklistMap(std::hash_map<int, std::set<int> >* block_map) {
		BOOST_FOREACH(int uid, user_id_set_) {
			std::set<int> block_list;
			block_list.insert(uid);
			block_map->insert(std::make_pair(uid, block_list));
		}

//		std::cout << "GetBlocklistMap dumpblock:step1\t" << std::endl;
//		dumpMap(*block_map);

    try {
      MyUtil::Int2IntSeqMap friend_map =
				xce::buddy::adapter::BuddyByIdCacheAdapter::instance().getFriendLists(user_id_set_);

      MyUtil::Int2IntSeqMap::iterator ir_frimp = friend_map.begin();
      for(; ir_frimp != friend_map.end(); ++ir_frimp) {
        int uid = ir_frimp->first;
        std::set<int> block_list;
        MyUtil::IntSeq fri_list = ir_frimp->second;

				std::hash_map<int, std::set<int> >::iterator ir_bmp = block_map->find(uid);
        ir_bmp->second.insert(fri_list.begin(), fri_list.end());
		}
		}catch (...) {
      LOG(LERROR) << "BuddyByIdCacheAdapter::instance().getFriendLists() exception ... "
                  << "user set size:" << user_id_set_.size();
			BOOST_FOREACH(int uid, user_id_set_) {
			MyUtil::IntSeq friend_seq;
			try {
				friend_seq = xce::buddy::adapter::BuddyByIdCacheAdapter::instance().getFriendListAsc(uid, 5000);
				std::hash_map<int, std::set<int> >::iterator ir_bmp = block_map->find(uid);
				ir_bmp->second.insert(friend_seq.begin(), friend_seq.end());
			} catch (...) {
				LOG(LERROR) << "BuddyByIdCacheAdapter::instance().getFriendListAsc() exception ... " << "user_id_:" << uid;
				std::hash_map<int, std::set<int> >::iterator ir_bmp = block_map->find(uid);
				if (ir_bmp != block_map->end()) {
					block_map->erase(ir_bmp);
				}
			}
		}
	}

//    std::cout << "GetBlocklistMap dumpblock:step2\t" << std::endl;
//    dumpMap(*block_map);

	BOOST_FOREACH(int uid, user_id_set_) {
		std::hash_map<int, std::set<int> >::iterator ir_bmp = block_map->find(uid);
		if (ir_bmp != block_map->end()) {
			try {
				std::vector<int> block_seq;
				block_seq = com::xiaonei::service::buddy::BuddyCoreAdapter::instance().getBlockSeq(uid, 0, -1);
				if(block_seq.size() > 0) {
//					std::cout << "buddy block for:\t" << uid << ":" << std::endl;
//					dumpSeq(block_seq);
				}
				ir_bmp->second.insert(block_seq.begin(), block_seq.end());
			}catch (...) {
        	     LOG(LERROR) << "BuddyCoreAdapter.getBlockSeq ... exception user_id_:" << uid;
        	     block_map->erase(ir_bmp);
			}
		}
	}

//    std::cout << "GetBlocklistMap dumpblock:step3\t" << std::endl;
//    dumpMap(*block_map);

	std::set<int> uidset;
	BOOST_FOREACH(int uid, user_id_set_) {
		uidset.insert(uid);
	}

	try {
		std::hash_map<int, std::vector<int> > sg_block_map = DbUtil::GetInstance().GetSgBlockListByBizBatch(uidset, "FFUCR");
	  std::hash_map<int, std::set<int> >::iterator ir_bmp2 = block_map->begin();
	  for(; ir_bmp2 != block_map->end(); ++ir_bmp2) {
			std::hash_map<int, std::vector<int> >::iterator ir_sgbmp = sg_block_map.find(ir_bmp2->first);
			if (ir_sgbmp != sg_block_map.end()) {
				if (ir_sgbmp->second.size() > 0) {
//					std::cout << "sg block for:\t" << ir_bmp2->first  << ":" << std::endl;
//					dumpSeq(ir_sgbmp->second);
				}
				ir_bmp2->second.insert(ir_sgbmp->second.begin(), ir_sgbmp->second.end());
			}
		}
	} catch(...) {
		BOOST_FOREACH(int uid, user_id_set_) {
			std::vector<int>  sg_block_seq = DbUtil::GetInstance().GetSgBlockListByBiz(uid, "FFUCR");
			if (sg_block_seq.size() > 0) {
//				std::cout << "sg block for:\t" << uid << ":" << std::endl;
//				dumpSeq(sg_block_seq);
			}
			std::hash_map<int, std::set<int> >::iterator ir_bmp2 = block_map->find(uid);
			if (ir_bmp2 != block_map->end()) {
				ir_bmp2->second.insert(sg_block_seq.begin(), sg_block_seq.end());
			}
		}
	}

//    std::cout << "GetBlocklistMap dumpblock:step4\t" << std::endl;
//    dumpMap(*block_map);

}


  void FilterBlockUsers(std::hash_map<int, std::set<int> >* urecommendmap, std::hash_map<int, std::set<int> >* ublockmap) {
	  std::set<int> exception_ids;
	  std::hash_map<int, std::set<int> >::iterator ir_rcdmp = urecommendmap->begin();
	  for (; ir_rcdmp != urecommendmap->end(); ++ir_rcdmp) {
		  if (ublockmap->find(ir_rcdmp->first) == ublockmap->end()) {
			  exception_ids.insert(ir_rcdmp->first);
		  }
	  }

	  BOOST_FOREACH(int expn_id, exception_ids) {
		  std::hash_map<int, std::set<int> >::iterator ir_rcdmp2 = urecommendmap->find(expn_id);
		  if (ir_rcdmp2 != urecommendmap->end()) {
			  urecommendmap->erase(ir_rcdmp2);
		  }
	  }

	  ir_rcdmp = urecommendmap->begin();
	  std::set<int> emptyrcd_ids;
	  for (; ir_rcdmp != urecommendmap->end(); ++ir_rcdmp) {
		  std::hash_map<int, std::set<int> >::iterator ir_blkmp = ublockmap->find(ir_rcdmp->first);
		  if (ir_blkmp != ublockmap->end()) {
			  BOOST_FOREACH(int bid, ir_blkmp->second) {
				  std::set<int>::iterator ir_rcdst = ir_rcdmp->second.find(bid);
				  if (ir_rcdst != ir_rcdmp->second.end()) {
					  ir_rcdmp->second.erase(ir_rcdst);
				  }
			  }
			  if (ir_rcdmp->second.empty()) {
				  emptyrcd_ids.insert(ir_rcdmp->first);
			  }
		  }
	  }

	  BOOST_FOREACH(int emptyid, emptyrcd_ids) {
		  std::hash_map<int, std::set<int> >::iterator ir_rcdmp3 = urecommendmap->find(emptyid);
		  if (ir_rcdmp3 != urecommendmap->end()) {
			  urecommendmap->erase(ir_rcdmp3);
		  }
	  }

//    std::cout << "Filter block. Rcdmap:\t" << std::endl;
 //   dumpMap(*urecommendmap);
  }

	void dumpItems(Items items) {
		BOOST_FOREACH(Item item, items.items()) {
			std::cout << "id:\t" << item.id() << "\t" << item.field() << std::endl;
			BOOST_FOREACH(int i, item.fields()) {
				std::cout << i << "\t";
			}
			std::cout << std::endl;
			std::cout << "exp:\t" << item.messages(0) << std::endl;
		}

	}


  void BuildItems_FriendCluster(std::hash_map<int, std::set<int> >* uid2recommend_map) {
	  std::hash_map<int, std::set<int> >::iterator ir_rcdmp = uid2recommend_map->begin();
	  for (; ir_rcdmp != uid2recommend_map->end(); ++ir_rcdmp) {
		  int hostid = ir_rcdmp->first;
		  std::set<int> rcd_set = ir_rcdmp->second;
		  Items items;

		  std::hash_map<int, std::vector<std::string> >::iterator ir_host = uid2clustername_map_->find(hostid);
		  std::set<std::string> host_cluster_set;
		  if (ir_host != uid2clustername_map_->end()) {
			  BOOST_FOREACH(std::string host_cluster, ir_host->second) {
		  		 host_cluster_set.insert(host_cluster);
		  	 }
		  }

		  BOOST_FOREACH(int rcdid, rcd_set) {
			  std::string explanation;
			  std::hash_map<int, std::vector<std::string> >::iterator ir_rcd = uid2clustername_map_->find(rcdid);

			  if (ir_rcd != uid2clustername_map_->end()) {
				  BOOST_FOREACH(std::string rcd_cluster, ir_rcd->second) {
					  if (host_cluster_set.count(rcd_cluster) > 0) {
						  explanation = rcd_cluster;
						  break;
					  }
				  }
			  }

			  if (explanation.length() > 0) {
				  //好友簇的解释
				  int friend_cluster_size = -1;
				  std::vector<int> friend_list;

				  std::hash_map<std::string, FriendCluster_S>::iterator ir_cluster = cluster_data_map_->find(explanation);
				  if (ir_cluster != cluster_data_map_->end()) {
				      BOOST_FOREACH(const FriendClusterUnit_S& fcu, (ir_cluster->second).units_) {
				    	  //里面包含hostid和rcdid，要去掉
				    	  if (fcu.user_id_ != hostid && fcu.user_id_ != rcdid){
				    		  friend_list.push_back(fcu.user_id_);
				    	  }
				      }
				  }

				  friend_cluster_size = friend_list.size();

				  if (friend_cluster_size > 0) {
					  Item* item = items.add_items();
					  item->set_id(rcdid);
					  item->set_field(friend_cluster_size);
						int count = 0;
					  BOOST_FOREACH(int cid, friend_list) {
						  if(count < 10) {
							  item->add_fields(cid);
							  count++;
						  }
						  else {
							  break;
						  }
					  }
						std::vector<std::string> exp_seq;
						std::cout << explanation << std::endl;
						ParseExpStr(explanation, &exp_seq);
						BOOST_FOREACH(std::string s, exp_seq) {
							std::cout << s << std::endl;
							item->add_messages(s);
						}
				  }
			  }
			  else {
				  //共同好友的解释
				  try {
				  	   MyUtil::IntSeq cf = xce::buddy::adapter::BuddyFriendsCacheAdapter::instance().getCommonFriends(hostid, rcdid);

				  	   int common_friend_size = -1;
				  	   std::vector<int> common_friend_list;
				  	   common_friend_size = cf.size();
				  	   if (common_friend_size > 10) {
				  		   for (size_t i = 0; i < 10; ++i) {
				  	 	     	common_friend_list.push_back(cf[i]);
				  	 	    }
				  	 	}
				  	 	else {
				  	 	     common_friend_list = cf;
				  	 	}

				  	 	if (common_friend_size > 0) {
				  	 	     Item* item = items.add_items();
				  	 	     item->set_id(rcdid);
				  	 	     item->set_field(common_friend_size);
				  	 	     BOOST_FOREACH(int cid, common_friend_list) {
				  	 	    	 item->add_fields(cid);
				  	 	     }
									 std::string cfs = boost::lexical_cast <string>(common_friend_size);
									 item->add_messages(cfs + "个共同好友");
				  	 	}
				  }catch(...){
				  }
			  }
		  }
			std::cout << "hostid:\t" << hostid <<std::endl;
//			dumpItems(items);
	     int byte_size = items.ByteSize();
	     std::string s;
	     items.SerializeToString(&s);
	     std::vector<unsigned char> bin_data;
	     bin_data.reserve(byte_size);
	     bin_data.assign(s.data(), s.data() + byte_size);
	     const static std::string kBiz = "FFUCR";
	 //    int64_t user_id_code = hostid + 100000000000000;
	// 		 BusinessCacheAdapter::GetInstance().Set(kBiz, user_id_code, bin_data);
			 totalbytes_ += byte_size;
	     LOG(INFO) << "RCD INFO user_id_" << hostid << " rcd_set.size():" << rcd_set.size() << "\t total data size:\t" << totalbytes_;

	  }

  }


	void ParseExpStr(std::string explanation, std::vector<std::string>* result) {
		int splitpos = explanation.find_last_of("-");
    if (splitpos != std::string::npos) {
      std::string s = explanation;
      if (explanation.substr(splitpos+1) == "UN") {
        s = explanation.substr(0, splitpos);
      }
      int splitpos1 = s.find_last_of("-");
      if (splitpos1 != std::string::npos) {
    	  std::string expstr = s.substr(0, splitpos1);
				
				int schoolpos = expstr.find_first_of("-");
				std::string school = expstr.substr(0, schoolpos);
				std::string expshow = expstr;
				if (explanation.substr(splitpos+1) == "UN") {
					expshow = expstr.substr(schoolpos+1);
				}
				result->push_back(expshow);
				result->push_back(school);	
      }
    }	

	}


  void BuildItems_CommenFriend(std::hash_map<int, std::set<int> >* uid2recommend_map) {
	  std::hash_map<int, std::set<int> >::iterator ir_rcdmp = uid2recommend_map->begin();
	  for (; ir_rcdmp != uid2recommend_map->end(); ++ir_rcdmp) {
	     	int hostid = ir_rcdmp->first;
	     	std::set<int> rcd_set = ir_rcdmp->second;

	     	MyUtil::Int2IntMap rcd2host;
	     	BOOST_FOREACH(int rcdid, rcd_set) {
	     		rcd2host.insert(make_pair(rcdid, hostid));
	     	}

	     	MyUtil::Int2IntSeqMap rcd2comlist_mp;
	     	try {
	     		MyUtil::Str2IntSeqMap comfri_mp;
	     		comfri_mp = xce::buddy::adapter::BuddyFriendsCacheAdapter::instance().getCommonFriendsBatch(rcd2host);
	     		int hostid_length = log10(hostid) + 1;
	     		MyUtil::Str2IntSeqMap::iterator  ir_cmfmp= comfri_mp.begin();
	     		for(; ir_cmfmp != comfri_mp.end(); ++ir_cmfmp) {
	     			std::string rcd_host = ir_cmfmp->first;
	     			int rid = atoi(rcd_host.substr(0, rcd_host.size() - hostid_length).c_str());
	     			rcd2comlist_mp.insert(std::make_pair(rid, ir_cmfmp->second));
	     		}
	     	}catch(...) {
	     		BOOST_FOREACH(int rid, rcd_set) {
	     			try {
	     				MyUtil::IntSeq cf = xce::buddy::adapter::BuddyFriendsCacheAdapter::instance().getCommonFriends(hostid, rid);
	     				rcd2comlist_mp.insert(std::make_pair(rid, cf));
	     			}catch(...) {
		     		    continue;
		     		}
	     		}
	     	}

	     	Items items;
	     	MyUtil::Int2IntSeqMap::iterator ir_r2clistmp = rcd2comlist_mp.begin();
	     	for (; ir_r2clistmp != rcd2comlist_mp.end(); ++ir_r2clistmp) {
	     		int rid = ir_r2clistmp->first;
	     		MyUtil::IntSeq cf = ir_r2clistmp->second;

	     		int common_friend_size = -1;
	     		std::vector<int> common_friend_list;
	     		common_friend_size = cf.size();
	     		if (common_friend_size > 10) {
	     		   for (size_t i = 0; i < 10; ++i) {
	     				common_friend_list.push_back(cf[i]);
	     		   }
	     		}
	     		else {
	     		    common_friend_list = cf;
	     		}

	     		if (common_friend_size > 0) {
	     			Item* item = items.add_items();
	     			item->set_id(rid);
	     			item->set_field(common_friend_size);
	     			BOOST_FOREACH(int cid, common_friend_list) {
	     				  item->add_fields(cid);
	     			}
						std::string cfs =	boost::lexical_cast <string>(common_friend_size);
						item->add_messages(cfs + "个共同好友");
	     		}
	     	}

	     	int byte_size = items.ByteSize();
	     	std::string s;
	     	items.SerializeToString(&s);
	     	std::vector<unsigned char> bin_data;
	     	bin_data.reserve(byte_size);
	     	bin_data.assign(s.data(), s.data() + byte_size);
	     	const static std::string kBiz = "FFUCR";
	     	int64_t user_id_code = hostid + 100000000000000;
	   		BusinessCacheAdapter::GetInstance().Set(kBiz, user_id_code, bin_data);
	     	LOG(INFO) << "RCD INFO user_id_" << hostid << " rcd_set.size():" << rcd_set.size();
	  }
  }



  void Run() {
    //1,取好友簇数据
    //2,取uid好友列表
    //3,计算推荐人。过滤掉好友，过滤掉黑名单,过滤掉sg_block
    
	//4,组合Items数据结构
    //5,保存数据到cache
    std::ostringstream otem;
    otem << "ComputeMessage::Run() user_id_set_->size(): " << user_id_set_.size();
    PTIME(pt, otem.str(), true, 0);

    timeval t1;
    gettimeofday(&t1, NULL);
 
	//获得推荐结果
    std::hash_map<int, std::set<int> > uid2recommend_map;

    BOOST_FOREACH (int uid, user_id_set_) {
      std::set<int> recommend_list;
      std::hash_map<int, std::string> rcdexp_mp;
      if (GetRecommend(uid, &recommend_list) && !recommend_list.empty()) {
          uid2recommend_map.insert( std::make_pair(uid, recommend_list));
      }
    }


//		std::cout << "recommend map:\t" << std::endl;
//		dumpMap(uid2recommend_map);

    timeval t2;
    gettimeofday(&t2, NULL);
    double dur1 = xce::detail::timeval_sub(&t1, &t2);
    LOG(INFO) << "processed " << user_id_set_.size() 
	          << " users. -->time of getting cluster data from cluster_data_map:\t" << dur1 << "ms";

	//获得block结果
    std::hash_map<int, std::set<int> > block_map;
    GetBlocklistMap(&block_map);

    //从cluster_user_set中去掉block_set的人
    FilterBlockUsers(&uid2recommend_map, &block_map);

    //组装Items数据,并设置cache
 //   BuildItems_CommenFriend(&uid2recommend_map);
    BuildItems_FriendCluster(&uid2recommend_map);
  }

 private:
  MyUtil::IntSeq user_id_set_;
  std::hash_map<std::string, FriendCluster_S>* cluster_data_map_;
  std::hash_map<int, std::vector<std::string> >* uid2clustername_map_;
};

}}

int64_t xce::socialgraph::ComputeMessage::totalbytes_ = 0;

int main(int argc, char **argv) {
  using namespace xce::socialgraph;
  PTIME(pt, "main", true, 0);
	LOG(INFO) << "argc:" << argc;
  if (argc < 3) {
    LOG(INFO) << "Usage: " << argv[0] << " cluster_data uid2cluster_data ......";
    exit(0);
  }

  if (argc == 3) {
    LOG(INFO) << "Load cluster and uid data. Running:  " << argv[0] << "\t" << argv[1] << "\t" << argv[2];
  }
  // load cluster_data file
  std::ifstream cluster_data_in(argv[1]);
  if (!cluster_data_in) {
    LOG(LERROR) << "cannot open cluster_data file:" << argv[1];
    exit(1);
  }
  
  std::hash_map<std::string, FriendCluster_S> cluster_data_map;
  LoadClusterData(&cluster_data_in, &cluster_data_map);

  //std::hash_map<std::string, FriendCluster_S>::iterator it = cluster_data_map.begin();
  //for (; it != cluster_data_map.end(); ++it) {
  //  LOG(INFO) << it->second.c_name_ << "\t" << it->second.c_weight_;
  //  BOOST_FOREACH(const FriendClusterUnit_S& fcu, it->second.units_) {
  //    LOG(INFO) << "\t" << fcu.user_id_ << "\t" << fcu.u_weight_;
  //  }
  //}

  std::ifstream uid2clustername_in(argv[2]);
  if (!uid2clustername_in) {
    LOG(LERROR) << "cannot open uid2clusterdata file: " << argv[2];
    exit(1);
  }

  std::hash_map<int, std::vector<std::string> > uid2clustername_map;
  LoadUid2ClusterData(&uid2clustername_in, &uid2clustername_map);


  if (argc == 5) {
    std::ifstream add_cluster_data_in(argv[3]);
    if (!add_cluster_data_in) {
      LOG(LERROR) << "cannot open cluster_data file: " << argv[3];
      exit(1);
    }
    std::ifstream add_uid2clustername_in(argv[4]);
    if (!add_uid2clustername_in) {
      LOG(LERROR) << "cannot open uid2clusterdata file: " << argv[4];
      exit(1);
    }
    std::string tag = "UN";
    Merge_UN_HS(&add_cluster_data_in, &add_uid2clustername_in, &cluster_data_map, &uid2clustername_map, tag); 
  }
  //std::hash_map<int, std::vector<std::string> >::iterator iter = uid2clustername_map.begin();
  //for (; iter != uid2clustername_map.end(); ++iter) {
  //  LOG(INFO) << "uid:" << iter->first;
  //  BOOST_FOREACH(const std::string& s, iter->second) {
  //    LOG(INFO) << "\t" << s;
  //  }
  //}



  xce::ThreadPool thread_pool(20, 100);
  std::hash_map<int, std::vector<std::string> >::iterator iter = uid2clustername_map.begin();
//  while (iter != uid2clustername_map.end()) {
		while (iter != uid2clustername_map.end()) {
			MyUtil::IntSeq uid_set;
			for(int i=0; i<10 && iter != uid2clustername_map.end(); i++) {
				uid_set.push_back(iter->first);
				++iter;
			}
			xce::Message* msg = new xce::socialgraph::ComputeMessage(uid_set, &cluster_data_map, &uid2clustername_map);
			thread_pool.Post(msg);
		}
//  }
   
  sleep(3);
  thread_pool.Stop();
  thread_pool.Join();
  return 0;
}
  /*
  PTIME(pt, "main", true, 0);
	LOG(INFO) << "argc:" << argc;
	if (argc < 2) {
		LOG(INFO) << "uasge: ./ffucr_compute uid.txt";
		exit(-1);
	}

  std::ifstream in(argv[1]);
  if (!in) {
    LOG(LERROR) << "cannot open file:" << argv[1];
    exit(1);
  }

  std::string line;
  std::list<int> _uidseq;
  do {
    line.clear();
    std::getline(in, line);
    try {
      int uid = boost::lexical_cast<int>(line);
      _uidseq.push_back(uid);
    } catch (...) {
      continue;
    }
  } while(!line.empty());
  LOG(INFO) << "_uidseq.size():" << _uidseq.size();
  
	xce::ThreadPool threadPool(20, 60);
  BOOST_FOREACH(int id, _uidseq) {
    xce::Message* message = new FFUCRMessage(id);
    threadPool.Post(message);

  }
  sleep(3);
  threadPool.Stop();
  threadPool.Join();
  return 0;
  */
