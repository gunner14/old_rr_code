#include "RmessageGateI.h"

#include <cstring>
#include <boost/algorithm/string/replace.hpp>
#include "pugixml.hpp"
#include "Notify/util/headurl.h"
#include "Notify/util/json_escape.h"
#include "Notify/util/schema_manager.h"
#include "Notify/util/rmessage_util.h"
#include "Notify/util/time_stat.h"
#include "Notify/util/url_encode.h"
#include "Notify/util/clear_silver_wrap.h"
#include "Notify/util/XceLoggerWrap.h"
#include "NotifyContentAdapter.h"
#include "NotifyIndexNAdapter.h"
#include "NotifyIndexAdapter.h"
#include "QueryRunner.h"
#include "client/linux/handler/exception_handler.h"

using namespace std;
using namespace MyUtil;
using namespace xce::feed;
using namespace xce::notify;

static const int kRemindMsgNumOfOnePage = 20;

bool RMsgGateCrashCallback(const char *dump_path, const char *minidump_id, 
                             void *context, bool succeeded) {
  MCE_FATAL("RmessagGate crash " << dump_path <<  minidump_id);
  return true;
}

static string GetIndexTableName(int64_t uid) {
  ostringstream os;
  os << "index_" << uid % 100;
  return os.str();
}

static void RemoveIndex(int uid, vector<Ice::Long> fail_ids) {
  try {
    NotifyIndexAdapter::instance().removeByIds(uid, fail_ids);
    //NotifyIndexNAdapter::instance().RemoveRMessageByIds(uid, fail_ids);
  } catch (Ice::Exception& e) {
    MCE_WARN("RemoveIndex exception " << " uid:" << uid << " fail_ids size:" 
        << fail_ids.size() << e);
    return;
  }
  MCE_INFO("RemoveIndex uid:" << uid << " fail_ids size:" << fail_ids.size());
}
/*
static void RemoveIndex(vector<int> uids, vector<Ice::Long> fail_ids) {
  for (unsigned i = 0; i != uids.size(); ++i) {
    RemoveIndex(uids.at(i), fail_ids);
  }
}
*/
static void BodySeq2StrSeqX(const NotifyBodySeq& n, Ice::Int view, StrSeq& s, 
    vector<Ice::Long> &fail_ids, bool is_kaixin) {
  if (n.empty()) {
    return;
  }
  typedef map<pair<Ice::Int, Ice::Long> , NotifyBodySeq> MergeMap;
  typedef map<Ice::Int, Ice::Int> ExposeMap;
  MergeMap mm;
  ExposeMap em;
  map<int64_t, vector<string> > nid_strings_map;
  size_t cnt = 0;
  MCE_DEBUG("BodySeq2StrSeqX size:" << n.size());
  for (size_t i = 0; i < n.size(); ++i) {
    Ice::Int type = RMessageUtil::instance().Util_GetRMessageType(n.at(i)->type);
		if(em.find(type) == em.end()){
			em[type] = 1;
		}
		else{
			em[type] += 1;
		}
    if(type == 193){
      fail_ids.push_back(n.at(i)->id);
      MCE_DEBUG("BodySeq2StrSeqX --> kill type:" << type <<  " nid:" << n.at(i)->id);
    }
    Ice::Long source = n.at(i)->source;
    if (!SchemaManager::instance().MustMerge(type, view)) {
      string txt = SchemaManager::instance().toReplyTxtForApply(n.at(i), view, is_kaixin, fail_ids);
      MCE_DEBUG("BodySeq2StrSeqX nid:" << n.at(i)->id <<  " type:" 
          << n.at(i)->type << " txt:" << txt);
      if (!txt.empty()) {
        nid_strings_map[n.at(i)->id].push_back(txt);
      }
      ++cnt;
    } else {
      mm[make_pair(type, source)].push_back(n.at(i));
    }
  }
  if (!mm.empty()) {
    for (MergeMap::iterator i = mm.begin(); i != mm.end(); ++i) {
      string txt = SchemaManager::instance().toReplyTxtMerge(i->second, view, is_kaixin, fail_ids);
      MCE_DEBUG("RmessageGateI::BodySeq2StrSeqX merge type:" 
          << i->second.at(0)->type << " merge_map_size:" 
          << i->second.size() << " txt:" << txt);
      if (!txt.empty()) {
        int64_t max_nid = 0;
        for (size_t k = 0; k < i->second.size(); k++) {
          max_nid = i->second.at(k)->id > max_nid ? i->second.at(k)->id : max_nid;
          MCE_DEBUG(" type:" << i->second.at(k)->type << " nid:" 
              <<  i->second.at(k)->id << " max_nid:" << max_nid);
        }
        nid_strings_map[max_nid].push_back(txt);
      }
    }
  }

  for (map<int64_t, vector<string> >::reverse_iterator i = nid_strings_map.rbegin(); 
      i != nid_strings_map.rend(); ++i) {
    s.insert(s.end(), i->second.begin(), i->second.end());
  }
	ostringstream expose_log;
	expose_log << "RmessageGateI::BodySeq2StrSeq:Expose:";
	for (map<int,int>::iterator i = em.begin(); i != em.end(); ++i){
		expose_log << "type=" << i->first << ":" << i->second << ",";		
	}
	MCE_INFO(expose_log.str());
	//XceLogSender::instance().SendLog(expose_log.str());
  return;
}

bool RmessageGateI::reloadSchema(const Ice::Current&) {
  MCE_INFO("RmessageGateI::reloadSchema");
  return SchemaManager::instance().reload();
}

ctemplate::Template * MessageCenterRequest::template_ = NULL;
ctemplate::Template * MessageMoreRequest::template_ = NULL;
ctemplate::Template * MessageTestCenterRequest::template_ = NULL;
ctemplate::Template * MessageCenterApply::template_ = NULL;
ctemplate::Template * MessageCenterAdd::template_ = NULL;

void MyUtil::initialize() {
  static google_breakpad::ExceptionHandler eh(".", 0, RMsgGateCrashCallback, 0, true);
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&RmessageGateI::instance(), service.createIdentity("M", ""));
  MessageCenterRequest::GetTemplate();

  Ice::PropertiesPtr props = service.getCommunicator()->getProperties();
  string fcgi_socket = props->getPropertyWithDefault("Service."
      + service.getName() + ".FcgiSocket", "0.0.0.0:9001");
  MCE_INFO(service.getName() << " Fcgi listens on : " << fcgi_socket);
  FcgiServer * fcgi_server = new FcgiServer(fcgi_socket, 64);
	fcgi_server->RegisterRequestFactory(new FeedRequestFactory());
	fcgi_server->Start();
	std::vector<string> category;
	category.push_back("notify");
	category.push_back("expose");
	XceLogSender::instance().Init(category);

}

bool MessageMoreRequest::response() {
  if (!_fcgi_out) {
    MCE_WARN("fcgi_out null");
    return false;
  }
  if (!Request::response()) {
    stringstream ss;
    ss << "Location: " << "http://www.renren.com/SysHome.do?origURL="
      << UrlEncode("http://notify.renren.com/rmessage/rmessage-center.html")
      << "\r\n\r\n";
    FCGX_PutS(ss.str().c_str(), _fcgi_out);
    return false;
  }

  Ice::Int type = getPropertyLong("type");
  Ice::Int limit = getPropertyInt("page");
  Ice::Int bigType = getPropertyInt("bigtype");
  int begin = (limit-1) * 10; 
  string emptyRet = "Content-type: text/html\r\n\r\n{}";
  FCGX_PutS("Content-type: text/html; charset=utf-8\r\n\r\n", _fcgi_out);
  ctemplate::TemplateDictionary dict("msg-more");
  ctemplate::Template * tpl = MessageMoreRequest::GetTemplate();
  map<int, ctemplate::TemplateDictionary *> li_maps;
  vector<int> types;
  types.push_back(type);
  NotifyBodySeq rmessages;
  try {
    rmessages = NotifyIndexNAdapter::instance().GetRMessageByType(_login_uid, 
        types, begin, 10, bigType);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyIndexNAdapter::GetRMessageByType uid:" << _login_uid 
        << " bigtype:" << bigType << " begin:" << begin << " limit:" 
        << limit << " type:" << type << " exception:" << e);
  }
  stringstream li_file, li_dict;
  li_file << "../etc/rmessage-center/" << type << "-li.tpl";
  li_dict << "LI_" << type;
  //for(size_t i = 0; i < rmessages.size(); ++i) {
  for(int i = rmessages.size() - 1; i >= 0; --i) {
    bool JudgeApply = false;
    bool JudgeFriend = false;
    int rtype = rmessages[i]->type & 0x7FFFFFFF;
    if (rtype != type) {
      continue;
    }
    ctemplate::TemplateDictionary * li_dict_x = dict.AddIncludeDictionary(li_dict.str());
    li_dict_x->SetFilename(li_file.str());
    ctemplate::TemplateDictionary * sec_dict = li_dict_x->AddSectionDictionary("LI_SEC");
    for(map<string, string>::const_iterator it = rmessages[i]->props.begin(); it != rmessages[i]->props.end(); ++it) {
      if((rtype == 258) &&  (!strcmp(it->first.c_str(),"targetId")) && (it->second == "0")){
        JudgeApply = true;
        li_dict_x->ShowSection("NO_LINK");
      }
      if((rtype == 256) &&  (!strcmp(it->first.c_str(),"net")) && (it->second == "")){
        sec_dict->ShowSection("LINK");
        sec_dict->ShowSection("test");
        JudgeFriend = true;
      }
      if((rtype == 256) &&  (!strcmp(it->first.c_str(),"net")) && (it->second != "")){
        sec_dict->ShowSection("NO_LINK");
        JudgeFriend = true;
      }
      MCE_DEBUG("x == " << it->first.c_str() << "y == " << it->second.c_str());
      sec_dict->SetValue(it->first, it->second);
      li_dict_x->SetValue(it->first, it->second);
    }
    if((rtype == 258) && !JudgeApply){
      li_dict_x->ShowSection("LINK");
    }
  }
  string res;
  tpl->Expand(&res, &dict);
  FCGX_PutS(res.c_str(), _fcgi_out);
  MCE_INFO("NotifyIndexNAdapter::GetRMessageByType --> uid:" << _login_uid 
      << " bigtype:" << bigType << " begin:" << begin << " limit:" 
      << limit << " type:" << type << " msgsize:" << rmessages.size());
  return true;
}

bool MessageCenterAdd::response() {
  TimeStatII ts_spam;
  if (!_fcgi_out) {
    MCE_WARN("fcgi_out null");
    return false;
  }
  if (!Request::response()) {
    stringstream ss;
    ss << "Location: " << "http://www.renren.com/SysHome.do?origURL="
      << UrlEncode("http://notify.renren.com/rmessage/rmessage-center.html")
      << "\r\n\r\n";
    FCGX_PutS(ss.str().c_str(), _fcgi_out);
    return false;
  }
  int begin;
  Ice::Int bigType = getPropertyInt("bigtype");
  Ice::Int remind = getPropertyInt("remind");
  Ice::Int view = getPropertyInt("view");

  if(bigType == 1 || bigType == 3) {
    begin = 1;
    remind = 8;
  } else if(bigType == 2) {
    begin = 1;
    remind = 5;
  } else {
    return false;
  }
  string emptyRet = "Content-type: text/html\r\n\r\n{}";
  FCGX_PutS("Content-type: text/html; charset=utf-8\r\n\r\n", _fcgi_out);
  ctemplate::TemplateDictionary dict("msg-more");
  ctemplate::Template * tpl = MessageCenterApply::GetTemplate();
  map<int, ctemplate::TemplateDictionary *> li_maps;
  vector<int> types;
  NotifyBodySeq rmessages;
  try {
    rmessages = NotifyIndexNAdapter::instance().GetRMessageByBigtype(_login_uid, begin, remind, bigType);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyIndexNAdapter::GetRMessageByBigtype --> uid:" << _login_uid
        << " bigtype:" << bigType << " begin:" << begin << " remind:" << remind 
        << " view:" << view << " Exception:" << e);
  }
 
  int index = rmessages.size();
  Ice::Int untreat = 0;
  try {
    untreat = NotifyIndexNAdapter::instance().GetUndealCountByBigtype(_login_uid, bigType);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyIndexNAdapter::GetUndealCountByBigtype --> uid:" 
        << _login_uid << " bigtype:" << bigType << " begin:" << begin 
        << " remind:" << remind << " view:" << view << " Exception:" << e);
  }
  StrSeq ret;
  StrSeq last_ret;
  vector<Ice::Long> fail_ids;
  stringstream ss,uncount;
  bool judge_add = false;
  uncount << untreat;
  int json_count = 0;
  bool is_kaixin = (host().find("kaixin") != string::npos);
  BodySeq2StrSeqX(rmessages,view, ret, fail_ids, is_kaixin);
  index = ret.size();
  ostringstream log_info;
  for (int i = 0; i < index; i++) {
    log_info << "num[" <<  i << "][" << ret[i] << "] ";
  }
  MCE_DEBUG("RmessageGate::MessageCenterAdd:: --> size:" << index << " msg:" 
      << log_info.str());
  if((bigType == 1 || bigType == 3)){
    if(index == 9)
      last_ret.push_back(ret[index - 3]);
    if(index == 8)
      last_ret.push_back(ret[index - 2]);
    if(index == 7)
      last_ret.push_back(ret[index - 1]);
    //last_ret.push_back(ret[0]);
  } else if (index == 5 && bigType == 2){
    //last_ret.push_back(ret[0]);
    last_ret.push_back(ret[index - 1]);
  } else {
    return false;
  }

  stringstream ss2;
  for (size_t h = 0; h < last_ret.size(); ++h) {
    string txt = last_ret.at(h);
    if (++json_count > 1)
      ss << ',';
    ss << txt;
    judge_add = true;
  }
  for (size_t h = 0; h < ret.size(); ++h) {
    string txt = ret.at(h);
    if (++json_count > 1)
      ss2 << ',';
    ss2 << txt;
    judge_add = true;
  }
  MCE_DEBUG("res.size[" << last_ret.size() << "]ss=" << ss.str());

  if (!fail_ids.empty()) {
    RemoveIndex(_login_uid, fail_ids);
  }
  stringstream li_file, li_dict;
  li_file << "../etc/rmessage-center/add.tpl";
  li_dict << "LI_ADD";
  ctemplate::TemplateDictionary * li_dict_x = dict.AddIncludeDictionary(li_dict.str());
  li_dict_x->SetFilename(li_file.str());
  if(judge_add){
    li_dict_x->SetValue("LI_SEC", ss.str());
  }
  else{
    li_dict_x->SetValue("LI_SEC", "{}");
  }
  li_dict_x->SetValue("UNTREAT", uncount.str());
  string res;
  tpl->Expand(&res, &dict);
  FCGX_PutS(res.c_str(), _fcgi_out);
  MCE_INFO("RmessageGate::MessageCenterAdd:: --> cost:" << ts_spam.getTime() 
      << " uid:"<< _login_uid << " begin:" << begin << " remind:" << remind 
      << " bigType:" << bigType << " index:" << index << " untreat:" << untreat);
  return true;
}

bool MessageCenterApply::response() {
  TimeStatII ts_spam;
  if (!_fcgi_out) {
    MCE_WARN("fcgi_out null");
    return false;
  }
  if (!Request::response()) {
    stringstream ss;
    ss << "Location: " << "http://www.renren.com/SysHome.do?origURL="
      << UrlEncode("http://notify.renren.com/rmessage/rmessage-center.html")
      << "\r\n\r\n";
    FCGX_PutS(ss.str().c_str(), _fcgi_out);
    return false;
  }
  Ice::Int limit = getPropertyInt("page");
  Ice::Int bigType = getPropertyInt("bigtype");
  Ice::Int view = getPropertyInt("view");
  Ice::Int remind = getPropertyInt("remind");
  int begin;
  if(remind!=10&&(bigType==1)){
    remind = 8;
    begin = (limit-1) * 8;
  } else if(remind!=10&&bigType==2){
    remind = 5;
    begin = (limit-1) * 5;
  } else if(remind!=10&&bigType==3){
    remind = 7;
    begin = (limit-1) * 7;
  } else {
    remind = 10;  
    begin = (limit-1) * 10;
  }
  try {
    if (bigType == 2) {
      NotifyIndexNAdapter::instance().RemoveRepeatedRMsgByBigtype(_login_uid, bigType);
      MCE_DEBUG("MessageCenterApply::response --> RemoveRepeatedRMsgByBigtype uid:" 
          << _login_uid << " bigtype:" << bigType);
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("MessageCenterApply::response --> RemoveRepeatedRMsgByBigtype uid:" << _login_uid 
        << " begin:" << begin << " remind:" << remind << " bigtype:" << bigType
        << " view:" << view << " exception:" << e);
  }
  string emptyRet = "Content-type: text/html\r\n\r\n{}";
  FCGX_PutS("Content-type: text/html; charset=utf-8\r\n\r\n", _fcgi_out);
  ctemplate::TemplateDictionary dict("msg-more");
  ctemplate::Template * tpl = MessageCenterApply::GetTemplate();
  map<int, ctemplate::TemplateDictionary *> li_maps;
  vector<int> types;
  NotifyBodySeq rmessages;
  try {
    rmessages = NotifyIndexNAdapter::instance().GetRMessageByBigtype(_login_uid, begin, remind, bigType);
  } catch (Ice::Exception& e) {
    MCE_WARN("Apply NotifyIndexNAdapter::GetRMessageByBigtype --> uid:" << _login_uid 
        << " begin:" << begin << " remind:" << remind << " bigtype:" << bigType
        << " view:" << view << " exception:" << e);
  } 
  Ice::Int untreat = 0;
  try {
    untreat = NotifyIndexNAdapter::instance().GetUndealCountByBigtype(_login_uid, bigType);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyIndexNAdapter::GetUndealCountByBigtype uid:" << _login_uid 
        << " begin:" << begin << " remind:" << remind << " bigtype:" << bigType
        << " untreat:" << untreat << " view:" << view << " Exception:" << e );
  }
  StrSeq ret;
  vector<Ice::Long> fail_ids;
  stringstream ss,uncount;
  uncount << untreat;
  ss << "[" ;
  //ss = "Content-type: text/html; charset=utf-8\r\n\r\n[";
  int json_count = 0;
  bool is_kaixin = (host().find("kaixin") != string::npos);
  BodySeq2StrSeqX(rmessages, view, ret, fail_ids, is_kaixin);
  for (size_t h = 0; h < ret.size(); ++h) {
    string txt = ret.at(h);
    if (++json_count > 1)
      ss << ',';
    ss << txt;
  }
  ss << ']';

  if (!fail_ids.empty()) {
    RemoveIndex(_login_uid, fail_ids);
  }
  stringstream li_file, li_dict;
  if(bigType == 1) {
    li_file << "../etc/rmessage-center/remind.tpl";
    li_dict << "LI_REMIND";
  } else if(bigType == 2) {
    li_file << "../etc/rmessage-center/apply.tpl";
    li_dict << "LI_APPLY";
  } else if(bigType == 3) {
    li_file << "../etc/rmessage-center/notice.tpl";
    li_dict << "LI_NOTICE";
  } else {
    return false;
  }
  ctemplate::TemplateDictionary * li_dict_x = dict.AddIncludeDictionary(li_dict.str());
  li_dict_x->SetFilename(li_file.str());
  li_dict_x->SetValue("LI_SEC", ss.str()); 
  li_dict_x->SetValue("UNTREAT", uncount.str()); 
  //ADD TOKEN
  string TOKEN_PREFIX = "843a3582";
  string cokie = cookie("t");
  string bindTicket = TOKEN_PREFIX + cokie;
  char buffer[20];
  int len = bindTicket.length();
  int off = 0;
  int h = 0;
  for (int i = 0; i < len; i++) {
    h = 31*h + bindTicket[off++];
  }
  sprintf(buffer, "%x", h);
  li_dict_x->SetValue("TOKEN", buffer);
  off = 0;
  h = 0;
  len = cokie.length();
  memset(buffer, 0,sizeof(buffer));
  for (int i = 0; i < len; i++) {
    h = 31*h + cokie[off++];
  }
  sprintf(buffer, "%d", h);
  li_dict_x->SetValue("OLD_TOKEN", buffer);
  stringstream user_id;
  user_id << _login_uid;
  li_dict_x->SetValue("USERID", user_id.str());

  string res;
  tpl->Expand(&res, &dict);
  FCGX_PutS(res.c_str(), _fcgi_out);
  MCE_INFO("RmessageGate::MessageCenterApply --> cost:" << ts_spam.getTime() 
      << " uid:" << _login_uid << " begin:" << begin << " remind:" << remind
      << " bigtype:" << bigType << " view:" << view << " untreat:" << untreat);
  return true;
}

bool MessageCenterRequest::response() {
  TimeStatII ts_spam;
  if (!_fcgi_out) {
    MCE_WARN("fcgi_out null");
    return false;
  }
  if (!Request::response()) {
    stringstream ss;
    ss << "Location: " << "http://www.renren.com/SysHome.do?origURL="
      << UrlEncode("http://notify.renren.com/rmessage/rmessage-center.html")
      << "\r\n\r\n";
    FCGX_PutS(ss.str().c_str(), _fcgi_out);
    return false;
  }
  string emptyRet = "Content-type: text/html\r\n\r\n{}";
  FCGX_PutS("Content-type: text/html; charset=utf-8\r\n\r\n", _fcgi_out);
  ctemplate::TemplateDictionary dict("msg-center");
  ctemplate::Template * tpl = MessageCenterRequest::GetTemplate();
  map<int, ctemplate::TemplateDictionary *> ul_dicts;
  string TOKEN_PREFIX = "843a3582";
  string cokie = cookie("t");
  string bindTicket = TOKEN_PREFIX + cokie;
  char buffer[20];
  int len = bindTicket.length();
  int off = 0;
  int h = 0;
  for (int i = 0; i < len; i++) {
    h = 31*h + bindTicket[off++];
  }
  sprintf(buffer, "%x", h);
  dict.SetValue("TOKEN", buffer);
  off = 0;
  h = 0;
  len = cokie.length();
  memset(buffer,'\0',sizeof(buffer));
  for (int i = 0; i < len; i++) {
    h = 31*h + cokie[off++];
  }
  sprintf(buffer, "%d", h);
  dict.SetValue("OLD_TOKEN", buffer);
  stringstream user_id;
  user_id << _login_uid;
  dict.SetValue("USERID", user_id.str());
  Ice::Int untreat_remind = 0;
  try {
    untreat_remind = NotifyIndexNAdapter::instance().GetUndealCountByBigtype(
        _login_uid, 1);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyIndexNAdapter::GetUndealCountByBigtype --> uid:" << _login_uid 
        << " untreat_remind:" << untreat_remind << " bigtype:1 exception:" << e);
  }

  Ice::Int untreat_hi = 0;
  try {
    untreat_hi = NotifyIndexNAdapter::instance().GetUndealCountByBigtype(_login_uid, 3);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyIndexNAdapter::GetUndealCountByBigtype uid[" << _login_uid 
        << "]untreat_remind[" << untreat_remind 
        << "]bigtype[3]Exception[" << e << "]");
  }

  stringstream count_remind;
  count_remind << untreat_remind;
  dict.SetValue("REMIND", count_remind.str());
  ctemplate::TemplateDictionary * ul_dict_x = dict.AddIncludeDictionary("UL_MESSAGE");
  ul_dict_x->SetFilename("../etc/rmessage-center/UL_MESSAGE.tpl");
  ul_dict_x->SetValue("REMIND_NUM",count_remind.str());
  if(untreat_remind != 0) {
    dict.ShowSection("FLAG_REMIND");
    ul_dict_x->ShowSection("HEAD");
  } else {
    dict.ShowSection("NOREMIND_DEAL");
    ctemplate::TemplateDictionary * ul_dict_x = dict.AddIncludeDictionary("NOREMIND_DEAL");
    ul_dict_x->SetFilename("../etc/rmessage-center/apply-empty.tpl");
  }
  if(untreat_hi == 0){
    ctemplate::TemplateDictionary * ul_dict_x = dict.AddIncludeDictionary("NOHI_DEAL");
    ul_dict_x->SetFilename("../etc/rmessage-center/apply-empty.tpl");
  }
  if(untreat_remind > 10){
    ul_dict_x->ShowSection("MORE");
  }
  stringstream nid;
  int types[] = {247,251,256,258,259,260,261,262,264,265,266,278,279,280,281,282,283};
  vector<Ice::Int> typeSeq;
  for(unsigned int i = 0 ; i < sizeof(types) / sizeof(int); ++i) {
    typeSeq.push_back(types[i]);
  }
  int rmessage_count = 0;
  NotifyBodySeqMap rmessages;
  try {
    rmessages = NotifyIndexNAdapter::instance().GetRMessageByTypeAll(_login_uid,typeSeq,0,10);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyIndexNAdapter::GetRMessageByTypeAll --> uid:" << _login_uid 
        << " msgsize:" << rmessages.size() << " exception:" << e);
  }
  if(rmessages.size() == 0){
    dict.ShowSection("NOAPPLY_DEAL");
    ctemplate::TemplateDictionary * ul_dict_x = dict.AddIncludeDictionary("NOAPPLY_DEAL");
    ul_dict_x->SetFilename("../etc/rmessage-center/apply-empty.tpl");
  }
  for(unsigned int i = 0 ; i < sizeof(types) / sizeof(int); ++i) {
    stringstream ul_dict,more_dict;
    ul_dict << "UL_" << types[i];
    MCE_DEBUG("ul_dict" << ul_dict.str());
    stringstream ul_file;
    ul_file << "../etc/rmessage-center/" << types[i] << ".tpl";
    ctemplate::TemplateDictionary * ul_dict_x = dict.AddIncludeDictionary(ul_dict.str());
    ul_dict_x->SetFilename(ul_file.str());
    ul_dicts[types[i]] = ul_dict_x;
  }
  Int2IntMap undeal_count;
  try {
    undeal_count = NotifyIndexNAdapter::instance().GetUndealCountByTypes(_login_uid,typeSeq);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyIndexNAdapter::GetUndealCountByTypes --> uid:" << _login_uid 
        << " exception:" << e);
  }
  for(map<int, int>::const_iterator iter = undeal_count.begin(); 
      iter != undeal_count.end(); ++iter) {
    Ice::Int untreat = iter->second;
    Ice::Int type_i = iter->first;
    if(!untreat){
      continue;
    }
    stringstream li_file, li_dict,type_count,FLAG;
    FLAG << "FLAG_" << type_i;
    type_count << type_i << "_count";
    stringstream count;
    count << untreat;
    li_file << "../etc/rmessage-center/" << type_i << "-li.tpl";
    li_dict << "LI_" << type_i;
    NotifyBodySeqMap::iterator it = rmessages.find(type_i);
    if(it != rmessages.end()){
      if(!it->second.size())
        continue;
      rmessage_count += 1;
      for(unsigned int j =0; j < it->second.size() ; j++){
        nid.str("");
        int type = it->second[j]->type & 0x7FFFFFFF;
        nid << it->second[j]->id;
        map<int, ctemplate::TemplateDictionary *>::iterator iter = ul_dicts.find(type);
        if (iter == ul_dicts.end()) {
          continue;
        }
        ctemplate::TemplateDictionary * sub_ui_dict = iter->second;
        ctemplate::TemplateDictionary * li_dict_x = sub_ui_dict->AddIncludeDictionary(li_dict.str());
        li_dict_x->SetFilename(li_file.str());
        for(map<string, string>::const_iterator iter = it->second[j]->props.begin(); 
            iter != it->second[j]->props.end(); ++iter) {
          string TypeNum = iter->second;
          if(TypeNum > "99"){
            li_dict_x->SetValue(iter->first, "99+");
          } else {
            li_dict_x->SetValue(iter->first, iter->second);
          }
        }
        li_dict_x->ShowSection("LI_SEC");
        if(untreat > 10){
          sub_ui_dict->ShowSection("MORE");
        }
        sub_ui_dict->ShowSection("HEAD");
        dict.ShowSection(FLAG.str());
        li_dict_x->SetValue("nid", nid.str());
        sub_ui_dict->SetValue(type_count.str(), count.str());
        dict.SetValue(type_count.str(), count.str());
      }
    }
  }
  if(rmessage_count == 0){
    ctemplate::TemplateDictionary * ul_dict_x = dict.AddIncludeDictionary("NOAPPLY_DEAL");
    ul_dict_x->SetFilename("../etc/rmessage-center/apply-empty.tpl");
  }
  string res;
  tpl->Expand(&res, &dict);
  FCGX_PutS(res.c_str(), _fcgi_out);
  MCE_INFO("RmessageGate::MessageCenterRequest:: --> cost:" 
      << ts_spam.getTime() << " uid:" << _login_uid);
  return true;
}

bool MessageTestCenterRequest::response() {
  if (!_fcgi_out) {
    MCE_WARN("fcgi_out null");
    return false;
  }
  if (!Request::response()) {
    stringstream ss;
    ss << "Location: " << "http://www.renren.com/SysHome.do?origURL="
      << UrlEncode("http://notify.renren.com/rmessage/rmessage-center.html")
      << "\r\n\r\n";
    FCGX_PutS(ss.str().c_str(), _fcgi_out);
    return false;
  }
  string emptyRet = "Content-type: text/html\r\n\r\n{}";
  FCGX_PutS("Content-type: text/html; charset=utf-8\r\n\r\n", _fcgi_out);
  ctemplate::TemplateDictionary dict("msg-center");
  ctemplate::Template * tpl = MessageCenterRequest::GetTemplate();

  map<int, TplIncludeInfo> sub_tpls;
  sub_tpls.insert(make_pair(256, TplIncludeInfo(2001, "REMIND_LEAVE_WORD", "../etc/rmessage-center/remind-leave-word.tpl")));

  sub_tpls.insert(make_pair(2002, TplIncludeInfo(2001, "APPLY_FRIEND_REQUEST", "../etc/rmessage-testcenter/apply-friend-request.tpl")));
  sub_tpls.insert(make_pair(2003, TplIncludeInfo(2001, "APPLY_FRIEND_RECOMMEND", "../etc/rmessage-testcenter/apply-friend-recommend.tpl")));
  sub_tpls.insert(make_pair(2004, TplIncludeInfo(2001, "APPLY_QUANREN", "../etc/rmessage-center/apply-quanren.tpl")));
  sub_tpls.insert(make_pair(2005, TplIncludeInfo(2001, "APPLY_ADDRESS_LIST", "../etc/rmessage-center/apply-address-list.tpl")));
  sub_tpls.insert(make_pair(2006, TplIncludeInfo(2001, "APPLY_DECORATION_GIFT", "../etc/rmessage-center/apply-decoration-gift.tpl")));
  sub_tpls.insert(make_pair(263, TplIncludeInfo(2001, "APPLY_GROUP_JOIN", "../etc/rmessage-center/apply-group-join.tpl")));
  sub_tpls.insert(make_pair(2008, TplIncludeInfo(2001, "APPLY_PAGE_RECOMMEND", "../etc/rmessage-center/apply-page-recommend.tpl")));

  sub_tpls.insert(make_pair(2009, TplIncludeInfo(2001, "NOTICE_GAMES", "../etc/rmessage-center/notice-games.tpl")));
  sub_tpls.insert(make_pair(2010, TplIncludeInfo(2001, "NOTICE_VIP", "../etc/rmessage-center/notice-vip.tpl")));
  sub_tpls.insert(make_pair(2011, TplIncludeInfo(2001, "NOTICE_HELLO", "../etc/rmessage-center/notice-hello.tpl")));
  map<int, ctemplate::TemplateDictionary *> sub_dicts;
  NotifyBodySeq rmessages = NotifyIndexNAdapter::instance().GetRMessageByBigtype(_login_uid, 0, 10, 1);

  //MyUtil::LongSeq rmessage_ids;
  //rmessage_ids.reserve(rmessages.size());
  //for(size_t i = 0; i < rmessages.size(); ++i) {
  //  rmessage_ids.push_back(rmessages[i].id);
  //}
  //NotifyContentDict rmessage_contents = NotifyContentAdapter::instance().getNotifyDict(rmessage_ids);

  for(map<int, TplIncludeInfo>::const_iterator it = sub_tpls.begin(); it != sub_tpls.end(); ++it) {
    MCE_DEBUG("add sub tpl : type = " << it->first << " file=" << it->second.tpl_file);
    ctemplate::TemplateDictionary * sub_dict = dict.AddIncludeDictionary(it->second.dict_name);
    sub_dict->SetFilename(it->second.tpl_file);

    if(it->first == 2002) { // 测试好友请求SECTION
      ctemplate::TemplateDictionary * sec_dict = sub_dict->AddSectionDictionary("LI_SEC");
      sec_dict->SetValue("test", "test-value");
      sec_dict->SetValue("test2", "test2-value");

      sec_dict = sub_dict->AddSectionDictionary("LI_SEC");
      sec_dict->SetValue("test", "test-value-2");
      sec_dict->SetValue("test2", "test2-value-2");
    }
    sub_dicts[it->first] = sub_dict;
  }

  for(size_t i = 0; i < rmessages.size(); ++i) {
    int type = rmessages[i]->type & 0x7FFFFFFF;
    MCE_DEBUG("type = " << rmessages[i]->type);
    map<int, ctemplate::TemplateDictionary *>::iterator it = sub_dicts.find(type);
    if (it == sub_dicts.end()) {
      continue;
    }
    ctemplate::TemplateDictionary * sub_dict = it->second;
    ctemplate::TemplateDictionary * sec_dict = sub_dict->AddSectionDictionary("LI_SEC");
    map<string, string>::iterator prop_it = rmessages[i]->props.begin();
    while(prop_it != rmessages[i]->props.end()) {
      sec_dict->SetValue(prop_it->first, prop_it->second);
      prop_it++;
    }
  }
  // leave_word_dict->SetFilename("/data/xce/Notify/etc/rmessage-center/remind-leave-word.tpl");
  string res;
  tpl->Expand(&res, &dict);
  FCGX_PutS(res.c_str(), _fcgi_out);
  return true;
}

int RmessageGateI::HasRemindMsgInDb(int uid) {
  Statement sql;
  TimeStat time_cost;
  sql << "select id from " << GetIndexTableName(uid) 
      << " force index(idx_toid_bigtype_id) where to_id = " << uid 
      << " and bigtype = 1 order by id desc limit " 
      << (kRemindMsgNumOfOnePage + 1); 
  int query_count = 0;
  try {
    mysqlpp::StoreQueryResult res = QueryRunner("notify", CDbRServer, 
      GetIndexTableName(uid)).store(sql);
    if (res) {
      query_count = res.num_rows();
    }
  } catch (mysqlpp::Exception& e) {
    MCE_WARN("RmessageGateI::HasRemindMsgInDb --> uid:" << uid 
        << " exception:" << e.what());
    return -1;
  }
  MCE_INFO("RmessageGateI::HasRemindMsgInDb --> uid:" << uid << " count:" 
      << query_count << " cost:" << time_cost.getTime());
  return query_count;
}

string RmessageGateI::GetHtmlContent(::Ice::Int uid ,const ::Ice::Current&) {
  TimeStatII ts_spam;
  ctemplate::TemplateDictionary dict("msg-center");
  string res;
  ctemplate::Template* tpl = MessageCenterRequest::GetTemplate();
  map<int, ctemplate::TemplateDictionary *> ul_dicts;
  stringstream user_id;
  user_id << uid;
  dict.SetValue("USERID", user_id.str());
  Ice::Int untreat_remind = 0;
  try {
    untreat_remind = NotifyIndexNAdapter::instance().GetUndealCountByBigtype(uid, 1);
  } catch (Ice::Exception& e) {
    MCE_WARN("RmessageGateI::GetHtmlContent GetUndealCountByBigtype --> uid:" 
        << uid << " bigtype:1 exception:" << e);
  }
  Ice::Int untreat_hi = 0;
  try {
    untreat_hi = NotifyIndexNAdapter::instance().GetUndealCountByBigtype(uid, 3);
  } catch (Ice::Exception& e) {
    MCE_WARN("RmessageGateI::GetHtmlContent GetUndealCountByBigtype --> uid:" 
        << uid << " bigtype:3 exception:" << e);
  }
  stringstream count_remind,count_notice;
  count_remind << untreat_remind;
  count_notice << untreat_hi;
  if(untreat_remind > 99){
    dict.SetValue("REMIND", "99+");
  } else{
    dict.SetValue("REMIND", count_remind.str());
  }
  if(untreat_hi > 99) {
    dict.SetValue("NOTICE_NUM", "99+");
  } else {
    dict.SetValue("NOTICE_NUM", count_notice.str());
  }
  ctemplate::TemplateDictionary * ul_dict_x = dict.AddIncludeDictionary("UL_MESSAGE");
  ul_dict_x->SetFilename("../etc/rmessage-center/UL_MESSAGE.tpl");
  ctemplate::TemplateDictionary * ul_dict_y = dict.AddIncludeDictionary("UL_NOTICE");
  ul_dict_y->SetFilename("../etc/rmessage-center/UL_NOTICE.tpl");
  ul_dict_x->SetValue("REMIND_NUM",count_remind.str());
  ul_dict_y->SetValue("NOTICE_NUM",count_notice.str());
  int has_remind_msg = HasRemindMsgInDb(uid);
  if(has_remind_msg > 0){
    dict.ShowSection("FLAG_REMIND");
    ul_dict_x->ShowSection("HEAD");
  } else {
    dict.ShowSection("NOREMIND_DEAL");
    ctemplate::TemplateDictionary * ul_dict_x = dict.AddIncludeDictionary("NOREMIND_DEAL");
    ul_dict_x->SetFilename("../etc/rmessage-center/apply-empty.tpl");
  }
  if(untreat_hi == 0){
    ctemplate::TemplateDictionary * ul_dict_x = dict.AddIncludeDictionary("NOHI_DEAL");
    ul_dict_x->SetFilename("../etc/rmessage-center/apply-empty.tpl");
  } else {
    dict.ShowSection("FLAG_NOTICE");
    ul_dict_y->ShowSection("HEAD");
  }
  if(has_remind_msg > kRemindMsgNumOfOnePage){
    ul_dict_x->ShowSection("MORE");
  }
  if(untreat_hi > 10){
    ul_dict_y->ShowSection("MORE");
  }

  stringstream nid;
  int types[] = {247,251,256,258,259,260,261,262,263,264,265,266,278,279,280,
      281,282,283,299,317, 325, 373, 378, 379, 380, 383};
  vector<Ice::Int> typeSeq;
  for(unsigned int i = 0 ; i < sizeof(types) / sizeof(int); ++i) {
    typeSeq.push_back(types[i]);
  }
  int rmessage_count = 0;
  NotifyBodySeqMap rmessages;
  try {
    rmessages = NotifyIndexNAdapter::instance().GetRMessageByTypeAll(uid,typeSeq,0,10);
  } catch (Ice::Exception& e) {
    MCE_WARN("RmessageGate::GetHtmlContent --> GetRMessageByTypeAll uid:" 
        << uid << " Exception:" << e);
  }
  if(rmessages.size() == 0){
    dict.ShowSection("NOAPPLY_DEAL");
    ctemplate::TemplateDictionary * ul_dict_x = dict.AddIncludeDictionary("NOAPPLY_DEAL");
    ul_dict_x->SetFilename("../etc/rmessage-center/apply-empty.tpl");
  }
  for(unsigned int i = 0 ; i < sizeof(types) / sizeof(int); ++i) {
    stringstream ul_dict,more_dict;
    ul_dict << "UL_" << types[i];
    MCE_DEBUG("ul_dict" << ul_dict.str());
    stringstream ul_file;
    ul_file << "../etc/rmessage-center/" << types[i] << ".tpl";
    ctemplate::TemplateDictionary * ul_dict_x = dict.AddIncludeDictionary(ul_dict.str());
    ul_dict_x->SetFilename(ul_file.str());
    ul_dicts[types[i]] = ul_dict_x;
  }
  Int2IntMap test;
  try {
    test = NotifyIndexNAdapter::instance().GetUndealCountByTypes(uid,typeSeq);
  } catch (Ice::Exception& e) {
    MCE_WARN("RmessageGate::GetHtmlContent GetUndealCountByTypes --> uid:" 
        << uid << " exception:" << e);
  }
  for(map<int, int>::const_iterator iter = test.begin(); iter != test.end(); ++iter){
    Ice::Int untreat = iter->second;
    Ice::Int type_i = iter->first;
    if(!untreat){
      continue;
    }
    stringstream li_file, li_dict,type_count,FLAG;
    FLAG << "FLAG_" << type_i;
    type_count << type_i << "_count";
    stringstream count;
    count << untreat;
    li_file << "../etc/rmessage-center/" << type_i << "-li.tpl";
    li_dict << "LI_" << type_i;
    NotifyBodySeqMap::iterator it = rmessages.find(type_i);
    if(it != rmessages.end()){
      if(!it->second.size())
        continue;
      rmessage_count += 1;
      //for (unsigned int j=0; j < it->second.size(); j++){
      for (int j = it->second.size() -1 ; j >= 0 ; --j){
        bool JudgeApply = false;
        bool JudgeFriend = false;
        nid.str("");
        int type = it->second[j]->type & 0x7FFFFFFF;
        nid << it->second[j]->id;
        map<int, ctemplate::TemplateDictionary *>::iterator iter = ul_dicts.find(type);
        if (iter == ul_dicts.end()) {
          continue;
        }
        ctemplate::TemplateDictionary * sub_ui_dict = iter->second;
        ctemplate::TemplateDictionary * li_dict_x = sub_ui_dict->AddIncludeDictionary(li_dict.str());
        li_dict_x->SetFilename(li_file.str());
        for(map<string, string>::const_iterator mapiter = it->second[j]->props.begin(); 
            mapiter != it->second[j]->props.end(); ++mapiter) {
          if((type_i == 258) &&  (!strcmp(mapiter->first.c_str(),"targetId")) && (mapiter->second == "0")){
            JudgeApply = true;
            li_dict_x->ShowSection("NO_LINK");
          }
          if((type_i == 256) &&  (!strcmp(mapiter->first.c_str(),"net")) && (mapiter->second == "")){
            li_dict_x->ShowSection("LINK");
            JudgeFriend = true;
          }
          li_dict_x->SetValue(mapiter->first, mapiter->second);
        }
        if((type_i == 258) && !JudgeApply){
          li_dict_x->ShowSection("LINK");
        }
        if((type_i == 256) && !JudgeFriend){
          li_dict_x->ShowSection("NO_LINK");
        }
        li_dict_x->ShowSection("LI_SEC");
        if(untreat > 10){
          sub_ui_dict->ShowSection("MORE");
        }
        sub_ui_dict->ShowSection("HEAD");
        dict.ShowSection(FLAG.str());
        li_dict_x->SetValue("nid", nid.str());
        int TypeNum = untreat;
        if(TypeNum > 99){
          sub_ui_dict->SetValue(type_count.str(), "99+");
          dict.SetValue(type_count.str(), "99+");
        } else{
          sub_ui_dict->SetValue(type_count.str(), count.str());
          dict.SetValue(type_count.str(), count.str());
        }
      }
    }
  }
  if(rmessage_count == 0){
    ctemplate::TemplateDictionary * ul_dict_x = dict.AddIncludeDictionary("NOAPPLY_DEAL");
    ul_dict_x->SetFilename("../etc/rmessage-center/apply-empty.tpl");
  }
  tpl->Expand(&res, &dict);
  MCE_INFO("RmessageGate::GetHtmlContent --> uid:" << uid << " cost:" 
      << ts_spam.getTime() << " remind:" << untreat_remind 
      << " notice:" << untreat_hi);
  return res;
}

bool RemoveAllMessageByBigtype::response(){
  // /rmessage/rmessage-rmall.html
  if (!_fcgi_out) {
    MCE_WARN("RemoveAllMessageByBigtype fcgi_out is null");
    return false;
  }
  if (!Request::response()) {
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\nverify failure");
    return false;
  }
  int view = getPropertyInt("view");
  int bigtype = getPropertyInt("bigtype");
  if (bigtype < 1 || bigtype > 3) {
    MCE_WARN("RemoveAllMessageByBigtype::response --> uid:" 
        << _login_uid << " bigtype:" << bigtype << " view:" << view);
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html \r\n\r\n bigtype error");
    return false;
  }
  TimeStatII cost_time;
  try {
    NotifyIndexNAdapter::instance().RemoveRMessageByBigtype(_login_uid, bigtype);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyIndexNAdapter RemoveRMessageByBigtyp --> uid:" << _login_uid 
        << " bigtype:" << bigtype <<  " exception:" << e);
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\n remove exception");
    return false;
  }
  MCE_INFO("RemoveRMessageRequest::response --> cost:" << cost_time.getTime()
      << " uid:" << _login_uid << " bigtype:" << bigtype);
  FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\n0");
  return true;
}

bool ReadAllMessageByBigtype::response(){
  // /rmessage/rmessage-rmall.html
  if (!_fcgi_out) {
    MCE_WARN("ReadAllMessageByBigtype --> fcgi_out is null");
    return false;
  }
  if (!Request::response()) {
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\nverify failure");
    return false;
  }
  int view = getPropertyInt("view");
  int bigtype = getPropertyInt("bigtype");
  if (bigtype != 1) {
    MCE_WARN("ReadAllMessageByBigtype::response --> bigtype error uid:" 
        << _login_uid << " bigtype:" << bigtype << " view:" << view);
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html \r\n\r\n bigtype error");
    return false;
  }
  TimeStatII cost_time;
  try {
    NotifyIndexNAdapter::instance().ReadAllMessageByBigtype(_login_uid, bigtype);
  } catch (Ice::Exception& e) {
    MCE_WARN("NotifyIndexNAdapter ReadAllMessageByBigtyp --> uid:" << _login_uid 
        << " bigtype:" << bigtype <<  " exception:" << e);
    FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\n read all exception");
    return false;
  }
  MCE_INFO("ReadAllMessageiByBigtype::response --> uid:" << _login_uid 
      << " bigtype:" << bigtype << " cost:" << cost_time.getTime());
  FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\n0");
  return true;
}

