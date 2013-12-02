#include "ClusterManager.h"

namespace xce{
namespace messagepipe{

using namespace decaf::util::concurrent;
using namespace decaf::util;

bool checkSyntex(const std::string key){
	return key.find("|")==std::string::npos;
}

ClusterManager::ClusterManager(): cluster2nodes_(boost::bind<bool>(&ClusterManager::create,this,_1,_2)){
}
NodeSetPtr ClusterManager::getConnection(const std::string& zk_address, const std::string& cluster){
	//没有多zk的地址进行检测，应该使用正则表达式进行更严格的检测
	NodeSetPtr pool;
	if(!checkSyntex(zk_address) || !checkSyntex(cluster)){
      MCE_WARN("ClusterManager::getConnection zk_addresss and cluster can't include '|' ");
	}
	std::string key = zk_address + "|" + cluster;
	//使用ComputingMap在key没有的情况会调用create方法进行构造
	try{
		pool = cluster2nodes_.get(key);
	}catch(NoSuchElementException& ex){
		MCE_WARN("ClusterManager::getConnection no such cluster in zk dir ");
	}
	return pool;
}

}
}
