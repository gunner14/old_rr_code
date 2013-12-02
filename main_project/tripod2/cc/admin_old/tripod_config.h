/*
 * tripod_config.h
 *
 *  Created on: Apr 18, 2012
 *      Author: liuwenju
 */

#ifndef TRIPODCONFIG_H_
#define TRIPODCONFIG_H_

#include <map>
#include <string>
#include <set>
#include "tripod2/cc/admin/xml_node.h"

namespace xce {
namespace tripod {

class TripodConfig {
 public:
  std::map<std::string, std::set<XmlNode> >& caches();
  std::map<std::string, std::set<XmlNode> >& cachemanagers();
  std::map<std::string, std::set<XmlNode> >& producermanagers();
 private:
  std::map<std::string, std::set<XmlNode> > caches_;
  std::map<std::string, std::set<XmlNode> > cachemanagers_;
  std::map<std::string, std::set<XmlNode> > producermanagers_;
};  // end class
};
};  // end xce::tripod


#endif /* TRIPODCONFIG_H_ */
