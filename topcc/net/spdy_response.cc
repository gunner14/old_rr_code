#include "net/spdy_response.h"

#include <sstream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "net/url_parse.h"

namespace xoa {
bool XoaResponse::FromJson(ptree *pt) {
  if (!pt)
    return false;
  std::stringstream ss(content_);
  boost::property_tree::json_parser::read_json(ss, *pt);
  return true;
}

bool XoaResponse::FromDecodeUrl(std::map<std::string, std::string> *kv) {
   return arch::ParseParam(content_, kv);
}

bool XoaResponse::FromXml(ptree *pt) {
  if (!pt)
    return false;
  std::stringstream ss(content_);
  boost::property_tree::xml_parser::read_xml(ss, *pt);
  return true;
}

} // namespace
