#include "TicketManagerI.h"
#include "tripod_service/Ticket/share/ticket_const.h"
#include "tripod_service/Ticket/share/ticket_util-inl.h"
#include "app/Utils/src/DBUtils.h"
#include "OceCxxAdapter/src/UserPassportAdapter.h"
#include "OceCxxAdapter/src/UserSyncAdapter.h"
#include "util/cpp/InvokeMonitor.h"
#include "Monitor/client/service/ServiceMonitor.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

using namespace MyUtil;
using namespace xce::serverstate;
using namespace com::xiaonei::xce;
using namespace xce::tripod::ticket;
using namespace xce::tripod;


typedef std::vector<std::string> KeySeq;

//初始化
//--------------------------------------------------------------
void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();

  //读取配置
  string cfg_biz = service.getCommunicator()->getProperties()->getPropertyWithDefault(SERVER_CONFIG_PREFIX + ".Type", BUSINESS_T_TICKET);
  string cfg_namespace = service.getCommunicator()->getProperties()->getPropertyWithDefault(SERVER_CONFIG_PREFIX + ".Space", NAMESPACE_DEFAULT);
  int cfg_mod = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault(SERVER_CONFIG_PREFIX + ".Mod", 0);
  int cfg_interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault(SERVER_CONFIG_PREFIX + ".Interval", 5);
  int cfg_status = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault(SERVER_CONFIG_PREFIX + ".Status", 1);

  //注册服务
  service.getAdapter()->add(&TicketManagerI::instance(), service.createIdentity(SERVICE_IDENTITY, ""));

  //HA
  ServerStateSubscriber::instance().initialize(CONTROLLER + cfg_biz, &TicketManagerI::instance(), cfg_mod, cfg_interval);
  {
    ServerStateSubscriber::instance().setStatus(cfg_status);
    std::ostringstream os;
    os << "MyUtil::initialize set status : " << cfg_status;
    MCE_WARN(os.str());
  }

  TicketManagerI::instance().init(cfg_namespace, cfg_biz);

  //监控
  ServiceMonitorManager::instance().start();
}

//TicketManagerI
//--------------------------------------------------------------
TicketManagerI::TicketManagerI() {
  _tempMutexManager = new xce::tempmutex::TempMutexManager();
}

void TicketManagerI::init(const string& ns, const string& biz) {
  MCE_WARN("TicketManagerI::init ns: " << ns << " biz: " << biz);
  if(biz == BUSINESS_P_TICKET) {
    _table = TABLE_P_TICKET;
  } else {
    _table = TABLE_T_TICKET;
  }
  _biz = biz;
  _tripodClient = new TripodClient(ns, biz);
}

string TicketManagerI::createTicket(const Str2StrMap& props, int expiredTime, const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, "", InvokeClient::INVOKE_LEVEL_INFO);

  string result;
  long id, realId;
  unsigned int type;
  string ip;

  Str2StrMap::const_iterator itId = props.find(CTID);
  Str2StrMap::const_iterator itRealId = props.find(CTREALID);
  Str2StrMap::const_iterator itType = props.find(CTTYPE);
  Str2StrMap::const_iterator itIp = props.find(CTIP);
  if(itId == props.end() || itRealId == props.end() || itType == props.end() || itType == props.end()) {
    MCE_WARN("TicketManagerI::createTicket Exception: info not complete");
  } else {
    id = boost::lexical_cast<long>(itId->second);
    realId = boost::lexical_cast<long>(itRealId->second);
    type = boost::lexical_cast<unsigned int>(itType->second);
    ip = itIp->second;

    Statement sqlQuery;
    sqlQuery << "select " << CTTICKET << "," << CTEXPIREDTIME << " from " << _table 
        << " where " << CTID  << "=" << id << " and " << CTREALID << "=" << realId << " and " << CTTYPE << "=" << type;
    MyUtil::Str2IntMap keyMap;
    BatchValidTicketHandlerI handler(keyMap);
    QueryRunner(DB_TRIPOD_TICKET, CDbWServer).query(sqlQuery, handler);
    if(keyMap.size()) {
      result = keyMap.begin()->first;
      int expiredTime = keyMap.begin()->second;
      _tripodClient->Load(result, expiredTime, true);
      if(_biz == BUSINESS_P_TICKET) {
          com::renren::datasync::PTicketLoadSyncAdapter::instance().add(result);
      } else {
          com::renren::datasync::TTicketLoadSyncAdapter::instance().add(result);
      }
    } else { //不存在有效票
      try {
        stringstream ss;
        xce::adapter::userpassport::UserPassportInfoPtr up =
          xce::adapter::userpassport::UserPassportAdapter::instance().getUserPassport(realId);
        if(!up) {
          MCE_WARN("TicketManagerI::createTicket Exception: fetch passport fail, realId: " << realId);
        } else {
          ss << _table << up->account() << up->passwordmd5() << up->domain();

          string ticket = TicketUtil::genTicket(id, (unsigned char)type, ss.str());
          unsigned dbTime = time(NULL); //DB保存超时的绝对时间
          dbTime += expiredTime;
          Statement sqlInsert;
          //insert into p_ticket(id,realid,type,ticket,ip,expiredtime) values(2,2,2,'2','2.2.2.2',222)
          //on duplicate key update ticket='2', ip='2.2.2.2', expiredtime=222;
          sqlInsert << "insert into " << _table << "(" << CTID << "," << CTREALID << "," << CTTYPE
            << "," << CTTICKET << "," << CTIP << "," << CTEXPIREDTIME << ") value(" << id << "," 
            << realId << "," << type << ",'" << ticket << "','" << ip << "'," << dbTime << ")"
            << " on duplicate key update " << CTTICKET << "='" << ticket << "'," 
            << CTIP << "='" << ip << "'," << CTEXPIREDTIME << "=" << dbTime;
          QueryRunner(DB_TRIPOD_TICKET, CDbWServer).execute(sqlInsert);
          _tripodClient->Load(ticket, expiredTime, true);
          if(_biz == BUSINESS_P_TICKET) {
              com::renren::datasync::PTicketLoadSyncAdapter::instance().add(ticket);
          }else {
              com::renren::datasync::TTicketLoadSyncAdapter::instance().add(ticket);
          }
          result = ticket;
        }
      } catch (Ice::Exception& e) {
        MCE_WARN("[TicketManagerI::createTicket] Ice::Exception : " << e.what());
      } catch (std::exception& e) {
        MCE_WARN("[TicketManagerI::createTicket] std::exception : " << e.what());
      } catch (...) {
        MCE_WARN("[TicketManagerI::createTicket] UNKNOWN exception");
      }
    }
  }
  return result;
}

bool TicketManagerI::destroyTicket(const string& ticket, const Ice::Current& current) {
  ostringstream oss;
  oss << "ticket=" << ticket;
  InvokeClient ic = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

  bool result = true;

  Statement sqlDelete;
  sqlDelete << "delete from " << _table << " where " << CTTICKET << "='" << ticket << "'";
  QueryRunner(DB_TRIPOD_TICKET, CDbWServer).execute(sqlDelete);
  result = _tripodClient->Remove(ticket);
 
  if(_biz == BUSINESS_P_TICKET) {
      com::renren::datasync::PTicketDelSyncAdapter::instance().add(ticket);
  }else {
      com::renren::datasync::TTicketDelSyncAdapter::instance().add(ticket);
  }

  return result;
}

bool TicketManagerI::destroyTicketById(long id, const IntSeq& types, const Ice::Current& current) {
  ostringstream oss;
  oss << "id=" << id;
  InvokeClient ic = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

  bool result = true;

  ostringstream sqlFilter;
  IntSeq::const_iterator it = types.begin();
  sqlFilter << " where " << CTREALID << "=" << id << " and " << CTTYPE << " in (";
  if(it != types.end()) {
    sqlFilter << *it++;
  }
  for (; it != types.end(); ++it) {
    sqlFilter << "," << *it;
  }
  sqlFilter << ")";

  Statement sqlQuery, sqlDelete;
  sqlQuery << "select " << CTTICKET << " from " << _table << sqlFilter.str();
  sqlDelete << "delete from " << _table << sqlFilter.str();
  KeySeq keys;
  xce::utils::BatchIdResultHandler<string> handler(CTTICKET, keys);
  QueryRunner(DB_TRIPOD_TICKET, CDbWServer).query(sqlQuery, handler);
  QueryRunner(DB_TRIPOD_TICKET, CDbWServer).execute(sqlDelete);
  for(KeySeq::iterator it_key = keys.begin(); it_key != keys.end(); ++it) {
    result = _tripodClient->Remove(*it_key);
  }
  
  if(_biz == BUSINESS_P_TICKET) {
      com::renren::datasync::PTicketDelSyncAdapter::instance().add(keys);
  }else {
      com::renren::datasync::TTicketDelSyncAdapter::instance().add(keys);
  }

  return result;
}

bool TicketManagerI::isValid(const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, "", InvokeClient::INVOKE_LEVEL_INFO);

  return ServerStateSubscriber::instance().getStatus();
}

void TicketManagerI::setValid(bool valid, const Ice::Current& current) {
  ostringstream oss;
  oss << "valid=" << valid;
  InvokeClient ic = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

  ServerStateSubscriber::instance().setStatus((int) valid);
}

//BatchValidTicketHandlerI
//获取有效的票和超时的时间，方便load的时候设置expiredTime参数
//--------------------------------------------------------------
BatchValidTicketHandlerI::BatchValidTicketHandlerI(MyUtil::Str2IntMap& results) : _results(results) {
}

bool BatchValidTicketHandlerI::handle(mysqlpp::Row& row) const {
  string ticket = (string)row[CTTICKET.c_str()];
  unsigned expireTime = (unsigned)row[CTEXPIREDTIME.c_str()];
  unsigned now = time(NULL);
  int expire = expireTime - now;
  if(expire > 0) {
    _results[ticket] = expire;
  } else {
    //可删除DB过时的票
  }
  return true;
}
