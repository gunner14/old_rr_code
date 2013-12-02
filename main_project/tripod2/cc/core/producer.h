#ifndef PRODUCER_H_
#define PRODUCER_H_

#include <string>
#include <map>

#include "boost/foreach.hpp"

#include "OceSlice/slice/tripod2.h"
#include "tripod2/cc/core/common.h"

namespace xce {
namespace tripod {

struct Producer {
  virtual bool Create(const std::string& key, ByteSeq* value, bool use_master) = 0;
  virtual std::map<std::string, bool> Create(const StringListType& keys, String2ByteSeqMap* values, bool use_master) {
    std::map<std::string, bool> result;
    if (!values) {
      return result;
    }
    BOOST_FOREACH(const std::string& key, keys) {
      ByteSeq value;
      bool c = Create(key, &value, use_master);
      if (c) {
        values->insert(std::make_pair(key, value));
      }
      result.insert(std::make_pair(key, c));
    }
    return result;   
  }
  virtual bool CreateKeys(const std::string& key,int limit, StringListType* keyseq) {
    return false;
  }
};

}} // xce::tripod
#endif

