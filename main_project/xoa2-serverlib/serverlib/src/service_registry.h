
#ifndef _SERVICE_REGISTRY_H_
#define _SERVICE_REGISTRY_H_

#include <iostream>

#include "xcs_service_factory.h"

namespace renren { namespace xoa2 { namespace registry {

using namespace std;
using namespace boost;
using namespace xcs;
class ServiceRegistry {
public:
  ServiceRegistry(const std::string& config, const ServiceNode& node):config_(config),node_(node) {}
  int registerNode(const std::string& name, const std::string& version, const std::string& state);
  void destroy();

private:
  std::string config_;
  ServiceNode node_;

};

class AbstractRegistryFactory {
public:
  virtual shared_ptr<ServiceRegistry> Create(const std::string& config, const ServiceNode& node) = 0;
};

class DefaultRegistryFactory : public AbstractRegistryFactory {
public:
  static DefaultRegistryFactory & Instance() {
    static DefaultRegistryFactory instance;
    return instance;
  }

  shared_ptr<ServiceRegistry> Create(const std::string& config, const ServiceNode& node) {
    if(config == "" || node.name_ == "" || node.content_ == "") { 
      MCE_WARN("RegistryFactory::Create error: 传入的参数不能为空");
      shared_ptr<ServiceRegistry> registry;
      return registry;
    }

    shared_ptr<ServiceRegistry> registry(new ServiceRegistry(config,node));
  
    return registry;
  }

};

}}}

#endif
