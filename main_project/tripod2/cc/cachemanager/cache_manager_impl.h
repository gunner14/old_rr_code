#ifndef CACHE_MANAGER_IMPL_H_
#define CACHE_MANAGER_IMPL_H_

#include <map>

#include "boost/thread/mutex.hpp"

#include "tripod2/cc/core/bucket_lock.h"
#include "tripod2/cc/core/singleton.h"
#include "tripod2/cc/core/cache_proxy.h"
#include "tripod2/cc/core/producer_manager_proxy.h"
#include "OceSlice/slice/tripod2.h"
#include "OceCxxAdapter/src/XceLoggerReplicaAdapter.h"



namespace xce {
namespace tripod {

class CacheManagerImpl : public CacheManager {
 public:
  CacheManagerImpl(int cache_time_out, int pm_time_out);
  virtual ~CacheManagerImpl();

  virtual ByteSeq get(const std::string& key, 
                            const std::string& ns, 
                            const std::string& biz, 
                            int expire_time, 
                            bool use_master, const ::Ice::Current& = ::Ice::Current());
    
  virtual bool load(const std::string& key, 
                    const std::string& ns, 
                    const std::string& biz, 
                    int expire_time, 
                    bool use_master, const ::Ice::Current& = ::Ice::Current());
    
  virtual bool remove(const std::string& key, 
                      const std::string& ns, 
                      const std::string& biz, 
                      const ::Ice::Current& = ::Ice::Current());

  virtual bool set(const std::string& key, 
                   const ByteSeq& value, 
                   const std::string& ns, 
                   const std::string& biz, 
                   Ice::Int expire_time, 
                   const Ice::Current& = ::Ice::Current());

  //add list operation by jipeng.lin
  virtual StringList getList(const std::string& key,
                   const std::string& ns,
                   const std::string& biz,
                   int begin,
                   int limit,
                   int expire_time,
                   bool use_master, const ::Ice::Current& = ::Ice::Current());

  virtual bool loadList(const std::string& key,
                   const std::string& ns,
                   const std::string& biz,
                   int expire_time,
                   bool use_master, const ::Ice::Current& = ::Ice::Current());

  virtual bool setList(const std::string& key,
                   const StringList& value,
                   const std::string& ns,
                   const std::string& biz,
                   int expire_time,
                   const ::Ice::Current& = ::Ice::Current());

  //返回增加后的结果，不成功返回空字符串
  virtual string inc(const std::string& key, 
                   const std::string& step, 
                   const std::string& ns, 
                   const std::string& biz, 
                   const Ice::Current& = ::Ice::Current());

  virtual String2StringMap getAllHash(const std::string& key,
                   const std::string& ns,
                   const std::string& biz,
                   int expire_time,
                   bool use_master, const ::Ice::Current& = ::Ice::Current());

  virtual bool loadHash(const std::string& key,
                   const std::string& ns,
                   const std::string& biz,
                   int expire_time,
                   bool use_master, const ::Ice::Current& = ::Ice::Current());
  
  virtual bool setPartHash(const std::string& key,
                   const String2StringMap& value,
                   const std::string& ns,
                   const std::string& biz,
                   const ::Ice::Current& = ::Ice::Current());

  virtual bool incFixedHashValue(const std::string& key,
                   const std::string& field,
                   const std::string& step,
                   const std::string& ns,
                   const std::string& biz,
                   const ::Ice::Current& = ::Ice::Current()); 
 
  virtual bool deletePartHash(const std::string& key,
                   const StringList& fields,
                   const std::string& ns,
                   const std::string& biz,
                   const ::Ice::Current& = ::Ice::Current());

  virtual bool zAdd(const std::string& key,
                    const std::string& ns,
                    const std::string& biz,
                    const String2StringMap& zsetValue,
                    const ::Ice::Current& = ::Ice::Current());

  virtual bool zRemRangeByRank(const std::string& key,
                    const std::string& ns,
                    const std::string& biz,
                    int begin,
                    int end,
                    const ::Ice::Current& = ::Ice::Current());

  virtual bool zRemRangeByScore(const std::string& key,
                    const std::string& ns,
                    const std::string& biz,
                    const std::string& min,
                    const std::string& max,
                    const ::Ice::Current& = ::Ice::Current());


  virtual bool zSet(const std::string& key,
                    const std::string& ns,
                    const std::string& biz,
                    const String2StringMap& zsetValue,
                    int expire_time,
                    const ::Ice::Current& = ::Ice::Current());
  
  virtual std::string ping(const ::Ice::Current& = ::Ice::Current());
 private:
  CacheProxy* LocateCacheProxy(const std::string& ns, const std::string& biz);
  ProducerManagerProxy* LocateProducerManagerProxy(const std::string& ns, const std::string& biz);

  typedef std::map<std::pair<std::string, std::string>, CacheProxy*> CacheProxyMapType;

  CacheProxyMapType cache_proxy_map_;
  boost::shared_mutex cache_proxy_mutex_;

  typedef std::map<std::pair<std::string, std::string>, ProducerManagerProxy*> ProducerManagerProxyMapType;

  ProducerManagerProxyMapType producer_manager_proxy_map_;
  boost::shared_mutex producer_manager_proxy_mutex_;


 private:
  StringList ListCut(const StringList& totalList, int begin, int limit);
  void AddRemoteLog(const std::string& message);
  int cache_time_out_;
  int pm_time_out_;
  com::xiaonei::service::XceLoggerReplicaPtr remote_logger_;
  std::vector<std::string> log_paths_;
};

}} // xce::tripod
#endif

