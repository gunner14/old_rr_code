/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 */
#ifndef ping_TYPES_H
#define ping_TYPES_H

#include <Thrift.h>
#include <TApplicationException.h>
#include <protocol/TProtocol.h>
#include <transport/TTransport.h>



namespace com { namespace renren { namespace ad { namespace ping {

typedef struct _StatEntry__isset {
  _StatEntry__isset() : serviceName(false), node(false), tag(false), cnt(false), sum(false), max(false), min(false), dayCnt(false), daySum(false), dayMax(false), dayMin(false), performance99(false) {}
  bool serviceName;
  bool node;
  bool tag;
  bool cnt;
  bool sum;
  bool max;
  bool min;
  bool dayCnt;
  bool daySum;
  bool dayMax;
  bool dayMin;
  bool performance99;
} _StatEntry__isset;

class StatEntry {
 public:

  static const char* ascii_fingerprint; // = "586B4437A9C08F6ED05229F35390ABDA";
  static const uint8_t binary_fingerprint[16]; // = {0x58,0x6B,0x44,0x37,0xA9,0xC0,0x8F,0x6E,0xD0,0x52,0x29,0xF3,0x53,0x90,0xAB,0xDA};

  StatEntry() : serviceName(""), node(""), tag(""), cnt(0), sum(0), max(0), min(0), dayCnt(0), daySum(0), dayMax(0), dayMin(0), performance99(0) {
  }

  virtual ~StatEntry() throw() {}

  std::string serviceName;
  std::string node;
  std::string tag;
  int64_t cnt;
  int64_t sum;
  int64_t max;
  int64_t min;
  int64_t dayCnt;
  int64_t daySum;
  int64_t dayMax;
  int64_t dayMin;
  int64_t performance99;

  _StatEntry__isset __isset;

  bool operator == (const StatEntry & rhs) const
  {
    if (!(serviceName == rhs.serviceName))
      return false;
    if (!(node == rhs.node))
      return false;
    if (!(tag == rhs.tag))
      return false;
    if (!(cnt == rhs.cnt))
      return false;
    if (!(sum == rhs.sum))
      return false;
    if (!(max == rhs.max))
      return false;
    if (!(min == rhs.min))
      return false;
    if (!(dayCnt == rhs.dayCnt))
      return false;
    if (!(daySum == rhs.daySum))
      return false;
    if (!(dayMax == rhs.dayMax))
      return false;
    if (!(dayMin == rhs.dayMin))
      return false;
    if (!(performance99 == rhs.performance99))
      return false;
    return true;
  }
  bool operator != (const StatEntry &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const StatEntry & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

}}}} // namespace

#endif
