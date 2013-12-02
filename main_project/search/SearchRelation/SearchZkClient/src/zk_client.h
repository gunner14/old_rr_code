#ifndef _XOA2_CLIENT_ZK_CLIENT_H_
#define _XOA2_CLIENT_ZK_CLIENT_H_

#include <zookeeper.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <set>

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#include "./zk_event_listener.h"

namespace search{
namespace relation{
using namespace std;

class ZookeeperClient {
 public:
  static ZookeeperClient & Instance() {
    static ZookeeperClient instance;
    return instance;
  }

  ~ZookeeperClient() {
    if (zhandle_) {
      zookeeper_close(zhandle_);
    }
  }
  void SetZk(const string & address){
 	cfg_line = address; 
  }

  bool Create(const char * path, const std::string & value, bool is_tmp);
  bool Delete(const char * path);
  bool Update(const char * path, const std::string & value);

  bool GetValue(const char * path, std::string * value) {
    return DoGetValue(path, value, false);
  }
  bool GetChildren(const char * path, std::vector<std::string> * children) {
    return DoGetChildren(path, children, false);
  }

  bool AddNodeListener(const char * path, ZkEventListenerPtr listenter);
  bool AddChildListener(const char * path, ZkEventListenerPtr listenter);

 private:
  zhandle_t * zhandle_;
  string cfg_line;

  static void EventWatcher(zhandle_t *zzh, int type, int state, 
      const char *path, void *watcher_ctx);
  bool Init();

  bool DoGetValue(const char * path, std::string * value, bool watch = true);
  bool DoGetChildren(const char * path, std::vector<std::string> * children, bool watch = true);

  void RemoveNodeListeners(const char * path);
  void TriggerNodeListeners(const char * path, const std::string & data);

  void RemoveChildListeners(const char * path);
  void TriggerChildListeners(const char * path, const std::vector<std::string> & children);

  void OnSessionEvent(const char * path, int state);
  void OnNodeEvent(const char * path, int state);
  void OnChildEvent(const char * path, int state);

  std::map<std::string, std::set<ZkEventListenerPtr> > node_listeners_;
  boost::mutex node_listener_mutex_;

  std::map<std::string, std::set<ZkEventListenerPtr> > child_listeners_;
  boost::mutex child_listener_mutex_;
  ZookeeperClient() : zhandle_(NULL) {}
};

}
}

#endif // _XOA2_CLIENT_ZK_CLIENT_H_

