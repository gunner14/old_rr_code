#ifndef PAGECACHEMANAGERI_H_
#define PAGECACHEMANAGERI_H_

#include "Singleton.h"
#include "ServiceI.h"
#include "TemplateDataCollector.h"
#include "PageCache.h"
#include "MucUserRoomAdapter.h"
#include "BuddyGroupAdapter.h"
#include "QueryRunner.h"

namespace talk {
namespace http {

using namespace MyUtil;
using namespace com::xiaonei::xce;

const static int AU = 0;
class Config {
public:
  string template_path;
  int timeout;
  string type;
  string content_type;
};
//-------------------------------------------

//---------------------------------------------

class Buddy : public IceUtil::Shared {
  int _id;
  string _name;
  string _tinyurl;
  string _doing;
  int _onlineStatus;
  vector<string> _buddygroups;
public:
  bool operator < (const Buddy& o) const{
    return _id < o._id;
  }
  void id(int id) {
    _id = id;
  }
  int id() {
    return _id;
  }

  void name(const string& name);

  string name() {
   return _name;
  }

  void buddyGroup(const vector<string>& buddygroups){
    _buddygroups = buddygroups;
  }
  vector<string> buddyGroup(){
    return _buddygroups;
  }

  void tinyUrl(const string& url) {
    _tinyurl = url;
  }
  string tinyUrl();

  void doing(const string& doing);

  string doing() {
    return _doing;
  }

  void onlineStatus(int stat) {
    _onlineStatus = stat;
  }
  int onlineStatus() {
    return _onlineStatus;
  }
};
typedef IceUtil::Handle<Buddy> BuddyPtr;

class ActiveUser : public Ice::Object {
  enum {
    BUDDY_LIST_TIME_STAMP = 60,
    BUDDY_GROUP_MAP_TIME_STAMP = 300,
  };
  int _userid;
  string _name;
  string _tiny_url;
  string _ticket;
  map<int, int> _online_buddys_map;  // 缓存在线好友ID
  IntSeq _offline_buddys_seq; // 缓存非在线好友ID
  mop::hi::svc::adapter::BuddiedGroupsMap _groupMap; // 缓存分组信息
  size_t _buddyListTimestamp;
  size_t _buddyGroupMapTimestamp;

  void load();
  void getUserOnlineType(const IntSeq &buddy_ids, map<int, int> &online_bdys, IntSeq &offline_bdys);
  // 获取在线用户和非在线用户状态，会进行缓存
  void seprateBuddyList();
  const map<int, int> &getOnlineMap();
  const IntSeq &getOfflineSeq();
  // 获取分组信息，会进行缓存
  const mop::hi::svc::adapter::BuddiedGroupsMap &getBuddyGroupMap();
  void getOnlineList(const map<int, int> &online, vector<BuddyPtr> &buddys_res);
  void getRecentList(vector<string> &recents);
  void getOfflineList(const IntSeq &offline, vector<BuddyPtr> &buddys_res);
  void getBuddySepList(const map<int, int> &online, const IntSeq &ids, vector<BuddyPtr> &buddys_res);

public:
  ActiveUser(int userid) :
  _userid(userid), _buddyListTimestamp(0), _buddyGroupMapTimestamp(0) {
  }

  void getFullBuddyList(vector<BuddyPtr> &buddylst);
  void getFullAndRecentBuddyList(int option, vector<BuddyPtr> &buddylst, vector<string> &recents, int &friend_count);
  bool getAllFullBuddyList(int begin, int limit, int &friend_count, int &online_count, vector<BuddyPtr> &buddylst);
  bool getUserInfoList(const vector<string> &uidlist, int &online_count, vector<BuddyPtr> &buddys);

  int getBuddyCount();
  bool checkTicket(const string& ticket, bool isWap=false);
  int userId();
  string name();
  string tiny_url();
};
typedef IceUtil::Handle<ActiveUser> ActiveUserPtr;
//------------------------------------------------------------------
class ActiveUserFactoryI : public ServantFactory {
public:
  virtual Ice::ObjectPtr create(Ice::Int id);
};

//-------------------------------------------------
class PageCacheCallStat {
 public:
  enum {
    BUDDY_LIST,
    BUDDY_GROUP,
    // 分类计数，循环使用，注意保证这个值在最后
    STAT_COUNT,
  };
 private:
  //map< 类型, pair<用户调用次数， 实际调用次数> >
  //typedef map< int, pair<volatile long, volatile long> > stat_type;
  
  //stat_type stat_[24];
  volatile long stat_[24][STAT_COUNT][2];
  
  size_t begin_time_;

 public:
  PageCacheCallStat() {
    ClearCount();
  }

  void ClearCount() {
    begin_time_ = time(NULL);
    for (int t = 0; t < 24; t++) {
      for (int i = 0; i < STAT_COUNT; i++) {
        stat_[t][i][0] = 0;
        stat_[t][i][1] = 0;
      }
    }

  }
  void UpCount(int cate, int sep) {
    //int idx = (time(NULL) % (24 * 3600)) / 3600;
    time_t tmp = time(NULL);
    int idx = localtime(&tmp)->tm_hour;
    //MCE_INFO("PageCacheCallStat.UpCount-->idx=" << idx << " cate=" << cate << " sep=" << sep);
    if (idx < 24 && cate < STAT_COUNT && (0 == sep || 1 == sep)) {
      stat_[idx][cate][sep]++;
    }

  }

  vector<long> GetStat() {
    vector<long> st;
    st.push_back(begin_time_);
    for (int t = 0; t < 24; t++) {
      for (int i = 0; i < STAT_COUNT; i++) {
        st.push_back(t);
        st.push_back(i);
        st.push_back((long)stat_[t][i][0]);
        st.push_back((long)stat_[t][i][1]);
      }

    }
    return st;
  }
  

};

class PageCacheManagerI : public PageCacheManager,virtual public Singleton<PageCacheManagerI> {
  enum {
    ONE_GET_LIMIT = 1000,
    MAX_GET_LIMIT = 3000,
    RATE_FRIEND_INFO = 900,
  };
public:
  PageCacheManagerI() : 
  _one_get_limit(ONE_GET_LIMIT), _max_get_limit(MAX_GET_LIMIT), _rate_friend_info(RATE_FRIEND_INFO)
  {

  }
  virtual ContentPtr GetContent(const string& path,
                                const MyUtil::Str2StrMap& cookies,
                                const MyUtil::Str2StrMap& parameter,
                                const Ice::Current& = Ice::Current());
  virtual void ReloadStaticPages(const Ice::Current& = Ice::Current());
  virtual int GetVideoViewPower(const Ice::Current& = Ice::Current());
  virtual void SetVideoViewPower(int value, const Ice::Current& = Ice::Current());
  virtual PageCacheCallStatList GetPageCacheCallStat(const Ice::Current& = Ice::Current()) {
    return _stat.GetStat();
  }
  virtual void ClearPageCacheCallStat(const Ice::Current& = Ice::Current()) {
    _stat.ClearCount();
  }

  void SetFriendListLimit(int oneget, int ceilget, int rate, const Ice::Current& = Ice::Current()) {
    _one_get_limit = oneget;
    _max_get_limit = ceilget;
    _rate_friend_info = rate;
    MCE_INFO("PageCacheManagerI::SetFriendListLimit one_get_limit "
             <<  _one_get_limit << " max_get_limit " << _max_get_limit
             << " rate " << _rate_friend_info);


  }
  IntSeq GetFriendListLimit(const Ice::Current& = Ice::Current()) {
    IntSeq sq;
    sq.push_back(_one_get_limit);
    sq.push_back(_max_get_limit);
    sq.push_back(_rate_friend_info);
    return sq;
  }
  int OneGetLimit() {return _one_get_limit;}
  int MaxGetLimit() {return _max_get_limit;}
  int RateFriendInfo() {return _rate_friend_info;}

  void loadConfig(const string& path);
  BuddyPtr MakeBuddyById(int userid, int mucstatus);
  TemplateDataCollectorPtr getTemplateCollector(const string& path);
  bool VerifyIsGuide(int uid);
  void UpCallCount(int cate, int sep) {
    _stat.UpCount(cate, sep);
  }

private:
  int _video_view_power;
  IceUtil::Mutex _mutex;
  IceUtil::RWRecMutex _video_power_mutex;
  map<string,Config> _config;
  map<string,TemplateDataCollectorPtr> _collectors;
  PageCacheCallStat _stat;
  int _one_get_limit;
  int _max_get_limit;
  int _rate_friend_info;

};

}
}

#endif /*PAGECACHEMANAGERI_H_*/
