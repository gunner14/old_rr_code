#include "tripod2/cc/producermanager/example_producer.h"

#include <algorithm>
#include "boost/lexical_cast.hpp"
#include "base/stringdigest.h"

#include "LogWrapper.h"
#include "OceProto/proto/UserBase.pb.h"

namespace xce {
namespace tripod {

bool ExampleProducer::Create(const std::string& key, ByteSeq* value, bool use_master) {
  if (!value) {
    return false;
  }  

  
  int64 id = -12;
  try {
    id = boost::lexical_cast<long>(key);
  } catch (...) { }

  using namespace xce::tripoddata::userbase;
  PbUserUrl url;
  url.set_id(id);
  url.set_largeurl("largeurl...");
  url.set_mainurl("mainurl...");
  url.set_tinyurl("tinyurl...");
  url.set_headurl("headurl...");
  url.set_flexurl("flexurl...");

  std::string v;
  url.SerializeToString(&v);
//  std::string v = xce::MD5(key);
  value->insert(value->begin(), v.begin(), v.end());
  return true;
}

}} // xce::tripod


