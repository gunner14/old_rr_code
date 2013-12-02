#ifndef __SYNCMESSAGE_I_HPP__
#define __SYNCMESSAGE_I_HPP__

#include <DataSync.pb.h>
#include <MessageData.h>
#include <Util.h>

namespace com {
namespace renren {
namespace datasync {

class SyncMessageI : public SyncMessage {
    public:
        SyncMessageI()
        {
        }
        //SyncMessageI(const xce::messagepipe::MessageDataPtr& data)
        SyncMessageI(const std::string& dbSource, const std::string& dbTable,
                     const std::vector<std::string>& keys, bool isRouter, 
                     const std::string& dbPattern, long version, long expire = 2147483647, bool isDel = false)
        {
            set_dbsource(dbSource);
            set_dbpattern(dbPattern);
            set_isrouter(isRouter);
            set_dbtable(dbTable);
            set_version(version);
            for(MyUtil::StrSeq::const_iterator it = keys.begin(); it != keys.end(); ++it) {
                add_keys(*it);
            }
            set_expire(expire);
            set_isdel(isDel);
        }

        void addKeys(const SyncMessageI& sm)
        {
            for(int i = 0; i < sm.keys_size(); ++i) {
                add_keys(sm.keys(i));
            }
        }
};

typedef std::vector<SyncMessageI> SyncMessageISeq; 

}
}
}

#endif
