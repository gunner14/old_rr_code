#ifndef _VERSIONMANAGER_H__
#define _VERSIONMANAGER_H__

#include <IceUtil/Mutex.h>
#include <TaskManager.h>

namespace com {
namespace renren {
namespace datasync {

const std::string TABLE_VERSION = "rr_sync_version";
const std::string FIELD_VERSION = "version";
const std::string FIELD_PKEY = "id";

const int TASK_LEVEL_UPDATEVERSION = 101;

class VersionManager {
    public:
        VersionManager();
    
    public:
        long getVersion(const std::string& dbSource, const std::string& dbPattern);

        void setVersion(const std::string& dbSource, const std::string& dbPattern, long version);
        
        void delVersion(const std::string& dbSource, const std::string& dbPattern);

        long getVersionFromDb(const std::string& dbSource, const std::string& dbPattern);

    private:
        IceUtil::RWRecMutex rwMutex_;
        std::map<std::string, long> versions_;
};

class UpdateVersionTask : public MyUtil::Task {
    public:
        UpdateVersionTask(VersionManager& versionManager, const std::string& dbSource, const std::string& dbPattern);

    public:
        void handle();

    private:
        VersionManager& versionManager_;
        std::string dbSource_;
        std::string dbPattern_;
};

}
}
}

#endif
