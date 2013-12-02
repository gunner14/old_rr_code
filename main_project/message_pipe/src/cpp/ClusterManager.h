#ifndef _MP_CLUSTERMANAGER_H__
#define _MP_CLUSTERMANAGER_H__

#include "Common.h"
#include "node_set.h"
#include "Singleton.h"
#include "ConcurrentComputingMap.h"

namespace xce{
namespace messagepipe{

using namespace decaf::util::concurrent;

class ClusterManager : public Singleton<ClusterManager>{	

private :
	ConcurrentComputingMap<std::string,NodeSetPtr> cluster2nodes_;	
public:
    ClusterManager();
	NodeSetPtr getConnection(const std::string& zk_address, const std::string& cluster);
	NodeSetPtr create(const std::string& zk_cluster);
    bool create(const std::string& zk_cluster,NodeSetPtr& nodes){
        MCE_WARN("ClusterManager::create computing map miss " << zk_cluster);
        size_t index = zk_cluster.find("|");
        if(index!=std::string::npos){
            std::string zk(zk_cluster,0,index);
            std::string cluster(zk_cluster,index+1);
            nodes.reset(new NodeSet(zk,cluster));
            bool ok = nodes->init();
            if(!ok){
                MCE_WARN("ClusterManager::create computing map create " << zk_cluster<<" fail ");
                return false;
            }
            com::renren::messageconfig::ZkClient::GetInstance(zk)->AddNamespaceListener(nodes);
            MCE_DEBUG("ClusterManager::create computing map create " << zk_cluster <<" ok ");
            return true;
        }else{
            return false;
        }
    }
};

}
}
#endif
