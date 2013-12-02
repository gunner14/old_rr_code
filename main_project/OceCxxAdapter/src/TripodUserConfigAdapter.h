#ifndef __XCE_ADAPTER_TRIPODUSERCONFIGADAPTER_H__
#define __XCE_ADAPTER_TRIPODUSERCONFIGADAPTER_H__

#include <UserConfig.pb.h>
#include <TripodSimpleAdapter.h>

namespace xce {
namespace adapter {

using xce::tripod::UserConfigData;

//统一管理，一些共用参数放在Proto中
const string DB_SOURCE_USERCONFIG =
  UserConfigData::descriptor()->options().GetExtension(::tripod::options::database_source);
const string DB_TABLE_USERCONFIG =
  UserConfigData::descriptor()->options().GetExtension(::tripod::options::table_name);
const string DB_KEY_USERCONFIG =
  UserConfigData::descriptor()->options().GetExtension(::tripod::options::primary_key);
const string BUSINESS_ID_USERCONFIG =
  UserConfigData::descriptor()->options().GetExtension(::tripod::options::business_id);


class TripodUserConfigAdapter : public TripodSimpleAdapter<UserConfigData> {
public:
  TripodUserConfigAdapter(
      const string& zkAddress,
      const string& namespaceId,
      const string& businessId = BUSINESS_ID_USERCONFIG,
      const string& db = DB_SOURCE_USERCONFIG,
      const string& table = DB_TABLE_USERCONFIG,
      const string& key = DB_KEY_USERCONFIG
      ) :
    TripodSimpleAdapter<UserConfigData>(zkAddress, namespaceId, businessId, db, table, key){}
};

}} //namespace xce::adapter

#endif
