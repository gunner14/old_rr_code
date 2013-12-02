#ifndef __MP_CONSUMER_H
#define __MP_CONSUMER_H

#include <IceUtil/Thread.h>
#include "Common.h"
#include "ClusterManager.h"
#include "node_set.h"
#include "Producer.h"

namespace xce {
namespace messagepipe{

class PrefetchThread : public IceUtil::Thread {

    public:

	PrefetchThread(const std::string& zk_address,
			 const std::string& cluster, const std::string& subject,bool is_topic=false, size_t batch_size=1000) {
		zk_address_ = zk_address;
		cluster_ = cluster;
		subject_ = subject;
    is_topic_ = is_topic;
    batch_size_ = batch_size;
    failCount_ = 0;
		start();
	}
    private:
        
	std::string zk_address_;
	std::string cluster_;
	std::string subject_;
  bool is_topic_;
	size_t batch_size_;
  std::vector<MessageDataPtr> data_;
  IceUtil::Monitor<IceUtil::Mutex> mutex_;
  int failCount_;

    public:
        std::vector<MessageDataPtr> getData()
        {
            std::vector<MessageDataPtr> data;
            {
            IceUtil::Monitor<IceUtil::Mutex>::Lock lock(mutex_);
            if(data_.empty()) {
                mutex_.wait();
            }
            data.swap(data_);
            mutex_.notify();
            }
            return data;
        }

    protected:

        void run()
        {
            int node_index = -1;
            while(true){
                try {
		                NodeSetPtr nodes = ClusterManager::getInstance()->getConnection(zk_address_, cluster_);
			
		                std::vector<MessageDataPtr> data;
		                node_index = nodes->receive(is_topic_, subject_, 10, data, node_index);
		                MCE_DEBUG("PrefetchThread::run subject:"<< subject_ <<" receive:" << data.size());
		                if (!data.empty()) {
                        failCount_ = 0;
		                    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(mutex_);
                        if(data_.size() == batch_size_) {
                            mutex_.wait();
                        }
		                    data_.insert(data_.end(), data.begin(), data.end());
		                    mutex_.notify();
		                }else {
                        if(++failCount_ == 100) {
                            failCount_ = 0;
                            MCE_DEBUG("PrefetchThread::run sleep 100 msec for zkaddress: " << zk_address_ << " cluster: " << cluster_ << " subject: " << subject_);
                            usleep(100*1000);
                        }
                    }
                    continue;
                } catch (cms::CMSException& ex ){
                    MCE_WARN("PrefetchThread::run cmsexception : "  << ex.getMessage());
                } catch (std::exception& ex) {
                    MCE_WARN("PrefetchThread::run exception : " << ex.what());
                } catch (...){
                    MCE_WARN("PrefetchThread::run unknown_exception ");
                }
                ++node_index;
            }
        }

};


class Consumer : virtual IceUtil::Thread {

protected:
	std::string zk_address_;
	std::string cluster_;
	std::string subject_;
  bool is_topic_;
	size_t batch_size_;

  PrefetchThread* pThread;
public:
	Consumer(const std::string& zk_address,
			 const std::string& cluster, const std::string& subject,bool is_topic=false, size_t batch_size=1000) {
		zk_address_ = zk_address;
		cluster_ = cluster;
		subject_ = subject;
    is_topic_ = is_topic;
    batch_size_ = batch_size;
    pThread = new PrefetchThread(zk_address, cluster, subject, is_topic, batch_size);
		start();
	}

    virtual ~Consumer(){}

    virtual void onMessage(const std::vector<MessageDataPtr>& receiveMessages)=0;

    virtual void run() {
				while (true) {
						std::vector<MessageDataPtr> data = pThread->getData();
		
		        MCE_DEBUG("Consumer::run subject:"<< subject_ <<" receive:" << data.size());
						if (!data.empty()) {
							onMessage(data);
						} 
		    }
    }

std::string getCluster() {
    return cluster_;
}

};

}
}

#endif
