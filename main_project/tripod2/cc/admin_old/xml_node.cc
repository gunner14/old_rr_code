/*
 * xml_node.cc
 *
 *  Created on: Apr 17, 2012
 *      Author: liuwenju
 */
#include <vector>
// #include <boost/algorithm/string/split.hpp>
#include "tripod2/cc/admin/xml_node.h"

namespace xce {
namespace tripod {
XmlNode::XmlNode(const std::string& ip, const std::string& port) : ip_(ip),
    port_(port) {
}
XmlNode::XmlNode(const std::string& nickname, const std::string& ip,
    const std::string& port) : nickname_(nickname), ip_(ip), port_(port) {
}

XmlNode::XmlNode(const std::string& node) {
//  std::vector<std::string> parts;
//  split(parts, node, is_any_of(":"));
//  if (parts.size() == 2) {
//    ip_ = parts[0];
//    port_ = parts[1];
//  } else if (parts.size() == 3) {
//    nickname_ = parts[0];
//    ip_ = parts[1];
//    port_ = parts[2];
//  }

  if (node.find(':') == node.rfind(':')) {
    ip_ = node.substr(0, node.find(':'));
    port_ = node.substr(node.find(':')+1);
  } else {
    nickname_ = node.substr(0, node.find(':'));
    ip_ = node.substr(node.find(':') + 1,
        (node.rfind(':') - node.find(':') - 1));
    port_ = node.substr(node.rfind(':') + 1);
  }
}

// 转为为字符串格式为: [nickname:]ip:port
std::string XmlNode::toString() const {
  if (nickname_.empty()) {
    return ip_ + ":" + port_;
  } else {
    return nickname_ + ":" + ip_ + ":" + port_;
  }
}

bool XmlNode::operator<(const XmlNode &m) const {
/*
  if (this->nickname_ < m.nickname_) {
    return true;
  } else if (this->nickname_ > m.nickname_) {
    return false;
  } else {
    if (this->ip_ < m.ip_) {
      return true;
    } else if (this->ip_ > m.ip_) {
      return false;
    } else {
      return this->port_ < m.port_;
    }
  }
*/
  return (this->toString() < m.toString());
}

std::string XmlNode::nickname() const {
  return this->nickname_;
}

std::string XmlNode::ip() const {
  return this->ip_;
}

std::string XmlNode::port() const {
  return this->port_;
}
};
};



