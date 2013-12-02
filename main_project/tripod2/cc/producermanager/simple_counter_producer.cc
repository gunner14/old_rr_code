#include "tripod2/cc/producermanager/simple_counter_producer.h" 
#include "boost/algorithm/string.hpp"

namespace xce {
namespace tripod {

using namespace com::xiaonei::xce;

SimpleCounterProducer::SimpleCounterProducer(const std::string& db, const std::string& table, 
    const std::string& key_field, const std::string& value_field) :
    db_(db), table_(table), key_field_(key_field), value_field_(value_field) {
  boost::split(key_fields_, key_field, boost::is_any_of(","));
  MCE_WARN("SimpleCounterProducer::SimpleCounterProducer() init, key_filed size=" << key_fields_.size());
}

bool SimpleCounterProducer::Create(const std::string& key, ByteSeq* value, bool use_master) {
  if (!value) {
    return false;
  }
  MCE_DEBUG("SimpleCounterProducer::Create(), key= " << key << ", use_master= " << use_master);
  //1. 生成sql语句
  StringMapType filter;
  if(key_fields_.size() == 1) {
    filter[key_fields_[0]] = key;
  } else { //多主键的情况
    std::vector<std::string> keys;
    boost::split(keys, key, boost::is_any_of(","));
    if(key_fields_.size() == keys.size()) {
      for(size_t i = 0; i != key_fields_.size(); ++i) {
        filter[key_fields_[i]] = keys[i];
      }
    } else {
      MCE_WARN("SimpleCounterProducer::Create() keys mismatch, key= " << key << ", field= " << key_field_);
      return false;
    }
  }
  //2. 查询返回
  std::string result;
  SimpleCounterResultHandler handler(value_field_, result);
  Statement sql = Statement::buildSelect(table_, filter);
  size_t count = QueryRunner(db_, use_master?CDbWServer:CDbRServer).query(sql, handler);
  if(!count) {
    MCE_WARN("SimpleCounterProducer::Create() key miss, key= " << key);
    //由于user_login_count有的id没有记录，对没有的DB记录的查询暂时默认返回0而不是空
    result="0";
  }
  value->reserve(result.size());
  value->insert(value->begin(), result.begin(), result.end());
  return true;
}

}}
