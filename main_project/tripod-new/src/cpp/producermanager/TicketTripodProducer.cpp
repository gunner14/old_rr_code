#include "TicketTripodProducer.h"
#include "boost/lexical_cast.hpp"
#include "app/Utils/src/DBUtils.h"
#include "QueryRunner.h"

using namespace com::xiaonei::xce;
using namespace com::renren::tripod;
using namespace xce::tripod::ticket;

DataMap TicketTripodProducer::create(const KeySeq& keys, bool isMaster) {
  DataMap result;
  BatchTicketHandlerI handler(result);
  Statement sql;
  sql << "SELECT * FROM " << _table << " WHERE " << CTTICKET << " IN (";
  KeySeq::const_iterator it = keys.begin();
  if(it != keys.end()) {
    sql << "'" << *it++ << "'";
  }
  for (; it != keys.end(); ++it) {
    sql << "," <<"'"<<*it<<"'";
  }
  sql << ")";
  QueryRunner(DB_TRIPOD_TICKET, isMaster ? CDbWServer : CDbRServer).query(sql, handler);
  MCE_DEBUG("[TicketTripodProducer::create] keySize = " << keys.size() << ", resultSize = " << result.size());

  return result;
}

KeySeq TicketTripodProducer::createHotKeys(const string& beginKey, int limit) {
  KeySeq result;
  return result;
}

KeySeq TicketTripodProducer::createKeys(const string& beginKey, int limit) {
  MCE_INFO("[TicketTripodProducer::createKeys] beginKey = " << beginKey << ", limit = " << limit);
  KeySeq result;
  Statement sql;
  sql << "select " << CTTICKET << " from " << _table << " where " << CTTICKET
      << " >= " << beginKey << " limit " << limit;
  xce::utils::BatchIdResultHandler<string> handler(CTTICKET, result);
  QueryRunner(DB_TRIPOD_TICKET, CDbRServer).query(sql, handler);
  MCE_INFO("[TicketTripodProducer::createKeys] beginKey = " << beginKey << ", limit = " << limit << ", result.size = " << result.size());
  return result;
}
