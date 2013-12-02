#ifndef __CONFIG_HOLDER_H__
#define __CONFIG_HOLDER_H_

#include <ServiceI.h>
#include <QueryRunner.h>

namespace com {
namespace renren {
namespace datasync {

const std::string TABLE_CONFIG = "sync_config";
const std::string DB_CONFIG = "user_config";
const std::string FIELD_TABLE = "table";
const std::string FIELD_NAMESPACE = "namespace";
const std::string FIELD_BUSINESS = "business";
const std::string FIELD_CLUSTER = "cluster";

class ConfigItem : public Ice::Object {
    public:
        ConfigItem(const std::string& dbTable, const std::string& cluster): dbTable_(dbTable), cluster_(cluster) {
        }

    public:

        void addCache(const std::string& namespaceId, const std::string& business)
        {
            cacheVec_.push_back(std::pair<std::string, std::string>(namespaceId, business));
        }
    
    public:
        std::string dbTable_;
        std::vector<std::pair<std::string, std::string> > cacheVec_;
        std::string cluster_;
};

typedef IceUtil::Handle<ConfigItem> ConfigItemPtr;

class BatchDbResultHandler : public com::xiaonei::xce::ResultHandler {
    public:
        BatchDbResultHandler(std::map<std::string, ConfigItemPtr>& config) : config_(config) {
        }
        bool handle(mysqlpp::Row& row) const
        {
            std::string table = (string) row[FIELD_TABLE.c_str()];
            std::string ns = (string) row[FIELD_NAMESPACE.c_str()];
            std::string business = (string) row[FIELD_BUSINESS.c_str()];
            std::string cluster = (string) row[FIELD_CLUSTER.c_str()]; 
            
            std::map<std::string, ConfigItemPtr>::const_iterator findIt;
            findIt = config_.find(table);
            if(findIt != config_.end()) {
                findIt->second->addCache(ns, business);
            }else {
                ConfigItemPtr nc = new ConfigItem(table, cluster);
                nc->addCache(ns, business);
                config_.insert(make_pair(table, nc));
            }
            return true;
        }
    
    private:
        std::map<std::string, ConfigItemPtr>& config_;
};

class ConfigHolder : public IceUtil::Thread {
    public:
        void run()
        {
            while(true) {
                try {
                    renew();
                }catch(...) {
                    MCE_WARN("ConfigHolder::run get Exception!");
                }

                sleep(10);
            }
        }

        void renew()
        {
            com::xiaonei::xce::Statement sql;
            sql << "SELECT * FROM " << TABLE_CONFIG;

            std::map<std::string, ConfigItemPtr> config; 
            
            BatchDbResultHandler handler(config); 
            com::xiaonei::xce::QueryRunner(DB_CONFIG, com::xiaonei::xce::CDbRServer).query(sql, handler); 
            
            std::vector<std::string> vecStr;
            for(std::map<std::string, ConfigItemPtr>::const_iterator it = config.begin(); it != config.end(); ++it) {
                vecStr.push_back(it->first);
            } 
            if(!config.empty()) {
                IceUtil::RWRecMutex::WLock wlock(rwMutex_);
                config_.swap(config);
                dbTableVec_.swap(vecStr);
            }
        }

        std::vector<std::pair<std::string, std::string> > getCache(const std::string& dbTable)
        {  
            IceUtil::RWRecMutex::RLock rLock(rwMutex_);
            std::map<std::string, ConfigItemPtr>::const_iterator findIt;
            findIt = config_.find(dbTable);
            if(findIt != config_.end()) {
                return findIt->second->cacheVec_;
            }
            MCE_WARN("ConfigHolder::get no config for dbTable " << dbTable);
            return std::vector<std::pair<std::string, std::string> >();
        }
        
        std::vector<std::string> getAllSubject()
        {
            IceUtil::RWRecMutex::RLock rLock(rwMutex_);
            return dbTableVec_;
        }

    private:
        IceUtil::RWRecMutex rwMutex_;
        std::map<std::string, ConfigItemPtr> config_;
        std::vector<std::string> dbTableVec_;
};

}
}
}

#endif
