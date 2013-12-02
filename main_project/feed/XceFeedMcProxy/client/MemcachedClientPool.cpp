#include "MemcachedClientPool.h"
//#include "FeedMemcachedProxyAdapter.h"
#include "FeedMemcProxyAdapter.h"
#include "IMMemcProxyAdapter.h"
#include "IceLogger.h"
#include <boost/lexical_cast.hpp>

using namespace xce::feed;
using namespace boost;

namespace xce {
namespace feedprx {

const double xce::feedprx::PoolManager::kLowestCntR = 0.9;
int xce::feedprx::PoolManager::begin_index_  = 0;

MemcachedClient* MemcachedClientPool::GetClient(){
  //IceUtil::Mutex::Lock lock(mutex_);
  MemcachedClient* cli = NULL;
  if(!free_client_.empty()){
    cli = free_client_.front();
    free_client_.pop_front();
  }
  if(!cli){
    cli = new MemcachedClient(ip_, port_, timeout_, recv_count_limit_);
  }
  return cli;
}

void MemcachedClientPool::ReleaseClient(MemcachedClient* cli){
  //IceUtil::Mutex::Lock lock(mutex_);
  if(!cli->Connected()){
    delete cli;
    return;
  }
  string ip = cli->GetIp();
  int port = cli->GetPort();
  if(ip!=ip_ || port!=port_){
    MCE_WARN("MemcachedClientPool::ReleaseClient --> invalid old:" << ip << " " << port << "  now:" << ip_ << " " << port_);
    delete cli;
    return;
  }
  free_client_.push_back(cli);
}

void MemcachedClientPool::Clear(){
  list<MemcachedClient*>::iterator it = free_client_.begin();
  for(; it!=free_client_.end(); ++it){
    delete *it;
  }
  free_client_.clear();
}

void PoolManager::CheckValid(){
	int cluster = 0;
	bool first_check = true; //error detection
//  cluster = FeedMemcachedProxyAdapter::instance().GetClusterSize();
  cluster = FeedMemcProxyAdapter::instance().GetClusterSize();
	MCE_INFO("PoolManager::CheckValid--> business_:" << business_ << " cluster:" << cluster);
  if(cluster == 0){
    cluster = 1;
  }

	srand((unsigned)time(NULL)); 
	double d_cluster = cluster;
  begin_index_ =  (int)(d_cluster * (rand() / (RAND_MAX + 1.0))); // 0 ~ cluster-1

  multimap<int, int> sorted;
  for(int i=0; i<cluster; i++){
    //string endpoint;
    ProxyInfoPtr info;
    try{
				//info = FeedMemcachedProxyAdapter::instance().GetProxyInfo(i);
				info = FeedMemcProxyAdapter::instance().GetProxyInfo(i);
    }catch(Ice::Exception& e){
      MCE_WARN("PoolManager::CheckValid --> get endpoint err! index:" << i << " exception:" << e);
      continue;
    }
    if(!info){
      MCE_WARN("PoolManager::CheckValid --> get endpoint err index:" << i);
      continue;
    }
    sorted.insert(pair<int, int>(info->connections, i));


    IceUtil::Mutex::Lock lock(mutex_);

		//////////// error detection
		if(first_check) { //initialization
			valid_proxy_map_.insert(make_pair(i, 0));
			valid_proxy_vec_.push_back(i);
		}
		////////////

    string oldip;
    int oldport = 0;
    int conn = 0;
    MemcachedClientPool* oldpool = NULL;
    if(pool_.count(i)>0){
      oldip = pool_[i]->GetIp();
      oldport = pool_[i]->GetPort();
      oldpool = pool_[i];
      conn = oldpool->FreeCount();
    }
    MCE_INFO("PoolManager::CheckValid --> proxy, index:" << i << "  ip:" << info->ip << " port:" << info->port 
              << " info connections:" << info->connections << " pool free count:" << conn << "  valid_proxy_map_ size:" <<  valid_proxy_map_.size() << " err_proxy_set_.size:" << err_proxy_set_.size() << " valid_proxy_vec.size:" << valid_proxy_vec_.size());

    if(oldip!=info->ip || oldport!=info->port){
      MCE_INFO("PoolManager::CheckValid --> proxy changed, old:" << oldip << " " << oldport << "  now:" << info->ip << " " << info->port);
      MemcachedClientPool* p = new MemcachedClientPool(i, info->ip, info->port, timeout_, recv_count_limit_);
      pool_[i] = p;
      if(oldpool){
        oldpool->Clear();
        delete oldpool;
      }
    }
  }//for

  MCE_INFO("PoolManager::CheckValid --> sorted.size:" << sorted.size() << " pool_.size:" << pool_.size());
  vector<int> tmp_valid;
  map<int, int>::iterator it = sorted.begin();
  int limit = cluster*0.9 + 1;
  for(; it!=sorted.end(); ++it){
    if(tmp_valid.size()<limit){
      tmp_valid.push_back(it->second); // error detection. tmp_valid当前不用
    }else{
      //IceUtil::Mutex::Lock lock(mutex_);
      //pool_[it->second]->Clear(); //error detection
      MCE_INFO("PoolManager::CheckValid --> proxy:" << it->second << " connections:" << it->first);
    }
  }

	map<int, MemcachedClient*> tmp_clients; //error detection

	{
		IceUtil::Mutex::Lock lock(mutex_);
		valid_.swap(tmp_valid); 

		///////error detection
		for (set<int>::const_iterator vit = err_proxy_set_.begin(); vit != err_proxy_set_.end(); vit++) {
			int index = *vit;
			MemcachedClient* cli = pool_[index]->GetClient();
			tmp_clients[index] = cli;
		}
	}

	set<int> recovery_proxies = CheckErrProxy(tmp_clients);
	
	{
		IceUtil::Mutex::Lock lock(mutex_);

		for (map<int, int>::const_iterator mit = valid_proxy_map_.begin(); mit != valid_proxy_map_.end(); mit++) {
			if (mit->second > kErrThreshold) {
				if (valid_proxy_map_.size() > kLowestCntR * cluster) {
					valid_proxy_map_.erase(mit->first);
					err_proxy_set_.insert(mit->first);
					MCE_WARN("PoolManager::CheckValid --> proxy err! index:" << mit->first << " err cnt:" << mit->second  << " valid_proxy_map_.size:" << valid_proxy_map_.size() << " err_proxy_set_.size:" << err_proxy_set_.size());   
				} else {
					MCE_WARN("PoolManager::CheckValid --> proxy err! Only " << kLowestCntR * cluster << " is alive!" << " index:" << mit->first << " err cn:" << mit->second  << " valid_proxy_map_.size:" << valid_proxy_map_.size() << " err_proxy_set_.size:" << err_proxy_set_.size());   
				}

			} else {
				valid_proxy_map_[mit->first] = 0; //定时清0
			}
		}

		for (set<int>::const_iterator vit = recovery_proxies.begin(); vit != recovery_proxies.end(); vit++) {
			valid_proxy_map_[*vit] = 0;
			err_proxy_set_.erase(*vit);
			MCE_INFO("PoolManager::CheckValid --> proxy recover. index:" << *vit << " valid_proxy_map_.size:" << valid_proxy_map_.size() << " err_proxy_set_.size:" << err_proxy_set_.size());
		}

		vector<int> tmp;	
		for (map<int, int>::const_iterator mit = valid_proxy_map_.begin(); mit != valid_proxy_map_.end(); mit++) {
			tmp.push_back(mit->first);
		}
		valid_proxy_vec_.swap(tmp);
		MCE_INFO("PoolManager::CheckValid -->  valid_proxy_map_.size:" << valid_proxy_map_.size() << " err_proxy_set_.size:" << err_proxy_set_.size() << " valid_proxy_vec_.size:" << valid_proxy_vec_.size());
	}
	/////////
}

set<int> PoolManager::CheckErrProxy(map<int, MemcachedClient* > clients) {

	const string test_key = "FEED#testabc";
	const string test_value = "testtest";

	set<int> recovery_proxies;

	for (map<int, MemcachedClient* >::const_iterator it = clients.begin(); it != clients.end(); it++) { 
 		MemcachedClient* cli = it->second;	
		int index = it->first;
		if (!cli) {
			MCE_WARN("PoolManager::CheckErrProxy MemcachedClient ptr is null! index:" << index);
			continue;
		}

		bool are_you_ok = true;

		for (int i = 0; i < kRetryThreshold; i++) {
			cli->Set(test_key, test_value, 0);

			vector<string> feedids;
			feedids.push_back(test_key);

			map<string, string> res;
			map<string, int> flags;
			bool succ = cli->Get(feedids, res, flags);
			if(!succ) {
				MCE_WARN("PoolManager::CheckValid --> proxy " << index << " err!");
				are_you_ok = false;
				break;
			}

		}
		if (are_you_ok) {
			recovery_proxies.insert(index);
		}

	}
	return recovery_proxies;

}

bool PoolManager::Init(int timeout, const string& business ,int recv_count_limit){
  timeout_ = timeout;
	business_ = business;
  recv_count_limit_ = recv_count_limit;
  CheckValid(); 

  TaskManager::instance().scheduleRepeated(this);
}

pair<int, MemcachedClient*> PoolManager::GetClient(Ice::Long index){
	if (index >= 0) {
		MCE_INFO("PoolManager::GetClient --> index:" << index);

		IceUtil::Mutex::Lock lock(mutex_);
		if(valid_.size()==0){
			return pair<int, MemcachedClient*>(-1, NULL);
		}
		int s = abs(index_++)%valid_.size();

		//MCE_INFO("PoolManager::GetClient --> " << valid_.size() << " " << s);
		return pair<int, MemcachedClient*>(valid_[s], pool_[valid_[s]]->GetClient());
	} else { //error detection
		
		IceUtil::Mutex::Lock lock(mutex_);
		if(valid_proxy_vec_.size()==0){
			return pair<int, MemcachedClient*>(-1, NULL);
		}
		int s = abs(edi_++)%valid_proxy_vec_.size();
		MCE_INFO("PoolManager::GetClient --> edi_:" << s);

		return pair<int, MemcachedClient*>(valid_proxy_vec_[s], pool_[valid_proxy_vec_[s]]->GetClient());
	}
}

void PoolManager::ReleaseClient(Ice::Long index, MemcachedClient* cli, bool isErr){
  IceUtil::Mutex::Lock lock(mutex_);
	pool_[index]->ReleaseClient(cli);
	if (isErr) {
		map<int, int>::iterator it = valid_proxy_map_.find(index);
		if (it != valid_proxy_map_.end()) {
			int cn = ++(it->second);
			MCE_WARN("PoolManager::ReleaseClient --> proxy err! index:" << index << " err cnt:" << cn);
		}
	} 
}

void PoolManager::handle(){
  CheckValid(); 
  /*int cluster = FeedMemcachedProxyAdapter::instance().GetClusterSize();
  if(cluster == 0){
    cluster = 1;
  }
  for(int i=0; i<cluster; i++){
    string endpoint = FeedMemcachedProxyAdapter::instance().GetEndpoint(i);
    int pos = endpoint.find(":");
    string ip = endpoint.substr(0, pos);
    string strport = endpoint.substr(pos+1, endpoint.size()-pos-1);
    int port = lexical_cast<int>(strport);
    
    IceUtil::Mutex::Lock lock(mutex_);
    string oldip = pool_[i]->GetIp();
    int oldport = pool_[i]->GetPort();
    if(oldip!=ip || oldport!=port){
      MCE_INFO("PoolManager::handle --> proxy changed, old:" << oldip << " " << oldport << "  now:" << ip << " " << port);
      MemcachedClientPool* p = new MemcachedClientPool(i, ip, port, timeout_);
      MemcachedClientPool* tmp = pool_[i];
      pool_[i] = p;
      tmp->Clear();
      delete tmp;
    }
  }*/
}

void PoolManager::PrintStatInfo(){
/*  map<ip::tcp::endpoint, MemcachedClientPool*>::iterator it = pool_.begin();
  int total = 0;
  for(; it!=pool_.end(); ++it){
    total += it->second->FreeCount();
    //total += it->second->BusyCount();
    MCE_INFO("###UPCONN " << index_ << " " << it->first.address().to_string() << " free:" << it->second->FreeCount());
  }
  MCE_INFO("###UPCONN TOTAL " << index_ << " " << total);
*/
}

}}
