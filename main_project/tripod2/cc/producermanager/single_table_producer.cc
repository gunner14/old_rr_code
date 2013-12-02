#include "tripod2/cc/producermanager/single_table_producer.h"

namespace xce {
namespace tripod {
     
std::string AssembleKeysForSql(const StringListType& keys) {
  std::ostringstream oss;
  oss << "(";
  for (std::list<std::string>::const_iterator iter = keys.begin(); iter != keys.end(); ++iter) {
    if (iter != keys.begin()) {
      oss << ",";
    }
    oss << *iter;
  }
  oss << ")";
  return oss.str();
}

std::map<int, StringListType> SeperateKeysByMod(const StringListType& keys, int mod) {
  std::map<int, StringListType> result;
  for (StringListType::const_iterator iter = keys.begin(); iter != keys.end(); ++iter) {
    long tmp_key = boost::lexical_cast<long>(*iter);
    std::map<int, StringListType>::iterator found_iter = result.find(tmp_key % mod);
    if (found_iter == result.end()) {
      StringListType mod_list;
      mod_list.push_back(*iter);
      result.insert(make_pair<int, StringListType>((int)tmp_key % mod, mod_list));
    } else {
      found_iter->second.push_back(*iter);
    }
  }
  return result;
}

}}//namespace
