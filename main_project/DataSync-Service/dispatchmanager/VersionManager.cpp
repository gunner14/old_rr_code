#include "VersionManager.h"
#include <QueryRunner.h>

namespace com {
namespace renren {
namespace datasync {

VersionManager::VersionManager() {
    MyUtil::TaskManager::instance().config(TASK_LEVEL_UPDATEVERSION, MyUtil::ThreadPoolConfig(5, 5));
}

long VersionManager::getVersion(const std::string& dbSource, const std::string& dbPattern) {
    {
        IceUtil::RWRecMutex::RLock rLock(rwMutex_);
        std::map<std::string, long>::const_iterator findIt = versions_.find(dbSource + dbPattern);
        if(findIt != versions_.end()) {
            return findIt->second;
        }
    }

    long version = getVersionFromDb(dbSource, dbPattern);

    if(version >= 0) {
        IceUtil::RWRecMutex::WLock wLock(rwMutex_);
        std::map<std::string, long>::iterator findIt = versions_.find(dbSource + dbPattern);
        if(findIt != versions_.end()) {
            if(findIt->second < version) {
                findIt->second = version;
            }
            return findIt->second;
        }else {
            versions_.insert(make_pair(dbSource + dbPattern, version));
            MyUtil::TaskManager::instance().execute(new UpdateVersionTask(*this, dbSource, dbPattern));
            return version;
        }
    }

    return -1;
}

void VersionManager::setVersion(const std::string& dbSource, const std::string& dbPattern, long version) {
    IceUtil::RWRecMutex::WLock wLock(rwMutex_);
    std::map<std::string, long>::iterator findIt = versions_.find(dbSource + dbPattern);
    if(findIt != versions_.end()) {
        if(findIt->second < version) {
            findIt->second = version;
        }
    }else {
        versions_.insert(make_pair(dbSource + dbPattern, version));
    }
}

void VersionManager::delVersion(const std::string& dbSource, const std::string& dbPattern) {
    IceUtil::RWRecMutex::WLock wLock(rwMutex_);
    std::map<std::string, long>::iterator findIt = versions_.find(dbSource + dbPattern);
    if(findIt != versions_.end()) {
        versions_.erase(findIt);
    }
}

long VersionManager::getVersionFromDb(const std::string& dbSource, const std::string& pattern) {
    MCE_DEBUG("VersionManager::getVersionFromDb dbSource " << dbSource << " dbPattern " << pattern);
    long version = -1;
    com::xiaonei::xce::ConnectionPoolPtr pool = com::xiaonei::xce::ConnectionPoolManager::instance().locate(dbSource);
    if(pool) {
        com::xiaonei::xce::ConnectionList conns = pool->getAll(com::xiaonei::xce::CDbRServer, pattern);
        if(conns.empty()) {
            MCE_WARN("VersionManager::getVersionFromDb no config for " << dbSource);
            return -1;
        }
        try {
            for(com::xiaonei::xce::ConnectionList::const_iterator it = conns.begin(); it != conns.end(); ++it) {
                com::xiaonei::xce::Statement selectSql;
                selectSql << "SELECT " << FIELD_VERSION << " FROM " << TABLE_VERSION << " WHERE " << FIELD_PKEY <<" = 0 limit 1";
                mysqlpp::Query query = (*it)->query();
                selectSql.query(query);
                mysqlpp::UseQueryResult res = query.use();
                
                long rowVersion = -1;
                while(mysqlpp::Row row = res.fetch_row()) {
                    rowVersion = (long) row[FIELD_VERSION.c_str()];
                }
               
                if(rowVersion == -1) {
                    version = -1;
                    break;
                }else 
                {
                    if(version == -1) {
                        version = rowVersion;
                    }else if(rowVersion < version) {
                        version = rowVersion;
                    }
                }
                
                if((*it)->errnum()) {
                    version = -1;
                    break;
                }
            }
        }catch(mysqlpp::Exception& e) {
            MCE_WARN("VersionManager::getVersionFromDb dbSource " << dbSource << " Exception " << e.what());
        }catch(...) {
            MCE_WARN("VersionManager::getVersionFromDb dbSource " << dbSource << " UnKnownException ");
            version = -1;
        }
        
        pool->release(conns, pattern);
    }


    return version;
}

UpdateVersionTask::UpdateVersionTask(VersionManager& versionManager, const std::string& dbSource, const std::string& dbPattern)
                    : MyUtil::Task(TASK_LEVEL_UPDATEVERSION), 
                      versionManager_(versionManager),
                      dbSource_(dbSource), 
                      dbPattern_(dbPattern) {
}

void UpdateVersionTask::handle() {
    long version = versionManager_.getVersionFromDb(dbSource_, dbPattern_);
    if(version != -1) {
        versionManager_.setVersion(dbSource_, dbPattern_, version);
        MyUtil::TaskManager::instance().execute(new UpdateVersionTask(versionManager_, dbSource_, dbPattern_));
    }else {
        versionManager_.delVersion(dbSource_, dbPattern_);
    }
}

}
}
}
