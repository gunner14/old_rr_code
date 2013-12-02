#ifndef __SOCIALGRAPH_BUDDY_BY_ID_I_H_
#define __SOCIALGRAPH_BUDDY_BY_ID_I_H_

//功能：给SocialGraph组推荐产品线提供取好友ID列表功能
//开发人：熊君武
//邮件：junwu.xiong@renren-inc.com
//时间：2012-01-05

#include "OceCxxAdapter/src/BuddyByIdReplicaAdapter.h"
#include "socialgraph/socialgraphutil/redis_adapter.h"
#include "SocialGraphBuddyById.h"
#include "socialgraph/socialgraphutil/zkclient.h"
#include "UtilCxx/src/Singleton.h"
#include "base/once.h"
#include "LogWrapper.h"
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <string>
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

namespace xce {
namespace socialgraph {

using namespace MyUtil;
const static int kBuddy_BY_ID_CACHE_DATA = 0;
const static std::string kSG_BUDDY_ZKNODE = "/BUDDY";

const static std::string kSG_BUDDY_ZKTOKEN1 = ":";
const static std::string kSG_BUDDY_ZKTOKEN2 = "_";

//zookeeper代理：负责获取zookeeper通知
class BuddyZKController : public xce::Observer {
 public: 
  BuddyZKController();
  static BuddyZKController& GetInstance();
  virtual ~BuddyZKController();
  virtual void Update(const std::string&, const std::string&);
  std::string GetAdapter(const std::string& keyy);
  bool HasType(const std::string& key);
  void SetZNode(const std::string&key, const std::string& value);

 private:
  void Init();
  void UpdateTypeMap(const std::string& key, const std::string& value);

  IceUtil::RWRecMutex init_flag_mutex_;
  std::map<std::string, std::string> type_adapter_map_;
};

////存储取好友列表类定义
//class BuddyByIdData : virtual public Ice::Object {
// public:
//  BuddyByIdData();
//  BuddyByIdData(MyUtil::IntSeq&);
//  Ice::Int getFriendCount();
//  MyUtil::IntSeq getFriendList(Ice::Int, Ice::Int);
//
//  void AddFriend(Ice::Int); 
//  void RemoveFriend(Ice::Int);
// 
// private:
//  MyUtil::IntSeq friend_ids_; 
//};
//
//typedef IceUtil::Handle<BuddyByIdData> BuddyByIdDataPtr;

//存储好友列表的操作接口类定义
class SocialGraphBuddyByIdI : virtual public SocialGraphBuddyById, virtual public MyUtil::Singleton<SocialGraphBuddyByIdI> {
 public:
  Ice::Int getFriendCount(Ice::Int hostId);
  virtual Ice::Int getFriendCount(Ice::Int, const Ice::Current&);
  virtual MyUtil::Int2IntMap getFriendCounts(const MyUtil::IntSeq&, const Ice::Current&);
  
  MyUtil::IntSeq getFriendList(Ice::Int hostId, Ice::Int begin, Ice::Int limit);
  virtual MyUtil::IntSeq getFriendList(Ice::Int, Ice::Int, const Ice::Current&);
  virtual MyUtil::IntSeq getFriendListN(Ice::Int, Ice::Int, Ice::Int, const Ice::Current&);
  virtual MyUtil::Int2IntSeqMap getFriendLists(const MyUtil::IntSeq&, const Ice::Current&);
 
  //void AddFriend(Ice::Int hostId, Ice::Int guestId);  
  virtual void AddFriend(Ice::Int, Ice::Int, const Ice::Current&);
  void RemoveFriend(Ice::Int hostId, Ice::Int guestId);
  virtual void RemoveFriend(Ice::Int, Ice::Int, const Ice::Current&); 

 private: 
  friend class MyUtil::Singleton<SocialGraphBuddyByIdI>;
  SocialGraphBuddyByIdI() {}
  //BuddyByIdDataPtr getBuddyByIdData(Ice::Int hostId);
};

};
};
#endif
