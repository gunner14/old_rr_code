#ifndef XCE_TRIPOD_ZKCLIENT_H_
#define XCE_TRIPOD_ZKCLIENT_H_

#include <map>
#include <string>

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#include <zookeeper/zookeeper.h>

namespace xce {
namespace tripod {

static const std::string kDefaultZkAddress = "UserZooKeeper1:2181,UserZooKeeper2:2181,UserZooKeeper3:2181,UserZooKeeper4:2181,UserZooKeeper5:2181";
static const std::string kDefaultZNodePrefix = "/tripod2";
static const std::string kZkSeparator = "/";
static const std::string kDefaultZkCacheZNode = kDefaultZNodePrefix + kZkSeparator + "cache";
static const std::string kDefaultZkCacheManagerZNode= kDefaultZNodePrefix + kZkSeparator + "cachemanager";
static const std::string kDefaultZkProducerManagerZNode = kDefaultZNodePrefix + kZkSeparator + "producermanager";

class ZNodeListener {
 public:
  explicit ZNodeListener(const std::string&);
  virtual bool Update(const std::list<std::string>& children_names) = 0;
  virtual ~ZNodeListener();

  std::string znode() const {
    return znode_;
  }
 private:
  std::string znode_;
};

typedef boost::shared_ptr<ZNodeListener> ZNodeListenerPtr;

class ZkClient;
typedef boost::shared_ptr<ZkClient> ZkClientPtr;

class ZkClient {
 public:
  virtual ~ZkClient();
  void AddMessage(const std::string&);
  void NotifyInitialize();
  bool Update(const std::string&);
  bool GetNodes(const std::string& znode_path, std::list<std::string>* );

  void AddZNodeListener(ZNodeListener* listener);
  void RemoveZNodeListener(ZNodeListener* listener);

  bool reinit_flag() const {
   return reinit_flag_;
  }
  void set_reinit_flag(bool f) {
    reinit_flag_ = f;
  }
 public:
  static const std::string kUpdateAllZNodes;
  static const std::string kDefaultPrefix;
  static std::map<std::string, ZkClientPtr> address_zk_client_map_;
  static std::map<zhandle_t*, ZkClientPtr> zhandle_zk_client_map_;
  static boost::mutex zk_client_map_mutex_;

  static ZkClientPtr GetInstance(const std::string& zk_address);
 private:
  explicit ZkClient(const std::string& zk_address);
  bool Initialize();
  bool Reinitialize();

  void NotifyThreadFunction();
  bool UpdateAllZNodes();
  bool UpdateZNode(const std::string&);

  bool GetZNodeVersion(const std::string&, std::string*);

  bool GetDataFromZk(const std::string&, std::string*);
  
  bool GetChildrenNamesFromZk(const std::string&, std::list<std::string>*);

  static void ZkWatcher(zhandle_t *zzh, int type, int state, const char *path, void *watcher_ctx);
 private:
  boost::shared_mutex znode_listener_mutex_;
  std::map<std::string, std::list<ZNodeListener*> > listeners_;

  boost::mutex znode_version_mutex_;
  std::map<std::string, std::string> znode_version_map_;

  bool reinit_flag_;

  zhandle_t* zk_;
  boost::mutex zk_mutex_;
  boost::condition_variable zk_cond_;
  std::string zk_address_;

  boost::mutex message_deque_mutex_;
  boost::shared_ptr<std::deque<std::string> > message_deque_;
  boost::condition_variable message_deque_cond_;
  boost::thread notify_thread_;
};



}} // xce::tripod

#endif

