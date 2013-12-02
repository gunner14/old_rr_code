/*
  Copyright 2012-09-11 shiqiang.ding@renren-inc.com
                       dinggangyilang@gmail.com

  This file is part of XCS.

  主要为XCS提供以下配置：
  zk服务器集群地址，root路径，username，password
  给定配置文件路径，读取配置文件生成以上配置
*/
#ifndef XCS_SRC_PROPERTIES_CONFIGURATION_H_
#define XCS_SRC_PROPERTIES_CONFIGURATION_H_

#include <string>

namespace xcs {
class PropertiesConfiguration {
 public:
  explicit PropertiesConfiguration(std::string file_path);

  std::string address() const {
    return address_;
  }

  std::string root() const {
    return root_;
  }

  std::string username() const {
    return username_;
  }

  std::string password() const {
    return password_;
  }

 private:
  std::string address_;
  std::string root_;
  std::string username_;
  std::string password_;

  static std::string Trim(std::string str);
};
}
#endif  // XCS_SRC_PROPERTIES_CONFIGURATION_H_
