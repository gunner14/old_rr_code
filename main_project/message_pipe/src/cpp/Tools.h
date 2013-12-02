#ifndef _MP_TOOLS_H__
#define _MP_TOOLS_H__



#include "Common.h"
#include "Producer.h"
#include "MessageData.h"
#include "Consumer.h"

namespace xce {
namespace messagepipe{

class ProducerTool : public IceUtil::Thread {

public:
	static int messageCount_ ;
	static int messageSize_;
	static volatile int parallelThreads_;
	static std::string cluster_;
	static std::string subject_;

    virtual void run() {
        std::cout << "thread start at " << time(NULL) << std::endl;
        try {
            sendLoop();
        } catch (std::exception& e) {
            std::cout << "exception : " << e.what() << std::endl;
        }
        --parallelThreads_;
        std::cout << "thread end at " << time(NULL) << std::endl;
    }
    void sendLoop() {

        while(true){
            for (int i = 0; i < messageCount_ || messageCount_ == 0; i++) {
                xce::messagepipe::Producer::getInstance()->send("localhost:2181",createMessageText(i));
            }
        }
    }

  MessageDataPtr createMessageText(int index) {
		std::vector<unsigned char> bytes(messageSize_);
		MessageDataPtr data(new MessageData(cluster_, subject_, bytes));
		return data;
	}

	static void setMessageCount(int messageCount) {
		messageCount_ = messageCount;
	}

	static void setMessageSize(int messageSize) {
		messageSize_ = messageSize;
	}

	static void setSubject(const std::string& subject) {
		subject_ = subject;
	}

	static void setParallelThreads(int parallelThreads) {
		if (parallelThreads < 1) {
			parallelThreads = 1;
		}
		parallelThreads_ = parallelThreads;
	}
	
};
typedef boost::shared_ptr<ProducerTool> ProducerToolPtr;

class ConsumerTool : virtual Consumer {

public:
	static volatile int parallelThreads_;
	static std::string subject_;
	static std::string cluster_;
	ConsumerTool(const std::string& zk_address, const std::string& cluster, const std::string& subject) : Consumer(zk_address, cluster, subject){
	}


	static void setSubject(const std::string& subject) {
		subject_ = subject;
	}

	static void setParallelThreads(int parallelThreads) {
		if (parallelThreads < 1) {
			parallelThreads = 1;
		}
		parallelThreads_ = parallelThreads;
	}

	virtual void onMessage(const std::vector<MessageDataPtr>& receiveMessages) {
		std::cout << "receive Message size :"  << receiveMessages.size() << std::endl;
	}
};

typedef boost::shared_ptr<ConsumerTool> ConsumerToolPtr;
}
}
#endif
