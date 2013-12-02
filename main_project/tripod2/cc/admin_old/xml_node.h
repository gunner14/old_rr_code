/*
 * xml_node.h
 *
 *  Created on: Apr 17, 2012
 *      Author: liuwenju
 */

#ifndef XMLNODE_H_
#define XMLNODE_H_

#include <string>

namespace xce {
namespace tripod {

class XmlNode {
 public:
  XmlNode(const std::string& ip, const std::string& port);
  XmlNode(const std::string& nickname, const std::string& ip,
      const std::string& port);
  explicit XmlNode(const std::string& node);
  std::string toString() const;  // 转为为字符串格式为: [nickname:]ip:port
  bool operator<(const XmlNode &m) const;
  std::string nickname() const;
  std::string ip() const;
  std::string port() const;
 private:
  std::string nickname_;  // 只有cache才有
  std::string ip_;
  std::string port_;
};  // end classs
};
};  // end namespace xce::tripod


#endif /* XmlNode_H_ */
