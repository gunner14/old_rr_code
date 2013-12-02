#ifndef __XCE_ADAPTER_TRIPODUSERSTATEADAPTER_H__
#define __XCE_ADAPTER_TRIPODUSERSTATEADAPTER_H__

#include <UserBase.pb.h>
#include <TripodSimpleAdapter.h>

namespace xce {
namespace adapter {

using xce::tripoddata::userbase::PbUserState;

//统一管理，一些共用参数放在Proto中
const string DB_SOURCE_USERSTATE =
  PbUserState::descriptor()->options().GetExtension(tripod::options::database_source);
const string DB_TABLE_USERSTATE =
  PbUserState::descriptor()->options().GetExtension(tripod::options::table_name);
const string DB_KEY_USERSTATE =
  PbUserState::descriptor()->options().GetExtension(tripod::options::primary_key);
const string BUSINESS_ID_USERSTATE =
  PbUserState::descriptor()->options().GetExtension(tripod::options::business_id);


class TripodUserStateAdapter : public TripodSimpleAdapter<PbUserState> {
public:
  TripodUserStateAdapter(
      const string& zkAddress,
      const string& namespaceId,
      const string& businessId = BUSINESS_ID_USERSTATE,
      const string& db = DB_SOURCE_USERSTATE,
      const string& table = DB_TABLE_USERSTATE,
      const string& key = DB_KEY_USERSTATE
      ) :
    TripodSimpleAdapter<PbUserState>(zkAddress, namespaceId, businessId, db, table, key){}
};

}} //namespace xce::adapter

#endif
