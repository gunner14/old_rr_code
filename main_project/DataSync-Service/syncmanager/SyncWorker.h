#ifndef _SYNCWORKER_H__
#define _SYNCWORKER_H__

#include <ServiceI.h>
#include <QueryRunner.h>
#include <DataSync/share/SyncMessageI.hpp>

namespace com {
namespace renren {
namespace datasync {

const std::string TABLE_VERSION = "rr_sync_version";
const std::string FIELD_VERSION = "version";
const std::string FIELD_PKEY = "id";

class SyncWorker : public MyUtil::Singleton<SyncWorker> {
    public:
        xce::messagepipe::MessageDataSeq sync(const xce::messagepipe::MessageDataSeq& datas);
    
    private:
        long incDbVersion(const std::string& dbSource, const std::string& dbPattern, bool isRouter = false);

        xce::messagepipe::MessageDataSeq send(const SyncMessageISeq& datas);
};

class VersionResultHandler : public com::xiaonei::xce::ResultHandler {
    public:
        VersionResultHandler(long& version);

    protected:
        bool handle(mysqlpp::Row& res) const;

    private:
        long& version_;
};

}
}
}
#endif
