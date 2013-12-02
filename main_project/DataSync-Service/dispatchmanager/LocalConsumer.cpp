#include "LocalConsumer.h"

#include <Producer.h>

namespace com {
namespace renren {
namespace datasync {

LocalConsumer::LocalConsumer(const std::string& zkAddress, const std::string cluster, 
                             const std::string& subject) :
                             xce::messagepipe::Consumer(zkAddress, cluster, subject) {
}

void LocalConsumer::onMessage(const xce::messagepipe::MessageDataSeq& receiveMessages) {
    SyncMessageISeq smSeq = getSyncMessage(receiveMessages);
    SyncMessageISeq failedSmSeq = DispatchWorker::instance().dispatch(smSeq);
    if(!failedSmSeq.empty()) {
        xce::messagepipe::MessageDataSeq failedMsgSeq = getMessageData(failedSmSeq);
        xce::messagepipe::Producer::getInstance()->sendBatch(zk_address_, failedMsgSeq);
    }
}

SyncMessageISeq LocalConsumer::getSyncMessage(const xce::messagepipe::MessageDataSeq& datas) {
    SyncMessageISeq smSeq;
    for(xce::messagepipe::MessageDataSeq::const_iterator it = datas.begin(); it != datas.end(); ++it) {
        SyncMessageI sm;
        std::string smStr((*it)->getData().begin(), (*it)->getData().end());
        sm.ParseFromString(smStr);
        smSeq.push_back(sm);
    }
    return smSeq;
}

xce::messagepipe::MessageDataSeq LocalConsumer::getMessageData(const SyncMessageISeq& datas) {
    xce::messagepipe::MessageDataSeq mdSeq;
    for(SyncMessageISeq::const_iterator it = datas.begin(); it != datas.end(); ++it) {
        std::string smStr;
        (*it).SerializeToString(&smStr);
        std::vector<unsigned char> smBytes(smStr.begin(), smStr.end());
        xce::messagepipe::MessageDataPtr md(new xce::messagepipe::MessageData(getCluster(), (*it).dbtable(), smBytes));
        mdSeq.push_back(md);
    }
    return mdSeq;
}
}
}
}
