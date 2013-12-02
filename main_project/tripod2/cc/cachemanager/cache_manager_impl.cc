#include "tripod2/cc/cachemanager/cache_manager_impl.h"

#include "boost/thread/locks.hpp"

#include "ServiceI.h"
#include "Monitor/client/service/ServiceMonitor.h"
#include "client/linux/handler/exception_handler.h"
//添加此头文件为了将从producer中得到的数据转化为map
#include "OceProto/proto/listorhashvalue.pb.h"
#include <sstream>

bool MyCallback(const char *dump_path, const char *minidump_id, void *context,
        bool succeeded) {
  printf("got crash %s %s\n", dump_path, minidump_id);
  return true;
}

void MyUtil::initialize() {
  using namespace xce::tripod;

  static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
  MCE_WARN("breakpad");
  MyUtil::ServiceI& service = MyUtil::ServiceI::instance();

  int cache_time_out = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service."+ ServiceI::instance().getName()+ ".CacheTimeOut", 1000);
  MCE_INFO("connect cache time out: "<< cache_time_out);
  int pm_time_out = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service."+ ServiceI::instance().getName() + ".PMTimeOut", 1000);
  MCE_INFO("connect producermanager time out: "<< pm_time_out);
 
  CacheManagerImpl* cache_manager = new CacheManagerImpl(cache_time_out,pm_time_out);

  service.getAdapter()->add(cache_manager, service.createIdentity("M", ""));

  MyUtil::ServiceMonitorManager::instance().start();

}

namespace xce {
namespace tripod {

CacheManagerImpl::CacheManagerImpl(int cache_time_out, int pm_time_out) : cache_time_out_(cache_time_out), pm_time_out_(pm_time_out) {
  MCE_INFO("CacheManagerImpl::CacheManagerImpl()");
  using namespace com::xiaonei::service;
  log_paths_.push_back("xce");
  log_paths_.push_back("tripod");
  remote_logger_ = XceLoggerReplicaAdapter::instance().getLogger();
  remote_logger_->initialize();
}

CacheManagerImpl::~CacheManagerImpl() {
  MCE_INFO("CacheManagerImpl::~CacheManagerImpl()");
  // TODO: 释放CacheProxyMap和ProducerMangerProxyMap
  //if (bucket_lock_) {
  //  delete bucket_lock_;
  //  bucket_lock_ = 0;
  //} 
}

void CacheManagerImpl::AddRemoteLog(const std::string& message) {
  if (!remote_logger_) {
    MCE_ERROR("XceLogger is not initialized!!");
  }
  else {
    remote_logger_->addLog(log_paths_, message);
  }
}

CacheProxy* CacheManagerImpl::LocateCacheProxy(const std::string& ns, const std::string& biz) {
  boost::unique_lock<boost::shared_mutex> wlock(cache_proxy_mutex_);
  CacheProxyMapType::const_iterator iter = cache_proxy_map_.find(std::make_pair(ns, biz));
  if (iter == cache_proxy_map_.end()) {
    //create
    CacheProxy* proxy = new CacheProxy(ns, biz, cache_time_out_);
    if (proxy->IsValid()) {
      cache_proxy_map_.insert(std::make_pair(std::make_pair(ns, biz), proxy));
      return proxy;
    }
    return NULL;
  }
  return iter->second;
}

ProducerManagerProxy* CacheManagerImpl::LocateProducerManagerProxy(const std::string& ns, const std::string& biz) {
  boost::unique_lock<boost::shared_mutex> wlock(producer_manager_proxy_mutex_);
  ProducerManagerProxyMapType::const_iterator iter = producer_manager_proxy_map_.find(std::make_pair(ns, biz));
  if (iter == producer_manager_proxy_map_.end()) {
    //create
    ProducerManagerProxy* proxy = new ProducerManagerProxy(ns, biz, pm_time_out_);
    if (proxy->IsValid()) {
      producer_manager_proxy_map_.insert(std::make_pair(std::make_pair(ns ,biz), proxy));
      return proxy;
    }
    return NULL;
  }
  return iter->second;
}

ByteSeq CacheManagerImpl::get(const std::string& key, 
                  const std::string& ns, 
                  const std::string& biz, 
                  int expire_time, 
                  bool use_master, const ::Ice::Current& current) {
  //bucket_lock_->Lock(key);
  std::ostringstream otem;
  otem << "CacheManagerImpl::get() key:" << key << "\tnamespace_id:" << ns << "\tbusiness_id:" << biz << "\texpire_time:" << expire_time << "\tuse_master:" << use_master;
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, otem.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  // 1. rpc produce
  // 2. save data to cache
  // 3. return result
  std::string result = "";
  ProducerManagerProxy* producer_manager_proxy = LocateProducerManagerProxy(ns, biz);
  if (!producer_manager_proxy) {
    MCE_ERROR("CacheManagerImpl::get() call LocateProducerManagerProxy() return null proxy");
    
    //bucket_lock_->Unlock(key);
    return ByteSeq();
  }
  bool flag = false;
  try {
    flag = producer_manager_proxy->Produce(key, &result, use_master);
    MCE_DEBUG("CacheManagerImpl::get() producer_manager_proxy->Produce() flag:" << flag << " ns:" << ns << " biz:" << biz << " key:" << key << " result:" << result);
  } catch (Ice::Exception& e) {
    MCE_ERROR("CacheManagerImpl::get() " << e.what())
    otem << "\t" << e.what();
    AddRemoteLog(otem.str());
  } catch (...) {
    MCE_ERROR("CacheManagerImpl::get() ...exception");
  }
  if (flag) { // save data to cache
    CacheProxy* cache_proxy = LocateCacheProxy(ns, biz);
    if (!cache_proxy) {
      MCE_ERROR("CacheManagerImpl::get() call LocateCacheProxy() return null proxy");
    }
    else {
      cache_proxy->Set(key, result, expire_time);
      AddRemoteLog(otem.str());
    }
  }
  ByteSeq byte_seq;
  if (!result.empty()) {
    byte_seq.insert(byte_seq.begin(), result.begin(), result.end());
  }
  //bucket_lock_->Unlock(key);
  return byte_seq;
}

String2StringMap CacheManagerImpl::getAllHash(const std::string& key,
          const std::string& ns,
          const std::string& biz,
          int expire_time,
          bool use_master, const ::Ice::Current& current) {
  //TODO:
  //bucket_lock_->Lock(key);
  std::ostringstream otem;
  otem << "CacheManagerImpl::getAllHash() key:" << key << "\tnamespace_id:" << ns << "\tbusiness_id:" << biz << "\texpire_time:" << expire_time << "\tuse_master:" << use_master;
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, otem.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  std::string result = "";
  ProducerManagerProxy* producer_manager_proxy = LocateProducerManagerProxy(ns, biz);
  if (!producer_manager_proxy) {
    MCE_ERROR("CacheManagerImpl::getAllHash() call LocateProducerManagerProxy() return null proxy");
    //bucket_lock_->Unlock(key);
    return String2StringMap();
  }
  bool flag = false;
  try {
    flag = producer_manager_proxy->Produce(key, &result, use_master);
    MCE_DEBUG("CacheManagerImpl::getAllHash() producer_manager_proxy->Produce() flag:" << flag << " ns:" << ns << " biz:" << biz << " key:" << key << " result:" << result);
  } catch (Ice::Exception& e) {
    MCE_ERROR("CacheManagerImpl::getAllHash() " << e.what());
    otem << "\t" << e.what();
    AddRemoteLog(otem.str());
  } catch (...) {
    MCE_ERROR("CacheManagerImpl::getAllHash() ...exception");
  }
  
  using namespace com::renren::tripod;

  ListOrHashValue listOrHashValue;
  String2StringMap map_data;
  if (!result.empty()) {
    std::istringstream in(result);
    try {
      listOrHashValue.ParseFromIstream(&in);
    } catch (...) {
      MCE_ERROR("CacheManagerImpl::getAllHash() parse from proto have exception ");
    }
  }
  if (listOrHashValue.values_size() % 2 == 0) {
    MCE_DEBUG("CacheManagerImpl::getAllHash() get the fields size is :" << listOrHashValue.values_size()); //待去log
    for(int i = 0; i != listOrHashValue.values_size(); i+=2) {
      map_data.insert(std::make_pair(listOrHashValue.values(i), listOrHashValue.values(i+1)));
    }
  } else
    flag = false; 
  if (flag) {
    CacheProxy* cache_proxy = LocateCacheProxy(ns, biz);
    if (!cache_proxy) {
      MCE_ERROR("CacheManagerImpl::getAllHash() call LocateCacheProxy() return null proxy");
    }
    else {
      cache_proxy->SetHash(key, map_data, expire_time); 
      AddRemoteLog(otem.str());
    }
  }
  
  //bucket_lock_->Unlock(key);
  return map_data;
}


bool CacheManagerImpl::loadHash(const std::string& key,
          const std::string& ns,
          const std::string& biz,
          int expire_time,
          bool use_master, const ::Ice::Current& current) {
  //bucket_lock_->Lock(key);
  std::ostringstream otem;
  otem << "CacheManagerImpl::loadHash() key:" << key << "\tnamespace_id:" << ns << "\tbusiness_id:" << biz << "\texpire_time:" << expire_time << "\tuse_master:" << use_master;
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, otem.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  std::string result = "";
  ProducerManagerProxy* producer_manager_proxy = LocateProducerManagerProxy(ns, biz);
  if (!producer_manager_proxy) {
    MCE_ERROR("CacheManagerImpl::loadHash() call LocateProducerManagerProxy() return null proxy");
    //bucket_lock_->Unlock(key);
    return false;
  }
  bool flag = false;
  try {
    flag = producer_manager_proxy->Produce(key, &result, use_master);
    MCE_DEBUG("CacheManagerImpl::loadHash() producer_manager_proxy->Produce() flag:" << flag << " ns:" << ns << " biz:" << biz << " key:" << key << " result:" << result);
  } catch (Ice::Exception& e) {
    MCE_ERROR("CacheManagerImpl::loadHash() " << e.what())
    otem << "\t" << e.what();
    AddRemoteLog(otem.str());
  } catch (...) {
    MCE_ERROR("CacheManagerImpl::loadHash() ...exception");
  }
  
  using namespace com::renren::tripod;

  ListOrHashValue listOrHashValue;
  String2StringMap map_data;
  if (!result.empty()) {
    std::istringstream in(result);
    try {
      listOrHashValue.ParseFromIstream(&in);
    } catch (...) {
      MCE_ERROR("CacheManagerImpl::loadHash() parse from proto have exception ");
    }
  }
  if (listOrHashValue.values_size() % 2 == 0) {
    MCE_DEBUG("CacheManagerImpl::loadHash() get the fields size is :" << listOrHashValue.values_size()); //待去log
    for(int i = 0; i != listOrHashValue.values_size(); i+=2) {
      map_data.insert(std::make_pair(listOrHashValue.values(i), listOrHashValue.values(i+1)));
    }
  } else
    flag = false; 
  if (flag) {
    CacheProxy* cache_proxy = LocateCacheProxy(ns, biz);
    if (!cache_proxy) {
      MCE_ERROR("CacheManagerImpl::loadHash) call LocateCacheProxy() return null proxy");
    }
    else {
      flag = cache_proxy->SetHash(key, map_data, expire_time); 
      AddRemoteLog(otem.str());
    }
  }
  //bucket_lock_->Unlock(key);
  return flag;
}
bool CacheManagerImpl::setPartHash(const std::string& key,
          const String2StringMap& value,
          const std::string& ns,
          const std::string& biz,
          const ::Ice::Current& current) {
  //TODO:
  //bucket_lock_->Lock(key);
  std::ostringstream otem;
  otem << "CacheManagerImpl::setPartHash() key:" << key << "\tnamespace_id:" << ns << "\tbusiness_id:" << biz;
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, otem.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  bool result = false;
  CacheProxy* cache_proxy = LocateCacheProxy(ns, biz);
  if (!cache_proxy) {
    MCE_ERROR("CacheManagerImpl::setPartHash() call LocateCacheProxy() return null proxy");
  } else {
    result = cache_proxy->HMSet(key, value);
    AddRemoteLog(otem.str());
  }
  //bucket_lock_->Unlock(key);
  return result;
}

bool CacheManagerImpl::deletePartHash(const std::string& key,
          const StringList& fields,
          const std::string& ns,
          const std::string& biz,
          const ::Ice::Current& current) {
  //bucket_lock_->Lock(key);
  std::ostringstream otem;
  otem << "CacheManagerImpl::deletePartHash() key:" << key << "\tnamespace_id:" << ns << "\tbusiness_id:" << biz;
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, otem.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  bool result = false;
  CacheProxy* cache_proxy = LocateCacheProxy(ns, biz);
  if (!cache_proxy) {
    MCE_ERROR("CacheManagerImpl::deletePartHash() call LocateCacheProxy() return null proxy");
  } else {
    result = cache_proxy->HDel(key, fields);
    AddRemoteLog(otem.str());
  }
  //bucket_lock_->Unlock(key);
  return result;
}

bool CacheManagerImpl::incFixedHashValue(const std::string& key,
                   const std::string& field,
                   const std::string& step,
                   const std::string& ns,
                   const std::string& biz,
                   const ::Ice::Current& current) {
  //bucket_lock_->Lock(key);
  std::ostringstream otem;
  otem << "CacheManagerImpl::incFixedHashValue() key:" << key << "\tnamespace_id:" << ns << "\tbusiness_id:" << biz;
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, otem.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  bool result = false;
  CacheProxy* cache_proxy = LocateCacheProxy(ns, biz);
  if (!cache_proxy) {
    MCE_ERROR("CacheManagerImpl::incFixedHashValue() call LocateCacheProxy() return null proxy");
  } else { 
    result = cache_proxy->HIncrBy(key, field, step);
    AddRemoteLog(otem.str());
  }
  //bucket_lock_->Unlock(key);
  return result;
} 
bool CacheManagerImpl::load(const std::string& key, 
          const std::string& ns, 
          const std::string& biz, 
          int expire_time, 
          bool use_master, const ::Ice::Current& current) {
  //bucket_lock_->Lock(key);
  std::ostringstream otem;
  otem << "CacheManagerImpl::load() key:" << key << "\tnamespace_id:" << ns << "\tbusiness_id:" << biz << "\texpire_time:" << expire_time << "\tuse_master:" << use_master;
  MCE_DEBUG("Entrance log: " << otem.str());
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, otem.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  bool result = false;
  ProducerManagerProxy* producer_manager_proxy = LocateProducerManagerProxy(ns, biz);
  if (!producer_manager_proxy) {
    MCE_ERROR("CacheManagerImpl::load() call LocateProducerManagerProxy() return null proxy");
    //bucket_lock_->Unlock(key);
    return result;
  }
  std::string value;
  bool flag = false;
  try {
    flag = producer_manager_proxy->Produce(key, &value, use_master);
    ic.step("after_producer_manager_proxy->Producer():");
    MCE_DEBUG("CacheManagerImpl::load() producer_manager_proxy->Produce() flag:" << flag << " ns:" << ns << " biz:" << biz << " key:" << key << " result:" << result);
  } catch (Ice::Exception& e) {
    MCE_ERROR("CacheManagerImpl::load() " << e.what())
    otem << "\t" << e.what();
    AddRemoteLog(otem.str());
  } catch (...) {
    MCE_ERROR("CacheManagerImpl::load() ...exception");
  }
  if (flag) { // save data to cache
    CacheProxy* cache_proxy = LocateCacheProxy(ns, biz);
    if (!cache_proxy) {
      MCE_ERROR("CacheManagerImpl::load() call LocateCacheProxy() return null proxy");
    }
    else {
      std::stringstream os;
      result = cache_proxy->Set(key, value, expire_time);
      os << "after_cache_proxy->Set()value.size():" << value.size();
      ic.step(os.str());
      AddRemoteLog(otem.str());
    }
  }
  //bucket_lock_->Unlock(key);
  return result;
}

bool CacheManagerImpl::remove(const std::string& key, 
                      const std::string& ns, 
                      const std::string& biz, 
                      const ::Ice::Current& current) {
  //bucket_lock_->Lock(key);
  std::ostringstream otem;
  otem << "CacheManagerImpl::remove() key:" << key << "\tnamespace_id:" << ns << "\tbusiness_id:" << biz;
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, otem.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  bool result = false;
  CacheProxy* cache_proxy = LocateCacheProxy(ns, biz);
  if (!cache_proxy) {
    MCE_ERROR("CacheManagerImpl::remove() call LocateCacheProxy() return null proxy");
  }
  else {
    result = cache_proxy->Remove(key);
    AddRemoteLog(otem.str());
  }
  //bucket_lock_->Unlock(key);
  return result;
}


bool CacheManagerImpl::set(const std::string& key, 
                   const ByteSeq& value, 
                   const std::string& ns, 
                   const std::string& biz, 
                   Ice::Int expire_time, 
                   const Ice::Current& current) {
  std::ostringstream otem;
  otem << "CacheManagerImpl::set() key:" << key << "\tnamespace_id:" << ns << "\tbusiness_id:" << biz << "\tsize:" << value.size();
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, otem.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  bool result = false;
  CacheProxy* cache_proxy = LocateCacheProxy(ns, biz);
  if (!cache_proxy) {
    MCE_ERROR("CacheManagerImpl::set() call LocateCacheProxy() return null proxy");
  }
  else {
    std::string v(value.begin(), value.end());
    result = cache_proxy->Set(key, v, expire_time);
    AddRemoteLog(otem.str());
  }
  return result;
}

string CacheManagerImpl::inc(const std::string& key, 
                           const std::string& step, 
                           const std::string& ns, 
                           const std::string& biz, 
                           const Ice::Current& current) {
  std::ostringstream otem;
  otem << "CacheManagerImpl::inc() key:" << key << "\tnamespace_id:" << ns << "\tbusiness_id:" << biz << "\tstep:" << step;
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, otem.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  std::string result;
  CacheProxy* cache_proxy = LocateCacheProxy(ns, biz);
  if (!cache_proxy) {
    MCE_ERROR("CacheManagerImpl::inc() call LocateCacheProxy() return null proxy");
  }
  else {
    result = cache_proxy->Inc(key, step);
    AddRemoteLog(otem.str());
  }
  return result;
}

std::string CacheManagerImpl::ping(const ::Ice::Current& current) {
  std::ostringstream otem;
  otem << "CacheManagerImpl::ping()";
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, otem.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  return "PONG";
}

StringList CacheManagerImpl::getList(const std::string& key,
                   const std::string& ns,
                   const std::string& biz,
                   int begin,
                   int limit,
                   int expire_time,
                   bool use_master, const ::Ice::Current& current) {
  //bucket_lock_->Lock(key);
  std::ostringstream otem;
  otem << "CacheManagerImpl::getList key: "<< key << "\tnamespace_id:" << ns << "\tbusiniess_id:"<<biz << "\texpiretime"<< expire_time << "\tuse_master" << use_master;
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, otem.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  std::string result = "";
  ProducerManagerProxy* producer_manager_proxy = LocateProducerManagerProxy(ns, biz);
  if (!producer_manager_proxy) {
    MCE_ERROR("CacheManagerImpl::getList call LocateProducerManagerProxy() return null proxy");
  }
  bool flag = false;
  try {
    flag = producer_manager_proxy->Produce(key, &result, use_master);
  } catch(Ice::Exception& e) {
    MCE_ERROR("CacheManagerImpl::getList() "<<e.what());
    otem<<"\t"<<e.what();
    AddRemoteLog(otem.str());
  } catch(...) {
    MCE_ERROR("CacheManagerImpl::getList() ... exception");
  }

  using namespace com::renren::tripod;

  ListOrHashValue listOrHashValue;
  StringList list_data;
  if (!result.empty()) {
    std::istringstream in(result);
    try {
      listOrHashValue.ParseFromIstream(&in);
    } catch(...) {
      MCE_ERROR("CacheManagerImpl::getList() key:"<< key << " parse from proto have occured exception!");
    }
  }
  if (listOrHashValue.values_size() != 0){
    for(int i = 0; i != listOrHashValue.values_size(); i++) {
      list_data.push_back(listOrHashValue.values(i));
    }
  } else
    flag = false;
  if (flag) {
    CacheProxy* cache_proxy = LocateCacheProxy(ns, biz);
    if(!cache_proxy) {
      MCE_ERROR("CacheManagerImpl::getList() call LocateCacheProxy() return null proxy!!");
    } else {
      cache_proxy->SetList(key, list_data, expire_time);
      AddRemoteLog(otem.str());
   }
  }
  //return list_data;
  //bucket_lock_->Unlock(key);
  return ListCut(list_data, begin, limit);
}
bool CacheManagerImpl::loadList(const std::string& key,
                   const std::string& ns,
                   const std::string& biz,
                   int expire_time,
                   bool use_master, const ::Ice::Current& current) {
  ostringstream otem;
  otem << "CacheManagerImpl::loadList key:" << key << "\tnamespace_id:" << ns << "\tbusiness_id:" << biz << "\texpire_time:" << expire_time << "\tuse_master:" << use_master;
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, otem.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  std::string result = "";
  ProducerManagerProxy* producer_manager_proxy = LocateProducerManagerProxy(ns, biz);
  if (!producer_manager_proxy) {
    MCE_ERROR("CacheManagerImpl::loadList() call LocateProducerManagerProxy() return null proxy");
    return false;
  }
  bool flag = false;
  try {
    flag = producer_manager_proxy->Produce(key, &result, use_master);
  } catch(Ice::Exception& e) {
    MCE_ERROR("CacheManagerImpl::loadList() "<<e.what());
    otem<<"\t"<<e.what();
    AddRemoteLog(otem.str());
  } catch(...) {
    MCE_ERROR("CacheManagerImpl::loadList() ... exception");
  }

  using namespace com::renren::tripod;

  ListOrHashValue listOrHashValue;
  StringList list_data;
  if (!result.empty()) {
    std::istringstream in(result);
    try {
      listOrHashValue.ParseFromIstream(&in);
    } catch(...) {
      MCE_ERROR("CacheManagerImpl::loadList() key:"<< key << " parse from proto have occured exception!");
    }
  }
  if (listOrHashValue.values_size() != 0){
    for(int i = 0; i != listOrHashValue.values_size(); i++) {
      list_data.push_back(listOrHashValue.values(i));
    }
  } else
    flag = false;
  if (flag) {
    CacheProxy* cache_proxy = LocateCacheProxy(ns, biz);
    if(!cache_proxy) {
      MCE_ERROR("CacheManagerImpl::loadList() call LocateCacheProxy() return null proxy!!");
    } else {
      cache_proxy->SetList(key, list_data, expire_time);
      AddRemoteLog(otem.str());
    }
  }
  return flag;
}

bool CacheManagerImpl::setList(const std::string& key,
                   const StringList& value,
                   const std::string& ns,
                   const std::string& biz,
                   int expire_time,
                   const ::Ice::Current& current) {
  std::ostringstream otem;
  otem << "CacheManagerImpl::setList() key:" << key << "\tnamespace_id:" << ns << "\tbusiness_id:" << biz << "\tsize:" << value.size();
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, otem.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  bool result = false;
  CacheProxy* cache_proxy = LocateCacheProxy(ns, biz);
  if (!cache_proxy) {
    MCE_ERROR("CacheManagerImpl::setList() call LocateCacheProxy() return null proxy");
  } else {
    result = cache_proxy->SetList(key, value, expire_time);
    AddRemoteLog(otem.str());
  }
  return result;
}

bool CacheManagerImpl::zAdd(const std::string& key,
                    const std::string& ns,
                    const std::string& biz,
                    const String2StringMap& zsetValue,
                    const ::Ice::Current& current) {
//TODO:
  std::ostringstream otem;
  otem << "CacheManagerImpl::zAdd() key:" << key << "\tnamespace_id:" << ns << "\tbusiness_id:" << biz;
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, otem.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  bool result = false;
  CacheProxy* cache_proxy = LocateCacheProxy(ns, biz);
  if (!cache_proxy) {
    MCE_ERROR("CacheManagerImpl::zAdd() call LocateCacheProxy() return null proxy");
  } else {
    result = cache_proxy->ZAdd(key, zsetValue);
    AddRemoteLog(otem.str());
  }
  return result;
}

bool CacheManagerImpl::zRemRangeByRank(const std::string& key,
                    const std::string& ns,
                    const std::string& biz,
                    int begin,
                    int end,
                    const ::Ice::Current& current) {
//TODO: 
  std::ostringstream otem;
  otem << "CacheManagerImpl::zRemRangeByRank() key:" << key << "\tnamespace_id:" << ns << "\tbusiness_id:" << biz;
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, otem.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  bool result = false;
  CacheProxy* cache_proxy = LocateCacheProxy(ns, biz);
  if (!cache_proxy) {
    MCE_ERROR("CacheManagerImpl::zRemRangeByRank() call LocateCacheProxy() return null proxy");
  } else {
    result = cache_proxy->ZRemRangeByRank(key, begin, end);
    AddRemoteLog(otem.str());
  }
  return result;
}

bool CacheManagerImpl::zRemRangeByScore(const std::string& key,
                    const std::string& ns,
                    const std::string& biz,
                    const std::string& min,
                    const std::string& max,
                    const ::Ice::Current& current) {
  std::ostringstream otem;
  otem << "CacheManagerImpl::zRemRangeByScore() key:" << key << "\tnamespace_id:" << ns << "\tbusiness_id:" << biz;
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, otem.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  bool result = false;
  CacheProxy* cache_proxy = LocateCacheProxy(ns, biz);
  if (!cache_proxy) {
    MCE_ERROR("CacheManagerImpl::zRemRangeByScore() call LocateCacheProxy() return null proxy");
  } else {
    result = cache_proxy->ZRemRangeByScore(key, min, max);
    AddRemoteLog(otem.str());
  }
  return result;
}

bool CacheManagerImpl::zSet(const std::string& key,
                      const std::string& ns,
                      const std::string& biz,
                      const String2StringMap& zsetValue,
                      int expire_time,
                      const ::Ice::Current& current) {
  std::ostringstream otem;
  otem << "CacheManagerImpl::zSet() key:" << key << "\tnamespace_id:" << ns << "\tbusiness_id:" << biz;
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, otem.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  bool result = false;
  CacheProxy* cache_proxy = LocateCacheProxy(ns, biz);
  if (!cache_proxy) {
    MCE_ERROR("CacheManagerImpl::zSet() call LocateCacheProxy() return null proxy");
  } else {
    result = cache_proxy->ZSet(key, zsetValue, expire_time);
    AddRemoteLog(otem.str());
  }
  return result;
}

StringList CacheManagerImpl::ListCut(const StringList& totalList, int begin, int limit) {
  int listSize = totalList.size();
  StringList result;
  if (begin < 0 || begin >= listSize) {
    return result;
  }
  try {
    if (limit < 0)
      limit = listSize - begin;
    StringList::const_iterator iter = totalList.begin();
    for(; begin > 0; --begin)
      iter++;
    for(; iter != totalList.end() && limit > 0; limit--, iter++) {
      result.push_back(*iter);
    }
  } catch(...) {
    MCE_ERROR("CacheManagerImpl::ListCut() ...exception");
  }
  return result;
}

}} // xce::tripod

