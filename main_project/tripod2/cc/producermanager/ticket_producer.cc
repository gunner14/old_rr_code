#include "tripod2/cc/producermanager/ticket_producer.h"
#include "OceSlice/slice/TripodTicket.h"
#include "OceProto/proto/Ticket.pb.h"
#include "DbCxxPool/src/QueryRunner.h"
#include "UtilCxx/src/LogWrapper.h"

namespace xce {
namespace tripod {

using namespace xce::tripod::ticket;
using namespace xce::tripoddata::ticket;
using namespace com::xiaonei::xce;

bool TicketProducer::Create(const std::string& key, ByteSeq* value, bool use_master) {
  StringListType temp_keys;
  String2ByteSeqMap temp_values; 
  temp_keys.push_back(key);
  std::map<std::string, bool> result = Create(temp_keys, &temp_values, use_master);
  String2ByteSeqMap::iterator it = temp_values.find(key);
  if( it != temp_values.end()) {
    (*value) = it->second;
    return true;
  } 
  return false;
}

std::map<std::string, bool> TicketProducer::Create(const StringListType& keys,
    String2ByteSeqMap* values, bool use_master){
  std::map<std::string, bool> result;
  if(!values) {
    return result;
  }
  BatchTicketHandlerI handler(*values);
  Statement sql;
  sql << "SELECT * FROM " << _table << " WHERE " << CTTICKET << " IN (";
  StringListType::const_iterator it = keys.begin();
  if(it != keys.end()) {
    sql << "'" << *it++ << "'";
  }
  for (; it != keys.end(); ++it) {
    sql << "," << "'" << *it << "'";
  }
  sql << ")";
  QueryRunner(DB_TRIPOD_TICKET, use_master ? CDbWServer : CDbRServer).query(sql, handler);
  MCE_DEBUG("[TicketTripodProducer::create] keySize = " << keys.size() << ", resultSize = " << result.size());

  //补齐result
  for(StringListType::const_iterator it = keys.begin(); it != keys.end(); ++it) {
    String2ByteSeqMap::iterator itm = values->find(*it);
    if(itm != values->end()) {
      result[*it] = true;
    } else {
      result[*it] =false;
    }
  }

  return result;
}

//BatchTicketHandlerI
//--------------------------------------------------------------
BatchTicketHandlerI::BatchTicketHandlerI(String2ByteSeqMap& results) : _results(results) {
}

bool BatchTicketHandlerI::handle(mysqlpp::Row& row) const {
  long id = (long)row[CTID.c_str()];
  long realId = (long)row[CTREALID.c_str()];
  string ticket = (string)row[CTTICKET.c_str()];
  string ip = (string)row[CTIP.c_str()];
  unsigned type = (unsigned)row[CTTYPE.c_str()];
  unsigned expireTime = (unsigned)row[CTEXPIREDTIME.c_str()];
  unsigned now = time(NULL);
  int expire = expireTime - now;
  if(expire > 0) {
    TicketData data;
    data.set_id(id);
    data.set_real_id(realId);
    data.set_type(type);
    data.set_ip(ip);
    ostringstream os;
    data.SerializeToOstream(&os);
    string value = os.str();
    _results[ticket] = ByteSeq(value.begin(), value.end());
  } else {
    //可删除DB过时的票
  }
  return true;
}

}} // xce::tripod
