#include "tripod2/cc/producermanager/user_view_count_producer.h"
#include "DbCxxPool/src/QueryRunner.h"

namespace xce {
namespace tripod {

bool UserViewCountProducer::Create(const std::string& key, ByteSeq* value, bool use_master) {
  using std::string;
  using namespace com::xiaonei::xce;
  using namespace xce::footprint;
  MCE_INFO("[UserViewCountProducer::create] " <<  ", key = " << key);
  Statement sql;
  sql << "select " << COLUMN_ID << ", " << COLUMN_VIEWCOUNT << " from " << TABLE_VIEW_COUNT
      << " where " << COLUMN_ID << " = " << key;
  vector<UserVisitCountInfoPtr> ptrs;
  UserVisitCountInfoResultHandlerI handler(ptrs);
  if ( ptrs.size() == 1 ) {
    QueryRunner(DESC_VIEW_COUNT, use_master?CDbWServer:CDbRServer).query(sql, handler);
    vector<UserVisitCountInfoPtr>::const_iterator it = ptrs.begin(); 
    string db_key = boost::lexical_cast<string>((*it)->getUserId());
    if ( db_key != key ) {
      MCE_WARN("[UserViewCountProducer::create] " << ", key = " << key << " fail");
      return false;
    }
    string byte_array = (*it)->serialize();
    (*value) = ByteSeq(byte_array.begin(), byte_array.end());
    return true;
  } else if ( ptrs.size() == 0 ) {
    UserVisitCountInfoPtr ptr = new UserVisitCountInfo(); 
    string byte_array = ptr->serialize();
    (*value) = ByteSeq(byte_array.begin(), byte_array.end());
    return true;
  } else {
    MCE_WARN("[UserViewCountProducer::create] " << ", key = " << key << " fail");
    return false;
  }
}

}}


