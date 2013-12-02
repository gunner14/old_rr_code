/*
 * tripod_config.cc
 *
 *  Created on: Apr 18, 2012
 *      Author: liuwenju
 */

#include "tripod2/cc/admin/tripod_config.h"


namespace xce {
namespace tripod {
std::map<std::string, std::set<XmlNode> >& TripodConfig::caches() {
  return caches_;
}

std::map<std::string, std::set<XmlNode> >& TripodConfig::cachemanagers() {
  return cachemanagers_;
}

std::map<std::string, std::set<XmlNode> >& TripodConfig::producermanagers() {
  return producermanagers_;
}
};
};
