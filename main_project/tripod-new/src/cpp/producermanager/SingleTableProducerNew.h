#ifndef __PRODUCERMANAGER_SINGLETABLEPRODUCER_H__
#define __PRODUCERMANAGER_SINGLETABLEPRODUCER_H__


#include <iostream>
#include "TripodProducer.h"
#include <QueryRunner.h>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <IceUtil/IceUtil.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/dynamic_message.h>
#include "tripod_options.pb.h"


namespace com {
namespace renren {
namespace tripod {

using com::xiaonei::xce::Statement;
using com::xiaonei::xce::QueryRunner;
using com::xiaonei::xce::CDbWServer;
using com::xiaonei::xce::CDbRServer;
using google::protobuf::Message;
using google::protobuf::FieldDescriptor;
using google::protobuf::MessageFactory;
using google::protobuf::Descriptor;
using google::protobuf::DescriptorPool;
using google::protobuf::Reflection;
using google::protobuf::MessageOptions;
using google::protobuf::DynamicMessageFactory;
using google::protobuf::compiler::MultiFileErrorCollector;


class ReflectionUtil
{
public:
    static bool SetField(Message* message, const Reflection* reflection, 
            const FieldDescriptor* descriptor,const mysqlpp::DateTime& value) {

        reflection->SetInt64(message,descriptor,(time_t)value);
        return true;
    }

    static bool SetField(Message* message, const Reflection* reflection, 
            const FieldDescriptor* descriptor,const mysqlpp::String& value) {

        switch(descriptor->cpp_type()) {
            case FieldDescriptor::CPPTYPE_UINT64 : 
                reflection->SetUInt64(message,descriptor,(uint64_t)value);break;
            case FieldDescriptor::CPPTYPE_INT64 :
                reflection->SetInt64(message,descriptor,(int64_t)value);break;
            case FieldDescriptor::CPPTYPE_INT32 :
                reflection->SetInt32(message,descriptor,(int32_t)value);break;
            case FieldDescriptor::CPPTYPE_UINT32 :
                reflection->SetUInt32(message,descriptor,(uint32_t)value);break;
            case FieldDescriptor::TYPE_STRING :
                reflection->SetString(message,descriptor,(std::string)value);break;
            case FieldDescriptor::CPPTYPE_DOUBLE :
                reflection->SetDouble(message,descriptor,(double)value);break;
            case FieldDescriptor::CPPTYPE_FLOAT :
                reflection->SetFloat(message,descriptor,(float)value);break;
            case FieldDescriptor::CPPTYPE_BOOL :
                reflection->SetBool(message,descriptor,(bool)value);break;
            case FieldDescriptor::CPPTYPE_ENUM : 
            case FieldDescriptor::CPPTYPE_MESSAGE :
            default:
                MCE_WARN("[SingleTableProducerNew] not support simpe message include enum && message ");
                return false;
        }
        return false;
    }

static bool FillMessage(Message* message, const mysqlpp::Row& row) { // deal with message type field
    
    const Reflection* reflection = message->GetReflection();
    const Descriptor* descriptor = message->GetDescriptor();

    for(size_t i=0; i < descriptor->field_count(); ++i) {
        const FieldDescriptor* field_descriptor = descriptor->field(i);
        assert(field_descriptor->label()!=FieldDescriptor::LABEL_REPEATED);
        const char* name = field_descriptor->name().c_str();
         
        std::string extension = field_descriptor->options().GetExtension(::tripod::options::mysql_type);
        if (extension == "datetime") {
            SetField(message,reflection,field_descriptor,mysqlpp::DateTime(row[name]));
        } else {
            SetField(message,reflection,field_descriptor,row[name]);
        } 
     }
    return true;
}


static bool FillBasic(Message* message, const FieldDescriptor* field_descriptor ,const mysqlpp::Row& row) { // deal with basic repeated type field 
    
    const Reflection* reflection = message->GetReflection();
    mysqlpp::String value;
    assert(field_descriptor->label()==FieldDescriptor::LABEL_REPEATED);
    const char* name = field_descriptor->name().c_str();
    value = row[name];
    if(!value.empty()){
        switch(field_descriptor->cpp_type()) {
          
            case FieldDescriptor::CPPTYPE_UINT64 :
                reflection->AddUInt64(message,field_descriptor,(uint64_t)value);break;
            case FieldDescriptor::CPPTYPE_INT64 :
                reflection->AddInt64(message,field_descriptor,(int64_t)value);break;
            case FieldDescriptor::CPPTYPE_INT32 :
                reflection->AddInt32(message,field_descriptor,(int32_t)value);break;
            case FieldDescriptor::CPPTYPE_UINT32 :
                reflection->AddUInt32(message,field_descriptor,(uint32_t)value);break;
            case FieldDescriptor::TYPE_STRING :
                reflection->AddString(message,field_descriptor,(std::string)value);break;
            case FieldDescriptor::CPPTYPE_DOUBLE :
                reflection->AddDouble(message,field_descriptor,(double)value);break;
            case FieldDescriptor::CPPTYPE_FLOAT :
                reflection->AddFloat(message,field_descriptor,(float)value);break;
            case FieldDescriptor::CPPTYPE_BOOL :
                reflection->AddBool(message,field_descriptor,(bool)value);break;
            default:
             return false; 
        }
    } 
    return true;
}

};

class CreateKeysResultHandler : virtual public com::xiaonei::xce::ResultHandler {

public:
	CreateKeysResultHandler(std::vector<std::string>& keys, const std::string& primary_key) 
				: keys_(keys), primary_key_(primary_key) {}
protected: 
	virtual bool handle(mysqlpp::Row& row) const {
		keys_.push_back((std::string)row[primary_key_.c_str()]);
		return true;
	} 
    std::vector<std::string>& keys_;
    std::string primary_key_;
};

template<class T>
class CreateValuesResultHandler : virtual public com::xiaonei::xce::ResultHandler {
public:
  CreateValuesResultHandler(DataMap& result,std::string& primary_key, std::string& repeated_field )
			 : result_(result),primary_key_(primary_key),repeated_field_(repeated_field) {}

private:
    void process() const {  // deal with single record
        std::string data;
        google::protobuf::Message* message = (google::protobuf::Message*)new T;
        mysqlpp::Row row = rows_.front();
        bool ok = ReflectionUtil::FillMessage(message,row);
        assert(ok == true);
        message->SerializeToString(&data);
        result_[last_key_] = Data(data.begin(),data.end());
        delete message;
    }

    void processBatch() const { // deal with multiple record which contain 'repeated' segment
        std::string data;
        google::protobuf::Message* message = (google::protobuf::Message*)new T;
        const google::protobuf::Reflection* reflection = message->GetReflection();

        const google::protobuf::FieldDescriptor* key_descriptor = message->GetDescriptor()->FindFieldByName(primary_key_);
        //TODO 
        //current primary key must int32 because current createKeys read from user_passport     
        reflection->SetInt32(message,key_descriptor,(int32_t)atoi(last_key_.c_str()));
       
        const google::protobuf::FieldDescriptor *field_descriptor = message->GetDescriptor()->FindFieldByName(repeated_field_);
        for(std::vector<mysqlpp::Row>::iterator it=rows_.begin();it!=rows_.end();++it){
              if (field_descriptor->cpp_type() == FieldDescriptor::CPPTYPE_MESSAGE) {
                  Message* field_message = reflection->AddMessage(message,field_descriptor);
                  ReflectionUtil::FillMessage(field_message,*it); 
              } else {
                  ReflectionUtil::FillBasic(message,field_descriptor,*it);
              }
        }
        message->SerializeToString(&data);    
        result_[last_key_] = Data(data.begin(),data.end());
        delete message;
    };
protected: 
    virtual bool handle(mysqlpp::Row& row) const {
        std::string key = (string)row[primary_key_.c_str()];

        if(key.empty())
            return true;	

        if(!(last_key_.empty()) && last_key_!=key){
            if(repeated_field_.empty()){
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

    virtual void finish() const { // deal with the last record
        if(last_key_.empty()) { //数据库不存在记录
            return;
        }
        if(repeated_field_.empty()){
            process();
        }else{
            processBatch();
        }
		vector<mysqlpp::Row>().swap(rows_);
    }
private:
  DataMap& result_;
  mutable std::string last_key_;
  mutable std::vector<mysqlpp::Row> rows_;
  std::string& primary_key_;
  std::string& repeated_field_;
};

template<class T>
class SingleTableProducer : public TripodProducer {

public :
    SingleTableProducer() {

        // get the options in the .proto file
        desc_ = T::descriptor()->options().GetExtension(::tripod::options::database_source);
        table_ = T::descriptor()->options().GetExtension(::tripod::options::table_name);
        fields_ = T::descriptor()->options().GetExtension(::tripod::options::select_fields);

        //!!current not support
        //mod_ = T::descriptor()->options().GetExtension(::tripod::options::table_mod);
        key_ = T::descriptor()->options().GetExtension(::tripod::options::primary_key);
        repeated_field_ = T::descriptor()->options().GetExtension(::tripod::options::repeated_field);
        if(!repeated_field_.empty()){
            assert(T::descriptor()->FindFieldByName(repeated_field_)->is_repeated());
        }
        MCE_INFO("[SingleTableProducerNew] read from  proto file datasource:"<<desc_ <<" table:"<<table_<<"table_:"<<" select_field:"<<fields_<<" key:"<<key_ );
    }

    virtual ~SingleTableProducer() {}
       virtual DataMap create(const KeySeq& keys, bool isMaster) {
           MCE_DEBUG("[SingleTableProducerNew::create] begin keys.size:" << keys.size());
           std::ostringstream oss;
           for (std::vector<std::string>::const_iterator iter = keys.begin(); iter != keys.end(); ++iter) {
               if (iter != keys.begin()) {
                   oss << ",";
               }
               oss << *iter;
           }

           DataMap result;
           Statement sql;
           sql << "select " << fields_ << " from " << table_ << " where "<<key_<<" in (" << oss.str() << ")";
           CreateValuesResultHandler<T> handler(result, key_, repeated_field_);

           QueryRunner(desc_, isMaster ? CDbWServer: CDbRServer).query(sql, handler);

           return result;
    }


     virtual KeySeq createHotKeys(const std::string& beginKey, int limit) {
    	return createKeys(beginKey,limit);
     }
     virtual KeySeq createKeys(const std::string& beginKey, int limit) {
         MCE_DEBUG("[SingleTableProducer::createKeys] begin beginKey:" << beginKey << " limit:" << limit);
         std::vector<std::string> userKeys;
         Statement sql;
         sql << "select id from user_passport where id >= " << mysqlpp::quote << beginKey << " limit " << limit;
         CreateKeysResultHandler handler(userKeys, "id");
         QueryRunner("user_passport", CDbRServer).query(sql, handler);
         return userKeys;
 
     }

private:
		std::string desc_;
		std::string table_;
		std::string fields_;
        std::string key_;
        int mod_;
        std::string repeated_field_;
};


}
}
}
#endif
