#ifndef __XCE_ADAPTER_TRIPODUSERURLADAPTER_H__
#define __XCE_ADAPTER_TRIPODUSERURLADAPTER_H__

#include <UserBase.pb.h>
#include <TripodSimpleAdapter.h>

namespace xce {
namespace adapter {

using xce::tripoddata::userbase::PbUserUrl;

//统一管理，一些共用参数放在Proto中
const string DB_SOURCE_USERURL =
  PbUserUrl::descriptor()->options().GetExtension(tripod::options::database_source);
const string DB_TABLE_USERURL =
  PbUserUrl::descriptor()->options().GetExtension(tripod::options::table_name);
const string DB_KEY_USERURL =
  PbUserUrl::descriptor()->options().GetExtension(tripod::options::primary_key);
const string BUSINESS_ID_USERURL =
  PbUserUrl::descriptor()->options().GetExtension(tripod::options::business_id);


class TripodUserUrlAdapter : public TripodSimpleAdapter<PbUserUrl> {
public:
  TripodUserUrlAdapter(
      const string& zkAddress,
      const string& namespaceId,
      const string& businessId = BUSINESS_ID_USERURL,
      const string& db = DB_SOURCE_USERURL,
      const string& table = DB_TABLE_USERURL,
      const string& key = DB_KEY_USERURL
      ) :
    TripodSimpleAdapter<PbUserUrl>(zkAddress, namespaceId, businessId, db, table, key){}
};

}} //namespace xce::adapter

#endif
