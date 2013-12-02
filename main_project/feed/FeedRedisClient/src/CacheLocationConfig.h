#ifndef __XCE_FEED_CACHE_LOCATION_CONFIG__
#define __XCE_FEED_CACHE_LOCATION_CONFIG__

#include <string>
#include <map>
#include "boost/shared_ptr.hpp"
#include "consistent_hashing.h"

namespace xce {
namespace feed {

typedef boost::shared_ptr<Continuum> ContinuumPtr;

class CacheLocationConfig : public MyUtil::Singleton < CacheLocationConfig >  {

public:
  
  CacheLocationConfig();
  bool InitCacheClusters();
  bool UpdateContinuums();
  //std::string GetNodeDesc(const char * key, size_t len);
  Continuum::CachePoint GetCachePoint(const char * key, size_t len);
  ContinuumPtr GetContinuum(const char * key, size_t len);


private:
  map<string, ContinuumPtr> clusters_;

};

}}

#endif 
