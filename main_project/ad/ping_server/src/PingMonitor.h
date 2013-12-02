#ifndef __AD_PINGSERVER_MONITOR_H__ 
#define __AD_PINGSERVER_MONITOR_H__

#include <string>

#include <IceUtil/Mutex.h>
#include <IceUtil/Shared.h>
#include <IceUtil/Monitor.h>
#include <IceUtil/RWRecMutex.h>

#include "Singleton.h"
#include "TaskManager.h"
#include "PingConfig.h"

using namespace std;
using namespace MyUtil;

namespace xce {
namespace ad {

//服务中方法的响应时间异常和报警参数
class MonitorConfig : public IceUtil::Shared {
public:
  MonitorConfig(const string& server_name, string& method_name, const int except_count = EXCEPT_COUNT_DEFAULT , 
      const int time_threshold = TIME_THRESHOLD_DEFAULT, const double avg_multiple = AVG_MULTIPLE_DEFAULT);
  int GetExceptCount() {
    return except_count_;
  }
  int GetTimeThreshold() {
    return time_threshold_;
  }
  double GetAvgMultiple() {
    return avg_multiple_;
  }
private:
  string server_name_; //AdMobEngine
  string method_name_; //GetAds
  int except_count_;    //异常数阈值
  int time_threshold_;  //异常时间阈值
  double avg_multiple_;    //异常时间倍数阈值
};
typedef IceUtil::Handle<MonitorConfig> MonitorConfigPtr;

//参数配置池
class MonitorConfigPool : public MyUtil::Singleton<MonitorConfigPool> {
public:
  void Init();
  MonitorConfigPtr GetConfig(const string& key);
  void SetConfigDBSource(const string& db_source){
    config_dbsource_ = db_source;
  }
private:
   map<string, MonitorConfigPtr> config_pool_;//key:AdMobEngine_GetAds
   string config_dbsource_; 
   IceUtil::RWRecMutex mutex_;
};

class MonitorConfigTimer : public MyUtil::Timer {
public:
  MonitorConfigTimer() : Timer(ONE_MINUTE){} 
  virtual void handle();
};

class MonitorData; 
//统计定时器，每隔10分钟统计下响应时间异常数
class StatisticTimer : public MyUtil::Timer {
public:
  StatisticTimer(MonitorData* data, int time = 10*ONE_MINUTE) : Timer(time) {
    data_ = data;
  }
  virtual void handle();
private:
  MonitorData* data_;
};

//一个服务方法的监控实体
class MonitorData : public IceUtil::Shared {
public:
  MonitorData(const string& server_name, const string& method_name, const string& ip)
    :server_name_(server_name), method_name_(method_name) {
    size_t pos = server_name.find_first_of("-0123456789");
    if (pos != string::npos) {
      key_ = server_name.substr(0, pos) + "_" + method_name;
    } else {
      key_ = server_name + "_" + method_name;
    }
    ip_ = ip;
    MCE_INFO("MonitorData new --> server_name_:" << server_name_ << " method_name_:" 
        << method_name_ << " ip_:" << ip_);
    int time = PingConfig::instance().GetStatisticMinute();
    TaskManager::instance().schedule(new StatisticTimer(this, time * ONE_MINUTE));
  }
  void Put(const float avg_time, const int response_count);
  void Statistic();

private:
  string server_name_; //AdMobEngine0
  string ip_; //服务ip
  string method_name_; //GetAds
  string key_; //configPool中的key，AdMobEngine_GetAds
  deque<float> before_response_time_; //以前的响应时间
  deque<int> before_response_count_; //以前的响应次数
  float before_avg_time_; //以前的平均响应时间
  vector<float> except_time_; //统计间隔内的异常响应时间
  IceUtil::Mutex mutex_;
};
typedef IceUtil::Handle<MonitorData> MonitorDataPtr;

//监控实体池
class MonitorDataPool : public MyUtil::Singleton<MonitorDataPool> {
public:
  void Put(const string& response_msg);
private:
  map<string, MonitorDataPtr> data_pool_; //AdMobEngine0GetAds, MonitorData
  IceUtil::RWRecMutex mutex_;
};



}//end ad
}//end xce
#endif
