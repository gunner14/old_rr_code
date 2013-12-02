/*
 * =====================================================================================
 *
 *       Filename:  compute_ratio.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年08月24日
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lei.yu1 
 *        Company:  renren
 *
 * =====================================================================================
 */

#ifndef __XCE_AD_COMPUTE_RATIO__
#define __XCE_AD_COMPUTE_RATIO__

#include <map>
#include <string>
#include "Singleton.h"
#include "../common/constant.h"
#include "config.h"
#include "LogWrapper.h"
#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>

namespace xce{
namespace ad{

class ComputeRatio : public IceUtil::Shared{
public:
  virtual ~ComputeRatio() {}
  virtual double Compute(std::map<std::string, double>& dataMap) = 0;
};//end of class ComputeRatio

typedef IceUtil::Handle<ComputeRatio> ComputeRatioPtr;

}//end of namespace ad
}//end of namespace xce

#endif
