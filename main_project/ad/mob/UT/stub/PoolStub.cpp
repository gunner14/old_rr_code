#include "ad/mob/UT/code/Struct.h"
#include "PoolStub.h"

using namespace xce::ad;

ZonePoolStub::ZonePoolStub() {
	for(int i=0; i<10; i++) {
		pool_fail_[i] = new AdZone();
		pool_pass_[i] = new AdZone();
	}
	pool_fail_[0]->set_zone_id(100000000000);
	pool_fail_[1]->set_zone_id(111111111111);
	pool_fail_[2]->set_zone_id(222222222222);
	pool_fail_[3]->set_zone_id(333333333333);
	pool_fail_[4]->set_zone_id(444444444444);

	pool_pass_[0]->set_zone_id(100000000098);
	pool_pass_[1]->set_zone_id(100000000099);
}

AdZonePtr ZonePoolStub::GetAdZonePass(Ice::Long zone_id) {
	if ( zone_id == 100000000098 ) {
		return pool_pass_[5];
	} else if ( zone_id == 100000000099 ) {
		return pool_pass_[6];
	} else {
		return NULL;
	}
}

AdZonePtr ZonePoolStub::GetAdZoneFail(Ice::Long zone_id) {
	if ( zone_id <5 ) {
		return pool_fail_[zone_id];
	} else {
		return NULL;
	}
}

AdZonePtr ZonePoolStub::GetAdZoneNull(Ice::Long zone_id) {
	return NULL;
}
