#ifndef __AD_PINGSERVER_CONFIG_H__ 
#define __AD_PINGSERVER_CONFIG_H__

#include <string>

#include "Util.h"
#include "Singleton.h"
#include "TaskManager.h"
#include "Singleton.h"
#include "LogWrapper.h"
#include "EmailJwsmpt.h"

using namespace std;
using namespace MyUtil;

namespace xce {
namespace ad {

class PingConfig : public MyUtil::Singleton<PingConfig> {
public:
  PingConfig() : before_minute_(10), statistic_minute_(10), send_msg_threshold_(10) {
    LoadConfig();
    TaskManager::instance().schedule(new ConfigLoadTimer(60*1000));
  }
  bool LoadConfig();
  int GetStatisticMinute() {
    return statistic_minute_;
  }
  int GetBeforMinute() {
    return before_minute_;
  }
  vector<string> GetPhones() {
    return phones_;
  }
  int GetMsgThreshold() {
    return send_msg_threshold_;
  }
private:
  int before_minute_;//参考时间段
  int statistic_minute_;//统计间隔
  int send_msg_threshold_;//发送短信的异常阈值
  string from_;
  string username_;
  string pwd_;
  string smpt_server_;
  string subject_;
  vector<string> email_receivers_;
  vector<string> phones_;

  class ConfigLoadTimer : public MyUtil::Timer{
    public:
      ConfigLoadTimer(int time = 0):Timer(time){}
      virtual void handle() {
        MyUtil::TaskManager::instance().schedule(new ConfigLoadTimer(60 * 1000));//1分钟更改一次
        PingConfig::instance().LoadConfig();
      }
  };
};

}//end ad
}//end xce
#endif
