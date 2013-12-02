#include "account_connect_producer.h"
#include "OceProto/proto/account_connect.pb.h"

namespace xce {
namespace tripod {

bool AccountConnectProducer::Create(const std::string& key, ByteSeq* value, bool use_master) {
  using namespace std;
  using namespace com::xiaonei::xce;
  using namespace xce::tripoddata::account;
  using namespace MyUtil;
  MCE_INFO("[AccountConnectProducer:Create] " << ", key = " << key);
  size_t spos = key.find("_");//以第一个下划线来划分type和cid
  int type = 0;
  string cid;
  if(spos != string::npos) {
    string stype(key, 0, spos);
    try {
      type = boost::lexical_cast<int>(stype);
      string tmp(key, spos + 1, string::npos);
      cid.swap(tmp);
    } catch(boost::bad_lexical_cast e) {
      MCE_WARN("[AccountConnectProducer::Create] invalid type: " << stype
          << " key: " << key << " e: " << e.what());
      return false; //type转换失败
    }
  }
  if(!cid.size()) {
    MCE_WARN("[AccountConnectProducer::Create] faild key: " << key);
    return false; //切分key失败
  }
  Statement sql;
  sql << "SELECT uid FROM account_connect WHERE cid='" << cid 
      << "' AND type=" << type << " LIMIT 1";
  int id;
  AccountConnectHandler handler(id);
  size_t rows = QueryRunner("user_passport", use_master?CDbWServer:CDbRServer).query(sql, handler);
  if(rows) {
    boost::shared_ptr<PbAccountConnect> message(new PbAccountConnect);
    message->set_uid(id);
    std::string str;
    message->SerializeToString(&str);
    (*value) = ByteSeq(str.begin(), str.end());
    return true;
  } else {
    return false;
  }
}

AccountConnectHandler::AccountConnectHandler(int &id) : id_(id) {
}

bool AccountConnectHandler::handle(mysqlpp::Row& row) const {
  id_ = (int) row["uid"];
  return true;
}

}}
