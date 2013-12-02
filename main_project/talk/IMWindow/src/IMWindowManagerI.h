#ifndef IMWINDOWMANAGER_H_
#define IMWINDOWMANAGER_H_
#include <ctemplate/template.h>
#include <ctemplate/template_dictionary.h>
#include "TemplateDataCollector.h"
#include "Singleton.h"
#include "ServiceI.h"
#include "TaskManager.h"
#include "Notify/util/fcgi_request.h"
#include "IMWindow.h"
#include <map>
#include <set>
#include "PhotoUrlTools.h"
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include "UserPassportAdapter.h"
#include "pugixml.hpp"


using namespace com::xiaonei::talk::util;
using namespace xce::notify;
using namespace ctemplate;
using namespace std;
using namespace com::xiaonei::talk::common;
using namespace MyUtil;
using namespace pugi;
namespace talk {
namespace window{

const int POOL_TUB_SIZE = 100;
const int TYPE_INSTANT_WND = 1;
const int TYPE_DELAY_WND = 2;
const int TYPE_ALL_WND = 2;

const int UPDATE_CONFIG_TIMER = 10*60*1000;
const int SEND_AD_TIMER = 2*60*1000;
const int SERVER_COUNT= 1;

inline int GetBufferIndex(int uid) {
  return (uid/SERVER_COUNT)%POOL_TUB_SIZE; 
}
class PathConfig : public IceUtil::Shared{
public:
	string template_path;
	int timeout;
	string type;
	string content_type;
	string reminder_type;
	string width;
	string height;
	string url;
	string showtime;
	string topmost;
	string title;

  string age;
  string gender;
  string region;
  string stage; 
  string xntalk_ver;
  string whitelist;
  string blacklist;
  string account_status;
  time_t start_date;
  time_t expiration_date;
};
typedef IceUtil::Handle<PathConfig> PathConfigPtr;

class UserInfo : public IceUtil::Shared{
public:
  int user_id;
  string age;
  string region;
  string gender;
  string xntalk_ver;
  string account_status;
};
typedef IceUtil::Handle<UserInfo> UserInfoPtr;

struct AdInfo {
  string url;
  PathConfigPtr ad_config;
};

class UserAdInfo : public IceUtil::Shared {
public:
  time_t first_send_time;
  time_t last_send_time;
  int last_send_status;
  UserInfoPtr user_info;
  vector<string> sended_ads;
  UserAdInfo() {
    first_send_time = last_send_time = time(NULL);
    last_send_status = 1;
  } 
}; 
typedef IceUtil::Handle<UserAdInfo> UserAdInfoPtr;

class IMWindowRequestFactory: public RequestFactory{
public:
	virtual RequestPtr Create(FCGX_Request * r);
};


class IMWindowRequest : public Request{
public:
  IMWindowRequest(FCGX_Request* r) : Request(r) {}
  virtual bool response();
	void ErrorResponse(int status_code);
};


class IMWindowManagerI : public IMWindowManager, virtual public Singleton<IMWindowManagerI> {
public:
	virtual void ReloadConfig(const Ice::Current& = Ice::Current());
  virtual void IMWindowReminder(const JidPtr& jid,  const Ice::Current& = Ice::Current());
	virtual void SendApplist(const string &info, const JidPtr& to_jid, const Ice::Current& = Ice::Current());
	virtual void ShowWindow(const string &window_title, const int uid, const Ice::Current& = Ice::Current());
	virtual void ShowNotify(const NotifyInfoPtr &ni, const int uid, const Ice::Current& = Ice::Current());

  void ReloadApplist();
	PathConfigPtr GetConfigByPath(const string& path, const int wnd_type);
	MessageSeq GetXmppMessage(const PathConfigPtr& config, const MyUtil::IntSeq& userids);
	MessagePtr GetXmppMessage(const PathConfigPtr& config, const JidPtr& jid, const bool xml_flag=false);
  MessageSeq GetXmppMessage(const PathConfigPtr& config, const Ice::Int& uid);
  MessageSeq GetNotifyMessage(const NotifyInfoPtr& ni, const int uid); 
	TemplateDataCollectorPtr GetTempCollector(const string& path);
  vector<string> GetAdPathsFromPool(const UserInfoPtr &user_info,const time_t current_time);
  void GetAdConfigsFromFile(vector<AdInfo> &ad_config_pool, vector<AdInfo> &instant_config_pool, int &ad_list);
  bool GetUserListFromFile(const string file_path, set<Ice::Int> &whitelist);
  void Initialize(const string& config_file_path, const string& whitelist_file_path, const string& blacklist_file_path, const string& applist_file_path);
//  bool LoadAvailableUsersFromDB();
  void UpdateConfiguration();
  int GetCurrentDay();
  void SendAds();
  void DeliverUpdateNote();
//  bool LoadUserAdInfoFromDB();

  bool GetWhiteListFromFile();
	bool WhiteListCheck(const Ice::Int userid);
	bool BlackListCheck(const Ice::Int userid);
  void SwapApplistMsg(MessageSeq &mseq);
  void SwapInstantWindowMsg(MessageSeq &mseq);
  void SwapNotifyMsg(MessageSeq &mseq);
  void ClearInstantUsers();
private:
  IceUtil::Mutex instant_users_mutex_;
  set<Ice::Int> instant_users_;
  IceUtil::RWRecMutex userlist_mutex_;
  set<Ice::Int> whitelist_;
  set<Ice::Int> blacklist_;

	IceUtil::RWRecMutex ad_config_rwmutex_;
//	map<string, PathConfigPtr> ad_config_pool_;
	vector<AdInfo> ad_config_pool_;
	vector<AdInfo> instant_config_pool_;
	map<int, UserAdInfoPtr> user_ad_cache_[POOL_TUB_SIZE];
  IceUtil::Mutex user_ad_mutex_[POOL_TUB_SIZE];
//  string whitelist_;
//  string blacklist_;
	string config_file_path_;
	string whitelist_file_path_;
	string blacklist_file_path_;
	string applist_file_path_;
  int current_day_;
   int usrid_;
  
	IceUtil::RWRecMutex _rwmutex;
	//IceUtil::RWRecMutex::WLock lock(_rwmutex);
	map<string, PathConfigPtr> _config;
  int ad_list_;
	map<int, time_t> _usersendcache;
	string _path;

  IceUtil::Mutex statistic_mutex_;
  int total_access_;
  IceUtil::RWRecMutex users_mutex_;
  set<Ice::Int> users_;
  IceUtil::RWRecMutex applist_mutex_;
//  xml_node applist_node_;
  xml_document applist_doc_;
  IceUtil::Mutex applist_mseq_mutex_;
  MessageSeq applist_mseq_;
  IceUtil::Mutex instant_wnd_mutex_;
  MessageSeq instant_wnd_mseq_;
  IceUtil::Mutex notify_mseq_mutex_;
  MessageSeq notify_mseq_;
};
	
class SendNotifyTimer: public MyUtil::Timer{
public:
	SendNotifyTimer() : Timer(500, 101){}
	virtual void handle();
};
class SendApplistTimer: public MyUtil::Timer{
public:
	SendApplistTimer() : Timer(1*1000, 102){}
	virtual void handle();
};
class InstantWindowTimer: public MyUtil::Timer{
public:
	InstantWindowTimer() : Timer(1*1000, 103){}
	virtual void handle();
};
class GetConfigTimer : public MyUtil::Timer{
public:
	GetConfigTimer() : Timer(UPDATE_CONFIG_TIMER ){}
	virtual void handle();
};

class HourTimer : public MyUtil::Timer{
public:
  HourTimer() : Timer(60*60*1000, 105){}
	virtual void handle();
};

class SendAdTimer: public MyUtil::Timer {
public:
  SendAdTimer(): Timer(SEND_AD_TIMER, 110)/*, reset_count(0)*/{}
//  SendAdTimer(): Timer(20*1000), reset_count(0){}
  virtual void handle();
//private:
//  int reset_count;
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

};
};

#endif //IMWindow 
