// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: aaa.proto

#ifndef PROTOBUF_aaa_2eproto__INCLUDED
#define PROTOBUF_aaa_2eproto__INCLUDED

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

namespace follower {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_aaa_2eproto();
void protobuf_AssignDesc_aaa_2eproto();
void protobuf_ShutdownFile_aaa_2eproto();

class SyncItem;
class StartSync;
class Snapshot;
class DiffPackage;
class DiffPackage_Row;
class ServerList;

// ===================================================================

class SyncItem : public ::google::protobuf::Message {
 public:
  SyncItem();
  virtual ~SyncItem();
  
  SyncItem(const SyncItem& from);
  
  inline SyncItem& operator=(const SyncItem& from) {
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
  static const SyncItem& default_instance();
  
  void Swap(SyncItem* other);
  
  // implements Message ----------------------------------------------
  
  SyncItem* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const SyncItem& from);
  void MergeFrom(const SyncItem& from);
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
  
  // required string tableName = 1;
  inline bool has_tablename() const;
  inline void clear_tablename();
  static const int kTableNameFieldNumber = 1;
  inline const ::std::string& tablename() const;
  inline void set_tablename(const ::std::string& value);
  inline void set_tablename(const char* value);
  inline void set_tablename(const char* value, size_t size);
  inline ::std::string* mutable_tablename();
  
  // required int64 endTime = 2;
  inline bool has_endtime() const;
  inline void clear_endtime();
  static const int kEndTimeFieldNumber = 2;
  inline ::google::protobuf::int64 endtime() const;
  inline void set_endtime(::google::protobuf::int64 value);
  
  // @@protoc_insertion_point(class_scope:follower.SyncItem)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::std::string* tablename_;
  static const ::std::string _default_tablename_;
  ::google::protobuf::int64 endtime_;
  friend void  protobuf_AddDesc_aaa_2eproto();
  friend void protobuf_AssignDesc_aaa_2eproto();
  friend void protobuf_ShutdownFile_aaa_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
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
  static SyncItem* default_instance_;
};
// -------------------------------------------------------------------

class StartSync : public ::google::protobuf::Message {
 public:
  StartSync();
  virtual ~StartSync();
  
  StartSync(const StartSync& from);
  
  inline StartSync& operator=(const StartSync& from) {
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
  static const StartSync& default_instance();
  
  void Swap(StartSync* other);
  
  // implements Message ----------------------------------------------
  
  StartSync* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const StartSync& from);
  void MergeFrom(const StartSync& from);
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
  
  // repeated .follower.SyncItem items = 1;
  inline int items_size() const;
  inline void clear_items();
  static const int kItemsFieldNumber = 1;
  inline const ::follower::SyncItem& items(int index) const;
  inline ::follower::SyncItem* mutable_items(int index);
  inline ::follower::SyncItem* add_items();
  inline const ::google::protobuf::RepeatedPtrField< ::follower::SyncItem >&
      items() const;
  inline ::google::protobuf::RepeatedPtrField< ::follower::SyncItem >*
      mutable_items();
  
  // @@protoc_insertion_point(class_scope:follower.StartSync)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::RepeatedPtrField< ::follower::SyncItem > items_;
  friend void  protobuf_AddDesc_aaa_2eproto();
  friend void protobuf_AssignDesc_aaa_2eproto();
  friend void protobuf_ShutdownFile_aaa_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
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
  static StartSync* default_instance_;
};
// -------------------------------------------------------------------

class Snapshot : public ::google::protobuf::Message {
 public:
  Snapshot();
  virtual ~Snapshot();
  
  Snapshot(const Snapshot& from);
  
  inline Snapshot& operator=(const Snapshot& from) {
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
  static const Snapshot& default_instance();
  
  void Swap(Snapshot* other);
  
  // implements Message ----------------------------------------------
  
  Snapshot* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Snapshot& from);
  void MergeFrom(const Snapshot& from);
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
  
  // required string tableName = 1;
  inline bool has_tablename() const;
  inline void clear_tablename();
  static const int kTableNameFieldNumber = 1;
  inline const ::std::string& tablename() const;
  inline void set_tablename(const ::std::string& value);
  inline void set_tablename(const char* value);
  inline void set_tablename(const char* value, size_t size);
  inline ::std::string* mutable_tablename();
  
  // required int64 snapShotTime = 2;
  inline bool has_snapshottime() const;
  inline void clear_snapshottime();
  static const int kSnapShotTimeFieldNumber = 2;
  inline ::google::protobuf::int64 snapshottime() const;
  inline void set_snapshottime(::google::protobuf::int64 value);
  
  // required string snapShotPath = 3;
  inline bool has_snapshotpath() const;
  inline void clear_snapshotpath();
  static const int kSnapShotPathFieldNumber = 3;
  inline const ::std::string& snapshotpath() const;
  inline void set_snapshotpath(const ::std::string& value);
  inline void set_snapshotpath(const char* value);
  inline void set_snapshotpath(const char* value, size_t size);
  inline ::std::string* mutable_snapshotpath();
  
  // @@protoc_insertion_point(class_scope:follower.Snapshot)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::std::string* tablename_;
  static const ::std::string _default_tablename_;
  ::google::protobuf::int64 snapshottime_;
  ::std::string* snapshotpath_;
  static const ::std::string _default_snapshotpath_;
  friend void  protobuf_AddDesc_aaa_2eproto();
  friend void protobuf_AssignDesc_aaa_2eproto();
  friend void protobuf_ShutdownFile_aaa_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];
  
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
  static Snapshot* default_instance_;
};
// -------------------------------------------------------------------

class DiffPackage_Row : public ::google::protobuf::Message {
 public:
  DiffPackage_Row();
  virtual ~DiffPackage_Row();
  
  DiffPackage_Row(const DiffPackage_Row& from);
  
  inline DiffPackage_Row& operator=(const DiffPackage_Row& from) {
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
  static const DiffPackage_Row& default_instance();
  
  void Swap(DiffPackage_Row* other);
  
  // implements Message ----------------------------------------------
  
  DiffPackage_Row* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const DiffPackage_Row& from);
  void MergeFrom(const DiffPackage_Row& from);
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
  
  // required string key = 1;
  inline bool has_key() const;
  inline void clear_key();
  static const int kKeyFieldNumber = 1;
  inline const ::std::string& key() const;
  inline void set_key(const ::std::string& value);
  inline void set_key(const char* value);
  inline void set_key(const char* value, size_t size);
  inline ::std::string* mutable_key();
  
  // repeated string values = 2;
  inline int values_size() const;
  inline void clear_values();
  static const int kValuesFieldNumber = 2;
  inline const ::std::string& values(int index) const;
  inline ::std::string* mutable_values(int index);
  inline void set_values(int index, const ::std::string& value);
  inline void set_values(int index, const char* value);
  inline void set_values(int index, const char* value, size_t size);
  inline ::std::string* add_values();
  inline void add_values(const ::std::string& value);
  inline void add_values(const char* value);
  inline void add_values(const char* value, size_t size);
  inline const ::google::protobuf::RepeatedPtrField< ::std::string>& values() const;
  inline ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_values();
  
  // @@protoc_insertion_point(class_scope:follower.DiffPackage.Row)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::std::string* key_;
  static const ::std::string _default_key_;
  ::google::protobuf::RepeatedPtrField< ::std::string> values_;
  friend void  protobuf_AddDesc_aaa_2eproto();
  friend void protobuf_AssignDesc_aaa_2eproto();
  friend void protobuf_ShutdownFile_aaa_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
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
  static DiffPackage_Row* default_instance_;
};
// -------------------------------------------------------------------

class DiffPackage : public ::google::protobuf::Message {
 public:
  DiffPackage();
  virtual ~DiffPackage();
  
  DiffPackage(const DiffPackage& from);
  
  inline DiffPackage& operator=(const DiffPackage& from) {
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
  static const DiffPackage& default_instance();
  
  void Swap(DiffPackage* other);
  
  // implements Message ----------------------------------------------
  
  DiffPackage* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const DiffPackage& from);
  void MergeFrom(const DiffPackage& from);
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
  
  typedef DiffPackage_Row Row;
  
  // accessors -------------------------------------------------------
  
  // required int64 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::int64 id() const;
  inline void set_id(::google::protobuf::int64 value);
  
  // required string tableName = 2;
  inline bool has_tablename() const;
  inline void clear_tablename();
  static const int kTableNameFieldNumber = 2;
  inline const ::std::string& tablename() const;
  inline void set_tablename(const ::std::string& value);
  inline void set_tablename(const char* value);
  inline void set_tablename(const char* value, size_t size);
  inline ::std::string* mutable_tablename();
  
  // required int64 startTime = 3;
  inline bool has_starttime() const;
  inline void clear_starttime();
  static const int kStartTimeFieldNumber = 3;
  inline ::google::protobuf::int64 starttime() const;
  inline void set_starttime(::google::protobuf::int64 value);
  
  // required int64 endTime = 4;
  inline bool has_endtime() const;
  inline void clear_endtime();
  static const int kEndTimeFieldNumber = 4;
  inline ::google::protobuf::int64 endtime() const;
  inline void set_endtime(::google::protobuf::int64 value);
  
  // repeated .follower.DiffPackage.Row data = 5;
  inline int data_size() const;
  inline void clear_data();
  static const int kDataFieldNumber = 5;
  inline const ::follower::DiffPackage_Row& data(int index) const;
  inline ::follower::DiffPackage_Row* mutable_data(int index);
  inline ::follower::DiffPackage_Row* add_data();
  inline const ::google::protobuf::RepeatedPtrField< ::follower::DiffPackage_Row >&
      data() const;
  inline ::google::protobuf::RepeatedPtrField< ::follower::DiffPackage_Row >*
      mutable_data();
  
  // @@protoc_insertion_point(class_scope:follower.DiffPackage)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::int64 id_;
  ::std::string* tablename_;
  static const ::std::string _default_tablename_;
  ::google::protobuf::int64 starttime_;
  ::google::protobuf::int64 endtime_;
  ::google::protobuf::RepeatedPtrField< ::follower::DiffPackage_Row > data_;
  friend void  protobuf_AddDesc_aaa_2eproto();
  friend void protobuf_AssignDesc_aaa_2eproto();
  friend void protobuf_ShutdownFile_aaa_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];
  
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
  static DiffPackage* default_instance_;
};
// -------------------------------------------------------------------

class ServerList : public ::google::protobuf::Message {
 public:
  ServerList();
  virtual ~ServerList();
  
  ServerList(const ServerList& from);
  
  inline ServerList& operator=(const ServerList& from) {
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
  static const ServerList& default_instance();
  
  void Swap(ServerList* other);
  
  // implements Message ----------------------------------------------
  
  ServerList* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ServerList& from);
  void MergeFrom(const ServerList& from);
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
  
  // repeated string server = 1;
  inline int server_size() const;
  inline void clear_server();
  static const int kServerFieldNumber = 1;
  inline const ::std::string& server(int index) const;
  inline ::std::string* mutable_server(int index);
  inline void set_server(int index, const ::std::string& value);
  inline void set_server(int index, const char* value);
  inline void set_server(int index, const char* value, size_t size);
  inline ::std::string* add_server();
  inline void add_server(const ::std::string& value);
  inline void add_server(const char* value);
  inline void add_server(const char* value, size_t size);
  inline const ::google::protobuf::RepeatedPtrField< ::std::string>& server() const;
  inline ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_server();
  
  // @@protoc_insertion_point(class_scope:follower.ServerList)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::RepeatedPtrField< ::std::string> server_;
  friend void  protobuf_AddDesc_aaa_2eproto();
  friend void protobuf_AssignDesc_aaa_2eproto();
  friend void protobuf_ShutdownFile_aaa_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
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
  static ServerList* default_instance_;
};
// ===================================================================


// ===================================================================

// SyncItem

// required string tableName = 1;
inline bool SyncItem::has_tablename() const {
  return _has_bit(0);
}
inline void SyncItem::clear_tablename() {
  if (tablename_ != &_default_tablename_) {
    tablename_->clear();
  }
  _clear_bit(0);
}
inline const ::std::string& SyncItem::tablename() const {
  return *tablename_;
}
inline void SyncItem::set_tablename(const ::std::string& value) {
  _set_bit(0);
  if (tablename_ == &_default_tablename_) {
    tablename_ = new ::std::string;
  }
  tablename_->assign(value);
}
inline void SyncItem::set_tablename(const char* value) {
  _set_bit(0);
  if (tablename_ == &_default_tablename_) {
    tablename_ = new ::std::string;
  }
  tablename_->assign(value);
}
inline void SyncItem::set_tablename(const char* value, size_t size) {
  _set_bit(0);
  if (tablename_ == &_default_tablename_) {
    tablename_ = new ::std::string;
  }
  tablename_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* SyncItem::mutable_tablename() {
  _set_bit(0);
  if (tablename_ == &_default_tablename_) {
    tablename_ = new ::std::string;
  }
  return tablename_;
}

// required int64 endTime = 2;
inline bool SyncItem::has_endtime() const {
  return _has_bit(1);
}
inline void SyncItem::clear_endtime() {
  endtime_ = GOOGLE_LONGLONG(0);
  _clear_bit(1);
}
inline ::google::protobuf::int64 SyncItem::endtime() const {
  return endtime_;
}
inline void SyncItem::set_endtime(::google::protobuf::int64 value) {
  _set_bit(1);
  endtime_ = value;
}

// -------------------------------------------------------------------

// StartSync

// repeated .follower.SyncItem items = 1;
inline int StartSync::items_size() const {
  return items_.size();
}
inline void StartSync::clear_items() {
  items_.Clear();
}
inline const ::follower::SyncItem& StartSync::items(int index) const {
  return items_.Get(index);
}
inline ::follower::SyncItem* StartSync::mutable_items(int index) {
  return items_.Mutable(index);
}
inline ::follower::SyncItem* StartSync::add_items() {
  return items_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::follower::SyncItem >&
StartSync::items() const {
  return items_;
}
inline ::google::protobuf::RepeatedPtrField< ::follower::SyncItem >*
StartSync::mutable_items() {
  return &items_;
}

// -------------------------------------------------------------------

// Snapshot

// required string tableName = 1;
inline bool Snapshot::has_tablename() const {
  return _has_bit(0);
}
inline void Snapshot::clear_tablename() {
  if (tablename_ != &_default_tablename_) {
    tablename_->clear();
  }
  _clear_bit(0);
}
inline const ::std::string& Snapshot::tablename() const {
  return *tablename_;
}
inline void Snapshot::set_tablename(const ::std::string& value) {
  _set_bit(0);
  if (tablename_ == &_default_tablename_) {
    tablename_ = new ::std::string;
  }
  tablename_->assign(value);
}
inline void Snapshot::set_tablename(const char* value) {
  _set_bit(0);
  if (tablename_ == &_default_tablename_) {
    tablename_ = new ::std::string;
  }
  tablename_->assign(value);
}
inline void Snapshot::set_tablename(const char* value, size_t size) {
  _set_bit(0);
  if (tablename_ == &_default_tablename_) {
    tablename_ = new ::std::string;
  }
  tablename_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Snapshot::mutable_tablename() {
  _set_bit(0);
  if (tablename_ == &_default_tablename_) {
    tablename_ = new ::std::string;
  }
  return tablename_;
}

// required int64 snapShotTime = 2;
inline bool Snapshot::has_snapshottime() const {
  return _has_bit(1);
}
inline void Snapshot::clear_snapshottime() {
  snapshottime_ = GOOGLE_LONGLONG(0);
  _clear_bit(1);
}
inline ::google::protobuf::int64 Snapshot::snapshottime() const {
  return snapshottime_;
}
inline void Snapshot::set_snapshottime(::google::protobuf::int64 value) {
  _set_bit(1);
  snapshottime_ = value;
}

// required string snapShotPath = 3;
inline bool Snapshot::has_snapshotpath() const {
  return _has_bit(2);
}
inline void Snapshot::clear_snapshotpath() {
  if (snapshotpath_ != &_default_snapshotpath_) {
    snapshotpath_->clear();
  }
  _clear_bit(2);
}
inline const ::std::string& Snapshot::snapshotpath() const {
  return *snapshotpath_;
}
inline void Snapshot::set_snapshotpath(const ::std::string& value) {
  _set_bit(2);
  if (snapshotpath_ == &_default_snapshotpath_) {
    snapshotpath_ = new ::std::string;
  }
  snapshotpath_->assign(value);
}
inline void Snapshot::set_snapshotpath(const char* value) {
  _set_bit(2);
  if (snapshotpath_ == &_default_snapshotpath_) {
    snapshotpath_ = new ::std::string;
  }
  snapshotpath_->assign(value);
}
inline void Snapshot::set_snapshotpath(const char* value, size_t size) {
  _set_bit(2);
  if (snapshotpath_ == &_default_snapshotpath_) {
    snapshotpath_ = new ::std::string;
  }
  snapshotpath_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Snapshot::mutable_snapshotpath() {
  _set_bit(2);
  if (snapshotpath_ == &_default_snapshotpath_) {
    snapshotpath_ = new ::std::string;
  }
  return snapshotpath_;
}

// -------------------------------------------------------------------

// DiffPackage_Row

// required string key = 1;
inline bool DiffPackage_Row::has_key() const {
  return _has_bit(0);
}
inline void DiffPackage_Row::clear_key() {
  if (key_ != &_default_key_) {
    key_->clear();
  }
  _clear_bit(0);
}
inline const ::std::string& DiffPackage_Row::key() const {
  return *key_;
}
inline void DiffPackage_Row::set_key(const ::std::string& value) {
  _set_bit(0);
  if (key_ == &_default_key_) {
    key_ = new ::std::string;
  }
  key_->assign(value);
}
inline void DiffPackage_Row::set_key(const char* value) {
  _set_bit(0);
  if (key_ == &_default_key_) {
    key_ = new ::std::string;
  }
  key_->assign(value);
}
inline void DiffPackage_Row::set_key(const char* value, size_t size) {
  _set_bit(0);
  if (key_ == &_default_key_) {
    key_ = new ::std::string;
  }
  key_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DiffPackage_Row::mutable_key() {
  _set_bit(0);
  if (key_ == &_default_key_) {
    key_ = new ::std::string;
  }
  return key_;
}

// repeated string values = 2;
inline int DiffPackage_Row::values_size() const {
  return values_.size();
}
inline void DiffPackage_Row::clear_values() {
  values_.Clear();
}
inline const ::std::string& DiffPackage_Row::values(int index) const {
  return values_.Get(index);
}
inline ::std::string* DiffPackage_Row::mutable_values(int index) {
  return values_.Mutable(index);
}
inline void DiffPackage_Row::set_values(int index, const ::std::string& value) {
  values_.Mutable(index)->assign(value);
}
inline void DiffPackage_Row::set_values(int index, const char* value) {
  values_.Mutable(index)->assign(value);
}
inline void DiffPackage_Row::set_values(int index, const char* value, size_t size) {
  values_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DiffPackage_Row::add_values() {
  return values_.Add();
}
inline void DiffPackage_Row::add_values(const ::std::string& value) {
  values_.Add()->assign(value);
}
inline void DiffPackage_Row::add_values(const char* value) {
  values_.Add()->assign(value);
}
inline void DiffPackage_Row::add_values(const char* value, size_t size) {
  values_.Add()->assign(reinterpret_cast<const char*>(value), size);
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
DiffPackage_Row::values() const {
  return values_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
DiffPackage_Row::mutable_values() {
  return &values_;
}

// -------------------------------------------------------------------

// DiffPackage

// required int64 id = 1;
inline bool DiffPackage::has_id() const {
  return _has_bit(0);
}
inline void DiffPackage::clear_id() {
  id_ = GOOGLE_LONGLONG(0);
  _clear_bit(0);
}
inline ::google::protobuf::int64 DiffPackage::id() const {
  return id_;
}
inline void DiffPackage::set_id(::google::protobuf::int64 value) {
  _set_bit(0);
  id_ = value;
}

// required string tableName = 2;
inline bool DiffPackage::has_tablename() const {
  return _has_bit(1);
}
inline void DiffPackage::clear_tablename() {
  if (tablename_ != &_default_tablename_) {
    tablename_->clear();
  }
  _clear_bit(1);
}
inline const ::std::string& DiffPackage::tablename() const {
  return *tablename_;
}
inline void DiffPackage::set_tablename(const ::std::string& value) {
  _set_bit(1);
  if (tablename_ == &_default_tablename_) {
    tablename_ = new ::std::string;
  }
  tablename_->assign(value);
}
inline void DiffPackage::set_tablename(const char* value) {
  _set_bit(1);
  if (tablename_ == &_default_tablename_) {
    tablename_ = new ::std::string;
  }
  tablename_->assign(value);
}
inline void DiffPackage::set_tablename(const char* value, size_t size) {
  _set_bit(1);
  if (tablename_ == &_default_tablename_) {
    tablename_ = new ::std::string;
  }
  tablename_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DiffPackage::mutable_tablename() {
  _set_bit(1);
  if (tablename_ == &_default_tablename_) {
    tablename_ = new ::std::string;
  }
  return tablename_;
}

// required int64 startTime = 3;
inline bool DiffPackage::has_starttime() const {
  return _has_bit(2);
}
inline void DiffPackage::clear_starttime() {
  starttime_ = GOOGLE_LONGLONG(0);
  _clear_bit(2);
}
inline ::google::protobuf::int64 DiffPackage::starttime() const {
  return starttime_;
}
inline void DiffPackage::set_starttime(::google::protobuf::int64 value) {
  _set_bit(2);
  starttime_ = value;
}

// required int64 endTime = 4;
inline bool DiffPackage::has_endtime() const {
  return _has_bit(3);
}
inline void DiffPackage::clear_endtime() {
  endtime_ = GOOGLE_LONGLONG(0);
  _clear_bit(3);
}
inline ::google::protobuf::int64 DiffPackage::endtime() const {
  return endtime_;
}
inline void DiffPackage::set_endtime(::google::protobuf::int64 value) {
  _set_bit(3);
  endtime_ = value;
}

// repeated .follower.DiffPackage.Row data = 5;
inline int DiffPackage::data_size() const {
  return data_.size();
}
inline void DiffPackage::clear_data() {
  data_.Clear();
}
inline const ::follower::DiffPackage_Row& DiffPackage::data(int index) const {
  return data_.Get(index);
}
inline ::follower::DiffPackage_Row* DiffPackage::mutable_data(int index) {
  return data_.Mutable(index);
}
inline ::follower::DiffPackage_Row* DiffPackage::add_data() {
  return data_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::follower::DiffPackage_Row >&
DiffPackage::data() const {
  return data_;
}
inline ::google::protobuf::RepeatedPtrField< ::follower::DiffPackage_Row >*
DiffPackage::mutable_data() {
  return &data_;
}

// -------------------------------------------------------------------

// ServerList

// repeated string server = 1;
inline int ServerList::server_size() const {
  return server_.size();
}
inline void ServerList::clear_server() {
  server_.Clear();
}
inline const ::std::string& ServerList::server(int index) const {
  return server_.Get(index);
}
inline ::std::string* ServerList::mutable_server(int index) {
  return server_.Mutable(index);
}
inline void ServerList::set_server(int index, const ::std::string& value) {
  server_.Mutable(index)->assign(value);
}
inline void ServerList::set_server(int index, const char* value) {
  server_.Mutable(index)->assign(value);
}
inline void ServerList::set_server(int index, const char* value, size_t size) {
  server_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ServerList::add_server() {
  return server_.Add();
}
inline void ServerList::add_server(const ::std::string& value) {
  server_.Add()->assign(value);
}
inline void ServerList::add_server(const char* value) {
  server_.Add()->assign(value);
}
inline void ServerList::add_server(const char* value, size_t size) {
  server_.Add()->assign(reinterpret_cast<const char*>(value), size);
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
ServerList::server() const {
  return server_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
ServerList::mutable_server() {
  return &server_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace follower

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_aaa_2eproto__INCLUDED