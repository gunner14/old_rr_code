#ifndef __PING_SENDER_H__
#define __PING_SENDER_H__
#include <string>
#include <vector>
#include <map>
#include <Ice/Ice.h>
#include "TaskManager.h"

namespace xce {
namespace ad {
using namespace std;

class PingItemQueue: public IceUtil::Monitor<IceUtil::Mutex> {

  public:

    PingItemQueue():size_(300){}

    void SetSize(size_t sz){
      if(sz < 300){
        size_ = 300;
      }else{
        size_ = sz;
      }
    }

    void Put(const std::string &tag, int64_t item);
    void Get(std::map<string, std::vector<int64_t> >& to_be_send);
    
  private:

    map<string, vector<int64_t> > messages_;
    set<string> ready_tags_;
    std::string server_name_;
    std::string node_;
    size_t size_;

};

class SenderThread : public IceUtil::Thread{

  public:

    SenderThread(PingItemQueue &queue, const std::string &server_name):
      queue_(queue),
      server_name_(server_name){
        node_ = "unknown";
      }

    virtual void run();

    void SetNode(const std::string &node){
      node_ = node;
    }

    void SetServerName(const std::string &name){
      server_name_ = name;
    }

  private:

    void Send(const std::string &tag, const std::vector<int64_t> &message);
    PingItemQueue &queue_;
    std::string server_name_;
    std::string node_;
};

class PingSender {
public:
  PingSender(const string& server_name = "localhost", int threshold = 300)
            : server_name_(server_name),
              threshold_(threshold),
              sender_thread_(queue_, server_name_){
                queue_.SetSize(threshold_);
                sender_thread_.start().detach();
              }; 

  void SetServerInfo(const string& server_name = "dommy@localhost", int threshold = 300);
  bool Send(const string& tag, const int64_t value);

private:
  bool BatchSend(const string& tag, const vector<int64_t>& value);
  PingItemQueue queue_;
  string server_name_;
  string node_;
  int threshold_;
  map<string, vector<int64_t> > messages_;
  IceUtil::Mutex mutex_;
  SenderThread sender_thread_;
  class SendTimer : public MyUtil::Timer {
  public:
    SendTimer(const string& server_name, const string& node, const string& tag,
                const vector<int64_t>& values, int time = 0)
                 : Timer(time), server_name_(server_name), node_(node),
                   tag_(tag), values_(values) {
    }
    virtual void handle();
  private:
    string server_name_;
    string node_;
    string tag_;
    vector<int64_t> values_;
  };
};

}
}

#endif //__PING_SENDER_H__
