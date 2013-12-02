#include "PingInfoManager.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "QueryRunner.h"
#include "DbDescriptor.h"
#include "PingMonitor.h"

using namespace com::xiaonei::xce;
using namespace xce::ad;
using namespace boost;
/*
 * Func:PingInfoManager::PingInfoManager
 * Desc：启动入库任务
 */
PingInfoManager::PingInfoManager(const int threshold, const string& db_source) {
  threshold_ = threshold;
  TaskManager::instance().execute(new WriteTask(this, db_source));
}

/*
 *Func:PingInfoManager::Put
 *Desc:写信息到容器中
 */
void PingInfoManager::Put(const vector<string>& messages) {
  if (messages.empty()) {
    return;
  }
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
  vector<string>::const_iterator it = messages.begin();
  while(it != messages.end()){
    if (pings_.size() >= threshold_) {
      break;
    }
    pings_.push_back(*it);
    MonitorDataPool::instance().Put(*it);
    ++it;
  }
  if (pings_.size() >= threshold_) {
    notify();
  }
}

/*
 *Func:PingInfoManager::Get
 *Desc:获取信息
 */
void PingInfoManager::Get(vector<string>& messages) {
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
  if (pings_.empty() || pings_.size() < threshold_) {
    timedWait(IceUtil::Time::seconds(10));
  }
  messages.swap(pings_);
}

/*
 *Func:WriteTask
 *Desc:构造函数
 */
WriteTask::WriteTask(PingInfoManager* ping_manager, const string& db_source) {
  ping_manager_ = ping_manager;
  DbInstance_ = db_source;
}

/*
 *Func:WriteTask::Write
 *Desc:写入数据库
 */
void WriteTask::Write(const vector<string>& pings) {
  MCE_DEBUG("WriteTask::Write --> start");
  if (pings.empty()) {
    return;
  }
  Statement sql;
  sql << " INSERT INTO server_ping_info (server_name, method_name, responsetime, require_num, updatetime) "
      << " VALUES ";
  vector<string>::const_iterator it = pings.begin();//AdMobEngine0,GetAds,3.2,5,ymdhms,ip
  vector<string> values;
  MCE_DEBUG("WriteTask::Write size:" << pings.size());
  for (; it != pings.end(); ++it) {
    values.clear();
    split(values,*it,is_any_of(","));
    if (values.size() < 5) {
      ostringstream os;
      copy(values.begin(), values.end(), ostream_iterator<string>(os, ","));
      MCE_WARN("WriteTask::Write -->" << os.str());
      continue;
    }

    MCE_INFO("WriteTask::Write " << values[0] << "," << values[1] << "," << values[2] << "," << values[3] << "," << values[4]);
    if (it != pings.begin()) {
      sql << ",";
    }
    sql << "(\"" << values[0] << "\", \"" << values[1] << "\", \"" << values[2] << "\","
      << values[3] << "," << values[4] << ")";
  }
  try {
    QueryRunner(DbInstance_, CDbWServer, "server_ping_info").store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN("WriteTask::Write: Ice Exception: " << e);
  } catch (std::exception& e) {
    MCE_WARN("WriteTask::Write: std exception: " << e.what());
  } catch (...) {
    MCE_WARN("WriteTask::Write: unknown exception");
  }
  MCE_DEBUG("WriteTask::Write --> end");
}

void WriteTask::handle() {
  while(true){
    vector<string> ping_info;
    ping_manager_->Get(ping_info);
    Write(ping_info);
  }
}
