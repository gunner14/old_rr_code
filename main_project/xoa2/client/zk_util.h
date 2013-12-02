#ifndef _XOA2_CLIENT_ZK_UTIL_H_
#define _XOA2_CLIENT_ZK_UTIL_H_

#include <string>
#include <map>

namespace xoa2 {
namespace client {
  
//解析节点数据
void ParseProperty(const std::string & prop_str, std::map<std::string, std::string> * res);

int GetPropertyInt(const std::map<std::string, std::string> props, const std::string& key, int def = 0);

}
}

#endif // _XOA2_CLIENT_ZK_UTIL_H_
