#include "tripod2/cc/core/tripod_client.h"
#include "tripod2/cc/core/invoke_client.h"
#include "boost/foreach.hpp"
#include "LogWrapper.h"


namespace xce {
namespace tripod {

TripodClient::TripodClient(const std::string& ns, const std::string& biz, int read_time_out, int write_time_out) : namespace_id_(ns), biz_id_(biz) {
  cache_proxy_.reset(new CacheProxy(namespace_id_, biz_id_, read_time_out));
  cache_manager_proxy_.reset(new CacheManagerProxy(namespace_id_, biz_id_,write_time_out ));
}

TripodClient::~TripodClient() {
  if (cache_proxy_) {
    cache_proxy_.reset();
  }
  if (cache_manager_proxy_) {
    cache_manager_proxy_.reset();
  }
}

bool TripodClient::Get(const std::string& key, std::string* value, bool use_master, int secs) {
  InvokeClientSingleton::instance().Log("get", namespace_id_, biz_id_);
  if (!cache_proxy_) {
    MCE_ERROR("TripodClient::Get() cache_proxy is NULL " << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  if (!cache_manager_proxy_) {
    MCE_ERROR("TripodClient::Get() cache_manager_proxy is NULL " << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  bool cache_result = cache_proxy_->Get(key, value);
  if (cache_result) {
    return cache_result;
  }
  else {
    InvokeClientSingleton::instance().Log("miss", namespace_id_, biz_id_);
    return cache_manager_proxy_->Get(key, value, secs , use_master );
  }
}


bool TripodClient::Get(const std::string& key, std::string* value, int secs) {
  return Get(key,value,false,secs);
}

bool TripodClient::Set(const std::string& key, const std::string& value, int secs) {
  InvokeClientSingleton::instance().Log("set", namespace_id_, biz_id_);
  if (!cache_manager_proxy_) {
    MCE_ERROR("TripodClient::Set() cache_manager_proxy is NULL ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  return cache_manager_proxy_->Set(key, value, secs);
}

std::string TripodClient::Inc(const std::string& key, const std::string& step) {
  InvokeClientSingleton::instance().Log("inc", namespace_id_, biz_id_);
  if (!cache_manager_proxy_) {
    MCE_ERROR("TripodClient::Inc() cache_manager_proxy is NULL ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  return cache_manager_proxy_->Inc(key, step);
}

//允许miss的情况
bool TripodClient::GetAndIgnoreMiss(const std::string& key, std::string* value) {
  InvokeClientSingleton::instance().Log("getAndIgnoreMiss", namespace_id_, biz_id_);
  if (!cache_proxy_) {
    MCE_ERROR("TripodClient::GetAndIgnoreMiss() cache_proxy is NULL " << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  bool cache_result = cache_proxy_->Get(key, value);
  if (!cache_result) {
    InvokeClientSingleton::instance().Log("miss", namespace_id_, biz_id_);
  }
  return cache_result;
}

std::map<std::string, bool> TripodClient::MultiGet(const StringListType& keys,
    StringMapType* values, bool use_master, int secs) {
  InvokeClientSingleton::instance().Log("multiGet", namespace_id_, biz_id_, keys.size());
  std::map<std::string, bool> result;
  if (!cache_proxy_) {
    MCE_ERROR("TripodClient::MultiGet() cache_proxy is NULL " << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return result;
  }
  result = cache_proxy_->MultiGet(keys,values);
  long miss_count =0;
  BOOST_FOREACH(const std::string& key, keys) {
    std::map<std::string, bool>::iterator iter = result.find(key);
    if(iter == result.end() || !iter->second) {
      ++ miss_count;
      std::string value;
      result[key] = cache_manager_proxy_->Get(key, &value, secs, use_master);
      (*values)[key] = value;
    }
  }
  InvokeClientSingleton::instance().Log("miss_multiGet", namespace_id_, biz_id_, miss_count);
  return result;
}


std::map<std::string, bool> TripodClient::MultiGet(const StringListType& keys,
    StringMapType* values, int secs) {
  return MultiGet(keys, values, false, secs);
}

std::map<std::string, bool> TripodClient::MultiGetAndIgnoreMiss(const StringListType& keys,
    StringMapType* values) {
  InvokeClientSingleton::instance().Log("multiGetAndIgnoreMiss", namespace_id_, biz_id_, keys.size());
  std::map<std::string, bool> result;
  if (!cache_proxy_) {
    MCE_ERROR("TripodClient::MultiGet() cache_proxy is NULL ");
    return result;
  }
  result = cache_proxy_->MultiGet(keys,values);
  long miss_count =0;
  BOOST_FOREACH(const std::string& key, keys) {
    std::map<std::string, bool>::iterator iter = result.find(key);
    if(iter == result.end() || !iter->second) {
      ++ miss_count;
    }
  }
  InvokeClientSingleton::instance().Log("miss_multiGetAndIngoreMiss", namespace_id_, biz_id_, miss_count);
  return result;
}

bool TripodClient::Load(const std::string& key, int secs, bool use_master) {
  InvokeClientSingleton::instance().Log("load", namespace_id_, biz_id_);
  if (!cache_manager_proxy_) {
    MCE_ERROR("TripodClient::Load() cache_manager_proxy is NULL " << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  return cache_manager_proxy_->Load(key, secs, use_master);
}

bool TripodClient::Remove(const std::string& key) {
  InvokeClientSingleton::instance().Log("remove", namespace_id_, biz_id_);
  if (!cache_manager_proxy_) {
    MCE_ERROR("TripodClient::Remove() cache_manager_proxy is NULL " << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  return cache_manager_proxy_->Remove(key);

}

bool TripodClient::GetAllHash(const std::string& key, 
    StringMapType* hash_value, int secs) {
  InvokeClientSingleton::instance().Log("getAllHash", namespace_id_, biz_id_);
  if (!cache_proxy_) {
    MCE_ERROR("TripodClient::GetAllHash() cache_proxy is NULL " << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  if (!cache_manager_proxy_) {
    MCE_ERROR("TripodClient::GetAllHash() cache_manager_proxy is NULL!!" << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  bool exists = cache_proxy_->Exists(key);
  if (exists) {
    return cache_proxy_->HGetAll(key, hash_value);  
  } else {
    return cache_manager_proxy_->GetAllHash(key, hash_value, secs, true);
  }
}

bool TripodClient::IncFixedHashValue(const std::string& key, const std::string& field, const std::string& step) {
  InvokeClientSingleton::instance().Log("incFixedHashValue", namespace_id_, biz_id_);
  if (!cache_proxy_) {
    MCE_ERROR("TripodClient::IncFixedHashValue() cache_proxy_ is NULL" << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  if (!cache_manager_proxy_) {
    MCE_ERROR("TripodClient::IncFixedHashValue() cache_manager_proxy is null!!" << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  bool exists = cache_proxy_->Exists(key);
  if (exists) {
    return cache_manager_proxy_->IncFixedHashValue(key, field, step);
  } else {
    MCE_INFO("TripodClient::IncFixedHashValue() : There is nothing about the key(" << key <<") in the cache!");
    return false;
  }
}

//
bool TripodClient::GetAllHashAndIgnoreMiss(const std::string& key, 
    StringMapType* hash_value) {
  InvokeClientSingleton::instance().Log("getAllHashAndIgnoreMiss", namespace_id_, biz_id_);
  if (!cache_proxy_) {
    MCE_ERROR("TripodClient::GetAllHashAndIgnoreMiss() cache_proxy is NULL " << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  return cache_proxy_->HGetAll(key, hash_value);
}

bool TripodClient::LoadHash(const std::string& key, int secs, bool use_master) {
  InvokeClientSingleton::instance().Log("loadHash", namespace_id_, biz_id_);
  if (!cache_manager_proxy_) {
    MCE_ERROR("TripodClient::LoadHash() cache_manager_proxy is NULL " << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  return cache_manager_proxy_->LoadHash(key, secs, use_master);
}

bool TripodClient::SetPartHash (const std::string& key, 
    const StringMapType& hash_value) {
  InvokeClientSingleton::instance().Log("setPartHash", namespace_id_, biz_id_);
  if (!cache_proxy_) {
    MCE_ERROR("TripodClient::SetPartHash() cache_proxy_ is NULL" << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  if (!cache_manager_proxy_) {
    MCE_ERROR("TripodClient::SetPartHash() cache_manager_proxy_ is NULL" << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  bool exists = cache_proxy_->Exists(key);
  if (exists) {
    return cache_manager_proxy_->SetPartHash(key, hash_value);
  } else {
    MCE_INFO("TripodClient::SetPartHash() : There is nothing about the key(" << key <<") in the cache!");
    return false;
  }
}

bool TripodClient::DeletePartHash (const std::string& key, 
    const StringListType& fields) {
  InvokeClientSingleton::instance().Log("deletePartHash", namespace_id_, biz_id_);
  if (!cache_proxy_) {
    MCE_ERROR("TripodClient::DeletePartHash() cache_proxy is NULL" << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  if (!cache_manager_proxy_) {
    MCE_ERROR("TripodClient::DeletePartHash() cache_manager_proxy is NULL" << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  bool exists = cache_proxy_->Exists(key);
  if (exists) {
    return cache_manager_proxy_->DeletePartHash(key, fields);
  } else {
    MCE_INFO("TripodClient::DeletePartHash() : There is nothing about the key(" << key <<") in the cache!");
    return false;
  }
}

bool TripodClient::GetList(const std::string& key, StringListType* value, int begin, int limit, bool use_master, int secs) {
  InvokeClientSingleton::instance().Log("getList", namespace_id_, biz_id_);
  if (!cache_proxy_) {
    MCE_ERROR("TripodClient::GetList() cache_proxy is NULL " << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  if (!cache_manager_proxy_) {
    MCE_ERROR("TripodClient::GetList() cache_manager_proxy is NULL " << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  bool cache_result = cache_proxy_->GetList(key, value, begin, limit);
  if (cache_result) {
    return cache_result;
  } else {
    //TODO: 排序
    MCE_WARN("It havn`t complete!!");
    return cache_manager_proxy_->GetList(key, value, begin, limit, secs, use_master);
  }
}

bool TripodClient::GetListAndIgnoreMiss(const std::string& key, StringListType* value, int begin, int limit) {
  InvokeClientSingleton::instance().Log("getListAndIgnoreMiss", namespace_id_, biz_id_);
  if (!cache_proxy_) {
    MCE_ERROR("TripodClient::GetListAndIgnoreMiss() cache_proxy is NULL " << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  return cache_proxy_->GetList(key, value, begin, limit);
}

bool TripodClient::SetList(const std::string& key, const StringListType& value, int secs) {
  InvokeClientSingleton::instance().Log("setList", namespace_id_, biz_id_);
  if (!cache_manager_proxy_) {
    MCE_ERROR("TripodClient::SetList() cache_manager_proxy is NULL " << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  return cache_manager_proxy_ ->SetList(key, value, secs);
}

bool TripodClient::LoadList(const std::string& key, bool use_master, int secs) {
  InvokeClientSingleton::instance().Log("loadList", namespace_id_, biz_id_);
  if (!cache_manager_proxy_) {
    MCE_ERROR("TripodClient::LoadList() cache_manager_proxy is NULL " << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  return cache_manager_proxy_->LoadList(key, secs, use_master);

}

bool TripodClient::ZAdd(const std::string& key, const StringMapType& set_with_scores) {
  InvokeClientSingleton::instance().Log("zAdd", namespace_id_, biz_id_);
  if (!cache_manager_proxy_) {
    MCE_ERROR("TripodClient::ZAdd() cache_manager_proxy is NULL " << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  return cache_manager_proxy_->ZAdd(key, set_with_scores);
}

bool TripodClient::ZRemRangeByRank(const std::string& key, int begin, int end) {
  InvokeClientSingleton::instance().Log("zRemRangeByRank", namespace_id_, biz_id_);
  if (!cache_manager_proxy_) {
    MCE_ERROR("TripodClient::ZRemRangeByRank() cache_manager_proxy is NULL " << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  return cache_manager_proxy_->ZRemRangeByRank(key, begin, end);
}

bool TripodClient::ZRemRangeByScore(const std::string& key, const std::string& min, const std::string& max) {
  InvokeClientSingleton::instance().Log("zRemRangeByScore", namespace_id_, biz_id_);
  if (!cache_manager_proxy_) {
    MCE_ERROR("TripodClient::ZRemRangeByScore() cache_manager_proxy is NULL " << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  return cache_manager_proxy_->ZRemRangeByScore(key, min, max);
}

bool TripodClient::ZRange(const std::string& key, StringListType* zset_value, int begin, int limit) {
  InvokeClientSingleton::instance().Log("zRange", namespace_id_, biz_id_);
  if (!cache_proxy_) {
    MCE_ERROR("TripodClient::ZRange() cache_proxy is NULL " << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  return cache_proxy_->ZRange(key, zset_value, begin, limit);
}

bool TripodClient::ZSet(const std::string& key, const StringMapType& set_with_scores, int secs) {
  InvokeClientSingleton::instance().Log("zSet", namespace_id_, biz_id_);
  if (!cache_manager_proxy_) {
    MCE_ERROR("TripodClient::ZSet() cache_manager_proxy_ is NULL " << " ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  return cache_manager_proxy_->ZSet(key, set_with_scores, secs);
}

int TripodClient::Size(const std::string& key) {
  InvokeClientSingleton::instance().Log("size", namespace_id_, biz_id_);
  int result = 0;
  if (!cache_proxy_) {
    MCE_ERROR("TripodClient::Size() cache_proxy is NULL " << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return 0;
  }
  result = cache_proxy_->Size(key);
  //that key is not in the redis
  if (result < 0) {
    result = 0;
  }
  return result;
}

}} // xce::tripod


