#ifndef __AD_MOB_LOG_R_I_H__
#define __AD_MOB_LOG_R_I_H__

#include "AdMobLogR.h"
#include "ServiceI.h"
#include "Singleton.h"
#include "AdMobLogRMaker.h"

namespace xce {
namespace ad {

using namespace MyUtil;
using namespace std;
using namespace com::xiaonei::xce;

class AdMobLogRI: virtual public AdMobLogR, public MyUtil::Singleton<AdMobLogRI> {
public:
  void Initialize();
  
  virtual void ReqLog(const string& log, const Ice::Current& current);
  
	void SetBufSize(size_t size){
    buf_size_ = size;
	}
private:
  AdMobLogMakerPtr req_log_maker_;
  size_t buf_size_; //缓冲log条数
};

}
}
#endif
