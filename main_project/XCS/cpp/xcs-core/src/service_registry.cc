/*
  Copyright 2012-08-28 shiqiang.ding@renren-inc.com
                       dinggangyilang@gmail.com

  This file is part of XCS.

  This code is used for service-publisher to register a service in order that
  service-consumer can find it.
  Note that one ServiceRegistry' instance can register only one service.
*/

#include "service_registry.h"

namespace xcs {
ServiceRegistry::~ServiceRegistry() {
  if (is_tmp_) {
    std::string path = std::string("/") + service_ + "/"
                       + version_ + "/" + stat_ + "/" + node_.name_;
    zk_client_.RemoveTempPath(path);
  }
}

int ServiceRegistry::PublishService(const std::string &service,
                                    const std::string &version,
                                    const std::string &stat,
                                    const ServiceNode &node,
                                    bool is_tmp) {
  if (service == "" || version == "" || stat == "" || node.name_ == "") {
  MCE_WARN("ServiceRegistry::PublishService() error: 输入参数不能为空");
  return -9;
  }

  service_ = service;
  version_ = version;
  stat_ = stat;
  node_ = node;
  is_tmp_ = is_tmp;
  std::string path = std::string("/") + service_ + "/"
                     + version_ + "/" + stat_ + "/" + node_.name_;
  int rc = zk_client_.Create(path.c_str(), node_.content_, is_tmp_);
  if (rc) {
    return rc;
  }
  path = std::string("/") + service_ + "/" + version_ + "/" + stat_;
  rc = zk_client_.AddExpiredListener(path.c_str(), shared_from_this());
  return rc;
}
}
