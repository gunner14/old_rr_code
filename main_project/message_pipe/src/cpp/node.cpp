#include "node.h"
#include <activemq/cmsutil/MessageCreator.h>
#include <activemq/commands/ActiveMQQueue.h>
#include <activemq/commands/ActiveMQTopic.h>
#include <activemq/commands/ActiveMQDestination.h>
#include "MessageData.h"
#include "util/cpp/TimeCost.h"
#include <time.h>

namespace xce{
namespace messagepipe{
using namespace activemq::commands;

SendReceiveController::SendReceiveController() {
  within_minutes_ = 1;
  max_exception_time_ = 3000;
}

void SendReceiveController::pushExceptionTime(time_t exception_time) {
  IceUtil::Mutex::Lock lock(*this);
  MCE_WARN("[SendReceiveController::pushExceptionTime] exception times = " << exception_times_.size());
  if (exception_time == 0) {
    exception_time = time(NULL);
  }
  exception_times_.push_back(exception_time);
  while (exception_time - within_minutes_* 60 > exception_times_.front()) {
    exception_times_.pop_front();
  }
}

bool SendReceiveController::exceedMaxExceptions() const {
  IceUtil::Mutex::Lock lock(*this);
  //下面的600代表10分钟
  return exception_times_.size() > (time_t)max_exception_time_ && exception_times_.back() + 600 > time(NULL);
}

class ByteMessageCreator: public activemq::cmsutil::MessageCreator {

    public:
        ByteMessageCreator(const MessageDataPtr& data) : activemq::cmsutil::MessageCreator() ,data_(data->getData()){}	
        virtual ~ByteMessageCreator() throw() {
            std::vector<unsigned char>().swap(data_);
        }
        virtual cms::Message* createMessage( cms::Session* session ) {

            cms::BytesMessage* message = NULL;
            if( session != NULL ) {
                message = session->createBytesMessage();
                message->writeBytes(data_);
            }
            //不用delete,CmsTemplate会自动帮你delete
            return message;
        }
     private:
        std::vector<unsigned char> data_;
};

bool Node::send(const MessageDataPtr& data){
    if (send_receive_controller_.exceedMaxExceptions()) {
      MCE_WARN("[Node::send] too many exceptions, try again later!"); 
      return false;
    }
    std::string dest = (data->isTopic() ? "topic://" : "queue://") + data->getSubject();

    MyUtil::TimeCost tc = MyUtil::TimeCost::create("send " + dest);
    CmsConnectionPoolPtr pool = send_pool_.get(dest);

    CmsTemplate* cms_template = NULL;
    try {
        cms_template = pool->takeConnection();
        if(cms_template == NULL){
            MCE_WARN("Node::send send pool full " << dest);
            return false;
        }
        
        ByteMessageCreator creator(data);	

        //destination destroy by message
        if(data->isTopic()){
            ActiveMQTopic topic(data->getSubject()) ;
            cms_template->send(&topic,&creator);
        }else{
            ActiveMQQueue queue(data->getSubject()) ;
            cms_template->send(&queue,&creator);
        }

        pool->returnConnection(cms_template);
        return true;
		
	}catch(cms::CMSException& ex){
		MCE_WARN("Node::send fail exception: "<< ex.getMessage() << " " << dest);
    send_receive_controller_.pushExceptionTime();
	}catch(...){
		MCE_WARN("Node::send fail unknown exception "<< dest);
    send_receive_controller_.pushExceptionTime();
	}
	if(cms_template!=NULL){
		pool->returnConnection(cms_template);
	}
	return false;
}	

void Node::receive(bool is_topic, const std::string& subject, size_t max_size, std::vector<MessageDataPtr>& messages ){

    /*
    if (send_receive_controller_.exceedMaxExceptions()) {
      MCE_WARN("[Node::send] too many exceptions, try again later!"); 
      return;
    }
    */
    std::string dest = (is_topic ? "topic://" : "queue://") + subject;
    MyUtil::TimeCost tc = MyUtil::TimeCost::create("receive " + dest + " " + host_);
    CmsConnectionPoolPtr pool = receive_pool_.get(dest);

    CmsTemplate* cms_template = NULL;
    cms::Message* last_message = NULL ;
    try {
        cms_template = pool->takeConnection(); 
        if(cms_template == NULL) {
            MCE_WARN("Node::receive receive pool full " << dest);
            return ;
        }

        for(size_t i=0 ; i<max_size ; ++i){

            cms::Message* message = NULL;
            if(is_topic){	
                ActiveMQTopic topic(subject) ;
                message =  cms_template->receive(&topic);
            }else{
                ActiveMQQueue queue(subject) ;
                message =  cms_template->receive(&queue);
            }
            if(message == NULL && !messages.empty()){
                MCE_DEBUG("Node::receive message break return size:" << messages.size());
                break;
            }

            if(message == NULL && messages.empty()) {
                MCE_DEBUG("Node::receive receive nothing continue");
                break;
            }
            
            std::vector<unsigned char> body(dynamic_cast<cms::BytesMessage*>(message)->getBodyLength());
            dynamic_cast<cms::BytesMessage*>(message)->readBytes(body);
            MessageDataPtr data( new MessageData(cluster_, subject, body, is_topic) );
            messages.push_back(data);
            message->acknowledge();
            delete message;
            /*
            if(last_message!=NULL){
                delete last_message;
            }
            last_message = message;
      
            */
        }

	}catch(cms::CMSException& ex){
		MCE_WARN("Node::receive receive message fail exception: "<< ex.getMessage() << " " << dest);
    send_receive_controller_.pushExceptionTime();
	}catch(...){
		MCE_WARN("Node::receive receive message fail unknown exception "<< dest);
    send_receive_controller_.pushExceptionTime();
	}
    /*
        if(last_message!=NULL) {
            last_message->acknowledge();
            delete last_message;
        }
        */
    if(cms_template!=NULL){
        pool->returnConnection(cms_template);
    }
}

}
}
