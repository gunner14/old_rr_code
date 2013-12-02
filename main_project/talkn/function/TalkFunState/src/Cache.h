#ifndef CACHE_H_
#define CACHE_H_ 

#include "TalkCommon.h"
#include "Singleton.h"
#include "TaskManager.h"
#include "XmppTools.h"

namespace com{
namespace xiaonei{
namespace talk{
namespace fun{
namespace state{

using namespace com::xiaonei::talk::fun::state;
using namespace com::xiaonei::talk::common;
using namespace MyUtil;
using namespace com::xiaonei::talk::util;


struct ActionInfo{
public:
	ActionInfo(const JidPtr& jid, int actiontype){
		jid_ = jid;
		actiontype_ = actiontype;
	}

	JidPtr jid_;
	int actiontype_;
};


class DoingCache : public IceUtil::Shared{
public:
	bool BeginWrite(const JidPtr& jid, int actiontype);
	void EndWrite(const JidPtr& jid, int actinotype);
	bool Check(const JidPtr& jid, int actiontype);
	void DealDelay();	
	
	typedef map<JidPtr, int> Jid2Action;
	typedef map<int, int> Uid2Action;
	typedef vector<ActionInfo>	ActionInfoSeq;;

	Uid2Action uids_; //check
	ActionInfoSeq delays_; //delay action	

	IceUtil::RWRecMutex rwmutex_;//每个桶内一把读写锁

};
typedef IceUtil::Handle<DoingCache> DoingCachePtr;


};
};
};
};
};


#endif
       
