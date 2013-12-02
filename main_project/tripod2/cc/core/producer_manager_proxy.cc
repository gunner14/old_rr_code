#include "tripod2/cc/core/producer_manager_proxy.h"

#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"

#include "LogWrapper.h"
#include "util/cpp/TimeCost.h"

namespace xce {
namespace tripod {

ProducerManagerProxy::ProducerManagerProxy(const std::string& ns, const std::string biz, int conn_time_out)
  : ZNodeListener(kDefaultZkProducerManagerZNode + kZkSeparator + ns), 
  namespace_id_(ns), biz_id_(biz), conn_time_out_(conn_time_out) {
  ZkClientPtr zk_client = ZkClient::GetInstance(kDefaultZkAddress);
  Ice::PropertiesPtr props = Ice::createProperties();
  // set Ice.MessageSizeMax
  props->setProperty("Ice.MessageSizeMax", "204800");
  props->setProperty("Ice.Override.Timeout", boost::lexical_cast<std::string>(conn_time_out_));
  Ice::InitializationData id;
  id.properties = props;
  communicator_ = Ice::initialize(id);
  if (!zk_client) {
    MCE_ERROR("ProducerManagerProxy::ProducerManagerProxy() cannot get ZkClient, zk_address:" << kDefaultZkAddress);
  }
  else {
    MCE_DEBUG("ProducerManagerProxy this:" << this);
    zk_client->AddZNodeListener(this);
    std::list<std::string> children_names;
    zk_client->GetNodes(znode(), &children_names);
    Update(children_names);
    set_valid(true);
  }
}

ProducerManagerProxy::~ProducerManagerProxy() {
  MCE_INFO("ProducerManagerProxy::~ProducerManagerProxy()");
  ZkClientPtr zk_client = ZkClient::GetInstance(kDefaultZkAddress);
  if (zk_client) {
    zk_client->RemoveZNodeListener(this);
  }
}

bool ProducerManagerProxy::Update(const std::list<std::string>& children_names) {
  MCE_INFO("ProducerManagerProxy::Update()");
  boost::unique_lock<boost::shared_mutex> write_lock(shared_mutex_);
  ProducerManagerPrxSeq proxy_seq;
  BOOST_FOREACH(const std::string& child_name, children_names) {
    MCE_DEBUG("child_name:" << child_name);
    std::vector<std::string> units;
    boost::split(units, child_name, boost::is_any_of(":"));   
    int unit_size = units.size();
    if (unit_size != 2) {
      MCE_ERROR("ProducerManagerProxy::Update() child_name:" << child_name << " format error");
      return false;
    }
    std::string address = units[0];
    int port = 0;
    try {
      port = boost::lexical_cast<int>(units[1]);
    } catch (...) {
      MCE_ERROR("ProducerManagerProxy::Update() port format error, child_name:" << child_name);
      return false;
    }
    MCE_DEBUG("address:" << address << "\t" << "port:" << port);
    std::ostringstream otem;
    otem << "M:tcp -h " << address << " -p " << port << " -t " << conn_time_out_; 
    ProducerManagerPrx prx = ProducerManagerPrx::uncheckedCast(communicator_->stringToProxy(otem.str()));
    proxy_seq.push_back(prx);
  }
  if (!proxy_seq.empty()) {
    producer_manager_prx_seq_.swap(proxy_seq);
  } 
  return true;
}

ProducerManagerPrx ProducerManagerProxy::LocateProducerManagerProxy(const std::string& key) {
  boost::shared_lock<boost::shared_mutex> read_lock(shared_mutex_);
  if (producer_manager_prx_seq_.empty()) {
    return ProducerManagerPrx();
  }
  int size = producer_manager_prx_seq_.size();
  //srand(time(0));
  int r = rand() % size;
  return producer_manager_prx_seq_[r];
}

bool ProducerManagerProxy::Produce(const std::string& key, std::string* value, bool use_master) {
  if (!value) {
    return value;
  }
  //1. get a proxy
  //2. invoke rpc method
  //3. return resutl  
  ProducerManagerPrx proxy = LocateProducerManagerProxy(key);
  if (!proxy) {
    MCE_ERROR("ProducerManagerProxy::Produce() call LocateProducerManagerProxy return null proxy!" << "ns:" << namespace_id_ << "\tbiz_id:" << biz_id_);
    return false;
  }
  bool result = false;
  try {
    std::ostringstream otem;
    otem << "ProducerManagerPrx->producer(key=" << key << ",ns=" << namespace_id_ << ",biz=" << biz_id_ << ",use_master=" << use_master;
    MyUtil::TimeCost tc = MyUtil::TimeCost::create(otem.str(), MyUtil::TimeCost::LOG_LEVEL_INFO);
    ByteSeq v = proxy->produce(key, namespace_id_, biz_id_, use_master);
    value->reserve(v.size());
    value->assign(v.begin(), v.end());
    result = true;
  } catch (Ice::Exception& e) {
    MCE_ERROR("ProducerManagerProxy::Produce() " << e.what() << "\tns_id:" << namespace_id_ << "\tbiz_id:" << biz_id_);
  } catch (...) {
    MCE_ERROR("ProducerManagerProxy::Produce() ...exception" << "\tns_id:" << namespace_id_ << "\tbiz_id:" << biz_id_);
  }
  return result;
}

}} // xce::tripod



