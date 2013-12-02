
#include <algorithm>
#include "BrandKeeper.h"
#include "ParametersController.h"

using namespace xce::ad;

namespace xce {
namespace ad {

bool BrandKeeper::reserved(MatchHandlerParamter& para){
  double ratio = limit_ratio_; // 最低比例限制
  /*
   * TODO 取出比例
   */
  if (ratio > 1.0) {
    ratio = 1.0; 
  } 
  if (ratio < limit_ratio_) {
    ratio = limit_ratio_; 
  }  
  MCE_DEBUG("BrandKeeper --> ratio = " << ratio << "range = " << range_);
  return (rand() % range_) <  (ratio * range_);
}

void BrandKeeper::RefreshParameters() {
  double ratio = limit_ratio_;
  string ratio_value = EngineParametersCache::instance().GetParameterByName("min_brand_reserved_ratio");
  MCE_DEBUG("BrandKeeper::RefreshParameters --> key = min_brand_reserved_ratio," << " value = " << ratio_value);
  if (!ratio_value.empty()) {
    try{
      ratio = boost::lexical_cast<double>(ratio_value);
      IceUtil::RWRecMutex::RLock lock(mutex_);
      limit_ratio_ = ratio;
    } catch (...){
      MCE_WARN("BrandKeeper::RefreshParameters --> min_brand_reserved_ratio parser error.");
    }
  }
}


}
}
