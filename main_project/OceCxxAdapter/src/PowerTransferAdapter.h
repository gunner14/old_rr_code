#ifndef __POWERTRANSFER_ADAPTER_H__
#define __POWERTRANSFER_ADAPTER_H__

#include "AdapterI.h"
#include "PowerTransfer.h"
#include "RFeed.h"
namespace com {
namespace xiaonei{
namespace talk {
namespace function{
namespace transfer{
class PowerTransferAdapter : public MyUtil::ReplicatedClusterAdapterI<PowerTransferPrx>, public MyUtil::Singleton<PowerTransferAdapter> {
public:
        PowerTransferAdapter() : MyUtil::ReplicatedClusterAdapterI<PowerTransferPrx> ("ControllerPowerTransfer", 120, 300, new MyUtil::XceFeedControllerChannel){
		_managers.resize(cluster() > 0 ? cluster() : 1);
                _managersOneway.resize(cluster() > 0 ? cluster() : 1);
        }	
	void deliverFeedWithFocusfriend(const ::xce::feed::FeedSeedPtr& feed, const MyUtil::IntSeq& target, const MyUtil::IntSeq& focusfriendSeq);
protected:
	virtual string name() {
		return "M";
	}
	virtual string endpoints() {
		return "@PowerTransfer";
	}
	virtual size_t cluster() {
		return 0;
	}
        PowerTransferPrx getManagerOneway(int id);
        PowerTransferPrx getManager(int id);
        vector<PowerTransferPrx> _managersOneway;
        vector<PowerTransferPrx> _managers;

};
};
};
}
}
};

#endif
