#ifndef _FOLLOW_RESOURCE_CAHCE_IMPL_H_
#define _FOLLOW_RESOURCE_CAHCE_IMPL_H_
#include "FollowResourceCache.h"
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

namespace xce {
namespace buddy {

const int ResourceCacheDataCategory = 0;

const int TASK_LEVEL_LOAD = 0;
const int TASK_LEVEL_RELOAD = 1;

//资源ID列表数据
//在所有读取和修改数据的代码中使用了ID锁，由于该数据结构与ID对应，因此不再需要锁
//此处ID为userId和type合成的guid
//?? 考虑用读写锁？
class IdSeqData: virtual public Ice::Object {
public:

  IdSeqData(MyUtil::LongSeq& ids) {
    _ids.swap(ids);
  }

  virtual ~IdSeqData() {
    ;
  }

  //从指定位置获取指定长度的ID列表
  MyUtil::LongSeq get(int begin, int limit) {
    if (limit < 0) {
      return _ids;
    }
    if (((unsigned int) begin >= _ids.size()) || (begin < 0)) {
      return MyUtil::LongSeq();
    }
    if ((unsigned int) (begin + limit) >= _ids.size()) {
      return MyUtil::LongSeq(_ids.begin() + begin, _ids.end());
    } else {
      return MyUtil::LongSeq(_ids.begin() + begin, _ids.begin() + begin + limit);
    }
  }

  //查找列表中是否存在指定ID
  bool find(long id) {
    MyUtil::LongSeq::iterator it = lower_bound(_ids.begin(), _ids.end(), id);
    //??
    if (it != _ids.end() && *it == id) {
      return true;
    } else {
      return false;
    }
  }

  //在列表中添加一个ID
  void add(long id) {
    MyUtil::LongSeq::iterator it = lower_bound(_ids.begin(), _ids.end(), id);
    if (it != _ids.end() && *it == id) {
    } else {
      _ids.insert(it, id);
    }
  }

  //在列表中删除一个ID
  void remove(int id) {
    MyUtil::LongSeq::iterator it = lower_bound(_ids.begin(), _ids.end(), id);
    if (it != _ids.end() && *it == id) {
      _ids.erase(it);
    }
  }

  //清空ID列表
  void clear() {
    _ids.clear();
  }

  //获取列表ID数
  int getCount() {
    //?? size_type to int?
    return _ids.size();
  }

private:
  MyUtil::LongSeq _ids;
};

typedef IceUtil::Handle<IdSeqData> IdSeqDataPtr;

// ICE中接口的实现，负责相应RPC远程调用
class FollowResourceCacheImpl: virtual public FollowResourceCache, virtual public MyUtil::Singleton<
    FollowResourceCacheImpl> {
public:

  virtual ~FollowResourceCacheImpl() {
  }

  //  获取关注的资源数量
  virtual int getCount(const int userId, const int type, const Ice::Current& current = Ice::Current());

  // 获取关注的资源列表，ID为资源在类别中的ID，而不是GID
  virtual MyUtil::LongSeq getList(const int userId, const int type, const int begin, const int limit,
      const Ice::Current& current = Ice::Current());

  // 返回一个用户是否关注了某类资源中的一批资源
  virtual MyUtil::BoolSeq hasRelationBatch(const int userId, const int type, const MyUtil::IntSeq& idSeq,
      const Ice::Current& current = Ice::Current());

  // 重新加载缓存数据
  virtual void reload(const int userId, const int type, const Ice::Current& current = Ice::Current());

  // 根据userId和type组合guid
  static long getGuid(int userId, int type) {
    MyUtil::GlobalIdPtr globalId = new MyUtil::GlobalId(type, userId);
    return MyUtil::GlobalIdTransfer::get(globalId);
  }

  static xce::tempmutex::TempMutexManagerPtr tempMutexManagerPtr_;

private:
  friend class MyUtil::Singleton<FollowResourceCacheImpl>;
};

//静态成员初始化
xce::tempmutex::TempMutexManagerPtr FollowResourceCacheImpl::tempMutexManagerPtr_ =
    new xce::tempmutex::TempMutexManager;

class ServiceSchemaI: public MyUtil::Descriptor {
public:
  virtual void configure(const Ice::PropertiesPtr& props);
};

class OrderByIdAscHandler: virtual public com::xiaonei::xce::ResultHandler {
public:
  OrderByIdAscHandler(std::vector<long>& data, const std::string& keyField) :
    _data(data), _keyField(keyField) {
  }
protected:
  virtual bool handle(mysqlpp::Row& res) const {
    _data.push_back((long) res[_keyField.c_str()]);
    return true;
  }
private:
  std::vector<long>& _data;
  std::string _keyField;
};

class IdSeqDataFactory: virtual public MyUtil::ServantFactory {
public:
  virtual Ice::ObjectPtr create(Ice::Long id) {
    // 获取guid中的type和userId
    long guid = id;
    MyUtil::GlobalIdPtr globalId = MyUtil::GlobalIdTransfer::get(guid);
    int userId = globalId-> id;
    int type = globalId->type;

    MCE_INFO("[IdSeqDataFactory::create:] userid:" << userId << " type:" << type);

    // 从DB中取出userId关注的所有object_id,按object_id升序排列
    std::vector<long> ids;
    com::xiaonei::xce::Statement sql;
    sql << "SELECT object_id" << " FROM user_member_" << (userId % 100) << " WHERE userid = " << userId
        << " order by object_id asc";
    std::ostringstream pattern;
    pattern << "user_member_" << (userId % 100);
    com::xiaonei::xce::QueryRunner("relation_center_user", com::xiaonei::xce::CDbRServer, pattern.str()).query(sql,
        OrderByIdAscHandler(ids, "object_id"));

    // 筛选出指定type的resourceId
    MyUtil::LongSeq resourceIds;
    for (MyUtil::LongSeq::iterator it = ids.begin(); it != ids.end(); ++it) {
      MyUtil::GlobalIdPtr globalObjectId = MyUtil::GlobalIdTransfer::get(*it);
      if (globalObjectId->type == type) {
        resourceIds.push_back(globalObjectId->id);
      }
    }

    return new IdSeqData(resourceIds);
  }
};

class LoadTask: virtual public MyUtil::Task {
public:
  LoadTask(int userId, int type, int level = TASK_LEVEL_LOAD) :
    MyUtil::Task(level), userId_(userId), type_(type) {
  }
  virtual void handle() {
    long guid = FollowResourceCacheImpl::getGuid(userId_, type_);
    xce::tempmutex::Lock lock(FollowResourceCacheImpl::tempMutexManagerPtr_, guid);
    MyUtil::ServiceI::instance().locateObject<IdSeqDataPtr> (ResourceCacheDataCategory, guid);
  }
private:
  int userId_;
  int type_;

};
class ReloadTask: virtual public MyUtil::Task {
public:
  ReloadTask(int userId, int type, int level = TASK_LEVEL_RELOAD) :
    MyUtil::Task(level), userId_(userId), type_(type) {
  }
  virtual void handle() {
    long guid = FollowResourceCacheImpl::getGuid(userId_, type_);
    xce::tempmutex::Lock lock(FollowResourceCacheImpl::tempMutexManagerPtr_, guid);
    MyUtil::ServiceI::instance().reloadObject(ResourceCacheDataCategory, guid);
  }
private:
  int userId_;
  int type_;
};
}
}
#endif
