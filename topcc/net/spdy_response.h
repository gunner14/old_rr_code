#ifndef _NET_SPDY_RESPONSE_H_
#define _NET_SPDY_RESPONSE_H_

#include <map>
#include <boost/property_tree/ptree.hpp>

namespace xoa {
class XoaResponse {
 public:
  int status_;
  int error_;
  std::string content_;
  std::string content_type_;

  typedef boost::property_tree::ptree ptree;

  bool FromJson(ptree *pt);
  bool FromDecodeUrl(std::map<std::string, std::string> *kv);
  bool FromXml(ptree *pt);
};

} // namespace
#endif // undef
