/*
 * =====================================================================================
 *
 *       Filename:  BrandEdmFilter.h
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

#ifndef __XCE_BRAND_EDM_FILTER_H_
#define __XCE_BRAND_EDM_FILTER_H_

#include "EdmFilter.h"
#include "../common/ns_common.h"
#include "../common/edm_monitor_constant.h"

NS_BEGIN(xce, edm)

class BrandEdmFilter : public EdmFilter{
public:

   // virtual bool IsOffline(DummyRecordPtr);
  virtual bool IsOffline(const EdmInfo& edmInfo);
  void CalcSendRatio(const std::map<Ice::Long, DummyRecordPtr> & records);
  double GetSendRatio(const Ice::Long & adgroupid);
private:
  std::map<Ice::Long, double> adGroups2Ratio_;
};

NS_END

#endif
