#ifndef __XCE_ADAPTER_TRIPODUSERRIGHTADAPTER_H__
#define __XCE_ADAPTER_TRIPODUSERRIGHTADAPTER_H__

#include <UserBase.pb.h>
#include <TripodSimpleAdapter.h>

namespace xce {
namespace adapter {

using xce::tripoddata::userbase::PbUserRight;

//统一管理，一些共用参数放在Proto中
const string DB_SOURCE_USERRIGHT =
  PbUserRight::descriptor()->options().GetExtension(tripod::options::database_source);
const string DB_TABLE_USERRIGHT =
  PbUserRight::descriptor()->options().GetExtension(tripod::options::table_name);
const string DB_KEY_USERRIGHT =
  PbUserRight::descriptor()->options().GetExtension(tripod::options::primary_key);
const string BUSINESS_ID_USERRIGHT =
  PbUserRight::descriptor()->options().GetExtension(tripod::options::business_id);


class TripodUserRightAdapter : public TripodSimpleAdapter<PbUserRight> {
public:
  TripodUserRightAdapter(
      const string& zkAddress,
      const string& namespaceId,
      const string& businessId = BUSINESS_ID_USERRIGHT,
      const string& db = DB_SOURCE_USERRIGHT,
      const string& table = DB_TABLE_USERRIGHT,
      const string& key = DB_KEY_USERRIGHT
      ) :
    TripodSimpleAdapter<PbUserRight>(zkAddress, namespaceId, businessId, db, table, key){}
};

}} //namespace xce::adapter

#endif
