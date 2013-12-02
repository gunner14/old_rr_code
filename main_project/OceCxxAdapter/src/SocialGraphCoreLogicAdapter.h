#ifndef _SOCIALGRAPH_CORE_LOGIC_ADAPTER_H_
#define _SOCIALGRAPH_CORE_LOGIC_ADAPTER_H_

#include "SocialGraphCoreLogic.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce {
namespace socialgraph {

class SocialGraphCoreLogicAdapter : public MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::SocialGraphCoreLogicPrx>,
	public MyUtil::Singleton<SocialGraphCoreLogicAdapter> {
public:
	SocialGraphCoreLogicAdapter():
		MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::SocialGraphCoreLogicPrx>("ControllerSocialGraphCoreLogic", 120, 1000,
																																								new MyUtil::SocialGraphChannel()) {
	}
	
	/*
	Report User Login message
	*/
	void ReportUserLogin(int userid);

	/*
	Report Buddy Apply message
	*/
	void ReportBuddyApply(int userid, int friendid);

	/*
	Report Add Page message
	*/
	void ReportAddPage(int userid, long pageid);

	/*
	Report User Refresh message
	*/
	void ReportRefresh(int userid);
};

}
}

#endif
