/*
 * RedirectionManager.h
 *
 *  Created on: 2010-5-21
 *      Author: ld
 */

#ifndef __HELLO_XOA_I_H__
#define __HELLO_XOA_I_H__

#include <vector>
#include <map>
#include "Ice/Ice.h"
#include "Singleton.h"
#include "ServiceI.h"
#include "IceLogger.h"
#include "HelloXoa.h"

using namespace MyUtil;

namespace xoa2 {

class HelloXoaI: public HelloXoa, public MyUtil::Singleton<HelloXoaI> {
public:
  HelloXoaI();
  virtual ::std::string SayHello(const ::std::string&, const ::Ice::Current& = ::Ice::Current());
private:
  int counter_;
};


}

#endif /* __NOTIFY_REDIRECTIONMANAGER_H__ */
