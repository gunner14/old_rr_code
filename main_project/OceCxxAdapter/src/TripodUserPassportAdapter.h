#ifndef __XCE_ADAPTER_TRIPODUSERPASSPORTADAPTER_H__
#define __XCE_ADAPTER_TRIPODUSERPASSPORTADAPTER_H__

#include <UserBase.pb.h>
#include <TripodSimpleAdapter.h>

namespace xce {
namespace adapter {

using xce::tripoddata::userbase::PbUserPassport;

//统一管理，一些共用参数放在Proto中
const string DB_SOURCE_USERPASSPORT =
  PbUserPassport::descriptor()->options().GetExtension(tripod::options::database_source);
const string DB_TABLE_USERPASSPORT =
  PbUserPassport::descriptor()->options().GetExtension(tripod::options::table_name);
const string DB_KEY_USERPASSPORT =
  PbUserPassport::descriptor()->options().GetExtension(tripod::options::primary_key);
const string BUSINESS_ID_USERPASSPORT =
  PbUserPassport::descriptor()->options().GetExtension(tripod::options::business_id);


class TripodUserPassportAdapter : public TripodSimpleAdapter<PbUserPassport> {
public:
  TripodUserPassportAdapter(
      const string& zkAddress,
      const string& namespaceId,
      const string& businessId = BUSINESS_ID_USERPASSPORT,
      const string& db = DB_SOURCE_USERPASSPORT,
      const string& table = DB_TABLE_USERPASSPORT,
      const string& key = DB_KEY_USERPASSPORT
      ) :
    TripodSimpleAdapter<PbUserPassport>(zkAddress, namespaceId, businessId, db, table, key){}
};

}} //namespace xce::adapter

#endif
