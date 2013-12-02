#include "IMWindowManagerI.h"
#include "HttpUtil.h"
#include <boost/lexical_cast.hpp>
#include <ctemplate/template.h>
#include <ctemplate/template_dictionary.h>
#include "Notify/util/fcgi_request.h"
#include "XmppTools.h"
#include "pugixml.hpp"
#include "TalkDeliverAdapter.h"
#include "MessageType.h"
#include "OnlineCenterAdapter.h"
#include "TalkFunStateAdapter.h"
//#include "ActiveUserNotify.h"
#include "MessageMsgHandlerAdapter.h"
#include "QueryRunner.h"
#include "TalkFunStateAdapter.h"

#include "XNTalkLoginFeedAdapter.h"
using namespace com::xiaonei::talk::fun::state;
using namespace talk::online::adapter;
using namespace mop::hi::svc::adapter;
using namespace com::xiaonei::xce;
using namespace com::xiaonei::talk::util;
using namespace talk::window;
using namespace MyUtil;
using namespace xce::adapter::userpassport;
using namespace xce::xntalk;

using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::common;
using namespace talk::adapter;

void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  Ice::PropertiesPtr props = service.getCommunicator()->getProperties();
	service.getAdapter()->add(&IMWindowManagerI::instance(), service.createIdentity("M", ""));
  string fcgi_socket = props->getPropertyWithDefault("Service." + service.getName() + ".FcgiSocket", "0.0.0.0:9001");
  MCE_INFO("Fcgi listens on : " << fcgi_socket);
  FcgiServer * fcgi_server = new FcgiServer(fcgi_socket, 128);
  fcgi_server->RegisterRequestFactory(new IMWindowRequestFactory());
  fcgi_server->Start();

	string configFilePath = props->getPropertyWithDefault("Service.ConfigFilePath", "/data/xce/XNTalk/etc/IMWindow/IMWindow.conf");
	string whiteList = props->getPropertyWithDefault("Service.WhiteList", "/data/xce/XNTalk/etc/IMWindow/whitelist");
	string blackList = props->getPropertyWithDefault("Service.BlackList", "/data/xce/XNTalk/etc/IMWindow/blacklist");
	string appList = props->getPropertyWithDefault("Service.AppList", "/data/xce/XNTalk/etc/IMWindow/applist");
	IMWindowManagerI::instance().Initialize(configFilePath, whiteList, blackList, appList);
  TaskManager::instance().scheduleRepeated(new GetConfigTimer());
  TaskManager::instance().scheduleRepeated(new SendAdTimer());
//  TaskManager::instance().scheduleRepeated(new LoadUsersTimer());
  TaskManager::instance().scheduleRepeated(new SendApplistTimer());
  TaskManager::instance().scheduleRepeated(new InstantWindowTimer());
  TaskManager::instance().scheduleRepeated(new SendNotifyTimer());
}

int GetHour() {
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  return tm->tm_hour;
};

RequestPtr IMWindowRequestFactory::Create(FCGX_Request * r) {
	char * path = FCGX_GetParam("SCRIPT_NAME", r->envp);
	if (path) {
		return new IMWindowRequest(r);
	}
	return NULL;
}

void IMWindowRequest::ErrorResponse(int status_code){
	ostringstream res;
	if(404 == status_code){
		res << "HTTP/1.1 404 Not Found" << "Content-type: text/html\r\n\r\n SORRY NOT FOUND";
	}
	else if(403 == status_code){
		res << "HTTP/1.1 404 Not Found" << "Content-type: text/html\r\n\r\n SORRY PLEASE LOGIN FIRST";
	}
	FCGX_PutS(res.str().c_str(), _fcgi_out);
}
bool IMWindowRequest::response() {
  if (!_fcgi_out) {
    MCE_WARN("_fcgi_out null");
    return false;
  }

	/*/---------------------------------------------------
	MCE_INFO("IMWindowRequest::response --> _login_uid = " << _login_uid << " _path = " << _path);
	for(Str2StrMap::iterator it = _cookies.begin(); it != _cookies.end(); ++it){
		MCE_INFO("IMWindowRequest::response --> key:" << it->first << " value:" << it->second);
	}
	for(Str2StrMap::iterator it = _props.begin(); it != _props.end(); ++it){
		MCE_INFO("IMWindowRequest::response --> key:" << it->first << " value:" << it->second);
	}
	//---------------------------------------------------*/


	stringstream res;
	PathConfigPtr config = IMWindowManagerI::instance().GetConfigByPath(_path, TYPE_ALL_WND);
	
	if(!config){
		ErrorResponse(404);
		MCE_WARN("IMWindowRequest::response --> could not found the path");
		return false;
	}
	ctemplate::Template* tpl = ctemplate::Template::GetTemplate(config->template_path, ctemplate::DO_NOT_STRIP);
	if (!tpl) {
		ErrorResponse(404);
		MCE_WARN("IMWindowRequest::response --> could not found the tpl path:" << config->template_path);
		return false;
	}
	else{
		if("template" != config->type){
			ErrorResponse(404);
			MCE_WARN("IMWindowRequest::response --> type is wrong type:" << config->type)
			return false;
		}
		TemplateDataCollectorPtr collector = IMWindowManagerI::instance().GetTempCollector(_path);
		if(!collector){
			ErrorResponse(404);
			MCE_WARN("IMWindowRequest::response --> could not found the collector")
			return false;
		}
		ctemplate::TemplateDictionary dict("page_template");
		bool success = collector->fillData(_path, _cookies, _props, dict);
		if(!success){
			ErrorResponse(403);
			MCE_WARN("IMWindowRequest::response --> userinfo error")
			return false;
		}
    res << "HTTP/1.1 200 " << GetReasonPhrase(200) << "\r\n"
        << "Content-type: " << config->content_type<< "\r\n" << "Cache-Control: no-cache"<< "\r\n" << "Expires: Thu, 01 Jan 1970 00:00:00 GMT"<< "\r\n";
		string ans;
		tpl->Expand(&ans, &dict);
		res << "\r\n" << ans;
	}
	//MCE_INFO(res.str());
  FCGX_PutS(res.str().c_str(), _fcgi_out);
  return true;
}


PathConfigPtr IMWindowManagerI::GetConfigByPath(const string& path, const int wnd_type){
	PathConfigPtr config;
  vector<AdInfo>::const_iterator iter;
	IceUtil::RWRecMutex::RLock lock(ad_config_rwmutex_);
  if (wnd_type==TYPE_DELAY_WND || wnd_type==TYPE_ALL_WND) {
    for (iter=ad_config_pool_.begin(); iter!=ad_config_pool_.end(); iter++) {
      if (iter->url == path) {
        config = iter->ad_config;
        break;
      }
    }
  }
  if (wnd_type==TYPE_ALL_WND && config)
    return config;
  if (wnd_type==TYPE_INSTANT_WND || wnd_type==TYPE_ALL_WND) {
    for (iter=instant_config_pool_.begin(); iter!=instant_config_pool_.end(); iter++) {
      if (iter->url == path) {
        config = iter->ad_config;
        break;
      }
    }
  }
  return config;
}
vector<string> IMWindowManagerI::GetAdPathsFromPool(const UserInfoPtr &user_info,const time_t current_time) {
  vector<string> ad_paths;
	IceUtil::RWRecMutex::RLock lock(ad_config_rwmutex_);

//  MCE_DEBUG("IMWindowManagerTestI::GetAdPathsFromPool   WhiteListCheck:" << user_info->user_id   
//      <<"  WhiteListCheck(user_info->user_id):"<< WhiteListCheck(user_info->user_id) );
//  MCE_DEBUG("IMWindowManagerTestI::GetAdPathsFromPool   BlackListCheck:" << user_info->user_id 
//      <<"  BlackListCheck(user_info->user_id):"<< BlackListCheck(user_info->user_id) );
  for (vector<AdInfo>::const_iterator iter=ad_config_pool_.begin(); iter!=ad_config_pool_.end(); iter++) {
    if ((iter->ad_config->xntalk_ver=="all" || iter->ad_config->xntalk_ver.find(user_info->xntalk_ver)!=string::npos) &&
        (iter->ad_config->whitelist=="0" || WhiteListCheck(user_info->user_id)) &&
        (iter->ad_config->blacklist=="0" || !BlackListCheck(user_info->user_id)) &&
        (iter->ad_config->account_status=="all" || user_info->account_status==iter->ad_config->account_status) &&
        ((iter->ad_config->start_date==0&&iter->ad_config->expiration_date==0) || (iter->ad_config->start_date<=current_time&&iter->ad_config->expiration_date>current_time))
        ) {
//      MCE_DEBUG("IMWindowManagerTestI::GetAdPathsFromPool   whitelist:"<<iter->ad_config->whitelist << "   blacklist:" << iter->ad_config->blacklist);
//      MCE_DEBUG("IMWindowManagerTestI::GetAdPathsFromPool   url:"<<iter->url);
      ad_paths.push_back(iter->url);
    }
  }
  return ad_paths; 
}

TemplateDataCollectorPtr IMWindowManagerI::GetTempCollector(const string& path){
	if("/imuserinfo/getuserinfo.do" == path){
		return new UserInfoCollector();
	}
	else if(!(path.find("/imadinfo"))){
		return new StaticAdInfoCollector();
	}
	return NULL;
}

void IMWindowManagerI::GetAdConfigsFromFile(vector<AdInfo> &ad_config_pool, vector<AdInfo> &instant_config_pool,int &ad_list) {
  MCE_DEBUG("IMWindowManagerTestI::GetAdConfigsFromFile   start");
	xml_document doc;
	if (doc.load_file(config_file_path_.c_str())) {
		xml_node configNode = doc.child("config");
    ad_list = atoi(configNode.child("ad_list").first_child().value().c_str());
//		for (xml_node n = configNode.child("page"); n; n = n.next_sibling("page")) {
		for (xml_node n = configNode.first_child(); n; n = n.next_sibling()) {
      if (strlen(n.name()) == 0)
        continue;
			string url = n.attribute("url").value();
			if (url != "") {
//			if(ad_config_pool_.count(url) <= 0)
        AdInfo ad;
				PathConfigPtr c = new PathConfig();
				c->template_path = n.attribute("file").value();
				c->timeout = boost::lexical_cast<int>(n.attribute("timeout").value());
				c->type = n.attribute("type").value();
				c->content_type = n.attribute("content_type").value();
				c->reminder_type= n.attribute("reminder_type").value();
				c->width= n.attribute("width").value();
				c->height= n.attribute("height").value();
				c->url= n.attribute("get_url").value();
				c->showtime= n.attribute("showtime").value();
				c->topmost= n.attribute("topmost").value();
				c->title= n.attribute("title").value();
				c->xntalk_ver = n.attribute("xntalk_version").value();
				c->whitelist = n.attribute("whitelist").value();
				c->blacklist = n.attribute("blacklist").value();
				c->account_status = n.attribute("account_status").value();

        c->start_date = c->expiration_date = 0;
        string start_date = n.attribute("start_date").value();
        string expiration_date = n.attribute("expiration_date").value();
        if (start_date!="" && expiration_date!="") {
          tm s,e;
          if (strptime(start_date.c_str(),"%Y-%m-%d %H:%M:%S",&s) && strptime(expiration_date.c_str(),"%Y-%m-%d %H:%M:%S",&e)) {
            c->start_date = mktime(&s);
            c->expiration_date = mktime(&e);
          }
        }
        /*
				c->age = n.attribute("age").value();
				c->gender= n.attribute("gender").value();
				c->region = n.attribute("region").value();
				c->stage = n.attribute("stage").value();
        */
        ad.url = url;
        ad.ad_config = c;
        if (!strcmp(n.name(),"page"))
          ad_config_pool.push_back(ad);
        else if (!strcmp(n.name(), "instance"))
          instant_config_pool.push_back(ad);
//				ad_config_pool_[url] = c;
			}
		}
	}
  MCE_INFO("IMWindowManagerTestI::GetAdConfigsFromFile   ad_config_pool.size:" << ad_config_pool.size() << "  instant_config_pool.size:"<<instant_config_pool.size());
//  for (map<string, PathConfigPtr>::iterator iter=ad_config_pool_.begin(); iter!=ad_config_pool_.end(); iter++) 
//    MCE_INFO("  url:"<<iter->first);
}

bool IMWindowManagerI::GetUserListFromFile(const string file_path, set<Ice::Int> &temp_users) {
  TimeStat ts;
  bool ret =true;
  ifstream ifile(file_path.c_str()) ; 
  if (!ifile) { 
    MCE_WARN("IMWindowManagerI::GetReceiversFromFile   Fail to open file ,path:" << file_path);
    ret = false;
  }
  if (ret) {
    char buf[128];
    for(int i=0; !ifile.eof(); i++) {
      if (ifile.getline(buf , sizeof(buf)-1).good()) {
        Ice::Int id = atoi(buf);
        if (id >0)
          temp_users.insert(id); 
      } else {
        ret = false;
        break;
      }
    }
  }
  MCE_INFO("IMWindowManagerI::GetWhiteListFromFile----> usetime : " << ts.getTime() << "temp_users.size:" << temp_users.size() << " file_path:" << file_path);
  return ret;
}

MessageSeq IMWindowManagerI::GetXmppMessage(const PathConfigPtr& config, const Ice::Int& uid){
	JidSeq jids;
	try{
		jids = TalkFunStateAdapter::instance().getUserJids(uid, 4);
	}catch(Ice::Exception& e){
		MCE_WARN("IMWindowManagerI::GetXmppMessage-->OnlineCenterAdapter::getUserJids-->uid:" << uid << " error:" << e);
	}

	MessageSeq ans;
	for(JidSeq::iterator vit = jids.begin(); vit != jids.end(); ++vit){
		MessagePtr msg = GetXmppMessage(config, (*vit));
		ans.push_back(msg);
	}
	return ans;
}
MessageSeq IMWindowManagerI::GetXmppMessage(const PathConfigPtr& config, const MyUtil::IntSeq& userids){
	JidSeqMap jids;
	try{
		jids =  TalkFunStateAdapter::instance().getUsersJids(userids, 4);
	}catch(Ice::Exception& e){
		MCE_WARN("IMWindowManagerI::GetXmppMessage-->OnlineCenterAdapter::getUsersJids-->size:" << userids.size() << " error:" << e);
	}
	MessageSeq ans;
	for(JidSeqMap::iterator mit = jids.begin(); mit != jids.end(); ++mit){
		for(JidSeq::iterator vit = mit->second.begin(); vit != mit->second.end(); ++vit){
			MessagePtr msg = GetXmppMessage(config, (*vit));
			ans.push_back(msg);
		}
	}
	return ans;
}
MessagePtr IMWindowManagerI::GetXmppMessage(const PathConfigPtr& config, const JidPtr& jid, const bool xml_flag){
	MCE_INFO("IMWindowManagerI::GetXmppMessage--> jid:" << jidToString(jid));
	xml_document doc;
	xml_node messageNode = doc.append_child();
	messageNode.set_name("message");
  if (xml_flag)
  	messageNode.append_attribute("type").set_value("reminderxml");
  else
  	messageNode.append_attribute("type").set_value("reminder");
	messageNode.append_attribute("from").set_value(jidToString(jid).c_str());
	xml_node remindNode = messageNode.append_child();
  if (xml_flag) {
  	remindNode.set_name("reminderxml");
	  remindNode.append_attribute("xmlns").set_value("http://talk.renren.com/reminderxml");
  } else {
	  remindNode.set_name("reminder");
	  remindNode.append_attribute("xmlns").set_value("http://talk.renren.com/reminder");
  }

	remindNode.append_attribute("type").set_value(config->reminder_type.c_str());
	
	xml_node widthNode = remindNode.append_child();
	widthNode.set_name("width");
	widthNode.append_child(node_pcdata).set_value(config->width.c_str());

	xml_node heightNode = remindNode.append_child();
	heightNode.set_name("height");
	heightNode.append_child(node_pcdata).set_value(config->height.c_str());


	xml_node urlNode = remindNode.append_child();
  if (xml_flag)
	  urlNode.set_name("xml");
  else 
	  urlNode.set_name("url");
	urlNode.append_child(node_pcdata).set_value(config->url.c_str());
	
	xml_node showtimeNode = remindNode.append_child();
	showtimeNode.set_name("showtime");
	showtimeNode.append_child(node_pcdata).set_value(config->showtime.c_str());

	xml_node topmostNode = remindNode.append_child();
	topmostNode.set_name("topmost");
	topmostNode.append_child(node_pcdata).set_value(config->topmost.c_str());


	xml_node titleNode = remindNode.append_child();
	titleNode.set_name("title");
	titleNode.append_child(node_pcdata).set_value(config->title.c_str());

	MessagePtr msg = new Message();
	msg->from = jid;
	msg->to = jid;
	msg->type = NOTIFY2_MESSAGE;
	msg->msg = doc.xml();

	return msg;
}

void LoginFeed(int uid) {
  MCE_INFO("IMWindowManagerI::LoginFeed    uid:" << uid);
  TimeStat ts;
  try {
    LoginFeedAdapter::instance().SendFeed(uid);
  }catch(Ice::Exception& e){
		MCE_WARN("IMWindowManagerI::LoginFeed   userid:" << uid << " error:" << e);
  }
  FunStatManager::instance().Stat("IMWindow.LoginFeed", ts.getTime(), (ts.getTime() > 250.0));
}
//////////////////////////////
bool IMWindowManagerI::WhiteListCheck(const Ice::Int userid) {
	IceUtil::RWRecMutex::RLock lock(userlist_mutex_);
  return whitelist_.find(userid) != whitelist_.end();
}
bool IMWindowManagerI::BlackListCheck(const Ice::Int userid) {
	IceUtil::RWRecMutex::RLock lock(userlist_mutex_);
  return blacklist_.find(userid) != blacklist_.end();
}

void IMWindowManagerI::ReloadApplist(){
	IceUtil::RWRecMutex::WLock lock(applist_mutex_);
  if (!applist_doc_.load_file(applist_file_path_.c_str())) {
    MCE_WARN("IMWindowManagerI::ReloadApplist   xml error.  path:" << applist_file_path_);
    return;
  }
  /*
  applist_node_ = applist_doc_.child("applist");
  if(!applist_node_) {
    MCE_DEBUG("IMWindowManagerI::ReloadApplist applist_node_ empty.");
    return ;
  }
  */
  /*
	IceUtil::RWRecMutex::WLock lock(applist_mutex_);
  xml_document doc;
  if (!doc.load_file(applist_file_path_.c_str())) {
    MCE_WARN("IMWindowManagerI::ReloadApplist   xml error.  path:" << applist_file_path_);
    return;
  }

  applist_node_ = doc.child("applist");
  if(!applist_node_) {
    MCE_DEBUG("IMWindowManagerI::ReloadApplist applist_node_ empty.");
    return ;
  }*/
  MCE_INFO("IMWindowManagerI::ReloadApplist applist_node_  doc:" <<applist_doc_.xml());
}

void IMWindowManagerI::SwapInstantWindowMsg(MessageSeq &mseq) {
  IceUtil::Mutex::Lock  lock(instant_wnd_mutex_);
  instant_wnd_mseq_.swap(mseq);
}

void IMWindowManagerI::SwapApplistMsg(MessageSeq &mseq) {
  IceUtil::Mutex::Lock  lock(applist_mseq_mutex_);
  applist_mseq_.swap(mseq);
}
void IMWindowManagerI::SendApplist(const string &info, const JidPtr& to_jid, const Ice::Current&) {
  MCE_INFO("MWindowManagerI::SendApplist  id:" << to_jid->userId << "    info:"<<info);
  TimeStat ts;
  xml_document doc;
  if (!doc.load(info.c_str())) {
    MCE_WARN("IMWindowManagerI::SendApplist   xml error.  info:" << info);
    return;    
  }
  {
    IceUtil::RWRecMutex::RLock lock(applist_mutex_);
    if (!applist_doc_) {
      MCE_WARN("IMWindowManagerI::SendApplist   applist_doc_ empty.");
      return;
    }
    if (!doc.child("iq").child("query").append_copy(applist_doc_.child("applist"))) {
      MCE_WARN("IMWindowManagerI::SendApplist   doc.append_copy error.   xml:"<<doc.xml());
      return ;
    }  
  }
  MessagePtr msg = new Message;
	msg->from = to_jid;
	msg->to = to_jid;
	msg->type = NOTIFY2_MESSAGE;
	msg->msg = doc.xml();
  IceUtil::Mutex::Lock lock(applist_mseq_mutex_);
  applist_mseq_.push_back(msg);
  FunStatManager::instance().Stat("IMWindowManagerI::SendApplist ", ts.getTime(), (ts.getTime() > 250.0));
}

MessageSeq IMWindowManagerI::GetNotifyMessage(const NotifyInfoPtr& ni, const int uid) {
  MessageSeq seqs;
  if (!ni)
    return seqs;
	JidSeq jids;
	try{
		jids =  TalkFunStateAdapter::instance().getUserJids(uid, 4);
	}catch(Ice::Exception& e){
		MCE_WARN("IMWindowManagerI::GetNotifyMessage-->OnlineCenterAdapter::getUserJids-->uid:" << uid << "    jids.size" << jids.size()<< " error:" << e);
    return seqs;
	}

	xml_document doc;
	xml_node messageNode = doc.append_child();
	messageNode.set_name("message");
	messageNode.append_attribute("to").set_value((boost::lexical_cast<string>(uid)).c_str());
	messageNode.append_attribute("from").set_value("feed.talk.renren.com@feed.talk.renren.com");
	messageNode.append_attribute("type").set_value("common");
	xml_node xfeedNode = messageNode.append_child();
	xfeedNode.set_name("xfeed");
	xfeedNode.append_attribute("xmlns").set_value("http://talk.renren.com/xfeed");
	xfeedNode.append_attribute("offline").set_value("0");

	xml_node stypeNode = xfeedNode.append_child();
	stypeNode.set_name("stype");
	stypeNode.append_child(node_pcdata).set_value(ni->stype.c_str());
  xml_node fNode = xfeedNode.append_child();
	fNode.set_name("f");
	fNode.append_attribute("version").set_value("1");
	fNode.append_attribute("tpl").set_value("cs");
  
	xml_node iconUrlNode = fNode.append_child();
	iconUrlNode.set_name("icon");
	iconUrlNode.append_child(node_pcdata).set_value(ni->iconUrl.c_str());

	xml_node iconLink = fNode.append_child();
	iconLink.set_name("icon-link");
	iconLink.append_child(node_pcdata).set_value(ni->iconLink.c_str());

	xml_node textNode = fNode.append_child();
	textNode.set_name("text");
	textNode.append_child(node_pcdata).set_value(ni->text.c_str());
	
	xml_node textLinkNode = fNode.append_child();
	textLinkNode.set_name("url");
	textLinkNode.append_child(node_pcdata).set_value(ni->textLink.c_str());

	xml_node xNode = fNode.append_child();
	xNode.set_name("size-x");
	xNode.append_child(node_pcdata).set_value(ni->sizeX.c_str());
	xml_node yNode = fNode.append_child();
	yNode.set_name("size-y");
	yNode.append_child(node_pcdata).set_value(ni->sizeY.c_str());

  if (uid == 225521695)
  MCE_INFO("IMWindowManagerI::GetNotifyMessage--->  uid:" << uid <<"   jid.size:" << jids.size() << "   xml:" << doc.xml());
	MessageSeq ans;
	for(JidSeq::iterator vit = jids.begin(); vit != jids.end(); ++vit){
	  MessagePtr msg = new Message();
	  msg->from = (*vit);
	  msg->to = (*vit);
	  msg->type = NOTIFY2_MESSAGE;
	  msg->msg = doc.xml();
		ans.push_back(msg);
	}
	return ans;
}

void IMWindowManagerI::ShowNotify(const NotifyInfoPtr &ni, const int uid, const Ice::Current& ) {
  TimeStat ts;
  MCE_INFO("IMWindowManagerI::ShowNotify  icon:" << ni->iconUrl << "     uid:"<<uid);
  MessageSeq temp_seq = GetNotifyMessage(ni, uid);
  MCE_INFO("IMWindowManagerI::ShowNotify  temp_seq.size:" << temp_seq.size() << "     uid:"<<uid);

  IceUtil::Mutex::Lock lock(notify_mseq_mutex_);
  notify_mseq_.insert(notify_mseq_.end(), temp_seq.begin(), temp_seq.end());
  FunStatManager::instance().Stat("IMWindowManagerI::ShowNotify ", ts.getTime(), (ts.getTime() > 250.0));
}

void IMWindowManagerI::SwapNotifyMsg(MessageSeq &mseq) {
  IceUtil::Mutex::Lock  lock(notify_mseq_mutex_);
  notify_mseq_.swap(mseq);
}
void IMWindowManagerI::ShowWindow(const string &window_title, const int uid, const Ice::Current&) {
  MCE_INFO("IMWindowManagerI::ShowWindow  window:" << window_title << "     uid:"<<uid);
  if (window_title == "")
    return;
  {
    IceUtil::Mutex::Lock lock(instant_users_mutex_);
    pair< set<int>::iterator, bool > pr = instant_users_.insert(uid);
    if (!(pr.second)) {
      MCE_DEBUG("IMWindowManagerI::ShowWindow  return uid:" << uid);
      return;
    }
  }
  const string path = "/imadinfo/" + window_title+".do";
  PathConfigPtr config;
  config = GetConfigByPath(path, TYPE_INSTANT_WND); 
  if (!config) {
    MCE_DEBUG("IMWindowManagerI::ShowWindow  cofig empty");
    return;
  }
  MessageSeq mseq = GetXmppMessage(config, uid);
  MCE_DEBUG("IMWindowManagerI::ShowWindow  path:" << path <<"   uid:" << uid <<"    mseq:" <<mseq.size() <<"    templath_path:" << config->template_path);
  
  IceUtil::Mutex::Lock  lock(instant_wnd_mutex_);
  instant_wnd_mseq_.insert(instant_wnd_mseq_.end(),mseq.begin(), mseq.end());
}

//   临时增加的老版本更新弹窗实现函数
void IMWindowManagerI::DeliverUpdateNote( ) {
    try {
    MessageMsgHandlerAdapter::instance().deliverAppNotice(usrid_, 3205, "人人桌面","http://app.xnimg.cn/application/20100225/17/55/Lb7Sl5d018153.gif", "<br/>&nbsp;&nbsp;<a href=\"http://im.renren.com/?subver=4\"> &nbsp;&nbsp;&nbsp;&nbsp;【人人桌面升级提示！！】</a><br/><br/>&nbsp;&nbsp;<a href=\"http://im.renren.com/?subver=4\"> 亲爱的用户，我们检测到你正在使用旧版本的人人桌面，为了向您提供更好的功能和服务，我们强烈建议你升级到新版人人桌面。</a><br/><br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href = \"http://im.renren.com/desktop/rrsetup-1.exe\">立即升级>></a>");
    } catch (Ice::Exception& e) {
       MCE_DEBUG("IMWindowManagerI::UserAdapterReminder-->deliverAppNotice error<<e");
    }

}

void IMWindowManagerI::IMWindowReminder(const JidPtr& jid,  const Ice::Current&){
  LoginFeed(jid->userId);
  TimeStat ts;
	MCE_INFO("IMWindowManagerI::UserAdReminder-->userid:" << jid->userId << " endpoint:"<<jid->endpoint<< "  index:"<<jid->index);
  {
    IceUtil::Mutex::Lock lock(statistic_mutex_);
    ++total_access_;
  }
 /* 
  if (WhiteListCheck(jid->userId)) {
		MCE_INFO("IMWindowManagerI::IMWindowReminder--> WhiteListCheck found:userid:" << jid->userId);
  } else
    return;
   */ 
  time_t current_time = time(NULL);
  UserInfoPtr user_info = new UserInfo();
  user_info->user_id = jid->userId;
  usrid_=jid->userId;
  user_info->xntalk_ver = "0";
  user_info->account_status = "1";
  AvatarPtr avatar;
  try {
    avatar = TalkFunStateAdapter::instance().getAvatar(jid);
  }catch(Ice::Exception& e){
		MCE_WARN("IMWindowManagerI::UserAdReminder-->OnlineCenter --> userid:" << jid->userId << " error:" << e);
  }
  if (avatar != NULL) 
    user_info->xntalk_ver = avatar->ver;
  else
	  MCE_DEBUG("IMWindowManagerI::UserAdReminderOnly-->  avatar == null:");

  if (user_info->xntalk_ver == "0") {
	  MCE_DEBUG("IMWindowManagerI::UserAdReminderOnly-->head_TalkDeliverAdapter::deliver-->userid:" << jid->userId<< "    xntalk_ver:"<<user_info->xntalk_ver );
    return ;
  }
  //临时增加老版更新提示弹窗
  if(user_info->xntalk_ver == "4.038"||user_info->xntalk_ver == "4.037"||user_info->xntalk_ver == "4.027"
     ||user_info->xntalk_ver == "4.024"||user_info->xntalk_ver == "4.018") {
        DeliverUpdateNote();  
        return;
  }
	UserPassportInfoPtr passport_info;
	try{
		passport_info = UserPassportAdapter::instance().getUserPassport(jid->userId);
	}catch(Ice::Exception& e){
		MCE_WARN("IMWindowManagerI::UserAdReminder--> call UserPassportAdapter err " << e << "    uid:"<< jid->userId);
	}
	int safe_status = 0;
	if(passport_info){
		safe_status = passport_info->safestatus();
	}
  if(3==safe_status || 4==safe_status || 5==safe_status){
    user_info->account_status = "0";
    MCE_WARN("IMWindowManagerI::UserAdReminder --> safe_status err, " << safe_status << "    uid:"<< jid->userId << "  account_status:" << user_info->account_status);
  }

  IceUtil::Mutex::Lock lock(user_ad_mutex_[GetBufferIndex(jid->userId)]);
  map<int, UserAdInfoPtr>::iterator iter = user_ad_cache_[GetBufferIndex(jid->userId)].find(jid->userId);
  if (iter == user_ad_cache_[GetBufferIndex(jid->userId)].end()) {
    UserAdInfoPtr user_ad_info = new UserAdInfo();
    user_ad_info->user_info = user_info;
    user_ad_cache_[GetBufferIndex(jid->userId)][jid->userId] = user_ad_info;
	  MCE_DEBUG("IMWindowManagerI::UserAdReminder-->imwindow login.. userid:" << jid->userId  << "     i:"<<GetBufferIndex(jid->userId));
  } else {
//    (iter->second)->first_send_time = (iter->second)->last_send_time = current_time;
    (iter->second)->last_send_time = current_time;
    (iter->second)->last_send_status = 1;
    (iter->second)->user_info->xntalk_ver = user_info->xntalk_ver;
    (iter->second)->user_info->account_status = user_info->account_status;
  }
  FunStatManager::instance().Stat("IMWindow.IMWindowReminder", ts.getTime(), (ts.getTime() > 250.0));
}


void IMWindowManagerI::Initialize(const string& config_file_path, const string& whitelist_file_path, const string& blacklist_file_path, const string& applist_file_path) {
  config_file_path_ = config_file_path;
  whitelist_file_path_ = whitelist_file_path;
  blacklist_file_path_ = blacklist_file_path;
  applist_file_path_ = applist_file_path;
  current_day_ = GetCurrentDay();
  UpdateConfiguration();
//  TaskManager::instance().execute(new LoadUsersTask);
//  TaskManager::instance().execute(new LoadWhiteListTask);
  IceUtil::Mutex::Lock lock(statistic_mutex_);
  total_access_ = 0;
}

/*说明：重载配置信息
 */
void IMWindowManagerI::UpdateConfiguration() {
  TimeStat ts;
  vector<AdInfo> ad_config_pool;
  vector<AdInfo> instant_config_pool;
  int ad_list = 1;
  GetAdConfigsFromFile(ad_config_pool,instant_config_pool, ad_list);

  set<Ice::Int> whitelist, blacklist;
  GetUserListFromFile(whitelist_file_path_ , whitelist);
  GetUserListFromFile(blacklist_file_path_ , blacklist);
  ReloadApplist();
  {
	  IceUtil::RWRecMutex::WLock lock(userlist_mutex_);
    whitelist.swap(whitelist_);
    blacklist.swap(blacklist_);
  }
  {
	  IceUtil::RWRecMutex::WLock lock(ad_config_rwmutex_);
    ad_list_ = ad_list;
    ad_config_pool.swap(ad_config_pool_);
    instant_config_pool.swap(instant_config_pool_);
  }
  MCE_INFO("IMWindowManagerI::UpdateConfiguration   time cost:" << ts.getTime());
}

void IMWindowManagerI::ClearInstantUsers() {
  IceUtil::Mutex::Lock lock(instant_users_mutex_);
  set<Ice::Int>().swap(instant_users_);
}

int IMWindowManagerI::GetCurrentDay() {
  time_t tmp = time(NULL);
  tm* t = localtime(&tmp);
  return t->tm_wday;
}

/*说明：重载配置信息
 */
void IMWindowManagerI::ReloadConfig(const Ice::Current& ) {
  UpdateConfiguration();
}

/*说明：定时检测当前登录用户信息，决定时候进行弹窗
 */
void IMWindowManagerI::SendAds(/*bool reset*/) {
  MCE_INFO("IMWindowManagerI::SendAds");
  bool remove_flag = false;
  int temp_day = GetCurrentDay();
  if (temp_day != current_day_) {
    remove_flag = true;  
    current_day_ = temp_day;

    IceUtil::Mutex::Lock lock(statistic_mutex_);
    MCE_INFO("IMWindowManagerI::SendAds total_access:" << total_access_);
    total_access_ = 0;
  }
  
//  if (reset)
//    remove_flag = true;
  int total, remove_count, reset_count;;
  total = remove_count = reset_count = 0;
  TimeStat ts; 
  time_t current_time = time(NULL);
  IntSeq send_ids;
  map<int, string> temp_urls;
  map<int, PathConfigPtr> temp_config;
  map<int, UserAdInfoPtr>::iterator temp_iter;
  for (int i=0; i<POOL_TUB_SIZE; ++i) {
    IceUtil::Mutex::Lock lock(user_ad_mutex_[i]);
    MCE_INFO("IMWindowManagerI::SendAds  i:" << i <<"    user_ad_cache_[i].size:" << user_ad_cache_[i].size());
    total += user_ad_cache_[i].size();
    for (map<int, UserAdInfoPtr>::iterator iter=user_ad_cache_[i].begin(); iter!=user_ad_cache_[i].end(); ) {
      if (remove_flag && iter->second->last_send_status<=0) {
        temp_iter = iter;
        iter++;
        user_ad_cache_[i].erase(temp_iter);
        remove_count++;
        MCE_DEBUG("IMWindowManagerI::SendAds  remove_flag && iter->second->last_send_status==0   id:" << iter->first);
        continue;
      }
      if (iter->second->sended_ads.size()<5 && (iter->second->last_send_status==1||iter->second->last_send_status==-1)
          && ((iter->second->first_send_time==iter->second->last_send_time && current_time>=iter->second->last_send_time+ 60*2 /* 20*/)
          || current_time>=iter->second->last_send_time+ 60*30 /*60*/) ) {
        vector<string> ads = GetAdPathsFromPool(iter->second->user_info, current_time);
        if (iter->first == 225521695)
          MCE_INFO("IMWindowManagerI::SendAds  ads:" << ads.size());
        MCE_DEBUG("IMWindowManagerI::SendAds user_id" <<iter->second->user_info->user_id << " ads.size:" << ads.size());
        if (ads.size() == 0) {
          if (remove_flag) {
            MCE_DEBUG("IMWindowManagerI::SendAds  ads.size=0&&remove_flag");
            temp_iter = iter;
            user_ad_cache_[i].erase(temp_iter);
          }
          iter++;
          MCE_DEBUG("IMWindowManagerI::SendAds  ads.size=0");
          continue;
        }
        int m, n;
        string temp_url;
        for (m=0; m<ads.size(); ++m) {
          for (n=0; n<iter->second->sended_ads.size(); ++n) {
            if (ads[m] == iter->second->sended_ads[n])
              break;
          }
          if (n ==iter->second->sended_ads.size())
            temp_url = ads[m];
        }
        if (iter->first == 225521695)
          MCE_INFO("IMWindowManagerI::SendAds  temp_urls:" << temp_url);
        if (temp_url != "") {
          temp_urls[iter->first] = temp_url;
          iter->second->last_send_status = 0;
          send_ids.push_back(iter->first);
        } else
          iter->second->last_send_status = -1;
      }
      iter++;
    }
  }
  MCE_INFO("IMWindowManagerI::SendAds  total:" << total << "   remove_count:"<<remove_count << "   reset_count:"<<reset_count <<"   temp_urls.size" << temp_urls.size());
  for (map<int, string>::iterator temp_iter=temp_urls.begin(); temp_iter!=temp_urls.end(); temp_iter++) {
    if (temp_iter->first == 225521695)
    MCE_INFO("IMWindowManagerI::MMSendAds  id:"<<temp_iter->first<<"    url:"<<temp_iter->second);
  }
  remove_flag = false;
  {
    for (map<int, string>::const_iterator iter=temp_urls.begin(); iter!=temp_urls.end(); iter++) {
      PathConfigPtr config;
      config = GetConfigByPath(iter->second, TYPE_DELAY_WND); 
//      config = ad_config_pool_[iter->second]; 
      if (config)
        temp_config[iter->first] = config;
    }
  }

  for (map<int, PathConfigPtr>::iterator temp_iter1=temp_config.begin(); temp_iter1!=temp_config.end(); temp_iter1++) {
    if (temp_iter1->first == 225521695)
    MCE_INFO("IMWindowManagerI::MMSendAds  id:"<<temp_iter1->first<<"   config:"<<temp_iter1->second);
  }
  MCE_INFO("IMWindowManagerI::SendAds temp_config.size :" << temp_config.size() <<"    send_ids:" <<send_ids.size());
  if (temp_config.size() > 0 && send_ids.size()>0) {
  	JidSeqMap jids;
    JidSeqMap temp_jids;
    IntSeq temp_send_ids;
    for (int send_ids_pos=0; send_ids_pos<send_ids.size(); send_ids_pos++) {
      temp_send_ids.push_back(send_ids.at(send_ids_pos));
      if (send_ids_pos%500==0 || send_ids_pos==send_ids.size()-1) {
  	    try{
  	    	temp_jids = TalkFunStateAdapter::instance().getUsersJids(temp_send_ids, 4);
  	    }catch(Ice::Exception& e){
  	    	MCE_WARN("SendAdTimer::handle-->OnlineCenterAdapter::getUsersJids-->size:" << send_ids.size() << " error:" << e);
  	    }
        jids.insert(temp_jids.begin(), temp_jids.end());
        temp_jids.clear();
        temp_send_ids.clear();
      }  
    }
    MCE_INFO("IMWindowManagerI::SendAds send_ids.size :" << send_ids.size() << "   jids.size:"<<jids.size());
    ////
    /*
    ostringstream os;
    for (int i=0; i<send_ids.size(); ++i) {
      os << send_ids.at(i) << "  ";
    }
    MCE_DEBUG("IMWindowManagerTestI::send_ids:" << os.str());
    os.clear();
    JidSeqMap::iterator iter ;
    for(iter = jids.begin(); iter != jids.end(); ++iter) {
      os << "id:"<<iter->first << "  " << "   jids.size:" << (iter->second).size()<<"   \n";
    }
    MCE_DEBUG("IMWindowManagerTestI:: jids:" << os.str());
    */
    ///
    if (jids.size() > 0) {
      MessageSeq mseq;
      for(JidSeqMap::iterator mit = jids.begin(); mit != jids.end(); ++mit) {
      	for(JidSeq::iterator vit = mit->second.begin(); vit != mit->second.end(); ++vit){
          MCE_DEBUG("IMWindowManagerI:: before GetXmppMessage :  first->" <<mit->first<<"    temp_config:" <<temp_config[mit->first] << "  stringToJid:"
              << jidToString(*vit));
      		MessagePtr msg = GetXmppMessage(temp_config[mit->first], (*vit), true);
      		mseq.push_back(msg);
      		msg = GetXmppMessage(temp_config[mit->first], (*vit), false);
      		mseq.push_back(msg);
      	}
      }

      MCE_INFO("IMWindowManagerI::SendAds mseq.size :" << mseq.size());

      try{
      	TalkDeliverAdapter::instance().deliver(mseq);
      }catch(Ice::Exception& e){
      	MCE_WARN("IMWindowManagerI::SendAds-->TalkDeliverAdapter::deliver-->users.size:" << mseq.size() << "  error:"<<e);
      }
      for(JidSeqMap::iterator mit = jids.begin(); mit != jids.end(); ++mit) {
        IceUtil::Mutex::Lock lock(user_ad_mutex_[GetBufferIndex(mit->first)]);
        user_ad_cache_[GetBufferIndex(mit->first)][mit->first]->last_send_time = current_time;
        user_ad_cache_[GetBufferIndex(mit->first)][mit->first]->last_send_status = 1;
        user_ad_cache_[GetBufferIndex(mit->first)][mit->first]->sended_ads.push_back(temp_urls[mit->first]);

        MCE_DEBUG("IMWindowManagerI::SendAds  userid:" << user_ad_cache_[GetBufferIndex(mit->first)][mit->first]->user_info->user_id
            << "   xntalk_version:" << user_ad_cache_[GetBufferIndex(mit->first)][mit->first]->user_info->xntalk_ver
            << "   first_send_time:" << user_ad_cache_[GetBufferIndex(mit->first)][mit->first]->first_send_time
            << "   last_send_time:" << user_ad_cache_[GetBufferIndex(mit->first)][mit->first]->last_send_time
            << "   last_send_state:" << user_ad_cache_[GetBufferIndex(mit->first)][mit->first]->last_send_status
            << "   sended_ads.size:" << user_ad_cache_[GetBufferIndex(mit->first)][mit->first]->sended_ads.size()
            );
      }
    }
  }
  MCE_INFO("IMWindowManagerI::SendAds----> usetime :IMWindowManagerI:: " << ts.getTime());
  FunStatManager::instance().Stat("IMWindowManagerI::SendAds", ts.getTime(), (ts.getTime() > 90*1000));
}

/*
bool IMWindowManagerI::LoadAvailableUsersFromDB() {
  {
	  IceUtil::RWRecMutex::RLock lock(_rwmutex);
    MCE_INFO("IMWindowManagerI::LoadAvailableUsersFromDB  start  .  ad_list_:" << ad_list_);
    if (!ad_list_)
      return true;
  }
  const int unit_size = 10000;
  int cur_id = -1;
  int retry_count = 0;
  bool ret = true;
  set<Ice::Int> temp_users;
  while (1) {
    Statement sql;
    sql << "SELECT user_id FROM buddy_id where type=1 and id>" << cur_id <<" limit " << unit_size;    
    try {
      mysqlpp::StoreQueryResult res = QueryRunner("im_stat", CDbRServer, "buddy_id").store(sql);       
      if (res.num_rows() == 0)
        break;
      for (size_t i=0; i<res.num_rows(); i++) {
        mysqlpp::Row row = res.at(i);
        temp_users.insert(static_cast<Ice::Int>(row["user_id"]));
      }
    } catch (std::exception &e) {
      MCE_WARN("IMWindowManagSendNotifyMsgserI::LoadAvailableUsersFromDB: "<< e.what());
      retry_count++;
    } catch (...) {
      MCE_WARN("IMWindowManagerI::LoadAvailableUsersFromDB: exception");
      retry_count++;
    }
    if (retry_count == 3) { 
      ret = false;
      break;
    }
    cur_id += unit_size;
  }
  if (ret) {
	  IceUtil::RWRecMutex::WLock lock(users_mutex_);
    users_.swap(temp_users);
  }
  MCE_INFO("IMWindowManagerI::LoadAvailableUsersFromDB  load users_.size:" << users_.size());
  return ret;
}
*/

/*
bool IMWindowManagerI::LoadUserAdInfoFromDB() {
  Statement sql;
  sql << "SELECT user_id, first_send_time,last_send_time,last_send_status,sended_ads FROM window_useradinfo";    
  try {
    mysqlpp::StoreQueryResult res = QueryRunner("im", CDbRServer, "window_useradinfo").store(sql);       
    for (size_t i=0; i<res.num_rows(); i++) {
      mysqlpp::Row row = res.at(i);
      Ice::Long temp_id = static_cast<Ice::Long>(row["id"]);
      row["title"].to_string(ov_info.title);
      row["thumb_url"].to_string(ov_info.thumb_url);
      row["page_url"].to_string(ov_info.page_url);
      row["swf_url"].to_string(ov_info.swf_url);
    }
  } catch (std::exception &e) {
    MCE_WARN("IMWindowManagerI::LoadUserAdInfoFromDB: "<< e.what());
    return false;
  } catch (...) {
    MCE_WARN("IMWindowManagerI::LoadUserAdInfoFromDB: exception");
    return false;
  }
  return true;
}

      */

void SendAdTimer::handle() {
  
 // bool reset = false;
//  MCE_DEBUG("\n----------------------------------------------------------------------> \nSendAdTimer handle.. reset_count:" << reset_count);
/*  
  if (++reset_count == 40) {
    MCE_DEBUG("another day..  count" << reset_count);
    reset = true;
    reset_count =0;
  }
  */
  IMWindowManagerI::instance().SendAds(/*reset*/);
//  reset = false;
}

void SendApplistTimer::handle() {
  TimeStat ts;
  MessageSeq mseq;
  IMWindowManagerI::instance().SwapApplistMsg(mseq);
  try{
  	TalkDeliverAdapter::instance().deliver(mseq);
  }catch(Ice::Exception& e){
  	MCE_WARN("SendAdTimer::handle-->TalkDeliverAdapter::deliver-->mseq.size:" << mseq.size() << "  error:"<<e);
  }
  MCE_INFO("SendApplistTimer::handle    mseq.size:" << mseq.size() << "   time cost:" << ts.getTime());
}

void InstantWindowTimer::handle() {
  TimeStat ts;
  MessageSeq mseq;
  IMWindowManagerI::instance().SwapInstantWindowMsg(mseq);
  try{
  	TalkDeliverAdapter::instance().deliver(mseq);
  }catch(Ice::Exception& e){
  	MCE_WARN("InstantWindowTimer::handle-->TalkDeliverAdapter::deliver-->meseq.size:" << mseq.size() << "  error:"<<e);
  }
  MCE_INFO("InstantWindowTimer::handle    mseq.size:" << mseq.size() << "   time cost:" << ts.getTime());
}

void SendNotifyTimer::handle() {
  TimeStat ts;
  MessageSeq mseq;
  IMWindowManagerI::instance().SwapNotifyMsg(mseq);
  try{
  	TalkDeliverAdapter::instance().deliver(mseq);
  }catch(Ice::Exception& e){
  	MCE_WARN("SendNotifyTimer::handle-->TalkDeliverAdapter::deliver-->mseq.size:" << mseq.size() << "  error:"<<e);
  }
  MCE_INFO("SendNotifyTimer::handle    mseq.size:" << mseq.size() << "   time cost:" << ts.getTime());
}

void GetConfigTimer::handle() {
  TimeStat ts;
  IMWindowManagerI::instance().UpdateConfiguration();
  MCE_INFO("GetConfigTimer::handle    time cost:" << ts.getTime());
}

void HourTimer::handle() {
  TimeStat ts;
  const int current_hour = GetHour();
  MCE_INFO("LoadUsersTimer::handle  current_hour:" << current_hour);
  if (current_hour==0)
    IMWindowManagerI::instance().ClearInstantUsers();

//  if (current_hour==3) 
//    TaskManager::instance().execute(new LoadUsersTask);
  MCE_INFO("HourTimer::handle    time cost:" << ts.getTime() << "    current_hour:" << current_hour);
}
