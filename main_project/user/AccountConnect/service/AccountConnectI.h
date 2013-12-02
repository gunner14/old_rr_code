#ifndef XCE_ACCOUNT_ACCOUNTCONNECTI_H_
#define XCE_ACCOUNT_ACCOUNTCONNECTI_H_

#include "AccountConnect.h"
#include "IceExt/src/TempMutex.h"
#include "ServiceI.h"

namespace xce {
namespace account {

//服务配置
const static string SERVICE_IDENTITY = "AC";
const static string SERVICE_PREFIX = "AccountConnect";
const static string CONTROLLER_ENDPOINT = "ControllerAccountConnect";

//帐号连接服务
class AccountConnectI : virtual public AccountConnect, public MyUtil::Singleton<AccountConnectI> {
public:
  AccountConnectI();
  bool set(const string &cid, int type, long uid, const Ice::Current& = Ice::Current());
  bool isValid(const Ice::Current& = Ice::Current());
  void setValid(bool valid, const Ice::Current& = Ice::Current());
private:
  xce::tempmutex::TempMutexManagerPtr tempMutexManagerPtr_;
}; 

}} //namespace xce::account

#endif
