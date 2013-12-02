#ifndef __POOL_STUB_H__
#define __POOL_STUB_H__

#include "ServiceI.h"
#include "ad/mob/UT/code/Struct.h" 
#include <vector>

class ZonePoolStub : public MyUtil::Singleton<ZonePoolStub> {
public:
	ZonePoolStub();
	xce::ad::AdZonePtr GetAdZonePass(Ice::Long);
	xce::ad::AdZonePtr GetAdZoneFail(Ice::Long);
	xce::ad::AdZonePtr GetAdZoneNull(Ice::Long);
	
	std::vector<xce::ad::AdZonePtr> pool_pass_;
	std::vector<xce::ad::AdZonePtr> pool_fail_;
};

#endif
