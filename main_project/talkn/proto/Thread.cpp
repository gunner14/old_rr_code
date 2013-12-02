#include "Thread.h"
#include "IMStormAdapter.h"
#include "Deliver.h"
#include "XmppTools.h"
#include "TalkUtil/src/expatxml.h"

using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::util;
using namespace xce::talk;


void DealThread::SendMsg(const MessageSeq& seq){
	MessageSeq mseq;
	for (size_t i = 0; i < seq.size(); ++i) {
		MessagePtr m = seq.at(i);
		if(m->to->endpoint[0] == 'T')
		{
			ExpatTool et;
			if(et.IsValid(m->msg)){
				mseq.push_back(m);
				//MCE_DEBUG("DealThread::SendMsg --> mseq xml prase success");
			}
			else{
				MCE_WARN("DealThread::SendMsg -->Expat parse InValid  to endpoint:"<< m->to << " Invalid msg is " << m->msg);
			}
		}
		else{
			mseq.push_back(m);
		}
	}
	try {
		if((mseq.at(0)->to->endpoint[0] == 'T') || (mseq.at(0)->to->endpoint[0] == 'W') || (mseq.at(0)->to->endpoint[0] == 'P')){
			if(mseq.at(0)->to->endpoint.length() >= 8 && "PIMStorm" == mseq.at(0)->to->endpoint.substr(0, 8)){
				//MCE_DEBUG("PostPipe::handle -->IMStormAdapter::delivere-->to endpoint:"<< mseq.at(0)->to << " mseq size=" << mseq.size());
				IMStormAdapter::instance().deliver(mseq);
			}
			else{
				//split
				MCE_DEBUG("PostPipi::handle -->DeliverManagerI::deliver--->to endpoint:" << jidToString(mseq.at(0)->to) << " msg:" << mseq.at(0)->msg);
				TalkDeliver::instance().getProxyServerOneway(mseq.at(0)->to)->deliver(mseq);
			}
		}
	} catch(Ice::Exception& e) {
		MCE_WARN("DealThread::SendMsg --> ProxyAdapter::delivere-->"<<e<<"  to endpoint:"<< mseq.at(0)->to->endpoint[0]);
	}

}




