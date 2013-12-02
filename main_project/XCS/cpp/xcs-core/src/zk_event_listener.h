/*
  Copyright 2012-8-28 yuwei.mu@renren-inc.com
                      shiqiang.ding@renren-inc.com

  This file is part of XCS.

  监听类：用来实现对zk事件的监听
*/

#ifndef XCS_SRC_ZK_EVENT_LISTENER_H_
#define XCS_SRC_ZK_EVENT_LISTENER_H_

#include <boost/enable_shared_from_this.hpp>
#include <string>
#include <vector>

namespace xcs {

class ZkEventListener : public boost::enable_shared_from_this<ZkEventListener> {
 public:
  // 当且仅当节点数据变化时触发该事件(本节点删除，不会调用该方法)
  virtual void HandleNodeEvent(const char *path, const std::string &value) = 0;

  // 当且仅当孩子节点有增删时触发该事件(本节点删除，不会调用该方法)
  virtual void HandleChildEvent(const char *path,
                                const std::vector<std::string> &children) = 0;

  // 当且仅当连接出现expired时会触发该事件
  virtual void HandleExpiredEvent() = 0;

  virtual ~ZkEventListener() {}
};

// 使用weak_ptr, 不增加额外引用，不影响listener对象的生存期
typedef boost::weak_ptr<ZkEventListener> ZkEventListenerPtr;
}

#endif  // XCS_SRC_ZK_EVENT_LISTENER_H_
