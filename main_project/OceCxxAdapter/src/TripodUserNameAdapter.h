#ifndef __XCE_ADAPTER_TRIPODUSERNAMEADAPTER_H__
#define __XCE_ADAPTER_TRIPODUSERNAMEADAPTER_H__

#include <UserBase.pb.h>
#include <TripodSimpleAdapter.h>

namespace xce {
namespace adapter {

using xce::tripoddata::userbase::PbUserName;

//统一管理，一些共用参数放在Proto中
const string DB_SOURCE_USERNAME =
  PbUserName::descriptor()->options().GetExtension(tripod::options::database_source);
const string DB_TABLE_USERNAME =
  PbUserName::descriptor()->options().GetExtension(tripod::options::table_name);
const string DB_KEY_USERNAME =
  PbUserName::descriptor()->options().GetExtension(tripod::options::primary_key);
const string BUSINESS_ID_USERNAME =
  PbUserName::descriptor()->options().GetExtension(tripod::options::business_id);


class TripodUserNameAdapter : public TripodSimpleAdapter<PbUserName> {
public:
  TripodUserNameAdapter(
      const string& zkAddress,
      const string& namespaceId,
      const string& businessId = BUSINESS_ID_USERNAME,
      const string& db = DB_SOURCE_USERNAME,
      const string& table = DB_TABLE_USERNAME,
      const string& key = DB_KEY_USERNAME
      ) :
    TripodSimpleAdapter<PbUserName>(zkAddress, namespaceId, businessId, db, table, key){}
};

}} //namespace xce::adapter

#endif
