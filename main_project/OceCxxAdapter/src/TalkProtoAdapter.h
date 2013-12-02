#ifndef TALKPROTOADAPTER_H_
#define TALKPROTOADAPTER_H_

#include "TalkProto.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace com{
namespace xiaonei{
namespace talk{
namespace proto{
using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::common;
using namespace MyUtil;

class TalkProtoAdapter : public MyUtil::ReplicatedClusterAdapterI<TalkProtoManagerPrx>, public MyUtil::Singleton<TalkProtoAdapter>  {
public:

	TalkProtoAdapter() : MyUtil::ReplicatedClusterAdapterI<TalkProtoManagerPrx> ("ControllerTalkProto",120,300,new XceFeedControllerChannel) {
		_managers.resize(cluster() > 0 ? cluster() : 1);
		_managersOneway.resize(cluster() > 0 ? cluster() : 1);
		count = 0;
	}
	
	void Send(const JidPtr& actor, const string& xml);
	void Receive(const Str2StrMap& map);
	void BatchReceive(const Str2StrMapSeq& maps);
	void BatchReceiveExpress(const Str2StrMapSeq& maps);
	void ReceiveExpress(const Str2StrMap& maps);
  

	size_t getCluster(){
		return 1;
	};

protected:
	virtual string name() {
		return "TalkProto";
	}
	virtual string endpoints() {
		return "@TalkProto";
	}
	virtual size_t cluster() {
		return 1;
	}

	TalkProtoManagerPrx getManager(int id);
	TalkProtoManagerPrx getManagerOneway(int id);

	vector<TalkProtoManagerPrx> _managersOneway;
	vector<TalkProtoManagerPrx> _managers;

	int count;
};

};
};
};
};


#endif /*TALKLOGICADAPTER_H_*/
