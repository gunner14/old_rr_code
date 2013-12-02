#ifndef _FEED_XMQ_PRODUCER_H_
#define _FEED_XMQ_PRODUCER_H_
#include<deque>
#include<map>
#include"boost/shared_ptr.hpp"
#include"XMQ/client/cpp/xclient.hpp"
#include"Singleton.h"
#include"LogWrapper.h"
#include"TaskManager.h"
namespace xce {
namespace feed{
using namespace xmq;
using namespace std;
const int XMQ_PRODUCER_TASK_LEVEL_BASE = 10000;
const int kXmqProducerSockets = 4;
const int kMaxMqSize = 10;
typedef boost::shared_ptr<Producer> ProducerPtr;
class XMQProducerResourceManager : public MyUtil::Singleton<XMQProducerResourceManager> {
  public:
    void Init(const deque<string> & endpoints) {
      int key = 0;
      mqSize_ = 0;
      for (deque<string>::const_iterator it = endpoints.begin(); it != endpoints.end()
          && mqSize_ <= kMaxMqSize; ++it) {
         if (!checkAddr(*it)) {
           continue;
         }
         int max_reconnect_times = 2;
         deque<ProducerPtr> pro_queue;
         for (int i = 0; i < kXmqProducerSockets; ++i) { // each mq open 4 socket links with it;
           ProducerPtr pro(new Producer());
           int ret = pro->open(*it);
           while (ret ==-1 && max_reconnect_times>0) { // ret = -1 connecting timeout 
             ret = pro->open(*it);
             max_reconnect_times--;
             MCE_WARN("XMQProducerResourceManager --> open:"<<*it<<"failed reconnect it :"<<2-max_reconnect_times<<"st");
           }
           if (ret == 0) {
             pro_queue.push_back(pro);
             MCE_INFO("XMQProducerResourceManager --> open:"<<*it<<" "<<i<<"st producer successed");
           } else {
             MCE_WARN("XMQProducerResourceManager --> open:"<<*it<<" "<<i<<"st producer finally failed");
           }
         }
         if (pro_queue.empty()) {
           continue;
         }
         producers_.insert(map<int,deque<ProducerPtr> >::value_type(key,pro_queue));
         endpoints_.insert(map<int,string>::value_type(key,*it));
         key++;
         mqSize_++;
      }
    }
    bool UpdateEndPoint(string endpoint) {
      if (!checkAddr(endpoint)) {
        return false;
      }
      {
        IceUtil::RWRecMutex::RLock lock(rwMutex_);
        for (map<int,string>::iterator it = endpoints_.begin(); it != endpoints_.end(); ++it) {
          if ( it->second == endpoint) {
            MCE_INFO("XMQProducerResourceManager::UpdateEndPoint --> the endponit:"<<endpoint<<" already exist");
            return false;
          }
        }
      }
      {
        IceUtil::RWRecMutex::WLock lock(rwMutex_);
        deque<ProducerPtr> pro_queue;
        int key;
        if (unavailableMods_.size()) { 
          key = unavailableMods_.front();
          unavailableMods_.pop_front();
        } else {
          key = endpoints_.size();
        }
        for (int i = 0; i < kXmqProducerSockets; ++i) {
          ProducerPtr pro(new Producer());
          int ret = pro->open(endpoint);
          if (ret == 0) {
            pro_queue.push_back(pro);
          } else {
            MCE_WARN("XMQProducerResourceManager::UpdateEndPoint --> open:"<<endpoint<<" "<<i<<"st producer failed")
          }
        }
        if (pro_queue.empty()) {
          MCE_WARN("XMQProducerResourceManager::UpdateEndPoint --> open:"<<endpoint<<"  producer all failed")
          return false;
        }
        map<int,string>::iterator epit;
        map<int,deque<ProducerPtr> >::iterator prit;
        epit = endpoints_.find(key);
        prit = producers_.find(key);
        if (epit != endpoints_.end()) {
          endpoints_[key] = endpoint;
          prit->second.swap(pro_queue);
        } else if (mqSize_ < kMaxMqSize){
          endpoints_.insert(map<int,string>::value_type(key,endpoint));
          producers_.insert(map<int,deque<ProducerPtr> >::value_type(key,pro_queue));
          mqSize_++;
        } else {
          MCE_WARN("XMQProducerResourceManager::UpdateEndPoint --> open too much mq :"<<mqSize_);
          return false;
        }
      }
      return true;
    }
    ProducerPtr getProducer(int &key) {
      IceUtil::RWRecMutex::RLock lock(rwMutex_);
      
      int count = 1 ;
      while (!checkAvailable(key) && mqSize_) {
        key = (++key) % mqSize_;  
        if ((++count) > mqSize_) {
          return ProducerPtr();
        }
      }
      map<int,deque<ProducerPtr> >::iterator prit = producers_.find(key);
      if ( prit != producers_.end()) {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lock2(monitors_[key % mqSize_]);
        if (prit->second.empty()) {
          monitors_[key % mqSize_].wait();
        }
        ProducerPtr pro = prit->second.front();
        prit->second.pop_front();
        MCE_DEBUG("XMQProducerResourceManager::getProducer --> get mq:"<<getEndPoint(key)<<" "<<4 - prit->second.size()<<"st producer");
        return pro;
      }
      return ProducerPtr();
    }
    bool giveBackProducer(int key,ProducerPtr pro) {
      IceUtil::RWRecMutex::RLock lock(rwMutex_);
      if (!checkAvailable(key)) { // this mq has unavailable just abandon it
        MCE_INFO("XMQProducerResourceManager::giveBackProducer --> key:"<<key<<" has be set unavailable so just abandon it");
        return true;
      }
      MCE_DEBUG("XMQProducerResourceManager::giveBackProducer --> give mq:"<<getEndPoint(key)<<" producer back");
      map<int,deque<ProducerPtr> >::iterator prit = producers_.find(key);
      if (prit != producers_.end()) {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lock2(monitors_[key % mqSize_]);
        if(pro) {
          prit->second.push_back(pro);
          monitors_[key % mqSize_].notify();
          return true;
        } else {
          MCE_WARN("XMQProducerResourceManager::giveBackProducer --> key:"<<key<<" producer is NULL");
          return false;
        }
      }
      return false;
    }
    void setProducerUnavailable(int key) {
      IceUtil::RWRecMutex::WLock lock(rwMutex_);
      for (size_t i = 0; i < unavailableMods_.size(); ++i) {
        if (key == unavailableMods_[i]) {
          return;
        }
      }
      unavailableMods_.push_back(key);
      if (endpoints_.find(key) != endpoints_.end()) {
        endpoints_[key] = "";
      }
    }

    bool checkAddr(string endpoint) {
      return true;
    }
    bool checkAvailable(int key) {
      IceUtil::RWRecMutex::RLock lock(rwMutex_);
      for (size_t i = 0; i < unavailableMods_.size(); ++i) {
        if (key == unavailableMods_[i]) {
          return false;
        }
      }
      return true;
    }
    int getMqSize() {
      IceUtil::RWRecMutex::RLock lock(rwMutex_);
      return mqSize_;
    }
    string getEndPoint(int key) {
      IceUtil::RWRecMutex::RLock lock(rwMutex_);
      map<int,string>::iterator it = endpoints_.find(key);
      if ( it != endpoints_.end()) {
        return it->second; 
      }
    }
  private:
    IceUtil::RWRecMutex rwMutex_;
    map<int,deque<ProducerPtr> > producers_;
    map<int,string> endpoints_;
    IceUtil::Monitor<IceUtil::Mutex> monitors_[kMaxMqSize];
    deque<int> unavailableMods_;
    int mqSize_;
};
class ProducerTask:public MyUtil::Task {
  public:
    ProducerTask(string msg,size_t len,int mod):
      Task(XMQ_PRODUCER_TASK_LEVEL_BASE + mod),msg_(msg),len_(len),mod_(mod) {
    }
    virtual void handle();
  private:
    string msg_;
    size_t len_; 
    int mod_;
};

}
}
#endif
