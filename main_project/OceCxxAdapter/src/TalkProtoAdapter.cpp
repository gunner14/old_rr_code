#include "TalkProtoAdapter.h"

using namespace com::xiaonei::talk::common;
using namespace com::xiaonei::talk::proto;
using namespace MyUtil;


TalkProtoManagerPrx TalkProtoAdapter::getManager(int id) {
	return getProxy(id);
}

TalkProtoManagerPrx TalkProtoAdapter::getManagerOneway(int id) {
	return getProxyOneway(id);
}

void TalkProtoAdapter::Send(const JidPtr& actor, const string& xml){
	if(++count > 1000){
		count = 0;
	}
	getManagerOneway(count)->Send(actor, xml);
}
void TalkProtoAdapter::Receive(const Str2StrMap& map){
	if(++count > 1000){
		count = 0;
	}
	getManagerOneway(count)->Receive(map);
}
void TalkProtoAdapter::BatchReceive(const Str2StrMapSeq& map){
	if(++count > 1000){
		count = 0;
	}
	getManagerOneway(count)->BatchReceive(map);
}
void TalkProtoAdapter::ReceiveExpress(const Str2StrMap& maps){
	if(++count > 1000){
		count = 0;
	}
	getManagerOneway(count)->ReceiveExpress(maps);
}
void TalkProtoAdapter::BatchReceiveExpress(const Str2StrMapSeq& maps){
	if(++count > 1000){
		count = 0;
	}
	getManagerOneway(count)->BatchReceiveExpress(maps);
}
