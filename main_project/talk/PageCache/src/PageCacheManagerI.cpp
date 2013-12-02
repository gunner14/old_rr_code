#include <boost/lexical_cast.hpp>
#include <ctemplate/template.h>
#include <ctemplate/template_dictionary.h>
#include <cstdio>
#include "BuddyByAddTimeReplicaAdapter.h"
#include "PageCacheManagerI.h"
#include "TalkFunStateAdapter.h"
#include "TicketAdapter.h"
#include "LoginCacheAdapter.h"
#include "util/cpp/String.h"
#include "XmppTools.h"
#include "pugixml.hpp"
#include "FeedMemcProxy/client/talk_cache_client.h"
#include "FeedMemcProxy/client/talk_recent_client.h"
#include "BuddyGroupAdapter.h"
#include "UserStateAdapter.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "PageCacheAdapter.h"
using namespace com::xiaonei::talk::fun::state; 
using namespace mop::hi::svc::adapter;
using namespace xce::talk;
using namespace xce::talk::recent;
using namespace xce::adapter::userstate;
using namespace talk::http;
using namespace xce::buddy::adapter;
using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::recent::loader;
using namespace pugi;
using namespace passport;
bool xiaonei = true;

void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  Ice::PropertiesPtr props = service.getCommunicator()->getProperties();
  service.getObjectCache()->registerCategory(AU, "AU", new ActiveUserFactoryI,props, true);
  xiaonei = (bool) props->getPropertyAsIntWithDefault("Service." + ServiceI::instance().getName() + ".IsXiaoNei", 1);
  string configFilePath = props->getPropertyWithDefault("Service.ConfigFilePath", "/opt/XNTalk/etc/pagecache.xml");
  PageCacheManagerI::instance().loadConfig(configFilePath);
  PageCacheManagerI::instance().SetVideoViewPower(1);
  service.getAdapter()->add(&PageCacheManagerI::instance(),service.createIdentity("M", ""));
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
  xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerPageCache", &PageCacheManagerI::instance(), mod, interval, new TalkChannel());
  MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void Buddy::name(const string& name) {
  _name = name;
  for (size_t i = 0; i < _name.size(); ++i) {
    if (_name[i] == '\r' || _name[i] == '\n' || _name[i] == '\t') {
      _name[i] = ' ';
    }
  }
}
string Buddy::tinyUrl() {
  if (_tinyurl == "") {
    return "http://head.xiaonei.com/photos/0/0/men_tiny.gif";
  } else {
    return _tinyurl;
  }
}

void Buddy::doing(const string& doing) {
  _doing = doing;
  for (size_t i = 0; i < _doing.size(); ++i) {
    if (_doing[i] == '\r' || _doing[i] == '\n' || _doing[i] == '\t') {
      _doing[i] = ' ';
    }
  }
}

//---------------------------------------------------------------------------
void ActiveUser::getUserOnlineType(const IntSeq &buddy_ids, map<int, int> &online_bdys, IntSeq &offline_bdys) {
  MCE_INFO("ActiveUser::getUserOnlineType-->buddy_ids.size," << buddy_ids.size());
  map<int, int> online;
  IntSeq offline;
  UserOnlineTypeSeq onlineStats;
  if (!buddy_ids.empty()) {
    try {
      onlineStats = TalkFunStateAdapter::instance().getOnlineUsers( buddy_ids);
    } catch (Ice::Exception &e) {
      MCE_WARN("ActiveUser::getUserOnlineType-->TalkFunStateAdapter::getOnlineUsers()-->uid="
               << _userid << "-" << e);
    }
  }

  MCE_INFO("ActiveUser::getUserOnlineType-->uid " << _userid << " onlineStats.size," << onlineStats.size());

  for (size_t i = 0; i < onlineStats.size(); ++i) {
    online[onlineStats.at(i)->uid] = onlineStats.at(i)->type;
  }

  for (size_t i = 0; i < buddy_ids.size(); ++i) {
    if (online.end() == online.find(buddy_ids[i])) {
      offline.push_back(buddy_ids[i]);
    }
  }

  online_bdys.swap(online);
  offline_bdys.swap(offline);
}

void ActiveUser::seprateBuddyList() {
  MCE_INFO("ActiveUser::seprateBuddyList-->CALL uid " << _userid);
  PageCacheManagerI::instance().UpCallCount(PageCacheCallStat::BUDDY_LIST, 0);
  if (time(NULL) - _buddyListTimestamp < BUDDY_LIST_TIME_STAMP) {
    return;
  }
  PageCacheManagerI::instance().UpCallCount(PageCacheCallStat::BUDDY_LIST, 1);

  MCE_INFO("ActiveUser::seprateBuddyList--> reload uid " << _userid);

  MyUtil::IntSeq buddy_ids;
  try {
    buddy_ids = xce::buddy::adapter::BuddyByAddTimeCacheAdapter::instance().getFriendList( _userid, 0, PageCacheManagerI::instance().MaxGetLimit()+1);
  } catch (Ice::Exception &e) {
    MCE_WARN("ActiveUser::seprateBuddyList-->BuddyByAddTimeCacheAdapter::getFriendList-->uid="
             << _userid << "-" << e);
    return;
  }

  MCE_INFO("ActiveUser::seprateBuddyList-->uid " << _userid << " buddy_ids.size," << buddy_ids.size());

  getUserOnlineType(buddy_ids, _online_buddys_map, _offline_buddys_seq);

  _buddyListTimestamp = time(NULL);
}

const map<int, int> &ActiveUser::getOnlineMap() {
  seprateBuddyList();
  return _online_buddys_map;
}
const IntSeq &ActiveUser::getOfflineSeq() {
  seprateBuddyList();
  return _offline_buddys_seq;
}


const BuddiedGroupsMap &ActiveUser::getBuddyGroupMap() {
  MCE_INFO("ActiveUser::getBuddyGroupMap-->CALL uid " << _userid);
  PageCacheManagerI::instance().UpCallCount(PageCacheCallStat::BUDDY_GROUP, 0);
  if (time(NULL) - _buddyGroupMapTimestamp < BUDDY_GROUP_MAP_TIME_STAMP) {
    return _groupMap;
  }
  PageCacheManagerI::instance().UpCallCount(PageCacheCallStat::BUDDY_GROUP, 1);
  MCE_INFO("ActiveUser::getBuddyGroupMap--> reload uid " << _userid);

  bool iswarn = true;
  try {
    BuddiedGroupsMap gm = BuddyGroupAdapter::instance().getBuddiedGroupsMap(_userid,500);
    _groupMap.swap(gm);
    iswarn = false;
  } catch (Ice::Exception &e) {
    MCE_WARN("ActiveUser::getBuddyGroupMap-->BuddyGroupAdapter::getBuddiedGroupsMap-->uid " << _userid << " err "<<e);
    
  }

  MCE_INFO("ActiveUser::getBuddyGroupMap-->uid " << _userid << " get UserCacheList " 
           << _userid << " gMap.size:" << _groupMap.size());

  if (!iswarn)
    _buddyGroupMapTimestamp = time(NULL);
  return _groupMap;
}

void ActiveUser::getOfflineList(const IntSeq &offline, vector<BuddyPtr> &buddys_res) {
  MCE_INFO("ActiveUser::getOfflineList-->CALL uid " << _userid << " offline.size " << offline.size());

  map<int, int> online;
  getBuddySepList(online, offline, buddys_res);
}

void ActiveUser::getOnlineList(const map<int, int> &online, vector<BuddyPtr> &buddys_res) {

  MCE_INFO("ActiveUser::getOnlineList-->CALL uid " << _userid);

  MyUtil::IntSeq ids;
  for (map<int, int>::const_iterator it = online.begin(); it != online.end(); it++) {
    ids.push_back(it->first);
  }

  getBuddySepList(online, ids, buddys_res);
}

void ActiveUser::getBuddySepList(const map<int, int> &online, const IntSeq &ids, vector<BuddyPtr> &buddys_res) {

  MCE_INFO("ActiveUser::getBuddySepList-->CALL uid " << _userid << " ids.size " << ids.size());

  const map<int, int> &onlineStat = online;
  TalkUserMap usermaps;
  try{ 
    usermaps = TalkCacheClient::instance().GetUserBySeqWithRate(_userid, ids, PageCacheManagerI::instance().RateFriendInfo());
  } catch (Ice::Exception &e) {
    MCE_WARN("ActiveUser::getBuddySepList-->TalkCacheClient::GetUserBySeqWithLoad-->uid " << _userid << " err " << e);
  } catch (std::exception &e) {
    MCE_WARN("ActiveUser::getBuddySepList-->TalkCacheClient::GetUserBySeqWithLoad-->uid " << _userid << " stl exception:" << e.what());
  }

  MCE_INFO("ActiveUser::getBuddySepList-->uid " << _userid << " usermaps.size," << usermaps.size());
  
  const BuddiedGroupsMap &gMap = getBuddyGroupMap();

  MCE_INFO("ActiveUser::getBuddySepList--> get UserCacheList " 
       << _userid << " ids:" << ids.size() << " props:" << usermaps.size() << " gMap.size:" << gMap.size());

  vector<BuddyPtr> res;
  vector<string> urls;
  TalkUserMap::iterator it = usermaps.begin();
  for (; it != usermaps.end(); ++it) {
    BuddyPtr b = new Buddy();
    TalkUserPtr u = it->second;
    b->id(u->id);
    b->name(u->name);
    try {
      b->tinyUrl(buildHeadUrl(u->tinyurl, xiaonei));
    } catch (std::exception& e){
      MCE_WARN("ActiveUser::getBuddySepList--> uid " << _userid << " buildHeadUrl error:" << e.what());
    }
    b->doing(u->statusOriginal);
    map<int, int>::const_iterator st_it = onlineStat.find(b->id());
    if (onlineStat.end() == st_it) {
      b->onlineStatus(0);
    } else {
      b->onlineStatus(st_it->second);
    }

    BuddiedGroupsMap::const_iterator gmapit = gMap.find(u->id);
    if (gmapit != gMap.end()) {
      if (!gmapit->second.empty()) {
        //MCE_INFO("uid:" << u->id << " gname:" << gmapit->second.size());
         b->buddyGroup(gmapit->second);
      }
    } else {
      string gname = "未分组好友";
      MyUtil::StrSeq v;
      v.push_back(gname);
      b->buddyGroup(v);
    }
    res.push_back(b);
  }


  buddys_res.swap(res);

}


bool ActiveUser::getUserInfoList(const vector<string> &uidlist, int &online_count, vector<BuddyPtr> &buddys) {
  MyUtil::IntSeq buddy_ids;
  TimeStatN ts;
  ostringstream os;
  os << "ActiveUser::getUserInfoList";

  int uid;
  for (size_t i = 0; i < uidlist.size(); ++i) {
    int rv = sscanf(uidlist.at(i).c_str(), "%d", &uid);
    if (rv == 1 && uid > 0)
      buddy_ids.push_back(uid);
  }

  if (buddy_ids.empty())
    return false;

  map<int, int> online;
  IntSeq offline;
  getUserOnlineType(buddy_ids, online, offline);
  MCE_INFO("ActiveUser::getUserInfoList-->uid " << _userid << " online," << online.size() << ",offline," << offline.size());

  buddys.clear();
  getOnlineList(online, buddys);
  MCE_INFO("ActiveUser::getUserInfoList-->uid " << _userid << " online res," << buddys.size());
  
  online_count = (int)buddys.size();

  vector<BuddyPtr> offres;
  getOfflineList(offline, offres);
  MCE_INFO("ActiveUser::getUserInfoList-->uid " << _userid << " offline res," << offres.size());

  buddys.insert(buddys.end(), offres.begin(), offres.end());
  FunStatManager::instance().Stat(os.str(), ts.getTime(), (ts.getTime() >= 250.0));
  return true;

}



bool ActiveUser::getAllFullBuddyList(int begin, int limit, int &friend_count, int &online_count, vector<BuddyPtr> &buddylst) {
  MCE_INFO("ActiveUser::getAllFullBuddyList--> CALL uid " << _userid << " begin " << begin << " limit " << limit);

  TimeStatN ts;
  ostringstream os;
  os << "ActiveUser::getAllFullBuddyList";
  const map<int, int> &online_buddymap = getOnlineMap();
  const IntSeq &offline_buddyList = getOfflineSeq();

  int onsize = online_buddymap.size();
  int offsize = offline_buddyList.size();

  friend_count = onsize + offsize;

  if (limit <= 0 || begin < 0 || begin > friend_count)
    return false;

  // set get one time max num
  if (limit > PageCacheManagerI::instance().OneGetLimit())
    limit = PageCacheManagerI::instance().OneGetLimit();

  int end = begin + limit;
  if (end > PageCacheManagerI::instance().MaxGetLimit())
    end = PageCacheManagerI::instance().MaxGetLimit();
  
  if (end > onsize + offsize)
    end = onsize + offsize;

  
  map<int, int> selonline;
  IntSeq seloffline;
  
  if (onsize >= end) {
    int cn = 0;
    for (map<int, int>::const_iterator it = online_buddymap.begin();
         it != online_buddymap.end(); it++, cn++) {
      if (cn < begin) {
        continue;
      } else if (cn >=begin && cn < end) {
        selonline[it->first] = it->second;
      } else {
        break;
      }
    }
    online_count = (int)selonline.size();
  } else if (onsize > begin && onsize < end) {
    int cn = 0;
    for (map<int, int>::const_iterator it = online_buddymap.begin();
         it != online_buddymap.end(); it++, cn++) {
      if (cn < begin) {
        continue;
      } else if (cn >=begin && cn < end) {
        selonline[it->first] = it->second;
      } else {
        break;
      }
    }
    online_count = (int)selonline.size();
    seloffline.assign(offline_buddyList.begin(), offline_buddyList.begin()+(end-onsize));
  } else if (onsize <= begin && onsize+offsize >= end) {
    seloffline.assign(offline_buddyList.begin()+(begin-onsize), offline_buddyList.begin()+(end-onsize));
    online_count = 0;
  } else {
    MCE_ERROR("ActiveUser::getAllFullBuddyList--> uid " << _userid << " Oh My Gog! The 2012 is coming");
    return false;
  }

  vector<BuddyPtr> onbuddylst;
  if (!selonline.empty()) {
    getOnlineList(selonline, onbuddylst);
  }

  vector<BuddyPtr> offbuddylst;
  if (!seloffline.empty()) {
    getOfflineList(seloffline, offbuddylst);
  }

  onbuddylst.insert(onbuddylst.end(), offbuddylst.begin(), offbuddylst.end());

  buddylst.swap(onbuddylst);
  FunStatManager::instance().Stat(os.str(), ts.getTime(), (ts.getTime() >= 250.0));
  MCE_INFO("ActiveUser::getAllFullBuddyList--> uid " << _userid
           << " limit " << limit << " begin " << begin << " end " << end
           << " buddylist.size " << buddylst.size()
           << " friend_count " << friend_count << " online_count " << online_count);
  return true;
}

void ActiveUser::getFullBuddyList(vector<BuddyPtr> &buddylst){
  MCE_INFO("ActiveUser::getFullBuddyList--> CALL uid " << _userid);
  TimeStatN ts;
  ostringstream os;
  os << "ActiveUser::getFullBuddyList";

  getOnlineList(getOnlineMap(), buddylst);
  FunStatManager::instance().Stat(os.str(), ts.getTime(), (ts.getTime() >= 250.0));
  MCE_INFO("ActiveUser::getFullBuddyList-->uid " << _userid << " online " << getOnlineMap().size() << " buddylst " << buddylst.size());

}

void ActiveUser::getRecentList(vector<string> &recents) {
  recents.clear();
  try { 
    TalkRecentUserSeq rseq = TalkRecentClient::instance().GetRecentUserByIdWithLoad(_userid);
    for (TalkRecentUserSeq::const_iterator it = rseq.begin(); it != rseq.end(); ++it) {
      recents.push_back((*it)->info);
    }
  } catch (Ice::Exception& e){
    MCE_WARN("ActiveUser::getRecentList-->TalkRecentClient.GetRecentUserByIdWithLoad-->uid " << _userid << " err " << e);
  }
}


void ActiveUser::getFullAndRecentBuddyList(int option, vector<BuddyPtr> &buddylst, vector<string> &recents, int &friend_count) {
  TimeStatN ts;
  ostringstream os;
  os << "ActiveUser::getFullAndRecentBuddyList";
  if (1 == option || 2 == option) {
    const map<int, int> &online_buddymap = getOnlineMap();
    const IntSeq &offline_buddyList = getOfflineSeq();

    int onsize = online_buddymap.size();
    int offsize = offline_buddyList.size();

    friend_count = onsize + offsize;
    getFullBuddyList(buddylst);
  }

  if (0 == option || 2 == option) {
    getRecentList(recents);
  }

  MCE_INFO("ActiveUser::getFullAndRecentBuddyList--> uid " << _userid << " option " << option << " buddylist.size " << buddylst.size()
           << " recents.size " << recents.size() << " friendcount " << friend_count);
  FunStatManager::instance().Stat(os.str(), ts.getTime(), (ts.getTime() >= 250.0));
}

int ActiveUser::getBuddyCount() {
  TimeStatN ts;
  ostringstream os;
  os << "ActiveUser::getBuddyCount";
  FunStatManager::instance().Stat(os.str(), ts.getTime(), (ts.getTime() >= 250.0)); 
  return getOnlineMap().size();
}

bool ActiveUser::checkTicket(const string& ticket, bool isWap) {
  if ((_ticket == ticket) && (_ticket != "")) {
    return true;
  }
  int id = 0;
  try {
    if (isWap) {
      try{
        id = WapLoginCacheAdapter::instance().ticket2Id(ticket);
      }
      catch(Ice::Exception& e){
        MCE_WARN("ActiveUser::checkTicket-->WapLoginCacheAdapter::ticket2Id-->uid " << _userid << " err" << e);
      }
    } else {
      id = TicketAdapter::instance().verifyTicket(ticket, "");
    }
    MCE_DEBUG("get passport -> id: " << id << " " << ticket << " " << isWap);
    if (id == _userid) {
      _ticket = ticket;
      return true;
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("ActiveUser::checkTicket-->TicketAdapter::verifyTicket-->" << _userid << " " << e);
  }
  _ticket = "";
  MCE_DEBUG("get passport -> err: userid:" << _userid << " uid:" << id << " " << ticket << " " << isWap);
  return false;
}

int ActiveUser::userId() {
  return _userid;
}

void ActiveUser::load() {
  IntSeq ids;
  ids.push_back(_userid);
  TalkUserMap usermaps;
  try{ 
    usermaps = TalkCacheClient::instance().GetUserBySeqWithLoad(_userid, ids);
  }catch(Ice::Exception& e){
    MCE_WARN("ActiveUser::load-->TalkCacheClient::GetUserBySeqWithLoad-->uid " << _userid << " err " << e);
  }

  TalkUserMap::iterator it = usermaps.find(_userid);
  if (it == usermaps.end()) {
    return;
  }
  TalkUserPtr u = it->second;
  _name = u->name;
  StrUtil::string_replace(_name, "\n", " ");
  StrUtil::string_replace(_name, "\r", " ");
  StrUtil::string_replace(_name, "\t", " ");

  _tiny_url = buildHeadUrl(u->tinyurl, xiaonei);
}

string ActiveUser::name() {
  if (!_name.empty()) {
    return _name;
  }
  load();
  if (_name.empty())
    return boost::lexical_cast<string>(_userid);
  return _name;
}


string ActiveUser::tiny_url() {
  if (!_tiny_url.empty()) {
    return _tiny_url;
  }
  load();
  if (_tiny_url.empty())
    return "http://head.xiaonei.com/photos/0/0/men_tiny.gif";
  return _tiny_url;
}
//-------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
Ice::ObjectPtr ActiveUserFactoryI::create(Ice::Int id) {
  ActiveUserPtr u = new ActiveUser(id);
  return u;
}
//------------------------------------------------
void PageCacheManagerI::loadConfig(const string& path) {
  MCE_INFO("PageCacheManagerI::loadConfig-->path:" << path);
  xml_document doc;
  if (doc.load_file(path.c_str())) {

    xml_node configNode = doc.child("config");
    for (xml_node n = configNode.child("page"); n; n = n.next_sibling("page")) {

      string url = n.attribute("url").value();
      if (url != "") {
        Config c;
        c.template_path = n.attribute("file").value();
        c.timeout = boost::lexical_cast<int>(n.attribute("timeout").value());
        c.type = n.attribute("type").value();
        c.content_type = n.attribute("content_type").value();
        _config[url] = c;
      }
    }
  }
}
/* brief : 根据不同的url 调用不同的接口获取数据
 *
 */
ContentPtr PageCacheManagerI::GetContent(const string& path,
    const MyUtil::Str2StrMap& cookies, const MyUtil::Str2StrMap& parameter,
    const Ice::Current&) {

  TimeStatN ts;
  ostringstream os;
  os << "PageCacheManagerI::GetContent";
  //取uid只为打log
  string strHostId;
  MyUtil::Str2StrMap::const_iterator tmpIt;
  if ((tmpIt = cookies.find("id")) != cookies.end()) {
    strHostId = tmpIt->second;
  } else if ((tmpIt = parameter.find("uid")) != parameter.end()) {
    strHostId = tmpIt->second;
  }
  int uid = 0;
  if (!strHostId.empty()) {
    try {
      uid = boost::lexical_cast<int>(strHostId);
    } catch (...) {
      MCE_WARN("PageCacheManagerI::GetContent --> cast err, " << strHostId);
    }
  }
  
  string pathn = path;
  if("/wtalk/ime.htm" == pathn){
    pathn = "/wtalk/imf.htm";
    MCE_INFO("PageCacheManagerI::GetContent --> userid:" << uid << " pathn:" << pathn);
  }

  map<string, Config>::iterator it = _config.find(pathn);
  if (it == _config.end()) {
    ContentPtr c = new Content();
    c->statusCode = 404;
    c->timeout = 0;
    c->data = "url not find";
    c->isBin = false;
    MCE_WARN("PageCacheManagerI::GetContent --> url not configed, " << uid << " " << pathn);
    return c;
  }
  MCE_INFO("PageCacheManagerI::GetContent --> pathn = " << pathn << " template_path = " << it->second.template_path);
  ctemplate::Template* tpl = ctemplate::Template::GetTemplate(it->second.template_path, ctemplate::DO_NOT_STRIP);
  if (!tpl) {
    ContentPtr c = new Content();
    c->statusCode = 404;
    c->timeout = 0;
    c->data = "url not find";
    c->isBin = false;
    MCE_WARN("PageCacheManagerI::GetContent --> template is absent, " << uid << " " << pathn);
    return c;
  }
  //tpl->ReloadIfChanged();
  ctemplate::TemplateDictionary dict("page_template");

  ContentPtr c = new Content();
  if (it->second.type == "bin") {
    c->isBin = true;
  } else {
    c->isBin = false;
  }
  if (it->second.type == "template") {
    MCE_DEBUG("PageCacheManagerI::GetContent --> template_path:" <<it->second.template_path << " userid:" << uid);
    TemplateDataCollectorPtr tc = getTemplateCollector(pathn);
    if (tc) {
      try{
        tc->fillData(path, const_cast<Str2StrMap&> (cookies), const_cast<Str2StrMap&> (parameter), dict);
      }catch(std::exception& e){
        MCE_WARN("PageCacheManagerI::GetContent --> userid:" << uid << " pathn: " 
                 << pathn << " tc->fillData error:" << e.what());
      }
    } else {
      c->statusCode = 404;
      c->timeout = 0;
      c->data = "url not find";
      c->isBin = false;
      return c;
    }
  }
  bool error_free = tpl->Expand(&c->data, &dict);

  if (error_free) {
    c->timeout = it->second.timeout;
    c->statusCode = 200;
    c->contentType = it->second.content_type;
  } else {
    c->statusCode = 404;
    c->timeout = 0;
    c->data = "build content err";
  }
  FunStatManager::instance().Stat(os.str(), ts.getTime(), (ts.getTime() >= 250.0));
  MCE_INFO("PageCacheManagerI::GetContent --> " << uid << " url:" << pathn << " page size:" << c->data.length());
  return c;

}

void PageCacheManagerI::ReloadStaticPages(const Ice::Current&) {
  map<string, Config>::iterator it = _config.begin();
  for (; it != _config.end(); ++it) {
    if (it->second.type != "template") {
      ctemplate::Template* tpl = ctemplate::Template::GetTemplate(
          it->second.template_path, ctemplate::DO_NOT_STRIP);
      if (tpl) {
        tpl->ReloadIfChanged();
      }
    }
  }
}


BuddyPtr PageCacheManagerI::MakeBuddyById(int userid, int mucstatus){
  int ot = mucstatus;
  TalkUserPtr uPtr;
  try{ 
    uPtr = TalkCacheClient::instance().GetUserByIdWithLoad(userid);
  }catch(Ice::Exception& e){
    MCE_WARN("PageCacheManagerI::MakeBuddyById-->TalkCacheClient::GetUserByIdWithLoad-->" << e);
  }
  if(!uPtr){
    MCE_INFO("PageCacheManagerI::MakeBuddyById--> get UserCacheList call TalkCacheClient return NULL");	
    return NULL;
  }
  BuddyPtr b = new Buddy();
  b->id(uPtr->id);
  b->name(uPtr->name);
  b->tinyUrl(buildHeadUrl(uPtr->tinyurl, xiaonei));
  b->doing(uPtr->statusOriginal);
  b->onlineStatus(ot);
  return b;
}


TemplateDataCollectorPtr PageCacheManagerI::getTemplateCollector(
    const string& path) {
  IceUtil::Mutex::Lock lock(_mutex);

  map<string, TemplateDataCollectorPtr>::iterator it = _collectors.find(path);
  if (it != _collectors.end()) {
    return it->second;
  }
  TemplateDataCollectorPtr c;
  if (path == "/getonlinefriends.do" || path == "/wap_getonlinefriends.do") {
    c = new OnlineBuddyListCollector();
    _collectors[path] = c;
  } else if (path == "/getonlinecount.do") {
    c = new OnlineBuddyCountCollector();
    _collectors[path] = c;
  } else if (path == "/toolbar/counts.xml") {
    c = new OnlineToolbarCountsCollector();
    _collectors[path] = c;
  }else if(path == "/getuserinfo.do"){
    c = new UserInfoCollector();
    _collectors[path] = c;
  }else if(path == "/getuserinfo2.do"){
    c = new UserInfoCollector2();
    _collectors[path] = c;
  }else if(path == "/getfullfriends.do"){
    c = new FullBuddyListCollector();
    _collectors[path] = c;
  } else if (path == "/getallfullfriends.do") {
    c = new AllFullBuddyListCollector();
    _collectors[path] = c;
  } else if (path == "/getfullrecentfriends.do"){
    c = new FullRecentBuddyListCollector();
    _collectors[path] = c;
  }
  return c;
}

int PageCacheManagerI::GetVideoViewPower(const Ice::Current& current){
  IceUtil::RWRecMutex::RLock lock(_video_power_mutex);
  return _video_view_power;
}
void PageCacheManagerI::SetVideoViewPower(int value, const Ice::Current& current){
  IceUtil::RWRecMutex::WLock lock(_video_power_mutex);
  _video_view_power = value;
}

bool PageCacheManagerI::VerifyIsGuide(int uid) {
  bool isguidedone = true;
  if (uid != 0) {
    try {
      UserStateInfoPtr ust = UserStateAdapter::instance().getUserState(uid);
      if (ust != 0) {
        isguidedone = ust->isGuideDone();
      }
    } catch (Ice::Exception& e) {
      MCE_WARN("PageCacheManagerI::VerifyIsGuide-->UserStateAdapter.getUserState-->uid=" << uid << " err " << e);
    }
  }


  return !isguidedone;

}

