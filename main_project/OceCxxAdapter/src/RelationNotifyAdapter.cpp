/* * =================================================================================== 
 *       Filename:  RelationNotifyAdapter.cpp 
 *       Description:  
 *       Version:  1.0 
 *       Created:  2012年03月16日 19时43分29秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *       Author:  jun.liu (), jun.liu@renren-inc.com
 *       Company:  renren-inc
 * =====================================================================================
 */
#include  <boost/thread/thread.hpp>
#include  <log4cplus/logger.h>
#include  <sys/select.h>
#include  <stdexcept>
#include  <stdlib.h>

#include  "UtilCxx/src/LogWrapper.h"
#include  "RelationNotifyAdapter.h"

using namespace std;
using namespace boost;
using namespace wap::notify;

typedef shared_lock<shared_mutex> ReadLock;
typedef unique_lock<shared_mutex> WriteLock;

static const char* ZK_STR = "UserZooKeeper1:2181,UserZooKeeper2:2181,UserZooKeeper3:2181,UserZooKeeper4:2181,UserZooKeeper5:2181";
static const char* SERVICE_NODE = "/3g/ice/talk/FriendRelationNotifier/.service_nodes";
static const char* SERVICE_NAME = "FriendRelationNotifier";
static const char* DEFAULT_HOST = "10.3.19.173";
static const int DEFAULT_PORT = 31090;
static const int DEFAULT_TIMEOUT = 100;


namespace wap {
namespace notify {

	RelationNotifyAdapter::RelationNotifyAdapter()
	{
		srand(time(0));
		zkHandler = NULL;
		if(connect() && initCommunicator())
			initProxy();
	}

	RelationNotifyAdapter::~RelationNotifyAdapter()
	{
		if(zkHandler)
	        zookeeper_close(zkHandler);
		if(communicator_)
			communicator_->destroy();
	}

	void RelationNotifyAdapter::notify(FriendRelationType type, int from, int target)
	{
		FriendRelationNotifierPrx notifier = getProxy();	
		if(!notifier) {
			MCE_ERROR("RelationNotifyAdapter::notify not find proxy");	
			return;
		}
		
		try {
			notifier->notify(type, from, target);
		}
		catch(std::exception& e)
		{
			MCE_ERROR("RelationNotifyAdapter::notify has something wrong :" << e.what());	
		}
	}


	void RelationNotifyAdapter::initProxy()
	{
		WriteLock lock(mutex_);
		if(!proxyList.empty())
			proxyList.clear();
	
		struct String_vector str_vec;
		
		//尝试三次获取
		for(int j = 0; j < 3; j++){
			int result = zoo_get_children(zkHandler, SERVICE_NODE ,1 ,&str_vec);
			if(result == ZOK) {
				int count = str_vec.count;
				for(int i = 0; i < count; i++)
				{
					string value = str_vec.data[i];
					size_t p = value.find(':');
					if(p == string::npos)
						continue;

					string host = value.substr(0, p);
					string port = value.substr(p + 1);

					ostringstream os;

					os << SERVICE_NAME << ":default -h " << host << " -p " << port; 

					Ice::ObjectPrx prx = communicator_->stringToProxy(os.str());			
					FriendRelationNotifierPrx notifier = FriendRelationNotifierPrx::checkedCast(prx);
					if(notifier) {
						notifier->ice_timeout(DEFAULT_TIMEOUT);
						proxyList.push_back(notifier);	
					}
				}
		
				deallocate_String_vector(&str_vec);
				return;
			}
			else {
				MCE_DEBUG("RelationNotifyAdapter::initProxy zoo_get_children[result=" << result << "]");
				MilliSleep(200);
			}
		}
	}

	bool RelationNotifyAdapter::connect()
	{
		log4cplus::Logger logger = log4cplus::Logger::getInstance("relationAdapter");
		if(zkHandler){
			zookeeper_close(zkHandler);
			MCE_WARN("RelationNotifyAdapter::connect already connect");
			MilliSleep(250);
		}
		zkHandler = zookeeper_init(ZK_STR, &RelationNotifyAdapter::watcher, DEFAULT_TIMEOUT, 0, static_cast<void *>(this), 0);
		bool connectResult = zkHandler != NULL;

		MCE_DEBUG("RelationNotifyAdapter::connect[result=" << connectResult << "]");
		return connectResult;

	}

	FriendRelationNotifierPrx RelationNotifyAdapter::getProxy()
	{
		
		if(proxyList.empty()) {
			//如果从zk中没有取到，则用默认，防止丢通知
			ostringstream os;

			os << SERVICE_NAME << ":default -h " << DEFAULT_HOST << " -p " << DEFAULT_PORT; 

			Ice::ObjectPrx prx = communicator_->stringToProxy(os.str());			
			FriendRelationNotifierPrx notifier = FriendRelationNotifierPrx::checkedCast(prx);
			if(notifier) {
				WriteLock lock(mutex_);
				notifier->ice_timeout(DEFAULT_TIMEOUT);
				proxyList.push_back(notifier);	
				MCE_DEBUG("RelationNotifyAdapter::getProxy use Default Proxy");
				return notifier;
			}
			return NULL;
		}
		ReadLock lock(mutex_);
		int size = proxyList.size();
		int index = rand() % size;
		return proxyList[index];

	}


	bool RelationNotifyAdapter::initCommunicator()
	{
		Ice::PropertiesPtr properties = Ice::createProperties();
		properties->setProperty("Ice.Override.ConnectTimeout", "1000");        
		properties->setProperty("Ice.Override.Timeout", "100");               
		properties->setProperty("Ice.ThreadPool.Client.Size", "50");           
		properties->setProperty("Ice.ThreadPool.Server.Size", "50");           

		Ice::InitializationData id;                                            
		id.properties = properties;                                            
		communicator_ = Ice::initialize(id);
		return true;
	}

	void RelationNotifyAdapter::watcher(zhandle_t * zh, int type, int state, const char* path, void * watcherCtx)
	{
		MCE_DEBUG("Zookeeper watcher triggered[type:=" << type << ", state=" << state << ", path=" << path << "]");
		RelationNotifyAdapter * adapterPtr = static_cast<RelationNotifyAdapter *>(watcherCtx);	
				
		if(type == ZOO_SESSION_EVENT)
		{   
			if(state == ZOO_EXPIRED_SESSION_STATE){
				if(adapterPtr->connect()) {
					adapterPtr->initProxy();
				}
				else {
					MCE_ERROR("RelationNotifyAdapter::watcher adapterPtr->connect() failed");
				}
			}   
		}
	
		if(type == ZOO_CHILD_EVENT)
		{
			adapterPtr->initProxy();		
		}
	}

	void MicroSleep(long microseconds) {
	    struct timeval val;
		val.tv_sec = 0;
		val.tv_usec = microseconds;
		select(0, 0, 0, 0, &val);   
	} 

	void MilliSleep(long milliseconds) {
	    MicroSleep(milliseconds * 1000);
	}

};
};
