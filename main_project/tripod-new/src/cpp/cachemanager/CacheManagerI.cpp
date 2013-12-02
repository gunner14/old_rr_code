#include "CacheManagerI.h"

#include <algorithm>
#include "boost/foreach.hpp"

#include "CacheClient.h"
#include "RedisCacheClient.h"
#include "ProducerManagerClient.h"

#include "listorhashvalue.pb.h"

#include "client/linux/handler/exception_handler.h"

bool MyCallback(const char *dump_path, const char *minidump_id, void *context,
        bool succeeded) {
  printf("got crash %s %s\n", dump_path, minidump_id);
  return true;
}

void MyUtil::initialize() {
  static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
  MCE_WARN("breakpad");
  MyUtil::ServiceI& service = MyUtil::ServiceI::instance();

  std::string zkAddress = service.getCommunicator()->getProperties()->getPropertyWithDefault("TripodCacheManager.ZkAddress", com::renren::tripod::DEFAULT_ZKADDRESS);
  MCE_WARN("ZkAddress : " << zkAddress);
  com::renren::tripod::CacheManagerI::instance().setZkAddress(zkAddress);
	
  service.getAdapter()->add(&com::renren::tripod::CacheManagerI::instance(), service.createIdentity("M", ""));
 
  MyUtil::ServiceMonitorManager::instance().start();
}

namespace com {
namespace renren {
namespace tripod {

CacheManagerI::CacheManagerI() {
}

CacheManagerI::~CacheManagerI() {
}

void CacheManagerI::setZkAddress(const std::string& zkAddress) {
    zkAddress_ = zkAddress;
}

ProducerManagerClientPtr CacheManagerI::getProducer() {
    if(producer_ == NULL) {
        IceUtil::Mutex::Lock lock(producerMutex_);
        producer_ = ClientFactory::getInstance()->getProducerManagerClient(zkAddress_);
    }
    return producer_;
}

CacheClientPtr CacheManagerI::getCache() {
    if(cache_ == NULL) {
        IceUtil::Mutex::Lock lock(cacheMutex_);
        cache_ = ClientFactory::getInstance()->getCacheClient(zkAddress_);
    }
    return cache_;
}

DataMap CacheManagerI::get(const KeySeq& keys, const std::string& namespaceId, const std::string& businessId, 
                int expiredTime, bool useMaster, const Ice::Current& current) {
    std::ostringstream os;
    os<<"Key : ";
    for(KeySeq::const_iterator it = keys.begin(); it != keys.end(); ++it) {
        os<<*it<<" ";
    }
    os<<" namespaceId :"<<namespaceId;
    os<<" businessId :"<<businessId;
    os<<" expiredTime :"<<expiredTime;
    os<<" useMaster :"<<useMaster;
    MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, os.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
    
    DataMap res;
    KeySeq inputKeys;
    
    CacheClientPtr cache = getCache();
    if(cache != NULL) {
        res = cache->get(keys, namespaceId, businessId, 300);
        for(KeySeq::const_iterator it = keys.begin(); it != keys.end(); ++it) {
            if(res.find(*it) == res.end()) {
                inputKeys.push_back(*it);
            }
        }
    }

    if(inputKeys.empty()) {
        return res;
    }

    
    ProducerManagerClientPtr producer = getProducer();
    if(producer == NULL) {
        return DataMap();
    }

    KeySeq lockedKeys;
    KeySeq failedLockedKeys;

    while(true) {
        xce::tempmutext::Locks<std::string> locks(&tempMutexManager_, inputKeys, lockedKeys, failedLockedKeys);
        if(!lockedKeys.empty()) {
            DataMap singleRes = producer->produce(keys, businessId, useMaster, 10000);
            if(!singleRes.empty()) {
                res.insert(singleRes.begin(), singleRes.end());
                CacheClientPtr cache = getCache();
                if(cache != NULL) {
                    cache->set(singleRes, namespaceId, businessId, expiredTime, 1000);
                }
            }
        
            if(failedLockedKeys.empty()) {
                break;
            }
        }
        inputKeys.swap(failedLockedKeys);
        lockedKeys.clear();
        failedLockedKeys.clear();
    }

    return res;
}

bool CacheManagerI::load(const KeySeq& keys, const std::string& namespaceId, const std::string& businessId, 
                int expiredTime, bool useMaster, const Ice::Current& current) {
    std::ostringstream os;
    os<<"Key : ";
    for(KeySeq::const_iterator it = keys.begin(); it != keys.end(); ++it) {
        os<<*it<<" ";
    }
    os<<" namespaceId :"<<namespaceId;
    os<<" businessId :"<<businessId;
    os<<" expiredTime :"<<expiredTime;
    os<<" useMaster :"<<useMaster;
    MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, os.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
    ProducerManagerClientPtr producer = getProducer();
    if(producer == NULL) {
        return false;
    }
    
    bool sucFlag = true;

    KeySeq inputKeys(keys.begin(), keys.end());
    KeySeq lockedKeys;
    KeySeq failedLockedKeys;
    
    while(true) {
        xce::tempmutext::Locks<std::string> locks(&tempMutexManager_, inputKeys, lockedKeys, failedLockedKeys);
        if(!lockedKeys.empty()) {
            DataMap singleRes = producer->produce(keys, businessId, useMaster, 10000);
       
            if(!singleRes.empty()) {
                CacheClientPtr cache = getCache();
                if(cache != NULL) {
                    sucFlag &= cache->set(singleRes, namespaceId, businessId, expiredTime, 1000);
                }
            }
            if(failedLockedKeys.empty()) {
                break;
            }
        }
        inputKeys.swap(failedLockedKeys);
        lockedKeys.clear();
        failedLockedKeys.clear();
    }

    return sucFlag;
}

bool CacheManagerI::deleteCache(const KeySeq& keys, const std::string& namespaceId, const std::string& businessId, const Ice::Current& current) {
    std::ostringstream os;
    os<<"Key : ";
    for(KeySeq::const_iterator it = keys.begin(); it != keys.end(); ++it) {
        os<<*it<<" ";
    }
    os<<" namespaceId :"<<namespaceId;
    os<<" businessId :"<<businessId;
    MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, os.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
    CacheClientPtr cache = getCache();
    if(cache == NULL) {
        return false;
    }
    
    bool sucFlag = true;

    KeySeq inputKeys(keys.begin(), keys.end());
    KeySeq lockedKeys;
    KeySeq failedLockedKeys;
    
    while(true) {
        xce::tempmutext::Locks<std::string> locks(&tempMutexManager_, inputKeys, lockedKeys, failedLockedKeys);
        if(!lockedKeys.empty()) {
            sucFlag &= cache->remove(keys, namespaceId, businessId, 1000);
            if(failedLockedKeys.empty()) {
                break;
            }
        }
        inputKeys.swap(failedLockedKeys);
        lockedKeys.clear();
        failedLockedKeys.clear();
    }

    DataMap res = cache->get(keys, namespaceId, businessId, 300);

    return res.empty();
}


bool CacheManagerI::loadListCache(const std::string& key, const std::string& namespace_id, const std::string& biz_id, bool use_master, const Ice::Current& current) {
  std::ostringstream os;
  os << "key :" << key;
  os << " namespaceId :" << namespace_id;
  os << " businessId :" << biz_id;
  os << " use_master :" << use_master;
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, os.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  ProducerManagerClientPtr producer = getProducer();
  if(!producer) {
    MCE_ERROR("CacheManagerI::loadListCache() getProducer() return NULL!!!");
    return false;
  }

  KeySeq input_keys;
  input_keys.push_back(key);
  KeySeq locked_keys;
  KeySeq failed_locked_keys;
  bool result = false;
  while(true) {
    xce::tempmutext::Locks<std::string> locks(&tempMutexManager_, input_keys, locked_keys, failed_locked_keys);
    if(!locked_keys.empty()) {
      DataMap data;
      try {
        data = producer->produce(locked_keys, biz_id, use_master, 10000);
      } catch (Ice::Exception& e) {
        MCE_ERROR("CacheManagerI::loadListCache() rpc call produce() " << e.what());
      }
      DataMap::const_iterator iter = data.begin();
      for (; iter != data.end(); ++iter) {
        if (!iter->second.empty()) {
          ListOrHashValue list_value;
          list_value.ParseFromArray(iter->second.data(), iter->second.size());
          if (list_value.values_size() > 0) {
            StrList str_list;
            BOOST_FOREACH(const std::string& v, list_value.values()) {
              str_list.push_back(v);
            }
            RedisCacheClientPtr cache = GetRedisCache();
            if (!cache) {
              MCE_ERROR("CacheManagerI::loadListCache() GetRedisCache() return NULL !!!");
              continue;
            }
            result = cache->SetList(key, str_list, namespace_id, biz_id);
          }
        }
      }
      if(failed_locked_keys.empty()) {
        break;
      }
    }
    input_keys.swap(failed_locked_keys);
    locked_keys.clear();
    failed_locked_keys.clear();
  }
  return result;
}

bool CacheManagerI::loadHashCache(const std::string& key, const std::string& namespace_id, const std::string& biz_id, bool use_master, const Ice::Current& current) {
  std::ostringstream os;
  os << "key :" << key;
  os << " namespaceId :" << namespace_id;
  os << " businessId :" << biz_id;
  os << " use_master :" << use_master;
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, os.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  ProducerManagerClientPtr producer = getProducer();
  if(!producer) {
    MCE_ERROR("CacheManagerI::loadHashCache() getProducer() return NULL!!!");
    return false;
  }

  KeySeq input_keys;
  input_keys.push_back(key);
  KeySeq locked_keys;
  KeySeq failed_locked_keys;
  bool result = false;
  while(true) {
    xce::tempmutext::Locks<std::string> locks(&tempMutexManager_, input_keys, locked_keys, failed_locked_keys);
    if(!locked_keys.empty()) {
      DataMap data;
      try {
        data = producer->produce(locked_keys, biz_id, use_master, 10000);
      } catch (Ice::Exception& e) {
        MCE_ERROR("CacheManagerI::loadHashCache() rpc call produce() " << e.what());
      }
      DataMap::const_iterator iter = data.begin();
      for (; iter != data.end(); ++iter) {
        if (!iter->second.empty()) {
          ListOrHashValue list_value;
          list_value.ParseFromArray(iter->second.data(), iter->second.size());
          if (list_value.values_size() > 0 && list_value.values_size() % 2 == 0) {
            HashPair hash_pair;
            for (size_t i = 0; i < list_value.values_size(); ++i) {
              std::string hash_key = list_value.values(i);
              std::string hash_value = list_value.values(++i);
              hash_pair.insert(std::make_pair(hash_key, hash_value));
            }
            RedisCacheClientPtr cache = GetRedisCache();
            if (!cache) {
              MCE_ERROR("CacheManagerI::loadListCache() GetRedisCache() return NULL !!!");
              continue;
            }
            result = cache->SetHash(key, hash_pair, namespace_id, biz_id);
          }
        }
        else {
          RedisCacheClientPtr cache = GetRedisCache();
          if (cache) {
            MCE_ERROR("CacheManagerI::loadListCache() GetRedisCache() return NULL !!!");
            cache->Remove(key, namespace_id, biz_id); // why? 如果曾经有，现在没有，则需要删除！
          }
          result = true; // no data does not mean unsuccess!
        }
      }
      if(failed_locked_keys.empty()) {
        break;
      }
    }
    input_keys.swap(failed_locked_keys);
    locked_keys.clear();
    failed_locked_keys.clear();
  }
  return result;
}


bool CacheManagerI::deleteListOrHashCache(const std::string& key, const std::string& namespace_id, const std::string& biz_id, const Ice::Current& current) {
  std::ostringstream os;
  os << "key :" << key;
  os << " namespaceId :" << namespace_id;
  os << " businessId :" << biz_id;
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, os.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  RedisCacheClientPtr cache = GetRedisCache();
  if (!cache) {
    MCE_ERROR("CacheManagerI::loadListCache() GetRedisCache() return NULL!!!");
    return false;
  }

  KeySeq input_keys;
  input_keys.push_back(key);
  KeySeq locked_keys;
  KeySeq failed_locked_keys;
  bool result = false;
  while(true) {
    xce::tempmutext::Locks<std::string> locks(&tempMutexManager_, input_keys, locked_keys, failed_locked_keys);
    if(!locked_keys.empty()) {
      BOOST_FOREACH(const std::string& locked_key, locked_keys) {
        result = cache->Remove(locked_key, namespace_id, biz_id);
      }
      if(failed_locked_keys.empty()) {
        break;
      }
    }
    input_keys.swap(failed_locked_keys);
    locked_keys.clear();
    failed_locked_keys.clear();
  }
  return result;
}

BigKeyDataMap CacheManagerI::getHashCache(const std::string& key, const StrList& fields, const std::string& namespace_id, const std::string& biz_id, bool use_master, const Ice::Current& current) {
  BigKeyDataMap ret_data;
  std::ostringstream os;
  os << "key :" << key;
  os << " namespaceId :" << namespace_id;
  os << " businessId :" << biz_id;
  os << " use_master :" << use_master;
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, os.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  if (fields.empty()) {
    return ret_data;
  }
  ProducerManagerClientPtr producer = getProducer();
  if(!producer) {
    MCE_ERROR("CacheManagerI::getHashCache() getProducer() return NULL!!!");
    return ret_data;
  }

  KeySeq input_keys;
  input_keys.push_back(key);
  KeySeq locked_keys;
  KeySeq failed_locked_keys;
  bool result = false;
  while(true) {
    xce::tempmutext::Locks<std::string> locks(&tempMutexManager_, input_keys, locked_keys, failed_locked_keys);
    if(!locked_keys.empty()) {
      DataMap data;
      try {
        data = producer->produce(locked_keys, biz_id, use_master, 10000);
      } catch (Ice::Exception& e) {
        MCE_ERROR("CacheManagerI::getHashCache() rpc call produce() " << e.what());
      }
      DataMap::const_iterator iter = data.begin();
      for (; iter != data.end(); ++iter) {
        if (!iter->second.empty()) {
          ListOrHashValue list_value;
          list_value.ParseFromArray(iter->second.data(), iter->second.size());
          if (list_value.values_size() > 0 && list_value.values_size() % 2 == 0) {
            HashPair hash_pair;
            for (size_t i = 0; i < list_value.values_size(); ++i) {
              std::string hash_key = list_value.values(i);
              std::string hash_value = list_value.values(++i);
              hash_pair.insert(std::make_pair(hash_key, hash_value));
              StrList::const_iterator iter = std::find(fields.begin(), fields.end(), hash_key);
              if (iter != fields.end()) {
                ret_data.insert(std::make_pair(hash_key, hash_value));
              }
            }
            RedisCacheClientPtr cache = GetRedisCache();
            if (!cache) {
              MCE_ERROR("CacheManagerI::getHashCache() GetRedisCache() return NULL !!!");
              continue;
            }
            result = cache->SetHash(key, hash_pair, namespace_id, biz_id);
          }
        }
      }
      if(failed_locked_keys.empty()) {
        break;
      }
    }
    input_keys.swap(failed_locked_keys);
    locked_keys.clear();
    failed_locked_keys.clear();
  }
  return ret_data;
}


RedisCacheClientPtr CacheManagerI::GetRedisCache() {
  if (!redis_cache_) {
    IceUtil::Mutex::Lock lock(redis_cache_mutex_);
    redis_cache_ = ClientFactory::getInstance()->GetRedisCacheClient(zkAddress_);
  }
  return redis_cache_;
}


}
}
}
