#ifndef XCE_CLIENT_USER_NAME_CLIENT_H_
#define XCE_CLIENT_USER_NAME_CLIENT_H_

#include <string>

namespace xce {
namespace client {

class UserNameClient {
public:
  static std::string GetUserName(int user_id);
};

}} //namespace xce::client

#endif
