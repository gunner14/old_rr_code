#include "DispatchWorker.h" 
#include <QueryRunner.h> 
#include <CacheManagerClient.h> 
#include "boost/thread/locks.hpp"

namespace com {
namespace renren {
namespace datasync {

DispatchWorker::DispatchWorker() {
    configHolder_.renew();
    configHolder_.start().detach();
    nameSpaceFilter_.insert("xce_user");
    nameSpaceFilter_.insert("xce_network");
    nameSpaceFilter_.insert("xce_misc");
}

com::renren::tripod::CacheManagerClientPtr DispatchWorker::getCacheMangerClient() {
    if(cm_ == NULL) {
        IceUtil::Mutex::Lock lock(mutex_);
        if(cm_== NULL) {
            cm_ = com::renren::tripod::ClientFactory::getInstance()->getCacheManagerClient(TRIPOD_ZK_ADDRESS);
        }
    }
    return cm_;
}

//tripod2的client获取
TripodClientPtr DispatchWorker::getClient(const std::string &ns, const std::string &biz) {
    TripodClientMap::const_iterator it; 
    {
      boost::unique_lock<boost::shared_mutex> read_lock(clientMapMutex_);
      it = clientMap_.find(std::make_pair(ns, biz));
    }
    if(it == clientMap_.end()) {
        boost::unique_lock<boost::shared_mutex> write_lock(clientMapMutex_);
        it = clientMap_.find(std::make_pair(ns, biz));
        if(it == clientMap_.end()) {
            TripodClientPtr client(new xce::tripod::TripodClient(ns, biz));
            clientMap_[std::make_pair(ns, biz)] = client;
            it = clientMap_.find(std::make_pair(ns, biz));
        }
    }
    return it->second;
}

SyncMessageISeq DispatchWorker::dispatch(const SyncMessageISeq& datas) {
    
    SyncMessageISeq failedDatas;
    SyncMessageISeq dispatchDatas;

    std::map<std::string, SyncMessageISeq> dbSourceMap;
    
    for(SyncMessageISeq::const_iterator it = datas.begin(); it != datas.end(); ++it) {
        std::map<std::string, SyncMessageISeq>::iterator findIt = dbSourceMap.find((*it).dbsource() + (*it).dbpattern());
        if(findIt == dbSourceMap.end()) {
            SyncMessageISeq tempSeq;
            tempSeq.push_back(*it);
            dbSourceMap.insert(make_pair((*it).dbsource() + (*it).dbpattern(), tempSeq));
        }else {
            findIt->second.push_back(*it);
        }
    }
    
    for(std::map<std::string, SyncMessageISeq>::const_iterator it = dbSourceMap.begin(); it != dbSourceMap.end(); ++it) {
        long version = 0;
        try {
            SyncMessageI data = *(it->second.begin());
            version = versionManager_.getVersion(data.dbsource(), data.dbpattern());
        }catch (mysqlpp::Exception& e) {
            MCE_WARN("DispatchWorker::dispatch Exception dbSource " << it->first << " " << e.what());
            failedDatas.insert(failedDatas.end(), it->second.begin(), it->second.end());
            continue;
        }
        
        for(SyncMessageISeq::const_iterator iIt = it->second.begin(); iIt != it->second.end(); ++iIt) {
            if((*iIt).version() <= version) {
                dispatchDatas.push_back(*iIt);
            }else {
                MCE_WARN("source " << (*iIt).dbsource() << " version msg " << (*iIt).version() << " db " << version);
                failedDatas.push_back(*iIt);
            }
        }
    }

    if(!dispatchDatas.empty()) {
        SyncMessageISeq tempFailedDatas = notify(dispatchDatas);
        if(!tempFailedDatas.empty()) {
            failedDatas.insert(failedDatas.end(), tempFailedDatas.begin(), tempFailedDatas.end());
        }
    }

    return failedDatas;
}

SyncMessageISeq DispatchWorker::notify(const SyncMessageISeq& datas) {

    SyncMessageISeq failedDatas;

    for(SyncMessageISeq::const_iterator it = datas.begin(); it != datas.end(); ++it) {

        std::vector<std::string> keys;
        for(int i = 0; i < (*it).keys_size(); ++i) {
            keys.push_back((*it).keys(i));
        }

        std::vector<std::pair<std::string, std::string> > vec = configHolder_.getCache((*it).dbtable());
        for(std::vector<std::pair<std::string, std::string> >::const_iterator iIt = vec.begin(); iIt != vec.end(); ++iIt) {
            //用一个Set来区分tripod2的cache
            if(nameSpaceFilter_.find((*iIt).first) != nameSpaceFilter_.end()) {
                MCE_DEBUG("DispatchWorker::notify tripod2 ns: " << (*iIt).first << " biz: " << (*iIt).second);
                bool success = true;
                TripodClientPtr client = getClient((*iIt).first, (*iIt).second);
                for(std::vector<std::string>::const_iterator itKey = keys.begin(); itKey != keys.end(); ++itKey) {
                    success &= client->Load(*itKey, 0, false);
                    if(!success) {
                        MCE_WARN("DispatchWorker::notify tripod2 failed, key: " << *itKey 
                            << " ns: " << (*iIt).first << " biz: " << (*iIt).second);
                        failedDatas.push_back(*it);
                        break;
                    }
                }
            } else {
                if((*it).isdel()) {
                    if(!getCacheMangerClient()->deleteCache(keys, (*iIt).first, (*iIt).second, 10000)) {
                        MCE_WARN("DispatchWorker::notify failed to delete for namespace " << (*iIt).first << " business " << (*iIt).second);
                        failedDatas.push_back(*it);
                        break;
                    }
                }else {
                    if(!getCacheMangerClient()->load(keys, (*iIt).first, (*iIt).second, (*it).expire(), false, 10000)) {
                        MCE_WARN("DispatchWorker::notify failed to load for namespace " << (*iIt).first << " business " << (*iIt).second);
                        failedDatas.push_back(*it);
                        break;
                    }
                }
            }
        }
    }
    return failedDatas;
}

}
}
}
