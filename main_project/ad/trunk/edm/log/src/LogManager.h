/*
 * LogManager.h
 *
 *  Created on: 2012-2-23
 *      Author: ping.zhang
 */

#ifndef LOGMANAGER_H_
#define LOGMANAGER_H_

#include <iostream>
#include <string>
#include <IceUtil/IceUtil.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/Thread.h>
#include <Ice/Ice.h>
#include <QueryRunner.h>
#include "Util.h"
#include "ServiceI.h"
#include "Singleton.h"
#include "EdmLog.h"


namespace xce{
namespace ad{
using namespace MyUtil;
using namespace std;
using namespace com::xiaonei::xce;

class EdmLogManagerI: virtual public EdmLogManager,virtual public MyUtil::Singleton<EdmLogManagerI> {
public:
  virtual void Expose(const string& log, const Ice::Current& = Ice::Current());
  virtual void Click(const string& log, const Ice::Current& = Ice::Current());
  virtual void Send(const string& log, const Ice::Current& = Ice::Current());


  virtual int UpdateOnceCount(Ice::Int uid , const Ice::Current& current){
    return uid;
  }


};
}
}

#endif /* LOGMANAGER_H_ */
