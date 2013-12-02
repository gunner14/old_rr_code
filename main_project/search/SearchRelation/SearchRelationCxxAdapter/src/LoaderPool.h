#ifndef LOADERPOOL_H
#define LOADERPOOL_H
#include "../../SearchZkClient/src/zk_client.h"
#include "../../SearchZkClient/src/zk_client.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string/split.hpp>
#include <vector>
#include  <boost/algorithm/string/classification.hpp>
#include <Ice/Ice.h>
#include <map>
#include "SearchRelationLoader.h"
#include "UtilCxx/src/LogWrapper.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>


namespace search{
namespace relation{
using namespace boost;
using namespace com::renren::searchrelation;
class PrintHelper{
public:
	template<typename T>
	static string Print(const T & container,int limit = 30) {
		ostringstream ios;

		ios << "(";
		int idx = 0;
		typedef typename T::value_type vt;
		BOOST_FOREACH(const vt & v,container){
			if(idx++ > 0) ios << ",";

			ios << v;

			if(idx >= limit) break;
		}
		ios << ")";
		return ios.str();
	}
};
class LoaderListerner : public ZkEventListener{
        public:
                LoaderListerner(){ }
                virtual ~LoaderListerner(){}
                virtual void HandleNodeEvent(const char * path, const std::string & value){
                        MCE_INFO("LoaderListerner::HandleNodeEvent:" << path  << " value:" << value  );
                }
                virtual void HandleChildEvent(const char * path, const std::vector<std::string> & children) ;
};

class LoaderPool{
public:
	static LoaderPool * GetInstance(){
		//static LoaderPool * pool_	;
		//if(pool_ == 0){//TODO 加锁
		//	pool_ = new LoaderPool()	;
		//}
		//return pool_;
		static LoaderPool * instance_;
		if( instance_ == NULL ) {
			boost::lock_guard<boost::mutex> lock(single_mutex_);
			if( instance_ == NULL ) {
				instance_ = new LoaderPool();
			}
		}
		return instance_;

	}
	void PingAllProxy(){
		while(1){
			int size;
			{
				boost::mutex::scoped_lock(loaderListMutex_);
				size = loaderList_.size();
			}
			if(size == 0){
				MCE_WARN("Proxy size is 0" );
				return;
			}
			set<int> suc,fail;
			for(int i = 0; i < size; ++i){
				SearchRelationLoaderPrx prx = getProxy(i);
				if(prx == 0)	{
					MCE_WARN("Proxy is null. idx:" << i );
					fail.insert(i);	
					continue;
				}
				try {
					prx->ice_ping();
					suc.insert(i);	
				} catch (Ice::Exception& e) {;
					fail.insert(i);	
					MCE_WARN("LoaderPool::PingAllProxy fail. idx:" << i << " prx:" << prx << e.what())
				}
			}
			if(!fail.empty()){
				UpdateNodes();	
			}
			MCE_INFO("PingAllProxy. proxylist size:" << size << " suc:" << PrintHelper::Print(suc) << " fail:" << PrintHelper::Print(fail));
			boost::posix_time::seconds workTime(5);  
			boost::this_thread::sleep(workTime);  
		}
	}
	void UpdateNodes(){
		vector<std::string> list;
		ZookeeperClient::Instance().GetChildren(loaderpath_.c_str(), &list);

		multimap<int,SearchRelationLoaderPrx> mmap;
		vector<SearchRelationLoaderPrx> loaderListNew;
		BOOST_FOREACH(const string & str , list){
			pair<int,SearchRelationLoaderPrx> p= ParseLoaderString(str)	;
			if(p.first== -1){
				MCE_WARN("LoaderPool::UpdateNodes. parse fail.str " << str);
				continue;	
			}
			try{
				p.second->ice_ping();	
				mmap.insert(p);
                        } catch (Ice::Exception& e) {;
                                MCE_WARN("UpdateNodes -> ice ping fail. idx:" << p.first << " str:" << str << " prx:" << p.second  << e.what())
                        }
		}
		typedef multimap<int,SearchRelationLoaderPrx>::value_type VT;
		BOOST_FOREACH(const VT & p, mmap ){
			loaderListNew.push_back(p.second);	
		}
		{
			boost::mutex::scoped_lock(loaderListMutex_);
			MCE_INFO("LoaderPool::UpdateNodes old size:" <<  loaderList_.size() << " new size:" << loaderListNew.size() );
			loaderList_ = loaderListNew;
		}
	}	
	SearchRelationLoaderPrx getProxy(int uid){
		{
			boost::mutex::scoped_lock(loaderListMutex_);
			int size = loaderList_.size();
			if(size != 0){
				int idx = uid % size;
				SearchRelationLoaderPrx	prx = loaderList_.at(idx);
				return prx;
			}
		}
		MCE_WARN("LoadPool::getProxy. loaderList is empty!!! begin Update");
		UpdateNodes();
		{
			boost::mutex::scoped_lock(loaderListMutex_);
			int size = loaderList_.size();
			if(size != 0){
				int idx = uid % size;
				SearchRelationLoaderPrx	prx = loaderList_.at(idx);
				MCE_INFO("LoaderPool::getProxy success at second time. uid:" << uid << " poolsize:" << size << " idx:" << idx << " prx:[" << prx << "]");
				return prx;
			}
		}
		MCE_WARN("LoadPool::getProxy. loaderList is empty at second time!!! ");
		return 0;
	}
	void Init(const string& zk,const string & loaderpath){
		zk_ = zk;
		loaderpath_ = loaderpath;
		zk_ = "st0.xce.d.xiaonei.com:2181";
		loaderpath_ = "/searchrelation/loader";
		ZookeeperClient::Instance().SetZk(zk_);
		ZkEventListenerPtr zl(new LoaderListerner());
		ZookeeperClient::Instance().AddChildListener(loaderpath_.c_str(), zl);
		UpdateNodes();
		pingThread_ = boost::thread( boost::bind(&LoaderPool::PingAllProxy, this));
	}
private:
	boost::thread pingThread_;
	static boost::mutex single_mutex_;


	boost::mutex loaderListMutex_;

	vector<SearchRelationLoaderPrx> loaderList_;
	Ice::CommunicatorPtr communicator_;
	string zk_ ;
	string loaderpath_ ;
	//10.3.18.75_9012_SearchRelationLoader2
	pair<int,SearchRelationLoaderPrx> ParseLoaderString(const string & str){
		if(communicator_ == 0){
			InitCommunicator();	
		}
		vector<string> ks;
		boost::split(ks,str, boost::is_any_of("_"));
		string ip = ks[0];
		string port = ks[1];
		string name = ks[2];
		string idx = name.substr(20);

		string endpoints = createEndpoints(ip,port);
		Ice::ObjectPrx prx = communicator_->stringToProxy(string("SearchRelationLoader:") + endpoints)->ice_timeout(1000);//todo
		SearchRelationLoaderPrx loaderPrx = SearchRelationLoaderPrx::uncheckedCast(prx);
		return make_pair(atoi(idx.c_str()), loaderPrx);
	}

	string createEndpoints(const string & ip, const string & port) {
		string endpoints = "tcp -h " + ip + " -p " + port + " : udp -h " + ip + " -p " + port;
		return endpoints;
	}
	void InitCommunicator(){
		Ice::PropertiesPtr properties = Ice::createProperties();
		properties->setProperty("Ice.MessageSizeMax", "20480");
		Ice::InitializationData id;
		id.properties = properties;
		communicator_ = Ice::initialize(id);
	}
};
//boost::mutex LoaderPool::single_mutex_;



}
}
#endif
