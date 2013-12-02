#ifndef __XCE_ADAPTER_TRIPODUSERLOGINCOUNTADAPTER_H__
#define __XCE_ADAPTER_TRIPODUSERLOGINCOUNTADAPTER_H__

#include <UserBase.pb.h>
#include <TripodSimpleAdapter.h>

namespace xce {
namespace adapter {

using xce::tripoddata::userbase::PbUserLoginCount;

//统一管理，一些共用参数放在Proto中
const string DB_SOURCE_USERLOGINCOUNT =
  PbUserLoginCount::descriptor()->options().GetExtension(tripod::options::database_source);
const string DB_TABLE_USERLOGINCOUNT =
  PbUserLoginCount::descriptor()->options().GetExtension(tripod::options::table_name);
const string DB_KEY_USERLOGINCOUNT =
  PbUserLoginCount::descriptor()->options().GetExtension(tripod::options::primary_key);
const string BUSINESS_ID_USERLOGINCOUNT =
  PbUserLoginCount::descriptor()->options().GetExtension(tripod::options::business_id);


class TripodUserLoginCountAdapter : public TripodSimpleAdapter<PbUserLoginCount> {
public:
  TripodUserLoginCountAdapter(
      const string& zkAddress,
      const string& namespaceId,
      const string& businessId = BUSINESS_ID_USERLOGINCOUNT,
      const string& db = DB_SOURCE_USERLOGINCOUNT,
      const string& table = DB_TABLE_USERLOGINCOUNT,
      const string& key = DB_KEY_USERLOGINCOUNT
      ) :
    TripodSimpleAdapter<PbUserLoginCount>(zkAddress, namespaceId, businessId, db, table, key){}
};

}} //namespace xce::adapter

#endif
