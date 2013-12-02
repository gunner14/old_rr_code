
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


class ComputeMessage : public xce::Message {
	
 public:
  ComputeMessage(MyUtil::IntSeq* ur_set,
                  std::ofstream* file_out) : user_id_set_(ur_set), f_out_(file_out) {}
 
  virtual ~ComputeMessage() { }

  void Run() {
	  std::hash_map<int64_t, MyUtil::IntSeq > friend_map;
	  GetFriendList(user_id_set_, &friend_map);
	  if (!friend_map.empty()) {
	  	dumpFriendList(&friend_map, f_out_);
	  }
  }

  void GetFriendList(MyUtil::IntSeq* user_id_set, std::hash_map<int64_t, MyUtil::IntSeq >* friend_map) {

     try {
       MyUtil::Int2IntSeqMap friend_map_cache =
  				xce::buddy::adapter::BuddyByIdCacheAdapter::instance().getFriendLists(*user_id_set);

       MyUtil::Int2IntSeqMap::iterator ir_frimp = friend_map_cache.begin();
       for(; ir_frimp != friend_map_cache.end(); ++ir_frimp) {
         int64_t uid = ir_frimp->first;
         MyUtil::IntSeq fri_list = ir_frimp->second;
         friend_map->insert(std::make_pair(uid, fri_list));
       }
  	}catch (...) {
       LOG(LERROR) << "BuddyByIdCacheAdapter::instance().getFriendLists() exception ... "
                   << "user set size:" << user_id_set_.size();
  			BOOST_FOREACH(int64_t uid, user_id_set) {
  			MyUtil::IntSeq friend_seq;
  			try {
  				friend_seq = xce::buddy::adapter::BuddyByIdCacheAdapter::instance().getFriendListAsc(uid, 5000);
  				friend_map->insert(std::make_pair(uid, friend_seq));
  			} catch (...) {
  				LOG(LERROR) << "BuddyByIdCacheAdapter::instance().getFriendListAsc() exception ... " << "user_id_:" << uid;
  			}
  		}
  	 }
   }

  void dumpFriendList(std::hash_map<int64_t, MyUtil::IntSeq >* friend_map , std::ofstream* f_out) {
  	std::hash_map<int64_t, MyUtil::IntSeq >::iterator ir = friend_map->begin();
  	for(; ir != friend_map->end(); ++ir) {
  		int64_t uid = ir->first();
  		(*f_out) << uid << "\t";
  		MyUtil::IntSeq fri_seq = ir->second();
  		BOOST_FOREACH(MyUtil::Int fid, fri_seq) {
  			(*f_out) << fid << ",";
  		}
  		(*f_out) << std::endl;
  	}
  	f_out->close();
  }

 private:
  MyUtil::IntSeq* user_id_set_;
  std::ofstream* f_out_;
};

}}


int main(int argc, char **argv) {
  using namespace xce::socialgraph;
  PTIME(pt, "main", true, 0);
	LOG(INFO) << "argc:" << argc;
  if (argc < 3) {
    LOG(INFO) << "Usage: " << argv[0] << " idfile outfile";
    exit(0);
  }

  std::ifstream idfile_in(argv[1]);
  if (!idfile_in) {
	  LOG(LERROR) << "can't open file: " << argv[1] ;
  }


  int user_set_size = 1000;

  xce::ThreadPool thread_pool(20, 100);
  int index = 0;

  std::string line;
  std::getline(idfile_in, line);
  while(!line.empty()) {
	MyUtil::IntSeq user_seq;
	for (int i=0; i<user_set_size && !line.empty(); i++) {
		try {
			int64_t uid = boost::lexical_cast<int64_t> (line);
			user_seq.push_back(uid);
			std::getline(idfile_in, line);
		} catch (...) {
			std::getline(idfile_in, line);
			continue;
		}
	}

	std::ofstream friendlist_out(argv[2] + boost::lexical_cast<std::string> (index));
	xce::Message* msg = new xce::socialgraph::ComputeMessage(&user_seq, &friendlist_out);
	index++;
	thread_pool.Post(msg);
	std::getline(*idfile_in, line);
  }
   
  sleep(3);
  thread_pool.Stop();
  thread_pool.Join();
  return 0;
}
