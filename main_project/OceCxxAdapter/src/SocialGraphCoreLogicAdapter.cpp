#include "SocialGraphCoreLogicAdapter.h"

using namespace xce::socialgraph;

void SocialGraphCoreLogicAdapter::ReportUserLogin(int userid) {
	SGMessage message;
	message.id = userid;
	message.action = USERLOGIN;

	getProxyOneway(userid)->SendSGMessage(message);	
}

void SocialGraphCoreLogicAdapter::ReportBuddyApply(int userid, int friendid) {
	SGMessage message;
	message.source = (long)userid;
	message.target = (long)friendid;
	message.action = BUDDYAPPLY;

	getProxyOneway(userid)->SendSGMessage(message);	
}

void SocialGraphCoreLogicAdapter::ReportAddPage(int userid, long pageid) {
	SGMessage message;
	message.source = (long)userid;
	message.target = (long)pageid;
	message.action = ADDPAGE;

	getProxyOneway(userid)->SendSGMessage(message);	
}

void SocialGraphCoreLogicAdapter::ReportRefresh(int userid) {
	SGMessage message;
	message.id = userid;
	message.action = REFRESH;

	getProxyOneway(userid)->SendSGMessage(message);	
}
