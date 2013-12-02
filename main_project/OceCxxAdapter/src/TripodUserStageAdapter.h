#ifndef __XCE_ADAPTER_TRIPODUSERSTAGEADAPTER_H__
#define __XCE_ADAPTER_TRIPODUSERSTAGEADAPTER_H__

#include <UserBase.pb.h>
#include <TripodSimpleAdapter.h>

namespace xce {
namespace adapter {

using xce::tripoddata::userbase::PbUserStage;

//统一管理，一些共用参数放在Proto中
const string DB_SOURCE_USERSTAGE =
  PbUserStage::descriptor()->options().GetExtension(tripod::options::database_source);
const string DB_TABLE_USERSTAGE =
  PbUserStage::descriptor()->options().GetExtension(tripod::options::table_name);
const string DB_KEY_USERSTAGE =
  PbUserStage::descriptor()->options().GetExtension(tripod::options::primary_key);
const string BUSINESS_ID_USERSTAGE =
  PbUserStage::descriptor()->options().GetExtension(tripod::options::business_id);


class TripodUserStageAdapter : public TripodSimpleAdapter<PbUserStage> {
public:
  TripodUserStageAdapter(
      const string& zkAddress,
      const string& namespaceId,
      const string& businessId = BUSINESS_ID_USERSTAGE,
      const string& db = DB_SOURCE_USERSTAGE,
      const string& table = DB_TABLE_USERSTAGE,
      const string& key = DB_KEY_USERSTAGE
      ) :
    TripodSimpleAdapter<PbUserStage>(zkAddress, namespaceId, businessId, db, table, key){}
};

}} //namespace xce::adapter

#endif
