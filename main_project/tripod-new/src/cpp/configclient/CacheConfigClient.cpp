#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>

#include <Common.h>
#include <ConfigData.h>
#include <CacheConfigClient.h>

namespace com {
namespace renren {
namespace tripod {

CacheConfigClient::CacheConfigClient(const std::string& zkAddress) : ConfigClient(zkAddress, TRIPOD_MANAGER) {
}

bool CacheConfigClient::update(const ConfigDataPtr& newConfig) {
    MCE_INFO("CacheConfigClient::update");
    std::map<std::string, ConfigDataPtr>::const_iterator findIt = newConfig->childrenDatas_.find(TRIPOD_CACHE);
    if(findIt == newConfig->childrenDatas_.end()) {
        MCE_WARN("CacheConfigClient::update no cache in config!");
        return false;
    }
    
    std::map<std::string, MemcachedClientPoolHashHolderPtr> newHashHolderMap;
    
    for(std::map<std::string, ConfigDataPtr>::const_iterator it = findIt->second->childrenDatas_.begin();
            it != findIt->second->childrenDatas_.end(); ++it) {
        std::map<std::string, MemcachedClientPoolPtr> memcachedClients;
        for(std::map<std::string, ConfigDataPtr>::const_iterator iIt = it->second->childrenDatas_.begin();
                iIt != it->second->childrenDatas_.end(); ++iIt) { 

            std::vector<std::string> parts;
            boost::algorithm::split(parts, iIt->second->name_, boost::algorithm::is_any_of(":"));

            int pSize = parts.size();
           
            std::string hashStr;
            std::string address;
            int port;

            if(pSize == 2) {
                hashStr = iIt->second->name_;
                address = parts[0];
                port = boost::lexical_cast<int>(parts[1]); 
                
            } else if (pSize >= 3) {
                for(int i = 0; i < pSize - 2; ++i) {
                    hashStr += parts[i];
                    if(i != pSize - 3) {
                        hashStr += ":";
                    }
                }
                address = parts[pSize - 2];
                port = boost::lexical_cast<int>(parts[pSize - 1]); 
                
            } else {
                std::ostringstream oss;
                oss << "CacheConfigClient::update Exception the config string" << iIt->second->name_
                    << " not match the format: <host>:<port>>";
                MCE_WARN(oss.str());
                return false;
            }
            
            MemcachedClientPoolPtr mcpp(new MemcachedClientPool(address, port)); 

            memcachedClients.insert(make_pair(hashStr, mcpp));
        }
        MemcachedClientPoolHashHolderPtr mcphhp(new HashHolder<MemcachedClientPoolPtr>);
        mcphhp->renew(memcachedClients);
        newHashHolderMap[it->second->name_] = mcphhp;
    }
    {
        boost::upgrade_lock<boost::shared_mutex> lock(hashHolderMaplock_);
        boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock1(lock);
        hashHolderMap_.swap(newHashHolderMap);
    }
    return true;
}

std::map<MemcachedClientPoolPtr, KeySeq> CacheConfigClient::getPrxToKeysMap(const KeySeq& keys, const std::string& namespaceId) {
    MemcachedClientPoolHashHolderPtr mcphhp;
    {
        boost::shared_lock<boost::shared_mutex> lock(hashHolderMaplock_);
        std::map<std::string, MemcachedClientPoolHashHolderPtr>::const_iterator findIt = hashHolderMap_.find(namespaceId);
        if(findIt == hashHolderMap_.end()) {
            return std::map<MemcachedClientPoolPtr, KeySeq>();
        }
        mcphhp = findIt->second;
    }
    return mcphhp->get(keys);
}

std::map<MemcachedClientPoolPtr, DataMap> CacheConfigClient::getPrxToDataMap(const DataMap& data, const std::string& namespaceId) {
    MemcachedClientPoolHashHolderPtr mcphhp;
    {
        boost::shared_lock<boost::shared_mutex> lock(hashHolderMaplock_);
        std::map<std::string, MemcachedClientPoolHashHolderPtr>::const_iterator findIt = hashHolderMap_.find(namespaceId);
        if(findIt == hashHolderMap_.end()) {
            return std::map<MemcachedClientPoolPtr, DataMap>();
        }
        mcphhp = findIt->second;
    }
    return mcphhp->get(data);
}

MemcachedClientPoolPtr CacheConfigClient::getPrx(const std::string& key, const std::string& namespaceId) {
    MemcachedClientPoolHashHolderPtr mcphhp;
    {
        boost::shared_lock<boost::shared_mutex> lock(hashHolderMaplock_);
        std::map<std::string, MemcachedClientPoolHashHolderPtr>::const_iterator findIt = hashHolderMap_.find(namespaceId);
        if(findIt == hashHolderMap_.end()) {
            return MemcachedClientPoolPtr();
        }
        mcphhp = findIt->second;
    }
    return mcphhp->get(key);
}

std::map<std::string, MemcachedClientPoolPtr> CacheConfigClient::getOriginalPrxMap() {
    std::map<std::string, MemcachedClientPoolPtr> res;
    {
        boost::shared_lock<boost::shared_mutex> lock(hashHolderMaplock_);
        for(std::map<std::string, MemcachedClientPoolHashHolderPtr>::const_iterator it = hashHolderMap_.begin(); it != hashHolderMap_.end(); ++it) {
            std::map<std::string, MemcachedClientPoolPtr> singleRes = it->second->getOriginalPrxMap();
            res.insert(singleRes.begin(), singleRes.end());
        }
    }
    return res;
}

KeySeq CacheConfigClient::getFilterKeys(const KeySeq& keys, const std::vector<std::string>& prxVec, const std::string& namespaceId) {
    {
        boost::shared_lock<boost::shared_mutex> lock(hashHolderMaplock_);
        std::map<std::string, MemcachedClientPoolHashHolderPtr>::const_iterator findIt = hashHolderMap_.find(namespaceId);
        if(findIt == hashHolderMap_.end()) {
            return KeySeq();
        }
        return findIt->second->get(keys, prxVec);
    }
}

}
}
}
