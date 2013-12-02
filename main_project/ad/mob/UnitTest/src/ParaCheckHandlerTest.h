#ifndef __PARACHECK_HANDLER_TEST__
#define __PARACHECK_HANDLER_TEST__

#include "ad/mob/AdMobEngine/src/Handler.h"
#include "gtest/gtest.h"
#include "Data.h"

using namespace xce::ad;

//Test ParaChechHandler Uid Check
class ParaCheckHandlerTestParas {
public:
	ParaCheckHandlerTestParas() {
		for(set<int>::const_iterator it = uid_list.pass.begin(); it != uid_list.pass.end(); ++it) {
			AdMobReqPara para;
			para.uid = *it;
			para_list_uid_pass.push_back(para);
		}
		for(set<int>::const_iterator it = uid_list.fail.begin(); it != uid_list.fail.end(); ++it) {
			AdMobReqPara para;
			para.uid = *it;
			para_list_uid_fail.push_back(para);
		}
		for(set<int>::const_iterator it_uid = uid_list.pass.begin(); it_uid !=  uid_list.pass.end(); ++it_uid) {
			for(set<std::string>::const_iterator it_net = net_list.pass.begin(); it_net != net_list.pass.end(); ++it_net) {
				for(set<std::string>::const_iterator it_screen = screen_list.pass.begin(); it_screen != screen_list.pass.end(); ++it_screen) {
					for(set<std::string>::const_iterator it_os = os_list.pass.begin(); it_os != os_list.pass.end(); ++it_os) {
						AdMobReqPara para;
						para.uid = *it_uid;
						para.adzoneId = 100000000098;
						para.netStatus = *it_net;
						para.client.screenSize = *it_screen;
						para.client.osVersion = *it_os;
						para_list_req_pass.push_back(para);
					}
				}
			}
		}
		for(set<int>::const_iterator it_uid = uid_list.fail.begin(); it_uid !=  uid_list.fail.end(); ++it_uid) {
			for(set<Ice::Long>::const_iterator it_zone = zone_list.fail.begin(); it_zone != zone_list.fail.end(); ++it_zone) {
				AdMobReqPara para;
				para.uid = *it_uid;
				para.adzoneId = *it_zone;
				para.netStatus = "";
				para.client.screenSize = "";
				para.client.osVersion = "";
				para_list_req_fail.push_back(para);
			}
		}
	}

public:
	vector<AdMobReqPara> para_list_uid_pass;
	vector<AdMobReqPara> para_list_uid_fail;
	vector<AdMobReqPara> para_list_req_pass;
	vector<AdMobReqPara> para_list_req_fail;
	
	Uid_List uid_list;
	Zone_List zone_list;
	Net_List net_list;
	IpArea_List ipArea_list;
	Screen_List screen_list;
	OS_List os_list;
};

class ParaCheckHandler_Test_Uid_Pass : public ::testing::TestWithParam<AdMobReqPara> {
};

class ParaCheckHandler_Test_Uid_Fail : public ::testing::TestWithParam<AdMobReqPara> {
};

class ParaCheckHandler_Test_Req_Pass : public ::testing::TestWithParam<AdMobReqPara> {
};

class ParaCheckHandler_Test_Req_Fail : public ::testing::TestWithParam<AdMobReqPara> {
};

TEST_P(ParaCheckHandler_Test_Uid_Pass, TEST_PASS) {
  const AdMobReqPara& request = GetParam();
  AdMobReqPara request_orgin = request;
  HandlerPara handler_para(request);
  HandlerPtr parachecker = new ParaCheckHandler;
  parachecker->handle(handler_para);
  EXPECT_EQ(request.uid, handler_para.request_.uid);
}

TEST_P(ParaCheckHandler_Test_Uid_Fail, TEST_FAIL) {
  const AdMobReqPara& request = GetParam();
  AdMobReqPara request_orgin = request;
  HandlerPara handler_para(request);
  HandlerPtr parachecker = new ParaCheckHandler;
  parachecker->handle(handler_para);
  EXPECT_EQ(2, handler_para.request_.uid);
}

TEST_P(ParaCheckHandler_Test_Req_Pass, TEST_PASS) {
  const AdMobReqPara& request = GetParam();
  AdMobReqPara request_orgin = request;
  HandlerPara handler_para(request);
  HandlerPtr parachecker = new ParaCheckHandler;
  parachecker->handle(handler_para);
  EXPECT_EQ(request.uid, handler_para.request_.uid);
	EXPECT_EQ(request.adzoneId, handler_para.request_.adzoneId);
	EXPECT_EQ(request.netStatus, handler_para.request_.netStatus);
	EXPECT_EQ(request.client.screenSize, handler_para.request_.client.screenSize);
	EXPECT_EQ(request.client.osVersion, handler_para.request_.client.osVersion);

}

TEST_P(ParaCheckHandler_Test_Req_Fail, TEST_FAIL) {
  const AdMobReqPara& request = GetParam();
  AdMobReqPara request_orgin = request;
  HandlerPara handler_para(request);
  HandlerPtr parachecker = new ParaCheckHandler;
  parachecker->handle(handler_para);
  EXPECT_EQ(2, handler_para.request_.uid);
	EXPECT_EQ(100000000098, handler_para.request_.adzoneId);
	EXPECT_EQ("0100", handler_para.request_.netStatus);
	EXPECT_EQ("320*480", handler_para.request_.client.screenSize);
	EXPECT_EQ("0", handler_para.request_.client.osVersion);

}

const ParaCheckHandlerTestParas para_para_check_handler; 
INSTANTIATE_TEST_CASE_P(Handler_Test, ParaCheckHandler_Test_Uid_Pass, ::testing::ValuesIn(para_para_check_handler.para_list_uid_pass));
INSTANTIATE_TEST_CASE_P(Handler_Test, ParaCheckHandler_Test_Uid_Fail, ::testing::ValuesIn(para_para_check_handler.para_list_uid_fail));
INSTANTIATE_TEST_CASE_P(Handler_Test, ParaCheckHandler_Test_Req_Pass, ::testing::ValuesIn(para_para_check_handler.para_list_req_pass));
INSTANTIATE_TEST_CASE_P(Handler_Test, ParaCheckHandler_Test_Req_Fail, ::testing::ValuesIn(para_para_check_handler.para_list_req_fail));


#endif
