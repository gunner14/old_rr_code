#include "AccountFilterI.h"

#include <boost/thread/locks.hpp>
#include <boost/foreach.hpp>

#include "base/hash.h"
#include "base/stringdigest.h"
#include "base/stringencode.h"
#include "util/cpp/InvokeMonitor.h"
#include "util/cpp/String.h"
#include "UtilCxx/src/LogWrapper.h"
#include "Monitor/client/service/ServiceMonitor.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "client/linux/handler/exception_handler.h"

using namespace std;
using namespace MyUtil;
using namespace xce::account;
using namespace xce::serverstate;

bool MyCallback(const char *dump_path, const char *minidump_id, void *context, bool succeeded) {
  printf("got crash %s %s\n", dump_path, minidump_id);
  return true;
}

//初始化
//--------------------------------------------------------------
void MyUtil::initialize() {
  static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
  MCE_WARN("breakpad");

  ServiceI& service = ServiceI::instance();

  //读取配置
  int cfg_mod = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault(SERVICE_PREFIX + ".Mod", 0);
  int cfg_interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault(SERVICE_PREFIX + ".Interval", 5);
  int cfg_status = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault(SERVICE_PREFIX + ".Status", 0);

  //注册服务
  service.getAdapter()->add(&AccountFilterI::instance(), service.createIdentity(SERVICE_IDENTITY, ""));

  //用来setdata的task
  TaskManager::instance().config(TASK_LEVEL_FILL, MyUtil::ThreadPoolConfig(CORE_THREAD_FILL, MAX_THREAD_FILL));

  //HA
  ServerStateSubscriber::instance().initialize(CONTROLLER_ENDPOINT, &AccountFilterI::instance(), cfg_mod, cfg_interval);
  {
    ServerStateSubscriber::instance().setStatus(cfg_status);
    std::ostringstream os;
    os << "MyUtil::initialize set status : " << cfg_status;
    MCE_WARN(os.str());
  }

  //监控
  ServiceMonitorManager::instance().start();
}

BucketManager::BucketManager(int size) : size_(size) {
  MCE_INFO("BucketManager::BucketManager init size: " << size);
}

int BucketManager::GetBucket(const string &key) const {
  return xce::MurmurHash2(key.c_str(), key.size(), 0) % size_;  
}

bool SHA256::operator==(const SHA256 &rhs) const {
  for(int i = 0; i < SHA256::RAW_SIZE; ++i) {
    if(key[i] != rhs.key[i]) return false;
  }
  return true;
}

bool SHA256::operator<(const SHA256 &rhs) const {
  for(int i = 0; i < SHA256::RAW_SIZE; ++i) {
    if(key[i] != rhs.key[i]) return key[i] < rhs.key[i];
  }
  return false;
}

SHA256& SHA256::operator=(const SHA256 &rhs) {
  memcpy(key, rhs.key, SHA256::RAW_SIZE);
  return *this;
}

SHA256::SHA256(const SHA256 &rhs) {
  memcpy(key, rhs.key, SHA256::RAW_SIZE);
}

SHA256::SHA256(const string &str) {
  char tmp[SHA256::HEX_SIZE + 1];
  hex_decode(tmp, SHA256::HEX_SIZE + 1, str.c_str(), SHA256::HEX_SIZE);
  memcpy(key, tmp, SHA256::RAW_SIZE);
}


AccountDataI::AccountDataI(const AccountDataI& rhs) : ids(rhs.ids.begin(), rhs.ids.end()) {
  ids.reserve(ids.size());
}

AccountDataI& AccountDataI::operator=(const AccountDataI& rhs) {
  ids.clear();
  ids.insert(ids.begin(), rhs.ids.begin(), rhs.ids.end());
  ids.reserve(ids.size());
  return *this;
}

AccountDataI::AccountDataI(const string &account, int id) {
  AddId(id);
}

void AccountDataI::AddId(int id) {
  if(find(ids.begin(), ids.end(), id) == ids.end()) {
    ids.reserve(ids.size() + 1);
    ids.push_back(id);
  }
}

AccountFilterI::AccountFilterI() : bucket_manager_(BUCKET_COUNT){
  MCE_INFO("AccountFilterI::AccountFilterI init");
}

void AccountFilterI::AddAccount(const string &account, int id, const Ice::Current &current) {
  ostringstream oss;
  oss << "account: " << account << " ,id: " << id;
  InvokeClient ic = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);
  AddAccountImpl(account, id);
}

void AccountFilterI::AddAccountImpl(const string &account, int id) {
  string digest = xce::SHA256(account);
  int bucket = bucket_manager_.GetBucket(digest);
  DigestMap& digest_map = buckets_[bucket];
  boost::unique_lock<boost::shared_mutex> write_lock(mutex_[bucket]);
  DigestMap::iterator it = digest_map.find(digest);
  if(it != digest_map.end()) {
    it->second.AddId(id);
  } else {
    AccountDataI account_new(account, id);
    digest_map[digest] = account_new;
  }
}

MyUtil::IntSeq AccountFilterI::GetIdBySHA256(const MyUtil::StrSeq &digests, const Ice::Current &current) {
  ostringstream oss;
  oss << "digest.size: " << digests.size();
  InvokeClient ic = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_DEBUG);
  MyUtil::IntSeq result;
  MyUtil::StrSeq::const_iterator it = digests.begin();
  for(; it != digests.end(); ++it) {
    if(it->size() != SHA256::HEX_SIZE || !MyUtil::StrUtil::isXDigit(*it)) {
      MCE_WARN("AccountFilterI::GetIdBySHA256 invalid digest: " << *it << " ,size: " << it->size());
      continue;
    }
    string digest_lower = MyUtil::StrUtil::toLower(*it);
    int bucket = bucket_manager_.GetBucket(digest_lower);
    DigestMap& digest_map = buckets_[bucket];
    boost::shared_lock<boost::shared_mutex> read_lock(mutex_[bucket]);
    DigestMap::iterator it_digest = digest_map.find(digest_lower);
    if(it_digest != digest_map.end()) {
      BOOST_FOREACH(int id, it_digest->second.ids) {
        result.push_back(id);
      }
    }
  }
  MCE_DEBUG("AccountFilterI::GetIdBySHA256 digests.size: " << digests.size()
      << " ,reuslt.size: " << result.size());
  return result;
}

void AccountFilterI::setData(const MyUtil::ObjectResultPtr &data, const Ice::Current &current) {
  ostringstream oss;
  oss << "size: " << data->data.size();
  if(data->data.size()) {
    oss << " ,key: " << data->data.begin()->first;
  }
  InvokeClient ic = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_DEBUG);
  TaskManager::instance().execute(new FillTask(data));  
}

bool AccountFilterI::isValid(const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, "", InvokeClient::INVOKE_LEVEL_INFO);
  return ServerStateSubscriber::instance().getStatus();
}

void AccountFilterI::setValid(bool valid, const Ice::Current& current) {
  ostringstream oss;
  oss << "value: " << valid;
  InvokeClient ic = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);
  ServerStateSubscriber::instance().setStatus((int) valid);
}

void FillTask::handle() {
  for (MyUtil::ObjectMap::iterator it = data_->data.begin(); it != data_->data.end(); ++it) {
    AccountFilterI::instance().AddAccountImpl(
        DBAccountPtr::dynamicCast(it->second)->account,
        DBAccountPtr::dynamicCast(it->second)->id);
  }
}
