// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "user_profile.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace xce {
namespace ad {

namespace {

const ::google::protobuf::Descriptor* UserProfile_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  UserProfile_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_user_5fprofile_2eproto() {
  protobuf_AddDesc_user_5fprofile_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "user_profile.proto");
  GOOGLE_CHECK(file != NULL);
  UserProfile_descriptor_ = file->message_type(0);
  static const int UserProfile_offsets_[12] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UserProfile, id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UserProfile, stage_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UserProfile, gender_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UserProfile, age_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UserProfile, school_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UserProfile, major_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UserProfile, grade_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UserProfile, home_area_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UserProfile, current_area_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UserProfile, ip_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UserProfile, ip_area_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UserProfile, work_place_),
  };
  UserProfile_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      UserProfile_descriptor_,
      UserProfile::default_instance_,
      UserProfile_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UserProfile, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UserProfile, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(UserProfile));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_user_5fprofile_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    UserProfile_descriptor_, &UserProfile::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_user_5fprofile_2eproto() {
  delete UserProfile::default_instance_;
  delete UserProfile_reflection_;
}

void protobuf_AddDesc_user_5fprofile_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\022user_profile.proto\022\006xce.ad\"\315\001\n\013UserPro"
    "file\022\n\n\002id\030\001 \002(\005\022\r\n\005stage\030\002 \002(\005\022\016\n\006gende"
    "r\030\003 \002(\005\022\013\n\003age\030\004 \002(\005\022\016\n\006school\030\005 \002(\003\022\r\n\005"
    "major\030\006 \002(\005\022\r\n\005grade\030\007 \002(\005\022\021\n\thome_area\030"
    "\010 \002(\t\022\024\n\014current_area\030\t \002(\t\022\n\n\002ip\030\n \002(\005\022"
    "\017\n\007ip_area\030\013 \002(\t\022\022\n\nwork_place\030\014 \001(\t", 236);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "user_profile.proto", &protobuf_RegisterTypes);
  UserProfile::default_instance_ = new UserProfile();
  UserProfile::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_user_5fprofile_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_user_5fprofile_2eproto {
  StaticDescriptorInitializer_user_5fprofile_2eproto() {
    protobuf_AddDesc_user_5fprofile_2eproto();
  }
} static_descriptor_initializer_user_5fprofile_2eproto_;


// ===================================================================

const ::std::string UserProfile::_default_home_area_;
const ::std::string UserProfile::_default_current_area_;
const ::std::string UserProfile::_default_ip_area_;
const ::std::string UserProfile::_default_work_place_;
#ifndef _MSC_VER
const int UserProfile::kIdFieldNumber;
const int UserProfile::kStageFieldNumber;
const int UserProfile::kGenderFieldNumber;
const int UserProfile::kAgeFieldNumber;
const int UserProfile::kSchoolFieldNumber;
const int UserProfile::kMajorFieldNumber;
const int UserProfile::kGradeFieldNumber;
const int UserProfile::kHomeAreaFieldNumber;
const int UserProfile::kCurrentAreaFieldNumber;
const int UserProfile::kIpFieldNumber;
const int UserProfile::kIpAreaFieldNumber;
const int UserProfile::kWorkPlaceFieldNumber;
#endif  // !_MSC_VER

UserProfile::UserProfile()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void UserProfile::InitAsDefaultInstance() {
}

UserProfile::UserProfile(const UserProfile& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void UserProfile::SharedCtor() {
  _cached_size_ = 0;
  id_ = 0;
  stage_ = 0;
  gender_ = 0;
  age_ = 0;
  school_ = GOOGLE_LONGLONG(0);
  major_ = 0;
  grade_ = 0;
  home_area_ = const_cast< ::std::string*>(&_default_home_area_);
  current_area_ = const_cast< ::std::string*>(&_default_current_area_);
  ip_ = 0;
  ip_area_ = const_cast< ::std::string*>(&_default_ip_area_);
  work_place_ = const_cast< ::std::string*>(&_default_work_place_);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

UserProfile::~UserProfile() {
  SharedDtor();
}

void UserProfile::SharedDtor() {
  if (home_area_ != &_default_home_area_) {
    delete home_area_;
  }
  if (current_area_ != &_default_current_area_) {
    delete current_area_;
  }
  if (ip_area_ != &_default_ip_area_) {
    delete ip_area_;
  }
  if (work_place_ != &_default_work_place_) {
    delete work_place_;
  }
  if (this != default_instance_) {
  }
}

void UserProfile::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* UserProfile::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return UserProfile_descriptor_;
}

const UserProfile& UserProfile::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_user_5fprofile_2eproto();  return *default_instance_;
}

UserProfile* UserProfile::default_instance_ = NULL;

UserProfile* UserProfile::New() const {
  return new UserProfile;
}

void UserProfile::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    id_ = 0;
    stage_ = 0;
    gender_ = 0;
    age_ = 0;
    school_ = GOOGLE_LONGLONG(0);
    major_ = 0;
    grade_ = 0;
    if (_has_bit(7)) {
      if (home_area_ != &_default_home_area_) {
        home_area_->clear();
      }
    }
  }
  if (_has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    if (_has_bit(8)) {
      if (current_area_ != &_default_current_area_) {
        current_area_->clear();
      }
    }
    ip_ = 0;
    if (_has_bit(10)) {
      if (ip_area_ != &_default_ip_area_) {
        ip_area_->clear();
      }
    }
    if (_has_bit(11)) {
      if (work_place_ != &_default_work_place_) {
        work_place_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool UserProfile::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 id = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &id_)));
          _set_bit(0);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_stage;
        break;
      }
      
      // required int32 stage = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_stage:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &stage_)));
          _set_bit(1);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_gender;
        break;
      }
      
      // required int32 gender = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_gender:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &gender_)));
          _set_bit(2);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_age;
        break;
      }
      
      // required int32 age = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_age:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &age_)));
          _set_bit(3);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(40)) goto parse_school;
        break;
      }
      
      // required int64 school = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_school:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &school_)));
          _set_bit(4);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(48)) goto parse_major;
        break;
      }
      
      // required int32 major = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_major:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &major_)));
          _set_bit(5);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(56)) goto parse_grade;
        break;
      }
      
      // required int32 grade = 7;
      case 7: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_grade:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &grade_)));
          _set_bit(6);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(66)) goto parse_home_area;
        break;
      }
      
      // required string home_area = 8;
      case 8: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_home_area:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_home_area()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->home_area().data(), this->home_area().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(74)) goto parse_current_area;
        break;
      }
      
      // required string current_area = 9;
      case 9: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_current_area:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_current_area()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->current_area().data(), this->current_area().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(80)) goto parse_ip;
        break;
      }
      
      // required int32 ip = 10;
      case 10: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_ip:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &ip_)));
          _set_bit(9);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(90)) goto parse_ip_area;
        break;
      }
      
      // required string ip_area = 11;
      case 11: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_ip_area:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_ip_area()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->ip_area().data(), this->ip_area().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(98)) goto parse_work_place;
        break;
      }
      
      // optional string work_place = 12;
      case 12: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_work_place:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_work_place()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->work_place().data(), this->work_place().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void UserProfile::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int32 id = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->id(), output);
  }
  
  // required int32 stage = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->stage(), output);
  }
  
  // required int32 gender = 3;
  if (_has_bit(2)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->gender(), output);
  }
  
  // required int32 age = 4;
  if (_has_bit(3)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(4, this->age(), output);
  }
  
  // required int64 school = 5;
  if (_has_bit(4)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(5, this->school(), output);
  }
  
  // required int32 major = 6;
  if (_has_bit(5)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(6, this->major(), output);
  }
  
  // required int32 grade = 7;
  if (_has_bit(6)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(7, this->grade(), output);
  }
  
  // required string home_area = 8;
  if (_has_bit(7)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->home_area().data(), this->home_area().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      8, this->home_area(), output);
  }
  
  // required string current_area = 9;
  if (_has_bit(8)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->current_area().data(), this->current_area().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      9, this->current_area(), output);
  }
  
  // required int32 ip = 10;
  if (_has_bit(9)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(10, this->ip(), output);
  }
  
  // required string ip_area = 11;
  if (_has_bit(10)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->ip_area().data(), this->ip_area().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      11, this->ip_area(), output);
  }
  
  // optional string work_place = 12;
  if (_has_bit(11)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->work_place().data(), this->work_place().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      12, this->work_place(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* UserProfile::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int32 id = 1;
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->id(), target);
  }
  
  // required int32 stage = 2;
  if (_has_bit(1)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->stage(), target);
  }
  
  // required int32 gender = 3;
  if (_has_bit(2)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->gender(), target);
  }
  
  // required int32 age = 4;
  if (_has_bit(3)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(4, this->age(), target);
  }
  
  // required int64 school = 5;
  if (_has_bit(4)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(5, this->school(), target);
  }
  
  // required int32 major = 6;
  if (_has_bit(5)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(6, this->major(), target);
  }
  
  // required int32 grade = 7;
  if (_has_bit(6)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(7, this->grade(), target);
  }
  
  // required string home_area = 8;
  if (_has_bit(7)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->home_area().data(), this->home_area().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        8, this->home_area(), target);
  }
  
  // required string current_area = 9;
  if (_has_bit(8)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->current_area().data(), this->current_area().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        9, this->current_area(), target);
  }
  
  // required int32 ip = 10;
  if (_has_bit(9)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(10, this->ip(), target);
  }
  
  // required string ip_area = 11;
  if (_has_bit(10)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->ip_area().data(), this->ip_area().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        11, this->ip_area(), target);
  }
  
  // optional string work_place = 12;
  if (_has_bit(11)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->work_place().data(), this->work_place().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        12, this->work_place(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int UserProfile::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 id = 1;
    if (has_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->id());
    }
    
    // required int32 stage = 2;
    if (has_stage()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->stage());
    }
    
    // required int32 gender = 3;
    if (has_gender()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->gender());
    }
    
    // required int32 age = 4;
    if (has_age()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->age());
    }
    
    // required int64 school = 5;
    if (has_school()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->school());
    }
    
    // required int32 major = 6;
    if (has_major()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->major());
    }
    
    // required int32 grade = 7;
    if (has_grade()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->grade());
    }
    
    // required string home_area = 8;
    if (has_home_area()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->home_area());
    }
    
  }
  if (_has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    // required string current_area = 9;
    if (has_current_area()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->current_area());
    }
    
    // required int32 ip = 10;
    if (has_ip()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->ip());
    }
    
    // required string ip_area = 11;
    if (has_ip_area()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->ip_area());
    }
    
    // optional string work_place = 12;
    if (has_work_place()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->work_place());
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void UserProfile::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const UserProfile* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const UserProfile*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void UserProfile::MergeFrom(const UserProfile& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_id(from.id());
    }
    if (from._has_bit(1)) {
      set_stage(from.stage());
    }
    if (from._has_bit(2)) {
      set_gender(from.gender());
    }
    if (from._has_bit(3)) {
      set_age(from.age());
    }
    if (from._has_bit(4)) {
      set_school(from.school());
    }
    if (from._has_bit(5)) {
      set_major(from.major());
    }
    if (from._has_bit(6)) {
      set_grade(from.grade());
    }
    if (from._has_bit(7)) {
      set_home_area(from.home_area());
    }
  }
  if (from._has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    if (from._has_bit(8)) {
      set_current_area(from.current_area());
    }
    if (from._has_bit(9)) {
      set_ip(from.ip());
    }
    if (from._has_bit(10)) {
      set_ip_area(from.ip_area());
    }
    if (from._has_bit(11)) {
      set_work_place(from.work_place());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void UserProfile::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void UserProfile::CopyFrom(const UserProfile& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool UserProfile::IsInitialized() const {
  if ((_has_bits_[0] & 0x000007ff) != 0x000007ff) return false;
  
  return true;
}

void UserProfile::Swap(UserProfile* other) {
  if (other != this) {
    std::swap(id_, other->id_);
    std::swap(stage_, other->stage_);
    std::swap(gender_, other->gender_);
    std::swap(age_, other->age_);
    std::swap(school_, other->school_);
    std::swap(major_, other->major_);
    std::swap(grade_, other->grade_);
    std::swap(home_area_, other->home_area_);
    std::swap(current_area_, other->current_area_);
    std::swap(ip_, other->ip_);
    std::swap(ip_area_, other->ip_area_);
    std::swap(work_place_, other->work_place_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata UserProfile::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = UserProfile_descriptor_;
  metadata.reflection = UserProfile_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace ad
}  // namespace xce

// @@protoc_insertion_point(global_scope)
