#ifndef ENTRY_H_
#define ENTRY_H_

#include <string>
#include <functional>

namespace xce {
namespace tripod {
namespace client {

class Entry {
 public:
  Entry(const std::string& namespace_id, const std::string& biz_id): namespace_(namespace_id),biz_(biz_id) {}
  const std::string GetNamespace() const { return namespace_;}
  const std::string GetBiz() const {return biz_;}
  const std::string ToString() const {
    return namespace_ + biz_;
  }
 private:
  std::string namespace_;
  std::string biz_;
}; 

struct EntryLessTo : public std::binary_function<Entry,Entry,bool> {
  bool operator() (const Entry& lhs, const Entry& rhs) {
    return lhs.ToString() < rhs.ToString();
  }
};

}}}

#endif

