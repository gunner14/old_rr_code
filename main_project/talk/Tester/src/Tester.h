#ifndef XNTALK_TESTER_H
#define XNTALK_TESTER_H
#include "ServiceI.h"
#include "Singleton.h"
#include "XNTalkTester.h"

namespace xce {
namespace xntalk {
namespace tester {

class TesterManagerI :public xce::xntalk::TesterManager, public MyUtil::Singleton<TesterManagerI> {
public:
  virtual void Invoke1(const ::Ice::Current& = ::Ice::Current());
  virtual void Invoke2(const ::Ice::Current& = ::Ice::Current());
private:
  void TestMC();
};

};
};
};



#endif
