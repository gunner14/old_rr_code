#include <TripodData.h>
#include <TripodClient.h>
#include <ClientFactory.h>
#include <CacheClient.h>
#include <RedisCacheClient.h>
#include <CacheManagerClient.h>

namespace com {
namespace renren {
namespace tripod {

TripodClient::TripodClient(const std::string& zkAddress, const std::string& namespaceId, 
                        const std::string& businessId) : 
                        zkAddress_(zkAddress), namespaceId_(namespaceId), businessId_(businessId) {
}

CacheClientPtr TripodClient::getCacheClient() {
    if(cacheClient_ == NULL) {
        boost::upgrade_lock<boost::shared_mutex> lock(cacheClientMutex_);
        boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock1(lock);
        if(cacheClient_ == NULL) {
            cacheClient_ = ClientFactory::getInstance()->getCacheClient(zkAddress_);
        }
    }
    return cacheClient_;
}

CacheManagerClientPtr TripodClient::getCacheManagerClient() {
    if(cacheManagerClient_ == NULL) {
        boost::upgrade_lock<boost::shared_mutex> lock(cacheManagerClientMutex_);
        boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock1(lock);
        if(cacheManagerClient_ == NULL) {
            cacheManagerClient_ = ClientFactory::getInstance()->getCacheManagerClient(zkAddress_);
        }
    }
    return cacheManagerClient_;
}

DataMap TripodClient::get(const KeySeq& keys, KeySeq& missedKeys, const long timeout) {
    CacheClientPtr client = getCacheClient();
    if(client == NULL) {
        return DataMap();
    }

    DataMap res = client->get(keys, namespaceId_, businessId_, timeout);
    
    getMissedKeys(res, keys, missedKeys);

    return res;
}

DataMap TripodClient::getMissed(const KeySeq& keys, long expiredTime, bool useMaster, const long timeout) {
    CacheManagerClientPtr client = getCacheManagerClient();
    if(client == NULL) {
        return DataMap();
    }

    return client->get(keys, namespaceId_, businessId_, expiredTime, useMaster, timeout);
}

DataMap TripodClient::getWithMissed(const KeySeq& keys, long expiredTime, bool useMaster, const long timeout) {
    KeySeq missedKeys;
    DataMap res = get(keys, missedKeys, timeout);

    if(!missedKeys.empty()) {
        DataMap missedRes = getMissed(missedKeys, expiredTime, useMaster, timeout);
        res.insert(missedRes.begin(), missedRes.end());
    }
    return res;
}


bool TripodClient::load(const KeySeq& keys, long expiredTime, bool useMaster, const long timeout) {
    CacheManagerClientPtr client = getCacheManagerClient();
    if(client == NULL) {
        return false;
    }

    return client->load(keys, namespaceId_, businessId_, expiredTime, useMaster, timeout);
}

bool TripodClient::deleteCache(const KeySeq& keys, const long timeout) {
    CacheManagerClientPtr client = getCacheManagerClient();
    if(client == NULL) {
        return false;
    }
    
    return client->deleteCache(keys, namespaceId_, businessId_, timeout);
}

void TripodClient::getMissedKeys(const DataMap& res, const KeySeq& keys, KeySeq& missedKeys) {
    for(KeySeq::const_iterator it = keys.begin(); it != keys.end(); ++it) {
        if(res.find(*it) == res.end()) {
            missedKeys.push_back(*it);
        }
    }
}



RedisCacheClientPtr TripodClient::GetRedisCacheClient() {
  if(cacheClient_ == NULL) {
    boost::upgrade_lock<boost::shared_mutex> lock(redis_cache_client_mutex_);
    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock1(lock);
    if(redis_cache_client_ == NULL) {
       MCE_DEBUG("zkAddress_" << zkAddress_ );
       redis_cache_client_ = ClientFactory::getInstance()->GetRedisCacheClient(zkAddress_);
    }
  }
  return redis_cache_client_;
}

void TripodClient::GetList(const std::string& key, StrList& value, int begin, int limit) {
  RedisCacheClientPtr client = GetRedisCacheClient();
  if (!client) {
    MCE_WARN("TripodClient::GetList() RedisCacheClient is NULL");
    return;
  }
  const StrList& list = client->GetList(key, begin, limit, namespaceId_, businessId_);
  if (!list.empty()) {
    value.insert(value.end(), list.begin(), list.end());
  }
}

void TripodClient::GetHash(const std::string& key, const StrList& fields, HashPair& value) {
  RedisCacheClientPtr client = GetRedisCacheClient();
  if (!client) {
    MCE_WARN("TripodClient::GetHash() RedisCacheClient is NULL");
    return;
  }
  int exists = client->Exists(key, namespaceId_, businessId_);
  if (exists == kReplySucess) { //命中
    bool ret = client->GetHash(key, fields, value, namespaceId_, businessId_);
  }
  else { //未命中
    return GetMissedHash(key, fields, value);
  }
}

bool TripodClient::Exists(const std::string& key) {
  RedisCacheClientPtr client = GetRedisCacheClient();
  if (!client) {
    MCE_WARN("TripodClient::GetHash() RedisCacheClient is NULL");
    return false;
  }
  int exists = client->Exists(key, namespaceId_, businessId_);
  return (exists == kReplySucess ? true : false);
}

void TripodClient::GetMissedHash(const std::string& key, const StrList& fields, HashPair& value, bool use_master) {
  CacheManagerClientPtr client = getCacheManagerClient();
  if(client == NULL) {
    MCE_WARN("TripodClient::GetMissedHash() getCacheManagerClient is NULL");
      return;
  }
  BigKeyDataMap ret_data = client->GetHashCache(key, fields, namespaceId_, businessId_, use_master);
  if (!ret_data.empty()) {
    BigKeyDataMap::const_iterator iter = ret_data.begin();
    for (; iter != ret_data.end(); ++iter) {
      value.insert(std::make_pair(iter->first, iter->second));
    }
  }
}

bool TripodClient::LoadListCache(const std::string& key, bool use_master, const long timeout) {
  CacheManagerClientPtr client = getCacheManagerClient();
  if(client == NULL) {
      return false;
  }
  return client->LoadListCache(key, namespaceId_, businessId_, use_master);
  
}

bool TripodClient::LoadHashCache(const std::string& key, bool use_master, const long timeout) {
  CacheManagerClientPtr client = getCacheManagerClient();
  if(client == NULL) {
      return false;
  }
  return client->LoadHashCache(key, namespaceId_, businessId_, use_master);
  
}

bool TripodClient::DeleteListOrHashCache(const std::string& key, const long timeout) {
  CacheManagerClientPtr client = getCacheManagerClient();
  if(client == NULL) {
      return false;
  }
  return client->DeleteListOrHashCache(key, namespaceId_, businessId_);
}

}}}


