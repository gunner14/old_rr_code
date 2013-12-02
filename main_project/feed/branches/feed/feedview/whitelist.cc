/**********************************|
|***** Author:tinghaier   *********|
|***** Created on:Feb 2.2012*******|
|**********************************/
#include <ctime>
#include "feed/base_feed/ptime.h"
#include "whitelist.h"
#include "LogWrapper.h"
#include "QueryRunner.h"

using namespace com::xiaonei::xce;

namespace xce {
namespace feed {
int Whitelist::IsWhiteUser(int uid) {
  IceUtil::RWRecMutex::RLock lock(_mutex);
  map<int, int>::iterator iter=white_map.find(uid); 
  if(iter != white_map.end())
    return iter->second;
  return 0;
}

void Whitelist::LoadWhitelist() {
  struct timeval start, end;
  gettimeofday(&start, NULL);
  Statement sql;
  sql << "SELECT uid,view FROM feednews_whitelist limit 5000";
  mysqlpp::StoreQueryResult res = QueryRunner("feed_db", CDbRServer, 
    "feednews_whitelist").store(sql);
  map<int, int> temp_map;
  int uid, view;
  for (size_t i = 0; i < res.size(); ++i) {
    uid = (int) res.at(i)["uid"];
    view = (int) res.at(i)["view"];
    temp_map.insert(pair<int, int>(uid, view));
  }
  gettimeofday(&end, NULL);
  double timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  MCE_INFO("Whitelist::LoadWhitelist --> LoadFrom DB used time : "<<timeuse/1000000);
  IceUtil::RWRecMutex::WLock lock(_mutex);
  white_map.swap(temp_map);
  MCE_INFO("Whitelist::LoadWhitelist --> Load done.Add: " << white_map.size());
}
void WhiteInitTask::handle(){
  Whitelist::instance().LoadWhitelist();
}
}
}
