#include "TalkFunctionAdapter.h"

using namespace com::xiaonei::talk::adapter::function;

TalkFunBasePrx TalkFunctionAdapter::getManager(int id) {
	return getProxy(id);
}

TalkFunBasePrx TalkFunctionAdapter::getManagerOneway(int id) {
	return getProxyOneway(id);
}

void TalkFunctionAdapter::Notify(const MyUtil::Str2StrMap& mp){
	if(++count > 1000){
		count = 0;
	}
	MCE_INFO("TalkFunctionAdapter::Notify-->" << mp.size());
	getManagerOneway(count)->notify(mp);;
}
