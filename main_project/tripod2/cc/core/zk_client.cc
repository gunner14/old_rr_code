#include "tripod2/cc/core/zk_client.h"

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include "base/port.h"

#include "LogWrapper.h"

namespace xce {
namespace tripod {

static const char* State2String(int state){
    switch(state){
        case 0:
            return "ZOO_CLOSED_STATE";
        case 1:
            return "ZOO_CONNECTING_STATE";
        case 2:
            return "ZOO_ASSOCIATING_STATE";
        case 3:
            return "ZOO_CONNECTED_STATE";
        case -112:
            return "ZOO_EXPIRED_SESSION_STATE";
        case -113:
            return "ZOO_AUTH_FAILED_STATE";
    }
    return "INVALID_STATE";
}

static const char* WatcherEvent2String(const int ev){
    switch(ev){
        case 0:
            return "ZOO_ERROR_EVENT";
        case 1:
            return "ZOO_CREATED_EVENT";
        case 2:
            return "ZOO_DELETED_EVENT";
        case 3:
            return "ZOO_CHANGED_EVENT";
        case 4:
            return "ZOO_CHILD_EVENT";
        case -1:
            return "ZOO_SESSION_EVENT";
        case -2:
            return "ZOO_NOTWATCHING_EVENT";
    }
    return "INVALID_EVENT";
}

ZNodeListener::ZNodeListener(const std::string& znode) : znode_(znode) {

}

ZNodeListener::~ZNodeListener() {
  
}


std::map<std::string, ZkClientPtr> ZkClient::address_zk_client_map_;
std::map<zhandle_t*, ZkClientPtr> ZkClient::zhandle_zk_client_map_;
boost::mutex ZkClient::zk_client_map_mutex_;

const std::string ZkClient::kUpdateAllZNodes = "HOLY_SHIT_UPDATE_ALL_ZNODES";
const std::string ZkClient::kDefaultPrefix = "";

ZkClient::ZkClient(const std::string& zk_address) : reinit_flag_(false), zk_address_(zk_address), zk_(NULL),
                       message_deque_(new std::deque<std::string>),
                       notify_thread_(boost::bind(&ZkClient::NotifyThreadFunction, this)) {

}

ZkClient::~ZkClient() {  
  MCE_INFO("ZkClient::~ZkClient");
  message_deque_.reset();
  message_deque_cond_.notify_all();
  notify_thread_.join();
  
  if (zk_) {
    zookeeper_close(zk_);
  }
}

void ZkClient::NotifyThreadFunction() {
  while (true) {
    if (!message_deque_) {
      MCE_DEBUG("deque_ is null, notify thread(" << boost::this_thread::get_id() << ") shuts down right now");
      break;
    }
    boost::unique_lock<boost::mutex> lock(message_deque_mutex_);
    if (message_deque_->size() == 0) {
      MCE_DEBUG("notify thread(" << boost::this_thread::get_id() << ") waits");
      message_deque_cond_.wait(lock);
    }
    else {
      std::string msg = message_deque_->front();
      message_deque_->pop_front();
      MCE_DEBUG("notify thread(" << boost::this_thread::get_id() << ") takes a message(" << msg << ")");
      bool ret = true;
      for (int i = 0; i < 3; ++i) {
        ret = Update(msg); 
        if (!ret) {
          boost::this_thread::sleep(boost::posix_time::milliseconds(100));
        }
        else {
          break;
        }
      }
      if (!ret) {
        //加入队列
        std::deque<std::string>::const_iterator iter = std::find(message_deque_->begin(), message_deque_->end(), msg);
        if (iter == message_deque_->end()) {
          message_deque_->push_back(msg);
        }
        message_deque_cond_.notify_all();
      }
    }
  }

}

void ZkClient::AddMessage(const std::string& msg) {
  boost::unique_lock<boost::mutex> lock(message_deque_mutex_);
  std::deque<std::string>::const_iterator iter = std::find(message_deque_->begin(), message_deque_->end(), msg);
  if (iter == message_deque_->end()) {
    message_deque_->push_back(msg);
  }
  message_deque_cond_.notify_all();
  MCE_DEBUG("ZkClient::AddMessage() msg:" << msg << "\tthread_id:" << boost::this_thread::get_id());
}

void ZkClient::AddZNodeListener(ZNodeListener* listener) {
  if (!listener) {
    return;
  }
  boost::unique_lock<boost::shared_mutex> write_lock(znode_listener_mutex_);
  std::string znode = listener->znode();
  std::map<std::string, std::list<ZNodeListener*> >::iterator iter = listeners_.find(znode);
  if (iter != listeners_.end()) {
    (iter->second).push_back(listener);
  }
  else {
    std::list<ZNodeListener*> list;
    list.push_back(listener);
    listeners_.insert(std::make_pair(znode, list));
  }
}

void ZkClient::RemoveZNodeListener(ZNodeListener* listener) {
  if (!listener) {
    return;
  }
  boost::unique_lock<boost::shared_mutex> write_lock(znode_listener_mutex_);
  std::string znode = listener->znode();
  std::map<std::string, std::list<ZNodeListener*> >::iterator iter = listeners_.find(znode);
  for (; iter != listeners_.end(); ++iter) {
    std::list<ZNodeListener*>::iterator sub_iter = (iter->second).begin();
    for (; sub_iter != (iter->second).end(); ++sub_iter) {
      if (listener == *sub_iter) {
        sub_iter = (iter->second).erase(sub_iter);
      }
    }
  }
}

bool ZkClient::Update(const std::string& znode_path) {
  MCE_INFO("ZkClient::Update znode_path:" << znode_path);
  if (znode_path == kUpdateAllZNodes) {
    return UpdateAllZNodes();
  }
  return UpdateZNode(znode_path);
}

bool ZkClient::GetNodes(const std::string& znode_path, std::list<std::string>* list) {
  if (!list) {
    return false;
  }
  return GetChildrenNamesFromZk(znode_path, list);
}

bool ZkClient::UpdateAllZNodes() {
  bool flag = true;
  boost::shared_lock<boost::shared_mutex> read_lock(znode_listener_mutex_);
  std::map<std::string, std::list<ZNodeListener*> >::const_iterator iter = listeners_.begin();
  for (; iter != listeners_.end(); ++iter) {
    std::string znode_path = iter->first;
    bool ret = Update(znode_path);
    if (!ret) {
      flag = false;
    }
  }
  return flag;
}

bool ZkClient::UpdateZNode(const std::string& znode_path) {
  {
    std::string new_version;
    GetZNodeVersion(znode_path, &new_version);
    boost::mutex::scoped_lock version_lock(znode_version_mutex_);
    std::map<std::string, std::string>::const_iterator iter = znode_version_map_.find(znode_path);
    if (iter == znode_version_map_.end()) {
      znode_version_map_.insert(std::make_pair(znode_path, new_version));
    }
    else {
      const std::string& old_version = iter->second;
      MCE_INFO("new_version is equal to old_version znode_path:" << znode_path << " old_v:" << old_version << " new_v:" << new_version);
      if (new_version == old_version) { // version没变，直接返回成功
        MCE_DEBUG("new_version is equal to old_version znode_path:" << znode_path);
        return true;
      }
      //znode_version_map_.insert(std::make_pair(znode_path, new_version));
      znode_version_map_[znode_path] = new_version;
    }
  }
  
  boost::shared_lock<boost::shared_mutex> read_lock(znode_listener_mutex_);
  bool ret = true;

  std::map<std::string, std::list<ZNodeListener*> >::const_iterator c_iter = listeners_.find(znode_path);
  if (c_iter == listeners_.end()) {
    return ret;
  }
  //取ns下面的结点数据
  std::list<std::string> children_names;
  if (!GetChildrenNamesFromZk(znode_path, &children_names)) {
    MCE_ERROR("ZkClient::UpdateZNode() call GetChildrenNamesFromZk return false, znode_path:" << znode_path);
    return false;
  }
  if (children_names.empty()) {
    MCE_DEBUG("ZkClient::UpdateZNode() call GetChildrenNamesFromZk return empty list, znode_path:" << znode_path);
    return true;
  }
  //更新
  MCE_DEBUG("children_names.size():" << children_names.size());
  BOOST_FOREACH(ZNodeListener* listener, c_iter->second) {
    bool f = listener->Update(children_names);
    if (!f) {
      ret = false;
    }
  }
  return ret;
}

bool ZkClient::GetZNodeVersion(const std::string& znode_path, std::string* new_version) {
  return GetDataFromZk(znode_path, new_version);
}

bool ZkClient::GetDataFromZk(const std::string& znode_path, std::string* data) {
  if (!data) {
    return false;
  }
  boost::unique_lock<boost::mutex> lock(zk_mutex_); 
  static const int kBufferLen = 4096;
  char buffer[kBufferLen];
  memset(buffer, 0, kBufferLen);
  int watch = 1;
  int buflen = kBufferLen;
  int rc = zoo_get(zk_, znode_path.c_str(), watch, buffer, &buflen, NULL);
  if (rc != ZOK) {
    return false;
  }
  std::string version(buffer, buflen);
  *data = version;
  return true;
}
  
bool ZkClient::GetChildrenNamesFromZk(const std::string& znode_path, std::list<std::string>* list) {
  if (!list) {
    return false;
  }
  boost::unique_lock<boost::mutex> lock(zk_mutex_); 
  int watch = 1;
  int rc = zoo_exists(zk_, znode_path.c_str(), watch, NULL);
  if (rc != ZOK) {
    MCE_ERROR("znode_path  doesn't exist! znode_path:" << znode_path);
    return false;
  }

  struct String_vector str_vec;
  rc = zoo_get_children(zk_, znode_path.c_str(), 0, &str_vec);
  if (rc != ZOK) {
    deallocate_String_vector(&str_vec);
    return false;
  } 
  for (int i = 0; i < str_vec.count; ++i) {
    std::string child_name = str_vec.data[i];
    list->push_back(child_name);
  } 
  deallocate_String_vector(&str_vec);
  return true;
}


ZkClientPtr ZkClient::GetInstance(const std::string& zk_address) {
  std::string new_zk_address = zk_address;
  MCE_INFO("ZkClient::GetInstance zk_address=" << new_zk_address);
  char * address = getenv ("ZK_ADDRESS");
  if (address!=NULL) {
    new_zk_address = address;
    MCE_INFO("ZkClient::GetInstance get zk_address from env ZK_ADDRESS, zk_address=" << new_zk_address);
  }
  {
    boost::mutex::scoped_lock lock(zk_client_map_mutex_);
    std::map<std::string, ZkClientPtr>::const_iterator iter = ZkClient::address_zk_client_map_.find(new_zk_address);
    if (iter != address_zk_client_map_.end()) {
      return iter->second;
    }
  }
  ZkClient* zk_client = new ZkClient(new_zk_address);
  MCE_INFO("ZkClient::GetInstance new ZkClient " << zk_client);
  if (zk_client->Initialize()) {
    boost::mutex::scoped_lock lock(zk_client_map_mutex_);
    std::map<std::string, ZkClientPtr>::const_iterator iter = ZkClient::address_zk_client_map_.find(new_zk_address);
    if (iter != address_zk_client_map_.end()) {
      return iter->second;
    }
  }
  return ZkClientPtr();
}

void ZkClient::NotifyInitialize() {
  zk_cond_.notify_all();
}

bool ZkClient::Initialize() {
  boost::unique_lock<boost::mutex> lock(zk_mutex_);
  zk_ = zookeeper_init(zk_address_.c_str(), &ZkClient::ZkWatcher, 10000, 0, 0, 0);
  MCE_INFO("ZkClient::Initialize zk_address=" << zk_address_);
  if (zk_ == NULL) {
    MCE_INFO("ZkClient::Initialize failed! zk_address:" << zk_address_);
    return false;
  }
  {
    boost::mutex::scoped_lock lock(zk_client_map_mutex_);
    ZkClientPtr client(this);
    address_zk_client_map_.insert(std::make_pair(zk_address_, client));
    zhandle_zk_client_map_.insert(std::make_pair(zk_, client));
  }
  zk_cond_.wait(lock);
  MCE_DEBUG("ZkClient::Initialize wakes up");
  return true;
}

bool ZkClient::Reinitialize() {
  boost::unique_lock<boost::mutex> lock(zk_mutex_); 
  //delete zhandle_zk_client_map_ pair, update address_zk_client_map_
  zhandle_t* old_zk = zk_;
  zhandle_zk_client_map_.erase(old_zk);
  int rc = zookeeper_close(zk_);
  MCE_INFO("ZkClient::Reinitialize zk_address=" << zk_address_);
  zk_ = NULL;
  zk_ = zookeeper_init(zk_address_.c_str(), &ZkClient::ZkWatcher, 10000, 0, 0, 0);
  if (zk_ == NULL) {
    MCE_ERROR("ZkClient::Reinitialize failed! zk_address:" << zk_address_);
    return false;
  }
  {
    boost::mutex::scoped_lock lock(zk_client_map_mutex_);
    std::map<std::string, ZkClientPtr>::const_iterator iter = address_zk_client_map_.find(zk_address_);
    if (iter != address_zk_client_map_.end()) {
      zhandle_zk_client_map_.insert(std::make_pair(zk_, iter->second));
    }
  }
  zk_cond_.wait(lock);
  set_reinit_flag(true);
  MCE_DEBUG("ZkClient::Reinitialize wakes up");
  return true;
}

void ZkClient::ZkWatcher(zhandle_t *zzh, int type, int state, const char *path, void *watcher_ctx) {
  MCE_INFO("ZkClient::ZkWatcher type:" << WatcherEvent2String(type) << "\tstate:" << State2String(state)
            << "\tpath:" << path
            << "\tthread_id:" << boost::this_thread::get_id() << "\tzzh:" << zzh);
  if (type != ZOO_SESSION_EVENT) {
    std::string znode_path(path);
    boost::unique_lock<boost::mutex> lock(ZkClient::zk_client_map_mutex_);
    std::map<zhandle_t*, ZkClientPtr>::const_iterator iter = ZkClient::zhandle_zk_client_map_.find(zzh);
    if (iter != ZkClient::zhandle_zk_client_map_.end()) {
      MCE_INFO("ZkClient::Initialize zhandle_zk_client_map_ end state:" << state << " zh:" << zzh);
      iter->second->AddMessage(znode_path);
    }
  }
  else {
    if (state == ZOO_CONNECTING_STATE) {
      boost::unique_lock<boost::mutex> lock(ZkClient::zk_client_map_mutex_);
      std::map<zhandle_t*, ZkClientPtr>::const_iterator iter = ZkClient::zhandle_zk_client_map_.find(zzh);
      if (iter != ZkClient::zhandle_zk_client_map_.end()) {
        MCE_INFO("ZkClient::Initialize zhandle_zk_client_map_ end state:" << state << " zh:" << zzh);
        iter->second->set_reinit_flag(true);
      }
    }
    if (state == ZOO_CONNECTED_STATE) {
      Sleep(50);//等待50ms，让Initialize()先把初始化zhandle_zk_client_map_的工作完成，Initialize()进入等待后，再执行以下的代码
      boost::unique_lock<boost::mutex> lock(ZkClient::zk_client_map_mutex_);
      std::map<zhandle_t*, ZkClientPtr>::const_iterator iter = ZkClient::zhandle_zk_client_map_.find(zzh);
      if (iter != ZkClient::zhandle_zk_client_map_.end()) {
        MCE_INFO("ZkClient::Initialize zhandle_zk_client_map_ end state:" << state << " zh:" << zzh);
        iter->second->NotifyInitialize();
        if (iter->second->reinit_flag()) {
          iter->second->AddMessage(kUpdateAllZNodes);
        }
      }
    }
    if (state == ZOO_EXPIRED_SESSION_STATE) {
      ZkClientPtr client;
      {
        boost::mutex::scoped_lock lock(ZkClient::zk_client_map_mutex_);
        std::map<zhandle_t*, ZkClientPtr>::const_iterator iter = ZkClient::zhandle_zk_client_map_.find(zzh);
        if (iter != ZkClient::zhandle_zk_client_map_.end()) {
          MCE_INFO("ZkClient::Initialize zhandle_zk_client_map_ end state:" << state << " zh:" << zzh);
          client = iter->second;
        }
      }
      if (client) {
        client->Reinitialize();
        client->AddMessage(kUpdateAllZNodes);
      }
    }
  }
}

}} //  xce::tripod


