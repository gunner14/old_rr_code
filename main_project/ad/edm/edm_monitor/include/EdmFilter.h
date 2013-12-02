/*
 * =====================================================================================
 *
 *       Filename:  EdmFilter.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年04月27日
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lei.yu1 
 *        Company:  renren
 *
 * =====================================================================================
 */

#ifndef __XCE_EDM_FILTER_H_
#define __XCE_EDM_FILTER_H_

#include <map>
#include <vector>
#include <string>
#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>
#include "EdmMonitorConfig.h"
#include "../common/ns_common.h"
#include "../common/edm_monitor_constant.h"
#include "../common/edm_monitor_struct.h"
#include "AdEdmMonitor.h"

NS_BEGIN(xce, edm)

const int UNIT = 100;

class EdmFilter : public IceUtil::Shared {
public:
    virtual bool IsOffline(const EdmInfo& edmInfo) = 0;
	  virtual void CalcSendRatio(const std::map<Ice::Long, DummyRecordPtr> & records) = 0;  
	///////////////
	//日预投量-日累计投放量 < 阈值  ： ratio = ((日预投量-日累计投放量)/阈值)*0.1
	//总预投量-总累计投放量 < 阈值  ： ratio = ((总预投量-总累计投放量)/阈值)*0.1
	double GetMinDailyRatio(const double & quota, const double & amount, const double & threshold ,const double & limit){
	  if((quota> -0.00000001 && quota < 0.00000001)|| quota < 0 || amount < 0){
	     return 0.0;
	  }
	  if((threshold > -0.00000001 && threshold < 0.00000001) || threshold<0 || limit < 0){
		return 1.0;
	  }
	  double temp = quota - amount;
	  double ratio = 1.0;
	  if(temp < threshold) {
	    ratio = (temp/threshold) * limit;
	  }
	  return ratio;
	}

	//日累计投放量/日预投量 > 90%   :  ratio = 1 - 日累计投放量/日预投量
	//总累计投放量/总预投量 > 90%   :  ratio = 1 - 总累计投放量/总预投量
	double GetMinTotalRatio(const double &quota, const double & amount, const double& threshold){
	  if((quota > -0.00000001 && quota < 0.00000001)|| quota < 0){
	    return 0.0;
	  }
	  if((threshold > -0.00000001&& threshold < 0.00000001)|| threshold < 0){
	    return 1.0;
	  }
	  double temp = amount/quota;
	  double ratio = 1.0;
	  if(temp > threshold){
	    ratio = 1 - temp;
	  }
	  return ratio;
	}

	//总账户余额 < 阈值 ：ratio = 总账户余额/阈值*0.1
	double GetMemberBalanceRatio(const double & balance,const double & threshold, const double& limit){
	  if((balance > -0.00000001 && balance < 0.00000001) || balance < 0){
	     return 0.0;	  
	  }
	  if((threshold > -0.00000001 && threshold < 0.00000001)|| threshold < 0 || limit < 0){
	    return 1.0;
	  }
	  double ratio = 1.0;
	  if(balance < threshold){
	    ratio = (balance/threshold) * limit;
	  }
	  return ratio;
	}
	/////////////

protected:
    bool SelectByRand(double limit){
      int modValue = rand() % UNIT;
	    double result = 1.0 * modValue / UNIT;
	    MCE_INFO("EdmFilter::SelectByRand() --> result:" << result << " limit:"<< limit);
	    return  result > limit;
    }

protected:
    IceUtil::RWRecMutex mutex_;    
};

typedef IceUtil::Handle<EdmFilter> EdmFilterPtr;

NS_END

#endif
