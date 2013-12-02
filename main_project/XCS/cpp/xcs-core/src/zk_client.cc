/*
  Copyright 2012-08-28 yuwei.mu@renren-inc.com
                       shiqiang.ding@renren-inc.com
  
  对zookeeper C的API进行了基本的封装。
  在expired重连之后，重新添加权限，创建临时节点，增加监听。
  而且还对普通的监听操作进行重新注册。
*/

#include "zk_client.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <boost/algorithm/string.hpp>

#include <utility>

#include "UtilCxx/src/LogWrapper.h"

namespace xcs {
ZookeeperClient ZookeeperClient::instance_client;
ZookeeperClient ZookeeperClient::instance_server;
ZookeeperClient ZookeeperClient::instance;

int ZookeeperClient::ErrorRecoding(int zk_code) {
  switch (zk_code) {
    case 0: return 0;
    case -102: return -1;
    case -9:  // zoo_add_auth()
    case -4: return -2;  // 其他操作

    case -110: return -4;
    case -101: return -5;
    case -111: return -6;

    case -8: return -9;

    default: return -7;
  }
}

int ZookeeperClient::AddNodeListener(const char *path,
                                     ZkEventListenerPtr listenter) {
  MCE_DEBUG("ZookeeperClient::AddNodeListener() "
      << path << " add node listener.");
  bool watched = true;

  {
    boost::mutex::scoped_lock(node_listener_mutex_);
    std::map<std::string, std::set<ZkEventListenerPtr> >::iterator it;
    it = node_listeners_.find(path);
    if (it == node_listeners_.end()) {
      node_listeners_[path].insert(listenter);
      watched = false;
    } else {
      it->second.insert(listenter);
    }
  }

  if (!watched) {
    std::string value;
    MCE_DEBUG("ZookeeperClient::AddNodeListener() "
        << path << " start watch node.");
    return DoGetValue(path, &value, true);
  }
  MCE_DEBUG("ZookeeperClient::AddNodeListener() "
      << path << " already watch node.");
  return 0;
}

int ZookeeperClient::AddChildListener(const char *path,
                                       ZkEventListenerPtr listenter) {
  MCE_DEBUG("ZookeeperClient::AddChildListener() "
      << path << " add child listener.");
  bool watched = true;
  {
    boost::mutex::scoped_lock(child_listener_mutex_);
    std::map<std::string, std::set<ZkEventListenerPtr> >::iterator it;
    it = child_listeners_.find(path);
    if (it == child_listeners_.end()) {
      child_listeners_[path].insert(listenter);
      watched = false;
    } else {
      it->second.insert(listenter);
    }
  }

  if (!watched) {
    MCE_DEBUG("ZookeeperClient::AddChildListener() "
        << path << " start watch children.");
    std::vector<std::string> children;
    return DoGetChildren(path, &children, true);
  }

  MCE_DEBUG("ZookeeperClient::AddChildListener() "
      << path << " already watch children.");
  return 0;
}

int ZookeeperClient::AddExpiredListener(const char *path,
                                         ZkEventListenerPtr listenter) {
  MCE_DEBUG("ZookeeperClient::AddExpiredListener() "
      << path << " add expired listener.");
  bool watched = true;
  {
    boost::mutex::scoped_lock(expired_listener_mutex_);
    std::map<std::string, std::set<ZkEventListenerPtr> >::iterator it;
    it = expired_listeners_.find(path);
    if (it == expired_listeners_.end()) {
      expired_listeners_[path].insert(listenter);
      watched = false;
    } else {
      it->second.insert(listenter);
    }
  }
  if (!watched) {
    MCE_DEBUG("ZookeeperClient::AddExpiredListener() "
        << path << " start watch expired.");
    std::string value;
    return DoGetValue(path, &value, true);
  }

  MCE_DEBUG("ZookeeperClient::AddExpiredListener() "
      << path << "already watch expired.");
  return 0;
}

int ZookeeperClient::AddAuthInfo(const std::string &name,
                                 const std::string &passwd) {
  MCE_DEBUG("ZookeeperClient::AddAuthInfo() add authority. name = " << name);

  {
    boost::mutex::scoped_lock(auth_info_mutex_);
    struct AuthInfo tmp(name, passwd);
    auth_info_.push_back(tmp);
  }
  // 若是已经连接好了,直接注册给zookeeper,立刻生效
  if (zhandle_ != NULL && (zoo_state(zhandle_) == ZOO_CONNECTED_STATE)) {
    const std::string scheme = "digest";
    std::string cert = "";
    cert = name + ":" + passwd;
    int rc = zoo_add_auth(zhandle_, scheme.c_str(),
                          cert.c_str(), cert.size(), NULL, NULL);
    if (rc != ZOK) {
      MCE_WARN("ZookeeperClient::AddAuthInfo() error: rc="
          << rc << "." << zerror(rc));
      return ErrorRecoding(rc);
    }
  }

  return 0;
}

void ZookeeperClient::RemoveNodeListeners(const char *path) {
  boost::mutex::scoped_lock(node_listener_mutex_);
  node_listeners_.erase(path);
}

void ZookeeperClient::RemoveChildListeners(const char *path) {
  boost::mutex::scoped_lock(child_listener_mutex_);
  child_listeners_.erase(path);
}

void ZookeeperClient::RemoveExpiredListeners(const char *path) {
  boost::mutex::scoped_lock(expired_listener_mutex_);
  expired_listeners_.erase(path);
}

void ZookeeperClient::RemoveTempPath(const std::string &path) {
  boost::mutex::scoped_lock(temporary_path_mutex_);
  std::map<std::string, std::string>::iterator it = temporary_path_.find(path);
  if (it != temporary_path_.end()) {
    temporary_path_.erase(it);
  }
}

void ZookeeperClient::InitWatcher(zhandle_t *zzh, int type, int state,
    const char *path, void *watcher_ctx) {
  if (state == ZOO_CONNECTED_STATE) {
//    void *tmp_ptr = const_cast<void *>(zoo_get_context(zzh));
    int *connect_state = static_cast<int *>(watcher_ctx);
    *connect_state = ZOO_CONNECTED_STATE;
//    zoo_set_context(zzh, NULL);
    zoo_set_watcher(zzh, NULL);
    MCE_DEBUG("ZookeeperClient::InitWatcher() ZOO_CONNECTED_STATE");
  } else if (state == ZOO_AUTH_FAILED_STATE) {
    MCE_DEBUG("ZookeeperClient::InitWatcher() ZOO_AUTH_FAILED_STATE");
  } else if (state == ZOO_EXPIRED_SESSION_STATE) {
    MCE_DEBUG("ZookeeperClient::InitWatcher() ZOO_EXPIRED_SESSION_STATE");
  } else if (state == ZOO_CONNECTING_STATE) {
    MCE_DEBUG("ZookeeperClient::InitWatcher() ZOO_CONNECTING_STATE");
  } else if (state == ZOO_ASSOCIATING_STATE) {
    MCE_DEBUG("ZookeeperClient::InitWatcher() ZOO_ASSOCIATING_STATE");
  }
}

void ZookeeperClient::EventWatcher(zhandle_t *zzh, int type, int state,
    const char *path, void *watcher_ctx) {
  ZookeeperClient *client = reinterpret_cast<ZookeeperClient *>(watcher_ctx);
  if (type == ZOO_SESSION_EVENT) {
    MCE_DEBUG("ZookeeperClient::EventWatcher() ZOO_SESSION_EVENT");
    client->OnSessionEvent(path, state);
  } else if (type == ZOO_CHANGED_EVENT) {
    MCE_DEBUG("ZookeeperClient::EventWatcher() ZOO_CHANGED_EVENT node="
        << path << ",state=" << state);
    client->OnNodeEvent(path, state);
  } else if (type == ZOO_CHILD_EVENT) {
    MCE_DEBUG("ZookeeperClient::EventWatcher() ZOO_CHILD_EVENT node="
        << path << ",state=" << state);
    client->OnChildEvent(path, state);
  } else if (type == ZOO_DELETED_EVENT) {
    // 删除不存在的路径的监听器
    client->RemoveChildListeners(path);
    client->RemoveNodeListeners(path);
    client->RemoveExpiredListeners(path);

    // 该事件忽略, 所有的删除事件，一律交给父节点的ZOO_CHILD_EVENT处理
    MCE_INFO("ZookeeperClient::EventWatcher() ZOO_DELETED_EVENT node="
        << path << ",state=" << state);
  } else {
    MCE_WARN("ZookeeperClient::EventWatcher() error: unhandled child event : "
        << path << ",state=" << state);
  }
}

void ZookeeperClient::TriggerNodeListeners(const char *path,
                                           const std::string &data) {
  boost::mutex::scoped_lock(node_listener_mutex_);

  std::map<std::string, std::set<ZkEventListenerPtr> >::iterator it;
  it  = node_listeners_.find(path);
  if (it != node_listeners_.end()) {
    MCE_DEBUG("ZookeeperClient::TriggerNodeListeners() path= " << path);
    std::set<ZkEventListenerPtr>::iterator i = it->second.begin();
    // TODO(yuwei.mu) : 同步调用，性能上可能不够好
    while (i != it->second.end()) {
      boost::shared_ptr<ZkEventListener> sp = (*i).lock();
      if (sp) {
        sp->HandleNodeEvent(path, data);
        ++i;
      } else {
        it->second.erase(i++);
      }
    }
  } else {
    MCE_WARN("ZookeeperClient::TriggerNodeListeners() error: "
        << path << " no group node listeners");
  }
}

void ZookeeperClient::TriggerChildListeners(
     const char *path, const std::vector<std::string> &children) {
  boost::mutex::scoped_lock(child_listener_mutex_);

  std::map<std::string, std::set<ZkEventListenerPtr> >::iterator it;
  it = child_listeners_.find(path);
  if (it != child_listeners_.end()) {
    MCE_DEBUG("ZookeeperClient::TriggerChildListeners() path= " << path);
    std::set<ZkEventListenerPtr>::iterator i = it->second.begin();
    // TODO(yuwei.mu) : 同步调用，性能上可能不够好
    while (i != it->second.end()) {
      boost::shared_ptr<ZkEventListener> sp = (*i).lock();
      if (sp) {
        sp->HandleChildEvent(path, children);
        ++i;
      } else {
        it->second.erase(i++);
      }
    }
  } else {
    MCE_WARN("ZookeeperClient::TriggerChildListeners() error: "
        << path << " no group node listeners");
  }
}

void ZookeeperClient::TriggerExpiredListeners() {
  boost::mutex::scoped_lock(expired_listener_mutex_);

  std::map<std::string, std::set<ZkEventListenerPtr> >::iterator it;
  it = expired_listeners_.begin();
  while (it != expired_listeners_.end()) {
    MCE_DEBUG("ZookeeperClient::TriggerExpiredListeners() path= "
        << it->first);
    std::set<ZkEventListenerPtr>::iterator i = it->second.begin();
    // TODO(yuwei.mu) : 同步调用，性能上可能不够好
    while (i != it->second.end()) {
      boost::shared_ptr<ZkEventListener> sp = (*i).lock();
      if (sp) {
        sp->HandleExpiredEvent();
        ++i;
      } else {
        it->second.erase(i++);
      }
    }
    it++;
  }
}

// 触发情况：
// 1. 当前连接断掉
// 2. 连接断掉后转到其他服务
// 3. 连接断掉后重新恢复
// 4. expire 已测
void ZookeeperClient::OnSessionEvent(const char *path, int state) {
  MCE_DEBUG("ZookeeperClient::OnSessionEvent() state=" << state);

  if (state == ZOO_EXPIRED_SESSION_STATE) {
    MCE_INFO("ZookeeperClient::OnSessionEvent() zookeeper expired.");
    if (zhandle_) {
      zookeeper_close(zhandle_);
      zhandle_ = NULL;
    }
    // expired状态和connecting状态zk会阻塞本程序运行，故而while (true)可以省略
    while (true) {
      // TODO(yuwei.mu) : 在这里循环，是否会导致没有线程处理其他事件?

      sleep(2 + rand() % 10);
      if (!Init()) {
        break;
      }
    }
    MCE_INFO("ZookeeperClient::OnSessionEvent() zookeeper reconnect succeed.");

    // 执行到这里只是给外部发送一个expired的状态，已经重连上了。
    TriggerExpiredListeners();

  } else if (state == ZOO_CONNECTED_STATE) {
    MCE_INFO("ZookeeperClient::OnSessionEvent() zookeeper connected.");
  } else if (state == ZOO_CONNECTING_STATE) {
    MCE_INFO("ZookeeperClient::OnSessionEvent() zookeeper connecting.");
  } else {
    MCE_WARN("ZookeeperClient::OnSessionEvent() error: "
        << "ZOO_SESSION_EVENT unhandled state : " << state);
  }
}

void ZookeeperClient::OnChildEvent(const char *path, int state) {
  MCE_DEBUG("ZookeeperClient::OnChildEvent() path= "
      << path << " state=" << state);
  std::vector<std::string> children;
  DoGetChildren(path, &children, false);
  TriggerChildListeners(path, children);
  DoGetChildren(path, &children, true);
}

void ZookeeperClient::OnNodeEvent(const char *path, int state) {
  MCE_DEBUG("ZookeeperClient::OnNodeEvent() path= "
      << path << " state=" << state);
  std::string data;
  DoGetValue(path, &data, false);
  TriggerNodeListeners(path, data);

  std::string data2;
  DoGetValue(path, &data2, true);  // start listening
  if (data != data2) {
    // TODO(yuwei.mu) : 数据变化漏通知的情况
  }
}

int ZookeeperClient::Init() {
  MCE_DEBUG("ZookeeperClient::Init()");

  if (zhandle_) {
    zookeeper_close(zhandle_);
  }
//  zoo_set_debug_level(ZOO_LOG_LEVEL_INFO);

  if (address_root_ == "") {
    MCE_WARN("ZookeeperClient::Init() error: bad server addr");
    return -3;
  }

  int connect_state = 0;  // 标示连接状态
  zhandle_ = zookeeper_init(address_root_.c_str(),
                            &ZookeeperClient::InitWatcher,
                            session_timeout_, NULL,
                            static_cast<void *>(&connect_state), 0);

  int cnt_loop = 0;
  while (connect_state == 0) {
    // 经测试正常连接一般循环15-26次,网络不好情况连接280-1000。
    if (cnt_loop == 5000) {
      connect_state = ZOO_CONNECTING_STATE;
    }

    cnt_loop++;
    usleep(1);    // 必须存在
  }
  if (connect_state != ZOO_CONNECTED_STATE) {
    MCE_WARN("ZookeeperClient::Init() error: EXPIRED SESSION!");
    return -3;
  }
  MCE_DEBUG("ZookeeperClient::Init() zookeeper connected!");
  if (zhandle_) {
    MCE_DEBUG("ZookeeperClient::Init() add authority.");
    {
      boost::mutex::scoped_lock(auth_info_mutex_);
      std::vector<AuthInfo>::iterator it = auth_info_.begin();
      const std::string scheme = "digest";
      std::string cert = "";
      while (it != auth_info_.end()) {
        cert = (*it).name_ + ":" + (*it).passwd_;
        int rc = zoo_add_auth(zhandle_, scheme.c_str(),
                              cert.c_str(), cert.size(), NULL, NULL);
        if (rc != ZOK) {
          MCE_WARN("zookeepr Init() error : rc=" << rc << "," << zerror(rc));
          return ErrorRecoding(rc);
        }
        cert = "";
        it++;
      }
    }

    MCE_DEBUG("ZookeeperClient::Init() create temporary path.");
    {
      boost::mutex::scoped_lock(temporary_path_mutex_);
      std::map<std::string, std::string>::iterator it = temporary_path_.begin();
      char res_path[128];
      while (it != temporary_path_.end()) {
        const char *path = it->first.c_str();
        std::string data = it->second;
        zoo_create(zhandle_, path, data.c_str(), data.size(),
                   &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL, res_path, 128);
        ++it;
      }
    }

    MCE_DEBUG("ZookeeperClient::Init() add watchers.");
    {
      boost::mutex::scoped_lock(node_listener_mutex_);
      std::map<std::string, std::set<ZkEventListenerPtr> >::iterator it;
      it = node_listeners_.begin();
      std::string tmp;
      while (it != node_listeners_.end()) {
        DoGetValue(it->first.c_str(), &tmp, true);
        ++it;
      }
    }

    {
      boost::mutex::scoped_lock(child_listener_mutex_);
      std::map<std::string, std::set<ZkEventListenerPtr> >::iterator it;
      it = child_listeners_.begin();
      std::vector<std::string> tmp;
      while (it != child_listeners_.end()) {
        DoGetChildren(it->first.c_str(), &tmp, true);
        ++it;
      }
    }

    {
      boost::mutex::scoped_lock(expired_listener_mutex_);
      std::map<std::string, std::set<ZkEventListenerPtr> >::iterator it;
      it = expired_listeners_.begin();
      std::string tmp;
      while (it != expired_listeners_.end()) {
        DoGetValue(it->first.c_str(), &tmp, true);
        ++it;
      }
    }
  }

  MCE_DEBUG("ZookeeperClient::Init() connect zk server="
      << address_root_ << " success!");
  return 0;
}

int ZookeeperClient::DoGetChildren(
     const char *path, std::vector<std::string> *children, bool watch) {
  if (!zhandle_) {
    int rc = Init();
    if (rc) {
      return rc;
    }
  }

  struct String_vector str_vec;
  int rc = 0;
  if (watch) {
    rc = zoo_wget_children(zhandle_, path, &ZookeeperClient::EventWatcher,
                           reinterpret_cast<void*>(this), &str_vec);
  } else {
    rc = zoo_wget_children(zhandle_, path, NULL, NULL, &str_vec);
  }
  if (rc) {
    MCE_WARN("ZookeeperClient::DoGetChildren() zoo_wget_children() error.path="
        << path << " rc=" << rc << "," << zerror(rc));
    return ErrorRecoding(rc);
  }
  children->clear();
  for (int i = 0; i < str_vec.count; ++i) {
    children->push_back(str_vec.data[i]);
  }
  deallocate_String_vector(&str_vec);
  return 0;
}

int ZookeeperClient::DoGetValue(
     const char *path, std::string *value, bool watch) {
  if (!zhandle_) {
    int rc = Init();
    if (rc) {
      return rc;
    }
  }

  static const int ZK_BUF_LEN = 2048;
  // TODO(yuwei.mu) : 将get的结果缓存在本地
  struct Stat stat;
  char *zk_buf = new char[ZK_BUF_LEN];
  zk_buf[ZK_BUF_LEN - 1] = '\0';
  int buflen = ZK_BUF_LEN - 1;

  int rc = 0;
  if (watch) {
    rc = zoo_wget(zhandle_, path, &ZookeeperClient::EventWatcher,
                  reinterpret_cast<void*>(this), zk_buf, &buflen, &stat);
  } else {
    rc = zoo_wget(zhandle_, path, NULL, NULL, zk_buf, &buflen, &stat);
  }
  if (rc) {
    MCE_WARN("ZookeeperClient::DoGetValue() zoo_wget() error. path="
        << path << " rc=" << rc << "," << zerror(rc));
  } else {
    zk_buf[buflen] = '\0';
    *value = zk_buf;
  }
  delete [] zk_buf;

  if (rc) {
    return ErrorRecoding(rc);
  }
  return 0;
}

int ZookeeperClient::Update(
     const char *path, const std::string &value, int version) {
  if (!zhandle_) {
    int rc = Init();
    if (rc) {
      return rc;
    }
  }

  int rc = zoo_set(zhandle_, path, value.c_str(), value.size(), version);
  if (rc != ZOK) {
    MCE_WARN("ZookeeperClient::Update() zoo_set() error. path="
        << path << " rc=" << rc << "," << zerror(rc));
    return ErrorRecoding(rc);
  }
  return 0;
}

int ZookeeperClient::Create(const char *path, const std::string &data,
                            bool is_tmp) {
  if (!zhandle_) {
    int rc = Init();
    if (rc) {
      return rc;
    }
  }

  char res_path[128];
  int rc = zoo_create(zhandle_, path, data.c_str(), data.size(),
           &ZOO_OPEN_ACL_UNSAFE, is_tmp ? ZOO_EPHEMERAL : 0, res_path, 128);
  if (rc) {
    MCE_WARN("ZookeeperClient::Create() zoo_create() error. path="
        << path << " rc=" << rc << "," << zerror(rc));
    return ErrorRecoding(rc);
  }
  if (is_tmp == true) {
  /*若是临时节点记录下来*/
    if (temporary_path_.find(std::string(path)) == temporary_path_.end()) {
      {
        boost::mutex::scoped_lock(temporary_path_mutex_);
        temporary_path_.insert(std::pair<std::string, std::string>
                                   (std::string(path), data));
      }
    }
  }

  return 0;
}

int ZookeeperClient::Delete(const char *path, bool recursive, int version) {
  if (!zhandle_) {
    int rc = Init();
    if (rc) {
      return rc;
    }
  }
  if (recursive == false) {
    int rc = zoo_delete(zhandle_, path, version);
    if (rc) {
      MCE_WARN("ZookeeperClient::Delete() zoo_delete() error. path="
          << path << " rc=" << rc << "," << zerror(rc));
      return ErrorRecoding(rc);
    }
  } else {
    std::vector<std::string> children;
    int has_child = DoGetChildren(path, &children, false);
    if (has_child) {  // DoGetChildren()执行失败
      MCE_WARN("ZookeeperClient::Delete() DoGetChildren() error. path="
               << path);
      return has_child;
    }
    std::vector<std::string>::iterator it = children.begin();
    while (it != children.end()) {
      std::string child = std::string(path) + "/" + (*it);
      int rc = Delete(child.c_str(), true, version);
      if (rc) {   // 删除子节点失败
        return rc;
      }
      it++;
    }

    int rc = zoo_delete(zhandle_, path, version);
    if (rc) {
      MCE_WARN("ZookeeperClient::Delete() zoo_delete() error. path="
          << path << " rc=" << rc << "," << zerror(rc));
      return ErrorRecoding(rc);
    }
  }
  return 0;
}

int ZookeeperClient::GetAcl(const char *path, struct ACL_vector *acl) {
  if (!zhandle_) {
    int rc = Init();
    if (rc) {
      return rc;
    }
  }
  struct Stat stat;
  int rc = zoo_get_acl(zhandle_, path, acl, &stat);
  if (rc) {
    MCE_WARN("ZookeeperClient::GetAcl() zoo_get_acl() error. path="
        << path <<" rc=" << rc << "," << zerror(rc));
    return ErrorRecoding(rc);
  }
  return 0;
}

int ZookeeperClient::SetAcl(const char *path, const struct ACL_vector &acl,
                            bool recursive, int version) {
  if (!zhandle_) {
    int rc = Init();
    if (rc) {
      return rc;
    }
  }
  int rc;
  if (recursive == false) {
    rc = zoo_set_acl(zhandle_, path, version, &acl);
    if (rc) {
      MCE_WARN("ZookeeperClient::SetAcl() zoo_set_acl() error. path="
          << path << " rc" << rc << "," << zerror(rc));
      return ErrorRecoding(rc);
    }
  } else {
    std::vector<std::string> children;
    int has_child = DoGetChildren(path, &children, false);
    if (has_child) {  // DoGetChildren()执行失败
      MCE_WARN("ZookeeperClient::SetAcl() DoGetChildren() error. path="
               << path);
      return has_child;
    }
    std::vector<std::string>::iterator it = children.begin();
    while (it != children.end()) {
      std::string child = std::string(path) + "/" + (*it);
      rc = SetAcl(child.c_str(), acl, true, version);
      if (rc) {
        return rc;
      }
      it++;
    }
    rc = zoo_set_acl(zhandle_, path, version, &acl);
    if (rc) {
      MCE_WARN("ZookeeperClient::SetAcl() zoo_set_acl() error. path="
          << path << " rc=" << rc << "," << zerror(rc));
      return ErrorRecoding(rc);
    }
  }
  return 0;
}
}
