/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月03日 14时29分47秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#include <iostream>
#include "DBWriterAdapter.h"

using namespace xce::dbmanager;
using namespace std;

void printWriteResultInfo(const WriteResultInfoPtr& ptr) {
  cout << "[insertId = " << ptr->insertId << ", affectRows = " << ptr->affectRows << ", "
       << "sqlExecuteOk = " << ptr->sqlExecuteOk << ", loadTripodCacheOk = " << ptr->loadTripodCacheOk
       << ", additionalDBInfo = " << ptr->info << "]" << endl;
}

int main(int argc, char* argv[]) {
  string sql = "update minigroup_0 set name = 'name_shangmin1', icon = 'icon_shangmin1' where id = 352261900";
  DBWriterAdapter& adapter = DBWriterAdapter::instance();
  WriteResultInfoPtr minigroupResultInfoPtr = adapter.write("minigroup", "minigroup_0", sql);
  printWriteResultInfo(minigroupResultInfoPtr);

  string likeSql = "update like_ship_13 set uid = 67917 where gid = 8000001683913 or gid = 8000201172613";
  vector<string> keys;
  keys.push_back("8000001683913");
  keys.push_back("8000201172613");
  WriteResultInfoPtr resultInfoPtr1 = adapter.writeWithKeys("ilikeit", "like_ship_13", likeSql, "test", "ILikeIt", keys, 1);
  printWriteResultInfo(resultInfoPtr1);

  likeSql = "insert into like_ship_13 values(8000001683913, 1), (8000201172613, 1)";
  string filterSql = "select distinct(gid) from like_ship_13 where gid = 8000001683913 or gid = 8000201172613";
  WriteResultInfoPtr resultInfoPtr2 = adapter.writeWithFilter("ilikeit", "like_ship_13", likeSql, "test", "ILikeIt", filterSql, "gid", 2);
  printWriteResultInfo(resultInfoPtr2);
  cout << "Test finished!" << endl;
  return 0;
}
