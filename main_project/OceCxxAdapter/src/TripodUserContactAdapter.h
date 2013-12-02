#ifndef __XCE_ADAPTER_TRIPODUSERCONTACTADAPTER_H__
#define __XCE_ADAPTER_TRIPODUSERCONTACTADAPTER_H__

#include <UserDesc.pb.h>
#include <TripodSimpleAdapter.h>

namespace xce {
namespace adapter {

using xce::tripoddata::userdesc::PbUserContact;

//统一管理，一些共用参数放在Proto中
const string DB_SOURCE_USERCONTACT =
  PbUserContact::descriptor()->options().GetExtension(tripod::options::database_source);
const string DB_TABLE_USERCONTACT =
  PbUserContact::descriptor()->options().GetExtension(tripod::options::table_name);
const string DB_KEY_USERCONTACT =
  PbUserContact::descriptor()->options().GetExtension(tripod::options::primary_key);
const string BUSINESS_ID_USERCONTACT =
  PbUserContact::descriptor()->options().GetExtension(tripod::options::business_id);


class TripodUserContactAdapter : public TripodSimpleAdapter<PbUserContact> {
public:
  TripodUserContactAdapter(
      const string& zkAddress,
      const string& namespaceId,
      const string& businessId = BUSINESS_ID_USERCONTACT,
      const string& db = DB_SOURCE_USERCONTACT,
      const string& table = DB_TABLE_USERCONTACT,
      const string& key = DB_KEY_USERCONTACT
      ) :
    TripodSimpleAdapter<PbUserContact>(zkAddress, namespaceId, businessId, db, table, key){}
};

}} //namespace xce::adapter

#endif
