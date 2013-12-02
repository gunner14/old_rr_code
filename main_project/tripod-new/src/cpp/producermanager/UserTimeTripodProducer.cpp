#include "UserTimeTripodProducer.h"
#include "app/Utils/src/DBUtils.h"
#include "QueryRunner.h"
#include "UserDesc.pb.h"

using namespace com::xiaonei::xce; //QueryRunner
using namespace com::renren::tripod; 
using namespace xce::tripod::userdesc;
using namespace xce::tripoddata::userdesc; //proto

DataMap UserTimeTripodProducer::create(const KeySeq& keys, bool isMaster) {
  DataMap result;
  BatchUserTimeHandlerI handler(result);
  Statement sql;
  sql << "SELECT * FROM " << TABLE_TRIPOD_USERTIME << " WHERE " << UTID << " IN (";
  KeySeq::const_iterator it = keys.begin();
  if(it != keys.end()) {
    sql << "'" << *it++ << "'";
  }
  for (; it != keys.end(); ++it) {
    sql << "," << *it;
  }
  sql << ")";
  QueryRunner(DB_TRIPOD_USERTIME, isMaster ? CDbWServer : CDbRServer).query(sql, handler);
  MCE_DEBUG("[UserTimeTripodProducer::create] keySize = " << keys.size() << ", resultSize = " << result.size());

  return result;
}

KeySeq UserTimeTripodProducer::createHotKeys(const string& beginKey, int limit) {
  KeySeq result;
  return result;
}

KeySeq UserTimeTripodProducer::createKeys(const string& beginKey, int limit) {
  MCE_INFO("[UserTimeTripodProducer::createKeys] beginKey = " << beginKey << ", limit = " << limit);
  KeySeq result;
  Statement sql;
  sql << "select " << UTID << " from " << TABLE_TRIPOD_USERTIME << " where " << UTID
      << " >= " << beginKey << " limit " << limit;
  xce::utils::BatchIdResultHandler<string> handler(UTID, result);
  QueryRunner(DB_TRIPOD_USERTIME, CDbRServer).query(sql, handler);
  MCE_INFO("[UserTimeTripodProducer::createKeys] beginKey = " << beginKey << ", limit = " << limit << ", result.size = " << result.size());
  return result;
}

//BatchUserTimeHandlerI
//--------------------------------------------------------------
BatchUserTimeHandlerI::BatchUserTimeHandlerI(DataMap& results) : _results(results) {
}

bool BatchUserTimeHandlerI::handle(mysqlpp::Row& row) const {
  string idKey = (string)row[UTID.c_str()];
  long id = (long)row[UTID.c_str()];
  long registertime = (time_t)mysqlpp::DateTime(row[UTREGISTERTIME.c_str()]);
  long activatetime = (time_t)mysqlpp::DateTime(row[UTACTIVETIME.c_str()]);
  long lastlogintime = (time_t)mysqlpp::DateTime(row[UTLASTLOGINTIME.c_str()]);
  long lastlogouttime = (time_t)mysqlpp::DateTime(row[UTLASTLOGOUTTIME.c_str()]);
  PbUserTime data;
  data.set_id(id);
  data.set_registertime(registertime);
  data.set_activatetime(activatetime);
  data.set_lastlogintime(lastlogintime);
  data.set_lastlogouttime(lastlogouttime);
  ostringstream os; 
  data.SerializeToOstream(&os);
  string value = os.str();
  _results[idKey] = Data(value.begin(), value.end());
  return true;
}
