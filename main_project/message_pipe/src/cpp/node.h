#ifndef _MP_NODE_H__
#define _MP_NODE_H__


#include "Common.h"
#include "cms_connection_pool.h"
#include "ConcurrentComputingMap.h"
#include <deque>

namespace xce{
namespace messagepipe{

using namespace ::decaf::util::concurrent;

/***
* 这个类是用来控制node send和receive的，因为现在activemq存在的问题，如果链接断了，
* 会重建链接，这样可能会导致activemq出现too many open files的异常，导致activemq
* 不可用。现在用SendReceiveController来记录send和receive失败的次数，如果一分钟内
* 失败次数达到一个上限后，下面的十分钟之内将不再send和receive数据，只是简单的返回，
* 十分钟后再send和receive数据
***/
class SendReceiveController : virtual public IceUtil::Mutex {
public:
  SendReceiveController();
  void pushExceptionTime(time_t exception_time = 0);
  bool exceedMaxExceptions() const;
private:
  //下面的within_minutes_和max_exception_time_表示在within_minutes_分钟之内异常的次数不能超过max_exception_time_次才可以正常
  //send或者receive，否则在接下来的十分钟之内将不再send和receive数据
  int within_minutes_;
  int max_exception_time_;
  std::deque<time_t> exception_times_;
};

class Node 
{

public:
	std::string cluster_;

	std::string node_id_;
	std::string host_;
	std::string port_;
	std::string status_;

	std::string node_info_;
    std::string uri_;

	bool send(const MessageDataPtr& data);
	void receive(bool is_topic, const std::string& subject, size_t max_size, std::vector<MessageDataPtr>& buffer);

	Node(const std::string& cluster,const std::string& node_info)
			:cluster_(cluster),node_info_(node_info),send_pool_(boost::bind<bool>(&Node::create,this,_1,_2)),receive_pool_(boost::bind<bool>(&Node::create,this,_1,_2))
		{}

    bool create(const std::string key, CmsConnectionPoolPtr& value) {
        value.reset(new CmsConnectionPool(uri_));
        return true;
    }
	bool init(){
		//should check info syntex
		std::vector<std::string> infos;
                boost::algorithm::split(infos, node_info_, boost::algorithm::is_any_of("|"));
		
		if(infos.size()!=4)
			return false;
		node_id_ = infos[0];
		host_ = infos[1];
                port_ = infos[2];
                status_ = infos[3];
		std::ostringstream oss;
		oss<<"tcp://"<< host_ << ":" << port_ <<"?keepAlive=true&connection.alwaysSyncSend=true&connection.sendTimeout=1000";
        uri_ = oss.str();

        return true;
	}
	
	bool valid(){
		return status_ == "ONLINE";
	}
	
	std::string toString(){
		return node_info_;
	}
    bool operator=(const Node& node){
        return node_id_ == node.node_id_;
    }

private:
    ConcurrentComputingMap<std::string,CmsConnectionPoolPtr> send_pool_;
    ConcurrentComputingMap<std::string,CmsConnectionPoolPtr> receive_pool_;
  SendReceiveController send_receive_controller_;
};

}
}

#endif

