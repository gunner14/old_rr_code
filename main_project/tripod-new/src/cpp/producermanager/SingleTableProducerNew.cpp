#include "SingleTableProducerNew.h"

namespace com {
namespace renren {
namespace tripod {

using google::protobuf::Message;
using google::protobuf::FieldDescriptor;
using google::protobuf::MessageFactory;
using google::protobuf::Descriptor;
using google::protobuf::DescriptorPool;
using google::protobuf::Reflection;
using google::protobuf::MessageOptions;
using google::protobuf::DynamicMessageFactory;
using google::protobuf::compiler::MultiFileErrorCollector;

/*
class MockErrorCollector : public MultiFileErrorCollector {
 public:        
  MockErrorCollector() {}
  ~MockErrorCollector() {}
                
  std::string text_;
            
  // implements ErrorCollector ---------------------------------------
  void AddError(const std::string& filename, int line, int column,
                const std::string& message) {
            std::cout << "filename "<<filename <<" message "<<message <<std::endl;
  }             
};          
                
MockErrorCollector errorCollector;
google::protobuf::compiler::DiskSourceTree sourceTree;
google::protobuf::compiler::Importer importer(&sourceTree, &errorCollector);
*/
//void ReflectionUtil::init(const std::string& type_name) {
//	type_name_ = type_name;
//    sourceTree.MapPath("", "./"); 
//    importer.Import("user_config.proto");
//}

/*
Message* ReflectionUtil::CreateMessage(const std::string& message_type)
{   
    static DynamicMessageFactory * dynamicMessageFactory=new DynamicMessageFactory();
    Message* message = NULL;
    const Descriptor* descriptor =
        DescriptorPool::generated_pool()->FindMessageTypeByName(message_type);
    if (descriptor)
    {   
        const Message* prototype =
            MessageFactory::generated_factory()->GetPrototype(descriptor);
        if (prototype)
        {
            message = prototype->New();
        }
    }else {
        descriptor = importer.pool()->FindMessageTypeByName(type_name_);
        if(descriptor){
            message = dynamicMessageFactory->GetPrototype(descriptor)->New();
        }else{
            std::cerr << "not found message type " << type_name_ << std::endl;
        }
    }
    assert(message!=NULL);
    return message;
}

*/

// option (tripod_message_options) = "Hello world!";
/*
bool ReflectionUtil::SetField(Message* message, const Reflection* reflection, const FieldDescriptor* descriptor,const mysqlpp::String& value){

    switch(descriptor->cpp_type()){
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
                return false;
    }
    return false;

}
bool ReflectionUtil::FillMessage(Message* message, const mysqlpp::Row& row){
    
    const Reflection* reflection = message->GetReflection();
    const Descriptor* descriptor = message->GetDescriptor();

    for(size_t i=0; i < descriptor->field_count(); ++i){
        const FieldDescriptor* field_descriptor = descriptor->field(i);
        assert(field_descriptor->label()!=FieldDescriptor::LABEL_REPEATED);
        const char* name = field_descriptor->name().c_str();
        if(!SetField(message,reflection,field_descriptor,row[name])){
            return false;
        } 
    }
    return true;
}
*/

}
}
}

