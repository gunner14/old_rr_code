#ifndef __FEED_NEW_FEEDMCPROXYCONSUMER_H__
#define __FEED_NEW_FEEDMCPROXYCONSUMER_H__
#include <map>
#include <string>
#include <vector>
#include <boost/thread.hpp>
#include "feed/XcsAdapterI.h"
using namespace std;
using namespace xcs;
using namespace MyUtil;


namespace xce {
namespace feedprx {
class FeedMcProxyConsumer : public XcsConsumer {
public:
		FeedMcProxyConsumer(const string& name, const string& version):
				serviceName_(name), version_(version) {}
    virtual ~FeedMcProxyConsumer() {}

    // 必须实现的两个函数
    virtual void update(const map<string, vector<string> >& endpoints);
    virtual void update(const string& shard, const vector<string>& endpoints);
	
    // 自定义函数 
    map<string, string> LocateAllServices();

		void initialize();

private:
    mutable boost::shared_mutex mutex_;
    map<string, vector<string> > endpoints_;
		ServiceRegistryAccessorPtr accessor_;
		XcsAdapterEventHandlerPtr handler_;
		string serviceName_;
		string version_;
};
typedef boost::shared_ptr<FeedMcProxyConsumer> FeedMcProxyConsumerPtr;

}
}

#endif
