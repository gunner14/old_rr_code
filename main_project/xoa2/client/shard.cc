#include "shard.h"

// #include <iostream>
#include "xoa2/client/zk_util.h"
#include "xoa2/client/endpoint.h"
#include "UtilCxx/src/LogWrapper.h"

namespace xoa2 {
namespace client {

void Shard::HandleNodeEvent(const char * path, const std::string & value) {
  std::string spath(path);
  size_t pos = spath.find_last_of('/');
  std::string addr = spath.substr(pos + 1);

  EndpointPtr ep(new Endpoint(path));
  ep->Load();

  MCE_INFO("HandleNodeEvent : " << addr 
    << " enabled=" << ep->enabled()
    << " healthy=" << ep->healthy());

  // 为减少lock时间，增加两个辅助的vector
  std::vector<EndpointPtr> active_eps, inactive_eps;
  {
    boost::shared_lock<boost::shared_mutex> rlock(endpoints_mutex_);
    active_eps = active_endpoints_;
    inactive_eps = inactive_endpoints_;
  }

  for(size_t i = 0; i < active_eps.size(); ++i) {
    if (active_eps[i]->addr() == addr) {
      active_eps.erase(active_eps.begin() + i);
      break;
    }
  }

  for(size_t i = 0; i < inactive_eps.size(); ++i) {
    if (inactive_eps[i]->addr() == addr) {
      inactive_eps.erase(inactive_eps.begin() + i);
      break;
    }
  }
  
  if (ep->active()) {
    active_eps.push_back(ep);
  } else {
    inactive_eps.push_back(ep);
  }

  if (active_eps.empty()) {
    // 没有可用的节点时，启用不可用节点
    boost::unique_lock<boost::shared_mutex> wlock(endpoints_mutex_);
    active_endpoints_.swap(inactive_eps);
    inactive_endpoints_.clear();
  } else {
    boost::unique_lock<boost::shared_mutex> wlock(endpoints_mutex_);
    active_endpoints_.swap(active_eps);
    inactive_endpoints_.swap(inactive_eps);
  }

  Dump();
}

void Shard::HandleChildEvent(const char * path, const std::vector<std::string> & children) {
  MCE_INFO("HandleChildEvent " << path);
  LoadEndpoints();
  Dump();
}

void Shard::Dump() {
  boost::shared_lock<boost::shared_mutex> rlock(endpoints_mutex_);
  MCE_INFO("active_endpoints_:");
  for(size_t i = 0; i < active_endpoints_.size(); ++i) {
    MCE_INFO("\t" << active_endpoints_[i]->addr());
  }

  MCE_INFO("inactive_endpoints_:");
  for(size_t i = 0; i < inactive_endpoints_.size(); ++i) {
    MCE_INFO("\t" << inactive_endpoints_[i]->addr());
  }
}

static int GetShardId(const std::string& shard_path) {
  return 0;
}

Shard::Shard(const std::string & path) : path_(path) {
  srand(time(NULL));
  id_ = GetShardId(path_);
}

std::string Shard::SelectEndpoint() const {
  boost::shared_lock<boost::shared_mutex> rlock(endpoints_mutex_);
  if (active_endpoints_.empty()) {
    return std::string();
  }
  int idx = rand() % active_endpoints_.size();
  return active_endpoints_.at(idx)->addr();
}

bool Shard::LoadEndpoints() {
  std::vector<std::string> endpoints; 
  if(!ZookeeperClient::Instance().GetChildren(path_.c_str(), &endpoints)) {
    return false;
  }
  ZookeeperClient::Instance().AddChildListener(path_.c_str(), shared_from_this());
  
  std::vector<EndpointPtr> active_endpoints, inactive_endpoints;

  for(size_t i = 0; i < endpoints.size(); ++i) {
    MCE_INFO("load endpoint : " << endpoints[i]);
    // ip:port 节点的值有变化时，通知shard
    ZookeeperClient::Instance().AddNodeListener((path_ + '/' + endpoints[i]).c_str(), shared_from_this());
    
    EndpointPtr ep(new Endpoint(path_ + '/' + endpoints[i])); 
    ep->Load();
    if (ep->active()) {
      active_endpoints.push_back(ep);
    } else {
      inactive_endpoints.push_back(ep);
    }
  }

  if (active_endpoints.empty()) {
    // 没有可用的节点时，启用不可用节点
    boost::unique_lock<boost::shared_mutex> wlock(endpoints_mutex_);
    active_endpoints_.swap(inactive_endpoints);
    inactive_endpoints_.clear();
  } else {
    boost::unique_lock<boost::shared_mutex> wlock(endpoints_mutex_);
    active_endpoints_.swap(active_endpoints);
    inactive_endpoints_.swap(inactive_endpoints);
  }
  return true;
}

}
}
