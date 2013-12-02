
#include "consistent_hashing.h"
#include "CacheLocationConfig.h"
#include "boost/foreach.hpp"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "boost/lexical_cast.hpp"
#include "LogWrapper.h"

namespace xce {
namespace feed {

                  

std::string FEED_INDEX_ADDR = "feedindex1:10.3.22.141:6379:100;"\
                     "feedindex2:10.3.22.140:6379:100;"\
                     "feedindex3:10.3.22.139:6379:100;"\
                     "feedindex4:10.3.22.138:6379:100;"\
                     "feedindex5:10.3.22.137:6379:100;";
std::string FEED_INDEX_NS = "FEEDINDEX";
//int FEED_INDEX_CAP = 100; 
                                
CacheLocationConfig::CacheLocationConfig() {
  if(!InitCacheClusters()) {
    exit(-3);
  }
}

bool CacheLocationConfig::InitCacheClusters() {

  {
    ContinuumPtr c(new Continuum(FEED_INDEX_NS));
    if(!c) {
      MCE_WARN("CacheLocationConfig::InitCacheClusters --> new Continuum error!");
      return false;
    }

    MCE_INFO("CacheLocationConfig::InitCacheClusters() --> continuum add desc:" << FEED_INDEX_ADDR);
    clusters_.insert(make_pair<string, ContinuumPtr>(FEED_INDEX_NS, c));
  }

  UpdateContinuums();

  return true;
}


bool CacheLocationConfig::UpdateContinuums() {
  MCE_INFO("CacheLocationConfig::UpdateContinuums --> clusters size:" << clusters_.size());

  for (std::map<string, ContinuumPtr>::const_iterator mit = clusters_.begin(); mit != clusters_.end(); mit++) {
    ContinuumPtr c = mit->second;
    if(c) {
      std::vector<std::string> addr_vec;
      boost::split(addr_vec, FEED_INDEX_ADDR, boost::is_any_of(";"));

#ifdef STDOUT
      std::cout << "CacheLocationConfig::UpdateContinuums() --> ns:" << mit->first << " addr_vec size:" << addr_vec.size()  << std::endl;
#else
      MCE_INFO("CacheLocationConfig::UpdateContinuums() --> ns:" << mit->first << " addr_vec size:" << addr_vec.size());
#endif

      for(std::vector<std::string>::const_iterator vit = addr_vec.begin(); vit != addr_vec.end(); vit++) {
        if (!(vit->empty())) {
          std::vector<std::string> units;
          boost::split(units, *vit, boost::is_any_of(":"));
          int unit_size = units.size();
          if (unit_size != 4) {
            MCE_INFO("CacheLocationConfig::UpdateContinuums() --> address format error! ns:" << mit->first << " addr:" << *vit);
            return false;
          }
          std::string nickname = units[0];
          std::string cache_address = units[1];
          int cache_port = 0;
          try {
            cache_port = boost::lexical_cast<int>(units[2]);
          } catch (...) {
            MCE_INFO("CacheLocationConfig::UpdateContinuums() --> address.port formate error! ns:" << mit->first << " addr:" << *vit);
            return false;
          }

          unsigned int cap = 0;
          try {
            cap = boost::lexical_cast<unsigned int>(units[3]);
          } catch (...) {
            MCE_INFO("CacheLocationConfig::UpdateContinuums() --> address.cap formate error! ns:" << mit->first << " addr:" << *vit);
            return false;
          }


          c->Add(nickname, cache_address, cache_port, cap);
#ifdef STDOUT
          std::cout << "CacheLocationConfig::UpdateContinuums() --> ns:" << mit->first << " desc:" << nickname << " ip:" << cache_address << " port:" << cache_port << " cap:" << cap << std::endl;
#else
          MCE_INFO("CacheLocationConfig::UpdateContinuums() --> ns:" << mit->first << " desc:" << nickname << " ip:" << cache_address << " port:" << cache_port << " cap:" << cap);
#endif
        }
      }

      c->Rebuild();

    }
  }

}

ContinuumPtr CacheLocationConfig::GetContinuum(const char * key, size_t len) {
  size_t delim_pos = 0;
  for (; delim_pos < len; ++ delim_pos) {
    if (key[delim_pos] == '#') {
      break;
    }
  }

  ContinuumPtr c;

  if (delim_pos != len) {
    std::string prefix(key, delim_pos);
    map<string, ContinuumPtr>::const_iterator it = clusters_.find(prefix);
    if (it != clusters_.end()) {
      c = it->second;
    }
  }   
  return c;
}

//std::string CacheLocationConfig::GetNodeDesc(const char * key, size_t len) {
//  ContinuumPtr c = GetContinuum(key, len);
//  if (c) { 
//    std::string desc = c->Locate(key, len);
//    return desc;
//  }
//  MCE_WARN("CacheLocationConfig::GetNodeDesc --> no continuum! key:" << key << " len:" << len);
//
//  return "";
//}

Continuum::CachePoint CacheLocationConfig::GetCachePoint(const char * key, size_t len) {
 Continuum::CachePoint  ret;
 ContinuumPtr c = GetContinuum(key, len);
  if (c) { 
    ret = c->Locate(key, len);
    return ret;
  }

  MCE_WARN("CacheLocationConfig::GetNodeDesc --> no continuum! key:" << key << " len:" << len);
  return ret;
}

}}


//int main() {
//  std::cout << xce::feed::CacheLocationConfig::instance().GetContinuum("FEEDINDEX#123", 13)->Dump() << std::endl;
//  xce::feed::Continuum::CachePoint cp = xce::feed::CacheLocationConfig::instance().GetCachePoint("FEEDINDEX#123", 13);
//  std::cout << "desc:" << cp.desc_ << " ip:" << cp.ip_addr_ << " port:" << cp.port_ << " cap:" << cp.capacity_ << std::endl;
//}

