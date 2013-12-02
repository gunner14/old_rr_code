/*
  Copyright 2012-09-12 shiqiang.ding@renren-inc.com
                       dinggangyilang@gmail.com

  This file is part of XCS.

  工厂类，用来创建ServiceRegistryPtr,ServiceRegistryAccessorPtr.
*/

#ifndef XCS_SRC_XCS_SERVICE_FACTORY_H_
#define XCS_SRC_XCS_SERVICE_FACTORY_H_

#include "service_registry.h"
#include "service_registry_accessor.h"

namespace xcs {
class XcsServiceFactory {
 public:
  ServiceRegistryPtr CreateServiceRegistry(
                 const PropertiesConfiguration &config, int session_timeout) {
    return ServiceRegistryPtr(new ServiceRegistry(config, session_timeout));
  }
  ServiceRegistryAccessorPtr CreateServiceRegistryAccessor(
                 const PropertiesConfiguration &config, int session_timeout) {
    return ServiceRegistryAccessorPtr(
              new ServiceRegistryAccessor(config, session_timeout));
  }
};
}

#endif  // XCS_SRC_XCS_SERVICE_FACTORY_H_
