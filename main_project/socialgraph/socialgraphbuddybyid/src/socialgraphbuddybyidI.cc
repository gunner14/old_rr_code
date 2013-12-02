#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include "socialgraphbuddybyidI.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "Monitor/client/service/ServiceMonitor.h"
#include <algorithm>

/**************************************************************************************************/
//redis initialization 
void MyUtil::initialize() {
  using namespace MyUtil;
  using namespace com::xiaonei::xce;
  using namespace xce::socialgraph;
  using namespace xce::serverstate;
 
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&SocialGraphBuddyByIdI::instance(), service.createIdentity("SBI", ""));

  //TaskManager::instance().config(LEVEL_ADD, ThreadPoolConfig(1,1));
  
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("SocialGraphBuddyById.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("SocialGraphBuddyById.Interval", 5);

  //register controller
  ServerStateSubscriber::instance().initialize("ControllerSocialGraphBuddyById", &SocialGraphBuddyByIdI::instance(), mod, interval, new MyUtil::SocialGraphChannel);
  ServerStateSubscriber::instance().setStatus(1);
  
  //register zookeeper
  //SocialGraphBuddyByIdI::instance().Register("BCM", "ControllerSocialGraphBuddyById");

  //redis configuration and initialization
  std::string ip = "10.3.16.85";
  int port = 6379;
  int timeout = 5000;
  int core_size = 10;
  int max_size = 100;
  RedisAdapter::instance().Init(ip, port, timeout, core_size, max_size);

  //start up monitor
  ServiceMonitorManager::instance().start();
}

/**************************************************************************************************/
namespace xce {
namespace socialgraph {

using namespace MyUtil;
//BuddyZKController class implementation
static BuddyZKController* zkc = NULL;
static void InitBuddyZKController() {
  zkc = new BuddyZKController();
}

void BuddyZKController::UpdateTypeMap(const std::string& key, const std::string& value) {
  MCE_INFO("[BuddyZKController] UpdateTypeMap key(" << key << ") value(" << value << ")");
  IceUtil::RWRecMutex::WLock lock(init_flag_mutex_);
  type_adapter_map_.clear();

  std::vector<std::string> raw_info;
  boost::algorithm::split(raw_info, value, boost::algorithm::is_any_of(kSG_BUDDY_ZKTOKEN1));
  if(!raw_info.empty()) {
    for(std::vector<std::string>::iterator it = raw_info.begin(); it != raw_info.end(); ++it) { 
      std::vector<std::string> info;
      boost::algorithm::split(info, *it, boost::algorithm::is_any_of(kSG_BUDDY_ZKTOKEN2));
      if(2 == (int)info.size()) { 
        type_adapter_map_.insert(make_pair<std::string, std::string>(info[0], info[1])); 
      }
    }
  }

  for(std::map<std::string, std::string>::iterator it = type_adapter_map_.begin(); 
      it != type_adapter_map_.end(); ++it ) {
    MCE_INFO("key : " << it->first << "value : " << it->second);
  }
}

void BuddyZKController::Init() {
  MCE_INFO("[BuddyZKConrtoller] Init");
  //static XCE_DECLARE_ONCE(once_guard_);
  std::string value;
  int limit = 10;
  while(!xce::ZooConfig::GetInstance().Get(kSG_BUDDY_ZKNODE, &value)) {
    MCE_FATAL("Not get kSG_BUDDY_ZKNODE value! limit(" << limit << ")");
    if(--limit <= 0) {
      break;
    }
  }

  if(!value.empty()) {
    UpdateTypeMap(kSG_BUDDY_ZKNODE, value);
  }
}

BuddyZKController& BuddyZKController::GetInstance() {
  MCE_INFO("[BuddyZKController] GetInstance");
  static XCE_DECLARE_ONCE(once_guard_);
  XceOnceInit(&once_guard_, &InitBuddyZKController);
  return *zkc;
}

BuddyZKController::BuddyZKController() {
  MCE_INFO("[BuddyZKController] BuddyZKController");
  //static XCE_DECLARE_ONCE(once_guard_);
  xce::ZooConfig::GetInstance().Attach(this);
  Init();
}

BuddyZKController::~BuddyZKController() {
  xce::ZooConfig::GetInstance().Detach(this);
}

void BuddyZKController::Update(const std::string& key, const std::string& value) {
  MCE_INFO("[BuddyZKController] Update key(" << key << ") value(" << value << ")");
  if(kSG_BUDDY_ZKNODE == key) {
    UpdateTypeMap(key, value);
  }
}

std::string BuddyZKController::GetAdapter(const std::string& key) {
  IceUtil::RWRecMutex::RLock lock(init_flag_mutex_);
  std::map<std::string, std::string>::iterator it = type_adapter_map_.find(key);
  if(it != type_adapter_map_.end()) {
    return it->second;
  }  

  std::string value="";
  return value;  
}

bool BuddyZKController::HasType(const std::string& key) {
  MCE_INFO("[BuddyZKController] HasType key(" << key << ")");
  IceUtil::RWRecMutex::RLock lock(init_flag_mutex_);
  for(std::map<std::string, std::string>::iterator it = type_adapter_map_.begin();
      it != type_adapter_map_.end(); ++it) {
    MCE_INFO("map key(" << it->first << ") value(" << it->second << ")");
  }
  
  std::map<std::string, std::string>::iterator it = type_adapter_map_.find(key);
  if(it != type_adapter_map_.end()) {
    return true;
  } 
  return false;  
}

void BuddyZKController::SetZNode(const std::string& key, const std::string& value) {
  MCE_INFO("[BuddyZKController] SetZNode key(" << key << ") value(" << value << ")");  
  if(HasType(key)) {
    return;
  }

  std::string content = "";
  if(type_adapter_map_.empty()) {
    MCE_INFO("[BuddyZKController] SetZNode type_adapter_map_ empty!");
    content = key + kSG_BUDDY_ZKTOKEN2 + value;
  } else {
    MCE_INFO("[BuddyZKController] SetZNode type_adapter_map_ not empty!");
    xce::ZooConfig::GetInstance().Get(kSG_BUDDY_ZKNODE, &content);
    content += kSG_BUDDY_ZKTOKEN1 + key + kSG_BUDDY_ZKTOKEN2 + value;
  }
  MCE_INFO("kSG_BUDDY_ZKNODE: " << kSG_BUDDY_ZKNODE << " content: " << content);
  if(!content.empty()) {
    xce::ZooConfig::GetInstance().Set(kSG_BUDDY_ZKNODE, &content);
  }
}

/**************************************************************************************************/
//BuddyByIdData class implementation
//BuddyByIdData::BuddyByIdData() {
//}
//
//Ice::Int BuddyByIdData::getFriendCount() {
//  return friend_ids_.size();
//}

//MyUtil::IntSeq BuddyByIdData::getFriendList(Ice::Int begin, Ice::Int limit) {
//  if(limit < 0) {
//    return friend_ids_;
//  }
//  
//  if( ((unsigned int)begin >= friend_ids_.size()) || begin < 0) {
//    return MyUtil::IntSeq();
//  }  
//
//  if( (unsigned int)(begin + limit) >= friend_ids_.size()) {
//    return MyUtil::IntSeq(friend_ids_.begin() + begin, friend_ids_.end());
//  } else {
//    return MyUtil::IntSeq(friend_ids_.begin() + begin, friend_ids_.begin() + begin + limit);
//  }
//}

//void BuddyByIdData::AddFriend(Ice::Int) {
////todo: add if logic judgement
//   
//}
//
//void BuddyByIdData::RemoveFriend(Ice::Int guestId) {
//  MyUtil::IntSeq::iterator it = lower_bound(friend_ids_.begin(), friend_ids_.end(), guestId);
//  if((it != friend_ids_.end()) && (*it == guestId)) {
//    friend_ids_.erase(it);
//  }
//}

/**************************************************************************************************/
//SocialGraphBuddyByIdI class implementation
//BuddyByIdDataPtr SocialGraphBuddyByIdI::getBuddyByIdData(Ice::Int hostId) {
//  return new BuddyByIdData();
//}

Ice::Int SocialGraphBuddyByIdI::getFriendCount(Ice::Int hostId) {
  MCE_INFO("SocialGraphBuddyById::getFriendCount]" << hostId << " from RedisAdapter");
  int frd_count = RedisAdapter::instance().GetListSize(boost::lexical_cast<std::string>(hostId));
  frd_count = -1;
  if(frd_count == -1) {
    MCE_INFO("SocialGraphBuddyById::getFriendCount]" << hostId << " from BuddyByIdCacheAdapter");
    frd_count = xce::buddy::adapter::BuddyByIdCacheAdapter::instance().getFriendCount(hostId);
  }  
  return frd_count;
}

Ice::Int SocialGraphBuddyByIdI::getFriendCount(Ice::Int hostId, const Ice::Current& current) {
  MCE_INFO("[SocialGraphBuddyById::getFriendCount] " << "hostId: " << hostId);
  Ice::Int frd_count = getFriendCount(hostId);
  return frd_count;
}

MyUtil::Int2IntMap SocialGraphBuddyByIdI::getFriendCounts(const MyUtil::IntSeq& hostIds,
    const Ice::Current& current) {
  MCE_INFO("[SocialGraphBuddyById::getFriendCounts]");
  Int2IntMap hostId_frdCount_map;
  for(MyUtil::IntSeq::const_iterator it = hostIds.begin(); it != hostIds.end(); ++it) {
    hostId_frdCount_map.insert( make_pair(*it, getFriendCount(*it)) );
  }
  return hostId_frdCount_map;
}

MyUtil::IntSeq SocialGraphBuddyByIdI::getFriendList(Ice::Int hostId, Ice::Int begin, Ice::Int limit) {
  std::vector<string> str_ids;
  std::vector<int> vec_ids;
  MCE_INFO("SocialGraphBuddyById::getFriendList]" << hostId << " from RedisAdapter");
  RedisAdapter::instance().GetList(boost::lexical_cast<std::string>(hostId), str_ids, begin, limit); 
  if(str_ids.empty()) {
    try {
      MCE_INFO("[SocialGraphBuddyById::getFriendList] hostId: " << hostId << " from BuddyByIdCacheAdapter");
      ( xce::buddy::adapter::BuddyByIdCacheAdapter::instance().getFriendListAsc(hostId, -1) ).swap(vec_ids);
      if(!vec_ids.empty()) {
        MCE_INFO("[SocialGraphBuddyById::getFriendList] hostId: " << hostId << " set friends list to redis");
        str_ids.reserve(vec_ids.size());
        BOOST_FOREACH(int i, vec_ids) {
          str_ids.push_back(boost::lexical_cast<std::string>(i));
        }
        RedisAdapter::instance().SetList(boost::lexical_cast<std::string>(hostId), str_ids, 7*24*3600);
      }
    } catch (Ice::TimeoutException e) {
      MCE_WARN("[SocialGraphBuddyById::LoadFriendList] " << " hostId: " << hostId << " found Ice::TimeOutException: " << e.what());
    } catch (Ice::Exception e) {
      MCE_WARN("[SocialGraphBuddyById::LoadFriendList] " << " hostId: " << hostId << " found Ice::Exception: " << e.what());
    } catch (...) {
      MCE_WARN("[SocialGraphBuddyById::LoadFriendList] " << " hostId: " << hostId << " found unknown exception: ");
    }
  } else { 
    vec_ids.reserve(str_ids.size());
    BOOST_FOREACH(std::string s, str_ids) {
      vec_ids.push_back(boost::lexical_cast<int>(s));
    } 
  }
  
  if(limit < 0) {
    return vec_ids;
  }
  if( ((unsigned int)begin >= vec_ids.size()) || begin < 0) {
    return MyUtil::IntSeq();
  }
  if( (unsigned int)(begin + limit) >= vec_ids.size()) {
    return MyUtil::IntSeq(vec_ids.begin() + begin, vec_ids.end());
  } else {
    return MyUtil::IntSeq(vec_ids.begin() + begin, vec_ids.begin() + begin + limit);
  }
}

MyUtil::IntSeq SocialGraphBuddyByIdI::getFriendList(Ice::Int hostId, Ice::Int limit,
    const Ice::Current& current) {
  MCE_INFO("[SocialGraphBuddyById::getFriendList] " << "hostId: " << hostId << " limit: " << limit);
  MyUtil::IntSeq friend_list = getFriendList(hostId, 0, limit);
  return friend_list;
}

MyUtil::IntSeq SocialGraphBuddyByIdI::getFriendListN(Ice::Int hostId, Ice::Int begin, Ice::Int limit, 
    const Ice::Current& current) {
  MCE_INFO("[SocialGraphBuddyById::getFriendListN] " << "hostId: " << hostId << " begin: " << begin << " limit: " << limit);
  MyUtil::IntSeq friend_list = getFriendList(hostId, begin, limit);
  return friend_list;
}

MyUtil::Int2IntSeqMap SocialGraphBuddyByIdI::getFriendLists(const MyUtil::IntSeq& hostIds, 
    const Ice::Current& current) {
  MCE_INFO("[SocialGraphBuddyById::getFriendLists]");
  Int2IntSeqMap hostId_frdList_map;
  for(MyUtil::IntSeq::const_iterator it = hostIds.begin(); it != hostIds.end(); ++it) {  
    hostId_frdList_map.insert( make_pair( *it, getFriendList(*it, 0, -1) ) );
  }
  return hostId_frdList_map;
}


//void SocialGraphBuddyByIdI::AddFriend(Ice::Int hostId, Ice::Int guestId) {
////todo: add if logic judgement
//}

void SocialGraphBuddyByIdI::RemoveFriend(Ice::Int hostId, Ice::Int guestId) {
  MCE_INFO("[SocialGraphBuddyById::RemoveFriend] " << "hostId: " << hostId << " guestId: " << guestId << " from redis");
  RedisAdapter::instance().Remove(boost::lexical_cast<std::string>(hostId)); 
}

void SocialGraphBuddyByIdI::AddFriend(Ice::Int hostId, Ice::Int guestId, const Ice::Current& current) {
  MCE_INFO("[SocialGraphBuddyById::AddFriend] " << "hostId: " << hostId << " guestId: " << guestId);
  RemoveFriend(hostId, guestId);
}

void SocialGraphBuddyByIdI::RemoveFriend(Ice::Int hostId, Ice::Int guestId, const Ice::Current& current) {
  MCE_INFO("[SocialGraphBuddyById::RemoveFriend] " << "hostId: " << hostId << " guestId: " << guestId);
  RemoveFriend(hostId, guestId);
}

};
};
