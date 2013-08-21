#include "dbc_client.h"
#include <stdio.h>
#include <map>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include "base/logging.h"


namespace entry {

void FieldsToJson(const std::vector<Field> &v, std::string *str) {
  // {"value":"888","key":"author","indexed":true}
  if (!str)
    return;
  str->clear();
  str->append("[");
  for (std::vector<Field>::const_iterator it = v.begin(); it != v.end(); ++it) {
    str->append("{\"key\":\"", sizeof("{\"key\":\"")-1);
    str->append(it->key_);
    str->append("\",\"value\":\"", sizeof("\",\"value\":\"")-1);
    str->append(it->value_);
    str->append("\",\"indexed\":", sizeof("\",\"indexed\":")-1);
    if (it != v.end()-1) {
      if (it->index_)
        str->append("true},", sizeof("true},")-1);
      else
        str->append("false},", sizeof("false},")-1);
    } else {
      if (it->index_)
        str->append("true}", sizeof("true}")-1);
      else
        str->append("false}", sizeof("false}")-1);
    }
  }
  str->append("]");
}

void JsonToEntry(const std::string &json, Entry* y) {
  if (!y || json.empty())
    return;
  typedef boost::property_tree::ptree ptree;
  ptree pt;
  std::stringstream ss(json);
  try {
    boost::property_tree::json_parser::read_json(ss, pt);
  } catch(boost::property_tree::ptree_error & e) {
    LOG(LS_WARNING) << "<> parse json error:" << json;
    return ;
  }
  boost::optional<std::string> v = pt.get_optional<std::string>("value");
  if (v)
    y->value_ = v.get();

  boost::optional<int64_t> id = pt.get_optional<int64_t>("entryId");
  if (id)
    y->id_ = id.get();

  BOOST_FOREACH(ptree::value_type &f, pt.get_child("fields")) {
    Field fld;
    BOOST_FOREACH(ptree::value_type &v, pt.get_child("")) {
      if (v.first == "indexed")
        fld.index_ = (v.second.data().size() == 4)? true: false; // size("true") == 4
      else if (v.first == "key")
        fld.key_ = v.second.data();
      else
        fld.value_ = v.second.data();
    }
    y->fields_.push_back(fld);
  }
  
}

void JsonToFindGids(const std::string &json, FindGids *f) {

  if (!f || json.empty())
    return;
  typedef boost::property_tree::ptree ptree;
  ptree pt;
  std::stringstream ss(json);
  try {
    boost::property_tree::json_parser::read_json(ss, pt);
  } catch(boost::property_tree::ptree_error & e) {
    LOG(LS_WARNING) << "<> parse json error:" << json;
    return ;
  }

  boost::optional<int64_t> cursor = pt.get<int64_t>("cursor");
  if (cursor)
    f->cursor_ = cursor.get();

  BOOST_FOREACH(ptree::value_type &g, pt.get_child("gids")) {
    int64_t gid = atol(g.second.data().c_str());
    f->ids_.push_back(gid);
  }
}

} // namespace 

