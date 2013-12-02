#ifndef _XOA2_CLIENT_ENDPOINT_H_
#define _XOA2_CLIENT_ENDPOINT_H_

#include "xoa2/client/zk_client.h"

namespace xoa2 {
namespace client {

class Endpoint {
 public:
  Endpoint(const std::string & path);
  ~Endpoint();
  bool Load();

  std::string addr() const {
    return addr_;
  }

  bool enabled() const {
    return enabled_;
  }

  bool healthy() const {
    return healthy_;
  }

  bool active() const {
    return enabled_ && healthy_;
  }
 private:
  std::string path_;
  std::string addr_;
  std::string host_;
  short port_;

  bool enabled_;
  bool healthy_;

  bool PropIsEnabled(const std::map<std::string, std::string> & props);
  bool PropIsHealthy(const std::map<std::string, std::string> & props);
};
typedef boost::shared_ptr<Endpoint> EndpointPtr;

}
}

#endif // _XOA2_CLIENT_ENDPOINT_H_
