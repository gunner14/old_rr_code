#include "zk_client.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>

#include "UtilCxx/src/LogWrapper.h"

namespace search{
namespace relation{
bool ZookeeperClient::AddNodeListener(const char * path, ZkEventListenerPtr listenter) {
  MCE_DEBUG(path << " add node listener.");
  cout << "AddNodeListener path:" << path << " add node listener." << endl;
  bool watched = true;//表示是否已经watch过?

  {
    boost::mutex::scoped_lock(node_listener_mutex_);
    std::map<std::string, std::set<ZkEventListenerPtr> >::iterator it = node_listeners_.find(path);
    if (it == node_listeners_.end()) {
      node_listeners_[path].insert(listenter);
      watched = false;
    } else {
      it->second.insert(listenter);
    }
  }

  if (!watched) {//如果没有watch过
    std::string value;
    MCE_DEBUG(path << " start watch node.");
    return DoGetValue(path, &value, true);
  }
  MCE_DEBUG(path << " already watch node.");
  return true;
}

bool ZookeeperClient::AddChildListener(const char * path, ZkEventListenerPtr listenter) {
  MCE_DEBUG(path << " add child listener.");
  cout << "AddChildListener path:" << path << " add node listener." << endl;
  bool watched = true;
  {
    boost::mutex::scoped_lock(child_listener_mutex_);
    std::map<std::string, std::set<ZkEventListenerPtr> >::iterator it = child_listeners_.find(path);
    if (it == child_listeners_.end()) {
      child_listeners_[path].insert(listenter);
      watched = false;
    } else {
      it->second.insert(listenter);
    }
  }

  if (!watched) {
    MCE_DEBUG(path << " start watch children.");
    std::vector<std::string> children;
    return DoGetChildren(path, &children, true);
  }

  MCE_DEBUG(path << " already watch children.");
  return true;
}

void ZookeeperClient::RemoveNodeListeners(const char * path) {
  boost::mutex::scoped_lock(node_listener_mutex_);
  node_listeners_.erase(path);
}

void ZookeeperClient::RemoveChildListeners(const char * path) {
  boost::mutex::scoped_lock(child_listener_mutex_);
  child_listeners_.erase(path);
}

void ZookeeperClient::EventWatcher(zhandle_t *zzh, int type, int state, 
    const char *path, void * watcher_ctx) {
  cout << "EventWatcher invoded" << endl;

  ZookeeperClient * client = reinterpret_cast<ZookeeperClient *>(watcher_ctx);
  if (type == ZOO_SESSION_EVENT) {
    client->OnSessionEvent(path, state);
  } else if (type == ZOO_CHANGED_EVENT) {
    client->OnNodeEvent(path, state);
  } else if (type == ZOO_CHILD_EVENT) {
    client->OnChildEvent(path, state);
  } else if (type == ZOO_DELETED_EVENT) {
    // 该事件忽略, 所有的删除事件，一律交给父节点的ZOO_CHILD_EVENT处理
    MCE_INFO("ZOO_DELETED_EVENT node=" << path << ",type=" << type 
        << ",state=" << state);
  } else {
    MCE_WARN("unhandled child event : " << path << ",type=" << type 
        << ",state=" << state);
  }
}

void ZookeeperClient::TriggerNodeListeners(const char * path, const string & data) {
  boost::mutex::scoped_lock(node_listener_mutex_);

  std::map<std::string, std::set<ZkEventListenerPtr> >::iterator it = node_listeners_.find(path);
  if (it != node_listeners_.end()) {
    std::set<ZkEventListenerPtr>::iterator i = it->second.begin();
    // TODO : 同步调用，性能上可能不够好
    while(i != it->second.end()) {
      //boost::shared_ptr<ZkEventListener> sp = (*i).lock();
      boost::shared_ptr<ZkEventListener> sp = (*i);
      if (sp) {
        MCE_DEBUG("handle group node event : " << path);
        sp->HandleNodeEvent(path, data);
        ++i;
      } else {
        MCE_DEBUG("node " << path << " null listener.");
        it->second.erase(i++);
      }
    }
  } else {
    MCE_DEBUG("Path no group node listeners : " << path);
  }
}

void ZookeeperClient::TriggerChildListeners(const char * path, const std::vector<std::string> & children) {
  boost::mutex::scoped_lock(child_listener_mutex_);

  std::map<std::string, std::set<ZkEventListenerPtr> >::iterator it = child_listeners_.find(path);
  if (it != child_listeners_.end()) {
    std::set<ZkEventListenerPtr>::iterator i = it->second.begin();
    // TODO : 同步调用，性能上可能不够好
    while(i != it->second.end()) {
      //boost::shared_ptr<ZkEventListener> sp = (*i).lock();
      boost::shared_ptr<ZkEventListener> sp = (*i);
      if (sp) {
        MCE_DEBUG("handle service node event : " << path);
        sp->HandleChildEvent(path, children);
        ++i;
      } else {
        MCE_DEBUG("remove null service node listener : " << path);
        it->second.erase(i++);
      }
    }
  } else {
    MCE_DEBUG("Path no service node listeners : " << path);
  }
}

// 触发情况：
// 1. 当前连接断掉
// 2. 连接断掉后转到其他服务
// 3. 连接断掉后重新恢复
// 4. expire? 未测
void ZookeeperClient::OnSessionEvent(const char * path, int state) {
  MCE_INFO("OnSessionEvent state=" << state);

  if (state == ZOO_EXPIRED_SESSION_STATE) {
    MCE_INFO("zookeeper expired.");
    if (zhandle_) {
      zookeeper_close(zhandle_);
      zhandle_ = NULL;
    }
    while(true) {
      // TODO: 在这里循环，是否会导致没有线程处理其他事件?
      sleep(2 + rand() % 10);
      if (Init()) {
        break;
      }
      MCE_WARN("zookeeper reconnect failed.");
    }
    MCE_INFO("zookeeper reconnect success on session expiration.");
  } else if(state == ZOO_CONNECTED_STATE) {
    MCE_DEBUG("zookeeper connected.");
  } else if(state == ZOO_CONNECTING_STATE) {
    MCE_DEBUG("zookeeper connecting.");
  } else {
    MCE_INFO("ZOO_SESSION_EVENT unhandled state : " << state);
  }
}

void ZookeeperClient::OnChildEvent(const char * path, int state) {
  MCE_INFO("OnChildEvent : " << path << " state=" << state);
  std::vector<std::string> children; 
  DoGetChildren(path, &children, false);
  MCE_INFO("ZOO_CHILD_EVENT : " << path << " child_count=" << children.size());
  TriggerChildListeners(path, children);
  DoGetChildren(path, &children, true);
}

void ZookeeperClient::OnNodeEvent(const char * path, int state) {
  std::string data;
  DoGetValue(path, &data, false);
  MCE_INFO("ZOO_CHANGED_EVENT : " << path << " data=" << data);
  TriggerNodeListeners(path, data);

  std::string data2;
  DoGetValue(path, &data2, true); // start listening
  if (data != data2) {
    // TODO : 数据变化漏通知的情况
  }
}

bool ZookeeperClient::Init() {
  if (zhandle_) {
    zookeeper_close(zhandle_);
  }

  //cfg_line = "st0.xce.d.xiaonei.com:2181";
  //ifstream cfg("./zookeeper.conf");
  //if (cfg.is_open() && cfg.good()) {
  //  getline(cfg, cfg_line);
  //}

  //if (cfg_line.empty()) {
  //  MCE_WARN("zookeepr Init() error : bad server addr");
  //  return false;
  //}

  // clientid_t zk_clientid;
  // zk_clientid.client_id = 11123408119L;
  // strcpy(zk_clientid.passwd, "hello");
  // zhandle_ = zookeeper_init(cfg_line.c_str(), NULL, 200, &zk_clientid, NULL, 0);
  // MCE_DEBUG("zk_clientid :" << zk_clientid.client_id << " " << zk_clientid.passwd);
  zhandle_ = zookeeper_init(cfg_line.c_str(), NULL, 200, NULL, NULL, 0);

  if (zhandle_) {
    MCE_WARN("zookeepr Init() add watchers.");
    {
      boost::mutex::scoped_lock(node_listener_mutex_);
      std::map<std::string, std::set<ZkEventListenerPtr> >::iterator it = node_listeners_.begin();
      std::string tmp;
      while(it != node_listeners_.end()) {
        DoGetValue(it->first.c_str(), &tmp, true);
        ++it;
      }
    }

    {
      boost::mutex::scoped_lock(child_listener_mutex_);
      std::map<std::string, std::set<ZkEventListenerPtr> >::iterator it = child_listeners_.begin();
      std::vector<std::string> tmp;
      while(it != child_listeners_.end()) {
        DoGetChildren(it->first.c_str(), &tmp, true);
        ++it;
      }
    }
  }

  MCE_WARN("connect zk server " << cfg_line<< " success=" << (zhandle_ != NULL));
  return zhandle_ != NULL;
}

bool ZookeeperClient::DoGetChildren(const char * path, std::vector<std::string> * children, bool watch) {
  if (!zhandle_) {
    if (!Init()) {
      return false;
    }
  }

  struct String_vector str_vec;
  int rc = 0;
  if (watch) {
    rc = zoo_wget_children(zhandle_, path, &ZookeeperClient::EventWatcher, reinterpret_cast<void*>(this), &str_vec);
  } else {
    rc = zoo_wget_children(zhandle_, path, NULL, NULL, &str_vec);
  }
  if (rc != 0) {
    MCE_WARN("zoo_wget_children() error. path=" << path << " rc=" << rc << "," << zerror(rc));
    return false;
  }
  for(int i = 0; i < str_vec.count; ++i) {
    children->push_back(str_vec.data[i]);
  }
  deallocate_String_vector(&str_vec);
  return true;
}

bool ZookeeperClient::DoGetValue(const char * path, std::string * value, bool watch) {
  if (!zhandle_) {
    if (!Init()) {
      return false;
    }
  }

  static const int ZK_BUF_LEN = 2048;
  // TODO : 将get的结果缓存在本地
  struct Stat stat;
  char * zk_buf = new char[ZK_BUF_LEN];
  zk_buf[ZK_BUF_LEN - 1] = '\0';
  int buflen = ZK_BUF_LEN - 1;

  int rc = 0;
  if (watch) {
    rc = zoo_wget(zhandle_, path, &ZookeeperClient::EventWatcher, reinterpret_cast<void*>(this), zk_buf, &buflen, &stat);
  } else {
    rc = zoo_wget(zhandle_, path, NULL, NULL, zk_buf, &buflen, &stat);
  }
  if (rc) {
    MCE_WARN("zoo_wget() error. path=" << path << " rc=" << rc << "," << zerror(rc));
  } else {
    zk_buf[buflen] = '\0';
    *value = zk_buf;
  }

  delete [] zk_buf;

  return rc == 0;
}

bool ZookeeperClient::Update(const char * path, const std::string & value) {
  return false;
}

bool ZookeeperClient::Create(const char * path, const string & data, bool is_tmp) {
  if (!zhandle_) {
    if (!Init()) {
      return false;
    }
  }

  char res_path[128];
  int rc = zoo_create(zhandle_, path, data.c_str(), data.size(), &ZOO_OPEN_ACL_UNSAFE, is_tmp ? ZOO_EPHEMERAL : 0, res_path, 128);
  if (rc) {
    MCE_WARN("zoo_create() error. path=" << path << " rc=" << rc << "," << zerror(rc));
  }

  return rc == 0;
}

bool ZookeeperClient::Delete(const char * path) {
  return false;
}

}
}

