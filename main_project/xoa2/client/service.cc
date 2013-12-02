#include "service.h"

#include <iostream>
#include <sstream>

#include <boost/algorithm/string.hpp>

#include "xoa2/client/zk_client.h"
#include "UtilCxx/src/LogWrapper.h"

namespace xoa2 {
namespace client {

Service::Service(const std::string & id) : id_(id) {
  path_ = GetPathById(id);
}

std::string Service::LocateService(int shard) const {
  if (active_group_) {
    return active_group_->ShardServerAddr(shard);
  }
  MCE_WARN(id_ << " no active group");
  return std::string();
}

void Service::UpdateServiceState(const std::string & endpoint) {
}

bool Service::Load() {
  std::vector<std::string> groups;
  if (!ZookeeperClient::Instance().GetChildren(path_.c_str(), &groups)) {
    return false;
  }
  // .service_group兄弟节点有增删时，通知
  ZookeeperClient::Instance().AddChildListener(path_.c_str(), shared_from_this());

  GroupPtr active_group;
  std::vector<GroupPtr> inactive_groups;

  // 需要监听所有 group 的数据, 但只加载active group 子节点即可 
  for(size_t i = 0; i < groups.size(); ++i) {
    MCE_DEBUG(id_ << " load group " << groups[i]);
    std::string group_path = path_ + '/' + groups[i];

    // 关注所有子节点的数据变化
    ZookeeperClient::Instance().AddNodeListener(group_path.c_str(), shared_from_this());

    GroupPtr group_ptr(new Group(group_path));
    if (group_ptr->Load()) {
      if (!active_group) {
        active_group = group_ptr;
      } else {
        if (active_group->priority() < group_ptr->priority()) {
          inactive_groups.push_back(active_group);
          active_group = group_ptr;
        } else {
          inactive_groups.push_back(group_ptr);
        }
      }
    }
  }

  if (active_group) {
    active_group_ = active_group;
    boost::mutex::scoped_lock(mutex_);
    inactive_groups.swap(inactive_groups_);
  }
  return true;
}

void Service::HandleNodeEvent(const char * path, const std::string & value) {
  // child group data change event
  // TODO: 重新加载是简单粗暴的方法
  Load();
}

void Service::HandleChildEvent(const char * path, const std::vector<std::string> & children) {
  Load();
}

std::string Service::GetPathById(const std::string & service_id) {
  std::stringstream ss;
  std::vector<std::string> strs;
  boost::split(strs, service_id, boost::is_any_of("."), boost::token_compress_on);
  for (int i = strs.size() - 1; i >= 0; --i) {
    ss << '/' << strs[i];
  }

  return ss.str();
}

}
}

