#ifndef _MP_NODE_SET_H__
#define _MP_NODE_SET_H__

#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/concurrent/ConcurrentMap.h>
#include <decaf/util/concurrent/Mutex.h>
#include "Common.h"
#include "node.h"
#include "zkclient.h"
#include "MessageData.h"

namespace xce{
namespace messagepipe{

using namespace com::renren::messageconfig;

class NodeSet : public NamespaceListener {

private:
	std::string zk_address_;
	std::string cluster_;
	
	static const int kMaxTry = 3;
	std::vector<NodePtr> nodes_;
	bool has_valid_;
    IceUtil::Mutex mutex_;
public:
	NodeSet(const std::string zk_address, const std::string& cluster) 
			: NamespaceListener(cluster), zk_address_(zk_address),cluster_(cluster) ,has_valid_(false){
		MCE_DEBUG("NodeSet::NodeSet cluster "<<cluster<<" connection");
	} 
	bool init() {
		std::list<std::string> childs;
		bool ok = com::renren::messageconfig::ZkClient::GetInstance(zk_address_)->GetNodes(cluster_,&childs);
		if(!ok){
            MCE_WARN("NodeSet::init zkclient GetNodes " << cluster_ <<" not ok ");
			return false;
		}
		std::list<std::string>::iterator it=childs.begin();
		for(; it!=childs.end(); ++it){
			NodePtr p(new Node(cluster_,*it));
			bool ok = p->init();
			if(ok){
				nodes_.push_back(p);
				if(p->valid()){
					has_valid_ = true;
				}
			}
		}
		MCE_INFO("NodeSet::init " << cluster_ << " get  brokers size: " << childs.size()<<" valid: " << has_valid_);
		printCurrentNode();
        return true;
	}

	bool send(const MessageDataPtr& data) ;

	int receive(bool is_topic, const std::string& subject,size_t max_size, std::vector<MessageDataPtr>& messages , const int index = -1) ;

	void printCurrentNode(); 

	void handleChildChange(const std::string& cluster,const std::list<std::string>& currentChilds);

	virtual bool Update(const std::list<std::string>& childrenNameList) ;

};

}
}
#endif
