/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月13日 17时58分51秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#include <iostream>
#include "app/MenuCache/MenuTripod/MenuTripodShare.h"
#include "DbCxxPool/src/QueryRunner.h"

using namespace MyUtil;
using namespace std;
using namespace xce::menutripod;
using namespace xce::menu;
using namespace com::xiaonei::xce;

CacheType getCacheTypeWithStr(const string& cacheTypeStr) {
  if (cacheTypeStr == "Recent") {
    return RECENTLY;
  } else {
    return FAVORITE;
  }
}

CacheType getCacheType() {
  cout << "> input cacheType, Recent or Favorite : ";
  string cacheTypeStr;
  cin >> cacheTypeStr;
  return getCacheTypeWithStr(cacheTypeStr);
}

int getUserId() {
  cout << "> input userId : ";
  int userId;
  cin >> userId;
  return userId;
}

void setDBSourceAndTableName(int userId, CacheType cacheType, string& dbSource, string& tableName) {
  dbSource = TripodHelper::getDBSource(cacheType);
  tableName = TripodHelper::getTablePrefix(cacheType) + boost::lexical_cast<string>(userId % TABLE_CLUSTER);
}

int main(int argc, char* argv[]) {
  ConnectionPoolManager::instance().initialize();
  while (true) {
    cout << "> input get, set or exit : ";
    string cmd, dbSource, tableName;
    cin >> cmd;
    if (cmd == "get") {
      CacheType cacheType = getCacheType();
      int userId = getUserId();
      setDBSourceAndTableName(userId, cacheType, dbSource, tableName);
      Statement sql;
      vector<MenuTripodDataPtr> ptrs;
      sql << "select user_id, app_ids from " << tableName << " where user_id = " << userId;
      QueryRunner(dbSource, CDbRServer, tableName).query(sql, BatchMenuTripodDataHandler(ptrs, cacheType));
      if (!ptrs.empty()) {
        cout << "> appIds : ";
        vector<int> appIds = (*(ptrs.begin()))->getAppIds();
        for (vector<int>::const_iterator it = appIds.begin(); it != appIds.end(); ++it) {
          cout << " " << *it;
        }
        cout << endl;
      } else {
        cout << "> no appIds" << endl;
      }
    } else if (cmd == "set") {
      CacheType cacheType = getCacheType();
      int userId = getUserId();
      setDBSourceAndTableName(userId, cacheType, dbSource, tableName);
      Statement sql;
      vector<int> appIds;
      cout << "> input appIds(0表示结束) ：";
      int tempAppId;
      while (cin >> tempAppId && tempAppId) {
        appIds.push_back(tempAppId);
      }
      sql << "insert into " << tableName << "(user_id, app_ids) values(" << userId << ", " 
          << mysqlpp::quote << IntegerSeqSerialize<int>::serialize(appIds) << ")"
          << " on duplicate key update app_ids = values(app_ids)";
      QueryRunner(dbSource, CDbWServer, tableName).execute(sql);
    } else if (cmd == "exit") {
      cout << "> Bye!" << endl;
      break;
    } else {
      cout << "> Wrong cmd, try again" << endl;
    }
  }
  return 0;
}
