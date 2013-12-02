#include"FeedXMQProducer.h"
#include <boost/lexical_cast.hpp>
#include<iostream>
#include<string>
//#define _STDOUT
using namespace xce::feed;
using namespace std;
using namespace xmq;
using namespace MyUtil;

void ProducerTask::handle() {

  timeval begin; 
  gettimeofday(&begin,NULL);
  long t = begin.tv_sec*1000 + begin.tv_usec/1000;
  string timesamp = boost::lexical_cast<string>(t);
  msg_.append(";");
  msg_.append(timesamp);
  len_ = msg_.size();
  const char * msg = msg_.c_str();
  char buf[len_ + 1];
  strncpy(buf,msg,len_+1);

  ProducerPtr producer = XMQProducerResourceManager::instance().getProducer(mod_);
  if (!producer) {
    MCE_WARN("ProducerTask::hanlde --> getProducer return NULL ");
    return;
  }
  int max_send_times = 3;
  int max_open_times = 2;
  MCE_DEBUG("timesamp:"<<timesamp<<" buf:"<<buf<<" len:"<<len_);
  int ret = producer->sendData(buf,len_ + 1);
  if (ret < 0) {
    MCE_WARN("ProducerTask::handle --> first senData failed return "<<ret);
    if (ret == -1) { // sending data timeout
      while ( ret == -1 && max_send_times > 0) {
        ret = producer->sendData(buf,len_ +1 );
        max_send_times--;
        MCE_WARN("ProducerTask::handle --> resend data "<<3-max_send_times<<"st failed"<<" return:"<<ret);
        if (ret >= 0) {
          break;
        }
        sleep(1);
      }

      string endpoint = XMQProducerResourceManager::instance().getEndPoint(mod_);
      while ((ret == -3||ret == -2) && max_open_times) {
        ret = producer->open(endpoint);
        max_open_times--;
        MCE_WARN("ProducerTask::handle --> reopen endpoint "<<endpoint<<" "<<3-max_send_times<<"st failed");
        if (ret ==0 ) {
          break;
        }
        sleep(1);
      }
      if ( ret == 0) {
        ret =  producer->sendData(buf,len_ +1);
      } 
    }
  }
  if (ret == -1) {
    //still timeout ,this  mq is unavailable;
    XMQProducerResourceManager::instance().giveBackProducer(mod_,producer);
    return;
    XMQProducerResourceManager::instance().setProducerUnavailable(mod_);
    return;
  } 
  XMQProducerResourceManager::instance().giveBackProducer(mod_,producer);
}
  
/*
   int main(int argc,char **argv) {
   if (argc !=3) {
   cout<<"usage: xmq_client [send times] [data len]"<<endl;
   cout<<"./xmq_client 1000 1024"<<endl;
   exit(1);
   }
   times = atoi(argv[1]);
   string data;
   data.assign(atoi(argv[2]),'A');
   cout<<data<<endl;
   deque<string> endpoints;
//endpoints.push_back("tcp://10.3.24.165:58533");
endpoints.push_back("tcp://10.3.24.165:63795");
XMQEndPoints::instance().UpdateEndPoints(endpoints);  
// TaskManager::instance().execute(new FeedXMQProducer("XXXXX",5));
FeedXMQProducer producer(data,data.size());
for (int i =0 ; i < times ; ++i ) {
producer.handle();
}

}
*/
