#include "McProxyConsumer.h"
#include "key_mapping.h"
#include "LogWrapper.h"
//#include "proxy_server.h"

using namespace std;
//using namespace xce::feed;

namespace xce {
namespace feedprx {
    // 必须实现的两个函数
void FeedMcProxyConsumer::update(const map<string, vector<string> >& endpoints) {
	MCE_INFO("FeedMcProxyConsumer::update --> version update");
  map<string, vector<string> > tmp(endpoints);
  boost::unique_lock<boost::shared_mutex> wlock(mutex_);
  endpoints_.swap(tmp);
}
void FeedMcProxyConsumer::update(const string& shard, const vector<string>& endpoints) {
	MCE_INFO("FeedMcProxyConsumer::update --> shard:" << shard);
  vector<string> tmp(endpoints);

  boost::unique_lock<boost::shared_mutex> wlock(mutex_);
  map<string, vector<string> >::iterator it = endpoints_.find(shard);
  if (it != endpoints_.end()) {
 	  it->second.swap(tmp);
		if (!it->second.empty()) {
			try {
				string endpoints;
				for (size_t i = 0; i<it->second.size(); ++i) {
					endpoints += it->second[i] + ";";
				}
				LoadCluster(it->first,endpoints);
			} catch(std::exception& e) {
				MCE_WARN("FeedMcProxyConsumer::update -->err," << e.what());
				}
		}

  }
}
	
    // 自定义函数 
map<string, string> FeedMcProxyConsumer::LocateAllServices() {
  boost::unique_lock<boost::shared_mutex> rlock(mutex_);
	map<string,string> res;
 for (map<string, vector<string> >::iterator it = endpoints_.begin();
	it!=endpoints_.end(); ++it) {
	if (!it->second.empty()) {
		string endpoints;
		for (size_t i = 0; i<it->second.size(); ++i) {
			endpoints += it->second[i] + ";";
		} 
		res.insert(make_pair(it->first,endpoints));
	}
}
 return res;
} 

void FeedMcProxyConsumer::initialize() {

	PropertiesConfiguration config("../etc/templates/zk_client_FeedMemcProxy.cfg");
	XcsServiceFactory accessorFactory;
	accessor_ = accessorFactory.CreateServiceRegistryAccessor(config, 500);
	if (accessor_) {
		accessor_->Connect(serviceName_, version_);
	}
	handler_ = XcsAdapterEventHandlerPtr(new XcsAdapterEventHandler(accessor_, *this));
	if (handler_) {
		handler_->initialize();
	}
}

}}

