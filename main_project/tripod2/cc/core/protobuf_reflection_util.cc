#include "tripod2/cc/core/protobuf_reflection_util.h"

namespace xce {
namespace tripod {

bool ProtobufReflectionUtil::SetField(google::protobuf::Message* message,
    const google::protobuf::Reflection* reflection, 
    const google::protobuf::FieldDescriptor* descriptor,
    const mysqlpp::DateTime& value) {
  using namespace google::protobuf;
  reflection->SetInt64(message,descriptor,(time_t)value);
  return true;
}

bool ProtobufReflectionUtil::SetField(google::protobuf::Message* message,
    const google::protobuf::Reflection* reflection, 
    const google::protobuf::FieldDescriptor* descriptor,
    const mysqlpp::String& value) {
  using namespace google::protobuf;
  switch(descriptor->cpp_type()) {
    case FieldDescriptor::CPPTYPE_UINT64 : 
      reflection->SetUInt64(message,descriptor,(value.is_null() ? 0 : (uint64_t)value));break;
    case FieldDescriptor::CPPTYPE_INT64 :
      reflection->SetInt64(message,descriptor,(value.is_null() ? 0 : (int64_t)value));break;
    case FieldDescriptor::CPPTYPE_INT32 :
      reflection->SetInt32(message,descriptor,(value.is_null() ? 0 : (int32_t)value));break;
    case FieldDescriptor::CPPTYPE_UINT32 :
      reflection->SetUInt32(message,descriptor,(value.is_null() ? 0 : (uint32_t)value));break;
    case FieldDescriptor::TYPE_STRING :
      reflection->SetString(message,descriptor,(value.is_null() ? "" : (std::string)value));break;
    case FieldDescriptor::CPPTYPE_DOUBLE :
      reflection->SetDouble(message,descriptor,(value.is_null() ? 0 : (double)value));break;
    case FieldDescriptor::CPPTYPE_FLOAT :
      reflection->SetFloat(message,descriptor,(value.is_null() ? 0 : (float)value));break;
    case FieldDescriptor::CPPTYPE_BOOL :
      reflection->SetBool(message,descriptor,(value.is_null() ? 0 : (bool)value));break;
    case FieldDescriptor::CPPTYPE_ENUM : 
    case FieldDescriptor::CPPTYPE_MESSAGE :
    default:
      MCE_WARN("[SingleTableProducerNew] not support simpe message include enum && message ");
      return false;
  }
  return false;
}

bool ProtobufReflectionUtil::FillMessage(google::protobuf::Message* message,
    const mysqlpp::Row& row) {
  // deal with message type field
  using namespace google::protobuf;
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


bool ProtobufReflectionUtil::FillBasic(google::protobuf::Message* message,
    const google::protobuf::FieldDescriptor* field_descriptor, 
    const mysqlpp::Row& row) { // deal with basic repeated type field 
  using namespace google::protobuf;
  const Reflection* reflection = message->GetReflection();
  mysqlpp::String value;
  assert(field_descriptor->label()==FieldDescriptor::LABEL_REPEATED);
  const char* name = field_descriptor->name().c_str();
  value = row[name];
  if(!value.empty()){
    switch(field_descriptor->cpp_type()) {
      case FieldDescriptor::CPPTYPE_UINT64 :
        reflection->AddUInt64(message,field_descriptor,(value.is_null() ? 0 : (uint64_t)value));break;
      case FieldDescriptor::CPPTYPE_INT64 :
        reflection->AddInt64(message,field_descriptor,(value.is_null() ? 0 : (int64_t)value));break;
      case FieldDescriptor::CPPTYPE_INT32 :
        reflection->AddInt32(message,field_descriptor,(value.is_null() ? 0 : (int32_t)value));break;
      case FieldDescriptor::CPPTYPE_UINT32 :
        reflection->AddUInt32(message,field_descriptor,(value.is_null() ? 0 : (uint32_t)value));break;
      case FieldDescriptor::TYPE_STRING :
        reflection->AddString(message,field_descriptor,(value.is_null() ? "" : (std::string)value));break;
      case FieldDescriptor::CPPTYPE_DOUBLE :
        reflection->AddDouble(message,field_descriptor,(value.is_null() ? 0 : (double)value));break;
      case FieldDescriptor::CPPTYPE_FLOAT :
        reflection->AddFloat(message,field_descriptor,(value.is_null() ? 0 : (float)value));break;
      case FieldDescriptor::CPPTYPE_BOOL :
        reflection->AddBool(message,field_descriptor,(value.is_null() ? 0 : (bool)value));break;
      default:
        return false; 
    }
  } 
  return true;
}

}
}// xce::tripod

