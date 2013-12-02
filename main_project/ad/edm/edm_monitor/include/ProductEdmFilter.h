/*
 * =====================================================================================
 *
 *       Filename:  ProductEdmFilter.h
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

#ifndef __XCE_PRODUCT_EDM_FILTER_H_
#define __XCE_PRODUCT_EDM_FILTER_H_

#include "EdmFilter.h"
#include "AdEdmMonitor.h"
#include "../common/ns_common.h"
#include "../common/edm_monitor_constant.h"
#include "ProductEdmQuotaLoader.h"

NS_BEGIN(xce, edm)

class ProductEdmFilter : public EdmFilter{
public:

  virtual bool IsOffline(const EdmInfo& edmInfo);
  virtual void CalcSendRatio(const std::map<Ice::Long, DummyRecordPtr> & records);
  double GetSendRatio(const Ice::Long & campaignID);
private:
  std::map<Ice::Long, double> campaignID2Ratio_;
};

NS_END

#endif
