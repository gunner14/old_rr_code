/*
 * =====================================================================================
 *
 *       Filename:  HomeFootprintProducer.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月15日 15时31分07秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#include "UserIdsProducer.h"
#include "HomeFootprintProducer.h"
#include "app/Utils/src/DBUtils.h"

using namespace com::renren::tripod;
using namespace xce::footprint;

HomeFootprintProducer::HomeFootprintProducer(int footprintSize) : footprintSize_(footprintSize) {
}

void HomeFootprintProducer::fillDataMap(DataMap& dataMap, const Statement& sql, const string& tableName) {
  vector<HomeFootprintSetPtr> ptrs;
  HomeFootprintSetSeqHandler handler(ptrs, footprintSize_);
  //warn
  QueryRunner(DESC_FOOTPRINT, CDbRServer, tableName).query(sql, handler);
  for (vector<HomeFootprintSetPtr>::const_iterator it = ptrs.begin(); it != ptrs.end(); ++it) {
    string byteArray = (*it)->serialize();
    string key = boost::lexical_cast<string>((*it)->getOwnerId());
    dataMap[key] = Data(byteArray.begin(), byteArray.end());
  }
}

DataMap HomeFootprintProducer::create(const KeySeq& keys, bool isMaster) {
  MCE_INFO("[HomeFootprintProducer::create] footprintSize = " << footprintSize_ << ", keys.size = " << keys.size());
  DataMap resultDataMap;
  vector<int> userIds;
  for (KeySeq::const_iterator it = keys.begin(); it != keys.end(); ++it) {
    userIds.push_back(boost::lexical_cast<int>(*it));
  }
  map<int, vector<int> > seperatedUserIds = xce::utils::DBUtils::seperateIdsByMod<int>(userIds, TABLE_HOME_FOOTPRINT_HEAD_MOD);
  for (map<int, vector<int> >::const_iterator it = seperatedUserIds.begin(); it != seperatedUserIds.end(); ++it) {
    Statement querySql;
    string tableName = getTablePrefix() + "_" + boost::lexical_cast<string>(it->first);
    querySql << "select " << COLUMN_OWNER << ", " << COLUMN_VISITOR << " from " << tableName 
        << " where " << COLUMN_OWNER << " in " << xce::utils::DBUtils::getIdsStr<int>(it->second);
    fillDataMap(resultDataMap, querySql, tableName);
  }
  return resultDataMap;
}

KeySeq HomeFootprintProducer::createHotKeys(const string& beginKey, int limit) {
  KeySeq resultKeySeq;
  return resultKeySeq;
}

KeySeq HomeFootprintProducer::createKeys(const string& beginKey, int limit) {
  string invokerInfo = "[HomeFootprintProducer::createKeys] footprintSize = " + boost::lexical_cast<string>(footprintSize_);
  return UserIdsProducer::createKeys(invokerInfo, beginKey, limit);
}

string HomeFootprintProducer::getTablePrefix() {
  if (footprintSize_ == CACHE_SIZE) {
    return TABLE_HOME_FOOTPRINT_HEAD;
  } else {
    return TABLE_HOME_FOOTPRINT_BODY;
  }
}
