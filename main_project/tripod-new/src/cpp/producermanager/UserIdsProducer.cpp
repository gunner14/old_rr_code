/*
 * =====================================================================================
 *
 *       Filename:  UserIdsProducer.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月15日 16时04分49秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#include "UserIdsProducer.h"
#include "app/Utils/src/DBUtils.h"
#include "ServiceI.h"
#include "DbCxxPool/src/QueryRunner.h"

using namespace com::xiaonei::xce;
using namespace com::renren::tripod;

KeySeq UserIdsProducer::createKeys(const string& invokerInfo, const string& beginKey, int limit) {
  MCE_INFO(invokerInfo << ", beginKey = " << beginKey << ", limit = " << limit);
  KeySeq resultKeySeq;
  Statement querySql;
  querySql << "select " << COLUMN_USERID << " from " << TABLE_USER_PASSPORT << " where " << COLUMN_USERID
    << " >= " << beginKey << " limit " << limit;
  vector<int> userIds;
  xce::utils::BatchIdResultHandler<int> handler(COLUMN_USERID, userIds);
  QueryRunner(DB_DESC_USERPASSPORT, CDbRServer).query(querySql, handler);
  for (vector<int>::const_iterator it = userIds.begin(); it != userIds.end(); ++it) {
    string userIdStr = boost::lexical_cast<string>(*it);
    resultKeySeq.push_back(userIdStr);
  }
  MCE_INFO(invokerInfo << ",beginKey = " << beginKey << ", limit = " << limit
      << ", result.size = " << resultKeySeq.size());
  return resultKeySeq;
}
