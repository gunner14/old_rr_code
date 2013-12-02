#include <ServiceI.h>
#include "SyncWorker.h" 
#include "RemoteProducer.h"

namespace com {
namespace renren {
namespace datasync {

xce::messagepipe::MessageDataSeq SyncWorker::sync(const xce::messagepipe::MessageDataSeq& datas) {

    xce::messagepipe::MessageDataSeq failedDatas;
    SyncMessageISeq sucessedDatas;

    std::map<std::string, SyncMessageISeq> dbSourceMap;
    std::map<std::string, xce::messagepipe::MessageDataSeq> dbSourceMsgMap;
    for(xce::messagepipe::MessageDataSeq::const_iterator it = datas.begin(); it != datas.end(); ++it) {
        SyncMessageI sm;
        std::string smStr((*it)->getData().begin(), (*it)->getData().end());
        sm.ParseFromString(smStr);

        MCE_WARN("source: "  << sm.dbsource() << " table: " << sm.dbtable() << " version: " 
            << sm.version());

        std::map<std::string, SyncMessageISeq>::iterator findIt = dbSourceMap.find(sm.dbsource() + sm.dbpattern());
        if(findIt == dbSourceMap.end()) {
            SyncMessageISeq tempSeq;
            tempSeq.push_back(sm);
            dbSourceMap.insert(make_pair(sm.dbsource() + sm.dbpattern(), tempSeq));
            xce::messagepipe::MessageDataSeq tempMsgSeq;
            tempMsgSeq.push_back(*it);
            dbSourceMsgMap.insert(make_pair(sm.dbsource() + sm.dbpattern(), tempMsgSeq));
        }else {
            findIt->second.push_back(sm);
            dbSourceMsgMap[sm.dbsource() + sm.dbpattern()].push_back(*it);
        }
    }

    for(std::map<std::string, SyncMessageISeq>::iterator it = dbSourceMap.begin(); it != dbSourceMap.end(); ++it) {
        long version = 0;
        try {
            SyncMessageI data = *(it->second.begin());
            version = incDbVersion(data.dbsource(), data.dbpattern(), data.isrouter());
        }catch(mysqlpp::Exception& e) {
            MCE_WARN("SyncWorker::sync Exception dbSource " << it->first << " " << e.what());
            failedDatas.insert(failedDatas.end(), dbSourceMsgMap[it->first].begin(), dbSourceMsgMap[it->first].end());
            continue;
        }
        
        for(SyncMessageISeq::iterator iIt = it->second.begin(); iIt != it->second.end(); ++iIt) {
            (*iIt).set_version(version);
        }
        sucessedDatas.insert(sucessedDatas.end(), it->second.begin(), it->second.end());
    }

    if(!sucessedDatas.empty()) {
        xce::messagepipe::MessageDataSeq sendFailedDatas = send(sucessedDatas);
        if(!sendFailedDatas.empty()) {
            failedDatas.insert(failedDatas.end(), sendFailedDatas.begin(), sendFailedDatas.end());
        }
    }

    return failedDatas;
}

xce::messagepipe::MessageDataSeq SyncWorker::send(const SyncMessageISeq& datas) {
    return RemoteProducer::instance().send(datas);
}

long SyncWorker::incDbVersion(const std::string& dbSource, const std::string& dbPattern, bool isRouter) {
    com::xiaonei::xce::Statement updateSql;
    updateSql << "UPDATE " << TABLE_VERSION << " SET " << FIELD_VERSION << " = " << FIELD_VERSION << " + 1 WHERE " << FIELD_PKEY <<" = 0";
    if(isRouter) {
        com::xiaonei::xce::QueryRunner(dbSource, com::xiaonei::xce::CDbWServer, dbPattern).execute(updateSql);
    }else {
        com::xiaonei::xce::QueryRunner(dbSource, com::xiaonei::xce::CDbWServer).execute(updateSql);
    }
  
    long version = 0;
    VersionResultHandler handler(version);
    com::xiaonei::xce::Statement selectSql;
    selectSql << "SELECT " << FIELD_VERSION << " FROM " << TABLE_VERSION << " WHERE " << FIELD_PKEY <<" = 0 limit 1";
    if(isRouter) {
        com::xiaonei::xce::QueryRunner(dbSource, com::xiaonei::xce::CDbWServer, dbPattern).query(selectSql, handler);
    }else {
        com::xiaonei::xce::QueryRunner(dbSource, com::xiaonei::xce::CDbWServer).query(selectSql, handler);
    }
    return version;
}

VersionResultHandler::VersionResultHandler(long& version) : version_(version) {
}

bool VersionResultHandler::handle(mysqlpp::Row& res) const {
    version_ = res[FIELD_VERSION.c_str()];
    return true;
}

}
}
}
