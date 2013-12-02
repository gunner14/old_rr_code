#include "follow_relation_cache_impl.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "BuddyByIdReplicaAdapter.h"
#include <algorithm>
#include "util/cpp/TimeCost.h"
#include "Monitor/client/service/ServiceMonitor.h"
#include "util/cpp/InvokeMonitor.h"

#define FRIEND_SIZE                   10000

using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace xce::serverstate;
using namespace xce::buddy;
using namespace xce::buddy::adapter;

// TODO  分批从DB加载  DONE
// TODO  代码结构优化
// TODO  完善LOG
// TODO  进一步测试


int main() {
  return 0;
}

void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();

  // 注册RelationCacheData的ObjectCache
  service.registerObjectCache(RelationCacheDataCategory, "RC", new SyncServantFactory(new RelationCacheDataFactory));

  service.registerObjectCache(CommonFriendCacheDataCategory, "CFC", new SyncServantFactory(
      new CommonFriendCacheDataFactory));

  service.getAdapter()->add(&FollowRelationCacheImpl::instance(), service.createIdentity("CFCID", ""));

  int mod = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FollowRelationCache.Mod", 0);
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault(
      "FollowRelationCache.Interval", 5);

  //----------------------------Monitor-----------------------------------------
  MyUtil::ServiceMonitorManager::instance().start();

  TaskManager::instance().config(TASK_LEVEL_GID_LOAD, ThreadPoolConfig(1, 100));
  TaskManager::instance().config(TASK_LEVEL_GID_RELOAD, ThreadPoolConfig(1, 100));

  service.registerXceObserver(new ServiceSchemaI);
  // register to Controller Service
  ServerStateSubscriber::instance().initialize("ControllerFollowRelationCache", &FollowRelationCacheImpl::instance(),
      mod, interval);

  MCE_DEBUG("VERTION: 2012-11-14 03:05");
}

//=========================================================

namespace xce {
namespace buddy {

// 根据BuddyByIdCache 的UID好友数据 和 本地Cache中的GID关注者数据计算共同关注好友数据
IdSeqDataPtr FollowRelationCacheImpl::ComputeCommonFriends(int userId, long globalId) {
  std::ostringstream oss;
  oss << __FUNCTION_NAME__ << "userId:" << userId << " globalId:" << globalId;
  Ice::Current current;
  InvokeClient ic = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

  IdSeqDataPtr commonFriendsData;

  // ====获取GID的关注者列表====
  RelationCacheEntrySeq userEntrySeq;

  userEntrySeq = syncLocateObject<RelationCacheDataPtr> (RelationCacheDataCategory, globalId)->getEntrySeq();

  ic.step("GetGidFollowers");

  if (userEntrySeq.size() == 0) {
    MyUtil::IntSeq seq;
    return new IdSeqData(seq);
  }

  // ====获取UID的好友列表====
  MyUtil::IntSeq friendIds;

  // 生成临时数据供测试 ----------
  //  for (int i = 0; i < FRIEND_SIZE; i += 1) {
  //    friendIds.push_back(i);
  //  }

  // 线上服务需要调用ByIdCache  可以一次取9000 如果好友数超过9000？ ----------
  try {
    friendIds = BuddyByIdCacheAdapter::instance().getFriendListAsc(userId, ID_LIST_LIMIT);
  } catch (std::exception& e) {
    MCE_WARN("Exception! BuddyByIdCacheAdapter::instance().getFriendListAsc userId:" << userId << " limit:"
        << ID_LIST_LIMIT << e.what());
  }

  ic.step("GetUidFriends");

  if (friendIds.size() == 0) {
    MyUtil::IntSeq seq;
    return new IdSeqData(seq);
  }

  MCE_DEBUG(__FUNCTION_NAME__ << "friendIds.size()=" << friendIds.size() << "," << "entrySeq.size()="
      << userEntrySeq.size());

  // ====计算共同好友====   ID需要按升序排列
  RelationCacheEntrySeq tmpSeq(friendIds.size());

  RelationCacheEntrySeq friendEntrySeq;
  for (int i = 0; i < friendIds.size(); ++i) {
    friendEntrySeq.push_back(RelationCacheEntry(friendIds.at(i), 0));
  }

  RelationCacheEntrySeq::iterator end = set_intersection(userEntrySeq.begin(), userEntrySeq.end(),
      friendEntrySeq.begin(), friendEntrySeq.end(), tmpSeq.begin(), compareUserId);

  MCE_DEBUG(__FUNCTION_NAME__ << "tmpSeq.size=" << tmpSeq.size() << ", capacity=" << tmpSeq.capacity());

  // vector 的resize只会改变size值，不会改变实际占用的内存
  tmpSeq.resize(end - tmpSeq.begin());

  MCE_DEBUG(__FUNCTION_NAME__ << "After resize: tmpSeq.size=" << tmpSeq.size() << ", capacity=" << tmpSeq.capacity());

  RelationCacheEntrySeq commonFriendSeq;

  // 使用一个新的vector来存储,节省空间  TODO 优化速度
  for (RelationCacheEntrySeq::iterator it = tmpSeq.begin(); it != tmpSeq.end(); ++it) {
    commonFriendSeq.push_back(*it);
  }

  MCE_DEBUG(__FUNCTION_NAME__ << "CommonFriendSeq.size=" << commonFriendSeq.size() << ", capacity="
      << commonFriendSeq.capacity());

  ic.step("ComputeCommonFriends");

  sort(commonFriendSeq.begin(), commonFriendSeq.end());

  ic.step("SortByTime");

  commonFriendsData = new IdSeqData(commonFriendSeq);

  MCE_INFO(__FUNCTION_NAME__ << "Return commonFriendsData." << " commonFriendsData->getCount()="
      << commonFriendsData->getCount() << " commonFriendsData->getTimestamp()=" << commonFriendsData->getTimestamp());

  return commonFriendsData;
}

// 取出与 uid 共同关注 gid 的好友数据
IdSeqDataPtr FollowRelationCacheImpl::getCommonFriendData(Ice::Int uid, const MyUtil::GlobalIdPtr& gid) {
  Ice::Long globalId = GlobalIdTransfer::get(gid);

  ostringstream msg;
  msg << " uid:" << uid << " gid:" << globalId;
  Ice::Current current;
  InvokeClient ic = InvokeClient::create(current, msg.str(), InvokeClient::INVOKE_LEVEL_INFO);

  // 通过gid的type和uid构造对应一个用户的共同好友数据的globalUserId
  GlobalIdPtr guid = new GlobalId(gid->type, uid);
  Ice::Long globalUserId = GlobalIdTransfer::get(guid);

  IdSeqDataPtr commonFriendsData;

  // locate 方法从Cache中取一个用户的共同好友数据，miss时自动创建新的对象
  CommonFriendCacheDataPtr object = syncLocateObject<CommonFriendCacheDataPtr> (CommonFriendCacheDataCategory,
      globalUserId);

  if (!object) {
    MCE_DEBUG(__FUNCTION_NAME__ << "object is null" << " uid:" << uid << " gid:" << globalId);
    return commonFriendsData;
  }
  // 在该用户的共同好友数据中获取对应gid的共同好友数据,如果没有或超时，则重新计算并存入Cache
  // ?? Map的传递方式?传值?
  // 注意：Map中存的是指针IdSeqDataPtr
  commonFriendsData = object->getIdSeqData(globalId);

  ic.step("GetCommonFriendDataFromCache");

  // 判断是否需要重新计算共同好友
  if (commonFriendsData) {
    // 找到对应gid的共同好友列表数据
    // 判断数据是否超时
    time_t now = time(0);
    time_t timestamp = commonFriendsData->getTimestamp();
    if (now - timestamp > OUT_OF_DATE_TIME) {
      // 注意：此处需要进行同步，避免多线程进行多次重新计算
      // 使用commonFriendsData进行加锁，可以使锁的粒度更小（针对每个uid-gid对加锁）
      IceUtil::Mutex::Lock lock(*commonFriendsData);
      MCE_DEBUG(__FUNCTION_NAME__ << "Get the lock: commonFriendData:" << commonFriendsData);
      // 注意：此处必须重新取Cache中的数据，以判断是否之前的线程已经进行过重新计算
      // 每个线程中有自己独立的变量，如果不重新取，commonFriendsData仍然指向之前的超时数据
      IdSeqDataPtr newCommonFriendsData = object->getIdSeqData(globalId);

      MCE_DEBUG(__FUNCTION_NAME__ << "Check again: newCommonFriendsData:" << newCommonFriendsData);

      if (newCommonFriendsData == commonFriendsData) {
        // 超时，重新计算共同好友
        MCE_DEBUG(__FUNCTION_NAME__ << "Common friend data out of date. Recompute data. uid:" << uid << " gid:"
            << globalId << " exist time:" << now - timestamp);
        newCommonFriendsData = ComputeCommonFriends(uid, globalId);
        object->setIdSeqData(globalId, newCommonFriendsData);
      } else {
        MCE_DEBUG(__FUNCTION_NAME__ << "Common friend data has been recomputed.");
      }
      return newCommonFriendsData;
    }

    return commonFriendsData;

  } else {
    //  没有对应gid的共同好友列表数据,则重新计算
    commonFriendsData = ComputeCommonFriends(uid, globalId);
    object->setIdSeqData(globalId, commonFriendsData);
    return commonFriendsData;
  }
}

//获取共同好友列表
MyUtil::IntSeq FollowRelationCacheImpl::getCommon(Ice::Int uid, const MyUtil::GlobalIdPtr& gid, Ice::Int begin,
    Ice::Int limit, const Ice::Current& current) {
  Ice::Long globalId = GlobalIdTransfer::get(gid);
  ostringstream msg;
  msg << " uid:" << uid << " gid:" << globalId << " begin:" << begin << " limit:" << limit;
  InvokeClient ic = InvokeClient::create(current, msg.str(), InvokeClient::INVOKE_LEVEL_INFO);

  // 参数合法性检查
  MyUtil::IntSeq seq;
  if ((!gid) || globalId < 0 || (uid < 0))
    return seq;

  if (!CheckParam(begin, limit)) {
    MCE_WARN(__FUNCTION_NAME__ << "Invalid params. " << "begin:" << begin << "limit:" << limit);
    return seq;
  }

  try {
    // 获取共同好友数据
    IdSeqDataPtr commonFriendsData = getCommonFriendData(uid, gid);
    seq = commonFriendsData->get(begin, limit);
  } catch (std::exception& e) {
    MCE_ERROR(__FUNCTION_NAME__ << e.what());
  }

  MCE_INFO(__FUNCTION_NAME__ << "Return seq.size():" << seq.size());
  return seq;
}

MyUtil::IntSeq FollowRelationCacheImpl::getCommonAfterTime(Ice::Int uid, const MyUtil::GlobalIdPtr& gid,
    const long time, Ice::Int begin, Ice::Int limit, const Ice::Current& current) {
  Ice::Long globalId = GlobalIdTransfer::get(gid);
  ostringstream msg;
  msg << " uid:" << uid << " gid:" << globalId << " time:" << time << " begin:" << begin << " limit:" << limit;
  InvokeClient ic = InvokeClient::create(current, msg.str(), InvokeClient::INVOKE_LEVEL_INFO);

  // 参数合法性检查
  MyUtil::IntSeq seq;
  if ((!gid) || globalId < 0 || (uid < 0))
    return seq;

  if (!CheckParam(begin, limit)) {
    MCE_WARN(__FUNCTION_NAME__ << "Invalid params. " << "begin:" << begin << "limit:" << limit);
    return seq;
  }

  ic.step("CheckParams");

  try {
    // 获取共同好友数据
    IdSeqDataPtr commonFriendsData = getCommonFriendData(uid, gid);
    ic.step("GetCommonFriendData");

    seq = commonFriendsData->getAfterTime(time, begin, limit);
    ic.step("GetAfterTime");

  } catch (std::exception& e) {
    MCE_ERROR(__FUNCTION_NAME__ << e.what());
  }

  MCE_INFO(__FUNCTION_NAME__ << "Return seq.size():" << seq.size());
  return seq;
}

int FollowRelationCacheImpl::getCommonCount(Ice::Int uid, const MyUtil::GlobalIdPtr& gid, const Ice::Current& current) {
  long globalId = GlobalIdTransfer::get(gid);
  ostringstream msg;
  msg << "uid:" << uid << " gid:" << globalId;
  InvokeClient ic = InvokeClient::create(current, msg.str(), InvokeClient::INVOKE_LEVEL_INFO);

  if ((!gid) || globalId < 0 || (uid < 0))
    return 0;

  int count = 0;

  try {
    // 获取共同好友数据
    IdSeqDataPtr commonFriendsData = getCommonFriendData(uid, gid);
    count = commonFriendsData->getCount();
  } catch (std::exception& e) {
    MCE_ERROR(__FUNCTION_NAME__ << e.what());
  }

  MCE_INFO(__FUNCTION_NAME__ << "Return count:" << count);
  return count;
}

int FollowRelationCacheImpl::getCommonCountAfterTime(Ice::Int uid, const MyUtil::GlobalIdPtr& gid, const long time,
    const Ice::Current& current) {
  long globalId = GlobalIdTransfer::get(gid);
  ostringstream msg;
  msg << "uid:" << uid << " gid:" << globalId << " time:" << time;
  InvokeClient ic = InvokeClient::create(current, msg.str(), InvokeClient::INVOKE_LEVEL_INFO);

  if ((!gid) || globalId < 0 || (uid < 0))
    return 0;

  int count = 0;

  try {
    // 获取共同好友数据
    IdSeqDataPtr commonFriendsData = getCommonFriendData(uid, gid);
    count = commonFriendsData->getCountAfterTime(time);
  } catch (std::exception& e) {
    MCE_ERROR(__FUNCTION_NAME__ << e.what());
  }

  MCE_INFO(__FUNCTION_NAME__ << "Return count:" << count);
  return count;
}

bool FollowRelationCacheImpl::hasRelation(Ice::Int uid, const MyUtil::GlobalIdPtr& gid, const Ice::Current& current) {
  long globalId = GlobalIdTransfer::get(gid);
  ostringstream msg;
  msg << "uid:" << uid << " gid:" << globalId;
  InvokeClient ic = InvokeClient::create(current, msg.str(), InvokeClient::INVOKE_LEVEL_INFO);

  if ((!gid) || globalId < 0 || (uid < 0))
    return false;

  bool hasRelation = false;

  hasRelation = syncLocateObject<RelationCacheDataPtr> (RelationCacheDataCategory, globalId)->find(uid);

  MCE_INFO(__FUNCTION_NAME__ << "Return hasRelation:" << hasRelation);
  return hasRelation;
}

void FollowRelationCacheImpl::reload(const long globalId, const Ice::Current& current) {
  ostringstream msg;
  msg << "gid:" << globalId;
  InvokeClient ic = InvokeClient::create(current, msg.str(), InvokeClient::INVOKE_LEVEL_INFO);

  if (globalId < 0)
    return;

  // reload 与locate 的区别？
  TaskManager::instance().execute(new ReloadTask(globalId));
}

int FollowRelationCacheImpl::getCount(const MyUtil::GlobalIdPtr& gid, const Ice::Current& current) {
  long globalId = GlobalIdTransfer::get(gid);
  ostringstream msg;
  msg << " gid:" << globalId;
  InvokeClient ic = InvokeClient::create(current, msg.str(), InvokeClient::INVOKE_LEVEL_INFO);

  if (!gid || globalId < 0)
    return 0;

  int count = 0;

  count = syncLocateObject<RelationCacheDataPtr> (RelationCacheDataCategory, globalId)->getCount();

  MCE_INFO(__FUNCTION_NAME__ << "Return count:" << count);

  return count;
}

MyUtil::IntSeq FollowRelationCacheImpl::getList(const MyUtil::GlobalIdPtr& gid, Ice::Int begin, Ice::Int limit,
    const Ice::Current& current) {
  long globalId = GlobalIdTransfer::get(gid);
  ostringstream msg;
  msg << " gid:" << globalId << " begin:" << begin << " limit:" << limit;
  InvokeClient ic = InvokeClient::create(current, msg.str(), InvokeClient::INVOKE_LEVEL_INFO);

  MyUtil::IntSeq seq;

  if (!gid || globalId < 0)
    return seq;

  if (!CheckParam(begin, limit))
    return seq;

  seq = syncLocateObject<RelationCacheDataPtr> (RelationCacheDataCategory, globalId)->getIdSeq(begin, limit);

  MCE_INFO(__FUNCTION_NAME__ << "Return seq.size():" << seq.size());
  return seq;
}

void ServiceSchemaI::configure(const Ice::PropertiesPtr& props) {
  TaskManager::instance().config(TASK_LEVEL_GID_LOAD, ThreadPoolConfig(1, 5));
  TaskManager::instance().config(TASK_LEVEL_GID_RELOAD, ThreadPoolConfig(1, 5));
}

void ReloadTask::handle() {
  ServiceI::instance().reloadObject(RelationCacheDataCategory, _gid);
}

}
} // end xce::buddy

