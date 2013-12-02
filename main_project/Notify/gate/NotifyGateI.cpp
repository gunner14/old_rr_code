#include "NotifyGateI.h"
#include "APNSNotifyAdapter.h"
#include "NotifyDispatcherAdapter.h"
#include "NotifyStormAdapter.h"
#include "Notify/util/schema_manager.h"
#include "Notify/util/rmessage_util.h"
#include "Notify/util/MergeConfig.h"
#include "Notify/util/PrintUtil.h"
#include "WTransitAdapter.h"
#include "NotifyDispatcherNAdapter.h"
#include "NotifyIndexAdapter.h"
#include "NotifyIndexNAdapter.h"
#include "NotifyContentAdapter.h"
#include "Notify/util/time_stat.h"
#include "QueryRunner.h"
#include "XmppTools.h"
#include "OceCxxAdapter/src/NotifyContentLoaderAdapter.h"
#include "client/linux/handler/exception_handler.h"
#include <cstring>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <openssl/md5.h>

using namespace std;
using namespace xce::notify;
using namespace Ice;
using namespace boost;

using namespace xce::feed;
using namespace com::xiaonei::talk::util;
using talk::adapter::WTransitAdapter;

static const char SOURCE_SEPERATOR = '-';
static const char * kClubDispatchKey = "renrenclub$^%&^%&$%^&";

static void RemoveIndex(int uid, vector<Ice::Long> fail_ids) {
  try {
    NotifyIndexAdapter::instance().removeByIds(uid, fail_ids);
  } catch (Ice::Exception& e) {
    MCE_WARN("RemoveNotifies " << e << " uid=" << uid << " fail_ids size:" << fail_ids.size());
    return;
  }
  MCE_INFO("RemoveIndex. uid:" << uid << " fail_ids size:" << fail_ids.size());
}
static void RemoveIndex(vector<int> uids, vector<Ice::Long> fail_ids) {
  for (unsigned i = 0; i != uids.size(); ++i) {
    RemoveIndex(uids.at(i), fail_ids);
  }
}

static void RemoveIndexRmessage(int uid, map<Ice::Long, int> fail_ids){
  vector<Ice::Long> nids;
  map<int, vector<Ice::Long> > type_nids;
  for(map<Ice::Long, int>::iterator it = fail_ids.begin(); it != fail_ids.end(); ++it ){
    nids.push_back(it->first);
    type_nids[it->second].push_back(it->first);
  }
  try {
    NotifyIndexAdapter::instance().removeByIds(uid, nids);
    MCE_INFO("RemoveIndexRmessage --> removeByIds nids:" 
        << nids.size() << " uid:" << uid << " fail_ids:" 
        << fail_ids.size());
  } catch (Ice::Exception& e) {
    MCE_WARN("RemoveIndexRmessage --> RemoveNotifies" << " uid:" << uid << " fail_ids:" 
        << fail_ids.size() << " exception:" << e);
    return;
  }

  for(map<int, vector<Ice::Long> >::iterator it = type_nids.begin(); it != type_nids.end(); ++it){
    try {
      NotifyIndexNAdapter::instance().RemoveRMessageByIds(uid, it->second, it->first, 0);
      MCE_INFO("RemoveIndexRmessag --> RemoveRMessageByIds nids:" 
          << it->second.size() << " uid:" << uid << " type:" << it->first);
    } catch (Ice::Exception& e) {
      MCE_WARN("rmessage RemoveNotifies " << e << " uid:" << uid 
          << " fail_ids:" << fail_ids.size());
      return;
    }
  }
}

static string GetIndexTableName(int64_t uid) {
  ostringstream os;
  os << "index_" << uid % 100;
  return os.str();
}

static string GetNotifyDbInstance() {
  return "notify";
}

class NotifyRequestFactory: public RequestFactory {
 public:
  virtual RequestPtr Create(FCGX_Request * r) {
    char * path = FCGX_GetParam("SCRIPT_NAME", r->envp);
    if (path) {
      if (strcmp(path, "/get.notify") == 0 /*|| strcmp(path, "/ebpt/show") == 0*/) {
        return new GetRequest(r);
      }
      if (strcmp(path, "/remove.notify") == 0) {
        return new RemoveRequest(r);
      }
      if (strcmp(path, "/dispatch.notify") == 0) {
        return new DispatchRequest(r);
      }
      if (strcmp(path, "/unreadcount.notify") == 0) {
        return new UnreadCountRequest(r);
      }
      if (strcmp(path, "/quitchat.notify") == 0) {
        return new QuitChatRequest(r);
      }
      //======================RMESSAGE=============================
      if (strcmp(path, "/rmessage/get") == 0) {
        return new GetRMessageRequest(r);
      }
      if (strcmp(path, "/rmessage/process") == 0) {
        return new ProcessRemindMsgRequest(r);
      }
      if (strcmp(path, "/rmessage/remove") == 0) {
        return new RemoveRMessageRequest(r);
      }
      if (strcmp(path, "/rmessage/getundealcount") == 0) {
        return new GetUndealCountRequest(r);
      }
      if (strcmp(path, "/rmessage/getunreadcount") == 0) {
        return new GetUnreadCountRequest(r);
      }
      //======================RMESSAGE=============================
    }
    return NULL;
  }
};
bool notifyGateCrashCallback(const char *dump_path, const char *minidump_id, 
                             void *context, bool succeeded) {
  printf("NotifyGate crash %s %s\n", dump_path, minidump_id);
  return true;
}


void MyUtil::initialize() {
  static google_breakpad::ExceptionHandler eh(".", 0, notifyGateCrashCallback, 0, true);
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&NotifyGateI::instance(), service.createIdentity(
      "M", ""));

  Ice::PropertiesPtr props = service.getCommunicator()->getProperties();
  string fcgi_socket = props->getPropertyWithDefault("Service."
      + service.getName() + ".FcgiSocket", "0.0.0.0:9001");
  MCE_DEBUG("Fcgi listens on : " << fcgi_socket);

#if 0
  if (FCGX_Init() != 0) {
    MCE_WARN("FCGX_Init err");
    return;
  }

  int socket = FCGX_OpenSocket(fcgi_socket.c_str(), 100);
  if(socket < 0) {
    MCE_WARN("FCGX_OpenSocket err");
    return;
  }
  for(int i = 0; i< 128; ++i)
  {
    FcgiTask * task = new FcgiTask(new NotifyRequestFactory());
    if(task->init(socket)) {
      TaskManager::instance().execute(task);
    } else {
      delete task;
      MCE_WARN("FCGX_InitRequest err " << i);
    }
  }
#else
  // fcgi_server memory managed as RequestFactory ...
  FcgiServer * fcgi_server = new FcgiServer(fcgi_socket, 128);
  fcgi_server->RegisterRequestFactory(new NotifyRequestFactory());
  fcgi_server->Start();
#endif
  MergeConfig::instance().Init();
}

NotifyGateI::NotifyGateI() {
  //ServiceI& service = ServiceI::instance();
  //Ice::PropertiesPtr props = service.getCommunicator()->getProperties();
  //_fcgi_socket = props->getPropertyWithDefault("Service." + service.getName() + ".FcgiSocket", "0.0.0.0:9001");
  //MCE_DEBUG("Fcgi listens on : " << _fcgi_socket);
}

bool NotifyGateI::reloadSchema(const Ice::Current&) {
  MCE_INFO("NotifyGateI::reloadSchema()");
  return SchemaManager::instance().reload();
}

void NotifyGateI::QuitChat(::Ice::Int uid, const GroupIndexSeq& groups,
    const ::Ice::Current&) {
	TimeStat ts;
  MCE_INFO("NotifyGateI::QuitChat uid:" << uid << " groups size:" << groups.size());
  try {
    NotifyDispatcherNAdapter::instance().QuitChat(uid, groups);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyDispatcherNAdapter QuitChat exception:" << e << " uid=" << uid);
  }
	FunStatManager::instance().Stat("NotifyGateI::QuitChat", ts.getTime(), false);
}


void ToWebpager(const xce::notify::NotifyBodyPtr& body,
    const vector<int>& webpagertargets, vector<Ice::Long>& fail_ids){
  //屏蔽好友申请 好友推荐 圈人请求的 老的数据的对webpager 的发送   红泡泡用的是v6 的View1 
  int type = RMessageUtil::instance().Util_GetRMessageType(body->type);
  if(23 == type || 62 == type || 36 == type || 141 == type || 66 == type || 67 == type || 70 == type){
    return;
  }
  string txt = SchemaManager::instance().toReplyTxt(body, kPagerViewId, false,
      fail_ids);
  if (!txt.empty()) {
    try {
      WTransitAdapter::instance().deliverNotifyWebpager(webpagertargets, txt);
      MCE_INFO("ToWebpager nid[" << body->id 
          << "]targets[" << PrintHelper::Print(webpagertargets) 
          << "]fail_ids[" << PrintHelper::Print(fail_ids) << "]");
    } catch (Ice::Exception& e) {
      MCE_WARN("WTransitAdapter deliverNotifyWebpager nid[" << body->id 
          << "]targets[" << PrintHelper::Print(webpagertargets) 
          << "]error[" << e << "]");
    }
  }
}

void ToPhone(const xce::notify::NotifyBodyPtr& body,
    const vector<int>& desktopiphonetargets, vector<Ice::Long>& fail_ids){
  string txt = SchemaManager::instance().toReplyTxt(body, kPhoneViewId, false, fail_ids);
  if (!txt.empty()) {
    try {
      WTransitAdapter::instance().deliverNotifyPhone(desktopiphonetargets, txt);
    } catch (Ice::Exception& e) {
      MCE_WARN("WTransitAdapter deliverNotifyPhone nid[" 
          << body->id << "]error[" << e << "]");
    }
  } else {
    //MCE_WARN("NotifyGateI ToPhone toReplyTxt empty" );
  }
}

void ToDesktop(const xce::notify::NotifyBodyPtr& body,
    const vector<int>& desktopiphonetargets, vector<Ice::Long>& fail_ids){
  string txt = SchemaManager::instance().toReplyTxt(body, kClientViewId, false, fail_ids);
  if (!txt.empty()) {
    try {
      WTransitAdapter::instance().deliverNotifyClient(desktopiphonetargets, txt);
      MCE_INFO("ToDesktop nid:" << body->id 
          << " targets:" << PrintHelper::Print(desktopiphonetargets) 
          << " fail_ids:" << PrintHelper::Print(fail_ids));
    } catch (Ice::Exception& e) {
      MCE_WARN("ToDesktop nid:" << body->id 
          << " targets:" << PrintHelper::Print(desktopiphonetargets) 
          << " e:" << e);
    }
  }
}

void ToNotifyStorm(const xce::notify::NotifyBodyPtr& body, 
    const vector<int>& desktopiphonetargets, vector<Ice::Long>& fail_ids){
  string txt = SchemaManager::instance().toReplyTxt(body, kIphoneViewId, false, fail_ids);
  if (!txt.empty()) {
    string ids_str = NotifyGateI::Print(desktopiphonetargets);
    map<string, string> m;
    m["users"] = ids_str;
    m["msg"] = txt;
    m["type"] = lexical_cast<string>(RMessageUtil::instance().Util_GetRMessageType(body->type));
    try {
      NotifyStormAdapter::instance().notify("APNSTEST", m);
      MCE_INFO("ToNotifyStorm nid:" << body->id 
          << " targets:" << PrintHelper::Print(desktopiphonetargets) 
          << " fail_ids:" << PrintHelper::Print(fail_ids));
    } catch (Ice::Exception& e) {
      MCE_WARN("ToNotifyStorm nid:" << body->id 
          << " targets:" << PrintHelper::Print(desktopiphonetargets) 
          << " fail_ids:" << PrintHelper::Print(fail_ids) << " e:" << e);
    }
  }
}

void ToPlatform(const xce::notify::NotifyBodyPtr& body, 
    const vector<int>& desktopiphonetargets, vector<Ice::Long> & fail_ids){
  string txt = SchemaManager::instance().toReplyTxt(body, kWebViewId, false, fail_ids);
  if (!txt.empty()) {
    string ids_str = NotifyGateI::Print(desktopiphonetargets);
    map<string, string> m;
    m["users"] = ids_str;
    m["type"] = lexical_cast<string> (RMessageUtil::instance().Util_GetRMessageType(body->type));
    try {
      NotifyStormAdapter::instance().notify("PLATFORM_REPLY", m);
      MCE_INFO("ToPlatform nid:" << body->id 
        << " targets.size:" << desktopiphonetargets.size() 
        << " to_ids:" << ids_str);
      MCE_DEBUG("ToPlatform toReplyTxt [" << txt << "]");
    } catch (Ice::Exception& e) {
      MCE_WARN("ToPlatform nid:" << body->id 
          << " targets:" << desktopiphonetargets.size() 
          << " to_ids:" << ids_str << " e:" << e);
    }
  }
}

void ToNotifyStormGeneral(const xce::notify::NotifyBodyPtr& body,
                          const vector<int> & desktopiphonetargets, vector<long> & fail_ids){
  const int kGeneralViewId = 17;
  string txt = SchemaManager::instance().toReplyTxt(body, kGeneralViewId, false, fail_ids);
  if (!txt.empty()) {
    string ids_str = NotifyGateI::Print(desktopiphonetargets);
    map<string, string> m;
    m["users"] = ids_str;
    m["msg"] = txt;
    m["type"] = lexical_cast<string> (body->type);
    try {
      NotifyStormAdapter::instance().notify("GENERAL", m);
      MCE_INFO("ToNotifyStormGeneral nid:" << body->id 
          << " targets:" << PrintHelper::Print(desktopiphonetargets) 
          << " fail_ids:" << PrintHelper::Print(fail_ids));
    } catch (Ice::Exception& e) {
      MCE_WARN("ToNotifyStormGeneral nid:" << body->id 
          << " targets:" << PrintHelper::Print(desktopiphonetargets) 
          << " fail_ids:" << PrintHelper::Print(fail_ids) 
          << " e:" << e);
    }
  }
}


bool IsPageNotify(int type,int target){
  static const int kMinPageUid = 600000000;
  static const int kMaxPageUid = 700000000;
  return (
      (type >= 26 && type <= 30) &&
      (target >= kMinPageUid && target <=kMaxPageUid));
}

//void NotifyGateI::deliverWebpager(const NotifyContentPtr& content,
//    const MyUtil::IntSeq& targets, const Ice::Current&) {
//  vector<Ice::Long> fail_ids;
//  if (targets.empty())
//    return;
//  NotifyBodyPtr body = new NotifyBody(content);
//  if (!body)
//    return;
//
//  MCE_INFO("NotifyGateI::deliverWebpager : size: " << targets.size() << " first: " << targets[0] << " nid: " << content->id << " schema: " << content->type );
//
//  string txt = SchemaManager::instance().toReplyTxt(body, kPagerViewId, false,
//      fail_ids);
//  if (!txt.empty()) {
//    try {
//      MCE_DEBUG("deliverWebpager to webpager : " << targets.size() << "-" << targets[0] << "-" << txt);
//      talk::adapter::WTransitAdapter::instance().deliverNotifyWebpager(targets,
//          txt);
//    } catch (Ice::Exception& e) {
//      MCE_WARN("deliverWebpager to webpager " << targets[0] << "  error : " << e);
//    }
//  }
//  if (!fail_ids.empty()) {
//    RemoveIndex(targets, fail_ids);
//  }
//}

string PrintContent(const NotifyContentPtr& cnt){
  ostringstream os;
  os << "nid:" << cnt->id <<" from:" << cnt->from << " type:" 
     << RMessageUtil::instance().Util_GetRMessageType(cnt->type) << " bigtype:"
     << RMessageUtil::instance().Util_GetRMessageBigtype(cnt->type);
  return os.str();
}
void NotifyGateI::deliverWebpager(const NotifyContentPtr& content,
    const MyUtil::IntSeq& targets, const Ice::Current&) {
	TimeStat ts;
  MCE_DEBUG("NotifyGateI::deliverWebpager--> content:"  << PrintContent(content));
  NotifyBodyPtr body = new NotifyBody(content);
  if (!body) {
    MCE_WARN("NotifyGateI::deliverWebpager: body null");
    return;
  }
  vector<int> targets_without_page;
  BOOST_FOREACH(int i,targets){
    if(!IsPageNotify(RMessageUtil::instance().Util_GetRMessageType(content->type),i)){
      targets_without_page.push_back(i);
    }
  }
  vector<Ice::Long> fail_ids;
  ToWebpager(body, targets_without_page, fail_ids);
  if (!fail_ids.empty()) {
    RemoveIndex(targets, fail_ids);
  }
  MCE_INFO("NotifyGateI::deliverWebpager." << PrintContent(content)
      << " targets:" << PrintHelper::Print(targets)
      << " targets_without_page:" << PrintHelper::Print(targets_without_page));
	FunStatManager::instance().Stat("NotifyGateI::deliverWebpager", ts.getTime(), false);
}

void NotifyGateI::instantNotify(const NotifyContentPtr& content,
    const MyUtil::IntSeq& targets, const Ice::Current&) {
	TimeStat ts;
  MCE_DEBUG("NotifyGateI::istantNotify content["  << PrintContent(content) 
          << "]props_size[" << content->props.size() << "]props[" 
          << content->props << "]");
  NotifyBodyPtr body = new NotifyBody(content);
  if (!body) {
    MCE_WARN("NotifyGateI::instantNotify body null, content[" 
        << PrintContent(content) << "]");
    return;
  }
  vector<int> targets_without_page;
  BOOST_FOREACH(int i,targets){
    if(!IsPageNotify(RMessageUtil::instance().Util_GetRMessageType(content->type),i)){
      targets_without_page.push_back(i);
    }
  }
  vector<Ice::Long> fail_ids;
  vector<Ice::Long> tmp;
  ToWebpager(body, targets_without_page, fail_ids);
  ToDesktop(body, targets_without_page, fail_ids);
  ToPhone(body, targets_without_page, fail_ids);
  ToNotifyStorm(body, targets_without_page, fail_ids);
  //ToPlatform(body, targets_without_page, fail_ids);
  ToPlatform(body, targets_without_page, tmp);
  ToNotifyStormGeneral(body, targets_without_page, fail_ids);
  if (!fail_ids.empty()) {
    RemoveIndex(targets, fail_ids);
    MCE_INFO("NotifyGateI::instantNotify remove index content:" 
        << PrintContent(content) << " nid:" << PrintHelper::Print(fail_ids)
        << " targets:" << PrintHelper::Print(targets));
  }
  MCE_INFO("NotifyGateI::instantNotify " << PrintContent(content)
      << " targets:" << PrintHelper::Print(targets) << " targets_without_page:" 
      << PrintHelper::Print(targets_without_page));
	FunStatManager::instance().Stat("NotifyGateI::instantNotify", ts.getTime(), false);
}

void NotifyGateI::BodySeq2StrSeq(const NotifyBodySeq & n, Ice::Int view,
    StrSeq & s, vector<Ice::Long> &fail_ids, bool is_kaixin) {
  for (size_t i = 0; i < n.size(); ++i) {
    // TODO : kaixin or renren?
    string txt = SchemaManager::instance().toReplyTxt(n.at(i), view, is_kaixin,
        fail_ids);
    if (!txt.empty()) {
      s.push_back(txt);
    }
  }
}

void NotifyGateI::BodySeq2StrSeqMerge(const NotifyBodySeq & n, Ice::Int view,
    StrSeq & s, vector<Ice::Long> &fail_ids, bool is_kaixin) {
  if (n.empty()) {
    return;
  }
  typedef map<pair<Int, Long> , NotifyBodySeq> MergeMap;
  MergeMap mm;
  map<time_t, string> time2str;
  size_t cnt = 0;
  for (size_t i = 0; i < n.size(); ++i) {
    //Int type = n.at(i)->type & 0x8FFFFFFF;
    Int type = RMessageUtil::instance().Util_GetRMessageType(n.at(i)->type);
    Long source = n.at(i)->source;
    if (!MergeConfig::instance().MustMerge(type, view)) {
      string txt = SchemaManager::instance().toReplyTxt(n.at(i), view,
          is_kaixin, fail_ids);
      if (!txt.empty()) {
        time2str[n.at(i)->time] = txt;
      }
      ++cnt;
    } else {
      mm[make_pair(type, source)].push_back(n.at(i));
    }
  }
  if (!mm.empty()) {
    for (MergeMap::iterator i = mm.begin(); i != mm.end(); ++i) {
      string txt = SchemaManager::instance().toReplyTxtMerge(i->second, view,
          is_kaixin, fail_ids);
      if (!txt.empty()) {
        time_t max_time = 0;
        for (size_t k = 0; k != i->second.size(); k++) {
          max_time = i->second.at(k)->time > max_time ? i->second.at(k)->time
              : max_time;
        }
        time2str[max_time] = txt;
      }
    }
  }

  for (map<time_t, string>::reverse_iterator i = time2str.rbegin(); i
      != time2str.rend(); ++i) {
    s.push_back(i->second);
  }
  return;
}

void NotifyGateI::BodySeq2StrSeqXRmessage(const NotifyBodySeq &n, 
    Ice::Int view, StrSeq &s, map<Ice::Long, int>& fail_ids, bool is_kaixin){
  if (n.empty()) {
    return;
  }
  typedef map<pair<Int, Long> , NotifyBodySeq> MergeMap;
  MergeMap mm;
  map<time_t, vector<string> > time2str;
  size_t cnt = 0;
  for (size_t i = 0; i < n.size(); ++i) {
    MCE_DEBUG("NotifyGateI::BodySeq2StrSeqXRmessage id:" << n.at(i)->id 
        << " type:" << n.at(i)->type << " source:" << n.at(i)->source
        << " from:" << n.at(i)->from);
    Int type = RMessageUtil::instance().Util_GetRMessageType(n.at(i)->type);
    if(type == 193){
      fail_ids[n.at(i)->id] = type;
      MCE_INFO("NotifyGateI::BodySeq2StrSeqXRmessage kill id:" << n.at(i)->id 
        << " type:" << n.at(i)->type << " source:" << n.at(i)->source
        << " from:" << n.at(i)->from);
    }
    Ice::Long source = n.at(i)->source;
    if (!SchemaManager::instance().MustMerge(type, view)) {
      string txt = SchemaManager::instance().toReplyTxt(n.at(i), view, is_kaixin, fail_ids);
      if (!txt.empty()) {
        time2str[n.at(i)->time].push_back(txt);
      } else {
        //MCE_INFO("NotifyGateI::BodySeq2StrSeqXRmessage. no merge uid:" 
        //<< n.at(i)->id << " type:" << type << " txt empty");
      }
      ++cnt;
    } else {
      mm[make_pair(type, source)].push_back(n.at(i));
    }
  }
  if (!mm.empty()) {
    for (MergeMap::iterator i = mm.begin(); i != mm.end(); ++i) {
      string txt = SchemaManager::instance().toReplyTxtMerge(i->second, view, is_kaixin, fail_ids);
      //int type = i->second.at(0)->type;
      if (!txt.empty()) {
        time_t max_time = 0;
        for (size_t k = 0; k != i->second.size(); k++) {
          max_time = i->second.at(k)->time > max_time ? i->second.at(k)->time : max_time;
        }
        time2str[max_time].push_back(txt);
      }
    }
  }

  for (map<time_t, vector<string> >::reverse_iterator i = time2str.rbegin();
      i != time2str.rend(); ++i) {
    s.insert(s.end(), i->second.begin(), i->second.end());
  }
  return;
}


void NotifyGateI::BodySeq2StrSeqX(const NotifyBodySeq& n, Ice::Int view, StrSeq& s, 
    vector<Ice::Long> &fail_ids, bool is_kaixin) {
  if (n.empty()) {
    return;
  }

  typedef map<pair<Int, Long> , NotifyBodySeq> MergeMap;
  MergeMap mm;
  map<time_t, vector<string> > time2str;
  size_t cnt = 0;
  for (size_t i = 0; i < n.size(); ++i) {
    Int type = RMessageUtil::instance().Util_GetRMessageType(n.at(i)->type);
    if(type == 193){
      fail_ids.push_back(n.at(i)->id);
      MCE_INFO("NotifyGateI::BodySeq2StrSeqX.kill type:" << type <<  " nid:" << n.at(i)->id);
    }
    Long source = n.at(i)->source;
    if (!SchemaManager::instance().MustMerge(type, view)) {
      string txt = SchemaManager::instance().toReplyTxt(n.at(i), view, is_kaixin, fail_ids);
      if (!txt.empty()) {
        time2str[n.at(i)->time].push_back(txt);
        //MCE_INFO("NotifyGateI::BodySeq2StrSeqX. no merge uid:" << n.at(i)->id << " type:" << type << " txt not empty");
      } else {
        //fail_ids.push_back(n.at(i)->id);
        //MCE_INFO("NotifyGateI::BodySeq2StrSeqX. no merge uid:" << n.at(i)->id << " type:" << type << " txt empty delete");
      }
      ++cnt;
    } else {
      //MCE_INFO("NotifyGateI::BodySeq2StrSeqX. merge uid:" << n.at(i)->id << " type:" << type );
      mm[make_pair(type, source)].push_back(n.at(i));
    }
  }
  if (!mm.empty()) {
    for (MergeMap::iterator i = mm.begin(); i != mm.end(); ++i) {
      string txt = SchemaManager::instance().toReplyTxtMerge(i->second, view, is_kaixin, fail_ids);
      //int type = i->second.at(0)->type;
      if (!txt.empty()) {
        time_t max_time = 0;
        for (size_t k = 0; k != i->second.size(); k++) {
          max_time = i->second.at(k)->time > max_time ? i->second.at(k)->time : max_time;
        }
        time2str[max_time].push_back(txt);
      //  MCE_INFO("NotifyGateI::BodySeq2StrSeqX. txt not empty. type:" << type << " id:" << i->second.at(0)->id );
      } else {
      //  MCE_INFO("NotifyGateI::BodySeq2StrSeqX. txt empty. type:" << type << " id:" << i->second.at(0)->id );
      }
    }
  }

  for (map<time_t, vector<string> >::reverse_iterator i = time2str.rbegin(); i != time2str.rend(); ++i) {
    s.insert(s.end(), i->second.begin(), i->second.end());
    //    s.push_back(i->second);
  }

  return;
}

/*
void NotifyGateI::BodySeq2StrSeqX(int login_uid, const NotifyBodySeq & n, Ice::Int view, StrSeq & s, vector<Ice::Long> &fail_ids, bool is_kaixin) {
  if (n.empty()) {
    return;
  }

  typedef map<pair<Int, Long> , NotifyBodySeq> MergeMap;
  MergeMap mm;
  map<time_t, vector<string> > time2str;
  size_t cnt = 0;
  for (size_t i = 0; i < n.size(); ++i) {
    //MCE_DEBUG("NotifyGateI::BodySeq2StrSeqX-->" << n.at(i).type << " ");
    //Int type = n.at(i)->type & 0x8FFFFFFF;
    Int type = RMessageUtil::instance().Util_GetRMessageType(n.at(i)->type);
    if(type == 193){
      fail_ids.push_back(n.at(i)->id);
      MCE_INFO("NotifyGateI::BodySeq2StrSeqX.kill type:" << type <<  " nid:" << n.at(i)->id);
    }
    Long source = n.at(i)->source;
    if (!SchemaManager::instance().MustMerge(type, view)) {
      string txt = "";
      if(login_uid == 345350729 || login_uid == 232626782){
        MCE_INFO("NotifyGateI::BodySeq2StrSeqX-->:login_uid = " << login_uid);
        txt = SchemaManager::instance().toReplyTxtTest(n.at(i), view, is_kaixin, fail_ids);
      }else{
        txt = SchemaManager::instance().toReplyTxt(n.at(i), view, is_kaixin, fail_ids);
      }
      if (!txt.empty()) {
        time2str[n.at(i)->time].push_back(txt);
        //MCE_INFO("NotifyGateI::BodySeq2StrSeqX. no merge uid:" << n.at(i)->id << " type:" << type << " txt not empty");
      } else {
        //fail_ids.push_back(n.at(i)->id);
        //MCE_INFO("NotifyGateI::BodySeq2StrSeqX. no merge uid:" << n.at(i)->id << " type:" << type << " txt empty delete");
      }
      ++cnt;
    } else {
      MCE_INFO("NotifyGateI::BodySeq2StrSeqX. merge uid:" << n.at(i)->id << " type:" << type );
      mm[make_pair(type, source)].push_back(n.at(i));
    }
  }
  if (!mm.empty()) {
    for (MergeMap::iterator i = mm.begin(); i != mm.end(); ++i) {
      string txt = SchemaManager::instance().toReplyTxtMerge(i->second, view, is_kaixin, fail_ids);
      int type = i->second.at(0)->type;
      if (!txt.empty()) {
        time_t max_time = 0;
        for (size_t k = 0; k != i->second.size(); k++) {
          max_time = i->second.at(k)->time > max_time ? i->second.at(k)->time : max_time;
        }
        time2str[max_time].push_back(txt);
        MCE_INFO("NotifyGateI::BodySeq2StrSeqX. txt not empty. type:" << type << " id:" << i->second.at(0)->id );
      } else {
        MCE_INFO("NotifyGateI::BodySeq2StrSeqX. txt empty. type:" << type << " id:" << i->second.at(0)->id );
      }
    }
  }

  for (map<time_t, vector<string> >::reverse_iterator i = time2str.rbegin(); i != time2str.rend(); ++i) {
    s.insert(s.end(), i->second.begin(), i->second.end());
    //    s.push_back(i->second);
  }

  return;
}
*/


StrSeq NotifyGateI::getNotifySeq(Ice::Int uid, Ice::Int view,
    Ice::Long last_nid, Ice::Int limit, const Ice::Current&) {
	TimeStat ts;
  MCE_INFO("NotifyGateI::getNotifySeq. uid:" << uid << " view:" << view << " last_nid:" << last_nid
      << " limit:" << limit);
  StrSeq ret;
  NotifyBodySeq seq;
  vector<Ice::Long> fail_ids;
  try {
    seq = NotifyIndexAdapter::instance().getNotifySeq(uid, last_nid, limit);
    BodySeq2StrSeq(seq, view, ret, fail_ids);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyGateI::getNotifySeq exception : " << e);
  }
  if (!fail_ids.empty()) {
    RemoveIndex(uid, fail_ids);
  }
  MCE_INFO("NotifyGateI::getNotifySeq. uid:" << uid << " NotifyBodySeq size:" 
      << seq.size() <<" result size:" << ret.size());
	FunStatManager::instance().Stat("NotifyGateI::getNotifySeq", ts.getTime(), false);
  return ret;
}

StrSeq NotifyGateI::getNotifySeqX(Ice::Int uid, Ice::Int view,
    Ice::Long last_nid, Ice::Int limit, const Ice::Current&) {
	TimeStat ts;
  StrSeq ret;
  NotifyBodySeq seq;
  vector<Ice::Long> fail_ids;
  try {
    seq = NotifyIndexAdapter::instance().getNotifySeqX(uid, view, last_nid,
        limit);
    BodySeq2StrSeqX(seq, view, ret, fail_ids);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyGateI::getNotifySeq exception : " << e << " uid:" << uid);
  }
  if (!fail_ids.empty()) {
    RemoveIndex(uid, fail_ids);
  }
  MCE_INFO("NotifyGateI::getNotifySeqX. uid[" << uid << "]NotifyBodySeq size:"
      << seq.size() <<" result size:" << ret.size());
	FunStatManager::instance().Stat("NotifyGateI::getNotifySeqX", ts.getTime(), false);
  return ret;
}

StrSeq NotifyGateI::getReplySeqX(Ice::Int uid, Ice::Int view,
    Ice::Long last_nid, Ice::Int limit, const Ice::Current&) {
	TimeStat ts;
  StrSeq ret;
  NotifyBodySeq seq;
  vector<Ice::Long> fail_ids;
  try {
    seq = NotifyIndexAdapter::instance().getReplySeqX(uid, view, last_nid,
        limit);
    BodySeq2StrSeqX(seq, view, ret, fail_ids);

  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyGateI::getReplySeqX exception : " << e << " uid:" << uid);
  }
  if (!fail_ids.empty()) {
    RemoveIndex(uid, fail_ids);
  }
  MCE_INFO("NotifyGateI::getReplySeqX. uid:" << uid << " NotifyBodySeq size:" << seq.size() <<" result size:" << ret.size());
	FunStatManager::instance().Stat("NotifyGateI::getReplySeqX", ts.getTime(), false);
  return ret;
}

StrSeq NotifyGateI::getReplySeq(Ice::Int uid, Ice::Int view,
    Ice::Long last_nid, Ice::Int limit, const Ice::Current&) {
	TimeStat ts;
  MCE_INFO("NotifyGateI::getReplySeq. uid:" << uid << " view:" << view << " last_nid:" << last_nid
      << " limit:" << limit);
  StrSeq ret;
  NotifyBodySeq seq;
  vector<Ice::Long> fail_ids;
  try {
    seq = NotifyIndexAdapter::instance().getReplySeq(uid, last_nid, limit);
    BodySeq2StrSeq(seq, view, ret, fail_ids);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyGateI::getReplySeq exception : " << e << " uid:" << uid << " view:" << view << " last_nid:" << last_nid
        << " limit:" << limit );
  }
  if (!fail_ids.empty()) {
    RemoveIndex(uid, fail_ids);
  }
	FunStatManager::instance().Stat("NotifyGateI::getReplySeq", ts.getTime(), false);
  return ret;
}

StrSeq NotifyGateI::getByType(Ice::Int uid, Ice::Int view,
    const MyUtil::IntSeq& types, Ice::Long last_nid, Ice::Int limit,
    const Ice::Current&) {
	TimeStat ts;
  MCE_INFO("NotifyGateI::getByType. uid:" << uid << " view:" << view << " last_nid:" << last_nid
      << " limit:" << limit << " types size:" << types.size());
  StrSeq ret;
  NotifyBodySeq seq;
  try {
    seq = NotifyIndexAdapter::instance().getByType(uid, types, last_nid, limit);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyGateI::getByType exception : " << e<< " uid:" << uid << " view:" << view
        << " last_nid:" << last_nid << " limit:" << limit << " types size:" << types.size());
    return ret;
  }
  vector<Ice::Long> fail_ids;
  BodySeq2StrSeq(seq, view, ret, fail_ids);
  if (!fail_ids.empty()) {
    RemoveIndex(uid, fail_ids);
  }
	FunStatManager::instance().Stat("NotifyGateI::getByType", ts.getTime(), false);
  return ret;
}

StrSeq NotifyGateI::getNotifySeq2(Ice::Int uid, Ice::Int view, Ice::Int begin,
    Ice::Int limit, const Ice::Current&) {
	TimeStat ts;
  StrSeq ret;
  NotifyBodySeq seq;
  TimeStat t1, ttotal;
  float index_t = 0.0f, tostr_t = 0.0f, total_t;
  try {
    seq = NotifyIndexAdapter::instance().getNotifySeq2(uid, begin, limit);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyGateI::getNotifySeq2 exception : " << e << " uid:" << uid << " view:" << view
        << " begin:" << begin << " limit:" << limit);
    return ret;
  }
  MCE_DEBUG("NotifyGateI::getNotifySeq2: index return seq size:" << seq.size());
  index_t = t1.getTime();
  vector<Ice::Long> fail_ids;
  BodySeq2StrSeq(seq, view, ret, fail_ids);

  if (!fail_ids.empty()) {
    RemoveIndex(uid, fail_ids);
  }
  size_t ret_size = ret.size();
  for (size_t i = 0; i < ret_size; ++i) {
    MCE_DEBUG("NotifyGateI::getNotifySeq2:" << i << ret[i]);
  }
  tostr_t = t1.getTime();
  total_t = ttotal.getTime();
  ostringstream oss;
  oss << " index:" << index_t << " tostr:" << tostr_t << " total:" << total_t;
  MCE_INFO("NotifyGateI::getNotifySeq2 seq.size(): " << seq.size() 
      << " ret.size()=" << ret.size() << " uid:" << uid << " view:" << view
      << " begin:" << begin << " limit:" << limit << " #COST#" << oss.str() );
	FunStatManager::instance().Stat("NotifyGateI::getNotifySeq2", ts.getTime(), false);
  return ret;
}

StrSeq NotifyGateI::getReplySeq2(Ice::Int uid, Ice::Int view, Ice::Int begin,
    Ice::Int limit, const Ice::Current&) {
	TimeStat ts;
  MCE_INFO("NotifyGateI::getReplySeq2. uid:" << uid << " view:" << view << " begin:" << begin
      << " limit:" << limit);
  StrSeq ret;
  NotifyBodySeq seq;
  TimeStat t1, ttotal;
  float index_t = 0.0f, tostr_t = 0.0f, total_t;
  try {
    seq = NotifyIndexAdapter::instance().getReplySeq2(uid, begin, limit);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyGateI::getNotifySeq exception : " << e << " uid:" << uid << " view:" << view
        << " begin:" << begin << " limit:" << limit);
    return ret;
  }
  index_t = t1.getTime();
  vector<Ice::Long> fail_ids;
  BodySeq2StrSeq(seq, view, ret, fail_ids);

  if (!fail_ids.empty()) {
    RemoveIndex(uid, fail_ids);
  }
  tostr_t = t1.getTime();
  total_t = ttotal.getTime();
  ostringstream oss;
  oss << " index:" << index_t << " tostr:" << tostr_t << " total:" << total_t;
  MCE_INFO("NotifyGateI::getReplySeq2 seq.size:" << seq.size() << " ret.size=" 
      << ret.size() << " uid:" << uid << " view:" << view
      << " begin:" << begin << " limit:" << limit << " #COST#" << oss.str());
	FunStatManager::instance().Stat("NotifyGateI::getReplySeq2", ts.getTime(), false);
  return ret;
}

StrSeq NotifyGateI::getByType2(Ice::Int uid, Ice::Int view,
    const MyUtil::IntSeq& types, Ice::Int begin, Ice::Int limit,
    const Ice::Current&) {
	TimeStat ts;
  StrSeq ret;
  NotifyBodySeq seq;
  TimeStat t1, ttotal;
  float index_t, total_t;
  try {
    seq = NotifyIndexAdapter::instance().getByType2(uid, types, begin, limit);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyIndexAdapter::getByType2 exception:" << e << " uid:" 
        << uid << " view:" << view << " begin:" << begin << " limit:" << limit);
    return ret;
  }
  index_t = t1.getTime();
  vector<Ice::Long> fail_ids;
  BodySeq2StrSeq(seq, view, ret, fail_ids);
  if (!fail_ids.empty()) {
    RemoveIndex(uid, fail_ids);
  }
  total_t = ttotal.getTime();
  ostringstream oss;
  oss << " index:" << index_t << " total:" << total_t;
  MCE_INFO("getByType2 bodys:" << seq.size() << " strseqs:" 
      << ret.size() << " uid:" << uid << " view:" << view << " begin:" 
      << begin << " limit:" << limit << " types:" << types.size() 
      << " COST:" << oss.str());
	FunStatManager::instance().Stat("NotifyGateI::getByType2", ts.getTime(), false);
  return ret;
}

bool RemoveRequest::response() {
  //remove.notify?uid=1234&nid=12&source=12-123
  if (!_fcgi_out) {
    MCE_WARN("remove notify fcgi_out null");
    return false;
  }

  if (!Request::response()) {
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\nverify failure");
    return false;
  }

  //MCE_DEBUG("ticket verify ok");

  Ice::Long nid = getPropertyLong("nid");
  int type = getPropertyLong("type");

  Ice::Long source = -1;
  int bigtype = 1;
  vector<Long> nids;
  string url_nl = getProperty("nl");
  string url_source = getProperty("source");

  try {
    boost::tokenizer<> tok(url_nl);
    for (boost::tokenizer<>::iterator tit = tok.begin(); tit != tok.end(); ++tit) {
      nids.push_back(lexical_cast<Long> (*tit));
    }
  } catch (...) {
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html; charset=utf-8\r\n\r\n-1");
    MCE_DEBUG("seperate notify id list fail");
    return false;
  }

  size_t pos = url_source.find_first_of(SOURCE_SEPERATOR);
  if (pos != string::npos) {
    try {
      type = boost::lexical_cast<Ice::Int>(url_source.substr(0, pos));
      source = boost::lexical_cast<Ice::Long>(url_source.substr(pos + 1));
    } catch (...) {
      FCGX_FPrintF(_fcgi_out, "Content-type: text/html; charset=utf-8\r\n\r\n-1");
      return false;
    }
  }

  if (nids.empty() && nid == -1 && source == -1) {
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html; charset=utf-8\r\n\r\n-1");
    return false;
  }
  try {
    if (!nids.empty()) {
      MCE_DEBUG("call removeByIds~~~");
      NotifyIndexAdapter::instance().removeByIds(_login_uid, nids);
      NotifyIndexNAdapter::instance().RemoveRMessageByIds(_login_uid, nids, type, bigtype);
    } else if (source >= 0) {
      MCE_DEBUG("call removeBySource");
      bigtype = SchemaManager::instance().GetBigtypeBytype(type);
      NotifyIndexAdapter::instance().removeBySource(_login_uid, type, source);
      NotifyIndexNAdapter::instance().RemoveRMessageBySource(_login_uid, type, source, bigtype);
    } else if (nid >= 0) {
      MCE_DEBUG("call removeById");
      NotifyIndexAdapter::instance().removeById(_login_uid, nid);
      NotifyIndexNAdapter::instance().RemoveRMessageById(_login_uid, nid, type, bigtype);
    }
  } catch (Ice::Exception& e) {
    MCE_DEBUG("NotifyIndexAdapter remove exception : " << e << " uid=" << _login_uid);
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\n-2");
    return false;
  }
  FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\n0");
  return true;
}

bool GetRequest::response() {
  float index_t = 0.0f;
  float tostr_t = 0.0f;
  float remove_t = 0.0f;
  TimeStat t1, ttotal;
  if (!_fcgi_out) {
    MCE_WARN("get notify fcgi_out null");
    return false;
  }
  if (!Request::response()) {
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\nverify failure");
    return false;
  }
  string emptyRet = "Content-type: text/html\r\n\r\n[]";

  NotifyBodySeq seq;
  string type_str = getProperty("type");
  Ice::Int view = getPropertyInt("view");
  Ice::Long nid = getPropertyLong("nid");
  Ice::Int limit = getPropertyInt("limit");
  Ice::Int reply = getPropertyInt("reply");

  if (view < 0) {
    MCE_DEBUG("get.notify param error. view = " << view);
    FCGX_FPrintF(_fcgi_out, emptyRet.c_str());
    return false;
  }

  if (limit <= 0)
    limit = 10;

  MyUtil::IntSeq typeSeq;
  if (!type_str.empty()) {
    try {
      size_t pos = 0;
      size_t end;
      // TODO : user boost::split
      while ((end = type_str.find(",", pos)) != string::npos) {
        typeSeq.push_back(boost::lexical_cast<int>(type_str.substr(pos, end - pos)));
        pos = end + 1;
      }
      typeSeq.push_back(boost::lexical_cast<int>(type_str.substr(pos,
          type_str.length() - pos)));
    } catch (boost::bad_lexical_cast & e) {
      MCE_WARN("bad_lexical_cast. line : " << __LINE__ << "," << e.what());
    }
  }

  try {
    if (typeSeq.empty()) {
      if (reply > 0) {
        seq = NotifyIndexAdapter::instance().getReplySeqX(_login_uid, view,
            nid, limit);
        MCE_DEBUG("NotifyIndex:getReplySeqX cost:  " << t1.getTime(false));

      } else {
        seq = NotifyIndexAdapter::instance().getNotifySeqX(_login_uid, view,
            nid, limit);
        MCE_DEBUG("NotifyIndex:getNotifySeqX cost:  " << t1.getTime(false));
      }
    } else {
      seq = NotifyIndexAdapter::instance().getByType(_login_uid, typeSeq, nid,
          limit);
      MCE_DEBUG("NotifyIndex:getByType cost:  " << t1.getTime(false));
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyIndexAdapter get exception : " << e << " uid=" << _login_uid);
  }
  index_t = t1.getTime();
  MCE_DEBUG("GetRequest::response NotifyBody seq.size() :  " << seq.size());
  string ss;
  ss = "Content-type: text/html; charset=utf-8\r\n\r\n[";

  /////////////yjw替换原来的字符串生成代码//////////////////
  int json_count = 0;
  bool is_kaixin = (host().find("kaixin") != string::npos);

  StrSeq ret;
  vector<Ice::Long> fail_ids;
  NotifyGateI::instance().BodySeq2StrSeqX(seq, view, ret, fail_ids, is_kaixin);
  size_t retSeqSize = ret.size();
  for (size_t h = 0; h < retSeqSize; ++h) {
    string txt = ret.at(h);
    if (++json_count > 1)
      ss += ',';
    ss += txt;
  }
  ss += ']';
  tostr_t = t1.getTime();
  FCGX_FPrintF(_fcgi_out, "%s", ss.c_str());
  MCE_DEBUG("GetRequest::response send to web " << ss);
  if (!fail_ids.empty()) {
    RemoveIndex(_login_uid, fail_ids);
  }
  remove_t = t1.getTime();
  ostringstream oss;
  oss << " index:" << index_t << " tostr:" << tostr_t << " remove:" << remove_t
      << " total:" << ttotal.getTime();
  MCE_INFO("GetRequest::response. uid: " << _login_uid 
      << " get.notify body_size=" << ss.size()
      << " notify_count=" << seq.size() << " ret size:" << ret.size()
      << " type: " << type_str << " nid: " << nid
      << " limit: " << limit << " reply: " << reply
      << " #COST# " << oss.str());
  return true;
}

bool checkMd5(string uid, const string & key) {
  string orig = uid;
  uid += kClubDispatchKey;
  unsigned char* md5 = MD5((unsigned char*) uid.c_str(), uid.size(), NULL);

  stringstream res;
  for (size_t i = 0; i < MD5_DIGEST_LENGTH; ++i) {
    res << hex << setw(2) << setfill('0') << (unsigned int) md5[i];
  }
  MCE_INFO("checkMd5.input seed:"<< orig <<",generate md5:" << res.str() << ",input key:" << key );
  return res.str() == key;
}

bool DispatchRequest::response() {
  //
  // dispatch.notify?schema=32&type=1
  // POST BODY : source=123&from=1231&....
  //
  if (!_fcgi_out) {
    MCE_WARN("get notify fcgi_out null");
    return false;
  }
  //Request::response(); // 不调用基类response()

  MyUtil::Str2StrMap dispatch_map;

  string schema = getProperty("schema");
  string type = getProperty("type");
  string post_body = UriDecode(getProperty("post_body"));
  MCE_INFO("DispatchRequest::response. schema:" << schema << " type:" << type);
  if (schema.empty() || type.empty() || post_body.empty()) {
    MCE_DEBUG("dispatch.notify param error");
    FCGX_FPrintF(_fcgi_out,
        "Content-type: text/html\r\n\r\n-2,schema type or post is empty");
    return false;
  }

  dispatch_map.insert(make_pair("schema_id", schema));
  dispatch_map.insert(make_pair("type", type));

  std::vector<std::string> strs;
  boost::split(strs, post_body, boost::is_any_of("&"), boost::token_compress_on);
  for (size_t i = 0; i < strs.size(); ++i) {
    size_t pos = strs[i].find('=');
    if (pos != string::npos) {
      dispatch_map.insert(make_pair(strs[i].substr(0, pos), strs[i].substr(pos
          + 1)));
    }
  }

  MyUtil::Str2StrMap::iterator it = dispatch_map.find("key");

  if (it == dispatch_map.end()) {
    //kClubDispatchKey
    string out = "Content-type: text/html\r\n\r\n-4,md5 key is absent";
    FCGX_FPrintF(_fcgi_out, out.c_str());
    return false;
  } else {
    MyUtil::Str2StrMap::iterator f = dispatch_map.find("from");
    MyUtil::Str2StrMap::iterator t = dispatch_map.find("to");
    MyUtil::Str2StrMap::iterator tp = dispatch_map.find("type");
    if (it == dispatch_map.end() || f == dispatch_map.end() || t
        == dispatch_map.end() || tp == dispatch_map.end()) {
      FCGX_FPrintF(_fcgi_out,
          "Content-type: text/html\r\n\r\n-5,from,to or type is absent");
      return false;
    }
    string tmp;
    tmp = f->second + "|" + t->second + "|" + tp->second;
    if (!checkMd5(tmp, it->second)) {
      FCGX_FPrintF(_fcgi_out,
          "Content-type: text/html\r\n\r\n-6,MD5 not match,");
      return false;
    }
  }

  dispatch_map.erase("key");

  try {
    NotifyDispatcherAdapter::instance().dispatch(dispatch_map);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyDispatcherAdapter dispatch exception : " << e <<" uid=" << _login_uid );
    string out = "Content-type: text/html\r\n\r\n-3,call notify fail";
    out += e.what();
    FCGX_PutS(out.c_str(), _fcgi_out);
    return false;
  }

  FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\n0,success");
  return true;
}

bool UnreadCountRequest::response() {
  //
  // unreadcount.notify?uid=1234
  //
  if (!_fcgi_out) {
    MCE_WARN("pager count fcgi_out null");
    return false;
  }

  if (!Request::response()) {
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\nverify failure");
    return false;
  }

  string emptyRet = "Content-type: text/html\r\n\r\n[]";

  MCE_DEBUG("ticket verify ok");

  Ice::Int count = 0;
  try {
    count = NotifyIndexAdapter::instance().getUnreadCount(_login_uid);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyIndexAdapter get pager count exception : " << e << " uid=" << _login_uid);
  }

  stringstream ss;
  ss << "Content-type: text/html; charset=utf-8\r\n\r\n";
  ss << count;

  FCGX_FPrintF(_fcgi_out, ss.str().c_str());
  return true;
}

bool QuitChatRequest::response() {
  //
  // quitchat.notify?uid=1234
  //
  if (!_fcgi_out) {
    MCE_WARN("pager count fcgi_out null");
    return false;
  }

  if (!Request::response()) {
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\nverify failure");
    return false;
  }

  MCE_DEBUG("ticket verify ok");

  string groups_str = getProperty("groups");
  GroupIndexSeq groups;
  try {
    tokenizer<> tok(groups_str);
    int size = 0;
    for (tokenizer<>::iterator tit = tok.begin(); tit != tok.end(); ++tit) {
      ++size;
    }
    if (size == 0 || size % 3 != 0) {
      MCE_WARN("QuitChatRequest::response wrong fomat! groups_str:" << groups_str);
      FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\n-1");
      return false;
    }
    for (tokenizer<>::iterator tit = tok.begin(); tit != tok.end();) {
      GroupIndex g;
      g.source = lexical_cast<Ice::Long> (*tit++);
      g.stype = lexical_cast<Ice::Int> (*tit++);
      g.actor = lexical_cast<Ice::Int> (*tit++);
      groups.push_back(g);
    }
  } catch (...) {
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html; charset=utf-8\r\n\r\n-2");
    return false;
  }

  MCE_DEBUG("QuitChat handle, uid:" << _login_uid <<":groups size:" << groups.size());
  try {
    NotifyDispatcherNAdapter::instance().QuitChat(_login_uid, groups);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyDispatcherNAdapter QuitChat FCGI exception : " << e << " uid=" << _login_uid);
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\n-3");
    return false;
  }
  FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\n0");
  return true;
}
string NotifyTargetHandler::ToString(const NotifyTargetPtr & t){
  ostringstream os;
  os << " uids:" << PrintHelper::Print(t->uids)
      << " minigroups:" << PrintHelper::Print(t->minigroups)
      << " minigroupMembers:" <<PrintHelper::Print( t->minigroupMembers)
      << " ugcUids:" <<PrintHelper::Print( t->ugcUids)
      << " ugcMinigroups:" << PrintHelper::Print(t->ugcMinigroups)
      << " ugcBlockUids:" <<PrintHelper::Print( t->ugcBlockUids);
  return os.str();
}
//======================================================================RMESSAGE===================================
bool GetRMessageRequest::response(){
  if (!_fcgi_out) {
    MCE_WARN("GetRMessageRequest::response --> fcgi_out null");
    return false;
  }
  
  if (!Request::response()) {
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\nverify failure");
    return false;
  }
  string emptyRet = "Content-type: text/html\r\n\r\n[]";
  string type_str = getProperty("type");
  Ice::Int view = getPropertyInt("view");
  Ice::Long nid = getPropertyLong("nid");
  Ice::Int para_limit = getPropertyInt("limit");
  int limit = para_limit;
  Ice::Int begin = getPropertyInt("begin");
  //Ice::Int reply = getPropertyInt("reply");
  Ice::Int bigtype = getPropertyInt("bigtype");
  //标记是否是按照bigtype 获取  即是调用 : GetRMessageByBigtype
  Ice::Int getbybigtype = getPropertyInt("getbybigtype");
  if (view < 0) {
    MCE_DEBUG("GetRMessageRequest::response --> get view error view:" << view);
    FCGX_FPrintF(_fcgi_out, emptyRet.c_str());
    return false;
  }
  if (limit <= 0){
    limit = 10;
  }
  if(begin < 0){
    begin = 0;
  }

  MyUtil::IntSeq typeSeq;
  if (!type_str.empty()) {
    try {
      size_t pos = 0;
      size_t end;
      // TODO : user boost::split
      while ((end = type_str.find(",", pos)) != string::npos) {
        typeSeq.push_back(boost::lexical_cast<int>(type_str.substr(pos, end - pos)));
        pos = end + 1;
      }
      typeSeq.push_back(boost::lexical_cast<int>(type_str.substr(pos, type_str.length() - pos)));
    } catch (boost::bad_lexical_cast & e) {
      MCE_WARN("GetRMessageRequest::response --> bad lexical cast:" << e.what());
    }
  }

  string ss;
  ss = "Content-type: text/html; charset=utf-8\r\n\r\n[";
  
  int json_count = 0;
  bool is_kaixin = (host().find("kaixin") != string::npos);

  NotifyBodySeq bodyseq;
  vector<Ice::Long> not_in_db_nids;
  
  StrSeq ret;
  vector<Ice::Long> fail_ids;
  map<Ice::Long, int> mp_fail_ids;

  try {
    if (getbybigtype > 0) {
      if (bigtype == 1) {
        //bodyseq = NotifyIndexNAdapter::instance().GetRemindMessage(_login_uid, begin, limit);
        bodyseq = NotifyGateI::instance().GetRemindMsg2Page(_login_uid, begin, limit);
      } else {
        bodyseq = NotifyIndexNAdapter::instance().GetRMessageByBigtype(_login_uid, begin, limit,bigtype);
        MCE_DEBUG("GetRMessageRequest::response GetRMessageByBigtype uid:" 
          << _login_uid << " view:" << view << " nid:" << nid << " limit:" 
          << limit << " bigtype:" << bigtype);
      }
    } else {
      if (typeSeq.empty()) {
        bodyseq = NotifyIndexNAdapter::instance().GetRMessageMerge(_login_uid, view, nid, limit, bigtype);
        MCE_DEBUG("GetRMessageRequest::response GetRMessageMerge uid:" 
            << _login_uid << " view:" << view << " nid:" << nid << " limit:"
            << limit << " bigtype:" << bigtype);
      } else {
        bodyseq = NotifyIndexNAdapter::instance().GetRMessageByType(_login_uid, typeSeq, begin, para_limit, bigtype);
        MCE_DEBUG("GetRMessageRequest::response GetRmessageByType uid:" 
             << _login_uid << " view:" << view << " nid:" << nid << " limit:" 
             << limit << " bigtype:" << bigtype);
      }
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyIndexNAdapter uid:" << _login_uid
             << " view:" << view << " nid:" << nid << " limit:" 
             << limit << " bigtype:" << bigtype << " exception:" << e);
  }
  try {
    if (bigtype == 1) {
      NotifyGateI::instance().BodySeq2StrSeq(bodyseq, view, ret, fail_ids);
    } else {
      NotifyGateI::instance().BodySeq2StrSeqXRmessage(bodyseq, view, ret, mp_fail_ids, is_kaixin);
    }
  }
  catch(...) {
    MCE_WARN("NotifyGateI::BodySeq2StrSeqXRmessage exception");
  }
  MCE_DEBUG("GetRMessageRequest::response mp_fail_ids:" 
      << mp_fail_ids.size() << " ret_size:" << ret.size());
  for (size_t h = 0; h < ret.size(); ++h) {
    string txt = ret.at(h);
    if (++json_count > 1)
      ss += ',';
    ss += txt;
  }
  ss += ']';
  FCGX_FPrintF(_fcgi_out, "%s", ss.c_str());
  if (!mp_fail_ids.empty()) {
    RemoveIndexRmessage(_login_uid, mp_fail_ids);
  }
  return true;
}

bool GetUnreadCountRequest::response(){
  if (!_fcgi_out) {
    MCE_WARN("GetRMessage fcgi_out null");
    return false;
  }
  if (!Request::response()) {
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\nverify failure");
    return false;
  }
  string emptyRet = "Content-type: text/html\r\n\r\n[]";
  /*
  Ice::Int bigtype = getPropertyInt("bigtype");
  if (bigtype < 0 || bigtype > 3) {
    MCE_DEBUG("get.notify param error. bigtype = " << bigtype);
    FCGX_FPrintF(_fcgi_out, emptyRet.c_str());
    return false;
  }
  */
  ostringstream res;
  res <<"Content-type: text/html\r\n\r\n";
  MyUtil::Int2IntMap uc;
  try{
    MyUtil::IntSeq types;
    types.push_back(1);
    types.push_back(2);
    types.push_back(3);
    uc = NotifyIndexNAdapter::instance().GetUnreadCountByBigtypes(_login_uid, types);
  }catch(Ice::Exception& e){
    MCE_WARN("GetUnreadCountRequest::response-->GetUnreadCountByBigtype uid[" 
        << _login_uid << "]err=" << e);
  }
  MyUtil::Int2IntMap::iterator it = uc.find(1);
  if(it != uc.end()){
    int cc = it->second;
    res << "{remind:" << cc;
  }
  it = uc.find(2);
  if(it != uc.end()){
    int cc = it->second;
    res << ",apply:" << cc;
  }
  it = uc.find(3);
  if(it != uc.end()){
    int cc = it->second;
    res << ",notice:" << cc << "}";
  }
  FCGX_FPrintF(_fcgi_out, res.str().c_str());
  return true;
}


bool GetUndealCountRequest::response(){
  if (!_fcgi_out) {
    MCE_WARN("GetRMessage fcgi_out null");
    return false;
  }
  if (!Request::response()) {
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\nverify failure");
    return false;
  }
  string emptyRet = "Content-type: text/html\r\n\r\n[]";
  /*
  Ice::Int bigtype = getPropertyInt("bigtype");
  if (bigtype < 0 || bigtype > 3) {
    MCE_DEBUG("get.notify param error. bigtype = " << bigtype);
    FCGX_FPrintF(_fcgi_out, emptyRet.c_str());
    return false;
  }
  */
  ostringstream res;
  res <<"Content-type: text/html\r\n\r\n";
  MyUtil::Int2IntMap uc;
  try{
    MyUtil::IntSeq types;
    types.push_back(1);
    types.push_back(2);
    types.push_back(3);
    uc = NotifyIndexNAdapter::instance().GetUnreadCountByBigtypes(_login_uid, types);
  }catch(Ice::Exception& e){
    MCE_WARN("GetUndealCountRequest::response-->GetUndealCountByBigtype err uid=" << _login_uid << "e=" << e);
  }
  MyUtil::Int2IntMap::iterator it = uc.find(1);
  if(it != uc.end()){
    int cc = it->second;
    res << "{remind:" << cc;
  }
  it = uc.find(2);
  if(it != uc.end()){
    int cc = it->second;
    res << ",apply:" << cc;
  }
  it = uc.find(3);
  if(it != uc.end()){
    int cc = it->second;
    res << ",notice:" << cc << "}";
  }
  FCGX_FPrintF(_fcgi_out, res.str().c_str());
  return true;
}

bool RemoveRMessageRequest::response(){
  // remove.notify?uid=1234&nid=12&source=12-123
  if (!_fcgi_out) {
    MCE_WARN("remove notify fcgi_out null");
    return false;
  }
  if (!Request::response()) {
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\nverify failure");
    return false;
  }
  MCE_DEBUG("ticket verify ok");
  Ice::Long nid = getPropertyLong("nid");
  Ice::Int removetype = getPropertyInt("type");
  Ice::Long removesource = getPropertyLong("source");
  Ice::Int type = -1;
  Ice::Long source = -1;
  int bigtype = 1;
  vector<Long> nids;
  string url_nl = getProperty("nl");
  string url_source = getProperty("source");

  try {
    boost::tokenizer<> tok(url_nl);
    for (boost::tokenizer<>::iterator tit = tok.begin(); tit != tok.end(); ++tit) {
      nids.push_back(lexical_cast<Long> (*tit));
    }
  } catch (...) {
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html; charset=utf-8\r\n\r\n-1");
    MCE_WARN("seperate notify id list fail");
    return false;
  }

  size_t pos = url_source.find_first_of(SOURCE_SEPERATOR);
  if (pos != string::npos) {
    try {
      type = boost::lexical_cast<Ice::Int>(url_source.substr(0, pos));
      source = boost::lexical_cast<Ice::Long>(url_source.substr(pos + 1));
    } catch (...) {
      FCGX_FPrintF(_fcgi_out, "Content-type: text/html; charset=utf-8\r\n\r\n-1");
      return false;
    }
  }
  try {
    MCE_INFO("RemoveRMessageRequest::response:type[" << removetype << "]"
        << " source[" << removesource << "] nid[" << nid << "]" 
        << "uid[" << _login_uid << "]nl[" << url_nl <<"]");
    if (removesource >= 0) {
      MCE_DEBUG("call removeBySource");
      NotifyIndexAdapter::instance().removeBySource(_login_uid, removetype, removesource);
      NotifyIndexNAdapter::instance().RemoveRMessageBySource(_login_uid, removetype, removesource, bigtype);
    } else if ( !nids.empty() ) {
      MCE_DEBUG("call removeByIds!!!");
      NotifyIndexAdapter::instance().removeByIds(_login_uid, nids);
      NotifyIndexNAdapter::instance().RemoveRMessageByIds(_login_uid, nids, removetype, bigtype);
    } else if (nid >= 0) {
      MCE_DEBUG("call removeById");
      NotifyIndexAdapter::instance().removeById(_login_uid, nid);
      NotifyIndexNAdapter::instance().RemoveRMessageById(_login_uid, nid, removetype, bigtype);
    }
  } catch (Ice::Exception& e) {
    MCE_DEBUG("NotifyIndexAdapter remove exception:" << e << " uid=" << _login_uid);
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\n-2");
    return false;
  }
  FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\n0");
  return true;
}

bool ProcessRemindMsgRequest::response(){
  //rmessage/process?nl=31124738698-31124738902&uid=433553862&type=392
  if (!_fcgi_out) {
    MCE_WARN("ProcessRemindMsgRequest::response --> fcgi_out null");
    return false;
  }
  if (!Request::response()) {
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\nverify failure");
    return false;
  }
  Ice::Long nid = getPropertyLong("nid");
  Ice::Int type = getPropertyInt("type");
  Ice::Long source = getPropertyLong("source");
  int bigtype = 1;
  vector<Long> nids;
  string url_nl = getProperty("nl");
  try {
    boost::tokenizer<> tok(url_nl);
    for (boost::tokenizer<>::iterator tit = tok.begin(); tit != tok.end(); ++tit) {
      nids.push_back(lexical_cast<Long> (*tit));
    }
  } catch (...) {
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html; charset=utf-8\r\n\r\n-1");
    MCE_WARN("ProcessRemindMsgRequest::response --> seperate nid list fail");
    return false;
  }
  try {
    MCE_INFO("ProcessRemindMsgRequest::response --> uid:" << _login_uid 
        << " type:" << type << " source:" << source << " nid:" << nid 
        << " nidlist:" << url_nl);
    if (!nids.empty()) {
      MCE_DEBUG("ProcessRemindMsgRequest::response --> ProcessMessageByIds uid:"
          << _login_uid);
      NotifyIndexNAdapter::instance().ProcessMessageByIds(_login_uid, nids,
          type, bigtype);
    } else if (nid >= 0) {
      MCE_DEBUG("ProcessRemindMsgRequest::response --> ProcessMessageById uid:"
          << _login_uid);
      NotifyIndexNAdapter::instance().ProcessMessageById(_login_uid, nid, 
          type, bigtype);
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("ProcessRemindMsgRequest::response --> process " 
      << " uid:" << _login_uid << " exception:" << e );
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\n-2");
    return false;
  }
  FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\n0");
  return true;
}

void NotifyGateI::RemoveLogicSource(int uid, int type, Ice::Long source, const Ice::Current&){
	TimeStat ts;
  try{
    NotifyIndexAdapter::instance().removeBySource(uid, type, source);
  }catch(Ice::Exception& e){
    MCE_WARN("NotifyGateI::RemoveLogicSource uid:" << uid 
        << " type:" << type << " source:" << source << " e:" << e);
  }
  try {
    NotifyIndexNAdapter::instance().RemoveRMessageBySource(uid, type, source, 1);
  } catch(Ice::Exception& e) {
    MCE_WARN("NotifyGateI::RemoveLogicSource uid:" << uid << " type:" 
        << type << " source:" << source << " e:" << e);
  }
  MCE_INFO("NotifyGateI::RemoveLogicSource uid:" << uid << " type:" 
      << type << " source:" << source );
	FunStatManager::instance().Stat("NotifyGateI::RemoveLogicSource", ts.getTime(), false);
}

void NotifyGateI::RemoveLogicBySourceSeq(int uid, int type, const MyUtil::LongSeq& sources, const Ice::Current& ){
	TimeStat ts;
  MCE_DEBUG("NotifyGateI::RemoveLogicSource--> uid:" << uid << " type:" << type << " sources.size: " << sources.size());
  for(MyUtil::LongSeq::const_iterator it = sources.begin(); it != sources.end(); ++it){
    Ice::Long source = *it;
    try{
      NotifyIndexAdapter::instance().removeBySource(uid, type, source);
    }catch(Ice::Exception& e){
      MCE_WARN("NotifyGateI::RemoveLogicSource--> uid:" << uid << " type:" 
          << type << " source:" << source << "exception:" << e);
    }
  }
  try{
    NotifyIndexNAdapter::instance().RemoveRMessageBySourceSeq(uid, type, sources, 1);
  }catch(Ice::Exception& e){
    MCE_WARN("NotifyGateI::RemoveLogicSource uid:" << uid << " type:" 
        << type << " sources.size[" << sources.size() << "]exp:" << e);
  }
	FunStatManager::instance().Stat("NotifyGateI::RemoveLogicBySourceSeq", ts.getTime(), false);
}

int NotifyGateI::getUndealCountByBigType(int uid, int bigType, const Ice::Current& ){
	TimeStat ts;
	int undealCount = 0;
	try{
		undealCount = NotifyIndexNAdapter::instance().GetUndealCountByBigtype(uid,bigType);
	}catch(Ice::Exception& e){
		MCE_WARN("NotifyGateI::getUndealCountByBigType--> uid:" << uid << " bigType:"
				<< bigType << "exception:" << e);
	}
	FunStatManager::instance().Stat("NotifyGateI::getUndealCountByBigType", ts.getTime(), false);
	return undealCount; 
}

MyUtil::StrSeq NotifyGateI::getRMsgByBigtype(int uid, int view, 
    int bigtype, int begin, int limit, const Ice::Current&) {
	TimeStat ts;
  MyUtil::StrSeq ret_str_vct;
  if (bigtype <= 0) {
    return ret_str_vct;
  }
  NotifyBodySeq notify_bodies;
  TimeStat time_stat; 
  try {
    notify_bodies = NotifyIndexNAdapter::instance().GetRMessageByBigtype(uid, 
        begin, limit, bigtype);
  } catch (Ice::Exception& e) {
      float cost_time = time_stat.getTime();
      MCE_WARN("NotifyGateI::GetRMsgByBigtype " << " uid:" << uid << " view:" 
          << view << " begin:" << begin << " limit:" << limit << " bigtype:" 
          << bigtype << " cost:" << cost_time << " excpetion:" << e);
      return ret_str_vct;
  }
  vector<Ice::Long> fail_ids;
  BodySeq2StrSeq(notify_bodies, view, ret_str_vct, fail_ids);
  float cost_time = time_stat.getTime();
  MCE_INFO("NotifyGate::getRMsgByBigtype notify_bodies:" << notify_bodies.size() 
      << " ret_str_vct size:" << ret_str_vct.size() 
      << " fail ids:" << fail_ids.size() << " uid:" << uid 
      << " view:" << view << " begin:" << begin << " limit:" << limit 
      << " bigtype:" << bigtype << " cost:" << cost_time);
	FunStatManager::instance().Stat("NotifyGateI::getRMsgByBigtype", ts.getTime(), false);
  return ret_str_vct;
}

MyUtil::StrSeq NotifyGateI::getRMsgByType(int uid, int view, 
    int type, int begin, int limit, const Ice::Current&) {
	TimeStat ts;
  MyUtil::StrSeq ret_str_vct;
  NotifyBodySeq notify_bodies;
  TimeStat time_stat; 
  map<int64_t, int> fail_id_map;
  int bigtype = 0;
  try {
    bigtype = SchemaManager::instance(). GetBigtypeBytype(type);
    if (bigtype <= 0) {
      return ret_str_vct;
    }
    MyUtil::IntSeq types_vct;
    types_vct.push_back(type);
    notify_bodies = NotifyIndexNAdapter::instance().GetRMessageByType(uid,
        types_vct, begin, limit, bigtype);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyGateI::getRMsgByType uid:" << uid 
        << " view:" << view << " type:" << type << " begin:" << begin 
        << " limit:" << limit << " btype:" << bigtype 
        << " exception:" << e);
  }
  BodySeq2StrSeqXRmessage(notify_bodies, view, ret_str_vct, fail_id_map, false);
  MCE_INFO("NotifyGateI::getRMsgByType uid:" << uid 
      << " view:" << view << " type:" << type << " begin:" << begin << " limit:" 
      << limit << " btype:" << bigtype << " rets:" << ret_str_vct.size() 
      << " cost:" << time_stat.getTime());
	FunStatManager::instance().Stat("NotifyGateI::getRMsgByType", ts.getTime(), false);
  return ret_str_vct;
}
//===================RMESSAGE===================================
NotifyBodySeq NotifyGateI::GetRemindMsg2Page(int64_t uid, int begin, int limit){
  Statement sql;
  TimeStat time_cost;
  NotifyIndexSeq index_seq;
  sql << "select id, type, source, from_id, is_reply, bigtype, unread from " 
      << GetIndexTableName(uid) 
      << " force index(idx_toid_bigtype_id) where to_id = " << uid 
      << " and bigtype = 1"    
      << " order by id desc limit " <<  begin << "," << limit; 
  mysqlpp::StoreQueryResult res = QueryRunner("notify", CDbRServer, 
      GetIndexTableName(uid)).store(sql);
  int query_count = res.num_rows();
  if (res && query_count > 0) {
    for (size_t i = 0; i < res.num_rows(); ++i) {
      NotifyIndex index;
      mysqlpp::Row row = res.at(i);
      index.id = (Ice::Long) row["id"];
      int bigtype = (int)row["bigtype"];
      int type = (Ice::Int) row["type"];
      type |= (bigtype << 16);
      index.type = type;
      index.source = (Ice::Long) row["source"];
      index.from = (Ice::Int) row["from_id"];
      index.isreply = (Ice::Int) row["is_reply"];
      index.unread = row["unread"];
      index_seq.push_back(index);
    }
  }
  NotifyBodySeq body_seq = GetRemindContentByIndex(uid, index_seq);
  MCE_INFO("NotifyGateI::GetRemindMsg2Page --> uid:" << uid << " begin:" 
      << begin << " limit:" << limit << " indexs:" << index_seq.size()
      << " bodys:" << body_seq.size() << " cost:" << time_cost.getTime());
  return body_seq;
}

NotifyBodySeq NotifyGateI::GetRemindContentByIndex(int64_t uid,
    const NotifyIndexSeq& index_seq) {
  TimeStat time_cost;
  MyUtil::LongSeq nid_seq;
  NotifyBodySeq body_seq;
  NotifyContentSeq content_seq;
  NotifyContentDict contentDict;
  for(NotifyIndexSeq::const_iterator it = index_seq.begin(); 
      it != index_seq.end(); ++it){
    NotifyIndex index = *it;
    nid_seq.push_back(index.id);
  }
  try {
    //get content from  memcache 
    contentDict = NotifyContentLoaderAdapter::instance().GetNotifyContentDict(nid_seq);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyGateI::GetRemindContentByIndex -->" 
        << " GetNotifyContentDict ids:" << PrintHelper::Print(nid_seq) 
        << " exception:" << e);
  }
  NotifyContentDict::iterator content_it;
  for(NotifyIndexSeq::const_iterator it = index_seq.begin();
      it != index_seq.end(); ++it){
    Ice::Long id = it->id;
    content_it = contentDict.find(id);
    if (content_it != contentDict.end()) {
      if(!content_it->second){
        continue;
      }
      content_it->second->unread = it->unread;
      content_seq.push_back(content_it->second);
    }
  }
  for(NotifyContentSeq::iterator it = content_seq.begin(); 
      it != content_seq.end(); ++it) {
    NotifyContentPtr content = *it;
    NotifyBodyPtr body = new NotifyBody(*it);
    body_seq.push_back(body);

    ostringstream os;
    for(map<string, string>::iterator it = body->props.begin(); 
        it != body->props.end(); ++it){
      os << " key:" << it->first << " value:" << it->second << ", ";
    }
    MCE_DEBUG("NotifyGateI::GetRemindContentByIndex --> uid:" << uid << " id:" 
        << body->id << " source:" << body->source << " owner:" << body->owner 
        << " from:" << body->from << " type:" << body->type << " time:" 
        << body->time << " unread:" << body->unread << " props:" << os.str());
  }
  MCE_DEBUG("NotifyGateI::GetRemindContentByIndex --> uid:" << uid   
      << " cost:" << time_cost.getTime());
  return body_seq;
}

