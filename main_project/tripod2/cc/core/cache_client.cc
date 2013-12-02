#include "tripod2/cc/core/cache_client.h"

#include "LogWrapper.h"

namespace xce {
namespace tripod {

//bool CacheClient::Get(const std::string& key, std::string* value) {
//  return false;
//}
//bool CacheClient::Set(const std::string& key, const std::string& value, int secs) {
//  return false;
//}

std::string CacheClient::Inc(const std::string& key, const std::string& step) {
  std::string result;
  return result;
}

std::map<std::string, bool> CacheClient::MultiGet(const StringListType& keys,StringMapType* values) {
  return std::map<std::string, bool>();
}

bool CacheClient::GetList(const std::string& key, StringListType* value, int begin, int limit) {
  return false;
}
bool CacheClient::SetList(const std::string& key, const StringListType& list_value, int secs) {
  return false;
}
bool CacheClient::GetHash(const std::string& key, const StringListType& fields, StringMapType* hash_value) {
  return false;
}

bool CacheClient::HMSet(const std::string& key, const StringMapType& hash_value) {
  return false;
}

bool CacheClient::HGetAll(const std::string& key, StringMapType* hash_value) {
  return false;
}

bool CacheClient::HDel(const std::string& key, const StringListType& fields) {
  return false;
}

bool CacheClient::SetHash(const std::string& key, const StringMapType& hash_value, int secs) {
  return false;
}
int CacheClient::Exists(const std::string& key) {
  return false;
}
bool CacheClient::Remove(const std::string& key) {
  return false;
}

bool CacheClient::HIncrBy(const std::string& key, const std::string& field, const std::string& step) {
  return false;
}

int CacheClient::Size(const std::string& key) {
  return -1;
}

bool CacheClient::IsValid() {
  return false;
}

std::string CacheClient::Ping() {
  return "";
}

bool CacheClient::ZAdd(const std::string& key, const StringMapType& set_with_scores) {
  return false;
}

bool CacheClient::ZSet(const std::string& key, const StringMapType& set_with_scores, int secs) {
  return false;
}

bool CacheClient::ZRemRangeByRank(const std::string& key, int begin, int end) {
  return false;
}

bool CacheClient::ZRemRangeByScore(const std::string& key, const std::string& min, const std::string& max) {
  return false;
}

bool CacheClient::ZRange(const std::string& key, int begin, int limit, StringListType* zset_value) {
  return false;
}

std::string CacheClient::Type(const std::string& key) {
  return "";
}

int CacheClient::Ttl(const std::string& key) {
  return -1;
}

//-----

int CacheClientPool::count_ = 0;

CacheClientPool::CacheClientPool(const std::string address, int port, int timeout, int core_size, int max_size)
  : address_(address), port_(port), timeout_(timeout), core_size_(core_size), max_size_(max_size) {
  used_ = 0;
  id_ = ++count_;
}

CacheClientPool::~CacheClientPool() {
  MCE_DEBUG("ClientPool::~ClientPool");
  boost::mutex::scoped_lock lock(un_used_mutex_);
  std::deque<CacheClient*>::iterator iter = un_used_.begin();
  for (; iter != un_used_.end(); ++iter) {
    Destroy(*iter);
  } 

}

CacheClient* CacheClientPool::BorrowItem() {
  CacheClient* client = NULL;
  bool create_new = false;
  boost::lock_guard<boost::mutex> lock(un_used_mutex_);
  if (un_used_.empty()) {
    if (used_ < max_size_)  {
      create_new = true;
    }
  } else {
    client = un_used_.front();
    un_used_.pop_front();
  }
  if (create_new) {
      client = Create();
  }
  if (client != NULL) {
      used_++;
  } else {
    MCE_WARN("CacheClientPool::BorrowItem get client fail, used size: " << used_ << " unused size: " << un_used_.size());
  }
  return client;
}

void CacheClientPool::ReturnItem(CacheClient *client) {
  if (client != NULL) {
    if (client->IsValid()) {
      boost::mutex::scoped_lock lock(un_used_mutex_);
      un_used_.push_back(client);
      used_--;
    }
    else {
      Destroy(client);
      boost::mutex::scoped_lock lock(un_used_mutex_);
      used_--;
    }
  } else {

  }
}

void CacheClientPool::Destroy(CacheClient* client) {
  if (client) {
    delete client;
    client = 0;
  }
}

std::string CacheClientPool::Dump() {
  boost::mutex::scoped_lock lock(un_used_mutex_);
  std::stringstream otem;
  otem << "[id:" << id_ << ", address:" << address_  << ", port:" << port_ ;
  otem << ", core_size:" << core_size_ << ", max_size:" << max_size_;
  otem << ", used:" << used_ << ", un_used.size():" << un_used_.size() << "]";
  return otem.str();
}

}} // xce::tripod



