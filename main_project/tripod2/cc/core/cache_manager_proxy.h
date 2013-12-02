#ifndef CACHE_MANAGER_PROXY_H_
#define CACHE_MANAGER_PROXY_H_

#include <string>
#include <map>


#include "base/consistenthash.h"

#include "boost/shared_ptr.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread/locks.hpp"

#include "tripod2/cc/core/common.h"
#include "tripod2/cc/core/zk_client.h"

#include "Ice/Ice.h"

#include "OceSlice/slice/tripod2.h"

namespace xce {
namespace tripod {

class CacheManagerProxy : public ZNodeListener {
 public:
  CacheManagerProxy(const std::string& ns, const std::string& biz, int conn_time_out);
  virtual ~CacheManagerProxy();

  virtual bool Update(const std::list<std::string>& children_names);

  bool Get(const std::string& key, std::string* value, int expire_time, bool use_master);
  bool Load(const std::string& key, int expire_time, bool use_master);
  bool Remove(const std::string& key);
  bool Set(const std::string& key, const std::string& value, int expire_time);

  //add by jipeng.lin for the condition of the cachebody is list.
  bool GetList(const std::string& key, StringList* value,int begin, int limit, int expire_time, bool use_master);
  bool LoadList(const std::string& key, int expire_time, bool use_master);
  bool SetList(const std::string& key, const StringList& value, int expire_time);
  
  //成功返回结果字符串，失败返回空字符串
  std::string Inc(const std::string& key, const std::string& step); 

  //Add by jipeng.lin for conditions of cachebody is hashtable and the change is only the fields of table
  bool GetAllHash(const std::string& key, String2StringMap* value, int expire_time, bool use_master);
  bool LoadHash(const std::string& key, int expire_time, bool use_master);
  bool SetPartHash(const std::string& key, const String2StringMap& value);
  bool DeletePartHash(const std::string& key, const StringList& field);
  bool IncFixedHashValue(const std::string& key, const std::string& field, const std::string& step); 

  bool ZAdd(const std::string& key, const String2StringMap& set_with_scores);
  bool ZRemRangeByRank(const std::string& key, int begin, int end);
  bool ZRemRangeByScore(const std::string& key, const std::string& min, const std::string& max);
  bool ZSet(const std::string& key, const String2StringMap& set_with_scores, int expire_time);
 
  std::string namespace_id() const {
    return namespace_id_;
  }
  std::string biz_id() const {
    return biz_id_;
  }

 private:
  CacheManagerPrx LocateCacheManagerProxy(const std::string& key);

 private:
  std::string namespace_id_;
  std::string biz_id_;
  int conn_time_out_; 


  ContinuumPtr continuum_;
  boost::shared_mutex continuum_mutex_;

  typedef std::map<std::string, CacheManagerPrx> CacheManagerPrxMapType;
  CacheManagerPrxMapType cache_manager_prx_map_;
  
  Ice::CommunicatorPtr communicator_;
};


}} // xce::tripod
#endif

