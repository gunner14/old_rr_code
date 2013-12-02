#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>
#include <protocol/TCompactProtocol.h>
#include "PingService.h"
#include "PingSender.h"
#include "IceLogger.h"

namespace xce {
namespace ad {

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace com::renren::ad::ping;

void PingSender::SetServerInfo(const string& server_name, int threshold){
  size_t it = server_name.find('@');
  if(it != string::npos) {
    server_name_ = server_name.substr(0, it);
    node_ = server_name.substr(it+1);
  } else {
    server_name_ = server_name;
    node_ = "unknown";
  }

  sender_thread_.SetServerName(server_name_);
  sender_thread_.SetNode(node_);
  threshold_ = threshold;
  queue_.SetSize(threshold_);
  MCE_DEBUG("PingSender::SetServerInfo() --> server_name = " << server_name << ",  server_name_ = " << server_name_ << ", node_ = " << node_ << ", threshold_ = " << threshold_);
}

bool PingSender::Send(const string& tag, const int64_t value) {
  queue_.Put(tag, value);
  return true;
}

void PingItemQueue::Put(const std::string &tag, int64_t item) {
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
  map<string, vector<int64_t> >::iterator it = messages_.find(tag);
  if(it == messages_.end()){
    std::vector<int64_t> v;
    v.push_back(item);
    messages_[tag] = v;
  }else {
    std::vector<int64_t> &v = it->second;        
    if(v.size() >= size_ * 5){
      MCE_WARN("PingItemQueue::Put" << tag << " queue's size is :" << v.size() << ", give up putting");
      ready_tags_.insert(tag);
      notify();
      return;
    }

    v.push_back(item);
    if(v.size() >= size_){
      ready_tags_.insert(tag);
      notify();
      MCE_DEBUG("PingItemQueue::Put queue's size:" << v.size() << ", size_:" << size_ << ", the queue will be sent to ping_server");
      return;
    }
  }

}

void PingItemQueue::Get(std::map<string, std::vector<int64_t> >& to_be_send) {

  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
  timedWait(IceUtil::Time::seconds(10));

  if(ready_tags_.size() > 0){
    MCE_DEBUG("PingItemQueue::Get, ready_tags_.size > 0");
    for(set<string>::iterator it = ready_tags_.begin(); it != ready_tags_.end(); ++it){
      vector<int64_t> &v = messages_[*it]; 
      vector<int64_t> &v2 = to_be_send[*it];
      v.swap(v2);
    }
    ready_tags_.clear();
  }else{
    MCE_DEBUG("PingItemQueue::Get, TimeOut");
    for(map<string, vector<int64_t> >::iterator it = messages_.begin(); it != messages_.end(); ++it){
      vector<int64_t> &v = it->second; 
      if(v.size() > 0){
        vector<int64_t> &v2 = to_be_send[it->first];
        v.swap(v2);
      }
    }
  }

}



void SenderThread::run(){
  while(true){
    std::map<std::string, std::vector<int64_t> > ready_messages;
    queue_.Get(ready_messages);
    MCE_DEBUG("SenderThread::run Get return " << ready_messages.size());
    for(std::map<std::string, std::vector<int64_t> >::iterator it = ready_messages.begin(); it != ready_messages.end(); ++it){
      if(it->second.size() > 0){
        MCE_DEBUG("SenderThread::run,Send " << it->first << "," << it->second.size());
        Send(it->first, it->second);
      }
    }
  }      
}


void SenderThread::Send(const std::string &tag, const std::vector<int64_t> &message) {
  try{
    boost::shared_ptr<TSocket> socket(new TSocket("ping.jebe.d.xiaonei.com", 6655));
    socket->setConnTimeout(200);
    socket->setSendTimeout(50);
    socket->setRecvTimeout(100);
    boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TCompactProtocol(transport));
    PingServiceClient client(protocol);
    transport->open();
    MCE_DEBUG("SenderThread::Send," << server_name_ << "," << node_ << "," << tag);
    client.pingBatch(server_name_, node_, tag, message);
    transport->close();
  } catch (...) {
    MCE_DEBUG("PingSender::SendTimer::handle --> unknown error");
  }
}

}
}
