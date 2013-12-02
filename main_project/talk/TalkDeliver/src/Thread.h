#ifndef THREAD_H_
#define THREAD_H_ 

#include "TalkCommon.h"
#include "Singleton.h"
#include "TaskManager.h"
#include "XmppTools.h"

namespace com{
namespace xiaonei{
namespace talk{


using namespace com::xiaonei::talk::common;
using namespace MyUtil;

static const int MAX_SIZE = 5000;
static const int T_POOL_SIZE = 1;

static int CHANNEL = 1;

class DealThread: public MyUtil::Timer{
public:
	DealThread(string endpoint, int index): Timer(20, index % 13){
		endpoint_ = endpoint;
		index_ = index;
	}
	void push(const MessageSeq& seq){
		IceUtil::Mutex::Lock lock(mutex_);
		int size = Size();
		if(size > MAX_SIZE){
			MCE_WARN("DealThread::push-->endpoint: " << endpoint_ << " index:" << index_ << " size:" << size << " max:" << MAX_SIZE << " from:" << seq.at(0)->from << " to:" << seq.at(0)->to <<  " msg:" << seq.at(0)->msg);
			return;
		}
    // fix message sequence bug by jian.wang3
		//msgseq_.insert(msgseq_.begin(), seq.begin(), seq.end());
		msgseq_.insert(msgseq_.end(), seq.begin(), seq.end());
	}
	void push(const MessagePtr& msg){
		IceUtil::Mutex::Lock lock(mutex_);
		int size = Size();
		if(size > MAX_SIZE){
			MCE_WARN("DealThread::push-->endpoint: " << endpoint_ << " index:" << index_ << "size:" << size << " max:" << MAX_SIZE);
			return;	
		}
		msgseq_.push_back(msg);
	}


	virtual void handle(){
		//MCE_INFO("DealThread:handle-->endpoint: " << endpoint_ << " index:" << index_ << " start" );
		MessageSeq seq;
		{
			IceUtil::Mutex::Lock lock(mutex_);
			seq.swap(msgseq_);	
		}
		if(!seq.empty()){
			MCE_INFO("DealThread:handle-->endpoint: " << endpoint_ << " index:" << index_ << " delver seq.size:" << seq.size());
			SendMsg(seq);
		}
	}

private:
	size_t Size(){
		return msgseq_.size();
	}
	
	void SendMsg(const MessageSeq& seq);

	MessageSeq msgseq_;
	IceUtil::Mutex mutex_;
	string endpoint_;
	int index_;
};

typedef IceUtil::Handle<DealThread> DealThreadPtr;
typedef vector<DealThreadPtr> DealThreadSeq;
typedef map<string, DealThreadSeq> DealThreadSeqMap;


class  DealThreadPool : public IceUtil::Shared{
public:
		

	void push(const string& endpoint, const MessageSeq& seq){
		if(seq.empty()){
			return;
		}
		{
			IceUtil::RWRecMutex::RLock lock(rwmutex_);
			DealThreadSeqMap::iterator it = threads_.find(endpoint);
			if(it != threads_.end()){
				int index = seq.at(0)->to->userId % T_POOL_SIZE;
				it->second.at(index)->push(seq);
				return;
			}
		}
		{
			IceUtil::RWRecMutex::WLock lock(rwmutex_);
			DealThreadSeq tseq;
			for(int i = 0; i < T_POOL_SIZE; ++i){
				//srand(time(NULL));
				//int channel = rand() % 19;
				MCE_INFO("DealThreadPool::channel-->" << CHANNEL);
				DealThreadPtr p = new DealThread(endpoint, CHANNEL);
				CHANNEL++;
				tseq.push_back(p);
				TaskManager::instance().scheduleRepeated(p);	
			}
			threads_[endpoint] = tseq;

			int index = seq.at(0)->to->userId % T_POOL_SIZE;
			tseq.at(index)->push(seq);
		}

	}

	void push(const string& endpoint, const MessagePtr& msg){
		MessageSeq seq;
		seq.push_back(msg);
		push(endpoint, seq);
	}



	DealThreadSeqMap threads_;
	IceUtil::RWRecMutex rwmutex_;	
};


};
};
};


#endif
