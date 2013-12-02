#ifndef __BRAND_ROTATE_HANDLER_TEST__
#define __BRADN_ROTATE_HANDLER_TEST__

#include "ad/mob/UT/code/Handler.h"
#include "ad/mob/UT/stub/Data.h"
#include "gtest/gtest.h"

using namespace xce::ad;

class BrandRotateHandlerTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}
};

TEST_F(BrandRotateHandlerTest, 1) {
	HandlerPara para;
	para.handle_flag_ = HandlerPara::UNHANDLED;
	BrandRotateHandler handler;
	handler.handle( para );
	EXPECT_EQ(-1, para.selected_group_ptr_);
	EXPECT_EQ(HandlerPara::UNHANDLED, para.handle_flag_);
}

TEST_F(BrandRotateHandlerTest, 2) {
	HandlerPara para;

}

#endif
