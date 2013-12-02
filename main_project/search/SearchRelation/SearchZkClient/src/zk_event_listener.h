#ifndef _XOA_CLIENT_ZK_EVENT_LISTENER_H_
#define _XOA_CLIENT_ZK_EVENT_LISTENER_H_

#include <vector>

#include <boost/enable_shared_from_this.hpp>
namespace search{
namespace relation{

class ZkEventListener : public boost::enable_shared_from_this<ZkEventListener> {
//class ZkEventListener {
 public:
  // 当且仅当节点数据变化时触发该事件(本节点删除，不会调用该方法)
  virtual void HandleNodeEvent(const char * path, const std::string & value) = 0;

  // 当且仅当孩子节点有增删时触发该事件(本节点删除，不会调用该方法)
  virtual void HandleChildEvent(const char * path, const std::vector<std::string> & children) = 0;

  virtual ~ZkEventListener() {}
};

// 使用weak_ptr, 不增加额外引用，不影响listener对象的生存期
//typedef boost::weak_ptr<ZkEventListener> ZkEventListenerPtr;
typedef boost::shared_ptr<ZkEventListener> ZkEventListenerPtr;

}
}

#endif // _XOA_CLIENT_ZK_EVENT_LISTENER_H_
