#ifndef _XOA2_CLIENT_SHARD_H_
#define _XOA2_CLIENT_SHARD_H_

#include "xoa2/client/endpoint.h"
#include "xoa2/client/zk_client.h"

namespace xoa2 {
namespace client {

struct Shard : public ZkEventListener {
 public:
  Shard(const std::string & path);
  ~Shard() {}

  virtual void HandleNodeEvent(const char * path, const std::string & value);
  virtual void HandleChildEvent(const char * path, const std::vector<std::string> & children);
 
  int id() const {
    return id_;
  }

  std::string SelectEndpoint() const;

  bool LoadEndpoints();
  void Dump();

 private:
  int id_;
  std::string path_;

  mutable boost::shared_mutex endpoints_mutex_;

  std::vector<EndpointPtr> active_endpoints_;
  std::vector<EndpointPtr> inactive_endpoints_;
};
typedef boost::shared_ptr<Shard> ShardPtr;

}
}

#endif // _XOA2_CLIENT_SHARD_H_

