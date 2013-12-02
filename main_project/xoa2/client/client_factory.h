#ifndef _CLIENT_FACTORY_H_
#define _CLIENT_FACTORY_H_

#include "xoa2/client/zk_client.h"
#include "UtilCxx/src/LogWrapper.h"

namespace xoa2 {

// 需要统一的接口，同时又不能指定统一继承自该接口, 故使用模板
template <typename T>
class ClientWrap {
 protected:
  T * client_;
  std::string endpoint_key_;

 public:
  ClientWrap(T * client, const char * endpoint_key) : client_(client)
    , endpoint_key_(endpoint_key) {
  }
  
  const std::string & endpoint_key() const {
    return endpoint_key_;
  }

  T * client() {
    return client_;
  }

  virtual ~ClientWrap() {
    MCE_DEBUG("Client " << client_ << " destroyed.");
    delete client_;
  }
};

template <typename ClientType>
class ClientFactory {
 public:
  virtual ClientWrap<ClientType> * Create(const char * addr) = 0;
  // virtual ClientType * Create(const char * addr) = 0;

  ClientFactory() {} 
  virtual ~ClientFactory() {}
};

}

#endif // _CLIENT_FACTORY_H_

