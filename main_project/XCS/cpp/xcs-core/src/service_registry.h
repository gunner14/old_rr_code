/*
  Copyright (c) 2012-09-05 shiqiang.ding@renren-inc.com
                           dinggangyilang@gmail.com

  This file is part of XCS.

  This code is used for service-publisher to register a service in order that
  service-consumer can find it.
  Note that one ServiceRegistry' instance can register only one service.

  帮助XCS的server角色发布服务
  注意: 一个ServiceRegistry实例只能注册一个服务
        若是想注册多个服务请创建多个实例
*/

#ifndef XCS_SRC_SERVICE_REGISTRY_H_
#define XCS_SRC_SERVICE_REGISTRY_H_

#include <string>
#include <vector>

#include "zk_client.h"
#include "UtilCxx/src/LogWrapper.h"

namespace xcs {
struct ServiceNode {
  std::string name_;     // 服务的地址(ip:port)
  std::string content_;  // 服务节点存储的value
};

/*
    提供给XCS server角色的操作返回的操作码详解：
    0. 操作成功
    -1. 没有权限
    -2. XCS服务器关掉或者网络波动导致的connectloss
    -3. 连接XCS服务器失败或者XCS服务器地址不对
    -4. 服务已经存在
    -5. 路径不存在
    -7. XCS内部其他错误

    -9. 接口使用不当,包含我们程序接口使用不当和XCS server使用XCS core接口不当。
*/

class ServiceRegistry : public ZkEventListener {
 public:
  ServiceRegistry(const PropertiesConfiguration &config, int session_timeout)
      : zk_client_(ZookeeperClient::Instance(config, session_timeout,
                                             "server")) {
  }
  // 析构函数需要去除临时服务,由于XCS core是Singleton的不去除临时节点会出现问题
  ~ServiceRegistry();

/**
  * 发布服务操作
  * 注意：此函数只允许调用一次，再次发布服务需要重新实例化一个ServiceRegistry
  * 
  * @param service 服务的唯一名称
  * @param version 特定的版本号，如101，注意不支持扩展格式
  * @param stat    服务状态分区名
  * @param node    发布的服务地址及其服务状态（value值）
  * @param is_tmp  标示发布的服务是临时的还是永久的，默认是临时的
  * return         操作状态码，0为成功
  */
  int PublishService(const std::string &service,
                     const std::string &version,
                     const std::string &stat,
                     const ServiceNode &node,
                     bool is_tmp = true);

  virtual void HandleNodeEvent(const char *path, const std::string &value) {
  }
  virtual void HandleChildEvent(const char *path,
                                const std::vector<std::string> &children) {
  }
  virtual void HandleExpiredEvent() {
  }

 private:
  std::string service_;
  std::string version_;
  std::string stat_;
  ServiceNode node_;
  bool is_tmp_;

  ZookeeperClient &zk_client_;
};
typedef boost::shared_ptr<ServiceRegistry> ServiceRegistryPtr;
}

#endif  // XCS_SRC_SERVICE_REGISTRY_H_
