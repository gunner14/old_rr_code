#include "tripod2/cc/core/cache_proxy.h"

#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"

#include "LogWrapper.h"

namespace xce {
namespace tripod {


CacheProxy::CacheProxy(const std::string& ns, const std::string& biz, int conn_time_out) : 
  ZNodeListener(kDefaultZkCacheZNode + kZkSeparator + ns), 
  namespace_id_(ns), biz_id_(biz), conn_time_out_(conn_time_out), valid_(false) {
  ZkClientPtr zk_client = ZkClient::GetInstance(kDefaultZkAddress);
  MCE_INFO("CacheProxy::CacheProxy() init zkpath=" << kDefaultZkCacheZNode << kZkSeparator << ns 
      << ", biz=" << biz << ", zkaddr=" << kDefaultZkAddress);
  if (!zk_client) {
    MCE_ERROR("CacheProxy::CacheProxy() cannot get ZkClient, zk_address:" << kDefaultZkAddress);
  }
  else {
    MCE_DEBUG("CacheProxy this:" << this);
    zk_client->AddZNodeListener(this);
    std::list<std::string> children_names;
    zk_client->GetNodes(znode(), &children_names);
    Update(children_names);
    set_valid(true);
  }
}

CacheProxy::~CacheProxy() {
  MCE_INFO("CacheProxy::~CacheProxy()");
  if (IsValid()) {
    ZkClientPtr zk_client = ZkClient::GetInstance(kDefaultZkAddress);
    if (zk_client) {
      zk_client->RemoveZNodeListener(this);
    }
  }
}

bool CacheProxy::Get(const std::string& key, std::string* value) {
// 1. generate cache_key
// 2. get a valid CacheClient
// 3. get value
  if (!value) {
    return false;
  }
  std::string cache_key = GenerateCacheKey(biz_id_, key);
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_ERROR("CacheProxy::Get() call LocateCacheClientPool return NULL cache_key" << cache_key << " ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  CacheClient* cache_client = pool->BorrowItem();
  //if (!cache_client || !cache_client->IsValid()) {
  //  return false;
  //}
  if (!cache_client) {
    return false;
  }
  if (!cache_client->IsValid()) {
    pool->ReturnItem(cache_client);
    return false;
  }
  bool result = cache_client->Get(cache_key, value);
  pool->ReturnItem(cache_client);
  return result;
}

bool CacheProxy::Set(const std::string& key, const std::string& value, int secs) {
// 1. generate cache_key
// 2. get a valid CacheClient
// 3. set value
  std::string cache_key = GenerateCacheKey(biz_id_, key);
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_ERROR("CacheProxy::Set() call LocateCacheClientPool return NULL cache_key:" << cache_key << " ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  CacheClient* cache_client = pool->BorrowItem();
  MCE_DEBUG("CacheProxy::Set() key " << key <<" pool->Dump():" << pool->Dump());
  if (!cache_client) {
    MCE_ERROR("CacheProxy::Set() pool->BorrowItem() key " << key <<" return null ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  if (!cache_client->IsValid()) {
    pool->ReturnItem(cache_client);
    MCE_ERROR("CacheProxy::Set() pool->BorrowItem() key " << key << " return not valid cache_client ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  bool result = cache_client->Set(cache_key, value, secs);
  pool->ReturnItem(cache_client);
  return result;
}

std::map<std::string, bool> CacheProxy::MultiGet(const StringListType& keys,
    StringMapType* values) {
  std::map<std::string, bool> result;
  std::map<CacheClientPoolPtr, list<std::string> > pool_map;
  BOOST_FOREACH(const std::string& key, keys) { 
    CacheClientPoolPtr pool = LocateCacheClientPool(key);
    if (!pool) {
      MCE_ERROR("CacheProxy::MultiGet() call LocateCacheClientPool return NULL key:" << key << " ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
      result.insert(std::make_pair(key, false));
    } else {
      if (pool_map.find(pool) != pool_map.end()) {
        pool_map[pool].push_back(key);
      } else {
        std::list<std::string> pool_keys;
        pool_keys.push_back(key);
        pool_map.insert(std::make_pair(pool,pool_keys));
      }
    }
  }
  MCE_TRACE("CacheProxy::MultiGet() call Pool size" << pool_map.size() );
  std::map<CacheClientPoolPtr, list<std::string> >::iterator map_iter;
  for (map_iter = pool_map.begin(); map_iter != pool_map.end(); ++map_iter){
    CacheClient* cache_client = (map_iter->first)->BorrowItem();
    if (!cache_client || !cache_client->IsValid()) {
      std::list<std::string>::iterator it = map_iter->second.begin();
      for(;it!= map_iter->second.end();++it) {
        result.insert(std::make_pair(*it, false));
      }
      continue;
    }
    MCE_TRACE("CacheProxy::MultiGet() call each pool size" << map_iter->second.size() );
    std::list<std::string> cache_keys;
    std::map<std::string,std::string> cache_map;
    BOOST_FOREACH(const std::string& iter_key, map_iter->second) { 
      std::string cache_key = GenerateCacheKey(biz_id_, iter_key);
      cache_keys.push_back(cache_key);
      cache_map.insert(std::make_pair(iter_key,cache_key));
    }
    std::map<std::string, bool> cache_result;
    std::map<std::string, std::string> cache_values;
    cache_result = cache_client->MultiGet(cache_keys,&cache_values);
    for(std::map<std::string,std::string>::iterator iter= cache_map.begin(); iter != cache_map.end(); ++iter) { 
      if(cache_result.find(iter->second) != cache_result.end()) {
        result.insert(std::make_pair(iter->first,cache_result[iter->second]));
        values->insert(std::make_pair(iter->first,cache_values[iter->second]));
      }
    }
    (map_iter->first)->ReturnItem(cache_client);
  }
  MCE_TRACE("CacheProxy::MultiGet() size" << result.size() << values->size() );
  return result;
}

std::string CacheProxy::Inc(const std::string& key, const std::string& step) {
  MCE_DEBUG(" CacheProxy::Inc(const std::string& key, const std::string& step) key : " << key );
  std::string result;
  std::string cache_key = GenerateCacheKey(biz_id_, key);
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_ERROR("CacheProxy::Inc() call LocateCacheClientPool return NULL cache_key" 
        << cache_key << " ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
  } else {
    MCE_DEBUG("CacheProxy::Inc() pool->Dump(): key " << key << " Dump" << pool->Dump());
    CacheClient* cache_client = pool->BorrowItem();
    if(cache_client) {
      MCE_DEBUG(" CacheProxy::Inc() key : " << key <<  " cache_client is exsist!");
      if(cache_client->IsValid()) {
        result = cache_client->Inc(cache_key, step);
      } else {
         MCE_DEBUG(" CacheProxy::Inc() key : " << key <<  " cache_client->IsValid() return false!");
      }
      pool->ReturnItem(cache_client);
    } else {
      MCE_DEBUG(" CacheProxy::Inc() key : " << key <<  " cache_client is not exsist!");
    }
  }
  return result;
}

bool CacheProxy::GetList(const std::string& key, StringListType* value, int begin, int limit) {
  std::string cache_key = GenerateCacheKey(biz_id_, key);
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_ERROR("CacheProxy::GetList() call LocateCacheClientPool return NULL cache_key" << cache_key);
    return false;
  }
  CacheClient* cache_client = pool->BorrowItem();
  if (!cache_client || !cache_client->IsValid()) {
    return false;
  }
  bool result = cache_client->GetList(cache_key, value, begin, limit);
  pool->ReturnItem(cache_client);
  return result;
}

bool CacheProxy::SetList(const std::string& key, const StringListType& list_value, int secs) {
  std::string cache_key = GenerateCacheKey(biz_id_, key);
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_ERROR("CacheProxy::SetList() call LocateCacheClientPool return NULL cache_key" << cache_key);
    return false;
  }
  CacheClient* cache_client = pool->BorrowItem();
  if (!cache_client || !cache_client->IsValid()) {
    return false;
  }
  bool result = cache_client->SetList(cache_key, list_value, secs);
  pool->ReturnItem(cache_client);
  return result;
}

bool CacheProxy::GetHash(const std::string& key, const StringListType& fields, StringMapType* hash_value) {
  std::string cache_key = GenerateCacheKey(biz_id_, key);
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_ERROR("CacheProxy::GetHash() call LocateCacheClientPool return NULL cache_key" << cache_key);
    return false;
  }
  CacheClient* cache_client = pool->BorrowItem();
  if (!cache_client || !cache_client->IsValid()) {
    return false;
  }
  bool result = cache_client->GetHash(cache_key, fields, hash_value);
  pool->ReturnItem(cache_client);
  return result;
}

bool CacheProxy::SetHash(const std::string& key, const StringMapType& hash_value, int secs) {
  std::string cache_key = GenerateCacheKey(biz_id_, key);
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_ERROR("CacheProxy::SetHash() call LocateCacheClientPool return NULL cache_key:" << cache_key);
    return false;
  }
  CacheClient* cache_client = pool->BorrowItem();
  if (!cache_client || !cache_client->IsValid()) {
    return false;
  }
  bool result = cache_client->SetHash(cache_key, hash_value, secs);
  pool->ReturnItem(cache_client);
  return result;
}

bool CacheProxy::HMSet(const std::string& key, const StringMapType& hash_value) {
  std::string cache_key = GenerateCacheKey(biz_id_, key);
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_ERROR("CacheProxy::HMSet() call LocateCacheClientPool return NULL cache_key:" << cache_key << " ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  CacheClient* cache_client = pool->BorrowItem();
  if (!cache_client || !cache_client->IsValid()) {
    return false;
  }
  bool result = cache_client->HMSet(cache_key, hash_value);
  pool->ReturnItem(cache_client);
  return result;
  
}

bool CacheProxy::HGetAll(const std::string& key, StringMapType* hash_value) {
  std::string cache_key = GenerateCacheKey(biz_id_, key);
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_ERROR("CacheProxy::HGetAll() call LocateCacheClientPool return NULL cache_key:" << cache_key << " ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  CacheClient* cache_client = pool->BorrowItem();
  if (!cache_client || !cache_client->IsValid()) {
    return false;
  }
  bool result = cache_client->HGetAll(cache_key, hash_value);
  pool->ReturnItem(cache_client);
  return result;
}

bool CacheProxy::HDel(const std::string& key, const StringListType& fields) {
  std::string cache_key = GenerateCacheKey(biz_id_, key);
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_ERROR("CacheProxy::HDel() call LocateCacheClientPool return NULL cache_key:" << cache_key << " ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  CacheClient* cache_client = pool->BorrowItem();
  if (!cache_client || !cache_client->IsValid()) {
    return false;
  }
  bool result = cache_client->HDel(cache_key, fields);
  pool->ReturnItem(cache_client);
  return result;
}

bool CacheProxy::HIncrBy(const std::string& key, const std::string& field, const std::string& step) {
  std::string cache_key = GenerateCacheKey(biz_id_, key);
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_ERROR("CacheProxy::HIncrBy() call LocateCacheClientPool return NULL cache_key:" << cache_key << " ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  CacheClient* cache_client = pool->BorrowItem();
  if (!cache_client || !cache_client->IsValid()) {
    return false;
  }
  bool result = cache_client->HIncrBy(cache_key, field, step);
  pool->ReturnItem(cache_client);
  return result;
 
}

int  CacheProxy::Exists(const std::string& key) {
  std::string cache_key = GenerateCacheKey(biz_id_, key);
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_ERROR("CacheProxy::Exists() call LocateCacheClientPool return NULL cache_key:" << cache_key << " ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  CacheClient* cache_client = pool->BorrowItem();
  if (!cache_client || !cache_client->IsValid()) {
    return false;
  }
  bool result = cache_client->Exists(cache_key);
  pool->ReturnItem(cache_client);
  return result;
}

bool CacheProxy::Remove(const std::string& key) {
  std::string cache_key = GenerateCacheKey(biz_id_, key);
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_ERROR("CacheProxy::Remove() call LocateCacheClientPool return NULL cache_key:" << cache_key << " ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  CacheClient* cache_client = pool->BorrowItem();
  if (!cache_client || !cache_client->IsValid()) {
    return false;
  }
  bool result = cache_client->Remove(cache_key);
  pool->ReturnItem(cache_client);
  return result;
}

int  CacheProxy::Size(const std::string& key) {
  std::string cache_key = GenerateCacheKey(biz_id_, key);
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_ERROR("CacheProxy::Size() call LocateCacheClientPool return NULL cache_key:" << cache_key << " ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  CacheClient* cache_client = pool->BorrowItem();
  if (!cache_client || !cache_client->IsValid()) {
    return false;
  }
  int result = cache_client->Size(cache_key);
  pool->ReturnItem(cache_client);
  return result;
}

bool CacheProxy::ZAdd(const std::string& key, const StringMapType& set_with_scores) {
  std::string cache_key = GenerateCacheKey(biz_id_, key);
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_ERROR("CacheProxy::ZAdd() call LocateCacheClientPool return NULL cache_key:" << cache_key << " ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  CacheClient* cache_client = pool->BorrowItem();
  if (!cache_client || !cache_client->IsValid()) {
    return false;
  }
  bool result = cache_client->ZAdd(cache_key, set_with_scores);
  pool->ReturnItem(cache_client);
  return result;
}

bool CacheProxy::ZSet(const std::string& key, const StringMapType& set_with_scores, int secs) {
  std::string cache_key = GenerateCacheKey(biz_id_, key);
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_ERROR("CacheProxy::ZSet() call LocateCacheClientPool return NULL cache_key:" << cache_key << " ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  CacheClient* cache_client = pool->BorrowItem();
  if (!cache_client || !cache_client->IsValid()) {
    return false;
  }
  bool result = cache_client->ZSet(cache_key, set_with_scores, secs);
  pool->ReturnItem(cache_client);
  return result;
}

bool CacheProxy::ZRemRangeByRank(const std::string& key, int begin, int end) {
  std::string cache_key = GenerateCacheKey(biz_id_, key);
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_ERROR("CacheProxy::ZRemRangeByRank() call LocateCacheClientPool return NULL cache_key:" << cache_key << " ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  CacheClient* cache_client = pool->BorrowItem();
  if (!cache_client || !cache_client->IsValid()) {
    return false;
  }
  bool result = cache_client->ZRemRangeByRank(cache_key, begin, end);
  pool->ReturnItem(cache_client);
  return result;
}

bool CacheProxy::ZRemRangeByScore(const std::string& key, const std::string& min, const std::string& max) {
  std::string cache_key = GenerateCacheKey(biz_id_, key);
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_ERROR("CacheProxy::ZRemRangeByScore() call LocateCacheClientPool return NULL cache_key:" << cache_key << " ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  CacheClient* cache_client = pool->BorrowItem();
  if (!cache_client || !cache_client->IsValid()) {
    return false;
  }
  bool result = cache_client->ZRemRangeByScore(cache_key, min, max);
  pool->ReturnItem(cache_client);
  return result;
}

bool CacheProxy::ZRange(const std::string& key, StringListType* zset_value, int begin, int limit) {
  std::string cache_key = GenerateCacheKey(biz_id_, key);
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_ERROR("CacheProxy::ZRange() call LocateCacheClientPool return NULL cache_key:" << cache_key << " ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  CacheClient* cache_client = pool->BorrowItem();
  if (!cache_client || !cache_client->IsValid()) {
    return false;
  }
  bool result = cache_client->ZRange(cache_key, begin, limit, zset_value);
  pool->ReturnItem(cache_client);
  return result;
}
CacheClientPoolPtr CacheProxy::LocateCacheClientPool(const std::string& key) {
  boost::shared_lock<boost::shared_mutex> read_lock(continuum_mutex_);
  std::string nickname = continuum_->Locate(xce::Continuum::Hash(key));
  MCE_TRACE("CacheProxy::LocateCacheClientPool() KEY " << key << " nickname: " << nickname);
  if (nickname.empty()) {
    return CacheClientPoolPtr();
  }
  CacheClientPoolMapType::const_iterator iter = pool_map_.find(nickname);
  if (iter == pool_map_.end()) {
    return CacheClientPoolPtr();
  }
  return iter->second;
}

bool CacheProxy::Update(const std::list<std::string>& children_names) {
  MCE_INFO("CacheProxy::Update()");
  boost::unique_lock<boost::shared_mutex> write_lock(continuum_mutex_);
  CacheClientPoolMapType new_pool_map;
  xce::Continuum* cm = new xce::Continuum();
  BOOST_FOREACH(const std::string& child_name, children_names) {
    MCE_DEBUG("child name:" << child_name);
    std::vector<std::string> units;
    boost::split(units, child_name, boost::is_any_of(":"));   
    int unit_size = units.size();
    if (unit_size != 3) {
      MCE_ERROR("CacheProxy::Update() child_name:" << child_name << " format error");
      return false;
    }
    std::string nickname = units[0];
    std::string cache_address = units[1];
    int cache_port = 0;
    try {
      cache_port = boost::lexical_cast<int>(units[2]);
    } catch (...) {
      MCE_ERROR("CacheProxy::Update() cache_port format error, child_name:" << child_name);
      return false;
    }
    MCE_DEBUG("nickname:" << nickname << "\t" << "cache_address:" << cache_address << "\t" << "cache_port:" << cache_port);
    //CacheClientPoolPtr pool(new RedisClientPool(cache_address, cache_port));    
    CacheClientPoolPtr pool = CacheClientPoolFactory::GetPool(cache_address, cache_port,conn_time_out_);
    cm->Add(nickname, 100);
    new_pool_map.insert(std::make_pair(nickname, pool)); 
  }
  if (!new_pool_map.empty()) {
    cm->Rebuild();
    continuum_.reset(cm);
    pool_map_.swap(new_pool_map);
  }
  MCE_DEBUG("CacheProxy::Update() continuum_->size():" << continuum_->size());
  MCE_DEBUG("CacheProxy::Update() pool_map_.size():" << pool_map_.size());
  return true;
}

}} // xce::tripod

