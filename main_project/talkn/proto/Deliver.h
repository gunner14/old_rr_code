#ifndef DELIVERI_H_
#define DELIVERI_H_

#include <Ice/Ice.h>
#include <IceUtil/AbstractMutex.h>
#include <IceUtil/Mutex.h>
#include "TalkProxy.h"
#include "ServiceI.h"
#include "TaskManager.h"
#include "Singleton.h"
#include "Pipe.h"
#include "Thread.h"
namespace com {
namespace xiaonei {
namespace talk {

using namespace com::xiaonei::talk::common;
using namespace MyUtil;
class PostPipe : public Pipe {
public:
	PostPipe(const string& name, int index) :
		Pipe(name, index) {
	}
	;
	virtual void handle(const ObjectSeq& seq);
};

class PipeFactoryI : public PipeFactory {
public:
	virtual PipePtr create(const string& type, int index) {
    PipePtr pipe;
    if (type == "MsgPost")
		  pipe = new PostPipe(type, index);
    return pipe;
	}
};

//----------------------------------------------------------------------------
class TalkDeliver: public Singleton<TalkDeliver> {
private:
	map<string, DeliverInterfacePrx> _manager;
	map<string, DeliverInterfacePrx> _managerOneway;
	IceUtil::Mutex _mutex;
	DealThreadPool threadpool_;
public:
	void deliver(const MessageSeq& seq);

	DeliverInterfacePrx getProxyServer(const JidPtr& ptr);
	DeliverInterfacePrx getProxyServerOneway(const JidPtr& ptr);
};

}
;
}
;
}
;

#endif /*MESSAGEI_H_*/
