/* 
  Copyright 2012-09-03 shiqiang.ding@renren-inc.com
                       dinggangyilang@gmail.com
  
  This file is part of XCS.

  This code is used for service-consumers to find the service they want.
  Note that one ServiceRegistryAccessor' instance corresponds to only one
  specific version of a service.

  帮助XCS的client角色获取服务
  注意：一个ServiceRegistryAccessor实例只能对应的获取一个服务版本
        若是想获取多个服务版本请创建多个实例
*/

#ifndef XCS_SRC_SERVICE_REGISTRY_ACCESSOR_H_
#define XCS_SRC_SERVICE_REGISTRY_ACCESSOR_H_

#include <vector>
#include <string>

#include "zk_client.h"
#include "service_event_listener.h"
#include "UtilCxx/src/LogWrapper.h"

namespace xcs {
/*
    提供给XCS client角色的操作返回的操作码详解：
    0. 操作成功
    -1. 没有权限
    -2. XCS服务器关掉或者网络波动导致的connectloss
    -3. 连接XCS服务器失败或者XCS服务器地址不对

    -5. 服务路径不存在
    -7. XCS内部其他错误

    -9. 接口使用不当,包含我们程序接口使用不当和XCS client使用XCS core接口不当。
*/


class ServiceRegistryAccessor {
 public:
  ServiceRegistryAccessor(const PropertiesConfiguration &config,
                          int session_timeout)
      : zk_client_(ZookeeperClient::Instance(config, session_timeout,
                                             "client")) {
    client_id_ = config.username();
  }

/**
  * 初始化ServiceRegistryAccessor
  * 获取version下配额（quota）
  * 注意：此函数只能调用一次
  *
  * @param service 服务的唯一名称
  * @param version 特定的版本号，如101,注意不支持扩展格式
  * return         操作状态码，0为成功
  */
  int Connect(const std::string &service, const std::string &version);

/**
  * 添加会话过期监听，针对于服务断网之后需要重新初始化，举个简单例子：
  * ICE服务中client角色需要获取服务的proxy，当发生断网重连时，我们所用的服务
  * 挂掉了，这时服务挂掉不会触发ChildChangeEvent，因为现在处在expired状态,
  * 当网络重新可以连接时，每个server服务重新注册上服务，我们也重新注册好监听，
  * 问题来了，我们之前缓存下来的proxy对应的服务现在挂掉了，我们也没有得到
  * ChildChangeEvent的监听。通过expired状态的监听，我们可以重新初始化proxy。
  * 
  * @param listener 监听类，不能为空（NULL）
  * return          操作状态码，0为成功
  */
  int AddListenExpired(const ServiceEventListenerPtr &listener);

/**
  * 递归或者非递归的获取version层或者stat层的子节点列表，可以选择是否添加监听
  * 当选择递归的获得version层的子节点列表时，传出的参数保存为stat/node的vector
  * 
  * @param stat     服务状态分区名，若是为空(""),则获取version层下面的子节点列表
  *                 否则获取特定stat下的子节点列表
  * @param listener 监听类，若为空（NULL），则不进行监听,传入空的listener方式：
  *                 ServiceEventListenerPtr((ServiceEventListener *)NULL)
  * @param is_recursive  是否进行递归，true为递归监听
  * @param[out] children 输出参数，保存子节点列表
  * return               操作状态码，0为成功
  */
  int ListAndListen(const std::string &stat,
                    const ServiceEventListenerPtr &listener,
                    const bool &is_recursive,
                    std::vector<std::string> *children);

/**
  * 给某个stat下面的所有子节点的内容变化加监听
  *
  * @param stat     服务状态分区名，不能为空（"")
  * @param listener 监听类，不能为空（NULL)
  * return          操作状态码，0为成功
  */
  int ContentListen(const std::string &stat,
                    const ServiceEventListenerPtr &listener);

/**
  * 获取某一个服务状态
  *
  * @param stat     服务状态分区名，不能为空("")
  * @param endpoint 服务地址(ip:port)，不能为空("")
  * @param[out] content 输出参数，存储服务状态信息
  * return              操作状态码，0为成功
  */
  int GetServiceStatus(const std::string &stat,
                       const std::string &endpoint,
                       std::string *content);
  // 获取service
  std::string service() {
    return service_;
  }

  // 获取version
  std::string version() {
    return version_;
  }

 private:
  std::string service_;
  std::string version_;
  std::string client_id_;

  // TODO(shiqiang.ding)
  std::string quota_;   // quota变量，配额限制

  ZookeeperClient &zk_client_;
};
typedef boost::shared_ptr<ServiceRegistryAccessor> ServiceRegistryAccessorPtr;
}

#endif  // XCS_SRC_SERVICE_REGISTRY_ACCESSOR_H_
