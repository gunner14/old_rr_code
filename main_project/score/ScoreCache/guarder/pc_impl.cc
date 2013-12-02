/** 
 * 具体的生产者消费者
 * xiaofeng.liang@renren-inc.com
 */

#include "pc_impl.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>

#include "DbCxxPool/src/ConnectionPoolManager.h"
#include "DbCxxPool/src/QueryRunner.h"
#include "util/cpp/TimeCost.h"
#include "OceCxxAdapter/src/ScoreCacheAdapter.h"

using namespace MyUtil;
using namespace std;
using namespace xce::util;
using namespace com::xiaonei::xce;
using namespace xce::score;
using namespace xce::scorecache::adapter;

//CreateKeysResultHandler DB处理类
//--------------------------------------------------------------
CreateKeysResultHandler::CreateKeysResultHandler(const string &key, vector<int> *keyseq) :
  _key(key), _keyseq(keyseq) {}

bool CreateKeysResultHandler::handle(mysqlpp::Row& row) const {
  _keyseq->push_back((int)row[_key.c_str()]);
  return true;
}

LoginTimeHandler::LoginTimeHandler(time_t &time) : _time(time) {}

bool LoginTimeHandler::handle(mysqlpp::Row& row) const {
  _time = (time_t) row["time"];
  return true;
}

//DBProducer 从数据库获取ID，可以指定范围
//--------------------------------------------------------------
DbProducer::DbProducer(Queue<int> *queue, int band, int batch, int min, int max) :
    ProducerThread<int>(queue, band, batch), _min(min), _max(max) {
  time_t t = time(NULL);
  t -= 86400;
  char date[9];
  strftime(date, sizeof(date), "%Y%m%d", localtime(&t));
  _table = "iplog_";
  _table += date;
  MCE_INFO("[DbProducer]Init, band=" << band << ", batch=" << batch
      << ", table=" << _table << ", min=" << min << ", max=" << max);
}

void DbProducer::produce(vector<int> *ids, int batch) {
  MyUtil::Clock clock;
  Statement sql;
  sql << "SELECT DISTINCT(id) FROM " << _table << " WHERE id > " << mysqlpp::quote << _min;
  if(_max > 0 && _max < 0x7fffffff) { //有上限
    sql << " AND id <= " << mysqlpp::quote << _max;
  }
  sql << " LIMIT " << batch;
  CreateKeysResultHandler handler("id", ids);
  QueryRunner("ip_log", CDbRServer).query(sql, handler);
  if(ids->size()) {
    _min = ids->back();
  }
  ostringstream oss;
  oss << "[DbProducer]produce, count=" << ids->size();
  if(ids->size()) {
    oss << " front= " << ids->front() << " back= " << ids->back();
  }
  oss << " time_cost=" << clock.total()/1000 << " ms";
  MCE_INFO(oss.str());
}

ScoreGuarder::ScoreGuarder(Queue<int> *queue, int batch, int& count) :
    ConsumerThread<int>(queue, batch) {  
  _pCount = &count;
  time_t yst = time(NULL) - 3600 * 24;
  struct tm *tm_yst = localtime(&yst);
  tm_yst->tm_hour = 0;
  tm_yst->tm_min = 0;
  tm_yst->tm_sec = 0;
  _min = mktime(tm_yst);
  MCE_INFO("[ScoreGuarder]Init, batch=" << batch << " ,min_time: " << _min);
}

bool ScoreGuarder::consum(vector<int> ids) {
  MyUtil::Clock clock;
  for(vector<int>::iterator it = ids.begin(); it != ids.end(); ++it) {
    time_t time;
    Statement sql;
    sql << "SELECT user_id, UNIX_TIMESTAMP(last_time) AS time FROM score_login_"
      << *it % 100 << " WHERE user_id=" << *it;
    ostringstream pattern;
    pattern << "user_score_" << (*it % 100);
    QueryRunner("user_score", CDbRServer, pattern.str()).query(sql, LoginTimeHandler(time));
    if(time < _min && time >= _min - 24 * 3600) {  /////昨天是连续登录
      (*_pCount)++;
      char buf[20];
      strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&time));
//      MCE_WARN("[ScoreGuarder::consum]Unsolved Score, id: " << *it << " ,time: " << time << " " << buf);
      //TODO 加入修复代码
      Statement sqlUpdate;
      sqlUpdate << "UPDATE score_login_" << *it % 100 << " SET history_days = history_days + 1, continue_days = continue_days + 1, last_time = "
        << "FROM_UNIXTIME(" << _min + 500 << ") where user_id = " << *it;
      try {
        QueryRunner("user_score", CDbWServer, pattern.str()).execute(sqlUpdate);
        try {
          ScoreLoaderNAdapter::instance().reload(*it);
          MCE_INFO("[ScoreGuarder::consum] update continue login userId:" << *it);
        } catch(exception & ex) {
          MCE_WARN("[ScoreGuarder::consum] continue reload exception userId:" << *it);
        }
      } catch (exception & ex) {
        MCE_WARN("{ScoreGuarder::consum] update continue login userId:" << *it << ", exception:" << ex.what());
      }

    } else if (time < _min) {  ////昨天非连续登录
      (*_pCount)++;
      Statement sqlUpdate;
      sqlUpdate << "UPDATE score_login_" << *it % 100 << " SET history_days = history_days + 1, last_time = "
        << "FROM_UNIXTIME(" << _min + 500 << ") where user_id = " << *it;
      try {
        QueryRunner("user_score", CDbWServer, pattern.str()).execute(sqlUpdate);
        try {
          ScoreLoaderNAdapter::instance().reload(*it);
          MCE_INFO("[ScoreGuarder::consum] update non-continue login userId:" << *it);
        } catch(exception & ex) {
          MCE_WARN("[ScoreGuarder::consum] non-continue reload exception userId:" << *it);
        }
      } catch(exception & ex) {
        MCE_WARN("[ScoreGuarder::consum] update non-continue login userId:" << *it << ", exception:" << ex.what());
      }
    }
  }
  MCE_INFO("[ScoreGuarder::consum]consum size: " << ids.size());
  return true;
}
