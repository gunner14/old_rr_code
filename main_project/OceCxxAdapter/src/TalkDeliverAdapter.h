#ifndef TALKDELIVERADAPTER_H_
#define TALKDELIVERADAPTER_H_

#include "TalkDeliver.h"
#include <vector>
#include "Singleton.h"
#include "AdapterI.h"

namespace mop {
namespace hi {
namespace svc {
namespace adapter {
using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::common;
using namespace MyUtil;

//class TalkDeliverAdapter : public AdapterI,
//	public AdapterISingleton<MyUtil::TalkChannel, TalkDeliverAdapter> {
class TalkDeliverAdapter : 
 public MyUtil::ReplicatedClusterAdapterI<DeliverManagerPrx>, public MyUtil::Singleton<TalkDeliverAdapter>  {

public:
   //	TalkDeliverAdapter() {
	TalkDeliverAdapter() : MyUtil::ReplicatedClusterAdapterI<DeliverManagerPrx> ("ControllerTalkDeliver",120,300,new XceFeedControllerChannel) {
     /*
		_managers.resize(cluster() > 0 ? cluster() : 1);
		_managersOneway.resize(cluster() > 0 ? cluster() : 1);
     */
	}
	void deliver(const MessageSeq& seq);
	void deliver(const MessagePtr& seq);
	void deliverUnVerify(const MessagePtr& msg);
	void deliverUnVerify(const MessageSeq& seq);
	//OfflineMessageSeq queryOfflineMessageSeq(Ice::Int userId);
protected:
  /*
	virtual string name() {
		return "TalkDeliver";
	}
	virtual string endpoints() {
		return "@TalkDeliver";
	}
	virtual size_t cluster() {
		return 12;
	}
  */
	DeliverManagerPrx getManager(int id);
	DeliverManagerPrx getManagerOneway(int id);
  /*
	vector<DeliverManagerPrx> _managersOneway;
	vector<DeliverManagerPrx> _managers;
  */
};

}
;
}
;
}
;
}
;

#endif /*TALKMESSAGEADAPTER_H_*/
