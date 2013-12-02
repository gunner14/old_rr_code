/*
 * =====================================================================================
 *
 *       Filename:  AbstractAdapter.h
 * *    Description
 *
 *        Version:  1.0
 *        Created:  2012年05月15日 20时59分33秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jun.liu (), jun.liu@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */

#ifndef  ABSTRACTADAPTER_H
#define  ABSTRACTADAPTER_H

#include  <boost/thread/thread.hpp>
#include  <string.h>
#include  <vector>
#include  <LogWrapper.h>
#include  <zookeeper/zookeeper.h>
#include  <Ice/Ice.h>
#include  <unistd.h>


using namespace std;
using namespace boost;
using namespace Ice;

typedef shared_lock<shared_mutex> ReadLock;
typedef unique_lock<shared_mutex> WriteLock;

static const char* ZK_STR = "UserZooKeeper1:2181,UserZooKeeper2:2181,UserZooKeeper3:2181,UserZooKeeper4:2181,UserZooKeeper5:2181";
//static const char* ZK_STR = "10.3.17.28:2181,10.3.17.33:2181,10.3.17.34:2181,10.3.17.35:2181,10.3.17.36:2181";

namespace wap {
namespace adapter {

template <class T>
class AbstractAdapter 
{
	protected:
		zhandle_t *zkHandler;
		shared_mutex mutex_;
		vector<T> proxyList;
		CommunicatorPtr communicator_;
		string serviceName;
		string serviceGroup;
		string serviceNode;
		int timeout;
	public:

		AbstractAdapter(string _serviceName, string _serviceGroup, int _timeout = 100) : serviceName(_serviceName), serviceGroup(_serviceGroup), timeout(_timeout)
		{
			ostringstream os;
			os << "/3g/ice/" << serviceGroup << "/" << serviceName << "/.service_nodes";
			serviceNode = os.str();

			init();
		}
		virtual ~AbstractAdapter();

		/**
		 * @brief watcher函数 
		 **/
		static void watcher(zhandle_t * zh, int type, int state, const char* path, void * watcherCtx); 
	
	protected:
		/**
		 * @brief 初始化
		 **/	
		void init();
		
		/**
		 * @brief 初始化连接器
		 */
		virtual bool initCommunicator();

		/**
		* @brief 连接zookeeper服务器
		*/
		virtual bool connect();
		
		/**
		* @brief 获取Proxy, 基本hash
		*/
		virtual T getProxy();

		/**
		 * @brief 初始化代理
		 */
		virtual void initProxy();


};
	
template <class T>
bool AbstractAdapter<T>::initCommunicator() {
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

	template <class T>
	AbstractAdapter<T>::~AbstractAdapter(){ 
		if(zkHandler)
			zookeeper_close(zkHandler);
		if(communicator_)
			communicator_->destroy();
	}

	template <class T>
	void AbstractAdapter<T>::init() {
		srand(time(0));
		zkHandler = NULL;
		if(connect() && initCommunicator())
			initProxy();	
	}
	template <class T>
	bool AbstractAdapter<T>::connect() {
		if(zkHandler){
			zookeeper_close(zkHandler);
			MCE_ERROR("WapAdapter::connect already connect");
			usleep(250000);
		}
		zkHandler = zookeeper_init(ZK_STR, &AbstractAdapter::watcher, timeout, 0, static_cast<void *>(this), 0);
		bool connectResult = zkHandler != NULL;

		MCE_INFO("WapAdapter::connect[result=" << connectResult << "]");
		return connectResult;
	}

	template <class T>
	T AbstractAdapter<T>::getProxy() {
		ReadLock lock(mutex_);
		if(proxyList.empty()) {
			return NULL;		
		}

		int size = proxyList.size();
		int index = rand() % size;
		return proxyList[index];
	}


	template <class T>
	void AbstractAdapter<T>::initProxy() {
		WriteLock lock(mutex_);
		if(!proxyList.empty())
			proxyList.clear();

		struct String_vector str_vec;

		//尝试三次获取
		for(int j = 0; j < 3; j++) {
			
			int result = zoo_get_children(zkHandler, serviceNode.c_str() ,1 ,&str_vec);
			
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
					os << serviceName << ":default -h " << host << " -p " << port; 
          MCE_INFO("AbstractAdapter try to add prx " << os.str());
					
					Ice::ObjectPrx prx = communicator_->stringToProxy(os.str());			
					if(!prx) {
						continue;
					}

					T proxy = T::checkedCast(prx);
					if(proxy) {
						proxy->ice_timeout(timeout);
						proxyList.push_back(proxy);	
					}
				}

				deallocate_String_vector(&str_vec);
				return;
			}
			else {
				usleep(200000);	
			}
		}
	}

	template <class T>
	void AbstractAdapter<T>::watcher(zhandle_t * zh, int type, int state, const char* path, void * watcherCtx)
	{
		
		MCE_INFO("Zookeeper watcher triggered[type:=" << type << ", state=" << state << ", path=" << path << "]");
		AbstractAdapter * adapterPtr = static_cast<AbstractAdapter *>(watcherCtx);	
				
		if(type == ZOO_SESSION_EVENT)
		{   
			if(state == ZOO_EXPIRED_SESSION_STATE){
				if(adapterPtr->connect()) {
					adapterPtr->initProxy();
				}
				else {
					MCE_ERROR("WapAdapter::watcher adapterPtr->connect() failed");
				}
			}   
		}
	
		if(type == ZOO_CHILD_EVENT)
		{
			adapterPtr->initProxy();		
		}
	}

};
};

#endif  /*ABSTRACTADAPTER_H*/
