#ifndef __XCE_AD_EDM_MONITOR_STRUCT_H_
#define __XCE_AD_EDM_MONITOR_STRUCT_H_

#include "ns_common.h"
#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>
#include "edm_monitor_constant.h"
#include "../../edm_sender/src/EdmStruct.h"

NS_BEGIN(xce, edm)

struct DummyRecord : public IceUtil::Shared{
};

typedef IceUtil::Handle<DummyRecord> DummyRecordPtr;

struct AdGroupRecord : public DummyRecord{
public:
    AdGroupRecord() : dailyQuota_(INIT_VALUE), dailyAmount_(INIT_VALUE), totalQuota_(INIT_VALUE), totalAmount_(INIT_VALUE) ,memberBalance_(INIT_VALUE),memberId_(INIT_VALUE){}

    Ice::Long dailyQuota_;
    Ice::Long dailyAmount_;
    Ice::Long totalQuota_;
    Ice::Long totalAmount_;
	  Ice::Long memberBalance_;
	  Ice::Long memberId_;
};

struct CampaignRecord : public DummyRecord{
public:
    CampaignRecord() : dailyQuota_(INIT_VALUE), dailyAmount_(INIT_VALUE),memberBalance_(INIT_VALUE),memberId_(INIT_VALUE) {}

    Ice::Long dailyQuota_;
    Ice::Long dailyAmount_;
	  Ice::Long memberBalance_;
	  Ice::Long memberId_;
};

NS_END

#endif
