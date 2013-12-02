#include "tripod2/cc/producermanager/home_foot_print_producer.h"

namespace xce {
namespace tripod {

HomeFootprintProducer::HomeFootprintProducer(int footprintSize) : footprintSize_(footprintSize) {
}

bool HomeFootprintProducer::Create(const std::string& key, ByteSeq* value, bool use_master) {
  using std::string;
  using namespace com::xiaonei::xce;
  using namespace xce::footprint;
  MCE_INFO("[HomeFootprintProducer::create] footprintSize = " << footprintSize_ << ", key = " << key);
  long userid = boost::lexical_cast<long>(key);
  int table_sufix = userid % TABLE_HOME_FOOTPRINT_HEAD_MOD;
  Statement sql;
  string tableName = GetTablePrefix() + "_" + boost::lexical_cast<string>(table_sufix);
  sql << "select " << COLUMN_OWNER << ", " << COLUMN_VISITOR << " from " << tableName 
      << " where " << COLUMN_OWNER << " = " << key;
  vector<HomeFootprintSetPtr> ptrs;
  HomeFootprintSetSeqHandler handler(ptrs, footprintSize_);
  QueryRunner(DESC_FOOTPRINT, use_master?CDbWServer:CDbRServer, tableName).query(sql, handler);
  if ( ptrs.size() != 1 ) {
    MCE_WARN("[HomeFootprintProducer::create] footprintSize = " << footprintSize_ << ", key = " << key << " fail");
    return false;
  }
  vector<HomeFootprintSetPtr>::const_iterator it = ptrs.begin(); 
  string db_key = boost::lexical_cast<string>((*it)->getOwnerId());
  if ( db_key != key ) {
    MCE_WARN("[HomeFootprintProducer::create] footprintSize = " << footprintSize_ << ", key = " << key << " fail");
    return false;
  }
  string byte_array = (*it)->serialize();
  (*value) = ByteSeq(byte_array.begin(), byte_array.end());
  return true;
}

string HomeFootprintProducer::GetTablePrefix() {
  using namespace xce::footprint;
  if (footprintSize_ == CACHE_SIZE) {
    return TABLE_HOME_FOOTPRINT_HEAD;
  } else {
    return TABLE_HOME_FOOTPRINT_BODY;
  }
}

}}


