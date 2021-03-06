/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 */
#ifndef engine_TYPES_H
#define engine_TYPES_H

#include <Thrift.h>
#include <TApplicationException.h>
#include <protocol/TProtocol.h>
#include <transport/TTransport.h>



namespace com { namespace renren { namespace ad { namespace engine {

struct ActionType {
  enum type {
    PAGEVIEW = 0,
    HEALTH_CHECK = 1,
    DIAGONOSIS = 2,
    DEBUG = 3
  };
};

typedef struct _UserAndAction__isset {
  _UserAndAction__isset() : id(false), stage(false), gender(false), age(false), school(false), userMajor(false), grade(false), homeArea(false), currentArea(false), ip(false), ipArea(false), actionType(false), object(false) {}
  bool id;
  bool stage;
  bool gender;
  bool age;
  bool school;
  bool userMajor;
  bool grade;
  bool homeArea;
  bool currentArea;
  bool ip;
  bool ipArea;
  bool actionType;
  bool object;
} _UserAndAction__isset;

class UserAndAction {
 public:

  static const char* ascii_fingerprint; // = "7B59D276409F5BB3949BDF420ADA8753";
  static const uint8_t binary_fingerprint[16]; // = {0x7B,0x59,0xD2,0x76,0x40,0x9F,0x5B,0xB3,0x94,0x9B,0xDF,0x42,0x0A,0xDA,0x87,0x53};

  UserAndAction() : id(-1), stage(-1), gender(-1), age(-1), school(-1LL), userMajor(-1), grade(-1), homeArea(""), currentArea(""), ip(0), ipArea(""), object("") {
  }

  virtual ~UserAndAction() throw() {}

  int32_t id;
  int16_t stage;
  int16_t gender;
  int16_t age;
  int64_t school;
  int32_t userMajor;
  int16_t grade;
  std::string homeArea;
  std::string currentArea;
  int32_t ip;
  std::string ipArea;
  ActionType::type actionType;
  std::string object;

  _UserAndAction__isset __isset;

  bool operator == (const UserAndAction & rhs) const
  {
    if (!(id == rhs.id))
      return false;
    if (!(stage == rhs.stage))
      return false;
    if (!(gender == rhs.gender))
      return false;
    if (!(age == rhs.age))
      return false;
    if (!(school == rhs.school))
      return false;
    if (!(userMajor == rhs.userMajor))
      return false;
    if (!(grade == rhs.grade))
      return false;
    if (!(homeArea == rhs.homeArea))
      return false;
    if (!(currentArea == rhs.currentArea))
      return false;
    if (!(ip == rhs.ip))
      return false;
    if (!(ipArea == rhs.ipArea))
      return false;
    if (!(actionType == rhs.actionType))
      return false;
    if (!(object == rhs.object))
      return false;
    return true;
  }
  bool operator != (const UserAndAction &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const UserAndAction & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _AdRequest__isset {
  _AdRequest__isset() : userAndAction(false), referer(false), channelId(false), syndicationId(false), memberId(false), matchId(false), zoneDict(false), attributes(false) {}
  bool userAndAction;
  bool referer;
  bool channelId;
  bool syndicationId;
  bool memberId;
  bool matchId;
  bool zoneDict;
  bool attributes;
} _AdRequest__isset;

class AdRequest {
 public:

  static const char* ascii_fingerprint; // = "B9B163C61BD22CB12D6F7371B816CEE2";
  static const uint8_t binary_fingerprint[16]; // = {0xB9,0xB1,0x63,0xC6,0x1B,0xD2,0x2C,0xB1,0x2D,0x6F,0x73,0x71,0xB8,0x16,0xCE,0xE2};

  AdRequest() : referer(""), channelId(0), syndicationId(0), memberId(0), matchId(0) {
  }

  virtual ~AdRequest() throw() {}

  UserAndAction userAndAction;
  std::string referer;
  int32_t channelId;
  int32_t syndicationId;
  int32_t memberId;
  int32_t matchId;
  std::map<int64_t, int16_t>  zoneDict;
  std::map<std::string, std::string>  attributes;

  _AdRequest__isset __isset;

  bool operator == (const AdRequest & rhs) const
  {
    if (!(userAndAction == rhs.userAndAction))
      return false;
    if (!(referer == rhs.referer))
      return false;
    if (!(channelId == rhs.channelId))
      return false;
    if (!(syndicationId == rhs.syndicationId))
      return false;
    if (!(memberId == rhs.memberId))
      return false;
    if (!(matchId == rhs.matchId))
      return false;
    if (!(zoneDict == rhs.zoneDict))
      return false;
    if (!(attributes == rhs.attributes))
      return false;
    return true;
  }
  bool operator != (const AdRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const AdRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ClickAction__isset {
  _ClickAction__isset() : userId(false), ip(false), referer(false), clickUrl(false), context(false), extra(false) {}
  bool userId;
  bool ip;
  bool referer;
  bool clickUrl;
  bool context;
  bool extra;
} _ClickAction__isset;

class ClickAction {
 public:

  static const char* ascii_fingerprint; // = "1F10F993631B3ABD721D79EE58C2056E";
  static const uint8_t binary_fingerprint[16]; // = {0x1F,0x10,0xF9,0x93,0x63,0x1B,0x3A,0xBD,0x72,0x1D,0x79,0xEE,0x58,0xC2,0x05,0x6E};

  ClickAction() : userId(0), ip(""), referer(""), clickUrl(""), context(""), extra("") {
  }

  virtual ~ClickAction() throw() {}

  int64_t userId;
  std::string ip;
  std::string referer;
  std::string clickUrl;
  std::string context;
  std::string extra;

  _ClickAction__isset __isset;

  bool operator == (const ClickAction & rhs) const
  {
    if (!(userId == rhs.userId))
      return false;
    if (!(ip == rhs.ip))
      return false;
    if (!(referer == rhs.referer))
      return false;
    if (!(clickUrl == rhs.clickUrl))
      return false;
    if (!(context == rhs.context))
      return false;
    if (!(extra == rhs.extra))
      return false;
    return true;
  }
  bool operator != (const ClickAction &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ClickAction & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Ad__isset {
  _Ad__isset() : creativeId(false), adGroupId(false), context(false), position(false), extra(false) {}
  bool creativeId;
  bool adGroupId;
  bool context;
  bool position;
  bool extra;
} _Ad__isset;

class Ad {
 public:

  static const char* ascii_fingerprint; // = "1448AFE5D8086FC1ADB058DEF2087478";
  static const uint8_t binary_fingerprint[16]; // = {0x14,0x48,0xAF,0xE5,0xD8,0x08,0x6F,0xC1,0xAD,0xB0,0x58,0xDE,0xF2,0x08,0x74,0x78};

  Ad() : creativeId(0), adGroupId(0), context(""), position(0), extra("") {
  }

  virtual ~Ad() throw() {}

  int64_t creativeId;
  int64_t adGroupId;
  std::string context;
  int16_t position;
  std::string extra;

  _Ad__isset __isset;

  bool operator == (const Ad & rhs) const
  {
    if (!(creativeId == rhs.creativeId))
      return false;
    if (!(adGroupId == rhs.adGroupId))
      return false;
    if (!(context == rhs.context))
      return false;
    if (!(position == rhs.position))
      return false;
    if (!(extra == rhs.extra))
      return false;
    return true;
  }
  bool operator != (const Ad &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Ad & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _AdResponse__isset {
  _AdResponse__isset() : candidates(false) {}
  bool candidates;
} _AdResponse__isset;

class AdResponse {
 public:

  static const char* ascii_fingerprint; // = "CA0CA6B8A2E981D02D982FFB3539599E";
  static const uint8_t binary_fingerprint[16]; // = {0xCA,0x0C,0xA6,0xB8,0xA2,0xE9,0x81,0xD0,0x2D,0x98,0x2F,0xFB,0x35,0x39,0x59,0x9E};

  AdResponse() {
  }

  virtual ~AdResponse() throw() {}

  std::map<int64_t, std::vector<Ad> >  candidates;

  _AdResponse__isset __isset;

  bool operator == (const AdResponse & rhs) const
  {
    if (!(candidates == rhs.candidates))
      return false;
    return true;
  }
  bool operator != (const AdResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const AdResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

}}}} // namespace

#endif
