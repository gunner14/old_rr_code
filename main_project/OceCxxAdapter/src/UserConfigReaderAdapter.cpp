#include "UserConfigReaderAdapter.h"

using namespace xce::readeradapter;
using namespace xce::adapter;
using namespace std;
using namespace mop::hi::svc::model;
using namespace MyUtil;
using namespace boost;

//原有的供外部调用的接口
//--------------------------------------------------------------
UserConfigInfoPtr UserConfigReaderAdapter::getUserConfig(int userId, const Ice::Context& ctx) {
	UserConfigInfoPtr info = new UserConfigInfo;
	info->parse(getUserConfigReader(userId)->getUserConfig(userId));
	return info;
}

//新加的供Writer/oldwr调用的接口
//--------------------------------------------------------------
void UserConfigReaderAdapter::setUserConfig(int userId, const MyUtil::Str2StrMap& properities) {
	if (userId <= 0) {
		return;
	}
	std::vector<UserConfigReaderPrx> allProxies = getAllProxySeq(userId);
	for (std::vector<UserConfigReaderPrx>::const_iterator it = allProxies.begin(); it != allProxies.end(); ++it) {
		try {
			(*it)->ice_timeout(10000)->setUserConfig(userId, properities);
		} catch (Ice::Exception& e) {
			MCE_WARN("UserConfigReaderAdapter::setUserConfig Exception id = " << userId << " " << e.what());
		}
	}
}

void UserConfigReaderAdapter::setData(int userId, const Ice::ObjectPtr& data) {
	if (!data) {
		return;
	}

	MyUtil::ObjectResultPtr res = new MyUtil::ObjectResult;
	res->data.insert(make_pair(userId, data));

	std::vector<UserConfigReaderPrx> allProxies = getAllProxySeq(userId);
	for (std::vector<UserConfigReaderPrx>::const_iterator it = allProxies.begin(); it != allProxies.end(); ++it) {
		try {
			(*it)->ice_timeout(10000)->setData(res);
		} catch (Ice::Exception& e) {
			MCE_WARN("UserConfigReaderAdapter::setData Exception id=" << userId << " " << e.what());
		}
	}
}

//获取代理
//--------------------------------------------------------------
UserConfigReaderPrx UserConfigReaderAdapter::getUserConfigReader(int userId) {
	return getProxy(userId);
}
