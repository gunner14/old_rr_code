/*
  Copyright 2012-09-05 shiqiang.ding@renren-inc.com
                       dinggangyilang@gmail.com

  This file is part of XCS.

  用户实现的监听类提供两个的接口，用户可以实现对特定节点特定事件的处理
  注意：1. 一个监听类实例只能绑定一个ServiceRegistryAccessor实例
        2. 监听类的生命周期
*/

#ifndef XCS_SRC_SERVICE_EVENT_LISTENER_H_
#define XCS_SRC_SERVICE_EVENT_LISTENER_H_

#include <vector>
#include <string>

#include "zk_event_listener.h"
#include "UtilCxx/src/LogWrapper.h"

namespace xcs {
class ServiceRegistryAccessor;
typedef boost::shared_ptr<ServiceRegistryAccessor> ServiceRegistryAccessorPtr;

class ServiceEventListener : public ZkEventListener {
 public:
  explicit ServiceEventListener(const ServiceRegistryAccessorPtr &accessor)
      : accessor_(accessor) {
  }

  virtual ~ServiceEventListener() {}

  /* 用户需要实现的三个个函数 */
/**
  * version 当前服务版本号，一定不为空
  * 当stat为空("")时，version层子节点变化
  * 当stat不为空时，相应的stat层子节点变化
  */
  virtual void HandleChildChange(const std::string &version,
                                 const std::string &stat) = 0;
/**
  * version  当前版本号
  * stat     服务分区名
  * endpoint 服务地址（ip:port）
  * 以上均不为空，endpoint节点的内容发生变化
  */
  virtual void HandleContentChange(const std::string &version,
                                   const std::string &stat,
                                   const std::string &endpoint) = 0;
/**
  * 断网重连成功后采取的操作，一般是初始化操作
  */
  virtual void HandleExpiredEvent() = 0;

/**
  * 用户可以通过调用shared_from_this()得到指向当前实例的智能指针
  */
  boost::shared_ptr<ServiceEventListener> shared_from_this() {
    return boost::dynamic_pointer_cast<ServiceEventListener>
           (ZkEventListener::shared_from_this());
  }

  virtual void HandleNodeEvent(const char *path, const std::string &value);
  virtual void HandleChildEvent(const char *path,
                                const std::vector<std::string> &children);

 protected:
  ServiceRegistryAccessorPtr accessor_;

 private:
  void ParserPath(const char *path, std::string *version,
                  std::string *stat, std::string *endpoint);
};

typedef boost::shared_ptr<ServiceEventListener> ServiceEventListenerPtr;
}

#endif  // XCS_SRC_SERVICE_EVENT_LISTENER_H_
