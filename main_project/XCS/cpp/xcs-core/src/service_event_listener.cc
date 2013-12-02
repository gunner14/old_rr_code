/*
 Copyright 2012-09-05 shiqiang.ding@renren-inc.com
                      dinggangyilang@gmail.com

 This file is part of XCS.

 用户实现的监听类提供两个的接口，用户可以实现对特定节点特定事件的处理
 注意：1. 一个监听类实例只能绑定一个ServiceRegistryAccessor实例
       2. 监听类的生命周期
*/

#include "service_event_listener.h"
#include <boost/algorithm/string.hpp>
#include <vector>

namespace xcs {
void ServiceEventListener::ParserPath(const char *path,
                                      std::string *version,
                                      std::string *stat,
                                      std::string *endpoint) {
  std::vector<std::string> split_path;
  std::string str_path = std::string(path);
  boost::split(split_path, str_path, boost::is_any_of("/"));
  *version = split_path[2];
  if (split_path.size() <= 3) {
    *stat = "";
    *endpoint = "";
  } else if (split_path.size() <= 4) {
    *stat = split_path[3];
    *endpoint = "";
  } else if (split_path.size() <= 5) {
    *stat = split_path[3];
    *endpoint = split_path[4];
  }
}

void ServiceEventListener::HandleNodeEvent(const char *path,
                                           const std::string &value) {
  std::string version, stat, endpoint;
  ParserPath(path, &version, &stat, &endpoint);
  HandleContentChange(version, stat, endpoint);
}

void ServiceEventListener::HandleChildEvent(const char *path,
                                            const std::vector<std::string> &) {
  std::string version, stat, endpoint;
  ParserPath(path, &version, &stat, &endpoint);
  HandleChildChange(version, stat);
}
}
