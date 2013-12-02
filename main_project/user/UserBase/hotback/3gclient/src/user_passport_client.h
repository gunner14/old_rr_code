#ifndef XCE_CLIENT_USER_PASSPORT_CLIENT_H_
#define XCE_CLIENT_USER_PASSPORT_CLIENT_H_

namespace xce {
namespace client {

class UserPassportClient {
public:
  //安全状态，具体含义有安全部门定义
  static int GetUserSafeStatus(int user_id);

  //0:激活；3:未激活；6:封禁；7:自杀
  static int GetUserStatus(int user_id);
};

}} //namespace xce::client

#endif
