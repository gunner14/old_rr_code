#include "endpoint.h"

// #include <iostream>

#include "xoa2/client/zk_util.h"
#include "UtilCxx/src/LogWrapper.h"

namespace xoa2 {
namespace client {

Endpoint::Endpoint(const std::string & path) 
    : path_(path)
    , enabled_(false)
    , healthy_(true) {
  size_t pos = path.find_last_of('/');
  addr_ = path.substr(pos + 1);
  pos = addr_.find(':');
  host_ = addr_.substr(0, pos);
  try {
    port_ = boost::lexical_cast<int>(addr_.substr(pos + 1));
  } catch (...) {
  }
}

Endpoint::~Endpoint() {
}

bool Endpoint::PropIsEnabled(const std::map<std::string, std::string> & props) {
  std::map<std::string, std::string>::const_iterator it = props.find("status");
  if (it != props.end() && (it->second == "enabled")) {
    return true;
  }
  return false;
}

bool Endpoint::PropIsHealthy(const std::map<std::string, std::string> & props) {
  std::map<std::string, std::string>::const_iterator it = props.find("healthy");
  if (it != props.end() && (it->second == "0")) {
    return false;
  }
  return true;
}

bool Endpoint::Load() {
  std::string data;
  if(!ZookeeperClient::Instance().GetValue(path_.c_str(), &data)) {
    // 加载失败时，保持现有状态不变
    return false;
  }

  std::map<std::string, std::string> props;
  ParseProperty(data, &props);
  enabled_ = PropIsEnabled(props);
  healthy_ = PropIsHealthy(props);

  MCE_INFO("xoa2 endpoint " << path_ << " : enabled=" 
    << enabled_ << " healthy=" << healthy_);
  // ZookeeperClient::Instance().AddNodeListener(path_.c_str(), shared_from_this());
  return true;
}

}
}

