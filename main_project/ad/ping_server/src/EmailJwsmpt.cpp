#include "EmailJwsmpt.h"
#include "LogWrapper.h"
#include "PingConfig.h"

using namespace xce::ad;
using namespace std;

void JwsmptEmail::Init() {
  mail_.clearrecipients();
  for(vector<string>::iterator it = receivers_.begin(); it != receivers_.end(); ++it){
    mail_.addrecipient(*it);
  }

  mail_.setsender(from_);
  mail_.username(from_);
  mail_.password(pwd_);  
  mail_.setserver(smpt_server_);
  mail_.setsubject(subject_);
}

/*
 *Func:JwsmptEmail::WriteMsg
 *Desc:保存要发送的内容
 */
void JwsmptEmail::WriteMsg(const string& msg){
  IceUtil::Mutex::Lock lock(mutex_);
  MCE_DEBUG("JwsmptEmail::WriteMsg --> msg=" << msg);
  msgs_.push_back(msg);
}

/*
 *Func:JwsmptEmail::Send
 *Desc:拼装内容，发送邮件
 */
void JwsmptEmail::Send(){
  Init();
  vector<string> local_msg;
  {
    IceUtil::Mutex::Lock lock(mutex_);
    local_msg.swap(msgs_);
  }
  MCE_DEBUG("JwsmptEmail::Send --> msg.size:" << local_msg.size());
  if (local_msg.empty()) {
    return;
  }
  ostringstream os;
  os << "<table bordercolorlight='#333333' bordercolordark='#efefef' cellspacing='0' width=\'100%\' border='1'>";
  os << "<tr bgcolor='#cccccc'><td>Service</td><td>Method</td><td>Avg Time(ms) in Last " << PingConfig::instance().GetBeforMinute()
    << "m</td><td>Abnormal Time(ms) in Last " << PingConfig::instance().GetStatisticMinute() << "m</td><td>Statistic At</td></tr>";
  for (vector<string>::iterator it = local_msg.begin(); it != local_msg.end(); ++it) {
    os << *it;
  }
  os << "</table>";
  mail_.setmessageHTML(os.str());
  mail_.send();
  if(mail_.response( ).substr(0,3) != "250") {
    MCE_WARN("JwsmptEmail::Send --> Send Error.");
  }
}

/*
 *Func:SendEmailTimer::handle
 *Desc:每隔一分钟执行一次
 */
void SendEmailTimer::handle(){
  JwsmptEmail::instance().Send();
}
