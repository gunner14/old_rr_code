#ifndef TALKFUNCTIONADAPTER_H_
#define TALKFUNCTIONADAPTER_H_

#include "TalkFunction.h"
#include "AdapterI.h"

namespace com{
namespace xiaonei{
namespace talk{
namespace adapter {
namespace function{
using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::function;
using namespace com::xiaonei::talk::common;
using namespace MyUtil;

//class TalkFunctionAdapter : public MyUtil::ReplicatedClusterAdapterI<TalkFunBasePrx>, public MyUtil::Singleton<TalkFunctionAdapter> {
class TalkFunctionAdapter : public MyUtil::ReplicatedClusterAdapterI<TalkFunBasePrx>, public IceUtil::Shared{
public:
 TalkFunctionAdapter(const string& controllername, const MyUtil::ChannelPtr& channel = new XceFeedControllerChannel):
	MyUtil::ReplicatedClusterAdapterI<TalkFunBasePrx> (controllername,120,300, channel), count(0) {

	}
	

	void Notify(const MyUtil::Str2StrMap& mp);

protected:

  int count;
	TalkFunBasePrx getManager(int id);
	TalkFunBasePrx getManagerOneway(int id);


};
typedef IceUtil::Handle<TalkFunctionAdapter> TalkFunctionAdapterPtr;


};
};
};
};
};

#endif /*TALKLOGICADAPTER_H_*/
