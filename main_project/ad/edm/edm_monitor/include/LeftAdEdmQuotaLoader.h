/*
 * =====================================================================================
 *
 *       Filename:  LeftAdEdmQuotaLoader.h
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

#ifndef __XCE_EDM_LEFT_AD_QUOTA_LOADER_H_
#define __XCE_EDM_LEFT_AD_QUOTA_LOADER_H_

#include "EdmQuotaLoader.h"

NS_BEGIN(xce, edm)

class LeftAdEdmQuotaLoader : public EdmQuotaLoader{
public:
    virtual ~LeftAdEdmQuotaLoader(){}
    virtual DummyRecordPtr GetQuota(const EdmInfo& edmInfo);

    virtual void UpdateData();
    virtual void Clear();
    bool Parse(const std::string&, std::map<Ice::Long, DummyRecordPtr>&);
	  virtual std::map<Ice::Long,DummyRecordPtr> GetAdGroupRecords(){
      IceUtil::RWRecMutex::RLock lock(mutex_);
	    return campaigns_;
	  }
private:
	void FillMemberBalance();
private:
    std::map<Ice::Long, DummyRecordPtr> campaigns_;
	std::map<Ice::Long, Ice::Long> members2Balance_;
};

NS_END

#endif
