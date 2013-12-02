#ifndef _FOLLOW_RELATION_CAHCE_I_H_
#define _FOLLOW_RELATION_CAHCE_I_H_
#include "FollowRelation.h"
#include <boost/thread/shared_mutex.hpp>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include "IceExt/src/ObjectCacheI.h"
#include "IceExt/src/ServiceI.h"
#include <ConnectionPoolManager.h>
#include <QueryRunner.h>
#include "UtilCxx/src/Singleton.h"
#include "UtilCxx/src/GlobalId.h"
#include "base/threadpool.h"
#include "base/rwlock.h"
#include "TempMutex.h"
#include <map>

#define DB_LIMIT_SIZE  1000  // 从DB分批加载时每次加载的量
#define OUT_OF_DATE_TIME      300         //IdSeqData超时时间（秒）
#define ID_LIST_LIMIT         9000        //批量取ID的limit参数
// DEBUG
//#define RELATION_CACHE_DATA_MAX_SIZE  1000

#define __FUNCTION_NAME__     "[" << __FUNCTION__ << ":] "

namespace xce {
namespace buddy {
const int CommonFriendCacheDataCategory = 0;
const int RelationCacheDataCategory = 1;

const int TASK_LEVEL_GID_LOAD = 0;
const int TASK_LEVEL_GID_RELOAD = 1;

class RelationCacheEntry: virtual public Ice::Object {
public:
  RelationCacheEntry(int userId, long followTime) :
    userId_(userId), followTime_(followTime) {
  }

  // 用于vector按指定大小初始化
  RelationCacheEntry() {

  }

  int getUserId() const {
    return userId_;
  }

  long getFollowTime() const {
    return followTime_;
  }

  // 重载等于比较符，用于set_intersection函数
  bool operator ==(const int userId) const {
    return userId_ == userId;

  }
  // 重载等于比较符
  bool operator ==(const RelationCacheEntry& entry) const {
    if ((userId_ == entry.getUserId()) && (followTime_ == entry.getFollowTime()))
      return true;
    else
      return false;
  }

  // 重载小于比较符，用于sort函数. 按关注时间进行比较
  bool operator <(const RelationCacheEntry& entry) const {
    // followTime值越大，标识其关注时间越短，因此followTime大应该排在前面
    if (followTime_ > entry.getFollowTime())
      return true;
    // followTime相等时，按用户ID从小到大排序
    else if (followTime_ == entry.getFollowTime() && userId_ < entry.getUserId())
      return true;
    else
      return false;
  }

private:
  int userId_;
  long followTime_;
};

// 比较函数,只按用户ID进行比较
bool compareUserId(const RelationCacheEntry& entry1, const RelationCacheEntry& entry2) {
  return entry1.getUserId() < entry2.getUserId();
}

typedef IceUtil::Handle<RelationCacheEntry> RelationCacheEntryPtr;
typedef vector<RelationCacheEntry> RelationCacheEntrySeq;

class RelationCacheEntrySeqData: virtual public Ice::Object {
public:
  RelationCacheEntrySeqData(RelationCacheEntrySeq& entrySeq) {
    entrySeq_.swap(entrySeq);
  }

  RelationCacheEntrySeqData() {
    ;
  }

  MyUtil::IntSeq getIdSeq(int begin, int limit) {
    MyUtil::IntSeq idSeq;

    if (begin < 0 || begin >= entrySeq_.size()) {
      return idSeq;
    }

    RelationCacheEntrySeq::iterator it_begin = entrySeq_.begin() + begin;

    RelationCacheEntrySeq::iterator it_end;
    if (begin + limit >= entrySeq_.size()) {
      it_end = entrySeq_.end();
    } else {
      it_end = it_begin + limit;
    }

    for (RelationCacheEntrySeq::iterator it = it_begin; it != it_end; ++it) {
      idSeq.push_back(it->getUserId());
    }

    return idSeq;
  }

  MyUtil::IntSeq getIdSeqAfterTime(long time, int begin, int limit) {
    MyUtil::IntSeq idSeq;
    MyUtil::IntSeq tmpSeq;

    for (RelationCacheEntrySeq::iterator iter = entrySeq_.begin(); iter != entrySeq_.end(); ++iter) {
      if (iter->getFollowTime() >= time) {
        tmpSeq.push_back(iter->getUserId());
      }
    }

    if (begin < 0 || begin >= tmpSeq.size()) {
      return idSeq;
    }

    MyUtil::IntSeq::iterator it_begin = tmpSeq.begin() + begin;

    MyUtil::IntSeq::iterator it_end;
    if (begin + limit >= tmpSeq.size()) {
      it_end = tmpSeq.end();
    } else {
      it_end = it_begin + limit;
    }

    for (MyUtil::IntSeq::iterator it = it_begin; it != it_end; ++it) {
      idSeq.push_back(*it);
    }

    return idSeq;
  }

  int getCount() {
    return entrySeq_.size();
  }

  int getCountAfterTime(long time) {
    int count = 0;

    for (RelationCacheEntrySeq::iterator iter = entrySeq_.begin(); iter != entrySeq_.end(); ++iter) {
      if (iter->getFollowTime() >= time) {
        ++count;
      }
    }

    return count;
  }

  RelationCacheEntrySeq getEntrySeq() {
    return entrySeq_;
  }

  bool find(int id) {
    RelationCacheEntry userIdEntry(id, 0);
    RelationCacheEntrySeq::iterator it = lower_bound(entrySeq_.begin(), entrySeq_.end(), userIdEntry, compareUserId);

    if (it != entrySeq_.end() && (it->getUserId() == id)) {
      return true;
    } else {
      return false;
    }
  }

private:
  RelationCacheEntrySeq entrySeq_;
};

// 保存一个资源的粉丝列表的数据结构，包含关注时间信息。按userId升序排列
class RelationCacheData: virtual public Ice::Object, virtual public IceUtil::Mutex {
public:
  RelationCacheData(RelationCacheEntrySeq& entrySeq) :
    entrySeqData_(entrySeq) {
  }

  MyUtil::IntSeq getIdSeq(int begin, int limit) {
    IceUtil::Mutex::Lock lock(*this);
    return entrySeqData_.getIdSeq(begin, limit);
  }

  int getCount() {
    IceUtil::Mutex::Lock lock(*this);
    return entrySeqData_.getCount();
  }

  RelationCacheEntrySeq getEntrySeq() {
    IceUtil::Mutex::Lock lock(*this);
    return entrySeqData_.getEntrySeq();
  }

  bool find(int id) {
    IceUtil::Mutex::Lock lock(*this);
    return entrySeqData_.find(id);
  }

private:
  RelationCacheEntrySeqData entrySeqData_;
};

typedef IceUtil::Handle<RelationCacheData> RelationCacheDataPtr;

// 保存共同好友ID列表的数据结构
class IdSeqData: virtual public Ice::Object, virtual public IceUtil::Mutex {
public:

  IdSeqData(MyUtil::IntSeq& ids) {
    ;
  }

  IdSeqData(RelationCacheEntrySeq& entrySeq) :
    entrySeqData_(entrySeq) {
    _timestamp = time(0);
  }

  virtual ~IdSeqData() {
    ;
  }

  //从指定位置获取指定长度的ID列表
  MyUtil::IntSeq get(int begin, int limit) {
    IceUtil::Mutex::Lock lock(*this);
    return entrySeqData_.getIdSeq(begin, limit);

  }

  MyUtil::IntSeq getAfterTime(long time, int begin, int limit) {
    IceUtil::Mutex::Lock lock(*this);
    return entrySeqData_.getIdSeqAfterTime(time, begin, limit);
  }

  //查找列表中是否存在指定ID
  bool find(int id) {
    IceUtil::Mutex::Lock lock(*this);
    return entrySeqData_.find(id);
  }

  //获取列表ID数
  int getCount() {
    IceUtil::Mutex::Lock lock(*this);
    return entrySeqData_.getCount();
  }

  int getCountAfterTime(long time) {
    IceUtil::Mutex::Lock lock(*this);
    return entrySeqData_.getCountAfterTime(time);
  }

  time_t getTimestamp() {
    IceUtil::Mutex::Lock lock(*this);
    return _timestamp;
  }

  void setTimestamp(time_t timestamp) {
    IceUtil::Mutex::Lock lock(*this);
    _timestamp = timestamp;
  }

private:
  RelationCacheEntrySeqData entrySeqData_;
  time_t _timestamp;
};
typedef IceUtil::Handle<IdSeqData> IdSeqDataPtr;

// 共同好友数据
typedef std::map<long, IdSeqDataPtr> CommonFriendsMap;

class CommonFriendCacheData: virtual public Ice::Object, virtual public IceUtil::Mutex {
public:
  CommonFriendCacheData() {
  }
  virtual ~CommonFriendCacheData() {
  }

  IdSeqDataPtr getIdSeqData(long globalId) {
    IceUtil::Mutex::Lock lock(*this);

    CommonFriendsMap::iterator it = mapData_.find(globalId);
    if (it == mapData_.end()) {
      return NULL;
    } else {
      return mapData_[globalId];
    }
  }

  void setIdSeqData(long globalId, const IdSeqDataPtr& idSeq) {
    IceUtil::Mutex::Lock lock(*this);
    //    mapData_.insert(CommonFriendsMap::value_type(globalId, idSeq));
    mapData_[globalId] = idSeq;
  }

private:
  CommonFriendsMap mapData_;

};
typedef IceUtil::Handle<CommonFriendCacheData> CommonFriendCacheDataPtr;

// ICE中接口的实现，负责相应RPC远程调用
class FollowRelationCacheImpl: virtual public FollowRelationCache, virtual public MyUtil::Singleton<
    FollowRelationCacheImpl> {
public:
  virtual ~FollowRelationCacheImpl() {
  }

  // 获取uid的好友中共同关注gid的ID列表
  virtual MyUtil::IntSeq getCommon(Ice::Int uid, const MyUtil::GlobalIdPtr& gid, Ice::Int begin = 0,
      Ice::Int limit = 0, const Ice::Current& current = Ice::Current());
  // 获取uid的好友中共同关注gid的ID列表。只取time时间之后的列表
  virtual MyUtil::IntSeq getCommonAfterTime(Ice::Int uid, const MyUtil::GlobalIdPtr& gid, const long time,
      Ice::Int begin = 0, Ice::Int limit = 0, const Ice::Current& current = Ice::Current());
  // 获取uid的好友中共同关注gid的ID数量
  virtual int
  getCommonCount(Ice::Int uid, const MyUtil::GlobalIdPtr& gid, const Ice::Current& current = Ice::Current());
  // 获取uid的好友中共同关注gid的ID数量。只取time时间之后的列表
  virtual int
  getCommonCountAfterTime(Ice::Int uid, const MyUtil::GlobalIdPtr& gid, const long time, const Ice::Current& current =
      Ice::Current());

  //  获取gid的关注者数量
  virtual int getCount(const MyUtil::GlobalIdPtr& gid, const Ice::Current& current = Ice::Current());

  // 获取gid的关注者ID列表
  virtual MyUtil::IntSeq getList(const MyUtil::GlobalIdPtr& gid, Ice::Int begin, Ice::Int limit,
      const Ice::Current& current = Ice::Current());

  // 判断uid是否关注了gid
  virtual bool hasRelation(Ice::Int uid, const MyUtil::GlobalIdPtr& gid, const Ice::Current& current = Ice::Current());

  // 重新加载gid对应的fans列表缓存数据
  virtual void reload(const long globalId, const Ice::Current& current = Ice::Current());

private:
  friend class MyUtil::Singleton<FollowRelationCacheImpl>;

  IdSeqDataPtr ComputeCommonFriends(const int userId, const long globalId);
  IdSeqDataPtr getCommonFriendData(const int userId, const MyUtil::GlobalIdPtr& gid);

  static xce::tempmutex::TempMutexManagerPtr relationCacheDataTempMutex_;
  static xce::tempmutex::TempMutexManagerPtr commonFriendCacheDataTempMutex_;

  template<class T> T syncLocateObject(const int category, const long id) {
    xce::tempmutex::TempMutexManagerPtr tempMutexPtr;

    switch (category) {
    case CommonFriendCacheDataCategory:
      tempMutexPtr = commonFriendCacheDataTempMutex_;
      break;
    case RelationCacheDataCategory:
      tempMutexPtr = relationCacheDataTempMutex_;
      break;
    default:
      break;
    }

    xce::tempmutex::Lock lock(tempMutexPtr, id);
    return MyUtil::ServiceI::instance().locateObject<T> (category, id);
  }

  bool CheckParam(Ice::Int begin, Ice::Int limit) {
    if (begin < 0 || limit < -1 || limit == 0)
      return false;
    else
      return true;
  }
};

//静态成员初始化
xce::tempmutex::TempMutexManagerPtr FollowRelationCacheImpl::relationCacheDataTempMutex_ =
    new xce::tempmutex::TempMutexManager;
xce::tempmutex::TempMutexManagerPtr FollowRelationCacheImpl::commonFriendCacheDataTempMutex_ =
    new xce::tempmutex::TempMutexManager;

class ServiceSchemaI: public MyUtil::Descriptor {
public:
  virtual void configure(const Ice::PropertiesPtr& props);
};

class RelationCacheDataHandler: virtual public com::xiaonei::xce::ResultHandler {
public:
  RelationCacheDataHandler(RelationCacheEntrySeq& entrySeq) :
    entrySeq_(entrySeq) {
  }
protected:
  virtual bool handle(mysqlpp::Row& res) const {
    int userId = ((int) res["userid"]);
    int followTime = ((long) res["followtime"]);
    RelationCacheEntry entry(userId, followTime);
    entrySeq_.push_back(entry);
    return true;
  }
private:
  RelationCacheEntrySeq& entrySeq_;
};

class RelationCacheDataFactory: virtual public MyUtil::ServantFactory {
public:
  virtual Ice::ObjectPtr create(Ice::Long id) {
    MCE_INFO(__FUNCTION_NAME__ << " id:" << id);
    RelationCacheEntrySeq entrySeq;

    std::ostringstream pattern;
    pattern << "member_" << (id % 100);

    int currSize = 0;
    int lastSize = 0;
    int maxId = 0;

    // 从DB中分批加载，每次加载记录数为DB_LIMIT_SIZE
    do {
      lastSize = currSize;

      com::xiaonei::xce::Statement sql;
      sql << "SELECT userid, UNIX_TIMESTAMP(followtime) as followtime" << " FROM member_" << (id % 100)
          << " WHERE object_id = " << id << " and userid > " << maxId << " limit " << DB_LIMIT_SIZE;
      com::xiaonei::xce::QueryRunner("relation_center", com::xiaonei::xce::CDbRServer, pattern.str()).query(sql,
          RelationCacheDataHandler(entrySeq));

      currSize = entrySeq.size();

      if (currSize > 0) {
        maxId = entrySeq[currSize - 1].getUserId();
      }
      MCE_DEBUG(__FUNCTION_NAME__ << " currSize:" << currSize << ", lastSize:" << lastSize << ", maxId:" << maxId);

      // == DEBUG ==
      //      if (currSize >= RELATION_CACHE_DATA_MAX_SIZE) {
      //        break;
      //      }

    } while (currSize - lastSize == DB_LIMIT_SIZE);

    return new RelationCacheData(entrySeq);
  }
};

class CommonFriendCacheDataFactory: virtual public MyUtil::ServantFactory {
public:
  virtual Ice::ObjectPtr create(Ice::Long id) {
    MCE_INFO("[CommonFriendCacheDataFactory::create:] id:" << id);

    CommonFriendCacheDataPtr object = new CommonFriendCacheData();
    return object;
  }
};

class ReloadTask: virtual public MyUtil::Task {
public:
  ReloadTask(Ice::Long gid, Ice::Int level = TASK_LEVEL_GID_RELOAD) :
    MyUtil::Task(level), _gid(gid) {
  }
  virtual void handle();
private:
  Ice::Long _gid;

};
}
}
#endif
