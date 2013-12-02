#include "MatchHandler.h"

namespace xce {
namespace ad {
using namespace std;
using namespace MyUtil;
using namespace IceUtil;
using namespace __gnu_cxx;

class BrandKeeper : public MyUtil::Singleton<BrandKeeper> {
public:
  BrandKeeper(){
    limit_ratio_ = 0.5; 
    range_ = 1000;
  }
  bool reserved(MatchHandlerParamter& para); 
  void Load();
  void RefreshParameters();
  
private:
  IceUtil::RWRecMutex mutex_;
  double limit_ratio_;
  int range_;
};

class BrandKeeperRefreshParametersTimer : public MyUtil::Timer {
public:
  BrandKeeperRefreshParametersTimer() :
    Timer(1 * 60 * 1000) { //1分钟重新Load一次 ， 暂定
  }
  virtual void handle() { 
    BrandKeeper::instance().RefreshParameters();
  }
};

class BrandKeeperLoadTimer : public MyUtil::Timer {
public:
  BrandKeeperLoadTimer() :
    Timer(24 * 60 * 60 * 1000) { //1天重新Load一次 ， 暂定
  }
  virtual void handle() { 
    BrandKeeper::instance().Load();
  }
};

}
}

