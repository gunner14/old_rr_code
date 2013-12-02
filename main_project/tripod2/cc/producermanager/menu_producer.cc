#include "tripod2/cc/producermanager/menu_producer.h"

namespace xce {
namespace tripod {

bool MenuProducer::Create(const std::string& key, ByteSeq* value, bool use_master) {
  using std::string;
  using namespace com::xiaonei::xce;
  using namespace xce::menutripod;
  using namespace xce::menu;
  string cache_type_str = TripodHelper::getCacheTypeStr(cache_type_);
  MCE_INFO("[MenuProducer:create] cacheType = " << cache_type_str  << ", key = " << key);
  long userid = boost::lexical_cast<long>(key);
  int table_sufix = userid % TABLE_CLUSTER;
  Statement sql;
  string tableName = TripodHelper::getTablePrefix(cache_type_) + boost::lexical_cast<string>(table_sufix);
  sql <<  "select user_id, app_ids from " << tableName << " where user_id = " << key;
  vector<MenuTripodDataPtr> ptrs;
  BatchMenuTripodDataHandler handler(ptrs, cache_type_);
  QueryRunner(TripodHelper::getDBSource(cache_type_), use_master?CDbWServer:CDbRServer, tableName).query(sql, handler);

  if ( ptrs.size() == 1 ) {
    vector<MenuTripodDataPtr>::const_iterator it = ptrs.begin();
    string db_key = boost::lexical_cast<string>((*it)->getUserId());
    if ( db_key != key ) {
      MCE_WARN("[MenuProducer::create] cacheType = " << cache_type_str << ", key = " << key << " fail");
      return false;
    }
    string byte_array = (*it)->serialize();
    (*value) = ByteSeq(byte_array.begin(), byte_array.end());
    return true;
  } else if ( ptrs.size() == 0 ) {
    MenuTripodDataPtr ptr( new MenuTripodData(0,cache_type_) );
    string byte_array = ptr->serialize();
    (*value) = ByteSeq(byte_array.begin(), byte_array.end());
    return true;
  } else {
    MCE_WARN("[MenuProducer::create] cacheType = " << cache_type_str << ", key = " << key << " fail");
    return false;
  }
}

}}


