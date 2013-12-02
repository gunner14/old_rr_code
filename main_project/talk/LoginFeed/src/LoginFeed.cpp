#include "LoginFeed.h"
#include "QueryRunner.h"
#include <time.h>
#include "BuddyFriendsAdapter.h"
#include "FeedCreatorAdapter.h"
#include "FeedMemcProxy/client/talk_cache_client.h"
#include "pugixml.hpp"
#include "PhotoUrlTools.h"

using namespace std;
using namespace xce::xntalk;
using namespace xce::talk;
using namespace xce::feed;
using namespace xce::buddy::adapter;
using namespace talk::online::adapter;
using namespace com::xiaonei::xce;
using namespace com::xiaonei::talk::common;
using namespace com::xiaonei::talk::util;
using namespace MyUtil;
using namespace pugi;

void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&LoginFeedManagerI::instance(), service.createIdentity("M", ""));
 /*    
  ServiceI& service = ServiceI::instance();
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
  xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerIMIdGenerator", 
      &IMIdGeneratorManagerI::instance(), mod, interval, new XceFeedControllerChannel());
  MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);
*/
  LoginFeedManagerI::instance().InitalizeUserInfo();
  PhotoUrlHelper::instance().StartReloadTimer();
  TaskManager::instance().scheduleRepeated(new SendFeedTimer);
  TaskManager::instance().scheduleRepeated(new LoadLoginUsersTimer);
  TaskManager::instance().scheduleRepeated(new LoginFeedDBTimer);

//  LoginFeedManagerI::instance().SendInstallFeed(225521695, "41611");
 // LoginFeedManagerI::instance().SendFeed(225521695);
}

/*说明：xml转义
 */
string XmlUnescape(const string &xml) {
  const int xml_len = xml.length();
  ostringstream os;
  for (int i=0; i<xml_len;) {
    if (xml[i] == '&') {
      if (xml.substr(i+1,4) =="amp;") { os << "&";
        i += 5;
      } else if (xml.substr(i+1,3) =="lt;") {
        os << "<";
        i += 4;
      } else if (xml.substr(i+1,3) == "gt;") {
        os << ">";
        i += 4;
      } else if (xml.substr(i+1,5) == "quot;") {
        os << "\"";
        i += 6;
      } else if (xml.substr(i+1,5) == "apos;") {
        os << "\'";
        i += 6;
      } else
      os << xml[i++];
    } else 
      os << xml[i++];
  }
  return os.str();
}

/*说明：获取当前的偏移日期
 */
string GetDate(const Ice::Int offset) {
  time_t t = time(NULL);
  t -= offset*24*3600;
  struct tm *tm = localtime(&t);
  
  char date[64] = {0};
  snprintf(date, 64, "%d%02d%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
  return string(date);
}

int GetHour() {
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  return tm->tm_hour;
};
int GetDay() {
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  return tm->tm_mday;
};

/*说明：从文件中读取数据
 *参数1：文件路径
 *参数2：分隔符
 *参数3：返回值
 *返回值: 是否成功
 */
bool GetDataFromFile(const string &file_path, const string &division, map<string,string> &info) {
  string line;
  ifstream ifile(file_path.c_str()); 
  if (!ifile) { 
    MCE_WARN("GetDateFromFile   Fail to open file ,path:" << file_path);
    return false;
  }
  while (getline(ifile, line)) {    
    if (line=="")  
      continue;
    int note_pos = line.find("//");
    if (note_pos == string::npos)
      note_pos = line.length();    
    string content = line.substr(0, note_pos);

    MCE_DEBUG("GetDateFromFile   content:"<< content);
    int eq_pos = content.find(division);
    if (eq_pos != string::npos) {
      string key = content.substr(0,eq_pos);
      string value =content.substr(eq_pos+1, content.length()-eq_pos);
      if (key!="" && value!="")
        info[key] = value;
    }
  }
  return true;
}

/*说明：读配置文件
 */
bool LoginFeedManagerI::LoadInstallFeedConfig() {
  xml_document doc;
  if (!doc.load_file(INSTALL_FEED_CONFIG_PATH.c_str())) {
    MCE_WARN("LoginFeedManagerI::LoadInstallFeedConfig   xml error.  info:" << INSTALL_FEED_CONFIG_PATH);
    return false;
  }
  map<string, map<string, string> > temp_install_feed_config;
  for (xml_node feed_node=doc.child("config").child("feed"); feed_node; feed_node=feed_node.next_sibling()) {
    string feed_id = feed_node.child("id").first_child().value();
    if (feed_id == "")
      continue;
    map<string, string> temp_feed_config;
    temp_feed_config.insert(pair<string,string>("title.prefix", XmlUnescape(feed_node.child("title").child("prefix").first_child().value())));
    temp_feed_config.insert(pair<string,string>("title.suffix", XmlUnescape(feed_node.child("title").child("suffix").first_child().value())));
    temp_feed_config.insert(pair<string,string>("title.content", XmlUnescape(feed_node.child("title").child("content").first_child().value())));
    temp_feed_config.insert(pair<string,string>("title.url", XmlUnescape(feed_node.child("title").child("url").first_child().value())));
    temp_feed_config.insert(pair<string,string>("text.href", XmlUnescape(feed_node.child("text").child("href").first_child().value())));
    temp_feed_config.insert(pair<string,string>("text.content", XmlUnescape(feed_node.child("text").child("content").first_child().value())));
    temp_feed_config.insert(pair<string,string>("image.href", XmlUnescape(feed_node.child("image").child("href").first_child().value())));
    temp_feed_config.insert(pair<string,string>("image.src", XmlUnescape(feed_node.child("image").child("src").first_child().value())));
    temp_feed_config.insert(pair<string,string>("actionLink1.name", XmlUnescape(feed_node.child("actionlink1").child("name").first_child().value())));
    temp_feed_config.insert(pair<string,string>("actionLink1.url", XmlUnescape(feed_node.child("actionlink1").child("url").first_child().value())));
    temp_feed_config.insert(pair<string,string>("actionLink2.name", XmlUnescape(feed_node.child("actionlink2").child("name").first_child().value())));
    temp_feed_config.insert(pair<string,string>("actionLink2.url", XmlUnescape(feed_node.child("actionlink2").child("url").first_child().value())));
    temp_install_feed_config.insert(pair<string, map<string,string> >(feed_id, temp_feed_config));
  }
  {
  	IceUtil::RWRecMutex::WLock lock(install_feed_config_mutex_);
    install_feed_config_.swap(temp_install_feed_config);
    ////////////////////////
    for (map<string, map<string, string> >::const_iterator type_iter=install_feed_config_.begin(); type_iter!=install_feed_config_.end(); type_iter++) {
      MCE_DEBUG("LoginFeedManagerI::LoadInstallFeedConfig   type:" << type_iter->first);
      for (map<string,string>::const_iterator content_iter=(type_iter->second).begin(); content_iter!=(type_iter->second).end(); ++content_iter) {
        MCE_DEBUG("LoginFeedManagerI::LoadInstallFeedConfig   name:" << content_iter->first <<"    content:" << content_iter->second);
      }
    }
    ////////////////////
  }
  return true;
}

/*说明：创建新鲜事数据
 */
void CreateGraduationFeedParas(const string &receivers,const TalkUserPtr talk_user,const Ice::Long send_time, Str2StrMap &props) {
  ostringstream os;
  os << (send_time*1000);
  MCE_DEBUG("CreateGraduationFeedParas   uid:" << talk_user->id << "   time:" << os.str() << "  receivers:" << receivers);
  props.insert(pair<string,string>("time", os.str()));
  os.str("");
  props.insert(pair<string,string>("expr", receivers)); 
  os << talk_user->id;
  props.insert(pair<string,string>("from.0.id", os.str()));
  os.str("");
  props.insert(pair<string,string>("from.0.name", talk_user->name));
  /*
  if (talk_user->tinyurl=="0/0/tiny.jpg")
    os <<"http://head.xiaonei.com/photos/0/0/men_tiny.gif";
  else
    os << "http://hdn.xnimg.cn/photos/" << talk_user->tinyurl;
    */
  string headurl = PhotoUrlHelper::instance().GetFullUrl(talk_user->tinyurl);
  props.insert(pair<string,string>("from.0.tinyimg",headurl));
  os.str("");
  os << "http://www.renren.com/profile.do?id="<<talk_user->id;
  props.insert(pair<string,string>("from.0.url", os.str()));
  os.str("");
  props.insert(pair<string,string>("title.prefix", "刚刚登录人人桌面，获得了"));
  props.insert(pair<string,string>("title.content", "20倍积分、20个礼券、7天礼物会员"));
  props.insert(pair<string,string>("title.url", "http://im.renren.com/huodong/byj.html?feed"));
  props.insert(pair<string,string>("title.suffix", ""));
  props.insert(pair<string,string>("text.href", ""));
  props.insert(pair<string,string>("text.content", "疯狂毕业趴，礼物拿到你手软！仅此一天！！登录人人桌面即可领取毕业大礼包！更可体验隐身登录、无痕查看等多项独家功能！<a href=\"http://im.renren.com/huodong/byj.html?feed\">立即领取&gt;&gt;</a>"));
  //props.insert(pair<string,string>("text.content", "疯狂毕业趴，礼物拿到你手软！仅此一天！！登录人人桌面即可领取毕业大礼包！更可体验隐身登录、无痕查看等多项独家功能！&lt;a href=&quot;http://im.renren.com/huodong/byj.html?feed&quot;&gt;立即领取>>&lt;/a&gt;"));
  props.insert(pair<string,string>("image.src", "http://gadget.talk.renren.com/images/graduation.png"));
  props.insert(pair<string,string>("image.href", "http://im.renren.com/huodong/byj.html?feed"));
  props.insert(pair<string,string>("actionLink1.name", "查看详情"));
  props.insert(pair<string,string>("actionLink1.url", "http://im.renren.com/huodong/byj.html?feed"));
  props.insert(pair<string,string>("actionLink2.name", "立即下载人人桌面"));
  props.insert(pair<string,string>("actionLink2.url", "http://im.renren.com/desktop/rrsetup-23.exe?feed"));
  props.insert(pair<string,string>("version", "5555"));
}

/*说明：创建登录新鲜事
 */
void CreateLoginFeedParas(const string &receivers,const TalkUserPtr talk_user,const Ice::Long send_time, Str2StrMap &props) {
  ostringstream os;
  os << (send_time*1000);
  MCE_DEBUG("CreateLoginFeedParas   uid:" << talk_user->id << "   time:" << os.str() << "  receivers:" << receivers);
  props.insert(pair<string,string>("time", os.str()));
  os.str("");
  props.insert(pair<string,string>("expr", receivers)); 
  os << talk_user->id;
  props.insert(pair<string,string>("from.0.id", os.str()));
  os.str("");
  props.insert(pair<string,string>("from.0.name", talk_user->name));
  //os << "http://hdn.xnimg.cn/photos/" << talk_user->tinyurl;
  string headurl = PhotoUrlHelper::instance().GetFullUrl(talk_user->tinyurl);
  props.insert(pair<string,string>("from.0.tinyimg", headurl));
  os.str("");
  os << "http://www.renren.com/profile.do?id="<<talk_user->id;
  props.insert(pair<string,string>("from.0.url", os.str()));
  os.str("");
  props.insert(pair<string,string>("title.prefix", "登录了"));
  props.insert(pair<string,string>("title.content", "人人桌面！"));
  props.insert(pair<string,string>("title.url", "http://im.renren.com/?subver=59&t01"));
  props.insert(pair<string,string>("title.suffix", ""));
  props.insert(pair<string,string>("text.href", ""));
  props.insert(pair<string,string>("text.content", "这么多好友都在用人人桌面，隐身登录、保存密码、无痕查看、回复签名等特权多多！快来体验吧！"));
  props.insert(pair<string,string>("image.src", "http://gadget.talk.renren.com/images/feed.jpg"));
  props.insert(pair<string,string>("image.href", "http://im.renren.com/?subver=59&p01"));
  props.insert(pair<string,string>("actionLink1.name", "了解详情"));
  props.insert(pair<string,string>("actionLink1.url", "http://im.renren.com/?subver=59&w01"));
  props.insert(pair<string,string>("actionLink2.name", "立即下载"));
  props.insert(pair<string,string>("actionLink2.url", "http://im.renren.com/desktop/rrsetup-59.exe?b01"));
  props.insert(pair<string,string>("version", "4444"));
}

string LoginFeedManagerI::GetLoginTable(const Ice::Int offset) {      
  ostringstream os;
  os << XNTALK_LOGIN_TABLE_PREFIX << GetDate(offset);
  return os.str();
}

/*说明：初始化用户信息
 */
void LoginFeedManagerI::InitalizeUserInfo() {
  login_users_.reset(new UsersBitmap());
  if (!login_users_->Initialize()) {
    MCE_WARN("LoginFeedManagerI::InitalizeUserInfo    initialize login_users_ error");
    login_users_.reset();
    return ;
  }
  TaskManager::instance().execute(new LoadLoginUsersTask);
  LoadLoginFeedConfig();
  LoadInstallFeedConfig();
  LoadReceivers();
}

void LoginFeedManagerI::ReloadConfig(const Ice::Current& current) {
  LoadLoginFeedConfig();
  LoadInstallFeedConfig();
  LoadReceivers();
}

/*说明：载入登录新鲜事配置
 */
bool LoginFeedManagerI::LoadLoginFeedConfig() {
  bool ret = true;
  {
  	IceUtil::RWRecMutex::WLock lock(config_info_mutex_);
    ret =  GetDataFromFile(CONFIG_PATH, "=", config_info_);
    for (map<string,string>::iterator iter=config_info_.begin(); iter!=config_info_.end(); ++iter) {  
      MCE_INFO("LoginFeedManagerI::LoadConfig     key:" <<iter->first <<"     value:"<<iter->second);
    }
  }
  return ret;
}

/*说明：读取接受者
 */
bool LoginFeedManagerI::LoadReceivers() {
  int senders_all = 1;
  {
	  IceUtil::RWRecMutex::RLock lock(config_info_mutex_);
    map<string, string>::const_iterator iter = config_info_.find("senders_all");
    if (iter != config_info_.end()) 
      senders_all = atoi((iter->second).c_str());
  }
  if (senders_all == 0) {
    TimeStat ts;
    map<string, string> user_version;
    if (!GetDataFromFile(RECEIVERS_PATH, " ", user_version))
      return false;
    for (map<string, string>::const_iterator iter=user_version.begin(); iter!=user_version.end(); ++iter) {
      Ice::Long uid = atoi((iter->first).c_str());
      UserInfoPtr user_info = new UserInfo();
      user_info->version_ = iter->second;

      IceUtil::Mutex::Lock lock(user_info_mutex_[GetPoolId(uid)]);
      user_info_pool_[GetPoolId(uid)][uid] = user_info;
    }
    MCE_INFO("LoginFeedManagerI::LoadReceivers  receivers.size:" <<user_version.size()<< " time cost:" << ts.getTime());    
  }
  return true;
}

/*说明：载入登录用户
 */
bool LoginFeedManagerI::LoadLoginUsers() {
  int login_user_period = 3;
  {
	  IceUtil::RWRecMutex::RLock lock(config_info_mutex_);
    map<string, string>::const_iterator iter = config_info_.find("login_user_period");
    if (iter != config_info_.end()) {
      login_user_period = atoi((iter->second).c_str());
    }
  }
  TimeStat ts;
//  set<Ice::Long> login_users;
  UsersBitmapPtr temp_login_users(new UsersBitmap());
  if (!temp_login_users->Initialize()) {
    MCE_WARN("LoginFeedManagerI::LoadLoginUsers    initialize login_users error");
    return false;
  }

  for (int i=0; i<login_user_period; ++i) {
    const string date = GetDate(i+1);
    const string login_user_table = GetLoginTable(i+1);
    for (int j=0; j<24; ++j) {
      Statement msg_sql;
      msg_sql << "select distinct(userid) from "<< login_user_table << " where action='talk_login' and ('" << date << (j<10?"0":"")<<j <<"0000"
        <<"')<time and ('" << date << (j<10?"0":"")<<j <<"5959')>time";
      try {
        mysqlpp::StoreQueryResult res = QueryRunner(LOGIN_INSTANCE, CDbRServer, login_user_table).store(msg_sql);
        if (!res) {
          MCE_WARN("LoginFeedManagerI::LoadLoginUsers --> load message error.  login_user_table:" << login_user_table
              << "    date:" <<date);
          return false;
        } 
        for (int i=0; i<res.num_rows(); ++i) {
          mysqlpp::Row row = res.at(i);
//          MCE_DEBUG("LoginFeedManagerI::LoadLoginUsers() uids:" << (Ice::Long)row["userid"]);
//          login_users.insert((Ice::Long)row["userid"]);
          temp_login_users->Set((Ice::Int)row["userid"]);
        }
      } catch(Ice::Exception &e) {    
        MCE_WARN(e << " at " << __FILE__ << ":" << __LINE__);
        return false;
      } catch (std::exception &e) {
        MCE_WARN(e.what() << " at " << __FILE__ << ":" << __LINE__);
        return false;
      } catch (...) {
        MCE_WARN("Unkown exception at " << __FILE__ << ":" << __LINE__);
        return false;
      }
    }
  }
//  UsersBitmapPtr temp;
  {
    IceUtil::RWRecMutex::WLock lock(login_users_mutex_);
//    temp = login_users_;
    login_users_ = temp_login_users;
//    login_users.swap(login_users_);          
  }

  MCE_INFO("LoginFeedManagerI::LoadLoginUsers  login_users_.size:" << login_users_->Size() << "   cost time:" << ts.getTime());
  return true;
}

void LoginFeedManagerI::SwapDBLoadUsers(vector<DBLoadUserPtr> &db_load_users, int i) {
  IceUtil::Mutex::Lock lock(db_load_users_mutex_[i]);
  db_load_users_[i].swap(db_load_users);
}

/*说明：载入用户新鲜事历史
 *参数1：用户talk版本号
 *参数2：用户在线状态
 *返回值：检测结果
 */
bool LoginFeedManagerI::CheckLoginUser(vector<string> &vers,OnlineStat &os) {
  bool ret = true;
  vector<string>::const_iterator iter;
  for (iter=vers.begin(); iter!=vers.end(); ++iter) {
    if ((*iter)=="4.059")
     break; 
  }
  if (iter == vers.end()) {
    MCE_DEBUG("LoginFeedManagerI::CheckLoginUser   ver wrong.");
    ret = false;
  }
  if (os == OFFLINE || os ==INVISIABLE) {
    MCE_DEBUG("LoginFeedManagerI::CheckLoginUser   onlinestat wrong.");
    ret = false;
  }

  return ret;
}

/*说明：对指定id 发送新鲜事
  */
void LoginFeedManagerI::SendGraduationFeed(const Ice::Long uid) {
  {
    IceUtil::Mutex::Lock lock(yesterday_login_mutex_);
    MCE_DEBUG("LoginFeedManagerI::SendGraduationFeed    day:" << GetDay());
    if (GetDay() != 15)
      return; 
    if (yesterday_login_.find(uid) != yesterday_login_.end()) {
      MCE_DEBUG("LoginFeedManagerI::SendGraduationFeed   already exists");
      return;
    }
    yesterday_login_.insert(uid);
  }
  Str2StrMap feed_config;
  ostringstream receivers;
  receivers << "f(" << uid << ")";  
  TalkUserPtr talk_user =TalkCacheClient::instance().GetUserByIdWithLoad(uid); 
  CreateGraduationFeedParas(receivers.str(), talk_user, time(NULL), feed_config); 
  ////////////
  for (map<string, string>::const_iterator iter=feed_config.begin(); iter!=feed_config.end(); ++iter) {
    MCE_DEBUG("LoginFeedManagerI::SendInstallFeed   name:" <<iter->first<<"    value:"<<iter->second);
  }
  //////////
  MCE_INFO("LoginFeedManagerI::SendGraduationFeed   uid:"<<uid);
  try {
    FeedCreatorAdapter::instance().Create(1012,1,feed_config);
  } catch(Ice::Exception& e){
		MCE_WARN("LoginFeedManagerI::SendInstallFeed-->FeedCreatorAdapter::Create -->   error:" << e);
  }
}

/*说明：对指定id 发送新鲜事
  */
void LoginFeedManagerI::SendFeed(Ice::Long uid,  const Ice::Current& current) {
//  SendGraduationFeed(uid);

  MCE_INFO("LoginFeedManagerI::SendFeed  uid:" << uid);
  int send_frequency = 14;
  int senders_all = 1;
  {
	  IceUtil::RWRecMutex::RLock lock(config_info_mutex_);
    map<string, string>::const_iterator iter = config_info_.find("send_frequency");
    if (iter != config_info_.end()) {
      send_frequency = atoi((iter->second).c_str());
    }
    iter = config_info_.find("senders_all");
    if (iter != config_info_.end()) 
      senders_all = atoi((iter->second).c_str());
  }
  {
    OnlineStat os = ONLINE;
    vector<string> vers;
    AvatarSeq avatars;
    try {
      avatars = OnlineCenterAdapter::instance().getUserAvatars(uid);
    }catch(Ice::Exception& e){
	  	MCE_WARN("LoginFeedManagerI::SendFeed-->OnlineCenter --> userid:" << uid << " error:" << e);
      return ;
    }
    AvatarSeq::const_iterator iter=avatars.begin();
    for (; iter!=avatars.end(); iter++) {
      vers.push_back((*iter)->ver);
      if (os > (*iter)->stat)
        os = (*iter)->stat;
    }
    if (!CheckLoginUser(vers, os))
      return;

    SendFeedUserPtr send_feed_user = new SendFeedUser;
    send_feed_user->uid = uid;
    string version;
    time_t current_time = 0;
    bool send_feed_flag = false;
    {
      IceUtil::Mutex::Lock lock(user_info_mutex_[GetPoolId(uid)]);
      map<Ice::Long, UserInfoPtr>::const_iterator user_info_pool_iter = user_info_pool_[GetPoolId(uid)].find(uid);
      if (user_info_pool_iter == user_info_pool_[GetPoolId(uid)].end()) {
        if (senders_all == 1) {
          UserInfoPtr user_info = new UserInfo;
          user_info->login_time_ = time(NULL);
          pair< map<Ice::Long, UserInfoPtr>::iterator , bool> insert_pair = user_info_pool_[GetPoolId(uid)].insert(pair<Ice::Long, UserInfoPtr>(uid, user_info));

          if (insert_pair.second) {
            DBLoadUserPtr db_load_user = new DBLoadUser();
            db_load_user->uid = uid;
            db_load_user->iter = insert_pair.first;
            MCE_DEBUG("LoginFeedManagerI::SendFeed dodo load db_  uid:" <<uid);
            IceUtil::Mutex::Lock lock(db_load_users_mutex_[GetPoolId(uid)]);
            db_load_users_[GetPoolId(uid)].push_back(db_load_user);
          }
        } 
      } else {
        current_time = time(NULL);
    //    if (current_time - user_info_pool_iter->second->login_time_ > send_frequency*10) { 
        if (current_time - user_info_pool_iter->second->login_time_ > send_frequency*24*3600) { 
          version = user_info_pool_iter->second->version_;
    //      send_feed_user->send_time = user_info_pool_iter->second->login_time_ = current_time;
          send_feed_flag = true;
        }
      }
    }
    if (!version.empty()) {
      vector<string>::const_iterator iter;
      for (iter=vers.begin(); iter!=vers.end(); ++iter) {
        if (version.find(*iter)!=string::npos)
         break; 
      }
      if (iter == vers.end())
        send_feed_flag = false;
    }
    if (send_feed_flag) {
      {
        IceUtil::Mutex::Lock lock(user_info_mutex_[GetPoolId(uid)]);
        map<Ice::Long, UserInfoPtr>::const_iterator user_info_pool_iter = user_info_pool_[GetPoolId(uid)].find(uid);
        if (user_info_pool_iter != user_info_pool_[GetPoolId(uid)].end())
          user_info_pool_iter->second->login_time_ = current_time;
      }
      {
        IceUtil::Mutex::Lock lock(send_feed_users_mutex_[GetPoolId(uid)]);
        send_feed_user->send_time = current_time;
        send_feed_users_[GetPoolId(uid)].push_back(send_feed_user);   
        MCE_DEBUG("LoginFeedManagerI::SendFeed dodo send feed_  uid:" <<send_feed_user->uid << "   time:" << send_feed_user->send_time);
      }
    }
  }
}

/*说明：对指定id 发送新鲜事
  */
void LoginFeedManagerI::SendInstallFeed(Ice::Long uid, const std::string &version, const Ice::Current&) {
  MCE_INFO("LoginFeedManagerI::SendInstallFeed    uid:" <<uid <<"   version:" <<version);
  Str2StrMap feed_config;
  {
    IceUtil::RWRecMutex::RLock lock(install_feed_config_mutex_);
    map<string, map<string, string> >::const_iterator iter=install_feed_config_.find(version);
    if (iter == install_feed_config_.end()) {
      MCE_INFO("LoginFeedManagerI::SendInstallFeed   uid:" <<uid<<"   version:"<<version<<"    message:not found config.");
      return;
    }
    feed_config = iter->second;
  }
  ostringstream receivers;
  /*
  Int2IntSeqMap friends;
  IntSeq ids;
  ids.push_back(uid);
  try {
    friends= BuddyFriendsCacheAdapter::instance().getFriendLists(ids);
    MCE_DEBUG("LoginFeedManagerI::SendInstallFeed     uid:"<< uid << "    friends.size:" << friends.size());
  }catch(Ice::Exception& e){
  	MCE_WARN("LoginFeedManagerI::SendInstallFeed -->BuddyFriendsCacheAdapter::getFriendLists ids-->size:" << ids.size() << " error:" << e);
  }
  const int receivers_limit = SEND_FRIENDS_SIZE>friends[uid].size() ? friends[uid].size(): SEND_FRIENDS_SIZE;
  for (int j=0; j<receivers_limit; ++j) {
    receivers <<  "u("  << friends[uid][j]<< ")";
    if ((j+1)!=receivers_limit)
      receivers << "+";
  } 
  */
  receivers << "f(" << uid << ")";  
  TalkUserPtr talk_user =TalkCacheClient::instance().GetUserByIdWithLoad(uid); 
  ostringstream os;
  os << (time(NULL)*1000);
  feed_config.insert(pair<string,string>("time", os.str()));
  os.str("");
  feed_config.insert(pair<string,string>("expr", receivers.str())); 
  os << talk_user->id;
  feed_config.insert(pair<string,string>("from.0.id", os.str()));
  os.str("");
  feed_config.insert(pair<string,string>("from.0.name", talk_user->name));
  /*
  if (talk_user->tinyurl=="0/0/tiny.jpg")
    os <<"http://head.xiaonei.com/photos/0/0/men_tiny.gif";
  else
    os << "http://hdn.xnimg.cn/photos/" << talk_user->tinyurl;
  */
  string headurl = PhotoUrlHelper::instance().GetFullUrl(talk_user->tinyurl);
  MCE_DEBUG(" LoginFeedManagerI::SendInstallFeed   os:"<<os.str() <<"    tinyurl:" <<talk_user->tinyurl<<"   uid:"<<uid);
  feed_config.insert(pair<string,string>("from.0.tinyimg", headurl));
  os.str("");
  os << "http://www.renren.com/profile.do?id="<<talk_user->id;
  feed_config.insert(pair<string,string>("from.0.url", os.str()));
  os.str("");
  feed_config.insert(pair<string,string>("version", version));
  ////////////
  for (map<string, string>::const_iterator iter=feed_config.begin(); iter!=feed_config.end(); ++iter) {
    MCE_DEBUG("LoginFeedManagerI::SendInstallFeed   name:" <<iter->first<<"    value:"<<iter->second);
  }
  //////////
  MCE_INFO("LoginFeedManagerI::SendInstallFeed   uid:"<<uid);
  try {
    FeedCreatorAdapter::instance().Create(1012,1,feed_config);
  } catch(Ice::Exception& e){
		MCE_WARN("LoginFeedManagerI::SendInstallFeed-->FeedCreatorAdapter::Create -->   error:" << e);
  }
}

bool LoginFeedManagerI::LoadUsersFeedTime(vector<DBLoadUserPtr> &db_load_users, map<Ice::Long, Ice::Long> &users_login_time) {
  const int db_load_users_size = db_load_users.size();
  const int db_unit_size = 5000; 
  Statement sql;
  for (int i=0; i<db_load_users_size ; ++i) {
    if (i%db_unit_size == 0) {
      sql.clear();
      sql << "SELECT uid, days from login_feed where ";
    }

    if ((i+1)/db_unit_size==0 || (i+1)==db_load_users_size) {
      sql << " uid=" <<db_load_users[i]->uid;
      try {
        mysqlpp::StoreQueryResult res = QueryRunner(FEED_INSTANCE, CDbRServer, LOGINFEED_TABLE).store(sql);
        if (!res) {
          MCE_WARN("LoginFeedManagerI::LoadUsersFeedTime --> load message error.  LOGINFEED_TABLE:" << LOGINFEED_TABLE);
          return false;
        } 
        for (int i=0; i<res.num_rows(); ++i) {
          mysqlpp::Row row = res.at(i);
          users_login_time[(Ice::Long)row["uid"]] = (Ice::Long)row["days"];
        }
      } catch(Ice::Exception &e) {    
        MCE_WARN(e << " at " << __FILE__ << ":" << __LINE__);
        return false;
      } catch (std::exception &e) {
        MCE_WARN(e.what() << " at " << __FILE__ << ":" << __LINE__);
        return false;
      } catch (...) {
        MCE_WARN("Unkown exception at " << __FILE__ << ":" << __LINE__);
        return false;
      }
    } else
      sql << " uid=" <<db_load_users[i]->uid << " or ";
  } 
  return true;
}
  
void LoginFeedManagerI::SwapSendFeedUsers(vector<SendFeedUserPtr> &send_feed_users, int i) {
  IceUtil::Mutex::Lock lock(send_feed_users_mutex_[i]);
  send_feed_users_[GetPoolId(i)].swap(send_feed_users);
}

/*说明：对指定id 发送新鲜事
  */
bool LoginFeedManagerI::UpdateSendUsers(std::vector<SendFeedUserPtr> &send_feed_users) {
  const int send_feed_users_size = send_feed_users.size();
  const int db_unit_size = 2000; 
  Statement sql;
  for (int i=0; i<send_feed_users_size; ++i) {
    if (i%db_unit_size == 0) {
      sql.clear();
      sql << "INSERT INTO "<<LOGINFEED_TABLE<<" values ";
    }

    if ((i+1)/db_unit_size==0 || (i+1)==send_feed_users_size) {
      sql << " (" << (send_feed_users.at(i))->uid << "," << send_feed_users.at(i)->send_time << ") ON DUPLICATE KEY UPDATE days=VALUES(days)";
      try {
        QueryRunner(FEED_INSTANCE, CDbWServer, LOGINFEED_TABLE).store(sql);
      } catch(Ice::Exception &e) {    
        MCE_WARN(e << " at " << __FILE__ << ":" << __LINE__);
        return false;
      } catch (std::exception &e) {
        MCE_WARN(e.what() << " at " << __FILE__ << ":" << __LINE__);
        return false;
      } catch (...) {
        MCE_WARN("Unkown exception at " << __FILE__ << ":" << __LINE__);
        return false;
      }
    } else
      sql << " (" << (send_feed_users.at(i))->uid << "," << send_feed_users.at(i)->send_time << "),";
  }  
  return true;
}

/*说明：对指定用户发送新鲜事
  */
void LoginFeedManagerI::SendFeeds(vector<SendFeedUserPtr> &send_feed_users) {
  vector<Ice::Int> ids;
  for (int i=0; i<send_feed_users.size(); ++i) {
    MCE_DEBUG("LoginFeedManagerI::SendFeeds  uid:" << send_feed_users[i]->uid);
    if ((ids.size()!=0 && ids.size()%5==0) || (i+1)==send_feed_users.size()) {
      if ((i+1)==send_feed_users.size())
        ids.push_back(send_feed_users[i]->uid);
      Int2IntSeqMap friends;
      try {
        friends= BuddyFriendsCacheAdapter::instance().getFriendLists(ids);
        MCE_DEBUG(" friends.size:" << friends.size());
  	  }catch(Ice::Exception& e){
  	  	MCE_WARN("LoginFeedManagerI::SendFeed -->BuddyFriendsCacheAdapter::getFriendLists ids-->size:" << ids.size() << " error:" << e);
  	  }
      ids.clear();
      Int2IntSeqMap receivers_list;
      {
        if (login_users_) {
          IceUtil::RWRecMutex::RLock lock(login_users_mutex_);
          for (Int2IntSeqMap::const_iterator friends_iter=friends.begin(); friends_iter!=friends.end(); friends_iter++) {
            const Ice::Long uid = friends_iter->first;
            IntSeq receivers;
            for (IntSeq::const_iterator id_iter=(friends_iter->second).begin(); id_iter!=(friends_iter->second).end(); id_iter++) {
           //   if (login_users_.find(*id_iter)==login_users_.end())
              if (!login_users_->Get(*id_iter))
                receivers.push_back(*id_iter);        
            }
            MCE_INFO("uid:"<<uid  << "  receivers.size:" << receivers.size());
            receivers_list[uid] = receivers;   
          }
        } else
          MCE_WARN("LoginFeedManagerI::SendFeeds   receivers_list empty");
      }
      for (Int2IntSeqMap::const_iterator receivers_list_iter=receivers_list.begin(); receivers_list_iter!=receivers_list.end();++receivers_list_iter) {
        TalkUserPtr talk_user =TalkCacheClient::instance().GetUserByIdWithLoad(receivers_list_iter->first); 
        ostringstream receivers;
        const int receivers_limit = SEND_FRIENDS_SIZE>(receivers_list_iter->second).size() ? (receivers_list_iter->second).size(): SEND_FRIENDS_SIZE;
        for (int j=0; j<receivers_limit; ++j) {
          receivers <<  "u("  << (receivers_list_iter->second)[j]<< ")";
          if ((j+1)!=receivers_limit)
            receivers << "+";
        }
        Str2StrMap props;
        MCE_INFO("LoginFeedManagerI::SendLoginFeed-   uid:" << send_feed_users[i]->uid <<"    time:"<<send_feed_users[i]->send_time);
        CreateLoginFeedParas(receivers.str(), talk_user, send_feed_users[i]->send_time, props);
        try {
          FeedCreatorAdapter::instance().Create(1012,1,props);
        } catch(Ice::Exception& e){
	      	MCE_WARN("LoginFeedManagerI::SendFeed-->FeedCreatorAdapter::Create -->   error:" << e);
        }
      }
    }
    else
      ids.push_back(send_feed_users[i]->uid);
  }
}

void LoginFeedManagerI::SendUsersFeed() {
  TimeStat st;
  for (int i=0; i<POOL_TUB_SIZE; ++i) {
    vector<SendFeedUserPtr> send_feed_users;
    SwapSendFeedUsers(send_feed_users, i);
    SendFeeds(send_feed_users);
    UpdateSendUsers(send_feed_users);
  } 
  MCE_INFO("LoginFeedManagerI::SendUsersFeed  time cost:" << st.getTime());
}

void LoginFeedManagerI::ClearGraduationLogin() {
  IceUtil::Mutex::Lock lock(yesterday_login_mutex_);
  set<Ice::Long>().swap(yesterday_login_);
}

/*说明：载入用户新鲜事历史
 */
void LoginFeedManagerI::LoadUsersFeedHistory() {
  TimeStat ts;
  int send_frequency = 14;
  {
	  IceUtil::RWRecMutex::RLock lock(config_info_mutex_);
    map<string, string>::const_iterator iter = config_info_.find("send_frequency");
    if (iter != config_info_.end()) {
      send_frequency = atoi((iter->second).c_str());
    }
  }
  for (int i=0; i<POOL_TUB_SIZE; ++i) {
    vector<DBLoadUserPtr> db_load_users;
    SwapDBLoadUsers(db_load_users, i);
    
    if (!db_load_users.empty())
      MCE_DEBUG("LoginFeedManagerI::LoadUsersFeedHistory  dodo  size:" << db_load_users.size() << "  i:" <<i);

    map<Ice::Long, Ice::Long> users_login_time;
    LoadUsersFeedTime(db_load_users, users_login_time);
         
    vector<SendFeedUserPtr> send_users;
    {
      IceUtil::Mutex::Lock lock(user_info_mutex_[i]);
      for (vector<DBLoadUserPtr>::const_iterator db_load_users_iter=db_load_users.begin(); db_load_users_iter!=db_load_users.end(); ++db_load_users_iter) {
        SendFeedUserPtr send_feed_user = new SendFeedUser;
        send_feed_user->uid = (*db_load_users_iter)->uid;
        send_feed_user->send_time = time(NULL);
        map<Ice::Long, Ice::Long>::const_iterator time_iter=users_login_time.find(send_feed_user->uid); 
        if (time_iter!=users_login_time.end()) {
          MCE_DEBUG("LoginFeedManagerI::LoadUsersFeedHistory   uid:" << send_feed_user->uid <<"   found in db;")
      //    if (send_feed_user->send_time-time_iter->second < send_frequency*10)
          if (send_feed_user->send_time-time_iter->second < send_frequency*24*3600)
            continue;
        }
        send_users.push_back(send_feed_user); 
        (*db_load_users_iter)->iter->second->login_time_ = send_feed_user->send_time;
      }
    }
    if (!send_users.empty()) {
      IceUtil::Mutex::Lock lock(send_feed_users_mutex_[i]);
      send_feed_users_[i].insert(send_feed_users_[i].end(), send_users.begin(), send_users.end());
    }
  }
  MCE_INFO("LoginFeedManagerI::LoadUsersFeedHistory   time cost:" << ts.getTime());
}

void SendFeedTimer::handle() {
  MCE_DEBUG("SendFeedTimer::handle dodo");
  LoginFeedManagerI::instance().SendUsersFeed() ;
}

void LoginFeedDBTimer::handle() {
  MCE_DEBUG("LoginFeedDBTimer::handle dodo");
  LoginFeedManagerI::instance().LoadUsersFeedHistory();
}

void ReloadConfigTimer::handle() {
  MCE_DEBUG("ReloadConfigTimer::handle dodo");
  LoginFeedManagerI::instance().ReloadConfig();
}
void LoadLoginUsersTimer::handle() {

  int hour = GetHour();
  if (hour == 3) 
    TaskManager::instance().execute(new LoadLoginUsersTask);
  else if (hour == 2)
    LoginFeedManagerI::instance().ClearGraduationLogin();
}

void LoadLoginUsersTask::handle() {
  LoginFeedManagerI::instance().LoadLoginUsers();
}
