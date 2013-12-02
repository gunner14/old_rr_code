#include "group.h"

// #include <iostream>

#include "xoa2/client/zk_client.h"
#include "xoa2/client/zk_util.h"
#include "UtilCxx/src/LogWrapper.h"

namespace xoa2 {
namespace client {

static size_t GetGroupId(const char * group_path) {
  if (group_path == NULL) {
    return 0;
  }

  size_t len = strlen(group_path);
  size_t maxlen = len;
  while(*(group_path + len - 1) != '_') {
    --len;
  }

  size_t index = 0;
  for(size_t i = len; i < maxlen; i++) {
    index = index * 10 + *(group_path + i) - '0';
  }
  return index % 10000;
}

Group::Group(const std::string & path) : path_(path), priority_(0), shard_factor_(1) {
  id_ = GetGroupId(path.c_str());
}

void Group::HandleNodeEvent(const char * path, const std::string & value) {
  // 自身数据的变化，由service节点处理
}

void Group::HandleChildEvent(const char * path, const std::vector<std::string> & children) {
  Load();
}


bool Group::Load() {
  std::vector<std::string> shards;
  if (!ZookeeperClient::Instance().GetChildren(path_.c_str(), &shards)) {
    MCE_WARN("get group children error " << path_);
    return false;
  }

  std::string group_data;
  if(!ZookeeperClient::Instance().GetValue(path_.c_str(), &group_data)) {
    MCE_WARN("get group data error " << path_);
    return false;
  }

  std::map<std::string, std::string> group_props;
  ParseProperty(group_data, &group_props);
  priority_ = GetPropertyInt(group_props, "priority");
  shard_factor_ = GetPropertyInt(group_props, "shard_factor", 1);

  // ZookeeperClient::Instance().AddNodeListener(path_.c_str(), shared_from_this());
  ZookeeperClient::Instance().AddChildListener(path_.c_str(), shared_from_this());

  std::map<int, ShardPtr> shard_ptrs;
  for(size_t i = 0; i < shards.size(); ++i) {
    // 只关注child, 不关注node本身
    std::string shard_path = path_ + '/' + shards[i];
    MCE_INFO("load shard : " << shard_path);
    // ZookeeperClient::Instance().AddChildListener(shard_path.c_str(), shared_from_this());
    ShardPtr shard(new Shard(shard_path));
    shard->LoadEndpoints();
    shard_ptrs.insert(std::make_pair(shard->id(), shard));
  }

  {
    boost::unique_lock<boost::shared_mutex> wlock(shards_mutex_);
    shard_ptrs.swap(shards_);
  }
  return true; 
}

std::string Group::ShardServerAddr(size_t shard) const {
  ShardPtr shard_ptr;
  {
    boost::shared_lock<boost::shared_mutex> rlock(shards_mutex_);
    if (shards_.empty()) {
      return std::string();
    }

    std::map<int, ShardPtr>::const_iterator it = shards_.find(shard);
    if (it != shards_.end()) {
      shard_ptr = it->second;
    } else {
      shard_ptr = shards_.begin()->second;
    }
  }

  if (shard_ptr) {
    return shard_ptr->SelectEndpoint();
  }
  return std::string();
}

}
}

