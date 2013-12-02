
#include "service_registry.h"
#include "UtilCxx/src/LogWrapper.h"
#include <iostream>
namespace renren { namespace xoa2 { namespace registry {

using namespace std;
using namespace xcs;

int ServiceRegistry::registerNode(const std::string& name, const std::string& version, const std::string& state) {
  if(name == "" || version == "" || state == "") {
    MCE_WARN("ServiceRegistry::registerNode error: 传入的参数不能为空");
    return -9;  
  }
  xcs::PropertiesConfiguration configuration(config_);
  xcs::ServiceRegistryPtr registry_ptr = xcs::ServiceRegistryPtr(new xcs::ServiceRegistry(configuration, 200));
  
  int rc = registry_ptr->PublishService(name, version, state, node_);
  if (rc) {
    MCE_ERROR("RegistryService error! rc=" << rc);
    cout << "RegistryService error! rc=" << rc << endl;
  }

  return rc;
}


void ServiceRegistry::destroy() {
  cout<<"ServiceRegistry::destroy"<<endl;
  return ;
}

}}}
