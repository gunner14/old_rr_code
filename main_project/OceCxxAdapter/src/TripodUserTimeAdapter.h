#ifndef __XCE_ADAPTER_TRIPODUSERTIMEADAPTER_H__
#define __XCE_ADAPTER_TRIPODUSERTIMEADAPTER_H__

#include <UserDesc.pb.h>
#include <TripodSimpleAdapter.h>

namespace xce {
namespace adapter {

using xce::tripoddata::userdesc::PbUserTime;

//统一管理，一些共用参数放在Proto中
const string DB_SOURCE_USERTIME =
  PbUserTime::descriptor()->options().GetExtension(tripod::options::database_source);
const string DB_TABLE_USERTIME =
  PbUserTime::descriptor()->options().GetExtension(tripod::options::table_name);
const string DB_KEY_USERTIME =
  PbUserTime::descriptor()->options().GetExtension(tripod::options::primary_key);
const string BUSINESS_ID_USERTIME =
  PbUserTime::descriptor()->options().GetExtension(tripod::options::business_id);


class TripodUserTimeAdapter : public TripodSimpleAdapter<PbUserTime> {
public:
  TripodUserTimeAdapter(
      const string& zkAddress,
      const string& namespaceId,
      const string& businessId = BUSINESS_ID_USERTIME,
      const string& db = DB_SOURCE_USERTIME,
      const string& table = DB_TABLE_USERTIME,
      const string& key = DB_KEY_USERTIME
      ) :
    TripodSimpleAdapter<PbUserTime>(zkAddress, namespaceId, businessId, db, table, key){}
};

}} //namespace xce::adapter

#endif
