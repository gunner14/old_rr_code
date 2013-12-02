/*
 * FrequencyCheck.cpp
 *
 * module role : 用户发送edm信息记录及频率检测
 * Created on: 2012-2-14
 * Author: shuangshuang
 */

#include "FrequencyCheck.h"
#include "QueryRunner.h"
#include "LogWrapper.h"
#include <boost/lexical_cast.hpp>
#include <string>
#include "Date.h"
#include "ServiceI.h"
#include "ParametersController.h"

namespace xce {
namespace ad {

static const int kDaySeconds = 24*60*60;
//static const int kDaySeconds = 2*60;   //  test for 2 min
static const int kEveryLoadNum = 100000;
//static const string DB_INSTANCE = EngineConfig::instance().db_source();
//static const string DB_INSTANCE = "ads_db";
static const string DB_INSTANCE = "edmsend_db";
static const std::string kWeeklySendedTableName = "ad_edm_sended_weekly";
static const std::string kDailySendedTableName = "ad_edm_sended_daily";

FrequencyCache::FrequencyCache() : weekly_load_sign_(false),daily_load_sign_(false) {
}

bool FrequencyCache::LoadFromDb() {
  timeval start_time,end_time;
  gettimeofday(&start_time,NULL);
  weekly_load_sign_ = LoadWeeklyData();
  daily_load_sign_ = LoadDailyData();
  gettimeofday(&end_time,NULL);
  MCE_DEBUG("LoadSendedEdmFromDb spend time:" << (end_time.tv_sec - start_time.tv_sec)* 1000000 + end_time.tv_usec - start_time.tv_usec);
  TaskManager::instance().scheduleRepeated(new CacheCleaner(10 * 60));
  //TaskManager::instance().scheduleRepeated(new CacheCleaner(1 * 60)); // for test
  MCE_DEBUG("LoadSendedEdmFromDb two map size: "<< user_weekly_send_time_.size() << "  " << user_daily_send_time_.size());
  UpdateCache(); 
  return true; 
}

bool FrequencyCache::Add(int user_id, SendType send_type, time_t update_time) {
  IceUtil::RWRecMutex::WLock lock(mutex_);
  switch (send_type) {
    case kWeekly:
      user_weekly_send_time_[user_id] = update_time;
      break;
    case kDaily:
      user_daily_send_time_[user_id] = update_time;
      break;
    default:
      MCE_WARN("send_type_error user_id "<<user_id<<" send_type "<<send_type);   
  }  
  return true;
}

bool FrequencyCache::WeeklySendValidate(int user_id) {
  IceUtil::RWRecMutex::RLock lock(mutex_);
  UidTimeMap::iterator it = user_weekly_send_time_.find(user_id);
  if(it != user_weekly_send_time_.end()) {
    return false;
    /*time_t now = time(NULL);
    if (now - it->second < kDaySeconds * 7) {
      return false;
    }*/ 
  }
  return true;
}

bool FrequencyCache::DailySendValidate(int user_id) {
  IceUtil::RWRecMutex::RLock lock(mutex_);
  UidTimeMap::iterator it = user_daily_send_time_.find(user_id);
  if(it != user_daily_send_time_.end()) {
    return false;
    /*time_t now = time(NULL);
    if (now - it->second < kDaySeconds) {
      return false;
    }*/
  }
  return true;
}

int FrequencyCache::CheckFrequencyLimit(int user_id) {
  int limit_check = 0;
  if (!weekly_load_sign_) {
    return 0;
  } else if (!daily_load_sign_) {
    if (WeeklySendValidate(user_id)) {
      limit_check += 1;
    }   
  } else {
    if (WeeklySendValidate(user_id)) {
      limit_check += 1;
    }
    if (DailySendValidate(user_id)) {
      limit_check += 2;
    }   
  }
  return limit_check;
}

void FrequencyCache::UpdateCache() {
  timeval start_time,end_time;
  gettimeofday(&start_time,NULL);
  MCE_DEBUG("UpdateCache begin two map size: "<< user_weekly_send_time_.size() << "  " << user_daily_send_time_.size());
  IceUtil::RWRecMutex::WLock lock(mutex_);
  time_t now = time(NULL);
  for (UidTimeMap::iterator it = user_weekly_send_time_.begin(); it != user_weekly_send_time_.end();) {
    if (now - it->second > kDaySeconds * 7) {
      user_weekly_send_time_.erase(it++);
    } else {
      ++it;
    }
  }
  now = time(NULL);
  for (UidTimeMap::iterator it = user_daily_send_time_.begin(); it != user_daily_send_time_.end();) {
    if (now - it->second > kDaySeconds) {
      user_daily_send_time_.erase(it++);
    } else {
      ++it;
    }
  }
  gettimeofday(&end_time,NULL);
  MCE_DEBUG("UpdateCache end two map size: "<< user_weekly_send_time_.size()<< "  " << user_daily_send_time_.size());
  MCE_DEBUG("UpdateCache number: " << user_weekly_send_time_.size() + user_daily_send_time_.size() << " spend time:" << (end_time.tv_sec - start_time.tv_sec)* 1000000 + end_time.tv_usec - start_time.tv_usec);
}

/*void FrequencyCache::GetNDaysAgoTime(int n_day,string &need_time) {  // 获取n天前的时间 样式 ： 2012-02-14 05:21:21
  time_t now_time = time(NULL);
  now_time -= kDaySeconds * n_day;
  char n_days_ago_time[32];
  strftime(n_days_ago_time, sizeof(n_days_ago_time), "%Y-%m-%d %H:%M:%S", localtime(&now_time));
  need_time = n_days_ago_time;
}*/

bool FrequencyCache::LoadWeeklyData() {
  com::xiaonei::xce::Statement sql;
  Ice::Long now_min_id = 0;
  int start_time = time(NULL) - 7 * kDaySeconds;
  sql << "select min(id) as min_id from " << kWeeklySendedTableName << " where send_time_int > " << start_time;
  try {
    mysqlpp::StoreQueryResult res =  com::xiaonei::xce::QueryRunner(DB_INSTANCE, com::xiaonei::xce::CDbRServer, kWeeklySendedTableName).store(sql);  //注意数据库变更
    if(res.num_rows() == 0) {
      return true;
    }
    mysqlpp::Row row = res.at(0);
    now_min_id = boost::lexical_cast<Ice::Long> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["min_id"]);
  } catch (Ice::Exception& e) {
    MCE_WARN("read max id from db " << kWeeklySendedTableName << " failed, ice err->" << e);
  } catch (std::exception& ex) {
    MCE_WARN("read max id from db " << kWeeklySendedTableName << " failed, err->" << ex.what());
  } catch(...){
    MCE_WARN("read max id from db " << kWeeklySendedTableName << " failed");
  }
  Ice::Long now_max_id = 0;
  sql.clear();
  sql << "select max(id) as max_id from " << kWeeklySendedTableName ;
  try {
    mysqlpp::StoreQueryResult res =  com::xiaonei::xce::QueryRunner(DB_INSTANCE, com::xiaonei::xce::CDbRServer, kWeeklySendedTableName).store(sql);  //注意数据库变更
    if(res.num_rows() == 0) {
      return true;
    }
    mysqlpp::Row row = res.at(0);
    now_max_id = boost::lexical_cast<Ice::Long> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["max_id"]);
  } catch (Ice::Exception& e) {
    MCE_WARN("read min id from db " << kWeeklySendedTableName << " failed, ice err->" << e);
  } catch (std::exception& ex) {
    MCE_WARN("read min id from db " << kWeeklySendedTableName << " failed, err->" << ex.what());
  } catch(...){
    MCE_WARN("read min id from db " << kWeeklySendedTableName << " failed");
  }
  unsigned row_id = 0;
  // test begin
  //now_min_id = now_max_id;
  // test end
  now_max_id += 10000;
  for (Ice::Long start_id = now_min_id; start_id < now_max_id; start_id += kEveryLoadNum) {
    Ice::Long end_id = start_id + kEveryLoadNum;
    sql.clear();
    sql << "select userid,send_time_int from " << kWeeklySendedTableName << " where id >= " << start_id << " and id < " << end_id;
    try {
      mysqlpp::StoreQueryResult res =  com::xiaonei::xce::QueryRunner(DB_INSTANCE, com::xiaonei::xce::CDbRServer, kWeeklySendedTableName).store(sql);
      for (row_id = 0;row_id < res.num_rows();++row_id) {
        mysqlpp::Row row = res.at(row_id);
        int user_id = row["userid"];
        time_t time_stamp = (time_t)row["send_time_int"];
        Add(user_id, kWeekly, time_stamp);
      }
    } catch (std::exception& ex) {
      MCE_WARN("load Weekly send edm from db " << kWeeklySendedTableName << "failed , err->" <<ex.what() << " row_id = " << start_id + row_id);
    } catch(...){
      MCE_WARN("load Weekly send edm from db " << kWeeklySendedTableName << "failed"<< " row_id = " << start_id + row_id);
    }
  } 
  MCE_INFO("load weekly history date done");
  return true;
}

bool FrequencyCache::LoadDailyData() {
  com::xiaonei::xce::Statement sql;
  Ice::Long now_min_id = 0;
  int start_time = time(NULL) - kDaySeconds;
  sql << "select min(id) as min_id from " << kDailySendedTableName << " where send_time_int > " << start_time;;
  try {
    mysqlpp::StoreQueryResult res =  com::xiaonei::xce::QueryRunner(DB_INSTANCE, com::xiaonei::xce::CDbRServer, kDailySendedTableName).store(sql);  //注意数据库变更
    if(res.num_rows() == 0) {
      return true;
    }
    mysqlpp::Row row = res.at(0);
    now_min_id = boost::lexical_cast<Ice::Long> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["min_id"]);
  } catch (Ice::Exception& e) {
    MCE_WARN("read max id from db " << kDailySendedTableName << " failed, ice err->" << e);
  } catch (std::exception& ex) {
    MCE_WARN("read max id from db " << kDailySendedTableName << " failed, err->" << ex.what());
  } catch(...){
    MCE_WARN("read max id from db " << kDailySendedTableName << " failed");
  }
  sql.clear();
  Ice::Long now_max_id = 0;
  sql << "select max(id) as max_id from " << kDailySendedTableName ;
  try {
    mysqlpp::StoreQueryResult res =  com::xiaonei::xce::QueryRunner(DB_INSTANCE, com::xiaonei::xce::CDbRServer, kDailySendedTableName).store(sql);  //注意数据库变更
    if(res.num_rows() == 0) {
      return true;
    }
    mysqlpp::Row row = res.at(0);
    now_max_id = boost::lexical_cast<Ice::Long> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["max_id"]);
  } catch (Ice::Exception& e) {
    MCE_WARN("read min id from db " << kDailySendedTableName << " failed, ice err->" << e);
  } catch (std::exception& ex) {
    MCE_WARN("read min id from db " << kDailySendedTableName << " failed, err->" << ex.what());
  } catch(...){
    MCE_WARN("read min id from db " << kDailySendedTableName << " failed");
  }
  unsigned row_id = 0;
  now_max_id += 10000;
  for (Ice::Long start_id = now_min_id; start_id < now_max_id; start_id += kEveryLoadNum) {
    Ice::Long end_id = start_id + kEveryLoadNum;
    sql.clear();
    sql << "select userid,send_time_int from " << kDailySendedTableName << " where id >= " << start_id << " and id < " << end_id;
    try {
      mysqlpp::StoreQueryResult res =  com::xiaonei::xce::QueryRunner(DB_INSTANCE, com::xiaonei::xce::CDbRServer, kDailySendedTableName).store(sql);
      for (row_id = 0;row_id < res.num_rows();++row_id) {
        mysqlpp::Row row = res.at(row_id);
        int user_id = row["userid"];
        time_t time_stamp = (time_t)row["send_time_int"];
        Add(user_id, kDaily, time_stamp);
      }
    } catch (std::exception& ex) {
      MCE_WARN("load daily send edm from db " << kDailySendedTableName << "failed , err->" <<ex.what() << " row_id = " << start_id + row_id);
    } catch(...){
      MCE_WARN("load daily send edm from db " << kDailySendedTableName << "failed"<< " row_id = " << start_id + row_id);
    }
  } 
  MCE_INFO("load dailyly history date done");
  return true;
}

void FrequencyCache::CacheCleaner::handle() {
  if (FrequencyCache::instance().load_sign()) {
    FrequencyCache::instance().UpdateCache();
  }
}

void FrequencyCacheLoadTask::handle() {
  FrequencyCache::instance().LoadFromDb();
}


}
}
