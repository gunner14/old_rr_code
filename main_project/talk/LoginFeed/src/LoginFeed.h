#ifndef LOGINFEED_H
#define LOGINFEED_H

#include <map>
#include "XNTalkLoginFeed.h"
#include "ServiceI.h"
#include "Singleton.h"
#include "UsersBitmap.h"
#include "OnlineCenterAdapter.h"

namespace xce {
namespace xntalk {
const std::string LOGINFEED_TABLE = "login_feed";
const std::string XNTALK_LOGIN_TABLE_PREFIX = "xntalk_action_";
const std::string LOGIN_INSTANCE = "im_stat";
const std::string FEED_INSTANCE = "im";
const std::string CONFIG_PATH = "/data/xce/XNTalk/etc/LoginFeed/loginfeed.cfg";
const std::string RECEIVERS_PATH = "/data/xce/XNTalk/etc/LoginFeed/feed_receivers.cfg";
const std::string INSTALL_FEED_CONFIG_PATH = "/data/xce/XNTalk/etc/LoginFeed/install_feed.cfg";

const int POOL_TUB_SIZE = 100;
const int SEND_FRIENDS_SIZE = 200;
#define GetPoolId(uid) (uid%POOL_TUB_SIZE)

class UserInfo :public IceUtil::Shared {
public:
  std::string version_;  //如果为空串，对所有版本都发
  Ice::Long login_time_;
};
typedef IceUtil::Handle<UserInfo> UserInfoPtr;
class DBLoadUser: public IceUtil::Shared {
public:
  Ice::Long uid;
  std::map<Ice::Long, UserInfoPtr>::iterator iter;
};
typedef IceUtil::Handle<DBLoadUser> DBLoadUserPtr;
class SendFeedUser : public IceUtil::Shared {
public:
  Ice::Long uid;
  Ice::Long send_time;
};
typedef IceUtil::Handle<SendFeedUser> SendFeedUserPtr;

class LoginFeedManagerI : public xce::xntalk::LoginFeedManager,public MyUtil::Singleton<LoginFeedManagerI> {
public:
  virtual void SendFeed(Ice::Long uid,  const Ice::Current& = Ice::Current());
  virtual void ReloadConfig(const Ice::Current& = Ice::Current());
  virtual void SendInstallFeed(Ice::Long uid, const std::string &version, const Ice::Current& = Ice::Current());
  
  void InitalizeUserInfo();
  void LoadUsersFeedHistory();
  bool LoadUsersFeedTime(vector<DBLoadUserPtr> &db_load_users, map<Ice::Long, Ice::Long> &users_login_time); 
  void SendUsersFeed();
  void SendGraduationFeed(const Ice::Long id);
  void ClearGraduationLogin();
  bool LoadLoginUsers();
private:
  void SwapDBLoadUsers(std::vector<DBLoadUserPtr> &db_load_users, int i);
  void SwapSendFeedUsers(std::vector<SendFeedUserPtr> &send_feed_users, int i);
  bool LoadLoginFeedConfig();
  bool LoadInstallFeedConfig();
  bool LoadReceivers();

  bool CheckLoginUser(std::vector<string> &vers,com::xiaonei::talk::common::OnlineStat &os); 
  void SendFeeds(std::vector<SendFeedUserPtr> &send_feed_users);
  std::string GetLoginTable(const Ice::Int offset);
  bool UpdateSendUsers(std::vector<SendFeedUserPtr> &send_feed_users);
  void SendFeed(std::vector<SendFeedUserPtr> &send_feed_users);
  std::map<Ice::Long, UserInfoPtr> user_info_pool_[POOL_TUB_SIZE];
  IceUtil::Mutex user_info_mutex_[POOL_TUB_SIZE];

  std::map<std::string, std::string> config_info_;
	IceUtil::RWRecMutex config_info_mutex_;

  std::set<Ice::Long> yesterday_login_;
	IceUtil::Mutex yesterday_login_mutex_;

  std::map<std::string, std::map<std::string, std::string> > install_feed_config_;
	IceUtil::RWRecMutex install_feed_config_mutex_;

//  std::set<Ice::Long> login_users_;
	IceUtil::RWRecMutex login_users_mutex_;
  xce::xntalk::UsersBitmapPtr login_users_;

  std::vector<DBLoadUserPtr> db_load_users_[POOL_TUB_SIZE];
  IceUtil::Mutex db_load_users_mutex_[POOL_TUB_SIZE];

  std::vector<SendFeedUserPtr> send_feed_users_[POOL_TUB_SIZE];
  IceUtil::Mutex send_feed_users_mutex_[POOL_TUB_SIZE];
};

class ReloadConfigTimer: public MyUtil::Timer {
public:
  ReloadConfigTimer(): Timer(5*60*1000){}
  virtual void handle();
};

class LoginFeedDBTimer: public MyUtil::Timer {
public:
  LoginFeedDBTimer(): Timer(90*1000){}
  virtual void handle();
};

class SendFeedTimer: public MyUtil::Timer {
public:
  SendFeedTimer(): Timer(90*1000){}
  virtual void handle();
};

class LoadLoginUsersTimer:public MyUtil::Timer {
public:
  LoadLoginUsersTimer(): Timer(60*60*1000) {}
  virtual void handle();
};

class TimeStat{    
private:
  timeval _begin, _end;      
public:      
  TimeStat() {
    reset();
  }
  void reset() {
    gettimeofday(&_begin, NULL);
  }
  float getTime(){
    gettimeofday(&_end, NULL);
    float timeuse=1000000*(_end.tv_sec-_begin.tv_sec)+_end.tv_usec-_begin.tv_usec;
    timeuse/=1000;
    return timeuse;
  }
};

class LoadLoginUsersTask : public MyUtil::Task{
public:
	LoadLoginUsersTask(){
	}
	virtual void handle();
};


};
};
#endif
