#ifndef __USERBASEI_H__
#define __USERBASEI_H__

#include "UserBase.h"
#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace userbase {

const std::string USER_LOGINCOUNT_MANAGER = "ULCM";

//-------------------------UserLoginCountManagerI-------------------------------
class UserLoginCountManagerI : virtual public UserLoginCountReader,
                               public MyUtil::Singleton<UserLoginCountManagerI> {
public:
	UserLoginCountManagerI() {}
	
  virtual int getUserLoginCount(Ice::Int userId, const Ice::Current& = Ice::Current());
  virtual void incUserLoginCount(Ice::Int userId, const Ice::Current& = Ice::Current());
	virtual void setData(const ::MyUtil::ObjectResultPtr& userData, const ::Ice::Current& = ::Ice::Current());

	virtual void setValid(bool valid, const ::Ice::Current& = ::Ice::Current());
	virtual bool isValid(const ::Ice::Current& = ::Ice::Current());
};

}} //namespace xce::userbase
#endif
