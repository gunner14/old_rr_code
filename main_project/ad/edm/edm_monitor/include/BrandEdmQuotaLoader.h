/*
 * =====================================================================================
 *
 *       Filename:  BrandEdmQuotaLoader.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年04月26日
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lei.yu1 
 *        Company:  renren
 *
 * =====================================================================================
 */

#ifndef __XCE_EDM_BRAND_QUOTA_LOADER_H_
#define __XCE_EDM_BRAND_QUOTA_LOADER_H_

#include "EdmQuotaLoader.h"

NS_BEGIN(xce, edm)

class BrandEdmQuotaLoader : public EdmQuotaLoader{
public:
    virtual DummyRecordPtr GetQuota(const EdmInfo&);

    virtual void UpdateData();
    virtual void Clear();
    virtual bool Parse(const std::string& result, std::map<Ice::Long, DummyRecordPtr>& records);
	  virtual std::map<Ice::Long,DummyRecordPtr> GetAdGroupRecords(){
  	  IceUtil::RWRecMutex::RLock lock(mutex_);
	    return adGroups_;
	  }
private:
	void FillMemberBalance();
private:
    std::map<Ice::Long, DummyRecordPtr> adGroups_;
    std::map<Ice::Long, Ice::Long> members2Balance_;
};

NS_END

#endif
