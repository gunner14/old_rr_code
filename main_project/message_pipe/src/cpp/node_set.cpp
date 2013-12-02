#include "node_set.h"

namespace xce{
namespace messagepipe{

using namespace decaf::util::concurrent;

bool NodeSet::send(const MessageDataPtr& data) {

	int max_try = kMaxTry;
	NodePtr np;
	while (max_try-- > 0) {

        {
            IceUtil::Mutex::Lock lock(mutex_);
			if(!has_valid_ || nodes_.empty()){
				return false;
			}
			size_t index;
			while(1){
				index = rand()%nodes_.size();
				if(nodes_[index]->valid())
					break;
			}
			//copy node ptr
			np = nodes_[index];
		}
		if(np->send(data))
			return true;
	}
	return false;
}
int NodeSet::receive(bool is_topic, const std::string& subject, size_t max_size, std::vector<MessageDataPtr>& messages, const int index){

    static uint64_t inc = 0;
    size_t i = -1; 
    NodePtr np ;
    {
        IceUtil::Mutex::Lock lock(mutex_);
        if(nodes_.empty()){
            return -1;
        }
        i = (index == -1) ? (inc++ % nodes_.size()) : ((index+1) % nodes_.size());
        np = nodes_[i];
    }
    np->receive(is_topic, subject, max_size, messages);
    return i;
}

void NodeSet::printCurrentNode() {


  std::ostringstream os;
  os<<"NodeSet::printCurrentNode current all brokers";
	for (std::vector<NodePtr>::const_iterator it=nodes_.begin();it!=nodes_.end();++it) {
		os<<(*it)->toString()<<"\n";
	}

  MCE_DEBUG(os.str());
}

// @Override

// if we shut down a machine ,we must change the status to offline
// and delete to avoid some message no consumer

void NodeSet::handleChildChange(const std::string& cluster, const std::list<std::string>& currentChilds){

	std::vector<NodePtr> current_nodes;
    {
        IceUtil::Mutex::Lock lock(mutex_);
		printCurrentNode();
		for (std::list<std::string>::const_iterator it = currentChilds.begin(); it!=currentChilds.end(); ++it) {
			NodePtr node(new Node(cluster,*it));
			if(!node->init())
				continue;
			std::vector<NodePtr>::iterator itV = nodes_.begin();
            for(;itV!=nodes_.end();++itV){
                if((*itV)->toString() == (*it)) {
                    MCE_DEBUG("NodeSet::handleChildChange reuse node " << (*it));
                    current_nodes.push_back(*itV);		
                    break;
                }
			}
            if(itV == nodes_.end()){
				current_nodes.push_back(node);
			}
            if(current_nodes.back()->valid())
                has_valid_=true;
		}
		current_nodes.swap(nodes_);
		printCurrentNode();
	}
}

bool NodeSet::Update(const std::list<std::string>& childrenNameList) {
	try {
		handleChildChange(ns(), childrenNameList);
	} catch (std::exception& e) {
		MCE_WARN("NodeSet::Update exception " << e.what());
		return false;
	}
	return true;
}

}
}
