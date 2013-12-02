#include <time.h>
#include "QueryRunner.h"
#include "TalkGroupAdminMsgLoaderI.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include <boost/lexical_cast.hpp>
#include "XmppTools.h"
#include "DbOpe.h"
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk::group::admmsgloader;

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&TalkGroupAdminMsgLoaderI::instance(), service.createIdentity("M", ""));

	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
	xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerTalkGroupAdminMsgLoader", &TalkGroupAdminMsgLoaderI::instance(), mod, interval, new XceFeedControllerChannel());
	MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);

}

/*说明：保存指定用户的消息
 *参数1：用户id
 */
bool TalkGroupAdminMsgLoaderI::saveMsg_(int uid, const string &msg) {
  MCE_INFO("TalkGroupAdminMsgLoaderI::saveMsg_ --> uid:" << uid);
  ostringstream sql;
  sql << "insert into " << DbOpe::GetDbTable(DB_PREF, TABLE_COUNT, uid) << " (uid, message) values ( "
      << uid << ", '" << EscapeString(msg) << "')";

  bool rv = DbOpe::SqlWrite(DB_INSTANCE, DbOpe::GetDbRoute(DB_ROUTE, TABLE_COUNT, uid).c_str(), sql.str().c_str());

  if (!rv) {
    MCE_WARN("TalkGroupAdminMsgLoaderI::saveMsg_ --> dosql error");
    return false;
  }

  return true;


}

/*说明：删除指定用户的消息
 *参数1：用户id
 */
bool TalkGroupAdminMsgLoaderI::delMsg_(int uid) {
  MCE_INFO("TalkGroupAdminMsgLoaderI::delMsg_ --> uid:" << uid);

  ostringstream sql;
  sql << "delete from " << DbOpe::GetDbTable(DB_PREF, TABLE_COUNT, uid) << " where uid = " << uid;


  bool rv = DbOpe::SqlWrite(DB_INSTANCE, DbOpe::GetDbRoute(DB_ROUTE, TABLE_COUNT, uid).c_str(), sql.str().c_str());	
  if (!rv) {
    MCE_WARN("TalkGroupAdminMsgLoaderI::delMsg_ --> dosql error");
    return false;
  }
    

  return true;
}

/*说明：得到指定用户的消息
 *参数1：用户id
 *参数2：是否同事删掉用户信息
 *参数3：用户信息
 *返回值：操作是否成功
 */
bool  TalkGroupAdminMsgLoaderI::getMsg_(int uid, bool del, MyUtil::StrSeq &msgs) {
  MCE_INFO("TalkGroupAdminMsgLoaderI::getMsg_ --> uid:" << uid);
  StrSeq &seq = msgs;
  seq.clear();
  ostringstream sql;

  sql << "select * from " << DbOpe::GetDbTable(DB_PREF, TABLE_COUNT, uid) << " where uid = " << uid ;

  mysqlpp::StoreQueryResult res;
  bool rv = DbOpe::SqlRead(DB_INSTANCE, DbOpe::GetDbRoute(DB_ROUTE, TABLE_COUNT, uid).c_str(), sql.str().c_str(), res);	
  if (!rv) {
    MCE_WARN("TalkGroupAdminMsgLoaderI::getMsg_ --> dosql error");
    return false;
  }


  for (size_t i = 0; i < res.num_rows(); ++i) {
    mysqlpp::Row row = res.at(i);
    seq.push_back((string)row["message"]);
  }

  if (del) {
    return delMsg_(uid);
  }



  return true;
}

/*说明：得到指定用户的消息
 *参数1：用户id
 *返回值：消息内容
 */
MyUtil::StrSeq TalkGroupAdminMsgLoaderI::getMsg(int uid, const Ice::Current &current) {
	TimeStatN ts;
  StrSeq seq;
  bool rv = getMsg_(uid, false, seq);
  if (!rv) {
    MCE_WARN("TalkGroupAdminMsgLoaderI::getMsg --> do error");
  }
  
	FunStatManager::instance().StatPlus(current, "TalkGroupAdminMsgLoaderI::getMsg", ts.getTime(), (ts.getTime() > 200));
  return seq;
}

/*说明：得到指定用户的消息
 *参数1：用户id
 *返回值：消息内容
 */
MyUtil::StrSeq TalkGroupAdminMsgLoaderI::getdelMsg(int uid, const Ice::Current &current) {
	TimeStatN ts;
  StrSeq seq;
  bool rv = getMsg_(uid, true, seq);
  if (!rv) {
    MCE_WARN("TalkGroupAdminMsgLoaderI::getdelMsg --> do error");
  }
  
	FunStatManager::instance().StatPlus(current, "TalkGroupAdminMsgLoaderI::getdelMsg", ts.getTime(), (ts.getTime() > 200));
  return seq;

}

/*说明：同步保存指定用户的消息
 *参数1：用户id
 */
bool TalkGroupAdminMsgLoaderI::saveMsg(int uid, const string &msg, const Ice::Current &current) {
	TimeStatN ts;
  bool rv = saveMsg_(uid, msg);
	FunStatManager::instance().StatPlus(current, "TalkGroupAdminMsgLoaderI::getMsg", ts.getTime(), (ts.getTime() > 200));
  return rv;

}

/*说明：异步保存指定用户的消息
 *参数1：用户id
 */
void TalkGroupAdminMsgLoaderI::saveMsgAsyn(int uid, const string &msg, const Ice::Current &current) {
	TimeStatN ts;
  saveMsg_(uid, msg);
	FunStatManager::instance().StatPlus(current, "TalkGroupAdminMsgLoaderI::getMsg", ts.getTime(), (ts.getTime() > 200));

}

/*说明：异步删除指定用户的消息
 *参数1：用户id
 */
void TalkGroupAdminMsgLoaderI::delMsgAsyn(int uid, const Ice::Current &current) {
	TimeStatN ts;
  delMsg_(uid);
	FunStatManager::instance().StatPlus(current, "TalkGroupAdminMsgLoaderI::getMsg", ts.getTime(), (ts.getTime() > 200));

}

/*说明：同步删除指定用户的消息
 *参数1：用户id
 */
bool TalkGroupAdminMsgLoaderI::delMsg(int uid, const Ice::Current &current) {
	TimeStatN ts;
  bool rv = delMsg_(uid);
	FunStatManager::instance().StatPlus(current, "TalkGroupAdminMsgLoaderI::getMsg", ts.getTime(), (ts.getTime() > 200));
  return rv;
}

/*说明：对字符串进行转义
 *参数1：原始字符串
 *参数2：替换模式
 *返回值：转义后的字符串
 */
string TalkGroupAdminMsgLoaderI::EscapeString(const string &raw_string, const bool mode) {
  int begin = 0;
  int end = raw_string.length();
  if (mode == 1) {
    begin = 1;
    end -= 1;
  }
  ostringstream os;
  if (mode == 1)
    os << raw_string.substr(0, begin);
  for (size_t i=begin; i<end; ++i) {
    if (raw_string[i]=='\''||raw_string[i]=='%' ||raw_string[i]=='\"' ) {
      os << '\\';
    }
    os << raw_string[i];
  }
  if (mode == 1)
    os << raw_string.substr(end,raw_string.length()-end);
  return os.str();
}



