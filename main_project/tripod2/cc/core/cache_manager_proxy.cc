#include "tripod2/cc/core/cache_manager_proxy.h"

#include <vector>
#include <sstream>

#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"

#include "LogWrapper.h"

namespace xce {
namespace tripod {

CacheManagerProxy::CacheManagerProxy(const std::string& ns, const std::string& biz, int conn_time_out) : 
  ZNodeListener(kDefaultZkCacheManagerZNode + kZkSeparator + ns), 
  namespace_id_(ns), biz_id_(biz), conn_time_out_(conn_time_out) {
  ZkClientPtr zk_client = ZkClient::GetInstance(kDefaultZkAddress);
  Ice::PropertiesPtr props = Ice::createProperties();
  props->setProperty("Ice.Override.Timeout", boost::lexical_cast<std::string>(conn_time_out_));
  props->setProperty("Ice.MessageSizeMax", "20480");
  Ice::InitializationData id;
  id.properties = props;
  communicator_ = Ice::initialize(id);
  if (!zk_client) {
    MCE_ERROR("CacheManagerProxy::CacheManagerProxy() cannot get ZkClient, zk_address:" << kDefaultZkAddress);
  }
  else {
    MCE_DEBUG("CacheManagerProxy this:" << this);
    zk_client->AddZNodeListener(this);
    std::list<std::string> children_names;
    zk_client->GetNodes(znode(), &children_names);
    Update(children_names);
  }
}

CacheManagerProxy::~CacheManagerProxy() {
  MCE_INFO("CacheManagerProxy::~CacheManagerProxy()");
  ZkClientPtr zk_client = ZkClient::GetInstance(kDefaultZkAddress);
  if (zk_client) {
    zk_client->RemoveZNodeListener(this);
  }
}

bool CacheManagerProxy::Update(const std::list<std::string>& children_names) {
  MCE_INFO("CacheManagerProxy::Update()");
  boost::unique_lock<boost::shared_mutex> write_lock(continuum_mutex_);
  CacheManagerPrxMapType prx_map;
  xce::Continuum* cm = new xce::Continuum();
  BOOST_FOREACH(const std::string& child_name, children_names) {
    MCE_DEBUG("child_name:" << child_name);
    std::vector<std::string> units;
    boost::split(units, child_name, boost::is_any_of(":"));   
    int unit_size = units.size();
    if (unit_size != 2) {
      MCE_ERROR("CacheManagerProxy::Update() child_name:" << child_name << " format error");
      return false;
    }
    std::string address = units[0];
    int port = 0;
    try {
      port = boost::lexical_cast<int>(units[1]);
    } catch (...) {
      MCE_ERROR("CacheManagerProxy::Update() port format error, child_name:" << child_name);
      return false;
    }
    MCE_DEBUG("address:" << address << "\t" << "port:" << port);
    std::ostringstream otem;
    otem << "M:tcp -h " << address << " -p " << port << " -t " << conn_time_out_; 
    CacheManagerPrx prx = CacheManagerPrx::uncheckedCast(communicator_->stringToProxy(otem.str()));
    cm->Add(child_name, 100);
    prx_map.insert(std::make_pair(child_name, prx));   
  } 
  if (!prx_map.empty()) {
    cm->Rebuild();
    continuum_.reset(cm);
    cache_manager_prx_map_.swap(prx_map);
  }
  MCE_DEBUG("CacheManagerProxy::Update() continuum_->size():" << continuum_->size());
  MCE_DEBUG("CacheManagerProxy::Update() cache_manager_prx_map_.size():" << cache_manager_prx_map_.size());
  return true;
}

CacheManagerPrx CacheManagerProxy::LocateCacheManagerProxy(const std::string& key) {
  boost::shared_lock<boost::shared_mutex> read_lock(continuum_mutex_);
  std::string name = continuum_->Locate(xce::Continuum::Hash(key));
  MCE_TRACE("CacheManagerProxy::LocateCacheManagerProxy() KEY: " << key << " name :" << name);
  if (name.empty()) {
    return CacheManagerPrx();
  }
  CacheManagerPrxMapType::const_iterator iter = cache_manager_prx_map_.find(name);
  if (iter == cache_manager_prx_map_.end()) {
    return CacheManagerPrx();
  }
  return iter->second;
}

bool CacheManagerProxy::Get(const std::string& key, std::string* value, int expired_time, bool use_master) {
  if (!value) {
    return false;
  }
  // 1. get a proxy
  // 2. invoke rpc method
  // 3. return result
  CacheManagerPrx proxy = LocateCacheManagerProxy(key);
  if (!proxy) {
    MCE_ERROR("CacheManagerProxy::Get() LocateCacheManagerProxy return null proxy! " << "ns:" << namespace_id_ << "\tbiz:" << biz_id_);
    return false;
  }
  bool result = false;
  try {
    ByteSeq v = proxy->get(key, namespace_id_, biz_id_, expired_time, use_master);
    MCE_DEBUG("CacheManagerProxy::Get proxy->get() ByteSeq v.size():" << v.size());
    value->insert(value->begin(), v.begin(), v.end());
    result = true;
  } catch (Ice::Exception& e) {
    MCE_ERROR("CacheManagerProxy::Get() " << " key= " << key << " ns= "
        << namespace_id_ << " biz= " << biz_id_ << " Ice::Exception: " << e.what());
  } catch (std::exception e) {
    MCE_ERROR("CacheManagerProxy::Get() " << " key= " << key << " ns= "
        << namespace_id_ << " biz= " << biz_id_ << " std::exception: " << e.what());
  } catch (...) {
    MCE_ERROR("CacheManagerProxy::Get() " << " key= " << key << " ns= "
        << namespace_id_ << " biz= " << biz_id_ << " unkown exception");
  }
  return result;
}

bool CacheManagerProxy::Load(const std::string& key, int expired_time, bool use_master) {
  // 1. get a proxy
  // 2. invoke rpc method
  // 3. return result
  CacheManagerPrx proxy = LocateCacheManagerProxy(key);
  if (!proxy) {
    MCE_ERROR("CacheManagerProxy::Load() LocateCacheManagerProxy return null proxy!" << "ns:" << namespace_id_ << "\tbiz:" << biz_id_);
    return false;
  }
  bool result = false;
  try {
    result = proxy->load(key, namespace_id_, biz_id_, expired_time, use_master);
  } catch (Ice::Exception& e) {
    MCE_ERROR("CacheManagerProxy::Load() " << " key= " << key << " ns= "
        << namespace_id_ << " biz= " << biz_id_ << " Ice::Exception: " << e.what());
  } catch (std::exception e) {
    MCE_ERROR("CacheManagerProxy::Load() " << " key= " << key << " ns= "
        << namespace_id_ << " biz= " << biz_id_ << " std::exception: " << e.what());
  } catch (...) {
    MCE_ERROR("CacheManagerProxy::Load() " << " key= " << key << " ns= "
        << namespace_id_ << " biz= " << biz_id_ << " unkown exception");
  }
  return result;
}

bool CacheManagerProxy::Remove(const std::string& key) {
  CacheManagerPrx proxy = LocateCacheManagerProxy(key);
  if (!proxy) {
    MCE_ERROR("CacheManagerProxy::Remove() LocateCacheManagerProxy return null proxy!" << "ns:" << namespace_id_ << "\tbiz:" << biz_id_);
    return false;
  }
  bool result = false;
  try {
    result = proxy->remove(key, namespace_id_, biz_id_);
  } catch (Ice::Exception& e) {
    MCE_ERROR("CacheManagerProxy::Delete() " << e.what());
  } catch (...) {
    MCE_ERROR("CacheManagerProxy::Delete() ...exception");
  }
  return result;
}

bool CacheManagerProxy::Set(const std::string& key, const std::string& value, int expire_time) {
  CacheManagerPrx proxy = LocateCacheManagerProxy(key);
  if (!proxy) {
    MCE_ERROR("CacheManagerProxy::Set() LocateCacheManagerProxy return null proxy!" << "ns:" << namespace_id_ << "\tbiz:" << biz_id_);
    return false;
  }
  bool result = false;
  try {
    ByteSeq byte_seq;
    byte_seq.insert(byte_seq.begin(), value.begin(), value.end());
    result = proxy->set(key, byte_seq, namespace_id_, biz_id_, expire_time);
  } catch (Ice::Exception& e) {
    MCE_ERROR("CacheManagerProxy::Set() " << e.what());
  } catch (...) {
    MCE_ERROR("CacheManagerProxy::Set() ...exception");
  }
  return result;
  
}

std::string CacheManagerProxy::Inc(const std::string& key, const std::string& step) {
  CacheManagerPrx proxy = LocateCacheManagerProxy(key);
  if (!proxy) {
    MCE_ERROR("CacheManagerProxy::Inc() LocateCacheManagerProxy return null proxy!" << "ns:" << namespace_id_ << "\tbiz:" << biz_id_);
    return false;
  }
  std::string result;
  try {
    result = proxy->inc(key, step, namespace_id_, biz_id_);
  } catch (Ice::Exception& e) {
    MCE_ERROR("CacheManagerProxy::Inc() " << e.what());
  } catch (...) {
    MCE_ERROR("CacheManagerProxy::Inc() ...exception");
  }
  return result;
}

bool CacheManagerProxy::GetAllHash(const std::string& key, String2StringMap* value, int expire_time, bool use_master) {
  CacheManagerPrx proxy = LocateCacheManagerProxy(key);
  if (!proxy) {
    MCE_ERROR("CacheManagerProxy::GetAllHash() LocateCacheManagerProxy return null proxy!" << "ns:" << namespace_id_ << "\tbiz:" << biz_id_);
    return false;
  }
  bool result = false;
  try {
    String2StringMap temp_value = proxy-> getAllHash(key, namespace_id_, biz_id_, expire_time, use_master);
    //*value  = proxy-> getHashFromProducer(key, namespace_id_, biz_id_, expire_time, use_master);
    MCE_DEBUG("CacheManagerProxy::GetAllHash() proxy->getHashFromProducer String2StringMap temp_value.size():" << temp_value.size());
    value->swap(temp_value);
    result = true;
  } catch(Ice::Exception& e) {
    MCE_ERROR("CacheManagerProxy::GetAllHash() "<< e.what());
  } catch(...) {
    MCE_ERROR("CacheManagerProxy::GetAllHash() ...exception");
  }
  return result;
}

bool CacheManagerProxy::LoadHash(const std::string& key, int expire_time, bool use_master) {
  CacheManagerPrx proxy = LocateCacheManagerProxy(key);
  if (!proxy) {
    MCE_ERROR("CacheManagerProxy::LoadHash() LocateCacheManagerProxy return null proxy!" << "ns:" << namespace_id_ << "\tbiz:" << biz_id_);
    return false;
  }
  bool result = false;
  try {
    result = proxy-> loadHash(key, namespace_id_, biz_id_, expire_time, use_master);
    MCE_DEBUG("CacheManagerProxy::LoadHash() the result is :" << result);
  } catch(Ice::Exception& e) {
    MCE_ERROR("CacheManagerProxy::LoadHash() "<< e.what());
  } catch(...) {
    MCE_ERROR("CacheManagerProxy::LoadHash() ...exception");
  }
  return result;
}

bool CacheManagerProxy::IncFixedHashValue(const std::string& key, const std::string& field, const std::string& step) { 
  CacheManagerPrx proxy = LocateCacheManagerProxy(key);
  if (!proxy) {
    MCE_ERROR("CacheManagerProxy::IncFixedHashValue() LocateCacheManagerProxy return null proxy!" << "ns:" << namespace_id_ << "\tbiz:" << biz_id_);
    return false;
  }
  bool result = false;
  try {
    result = proxy->incFixedHashValue(key, field, step, namespace_id_, biz_id_) ;
    MCE_DEBUG("CacheManagerProxy::IncFixedHashValue() the result is :" << result);
  } catch(Ice::Exception& e) {
    MCE_ERROR("CacheManagerProxy::IncFixedHashValue() "<< e.what());
  } catch(...) {
    MCE_ERROR("CacheManagerProxy::IncFixedHashValue() ...exception");
  }
  return result;

}

bool CacheManagerProxy::SetPartHash(const std::string& key, const String2StringMap& value) {
  CacheManagerPrx proxy = LocateCacheManagerProxy(key);
  if (!proxy) {
    MCE_ERROR("CacheManagerProxy::SetPartHash() LocateCacheManagerProxy return null proxy!" << "ns:" << namespace_id_ << "\tbiz:" << biz_id_);
    return false;
  }
  bool result = false;
  try {
    result = proxy->setPartHash(key, value, namespace_id_, biz_id_);
  } catch(Ice::Exception& e) {
    MCE_ERROR("CacheManagerProxy::SetPartHash() " << e.what());
  } catch(...) {
    MCE_ERROR("CacheManagerProxy::SetPartHash() ...exception");
  }
  return result; 
}

bool CacheManagerProxy::DeletePartHash(const std::string& key, const StringList& fields) {
  CacheManagerPrx proxy = LocateCacheManagerProxy(key);
  if (!proxy) {
    MCE_ERROR("CacheManagerProxy::DeletePartHash() LocateCacheManagerProxy return null proxy!" << "ns:" << namespace_id_ << "\tbiz:" << biz_id_);
    return false;
  }
  bool result = false;
  try {
    result = proxy->deletePartHash(key, fields, namespace_id_, biz_id_);
  } catch(Ice::Exception& e) {
    MCE_ERROR("CacheManagerProxy::DeletePartHash() " << e.what());
  } catch(...) {
    MCE_ERROR("CacheManagerProxy::DeletePartHash() ...exception");
  }
  return result; 
}

bool CacheManagerProxy::GetList(const std::string& key, StringList* value, int begin, int limit, int expire_time, bool use_master) {
  if (!value) {
    return false;
  }
  CacheManagerPrx proxy = LocateCacheManagerProxy(key);
  if (!proxy) {
    MCE_ERROR("CacheManagerProxy::GetList() LocateCacheManagerProxy return null proxy! Key: " << key << "ns:" << namespace_id_ << "\tbiz:" << biz_id_);
    return false;
  }
  bool result = false;
  try {
    *value = proxy->getList(key, namespace_id_, biz_id_, begin, limit, expire_time, use_master);
    MCE_DEBUG("CacheManagerProxy::GetList() proxy->getList StringList.size():" << value->size());
    result = true;
  } catch(Ice::Exception& e) {
    MCE_ERROR("CacheManagerProxy::GetList()" << e.what());
  } catch(...) {
    MCE_ERROR("CacheManagerProxy::GetList() ...exception()");
  }
  return result;
}
bool CacheManagerProxy::LoadList(const std::string& key, int expire_time, bool use_master) {
  CacheManagerPrx proxy = LocateCacheManagerProxy(key);
  if(!proxy) {
    MCE_ERROR("CacheManagerProxy::LoadList() LocateCacheManagerProxy return null proxy! key:"<< key << "\tns:" << namespace_id_ << "\tbiz:"<<biz_id_);
    return false;
  }
  bool result = false;
  try {
    result = proxy->loadList(key, namespace_id_, biz_id_, expire_time, use_master);
    MCE_DEBUG("CacheManagerProxy::LoadList() the result is :" << result);
  } catch(Ice::Exception& e) {
    MCE_ERROR("CacheManagerProxy::LoadList() " << e.what());
  } catch(...) {
    MCE_ERROR("CacheManagerProxy::LoadList() ...exception!");
  }
  return result;
}
bool CacheManagerProxy::SetList(const std::string& key, const StringList& value, int expire_time){
  CacheManagerPrx proxy = LocateCacheManagerProxy(key);
  if (!proxy) {
    MCE_ERROR("CacheManagerProxy::SetList() LocateCacheManagerProxy return null proxy! key:" << key << "ns:" << namespace_id_ << "\tbiz:" << biz_id_);
    return false;
  }
  bool result = false;
  try {
    result = proxy->setList(key, value, namespace_id_, biz_id_, expire_time);
    MCE_DEBUG("CacheManagerProxy::SetList() the result is :" << result);
  } catch(Ice::Exception& e) {
    MCE_ERROR("CacheManagerProxy::SetList() "<< e.what());
  } catch(...) {
    MCE_ERROR("CacheManagerProxy::SetList() ...exception");
  }
  return result;
}

bool CacheManagerProxy::ZAdd(const std::string& key, const String2StringMap& set_with_scores) {
  CacheManagerPrx proxy = LocateCacheManagerProxy(key);
  if (!proxy) {
    MCE_ERROR("CacheManagerProxy::ZAdd() LocateCacheManagerProxy return null proxy! key:" << key << "ns:" << namespace_id_ << "\tbiz:" << biz_id_);
    return false;
  }
  bool result = false;
  try {
    result = proxy->zAdd(key, namespace_id_, biz_id_, set_with_scores);
    MCE_DEBUG("CacheManagerProxy::ZAdd() the result is :" << result);
  } catch(Ice::Exception& e) {
    MCE_ERROR("CacheManagerProxy::ZAdd() "<< e.what());
  } catch(...) {
    MCE_ERROR("CacheManagerProxy::ZAdd() ...exception");
  }
  return result;
}

bool CacheManagerProxy::ZRemRangeByRank(const std::string& key, int begin, int end) {
  CacheManagerPrx proxy = LocateCacheManagerProxy(key);
  if (!proxy) {
    MCE_ERROR("CacheManagerProxy::ZRemRangeByRank() LocateCacheManagerProxy return null proxy! key:" << key << "ns:" << namespace_id_ << "\tbiz:" << biz_id_);
    return false;
  }
  bool result = false;
  try {
    result = proxy->zRemRangeByRank(key, namespace_id_, biz_id_, begin, end);
    MCE_DEBUG("CacheManagerProxy::ZRemRangeByRank() the result is :" << result);
  } catch(Ice::Exception& e) {
    MCE_ERROR("CacheManagerProxy::ZRemRangeByRank() "<< e.what());
  } catch(...) {
    MCE_ERROR("CacheManagerProxy::ZRemRangeByRank() ...exception");
  }
  return result;
}

bool CacheManagerProxy::ZRemRangeByScore(const std::string& key, const std::string& min, const std::string& max) {
  CacheManagerPrx proxy = LocateCacheManagerProxy(key);
  if (!proxy) {
    MCE_ERROR("CacheManagerProxy::ZRemRangeByScore() LocateCacheManagerProxy return null proxy! key:" << key << "ns:" << namespace_id_ << "\tbiz:" << biz_id_);
    return false;
  }
  bool result = false;
  try {
    result = proxy->zRemRangeByScore(key, namespace_id_, biz_id_, min, max);
    MCE_DEBUG("CacheManagerProxy::ZRemRangeByScore() the result is :" << result);
  } catch(Ice::Exception& ex) {
    MCE_ERROR("CacheManagerProxy::ZRemRangeByScore() ns:" << namespace_id_ << ", biz:" << biz_id_ << ", key:" << key <<", Ice::Exception: " << ex.what());
  } catch(std::exception& ex) {
    MCE_ERROR("CacheManagerProxy::ZRemRangeByScore() ns:" << namespace_id_ << ", biz:" << biz_id_ << ", key:" << key <<", std::exception: " << ex.what());
  } catch(...) {
    MCE_ERROR("CacheManagerProxy::ZRemRangeByScore() ns:" << namespace_id_ << ", biz:" << biz_id_ << ", key:" << key <<", unknown exception.");
  }
  return result;
}

bool CacheManagerProxy::ZSet(const std::string& key, const String2StringMap& set_with_scores, int expire_time) {
  CacheManagerPrx proxy = LocateCacheManagerProxy(key);
  if (!proxy) {
    MCE_ERROR("CacheManagerProxy::ZSet() LocateCacheManagerProxy return null proxy! key:" << key << "ns:" << namespace_id_ << "\tbiz:" << biz_id_);
    return false;
  }
  bool result = false;
  try {
    result = proxy->zSet(key, namespace_id_, biz_id_, set_with_scores, expire_time);
    MCE_DEBUG("CacheManagerProxy::ZSet() the result is : " << result);
  } catch(Ice::Exception& e) {
    MCE_ERROR("CacheManagerProxy::ZSet() " << e.what());
  } catch(...) {
    MCE_ERROR("CacheManagerProxy::ZSet() ... exception");
  }
  return result;
}

}} // xce::tripod


  
