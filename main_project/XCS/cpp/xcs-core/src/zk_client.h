/*
  Copyright 2012-08-28 yuwei.mu@renren-inc.com
                       shiqiang.ding@renren-inc.com
  
  This file is part of XCS.

  对zookeeper C的API进行了基本的封装。
  在expired重连之后，重新添加权限，创建临时节点，增加监听。
  而且还对普通的监听操作进行重新注册。
*/

#ifndef XCS_SRC_ZK_CLIENT_H_
#define XCS_SRC_ZK_CLIENT_H_

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#include <zookeeper.h>

#include <map>
#include <string>
#include <vector>
#include <set>

#include "zk_event_listener.h"
#include "zk_acl_provider.h"
#include "properties_configuration.h"

namespace xcs {

struct AuthInfo {
 public:
  std::string name_;
  std::string passwd_;
  AuthInfo(const std::string &name, const std::string &passwd)
      : name_(name), passwd_(passwd) {}
};

class ZookeeperClient {
 public:
  static ZookeeperClient &Instance(const PropertiesConfiguration &config,
                                   int session_timeout,
                                   std::string role = "") {
    if (role == "client") {
      instance_client.SetConfiguration(config, session_timeout);
      return instance_client;
    } else if (role == "server") {
      instance_server.SetConfiguration(config, session_timeout);
      return instance_server;
    } else {
      instance.SetConfiguration(config, session_timeout);
      return instance;
    }
  }

  ~ZookeeperClient() {
    if (zhandle_) {
      zookeeper_close(zhandle_);
    }
  }

  int Create(const char *path, const std::string &value, bool is_tmp);
  int Delete(const char *path, bool recursive = false, int version = -1);
  int Update(const char *path, const std::string &value, int version = -1);

  int GetAcl(const char *path, struct ACL_vector *acl);
  int SetAcl(const char *path, const struct ACL_vector &acl,
             bool recursive = false, int version = -1);

  int GetValue(const char *path, std::string *value) {
    return DoGetValue(path, value, false);
  }
  int GetChildren(const char *path, std::vector<std::string> *children) {
    return DoGetChildren(path, children, false);
  }

  int AddNodeListener(const char *path, ZkEventListenerPtr listenter);
  int AddChildListener(const char *path, ZkEventListenerPtr listenter);
  int AddExpiredListener(const char *path, ZkEventListenerPtr listenter);

  int AddAuthInfo(const std::string &name, const std::string &passwd);

  void RemoveTempPath(const std::string &path);

 private:
  zhandle_t *zhandle_;
  std::string address_root_;
  int session_timeout_;

  static ZookeeperClient instance_client;
  static ZookeeperClient instance_server;
  static ZookeeperClient instance;

  static void EventWatcher(zhandle_t *zzh, int type,
                           int state, const char *path, void *watcher_ctx);
  // InitWatcher仅用于zookeeper_init连接建立的第一次监听
  static void InitWatcher(zhandle_t *zzh, int type,
                          int state, const char *path, void *watcher_ctx);
/**
    对zookeeper返回的错误类型重新编码:
    0. 操作成功
    -1. 没有权限
    -2. zookeeper服务器关掉或者网络波动导致的connectloss
    -3. 连接zookeeper失败或者zookeeper服务器地址不对（初次连接）
    -4. 节点已经存在
    -5. 路径不存在
    -6. 节点下面存在子节点不能删除
    -7. 其他zookeeper使用错误

    -9. 接口使用不当,例如传入的路径以/结尾或者//连用情况等等。
  */
  static int ErrorRecoding(int zk_code);

  int Init();

  int DoGetValue(const char *path, std::string *value, bool watch = true);
  int DoGetChildren(const char *path, std::vector<std::string> *children,
                    bool watch = true);

  void RemoveNodeListeners(const char *path);
  void TriggerNodeListeners(const char *path, const std::string &data);

  void RemoveChildListeners(const char *path);
  void TriggerChildListeners(const char *path,
                             const std::vector<std::string> &children);

  void RemoveExpiredListeners(const char *path);
  void TriggerExpiredListeners();

  void OnSessionEvent(const char *path, int state);  // 注意这里的path为空
  void OnNodeEvent(const char *path, int state);
  void OnChildEvent(const char *path, int state);

  std::map<std::string, std::set<ZkEventListenerPtr> > node_listeners_;
  boost::mutex node_listener_mutex_;

  std::map<std::string, std::set<ZkEventListenerPtr> > child_listeners_;
  boost::mutex child_listener_mutex_;

  std::map<std::string, std::set<ZkEventListenerPtr> > expired_listeners_;
  boost::mutex expired_listeners_mutex_;

  std::map<std::string, std::string> temporary_path_;
  boost::mutex temporary_path_mutex_;

  std::vector<AuthInfo> auth_info_;
  boost::mutex auth_info_mutex_;

  boost::mutex singleton_mutex_;

  // 考虑多线程安全，singleton多线程需要double-check
  void SetConfiguration(const PropertiesConfiguration &config,
                        int session_timeout) {
    if (address_root_ != "")
      return;

    std::string tmp;
    if (config.address() == "") {
      tmp = "";
    } else if (config.root() == "" || config.root() == "/") {
      tmp = config.address();
    } else {
      tmp = config.address() + "/" + config.root();
    }

    if (session_timeout < 200) {
      session_timeout = 200;
    }
    {
      boost::mutex::scoped_lock(singleton_mutex_);
      if (address_root_ == "") {  // double-check
        address_root_ = tmp;
        session_timeout_ = session_timeout;
        if (config.username() != "") {
          AddAuthInfo(config.username(), config.password());
        }
      }
    }
  }

  ZookeeperClient() : zhandle_(NULL) {}
};
}

#endif  // XCS_SRC_ZK_CLIENT_H__
