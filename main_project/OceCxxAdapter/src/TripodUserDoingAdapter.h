#ifndef __XCE_ADAPTER_TRIPODUSERDOINGADAPTER_H__
#define __XCE_ADAPTER_TRIPODUSERDOINGADAPTER_H__

#include <UserBase.pb.h>
#include <TripodSimpleAdapter.h>

namespace xce {
namespace adapter {

using xce::tripoddata::userbase::PbUserDoing;

//统一管理，一些共用参数放在Proto中
const string DB_SOURCE_USERDOING =
  PbUserDoing::descriptor()->options().GetExtension(tripod::options::database_source);
const string DB_TABLE_USERDOING =
  PbUserDoing::descriptor()->options().GetExtension(tripod::options::table_name);
const string DB_KEY_USERDOING =
  PbUserDoing::descriptor()->options().GetExtension(tripod::options::primary_key);
const string BUSINESS_ID_USERDOING =
  PbUserDoing::descriptor()->options().GetExtension(tripod::options::business_id);


class TripodUserDoingAdapter : public TripodSimpleAdapter<PbUserDoing> {
public:
  TripodUserDoingAdapter(
      const string& zkAddress,
      const string& namespaceId,
      const string& businessId = BUSINESS_ID_USERDOING,
      const string& db = DB_SOURCE_USERDOING,
      const string& table = DB_TABLE_USERDOING,
      const string& key = DB_KEY_USERDOING
      ) :
    TripodSimpleAdapter<PbUserDoing>(zkAddress, namespaceId, businessId, db, table, key){}
};

}} //namespace xce::adapter

#endif
