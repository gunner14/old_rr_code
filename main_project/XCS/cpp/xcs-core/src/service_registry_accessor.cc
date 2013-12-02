/*
  Copyright 2012-09-06 shiqiang.ding@renren-inc.con
                       dinggangyilang@gmail.com

  This file is part of XCS.

  This code is used for service-consumers to find the service they want.
  Note that one ServiceRegistryAccessor' instance corresponds to only one
  specific version of a service.

  帮助XCS的client角色获取服务
  注意：一个ServiceRegistryAccessor实例只能对应的获取一个服务版本
        若是想获取多个服务版本请创建多个实例

*/

#include "service_registry_accessor.h"

#include <string>
#include <vector>

namespace xcs {
int ServiceRegistryAccessor::Connect(const std::string &service,
                                     const std::string &version) {
  if (service == "" || version == "") {
    MCE_WARN("ServiceRegistryAccessor::Connect() error: "
        << "输入参数不能为空");
    return -9;
  }
  service_ = service;
  version_ = version;
  std::string path = std::string("/") + service_ + "/" + version_;
  int rc = zk_client_.GetValue(path.c_str(), &quota_);
  return rc;
}

int ServiceRegistryAccessor::AddListenExpired(
    const ServiceEventListenerPtr &listener) {
  if (listener == NULL) {
    MCE_WARN("ServiceRegistryAccessor::AddListenExpired() error: "
        << "监听器不能为空");
    return -9;
  }
  std::string path = std::string("/") + service_ + "/" + version_;
  int rc = zk_client_.AddExpiredListener(path.c_str(), listener);
  return rc;
}

int ServiceRegistryAccessor::ListAndListen(const std::string &stat,
                                    const ServiceEventListenerPtr &listener,
                                    const bool &is_recursive,
                                    std::vector<std::string> *children) {
  int rc;
  std::string path = std::string("/") + service_ + "/" + version_;
  children->clear();
  if (stat == "") {   // version层子节点（考虑是否递归）
    if (is_recursive) {   // 递归
      std::vector<std::string> stat_list;
      rc = zk_client_.GetChildren(path.c_str(), &stat_list);
      if (rc) {
        MCE_WARN("ServiceRegistryAccessor::ListAndListen() error: "
            << "获取version层子节点失败:" << path);
        return rc;
      }
      std::vector<std::string>::iterator it = stat_list.begin();
      while (it != stat_list.end()) {
        std::string stat_path = path + "/" + (*it);
        children->push_back(*it);
        std::vector<std::string> node_list;
        rc = zk_client_.GetChildren(stat_path.c_str(), &node_list);
        if (rc) {
          MCE_WARN("ServiceRegistryAccessor::ListAndListen() error: "
              << "获取state层子节点失败:" << stat_path);
          return rc;
        }
        std::vector<std::string>::iterator i = node_list.begin();
        while (i != node_list.end()) {
          children->push_back((*it) + "/" + (*i));
          i++;
        }
        it++;
      }
      if (listener != NULL) {
        rc = zk_client_.AddChildListener(path.c_str(), listener);
        if (rc) {
          MCE_WARN("ServiceRegistryAccessor::ListAndListen() error: "
              << "version层添加监听失败:" << path);
          return rc;
        }
        it = stat_list.begin();
        while (it != stat_list.end()) {
          std::string stat_path = path + "/" + (*it);
          rc = zk_client_.AddChildListener(stat_path.c_str(), listener);
          if (rc) {
            MCE_WARN("ServiceRegistryAccessor::ListAndListen() error: "
                << "state层添加监听失败:" << stat_path);
            return rc;
          }
          it++;
        }
      }
    } else {    // 非递归
      rc = zk_client_.GetChildren(path.c_str(), children);
      if (rc) {
        MCE_WARN("ServiceRegistryAccessor::ListAndListen() error: "
            << "获取version层子节点失败:" << path);
        return rc;
      }
      if (listener != NULL) {
        rc = zk_client_.AddChildListener(path.c_str(), listener);
        if (rc) {
          MCE_WARN("ServiceRegistryAccessor::ListAndListen() error: "
              << "version层添加监听失败:" << path);
          return rc;
        }
      }
    }
  } else {    // stat层子节点
    path = path + "/" + stat;
    rc = zk_client_.GetChildren(path.c_str(), children);
    if (rc) {
      MCE_WARN("ServiceRegistryAccessor::ListAndListen() error: "
          << "获取state层子节点失败:" << path);
      return rc;
    }
    if (listener != NULL) {
      rc = zk_client_.AddChildListener(path.c_str(), listener);
      if (rc) {
        MCE_WARN("ServiceRegistryAccessor::ListAndListen() error: "
            << "state层添加监听失败:" << path);
        return rc;
      }
    }
  }
  return 0;
}

int ServiceRegistryAccessor::ContentListen(const std::string &stat,
                                     const ServiceEventListenerPtr &listener) {
  if (stat == "" || listener == NULL) {
    MCE_WARN("ServiceRegistryAccessor::ContentListen() error: "
        << "输入参数不能为空");
    return -9;
  }
  std::string path = std::string("/") + service_ + "/" + version_ + "/" + stat;
  std::vector<std::string> children;
  int rc = zk_client_.GetChildren(path.c_str(), &children);
  if (rc) {
    MCE_WARN("ServiceRegistryAccessor::ContentListen() error: "
        << "获取state层子节点失败:" << path);
    return rc;
  }
  std::vector<std::string>::iterator it = children.begin();
  while (it != children.end()) {
    std::string endpoint = path + "/" + (*it);
    rc = zk_client_.AddNodeListener(endpoint.c_str(), listener);
    if (rc) {
      MCE_WARN("ServiceRegistryAccessor::ContentListen() error: "
          << "服务器层添加内容监听失败:" << endpoint);
      return rc;
    }
    it++;
  }
  return 0;
}

int ServiceRegistryAccessor::GetServiceStatus(const std::string &stat,
                                              const std::string &endpoint,
                                              std::string *content) {
  std::string path = std::string("/") + service_ + "/" + version_ + "/" + stat
                                                 + "/" + endpoint;
  int rc = zk_client_.GetValue(path.c_str(), content);
  if (rc) {
    MCE_WARN("ServiceRegistryAccessor::GetServiceStatus() error: "
        << "获取服务器状态失败:" << endpoint);
    return rc;
  }
  return 0;
}
}
