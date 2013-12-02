#include "Deliver.h"
#include "ConnectionQuery.h"
#include "TalkProxy.h"
#include "Date.h"
#include "XmppTools.h"
#include "TaskManager.h"
#include "MessageType.h"
#include "TalkUtil/src/expatxml.h"
#include "IMStormAdapter.h"
//#include "WebEventAdapter.h"
using namespace xce::talk;
using namespace com::xiaonei::xce;
//using namespace mop::hi::svc::adapter;
using namespace MyUtil;
using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk;

void PostPipe::handle(const ObjectSeq& seq) {
	//MCE_INFO("PostPipe::handle --> thread_id = " << pthread_self());
	if (seq.empty()) {
		return;
	} else {
		//MCE_DEBUG("PostPipe::handle --> objectseq size = " <<seq.size());
	}
	//MCE_DEBUG("PostPipe::handle --> size:" << seq.size());
	MessageSeq mseq;
	for (size_t i = 0; i < seq.size(); ++i) {
		MessagePtr m = MessagePtr::dynamicCast(seq.at(i));
		if(MemcGetOnlineType(m->to->endpoint) & 4)
		{
			ExpatTool et;
			if(et.IsValid(m->msg)){
				mseq.push_back(m);
				MCE_DEBUG("PostPipe::handle --> mseq xml prase success");
			}
			else{				
				MCE_WARN("PostPipe::handle -->Expat parse InValid  to endpoint:"<< m->to << " Invalid msg is " << m->msg);
			}
		}
		else{
			mseq.push_back(m);
		}
	}
	try {
		//if((mseq.at(0)->to->endpoint[0] == 'T') || (mseq.at(0)->to->endpoint[0] == 'W') || (mseq.at(0)->to->endpoint[0] == 'P')){
    int ot = MemcGetOnlineType(mseq.at(0)->to->endpoint);
		if(ot&4 || (ot&2) || (ot&8)){
			if(16 & ot){
				IMStormAdapter::instance().deliver(mseq);
			}
			else{
				TalkDeliver::instance().getProxyServerOneway(mseq.at(0)->to)->deliver(mseq);
			}
		}
	} catch(Ice::Exception& e) {
		MCE_WARN("PostPipe::handle -->ProxyAdapter::delivere-->"<<e<<"  to endpoint:"<< mseq.at(0)->to);
	}
}

//--------------------------------------------------------------------------------

DeliverInterfacePrx TalkDeliver::getProxyServer(const JidPtr& ptr) {
	IceUtil::Mutex::Lock lock(_mutex);
	map<string,DeliverInterfacePrx>::iterator it = _manager.find(ptr->endpoint);
	if (it != _manager.end()) {
		return it->second;
	}
	Ice::ObjectPrx prx = ServiceI::instance().getAdapter()->getCommunicator()->stringToProxy("M@"+ptr->endpoint);
	DeliverInterfacePrx dPrx = DeliverInterfacePrx::uncheckedCast(prx);
	_manager[ptr->endpoint] = dPrx;
	return dPrx;
}
DeliverInterfacePrx TalkDeliver::getProxyServerOneway(const JidPtr& ptr) {
	IceUtil::Mutex::Lock lock(_mutex);

	map<string,DeliverInterfacePrx>::iterator it =
			_managerOneway.find(ptr->endpoint);
	if (it != _managerOneway.end()) {
		return it->second;
	}
	Ice::ObjectPrx prx = ServiceI::instance().getAdapter()->getCommunicator()->stringToProxy("M@"+ptr->endpoint);
	DeliverInterfacePrx dPrx =
			DeliverInterfacePrx::uncheckedCast(prx->ice_oneway());
	_managerOneway[ptr->endpoint] = dPrx;
	return dPrx;
}
void TalkDeliver::deliver(const MessageSeq& seq) {
	MCE_DEBUG("TalkDeliver::deliver --> seq.size="<<seq.size());
	if (!seq.empty()) {
		if (seq.at(0)->to->userId == 128487631 || seq.at(0)->to->userId == 200865373) {
			//MCE_DEBUG("TalkDeliver::deliver --> msg:"<< seq.at(0)->msg);
		}
	}
	for(MessageSeq::const_iterator it = seq.begin(); it != seq.end(); ++it){
		MessagePtr m = (*it);
		if(m->type == MESSAGE){
			MCE_INFO("TalkDeliver::deliver message-->from=" << jidToString(m->from) << " to=" << jidToString(m->to));
		}
		if(m->type == NOTIFY2_MESSAGE){
			MCE_INFO("TalkDeliver::deliver notify2-->from=" << jidToString(m->from) << " to=" << jidToString(m->to));
		}
	}
	for (size_t i = 0; i < seq.size(); ++i) {
		//PipePool::instance().push("MsgPost", abs(seq.at(i)->to->userId)%10, seq.at(i));
		threadpool_.push(seq.at(i)->to->endpoint,seq.at(i));
	}
}
