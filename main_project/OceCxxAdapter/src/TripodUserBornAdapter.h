#ifndef __XCE_ADAPTER_TRIPODUSERBORNADAPTER_H__
#define __XCE_ADAPTER_TRIPODUSERBORNADAPTER_H__

#include <UserBase.pb.h>
#include <TripodSimpleAdapter.h>

namespace xce {
namespace adapter {

using xce::tripoddata::userbase::PbUserBorn;

//统一管理，一些共用参数放在Proto中
const string DB_SOURCE_USERBORN =
  PbUserBorn::descriptor()->options().GetExtension(tripod::options::database_source);
const string DB_TABLE_USERBORN =
  PbUserBorn::descriptor()->options().GetExtension(tripod::options::table_name);
const string DB_KEY_USERBORN =
  PbUserBorn::descriptor()->options().GetExtension(tripod::options::primary_key);
const string BUSINESS_ID_USERBORN =
  PbUserBorn::descriptor()->options().GetExtension(tripod::options::business_id);


class TripodUserBornAdapter : public TripodSimpleAdapter<PbUserBorn> {
public:
  TripodUserBornAdapter(
      const string& zkAddress,
      const string& namespaceId,
      const string& businessId = BUSINESS_ID_USERBORN,
      const string& db = DB_SOURCE_USERBORN,
      const string& table = DB_TABLE_USERBORN,
      const string& key = DB_KEY_USERBORN
      ) :
    TripodSimpleAdapter<PbUserBorn>(zkAddress, namespaceId, businessId, db, table, key){}
};

}} //namespace xce::adapter

#endif
