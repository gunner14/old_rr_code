// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: user_profile.proto

#ifndef PROTOBUF_user_5fprofile_2eproto__INCLUDED
#define PROTOBUF_user_5fprofile_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2003000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2003000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
// @@protoc_insertion_point(includes)

namespace xce {
namespace ad {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_user_5fprofile_2eproto();
void protobuf_AssignDesc_user_5fprofile_2eproto();
void protobuf_ShutdownFile_user_5fprofile_2eproto();

class UserProfile;

// ===================================================================

class UserProfile : public ::google::protobuf::Message {
 public:
  UserProfile();
  virtual ~UserProfile();
  
  UserProfile(const UserProfile& from);
  
  inline UserProfile& operator=(const UserProfile& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const UserProfile& default_instance();
  
  void Swap(UserProfile* other);
  
  // implements Message ----------------------------------------------
  
  UserProfile* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const UserProfile& from);
  void MergeFrom(const UserProfile& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required int32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);
  
  // required int32 stage = 2;
  inline bool has_stage() const;
  inline void clear_stage();
  static const int kStageFieldNumber = 2;
  inline ::google::protobuf::int32 stage() const;
  inline void set_stage(::google::protobuf::int32 value);
  
  // required int32 gender = 3;
  inline bool has_gender() const;
  inline void clear_gender();
  static const int kGenderFieldNumber = 3;
  inline ::google::protobuf::int32 gender() const;
  inline void set_gender(::google::protobuf::int32 value);
  
  // required int32 age = 4;
  inline bool has_age() const;
  inline void clear_age();
  static const int kAgeFieldNumber = 4;
  inline ::google::protobuf::int32 age() const;
  inline void set_age(::google::protobuf::int32 value);
  
  // required int64 school = 5;
  inline bool has_school() const;
  inline void clear_school();
  static const int kSchoolFieldNumber = 5;
  inline ::google::protobuf::int64 school() const;
  inline void set_school(::google::protobuf::int64 value);
  
  // required int32 major = 6;
  inline bool has_major() const;
  inline void clear_major();
  static const int kMajorFieldNumber = 6;
  inline ::google::protobuf::int32 major() const;
  inline void set_major(::google::protobuf::int32 value);
  
  // required int32 grade = 7;
  inline bool has_grade() const;
  inline void clear_grade();
  static const int kGradeFieldNumber = 7;
  inline ::google::protobuf::int32 grade() const;
  inline void set_grade(::google::protobuf::int32 value);
  
  // required string home_area = 8;
  inline bool has_home_area() const;
  inline void clear_home_area();
  static const int kHomeAreaFieldNumber = 8;
  inline const ::std::string& home_area() const;
  inline void set_home_area(const ::std::string& value);
  inline void set_home_area(const char* value);
  inline void set_home_area(const char* value, size_t size);
  inline ::std::string* mutable_home_area();
  
  // required string current_area = 9;
  inline bool has_current_area() const;
  inline void clear_current_area();
  static const int kCurrentAreaFieldNumber = 9;
  inline const ::std::string& current_area() const;
  inline void set_current_area(const ::std::string& value);
  inline void set_current_area(const char* value);
  inline void set_current_area(const char* value, size_t size);
  inline ::std::string* mutable_current_area();
  
  // required int32 ip = 10;
  inline bool has_ip() const;
  inline void clear_ip();
  static const int kIpFieldNumber = 10;
  inline ::google::protobuf::int32 ip() const;
  inline void set_ip(::google::protobuf::int32 value);
  
  // required string ip_area = 11;
  inline bool has_ip_area() const;
  inline void clear_ip_area();
  static const int kIpAreaFieldNumber = 11;
  inline const ::std::string& ip_area() const;
  inline void set_ip_area(const ::std::string& value);
  inline void set_ip_area(const char* value);
  inline void set_ip_area(const char* value, size_t size);
  inline ::std::string* mutable_ip_area();
  
  // optional string work_place = 12;
  inline bool has_work_place() const;
  inline void clear_work_place();
  static const int kWorkPlaceFieldNumber = 12;
  inline const ::std::string& work_place() const;
  inline void set_work_place(const ::std::string& value);
  inline void set_work_place(const char* value);
  inline void set_work_place(const char* value, size_t size);
  inline ::std::string* mutable_work_place();
  
  // @@protoc_insertion_point(class_scope:xce.ad.UserProfile)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::int32 id_;
  ::google::protobuf::int32 stage_;
  ::google::protobuf::int32 gender_;
  ::google::protobuf::int32 age_;
  ::google::protobuf::int64 school_;
  ::google::protobuf::int32 major_;
  ::google::protobuf::int32 grade_;
  ::std::string* home_area_;
  static const ::std::string _default_home_area_;
  ::std::string* current_area_;
  static const ::std::string _default_current_area_;
  ::google::protobuf::int32 ip_;
  ::std::string* ip_area_;
  static const ::std::string _default_ip_area_;
  ::std::string* work_place_;
  static const ::std::string _default_work_place_;
  friend void  protobuf_AddDesc_user_5fprofile_2eproto();
  friend void protobuf_AssignDesc_user_5fprofile_2eproto();
  friend void protobuf_ShutdownFile_user_5fprofile_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(12 + 31) / 32];
  
  // WHY DOES & HAVE LOWER PRECEDENCE THAN != !?
  inline bool _has_bit(int index) const {
    return (_has_bits_[index / 32] & (1u << (index % 32))) != 0;
  }
  inline void _set_bit(int index) {
    _has_bits_[index / 32] |= (1u << (index % 32));
  }
  inline void _clear_bit(int index) {
    _has_bits_[index / 32] &= ~(1u << (index % 32));
  }
  
  void InitAsDefaultInstance();
  static UserProfile* default_instance_;
};
// ===================================================================


// ===================================================================

// UserProfile

// required int32 id = 1;
inline bool UserProfile::has_id() const {
  return _has_bit(0);
}
inline void UserProfile::clear_id() {
  id_ = 0;
  _clear_bit(0);
}
inline ::google::protobuf::int32 UserProfile::id() const {
  return id_;
}
inline void UserProfile::set_id(::google::protobuf::int32 value) {
  _set_bit(0);
  id_ = value;
}

// required int32 stage = 2;
inline bool UserProfile::has_stage() const {
  return _has_bit(1);
}
inline void UserProfile::clear_stage() {
  stage_ = 0;
  _clear_bit(1);
}
inline ::google::protobuf::int32 UserProfile::stage() const {
  return stage_;
}
inline void UserProfile::set_stage(::google::protobuf::int32 value) {
  _set_bit(1);
  stage_ = value;
}

// required int32 gender = 3;
inline bool UserProfile::has_gender() const {
  return _has_bit(2);
}
inline void UserProfile::clear_gender() {
  gender_ = 0;
  _clear_bit(2);
}
inline ::google::protobuf::int32 UserProfile::gender() const {
  return gender_;
}
inline void UserProfile::set_gender(::google::protobuf::int32 value) {
  _set_bit(2);
  gender_ = value;
}

// required int32 age = 4;
inline bool UserProfile::has_age() const {
  return _has_bit(3);
}
inline void UserProfile::clear_age() {
  age_ = 0;
  _clear_bit(3);
}
inline ::google::protobuf::int32 UserProfile::age() const {
  return age_;
}
inline void UserProfile::set_age(::google::protobuf::int32 value) {
  _set_bit(3);
  age_ = value;
}

// required int64 school = 5;
inline bool UserProfile::has_school() const {
  return _has_bit(4);
}
inline void UserProfile::clear_school() {
  school_ = GOOGLE_LONGLONG(0);
  _clear_bit(4);
}
inline ::google::protobuf::int64 UserProfile::school() const {
  return school_;
}
inline void UserProfile::set_school(::google::protobuf::int64 value) {
  _set_bit(4);
  school_ = value;
}

// required int32 major = 6;
inline bool UserProfile::has_major() const {
  return _has_bit(5);
}
inline void UserProfile::clear_major() {
  major_ = 0;
  _clear_bit(5);
}
inline ::google::protobuf::int32 UserProfile::major() const {
  return major_;
}
inline void UserProfile::set_major(::google::protobuf::int32 value) {
  _set_bit(5);
  major_ = value;
}

// required int32 grade = 7;
inline bool UserProfile::has_grade() const {
  return _has_bit(6);
}
inline void UserProfile::clear_grade() {
  grade_ = 0;
  _clear_bit(6);
}
inline ::google::protobuf::int32 UserProfile::grade() const {
  return grade_;
}
inline void UserProfile::set_grade(::google::protobuf::int32 value) {
  _set_bit(6);
  grade_ = value;
}

// required string home_area = 8;
inline bool UserProfile::has_home_area() const {
  return _has_bit(7);
}
inline void UserProfile::clear_home_area() {
  if (home_area_ != &_default_home_area_) {
    home_area_->clear();
  }
  _clear_bit(7);
}
inline const ::std::string& UserProfile::home_area() const {
  return *home_area_;
}
inline void UserProfile::set_home_area(const ::std::string& value) {
  _set_bit(7);
  if (home_area_ == &_default_home_area_) {
    home_area_ = new ::std::string;
  }
  home_area_->assign(value);
}
inline void UserProfile::set_home_area(const char* value) {
  _set_bit(7);
  if (home_area_ == &_default_home_area_) {
    home_area_ = new ::std::string;
  }
  home_area_->assign(value);
}
inline void UserProfile::set_home_area(const char* value, size_t size) {
  _set_bit(7);
  if (home_area_ == &_default_home_area_) {
    home_area_ = new ::std::string;
  }
  home_area_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* UserProfile::mutable_home_area() {
  _set_bit(7);
  if (home_area_ == &_default_home_area_) {
    home_area_ = new ::std::string;
  }
  return home_area_;
}

// required string current_area = 9;
inline bool UserProfile::has_current_area() const {
  return _has_bit(8);
}
inline void UserProfile::clear_current_area() {
  if (current_area_ != &_default_current_area_) {
    current_area_->clear();
  }
  _clear_bit(8);
}
inline const ::std::string& UserProfile::current_area() const {
  return *current_area_;
}
inline void UserProfile::set_current_area(const ::std::string& value) {
  _set_bit(8);
  if (current_area_ == &_default_current_area_) {
    current_area_ = new ::std::string;
  }
  current_area_->assign(value);
}
inline void UserProfile::set_current_area(const char* value) {
  _set_bit(8);
  if (current_area_ == &_default_current_area_) {
    current_area_ = new ::std::string;
  }
  current_area_->assign(value);
}
inline void UserProfile::set_current_area(const char* value, size_t size) {
  _set_bit(8);
  if (current_area_ == &_default_current_area_) {
    current_area_ = new ::std::string;
  }
  current_area_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* UserProfile::mutable_current_area() {
  _set_bit(8);
  if (current_area_ == &_default_current_area_) {
    current_area_ = new ::std::string;
  }
  return current_area_;
}

// required int32 ip = 10;
inline bool UserProfile::has_ip() const {
  return _has_bit(9);
}
inline void UserProfile::clear_ip() {
  ip_ = 0;
  _clear_bit(9);
}
inline ::google::protobuf::int32 UserProfile::ip() const {
  return ip_;
}
inline void UserProfile::set_ip(::google::protobuf::int32 value) {
  _set_bit(9);
  ip_ = value;
}

// required string ip_area = 11;
inline bool UserProfile::has_ip_area() const {
  return _has_bit(10);
}
inline void UserProfile::clear_ip_area() {
  if (ip_area_ != &_default_ip_area_) {
    ip_area_->clear();
  }
  _clear_bit(10);
}
inline const ::std::string& UserProfile::ip_area() const {
  return *ip_area_;
}
inline void UserProfile::set_ip_area(const ::std::string& value) {
  _set_bit(10);
  if (ip_area_ == &_default_ip_area_) {
    ip_area_ = new ::std::string;
  }
  ip_area_->assign(value);
}
inline void UserProfile::set_ip_area(const char* value) {
  _set_bit(10);
  if (ip_area_ == &_default_ip_area_) {
    ip_area_ = new ::std::string;
  }
  ip_area_->assign(value);
}
inline void UserProfile::set_ip_area(const char* value, size_t size) {
  _set_bit(10);
  if (ip_area_ == &_default_ip_area_) {
    ip_area_ = new ::std::string;
  }
  ip_area_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* UserProfile::mutable_ip_area() {
  _set_bit(10);
  if (ip_area_ == &_default_ip_area_) {
    ip_area_ = new ::std::string;
  }
  return ip_area_;
}

// optional string work_place = 12;
inline bool UserProfile::has_work_place() const {
  return _has_bit(11);
}
inline void UserProfile::clear_work_place() {
  if (work_place_ != &_default_work_place_) {
    work_place_->clear();
  }
  _clear_bit(11);
}
inline const ::std::string& UserProfile::work_place() const {
  return *work_place_;
}
inline void UserProfile::set_work_place(const ::std::string& value) {
  _set_bit(11);
  if (work_place_ == &_default_work_place_) {
    work_place_ = new ::std::string;
  }
  work_place_->assign(value);
}
inline void UserProfile::set_work_place(const char* value) {
  _set_bit(11);
  if (work_place_ == &_default_work_place_) {
    work_place_ = new ::std::string;
  }
  work_place_->assign(value);
}
inline void UserProfile::set_work_place(const char* value, size_t size) {
  _set_bit(11);
  if (work_place_ == &_default_work_place_) {
    work_place_ = new ::std::string;
  }
  work_place_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* UserProfile::mutable_work_place() {
  _set_bit(11);
  if (work_place_ == &_default_work_place_) {
    work_place_ = new ::std::string;
  }
  return work_place_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace ad
}  // namespace xce

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_user_5fprofile_2eproto__INCLUDED
