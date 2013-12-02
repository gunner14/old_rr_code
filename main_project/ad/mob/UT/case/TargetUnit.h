#ifndef __TARGET_UNIT_TEST__
#define __TARGET_UNIT_TEST__

#include "ad/mob/UT/code/Handler.h"
#include "ad/mob/UT/stub/Data.h"
#include "gtest/gtest.h"

using namespace xce::ad;

class TargetUnitTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}
};

TEST_F(TargetUnitTest, 1) {
	AdBidUnit bid_unit;
	bid_unit.set_group_id(  );
	bid_unit.set_bid_unit_id( );
	bid_unit.set_member_id( );
	bid_unit.set_gender( );
	bid_unit.set_stage( );
	bid_unit.set_age( );
	bid_unit.set_grade( );
	bid_unit.set_school( );
	bid_unit.set_area( );

	bid_unit.set_platform( );
	bid_unit.set_network( );
	bid_unit.set_brand( mysql_string(row["cast_brand_3G"]) );
	bid_unit.set_resolution( mysql_string(row["cast_resolution_3G"]) );
	bid_unit.set_lbs_flag( mysql_int(row["cast_lbs_3G"]) );
	bid_unit.set_lbs_pos( mysql_string(row["cast_pos"]) );
	bid_unit.set_lbs_radius( mysql_double(row["cast_radius"]) );
	
	HandlerPara para;
	para.handle_flag_ = HandlerPara::UNHANDLED;
	BrandRotateHandler handler;
	handler.handle( para );
	EXPECT_EQ(-1, para.selected_group_ptr_);
	EXPECT_EQ(HandlerPara::UNHANDLED, para.handle_flag_);
}

#endif
