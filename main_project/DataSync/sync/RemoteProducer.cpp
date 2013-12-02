#include "RemoteProducer.h"

namespace com {
namespace renren {
namespace datasync {

xce::messagepipe::MessageDataSeq RemoteProducer::send(const SyncMessageISeq& datas) {

    xce::messagepipe::MessageDataSeq failedDatas;
    
    SyncMessageISeq::const_iterator it = datas.begin();
  
    bool flag = false;

    while(it != datas.end()) {
        if((*it).isdel() != flag) {
            ++it;
            continue;
        }
        int count = 0;
        SyncMessageI cData = (*it);
        ++it;
        while(it != datas.end() && count != BATCH_SIZE) {
            if((*it).isdel() != flag) {
                ++it;
                continue;
            }
            cData.addKeys(*it);
            ++it;
            ++count;
        }
        std::string bytesData;
        cData.SerializeToString(&bytesData);
        std::vector<unsigned char> strVec(bytesData.begin(), bytesData.end());
        xce::messagepipe::MessageDataPtr msg(new xce::messagepipe::MessageData(REMOTE_CLUSTER, cData.dbtable(), strVec));
        if(!xce::messagepipe::Producer::getInstance()->send(LOCAL_ZKADDRESS, msg)) {
            failedDatas.push_back(msg);
        }
        if(it != datas.end()) {
            ++it;
        }else if(flag == false) {
            flag = true;
            it = datas.begin();
        }else {
            break;
        }
    }

    return failedDatas;
}

}
}
}
