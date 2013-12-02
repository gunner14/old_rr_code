#ifndef TRIPOD_CLIENT_H_
#define TRIPOD_CLIENT_H_

#include <string>

#include "boost/shared_ptr.hpp"

#include "tripod2/cc/core/cache_proxy.h"
#include "tripod2/cc/core/cache_manager_proxy.h"

#include "OceCxxAdapter/src/XceLoggerAdapter.h"

namespace xce {
namespace tripod {

class TripodClient {
 public:
  TripodClient(const std::string& ns, const std::string& biz , int read_time_out=1000, int write_time_out=1000);
  virtual ~TripodClient();

  bool Get(const std::string& key, std::string* value, int secs = 0);
  bool Get(const std::string& key, std::string* value, bool use_master, int secs = 0);
  bool Set(const std::string& key, const std::string& value, int secs = 0);
  std::map<std::string, bool> MultiGet(const StringListType& keys, StringMapType* values, int secs = 0);
  std::map<std::string, bool> MultiGet(const StringListType& keys, StringMapType* values, bool use_master, int secs = 0);
  bool Load(const std::string& key, int secs, bool use_master);
  bool Remove(const std::string& key);
  
  //成功返回结果字符串，失败返回空字符串
  std::string Inc(const std::string& key, const std::string& step);

  //cache中存在hashtable方法  
  bool GetAllHash(const std::string& key, StringMapType* hash_value, int secs = 0);
  bool LoadHash(const std::string& key, int secs, bool use_master);
  //向cache中存在hashtable情况仅修改部分fields方法
  bool SetPartHash(const std::string& key, const StringMapType& hash_value); 
  bool DeletePartHash(const std::string& key, const StringListType& fields); 
  bool IncFixedHashValue(const std::string& key, const std::string& field, const std::string& step);
  //允许miss的情况
  bool GetAndIgnoreMiss(const std::string& key, std::string* value);
  std::map<std::string, bool> MultiGetAndIgnoreMiss(const StringListType& keys, StringMapType* values);
  bool GetAllHashAndIgnoreMiss(const std::string& key, StringMapType* hash_value);

  bool GetList(const std::string& key, StringListType* value, int begin, int limit, bool use_master, int secs = 0);
  bool GetListAndIgnoreMiss(const std::string& key, StringListType* value, int begin, int limit);
  bool SetList(const std::string& key, const StringListType& value, int secs = 0);
  bool LoadList(const std::string& key, bool use_master, int secs = 0);
 
  //attion : this three operations is for the set-get mode, haven`t use producer need to add other operators for the total pm-cm-cc mode!!  
  bool ZAdd(const std::string& key, const StringMapType& set_with_scores);
  bool ZRemRangeByRank(const std::string& key, int begin, int end);
  bool ZRemRangeByScore(const std::string& key, const std::string& min, const std::string& max);
  bool ZRange(const std::string& key, StringListType* zset_value, int begin, int limit);
  bool ZSet(const std::string& key, const StringMapType& set_with_scores, int secs = 0);

  int Size(const std::string& key);
  std::string namespace_id() const {
    return namespace_id_;
  }
 
  std::string biz_id() const {
    return biz_id_;
  }
 private:
  std::string namespace_id_; 
  std::string biz_id_; 
  
  boost::shared_ptr<CacheProxy> cache_proxy_;
  boost::shared_ptr<CacheManagerProxy> cache_manager_proxy_;

};

}} // xce::tripod
#endif


