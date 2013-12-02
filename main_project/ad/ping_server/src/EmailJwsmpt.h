#ifndef __AD_PINGSERVER_EMAIL_H__
#define __AD_PINGSERVER_EMAIL_H__

#include <string>
#include <vector>
#include <IceUtil/RWRecMutex.h>
#include <jwsmtp/jwsmtp.h>
#include "Singleton.h"
#include "TaskManager.h"
#include "Util.h"

namespace xce{
namespace ad{

//发送邮件
class JwsmptEmail : public MyUtil::Singleton<JwsmptEmail>{
public:
  JwsmptEmail():from_("ad.report@renren-inc.com"), username_("ad.report@renren-inc.com"),pwd_("AdEngine9999"), 
    smpt_server_("smtp.renren-inc.com"), port_(jwsmtp::mailer::SMTP_PORT), subject_("Service Response Exception by Ping Server"){
    //receivers_.push_back("renren.ad.engine@renren-inc.com");
    receivers_.push_back("bin.qin@renren-inc.com");
  }
  void Init();
  void WriteMsg(const string& msg);
  void Send();
public:
  void SetFrom(const string& from) {
    from_ = from;    
  }
  void SetUsername(const string& username) {
    username_ = username;
  }
  void SetPwd(const string& pwd) {
    pwd_ = pwd;
  }
  void SetSmptServer(const string& server) {
    smpt_server_ = server;
  }
  void SetSubject(const string& sub){
    subject_ = sub;
  }
  void SetReceivers(vector<string>& vec) {
    receivers_ = vec;
  }
private:
  jwsmtp::mailer mail_;
  string from_;  //发件地址
  string username_; //发件人
  string pwd_;      //发件密码
  string smpt_server_;
  int port_;
  string subject_;
  vector<string> receivers_;
  vector<string> msgs_;
  IceUtil::Mutex mutex_;
};

//定时发送邮件
class SendEmailTimer : public MyUtil::Timer {
public:
  SendEmailTimer(): Timer(ONE_MINUTE){}
  virtual void handle();
};

}// end ad
}//end xce

#endif //end __AD_PINGSERVER_EMAIL_H__
