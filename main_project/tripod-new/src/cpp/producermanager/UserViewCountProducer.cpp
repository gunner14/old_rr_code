/*
 * =====================================================================================
 *
 *       Filename:  UserViewCountProducer.cpp
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
#include "UserViewCountProducer.h"
#include "UserIdsProducer.h"
#include "app/FootprintUnite/share/FootprintShare.h"
#include "ServiceI.h"
#include "DbCxxPool/src/QueryRunner.h"
#include "app/Utils/src/DBUtils.h"

using namespace com::renren::tripod;
using namespace com::xiaonei::xce;
using namespace xce::footprint;

DataMap UserViewCountProducer::create(const KeySeq& keys, bool isMaster) {
  MCE_INFO("[UserViewCountProducer::create] keys.size = " << keys.size());
  DataMap resultDataMap;
  Statement sql;
  sql << "select " << COLUMN_ID << ", " << COLUMN_VIEWCOUNT << " from " << TABLE_VIEW_COUNT
      << " where " << COLUMN_ID << " in " << xce::utils::DBUtils::getIdsStr<string>(keys);
  vector<UserVisitCountInfoPtr> userVisitCountInfos;
  UserVisitCountInfoResultHandlerI handler(userVisitCountInfos);
  QueryRunner(DESC_VIEW_COUNT, CDbRServer).query(sql, handler);
  for (vector<UserVisitCountInfoPtr>::const_iterator it = userVisitCountInfos.begin(); it != userVisitCountInfos.end(); ++it) {
    string byteArray = (*it)->serialize();
    string key = boost::lexical_cast<string>((*it)->getUserId());
    resultDataMap[key] = Data(byteArray.begin(), byteArray.end());
  }
  return resultDataMap;
}

KeySeq UserViewCountProducer::createHotKeys(const string& beginKey, int limit) {
  KeySeq resultKeySeq;
  return resultKeySeq;
}

KeySeq UserViewCountProducer::createKeys(const string& beginKey, int limit) {
  return UserIdsProducer::createKeys("[UserViewCountProducer::createKeys]", beginKey, limit);
}
