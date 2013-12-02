#include "EdmMemcachedClientPool.h"
#include "AdMemcProxyAdapter.h"
#include "IceLogger.h"
#include <boost/lexical_cast.hpp>

using namespace xce::ad;
using namespace boost;

EdmMemcachedClient* EdmMemcachedClientPool::GetClient(){
  //IceUtil::Mutex::Lock lock(mutex_);
  EdmMemcachedClient* cli = NULL;
  if(!free_client_.empty()){
    cli = free_client_.front();
    free_client_.pop_front();
  }
  if(!cli){
    cli = new EdmMemcachedClient(ip_, port_, timeout_, recv_count_limit_);
  }
  return cli;
}

void EdmMemcachedClientPool::ReleaseClient(EdmMemcachedClient* cli){
  //IceUtil::Mutex::Lock lock(mutex_);
  if(!cli->Connected()){
    delete cli;
    return;
  }
  string ip = cli->GetIp();
  int port = cli->GetPort();
  if(ip!=ip_ || port!=port_){
    MCE_WARN("EdmMemcachedClientPool::ReleaseClient --> invalid old:" << ip << " " << port << "  now:" << ip_ << " " << port_);
    delete cli;
    return;
  }
  free_client_.push_back(cli);
}

void EdmMemcachedClientPool::Clear(){
  list<EdmMemcachedClient*>::iterator it = free_client_.begin();
  for(; it!=free_client_.end(); ++it){
    delete *it;
  }
  free_client_.clear();
}

void EdmPoolManager::CheckValid(){
  int cluster = AdMemcProxyAdapter::instance().GetClusterSize();
  if(cluster == 0){
    cluster = 1;
  }
  multimap<int, int> sorted;
  for(int i=0; i<cluster; i++){
    //string endpoint;
    ProxyInfoPtr info;
    try{
      info = AdMemcProxyAdapter::instance().GetProxyInfo(i);
    }catch(Ice::Exception& e){
			MCE_WARN("EdmPoolManager::CheckInvalid --> get endpoint err " << i << " " << e);
			continue;
    }
    if(!info){
      MCE_WARN("EdmPoolManager::CheckInvalid --> get endpoint err " << i);
      continue;
    }
    sorted.insert(pair<int, int>(info->connections, i));
    IceUtil::Mutex::Lock lock(mutex_);
    string oldip;
    int oldport = 0;
    int conn = 0;
    EdmMemcachedClientPool* oldpool = NULL;
    if(pool_.count(i)>0){
      oldip = pool_[i]->GetIp();
      oldport = pool_[i]->GetPort();
      oldpool = pool_[i];
      conn = oldpool->FreeCount();
    }
    MCE_INFO("EdmPoolManager::CheckInvalid --> proxy, index:" << i << "  ip:" << info->ip << " port:" << info->port 
              << " connections:" << info->connections << " " << conn);
    if(oldip!=info->ip || oldport!=info->port){
      MCE_INFO("EdmPoolManager::CheckInvalid --> proxy changed, old:" << oldip << " " << oldport << "  now:" << info->ip << " " << info->port);
      EdmMemcachedClientPool* p = new EdmMemcachedClientPool(i, info->ip, info->port, timeout_, recv_count_limit_);
      pool_[i] = p;
      if(oldpool){
        oldpool->Clear();
        delete oldpool;
      }
    }
  }
  MCE_INFO("EdmPoolManager::CheckInvalid --> " << sorted.size() << " " << pool_.size());
  vector<int> tmp_valid;
  map<int, int>::iterator it = sorted.begin();
  unsigned int limit = (unsigned int)(cluster*0.9 + 1);
  for(; it!=sorted.end(); ++it){
    if(tmp_valid.size()<limit){
      tmp_valid.push_back(it->second);
    }else{
      IceUtil::Mutex::Lock lock(mutex_);
      pool_[it->second]->Clear();
      MCE_INFO("EdmPoolManager::CheckInvalid --> not use " << it->second);
    }
  }
  IceUtil::Mutex::Lock lock(mutex_);
  valid_.swap(tmp_valid);
}

bool EdmPoolManager::Init(int timeout, int recv_count_limit){
  timeout_ = timeout;
  recv_count_limit_ = recv_count_limit;
  CheckValid(); 
  TaskManager::instance().scheduleRepeated(this);
  return true;
}

pair<int, EdmMemcachedClient*> EdmPoolManager::GetClient(Ice::Long index){
  IceUtil::Mutex::Lock lock(mutex_);
  if(valid_.size()==0){
    return pair<int, EdmMemcachedClient*>(-1, NULL);
  }
  int s = abs(index_++)%valid_.size();
  //MCE_INFO("EdmPoolManager::GetClient --> " << valid_.size() << " " << s);
  return pair<int, EdmMemcachedClient*>(valid_[s], pool_[valid_[s]]->GetClient());
}

void EdmPoolManager::ReleaseClient(Ice::Long index, EdmMemcachedClient* cli){
  IceUtil::Mutex::Lock lock(mutex_);
  pool_[index]->ReleaseClient(cli);
}

void EdmPoolManager::handle(){
  CheckValid(); 
}

void EdmPoolManager::PrintStatInfo(){
/*  map<ip::tcp::endpoint, EdmMemcachedClientPool*>::iterator it = pool_.begin();
  int total = 0;
  for(; it!=pool_.end(); ++it){
    total += it->second->FreeCount();
    //total += it->second->BusyCount();
    MCE_INFO("###UPCONN " << index_ << " " << it->first.address().to_string() << " free:" << it->second->FreeCount());
  }
  MCE_INFO("###UPCONN TOTAL " << index_ << " " << total);
*/
}
