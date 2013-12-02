#ifndef SINGLE_TABLE_PRODUCER_H_
#define SINGLE_TABLE_PRODUCER_H_

#include "OceProto/proto/tripod_options.pb.h"
#include "tripod2/cc/core/producer.h"
#include "tripod2/cc/core/protobuf_reflection_util.h"
#include "DbCxxPool/src/QueryRunner.h"

namespace xce {
namespace tripod {

std::string AssembleKeysForSql(const StringListType& keys);
std::map<int, StringListType> SeperateKeysByMod(const StringListType& keys, int mod);

/*
 * single table producer
 */
template<typename T>
class SingleTableProducer : public Producer {
 public:
  SingleTableProducer();
  virtual ~SingleTableProducer() {};
  std::map<std::string, bool> Create(const StringListType& keys, std::map<std::string,ByteSeq>* values, bool use_master);
  bool Create(const std::string& key, ByteSeq* value, bool use_master); 
  bool CreateKeys(const std::string& key,int limit, StringListType* keyseq);
 private:
  void ExecuteQuery(const std::string& table_name, const std::string& pattern, const StringListType& keys, 
      std::map<std::string, bool>* result, std::map<std::string, ByteSeq>* values, bool use_master);
  std::string desc_;
  std::string table_;
  std::string fields_;
  std::string key_;
  std::string pattern_prefix_;
  int mod_;
  std::string repeated_field_;
};

/*
 * create values handler
 */
template<class T>
class CreateValuesResultHandler: virtual public com::xiaonei::xce::ResultHandler {
 public:
  CreateValuesResultHandler(const std::string& primary_key, const std::string& repeated_field, const StringListType& keys,
      std::map<std::string, ByteSeq>* values, std::map<std::string, bool>* result)
          :primary_key_(primary_key),repeated_field_(repeated_field),keys_(keys),
           result_(result),values_(values) {
  }
 protected: 
  virtual bool handle(mysqlpp::Row& row) const;
  virtual void finish() const;
 private:
  void process() const;
  void processBatch() const; 
  vector<unsigned char> processdbempty() const;
  mutable std::string last_key_;
  mutable std::vector<mysqlpp::Row> rows_;
  const std::string& primary_key_;
  const std::string& repeated_field_;
  const StringListType& keys_;
  std::map<std::string, bool>* result_;
  std::map<std::string, ByteSeq>* values_;
};

/*
 * create keys handler
 */
class CreateKeysResultHandler: virtual public com::xiaonei::xce::ResultHandler {
 public:
  CreateKeysResultHandler(const std::string& primary_key, StringListType* keyseq) 
      :primary_key_(primary_key), keyseq_(keyseq) {}
 protected: 
  virtual bool handle(mysqlpp::Row& row) const {
    keyseq_->push_back( (std::string)row[primary_key_.c_str()] );
    return true;
  }
 private: 
  const std::string& primary_key_;
  StringListType* keyseq_;
};

template<typename T>
SingleTableProducer<T>::SingleTableProducer() {
  desc_ = T::descriptor()->options().GetExtension(::tripod::options::database_source);
  table_ = T::descriptor()->options().GetExtension(::tripod::options::table_name);
  fields_ = T::descriptor()->options().GetExtension(::tripod::options::select_fields);
  pattern_prefix_ = T::descriptor()->options().GetExtension(::tripod::options::pattern_prefix);
  mod_ = T::descriptor()->options().GetExtension(::tripod::options::table_mod);
  key_ = T::descriptor()->options().GetExtension(::tripod::options::primary_key);
  repeated_field_ = T::descriptor()->options().GetExtension(::tripod::options::repeated_field);
  if(!repeated_field_.empty()){
  assert(T::descriptor()->FindFieldByName(repeated_field_)->is_repeated());
  }
  MCE_INFO("[SingleTableProducer] read from  proto file datasource:"<< desc_
      << " table:" << table_ << " table_:" << " select_field:" << fields_ << " key:" << key_ );
}

template<typename T>
void SingleTableProducer<T>::ExecuteQuery(const std::string& table_name,
    const std::string& pattern, const StringListType& keys,
    std::map<std::string, bool>* result, std::map<std::string, ByteSeq>* values, 
    bool use_master) {
  using namespace com::xiaonei::xce;
  std::string assembled_keys = AssembleKeysForSql(keys);
  Statement sql;
  sql << "SELECT " << fields_ << " FROM " << table_name << " WHERE " << key_ << " IN " << assembled_keys;
  CreateValuesResultHandler<T> handler(key_, repeated_field_, keys, values, result);
  QueryRunner(desc_, use_master?CDbWServer:CDbRServer, pattern).query(sql, handler);
}

template<typename T>
std::map<std::string, bool> SingleTableProducer<T>::Create(const StringListType& keys,
    std::map<std::string, ByteSeq>* values,
    bool use_master) {
  using namespace com::xiaonei::xce;
  MCE_DEBUG("[SingleTableProducer::create] begin keys.size:" << keys.size());
  std::map<std::string, bool> result;
  BOOST_FOREACH(const std::string& key, keys) {
    result.insert(std::make_pair(key, false));
  }
  if ( mod_==1 ) {
    ExecuteQuery(table_, pattern_prefix_, keys, &result, values, use_master);
  } else {
    std::map<int,StringListType> keys_mod_map = SeperateKeysByMod(keys, mod_);
    std::map<int, StringListType>::const_iterator iter = keys_mod_map.begin();
    for(; iter != keys_mod_map.end(); ++iter) {
      std::string table_name = table_ + boost::lexical_cast<std::string>(iter->first);
      std::string pattern = pattern_prefix_ + boost::lexical_cast<std::string>(iter->first);
      ExecuteQuery(table_name, pattern, keys, &result, values, use_master);
    }
  }
  return result;
}

template<typename T>
bool SingleTableProducer<T>::Create(const std::string& key, ByteSeq* value, bool use_master) {
  StringListType keys;
  std::map<std::string, ByteSeq> values; 
  keys.push_back(key);
  std::map<std::string, bool> result = Create(keys, &values, use_master);
  (*value) = values[key];
  return result[key];
}

template<typename T>
bool SingleTableProducer<T>::CreateKeys(const std::string& key,int limit, StringListType* keyseq) {
  using namespace com::xiaonei::xce;
  MCE_DEBUG("[SingleTableProducer::createKeys] begin beginKey:" << key << " limit:" << limit);
  Statement sql;
  sql << "SELECT id FROM user_passport WHERE id >= " << mysqlpp::quote << key << " LIMIT " << limit;
  CreateKeysResultHandler handler("id",keyseq);
  QueryRunner("user_passport", CDbRServer).query(sql, handler);
  if(!keyseq->size()) {
    return false;
  }
  return true;
}


template<typename T>
bool CreateValuesResultHandler<T>::handle(mysqlpp::Row& row) const {
  std::string key = (string)row[primary_key_.c_str()];
  if( key.empty() ) {
    return true;	
  }
  if( !(last_key_.empty()) && (last_key_!=key) ) {
    if( repeated_field_.empty() ) {
      process();
    } else {
      processBatch();    
    }
    vector<mysqlpp::Row>().swap(rows_);
  }
  last_key_ = key;
  rows_.push_back(row);
  return true;
}

template<typename T>
void CreateValuesResultHandler<T>::finish() const {
  if( !last_key_.empty() ) { 
    if( repeated_field_.empty() ) {
      process();
    } else {
      processBatch();
    }
  }
  vector<mysqlpp::Row>().swap(rows_);

  StringListType::const_iterator it = keys_.begin();
  for(; it != keys_.end(); ++it) {
    if(values_->find(*it) == values_->end()) {
      //对于数据库中没有的key，返回一条空结构占位符
      values_->insert(make_pair(*it, processdbempty()));
      (*result_)[*it] = true;
    }
  }
} 

template<typename T>
void CreateValuesResultHandler<T>::process() const {
  using namespace google::protobuf;
  google::protobuf::Message* message = (google::protobuf::Message*)new T;
  mysqlpp::Row row = rows_.front();
  bool ok = ProtobufReflectionUtil::FillMessage(message,row);
  assert(ok == true);
  std::string data;
  message->SerializeToString(&data);
  vector<unsigned char> bytedata(data.begin(),data.end());
  values_->insert(std::make_pair(last_key_, bytedata));
  (*result_)[last_key_] = true;
  delete message;
}

template<typename T>
void CreateValuesResultHandler<T>::processBatch() const {
  using namespace google::protobuf;
  google::protobuf::Message* message = (google::protobuf::Message*)new T;
  const google::protobuf::Reflection* reflection = message->GetReflection();
  const google::protobuf::FieldDescriptor* key_descriptor = message->GetDescriptor()->FindFieldByName(primary_key_);
  switch(key_descriptor->cpp_type()) {
    case FieldDescriptor::CPPTYPE_INT64 :
      reflection->SetInt64(message,key_descriptor,boost::lexical_cast<int64_t>(last_key_));break;
    case FieldDescriptor::CPPTYPE_INT32 :
      reflection->SetInt32(message,key_descriptor,boost::lexical_cast<int32_t>(last_key_));break;
    case FieldDescriptor::TYPE_STRING :
      reflection->SetString(message,key_descriptor,last_key_);break;
    default:
      break;
  }
  const google::protobuf::FieldDescriptor *field_descriptor = message->GetDescriptor()->FindFieldByName(repeated_field_);
  for(std::vector<mysqlpp::Row>::const_iterator it=rows_.begin();it!=rows_.end();++it) {
    if (field_descriptor->cpp_type() == FieldDescriptor::CPPTYPE_MESSAGE) {
      Message* field_message = reflection->AddMessage(message,field_descriptor);
      ProtobufReflectionUtil::FillMessage(field_message,*it); 
    } else {
      ProtobufReflectionUtil::FillBasic(message,field_descriptor,*it);
    }
  }
  std::string data;
  message->SerializeToString(&data);    
  vector<unsigned char> bytedata(data.begin(),data.end());
  values_->insert(std::make_pair(last_key_, bytedata));
  (*result_)[last_key_] = true;
  delete message;
}

//处理数据库中没有的key
template<typename T>
vector<unsigned char> CreateValuesResultHandler<T>::processdbempty() const {
  using namespace google::protobuf;
  google::protobuf::Message* message = (google::protobuf::Message*)new T( T::default_instance() );
  const google::protobuf::Reflection* reflection = message->GetReflection();
  const google::protobuf::FieldDescriptor* key_descriptor = message->GetDescriptor()->FindFieldByName(primary_key_);
  switch(key_descriptor->cpp_type()) {
    case FieldDescriptor::CPPTYPE_INT64 :
      reflection->SetInt64(message,key_descriptor,0);break;
    case FieldDescriptor::CPPTYPE_INT32 :
      reflection->SetInt32(message,key_descriptor,0);break;
    case FieldDescriptor::TYPE_STRING :
      reflection->SetString(message,key_descriptor,"0");break;
    default:
      break;
  }
  std::string data;
  message->SerializeToString(&data);    
  delete message;
  return vector<unsigned char>(data.begin(),data.end());
}

}}//namespace

#endif
