#ifndef PROTOBUF_REFLECTION_UTIL_H_
#define PROTOBUF_REFLECTION_UTIL_H_

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/dynamic_message.h>
#include "DbCxxPool/src/QueryRunner.h"
#include "OceProto/proto/tripod_options.pb.h"

namespace xce {
namespace tripod {

class ProtobufReflectionUtil {
 public:
  static bool FillMessage(google::protobuf::Message* message,const mysqlpp::Row& row);

  static bool FillBasic(google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field_descriptor,const mysqlpp::Row& row);

 private:
  static bool SetField(google::protobuf::Message* message,const google::protobuf::Reflection* reflection,
      const google::protobuf::FieldDescriptor* descriptor,const mysqlpp::DateTime& value);

  static bool SetField(google::protobuf::Message* message,const google::protobuf::Reflection* reflection, 
      const google::protobuf::FieldDescriptor* descriptor,const mysqlpp::String& value); 


};//class ProtobufReflectionUtil

}
}// xce::tripod

#endif
