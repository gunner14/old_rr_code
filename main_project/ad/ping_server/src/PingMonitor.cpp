#include "PingMonitor.h"
#include <iostream>
#include <algorithm>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>
#include "EmailJwsmpt.h"
#include "QueryRunner.h"
#include "Util.h"
#include <time.h>
#include <Date.h>

using namespace com::xiaonei::xce;
using namespace xce::ad;
using namespace MyUtil;
using namespace std;
using namespace boost;
//单个服务-方法的参数
MonitorConfig::MonitorConfig(const string& server_name, string& method_name, const int except_count,
          const int time_threshold, const double avg_multiple) {
  server_name_ = server_name;
  method_name_ = method_name;
  except_count_ = except_count == 0 ? EXCEPT_COUNT_DEFAULT : except_count;
  time_threshold_ = time_threshold == 0 ? TIME_THRESHOLD_DEFAULT : time_threshold;
  avg_multiple_ = (avg_multiple > -0.000001 && avg_multiple < 0.000001)  ? AVG_MULTIPLE_DEFAULT : avg_multiple;
}

/*
 *Func:MonitorConfigPool::Init
 *Desc:加载服务方法响应时间异常及报警相关的参数阈值
 */
void MonitorConfigPool::Init() {
  Statement sql;
  sql << " select server_name, method_name, except_count, time_threshold, avg_multiple "
      << " from server_ping_config ";
  map<string, MonitorConfigPtr> local_pool;
  try {
    mysqlpp::StoreQueryResult result = QueryRunner(config_dbsource_, CDbRServer, "server_ping_config").store(sql);
    if (result && result.num_rows() > 0) {
      for (size_t i = 0; i < result.num_rows(); ++i) {
        mysqlpp::Row row = result.at(i);
        string server_name = mysql_string(row["server_name"]);
        string method_name = mysql_string(row["method_name"]);
        int except_count = mysql_int(row["except_count"]);
        int time_threshold = mysql_int(row["time_threshold"]);
        double avg_multiple = mysql_double(row["avg_multiple"]);
        MonitorConfigPtr config_ptr = new MonitorConfig(server_name, method_name, except_count, time_threshold, avg_multiple);
        local_pool[server_name + '_' + method_name] = config_ptr;
      }
    }
    MCE_INFO("MonitorConfigPool::Init --> server_ping_config, data size:" << local_pool.size());
    {
      IceUtil::RWRecMutex::WLock lock(mutex_);
      config_pool_.swap(local_pool);
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("MonitorConfigPool::Init :" << e);
  } catch (std::exception& e) {
    MCE_WARN("MonitorConfigPool::Init :" << e.what());
  }  
}

/*
 *Func:MonitorConfigPool::GetConfig
 *Desc:获取对应方法的参数配置, AdMobEngine_GetAds
 */
MonitorConfigPtr MonitorConfigPool::GetConfig(const string& key) {
  IceUtil::RWRecMutex::RLock lock(mutex_);
  map<string, MonitorConfigPtr>::iterator it = config_pool_.find(key);
  if (it != config_pool_.end()) {
    return it->second;
  }
  return NULL;
}

/*
 *Func:MonitorData::Put
 *Desc:存入新的响应时间，并判断其是否异常
 */
void MonitorData::Put(const float avg_time, const int response_count) {
  IceUtil::Mutex::Lock lock(mutex_);
  MCE_INFO("MonitorData::Put --> server:" << server_name_ << ",method:" << method_name_ 
      << ",avg_time:" << avg_time << ",response_count:" << response_count);
  int before_minute = PingConfig::instance().GetBeforMinute();
  if (before_response_time_.size() < before_minute) {//保存的时间不足既定值，则不进行异常判断
    MCE_INFO("MonitorData::Put --> server:" << server_name_ << ",method:" << method_name_ 
        <<", saved response time < " << before_minute << ", no charge");
    before_response_time_.push_back(avg_time);
    before_response_count_.push_back(response_count);
    if (before_response_time_.size() == before_minute) {
      float sum_time = 0;
      int sum_count = 0;
      for (int i = 0; i < before_minute; ++i) {
        sum_time += before_response_time_[i]*before_response_count_[i];
        sum_count += before_response_count_[i];
      }
      before_avg_time_ = sum_time/sum_count;
    }
    return;
  }
  //异常公式avg_time>beforeavg*avg_multiple or avg_time > time_threshold
  MonitorConfigPtr ptr = MonitorConfigPool::instance().GetConfig(key_);
  double avg_multiple = AVG_MULTIPLE_DEFAULT;
  int time_threshold = TIME_THRESHOLD_DEFAULT; 
  if (ptr) {
    avg_multiple = ptr->GetAvgMultiple();
    time_threshold = ptr->GetTimeThreshold();
  }
  float avg_threshold = (float)(before_avg_time_*avg_multiple);
  float min = avg_threshold > time_threshold ? time_threshold : avg_threshold; 
  if (avg_time > min) {
    except_time_.push_back(avg_time);
    MCE_INFO("MonitorData::Put Except --> server:" << server_name_ << ",method:" << method_name_ 
        <<", now_avg_time:" << avg_time << ",before_avg_time_:" << before_avg_time_ 
        << ", avg_multiple:" << avg_multiple <<", time_threshold:" << time_threshold);
  }
  //更新之前的响应时间队列，计算新的平均响应时间
  before_response_time_.push_back(avg_time);
  before_response_time_.pop_front();
  before_response_count_.push_back(response_count);
  before_response_count_.pop_front();
  float sum_time = 0;
  int sum_count = 0;
  for (int i = 0; i < before_minute; ++i) {
    sum_time += before_response_time_[i]*before_response_count_[i];
    sum_count += before_response_count_[i];
  }
  before_avg_time_ = sum_time/sum_count;
  MCE_DEBUG("MonitorData::Put --> key:" << key_ << ", avg_multiple:" 
      << avg_multiple <<", time_threshold:" << time_threshold);
}

/*
 *Func:MonitorData::Statistic
 *Desc:统计该方法的异常条数，决定是否报警
 */
void MonitorData::Statistic() {
  vector<float> except_time;
  {
    IceUtil::Mutex::Lock lock(mutex_);
    if (before_response_time_.size() < PingConfig::instance().GetBeforMinute()) {
      //保存的时间不足设定，则不统计
      MCE_DEBUG("MonitorData::Statistic --> saved response time < " << PingConfig::instance().GetBeforMinute() << ", no charge");
      return;
    }
    except_time.swap(except_time_);//清空异常
  }
  
  MonitorConfigPtr ptr = MonitorConfigPool::instance().GetConfig(key_);
  int count = EXCEPT_COUNT_DEFAULT;
  if (ptr) {
    count = ptr->GetExceptCount();
  }
  MCE_DEBUG("MonitorData::Statistic --> server_name:" << server_name_ << ", method_name:" << method_name_ 
      << ", except count:" << except_time.size());  
  //是否发邮件
  if (except_time.size() >= count) {
    ostringstream os;
    copy(except_time.begin(), except_time.end(), ostream_iterator<float>(os, ","));
    string except = os.str();
    ostringstream msg_stream;
    MyUtil::Date date(time(NULL));
    msg_stream << "<tr bgcolor='#cccccc'><td>" << server_name_ << "@" << ip_ << "</td><td>" << method_name_
      << "</td><td>" << before_avg_time_ << "</td><td>" << except << "</td><td>" << date.toDateTime() << "</td></tr>";
    
    MCE_INFO("MonitorData::Statistic --> key:" << key_ << ",server_name:" << server_name_ << ", method_name:" 
        << method_name_<< ", before_avg_time_:" << before_avg_time_ << ", except res:" << except);
    
    JwsmptEmail::instance().WriteMsg(msg_stream.str());
  }
  //是否发短信
  if (except_time.size() >= PingConfig::instance().GetMsgThreshold()) {
    vector<string> phones = PingConfig::instance().GetPhones();
    ostringstream os;
    os << server_name_ << "@" << ip_ << "-" << method_name_ << ":" << except_time.size() << " abnormal response"; 
    SendPhoneMsg(phones, os.str());
  }
}

/*
 *Func:MonitorDataPool::Put
 *Desc:有响应时间过来时，更新其对应监控实体，
 *arg:服务名，方法，平均响应时间，响应次数，时间，ip,(AdMobEngine0,GetAds,3.2,5,ymdhms,ip)
 */
void MonitorDataPool::Put(const string& response_msg) {
  MCE_DEBUG("MonitorDataPool::Put -->" << response_msg);
  try {
    vector<string> values;
    split(values, response_msg, is_any_of(","));
    if(values.size() < 5){
      return;
    }
    string server_name = values[0];
    string method_name = values[1];
    float res_time = lexical_cast<float>(values[2]);
    int res_count = lexical_cast<int>(values[3]);
    string ip = values.size() >= 6 ? values[5] : "";

    string key = server_name + method_name;
    IceUtil::RWRecMutex::WLock lock(mutex_);
    map<string, MonitorDataPtr>::iterator it = data_pool_.find(key);
    if (it==data_pool_.end()) {
      MonitorDataPtr ptr = new MonitorData(server_name, method_name, ip);
      ptr->Put(res_time, res_count);
      data_pool_.insert(make_pair(key,ptr));
    } else {
      it->second->Put(res_time, res_count);
    }
  } catch (...) {
    MCE_WARN("MonitorDataPool::Put Exception");
  }
}

//每隔一分钟更新参数配置
void MonitorConfigTimer::handle() {
  MCE_DEBUG("MonitorConfigTimer::handle");
  MonitorConfigPool::instance().Init();
}

//MonitorData每隔StatisticMinute调用自己的Timer，进行统计
void StatisticTimer::handle() {
  MCE_DEBUG("StatisticTimer::handle");
  data_->Statistic();
  int time = PingConfig::instance().GetStatisticMinute();
  TaskManager::instance().schedule(new StatisticTimer(data_, time * ONE_MINUTE));
}
