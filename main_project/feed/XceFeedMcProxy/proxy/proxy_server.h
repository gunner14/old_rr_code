
#ifndef _FEED_NEW_MEMC_PROXY_PROXY_SERVER_
#define _FEED_NEW_MEMC_PROXY_PROXY_SERVER_

#include <vector>
#include <map>
#include <string>
#include <boost/bind.hpp>

#include "key_mapping.h"
#include "client_conn.h"
#include "McProxyConsumer.h"
#include "upstream_conn.h"
#include "Singleton.h"
//#include "../../../configcenter/src/client/service.h"//zk接口路径 

namespace xce {
namespace feedprx {
//using namespace xcs::client;

const int kServerPort = 11315;
class ProxyServer : public MyUtil::Singleton<ProxyServer>
{
public:
  ProxyServer();
  ~ProxyServer();

  void Run();
  
  void PushClient(int index, ClientConnection* cli){
    MCE_INFO("ProxyServer::PushClient --> index:" << index << " cli:" << cli << " thread:" << pthread_self());
    if(index < 0 || index >= dispatch_threads_)  {
      MCE_WARN("ProxyServer::PushClient --> index:" << index << " cli:" << cli << " thread:" << pthread_self());
      return ;
    }
    clients_mutex_vec_[index]->lock();
    clients_[index].insert(cli);
    clients_mutex_vec_[index]->unlock();
  }
  void EraseClient(int index, ClientConnection* cli){
    MCE_INFO("ProxyServer::EraseClient --> index:" << index << " cli:" << cli << " thread:" << pthread_self());
    if(index < 0 || index >= dispatch_threads_)  {
      MCE_WARN("ProxyServer::EraseClient --> index:" << index << " cli:" << cli << " thread:" << pthread_self());
      return ;
    }
    clients_mutex_vec_[index]->lock();
    clients_[index].erase(cli);
    clients_mutex_vec_[index]->unlock();
  }
  bool IsExist(int index, ClientConnection* cli){
    MCE_INFO("ProxyServer::IsExit --> index:" << index << " cli:" << cli << " thread:" << pthread_self());
    if(index < 0 || index >= dispatch_threads_)  {
      MCE_WARN("ProxyServer::IsExit --> index:" << index << " cli:" << cli << " thread:" << pthread_self());
      return false;
    }
    clients_mutex_vec_[index]->lock_shared();
    bool ret = clients_[index].find(cli)!=clients_[index].end();
    clients_mutex_vec_[index]->unlock_shared();
    return ret; 
  }
  int ClientSize(){
    MCE_INFO("ProxyServer::ClientSize -->  thread:" << pthread_self());
    int count = 0;
    for(int i=0; i<clients_.size(); i++){
      clients_mutex_vec_[i]->lock_shared();
      count += clients_.at(i).size();
      clients_mutex_vec_[i]->unlock_shared();
    }
    return count;
  }
	//迁移zk
	void SyncPoolManagers(vector<string>& endpoints);
  bool InitService(const string& cache_nodes_path);
	map<string,string> GetMemcIps();
private:
  void RunConnService(int i);
  void StartAccept();
  void HandleAccept(ClientConnection* conn, const boost::system::error_code& error);
  void StatTimerHandle(deadline_timer* timer, int index, const boost::system::error_code& error);

  io_service io_service_;
  io_service::work work_;
  ip::tcp::endpoint endpoint_;
  ip::tcp::acceptor acceptor_;
  
  size_t dispatch_threads_;
  size_t dispatch_round_;
  vector<io_service *>        dispatch_services_;
  vector<io_service::work *>  dispatch_works_;
  vector<set<ClientConnection*> > clients_;
  mutable vector<boost::shared_mutex *>  clients_mutex_vec_;

  // TODO : 结构丑陋不清晰
  vector<UpstreamConnPool *> upconn_pools_;
  vector<PoolManager*> pool_managers_;
  vector<boost::asio::deadline_timer*> stat_timers_;
	//迁移zk
	FeedMcProxyConsumerPtr consumer_;
	//xcs::client::ServicePtr zk_service_;

};

}
}

#endif  // _FEED_NEW_MEMC_PROXY_PROXY_SERVER_
