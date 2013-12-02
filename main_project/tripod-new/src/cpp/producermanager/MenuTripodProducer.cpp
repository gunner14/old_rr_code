/*
 * =====================================================================================
 *
 *       Filename:  MenuTripodProducer.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011年12月13日 17时03分15秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#include "MenuTripodProducer.h"
#include "app/Utils/src/DBUtils.h"
#include "app/MenuCache/MenuTripod/MenuTripodShare.cpp"
#include "MenuTripodData.pb.h"
#include "ServiceI.h"
#include "DbCxxPool/src/QueryRunner.h"
#include "UserIdsProducer.h"

using namespace xce::menutripod;
using namespace com::renren::tripod;
using namespace com::xiaonei::xce;

void MenuTripodProducer::fillDataMap(DataMap& dataMap, const Statement& sql, CacheType cacheType, const string& tableName) {
  vector<MenuTripodDataPtr> ptrs;
  BatchMenuTripodDataHandler handler(ptrs, cacheType);
  //warn
  QueryRunner(TripodHelper::getDBSource(cacheType), CDbRServer, tableName).query(sql, handler);
  for (vector<MenuTripodDataPtr>::const_iterator it = ptrs.begin(); it != ptrs.end(); ++it) {
    string byteArray = (*it)->serialize();
    string key = boost::lexical_cast<string>((*it)->getUserId());
    dataMap[key] = Data(byteArray.begin(), byteArray.end());
  }
}

DataMap MenuTripodProducer::create(const KeySeq& keys, bool isMaster) {
  string cacheTypeStr = TripodHelper::getCacheTypeStr(cacheType_);
  MCE_INFO("[MenuTripodProducer::create] cacheType = " << cacheTypeStr << ", keys.size = " << keys.size());
  map<CacheType, vector<int> > cacheTypeToUserIds = TripodHelper::getCacheTypeToUserIds(keys);

  DataMap resultDataMap;
  vector<int> userIds;
  for (KeySeq::const_iterator it = keys.begin(); it != keys.end(); ++it) {
    userIds.push_back(boost::lexical_cast<int>(*it));
  }
  map<int, vector<int> > seperatedUserIds = xce::utils::DBUtils::seperateIdsByMod<int>(userIds, TABLE_CLUSTER);
  for (map<int, vector<int> >::const_iterator it = seperatedUserIds.begin(); it != seperatedUserIds.end(); ++it) {
    Statement querySql;
    string tableName = TripodHelper::getTablePrefix(cacheType_) + boost::lexical_cast<string>(it->first);
    querySql << "select user_id, app_ids from " << tableName << " where user_id in "
        << xce::utils::DBUtils::getIdsStr<int>(it->second);
    fillDataMap(resultDataMap, querySql, cacheType_, tableName);
  }
  MCE_INFO("[MenuTripodProducer::create] cacheType = " << cacheTypeStr << ", keys.size = " << keys.size() << ", result.size = " << resultDataMap.size());
  return resultDataMap;
}

KeySeq MenuTripodProducer::createHotKeys(const string& beginKey, int limit) {
  KeySeq resultKeySeq;
  return resultKeySeq;
}

KeySeq MenuTripodProducer::createKeys(const string& beginKey, int limit) {
  string invokerInfo = "[MenuTripodProducer::createKeys] cacheType = " + TripodHelper::getCacheTypeStr(cacheType_);
  return UserIdsProducer::createKeys(invokerInfo, beginKey, limit);
}
