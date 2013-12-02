/*
 * DispatchManagerI.cpp
 *
 *  Created on: Dec 9, 2009
 *      Author: antonio
 */

#include "DispatchManagerI.h"
#include "QueryRunner.h"
#include "XmppTools.h"
#include "Notify/util/schema_manager.h"
#include "NotifyDispatcherNAdapter.h"
#include "UserPassportAdapter.h"
#include "Notify/util/time_stat.h"
#include "Notify/util/XceLoggerWrap.h"
#include "client/linux/handler/exception_handler.h"


using namespace xce::notify;

using namespace com::xiaonei::xce;
using namespace xce::adapter::userpassport;
using namespace com::xiaonei::talk::util;
using namespace MyUtil;

bool notifyDispatcherCrashCallback(const char *dump_path, 
    const char *minidump_id, void *context, bool succeeded) {
  printf("NotifyDispatcher crash %s %s\n", dump_path, minidump_id);
  return true;
}

void MyUtil::initialize() {
  static google_breakpad::ExceptionHandler eh(".", 0, 
      notifyDispatcherCrashCallback, 0, true);
  ServiceI& service = ServiceI::instance();

  service.getAdapter()->add(&DispatchManagerI::instance(),
      service.createIdentity("M", ""));
  std::vector<string> category;
  category.push_back("notify");
  category.push_back("dispatch");
  XceLogSender::instance().Init(category);
}

Ice::Long DispatchManagerI::GetServreIndex(Ice::Long nid, const MyUtil::Str2StrMap& props) {
  Ice::Long server_index = nid;
  MyUtil::Str2StrMap::const_iterator it = props.find("feed_source");
  if (it != props.end()) {
    try {
      server_index = boost::lexical_cast<Ice::Long>(it->second);
    } catch (boost::bad_lexical_cast & e) {}
  }
  return server_index;
}

void DispatchManagerI::deliverWebpager(const MyUtil::Str2StrMap& props,
    const Ice::Current&) {
  if (!SchemaManager::instance().checkDispatch(props)) {
    MCE_WARN("DispatchManagerI::deliverWebpager --> illegal input");
    return;
  }
  Ice::Long nid = createNotifyId();
  MCE_INFO("DispatchManagerI::deliverWebpager --> nid:" << nid << " to:"
      << props.find("to")->second);
  Ice::Long server_index = GetServreIndex(nid, props);
  try {
    NotifyDispatcherNAdapter::instance().dispatchN(server_index, nid, false, props);
  } catch(Ice::Exception& e) {
    MCE_WARN("DispatchManagerI::deliverWebpager --> nid:" << nid 
        << " exception:" << e);
  }
}

//string GetIp(const Ice::Current& current){
//  string ip = current.con->toString();
//  size_t found = ip.find('\n');
//  if (found != string::npos) {
//    ip[found] = ' ';
//  }
//  return ip;
//}
string GetIp(const Ice::Current& current){
  if(!current.con){
    MCE_WARN("GetIp --> Current con is false")
    return "NULL";
  }
  string ip = current.con->toString();
  size_t found = ip.find('\n');
  if (found == string::npos) {
    return ip;
  }
  string res ( ip.begin() + found + 1,ip.end());
  return res;
}

void DispatchManagerI::dispatch(const MyUtil::Str2StrMap& props,
    const Ice::Current& cur) {
  TimeStat ts;
  if (!SchemaManager::instance().checkDispatch(props)) {
    MCE_WARN("DispatchManagerI::dispatch --> illegal input");
    return;
  }
  if(props.find("type")->second == "213"){
    MCE_WARN("DispatchManagerI::dispatch --> type 213 can not send " << GetIp(cur));
    return ;
  }
  Ice::Int from_uid = 0;
  try {
    from_uid = boost::lexical_cast<Ice::Int>(props.find("from")->second);
  } catch (boost::bad_lexical_cast& e) {
    return;
  }
  UserPassportInfoPtr data; 
  try {
    data = UserPassportAdapter::instance().getUserPassport(from_uid);
  } catch(Ice::Exception& e) {
    MCE_WARN("DispatcherManagerI::dispatch --> getUserPassport from:"
        << from_uid  << " exception: " << e);
  }
  if (data && (data->safestatus() & 0x07C0)) {
    MCE_WARN("DispatchManagerI::dispatch --> user blocked uid:" << from_uid);
    return;
  }

  Ice::Long nid = createNotifyId();
  Ice::Long server_index = GetServreIndex(nid, props);
  try {
    NotifyDispatcherNAdapter::instance().dispatchN(server_index, nid, true, props);
  } catch(Ice::Exception& e) {
    MCE_WARN("DispatchManagerI::dispatch --> dispatchN nid:" << server_index
        << " from:" << from_uid << " exception:" << e);
  }

  MyUtil::Str2StrMap::const_iterator it = props.find("to");
  string to_ids= "NULL";
  if(it != props.end()){
    to_ids = it->second;
  }
  MyUtil::Str2StrMap::const_iterator sch_it = props.find("schema_id");
  string schema_id = "NULL";
  if(sch_it != props.end()){
    schema_id = sch_it->second;
  }
  MyUtil::Str2StrMap::const_iterator tomi_it = props.find("tominigroup");
  string groups = "NULL";
  if(tomi_it != props.end()){
    groups = tomi_it->second;
  }
  MyUtil::Str2StrMap::const_iterator from_it = props.find("from");
  string from = "NULL";
  if(from_it != props.end()){
    from = from_it->second;
  }
  MyUtil::Str2StrMap::const_iterator time_it = props.find("time");
  string time_str = "NULL";
  if(time_it != props.end()){
    time_str = time_it->second;
  }
  ostringstream out_info;
  out_info << "DispatchManagerI::dispatch --> nid:" << nid 
      << " to:" << to_ids 
      << " schema:" << schema_id
      << " type:" << props.find("type")->second
      << " from:" << from
      << " idx:" << server_index
      << " tog:" << groups
      << " time:" << time_str
      << " cost:" << ts.getTime() 
      << " " << GetIp(cur);
  XceLogSender::instance().Send(nid, out_info.str());
  MCE_INFO(out_info.str());
	FunStatManager::instance().Stat("DispatchManagerI::dispatch", ts.getTime(), false);
}

bool DispatchManagerI::reloadSchema(const Ice::Current&) {
  MCE_INFO("DispatchManagerI::reloadSchema --> start");
  return SchemaManager::instance().reload();
}

Ice::Long DispatchManagerI::createNotifyId() {
  {
    IceUtil::Mutex::Lock lock(_mutex);
    if (++_count > 5000) {
      MCE_WARN("DbHelper::createNotifyId --> IdSeqDb is blocked");
      --_count;
      return -1;
    }
  }
  {
    IceUtil::Mutex::Lock lock(_mutex);
    while (1) {
      MCE_DEBUG("_curIndex:" << _curIndex << ", _topIndex:" << _topIndex
          << ", count:" << _count);
      if (_curIndex != -1 && _curIndex < _topIndex) {
        --_count;
        return _curIndex++;
      }
      {
        Statement sql;
        sql << "update " << TAB_NOTIFY_ID_SEQ << " SET id = id +"
            << ID_SEQ_STEP;
        QueryRunner(DB_INSTANCE, CDbWServer, TAB_NOTIFY_ID_SEQ).store(
            sql);
      }
      {
        Statement sql;
        sql << "SELECT * FROM " << TAB_NOTIFY_ID_SEQ;
        mysqlpp::StoreQueryResult res = QueryRunner(DB_INSTANCE,
            CDbWServer, TAB_NOTIFY_ID_SEQ).store(sql);
        if (!res) {
          --_count;
          return -1;
        }
        for (size_t i = 0; i < res.num_rows(); ++i) {
          mysqlpp::Row row = res.at(i);
          _topIndex = (Ice::Long) row["ID"];
          _curIndex = _topIndex - ID_SEQ_STEP;
          MCE_DEBUG("topIndex:" << _topIndex);
          break;
        }
      }

    }
  }
}

