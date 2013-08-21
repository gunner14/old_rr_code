#ifndef _DBC_LIENT_H_ 
#define _DBC_LIENT_H_

#include <stdint.h>
#include <string>
#include <vector>

namespace entry {
struct Field {
  Field(const std::string &k, const std::string &v, bool idx)
  : key_(k)
  , value_(v)
  , index_(idx){}
  Field() {}

  std::string key_;
  std::string value_;
  bool index_;
};

struct Entry {
  std::vector<Field> fields_;
  std::string value_;
  int64_t id_;

};

struct FindGids {
  std::vector<int64_t> ids_;
  int64_t cursor_;
};

//{"value":"null","entryId":22000,"fields":[{"value":"888","indexed":true,"key":"author"},{"value":"4888","indexed":true,"key":"tag"},{"value":"3888","indexed":true,"key":"tag"}]}
void FieldsToJson(const std::vector<Field> &v, std::string *str);
void JsonToEntry(const std::string &json, Entry *y);
void JsonToFindGids(const std::string &json, FindGids *f);

} // namespace
#endif 
