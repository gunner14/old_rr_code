#ifndef _XOA2_CLIENT_SERVICE_H_
#define _XOA2_CLIENT_SERVICE_H_

#include <vector>

#include "xoa2/client/group.h"
#include "xoa2/client/zk_client.h"

namespace xoa2 {
namespace client {

class Service : public ZkEventListener {
 public:
  Service(const std::string & id);

  std::string LocateService(int shard) const;

  void UpdateServiceState(const std::string & endpoint);

  bool Load();

  virtual void HandleNodeEvent(const char * path, const std::string & value);
  virtual void HandleChildEvent(const char * path, const std::vector<std::string> & children);

  std::string id() const {
    return id_;
  }

 private:
  std::string id_;
  std::string path_;

  mutable boost::mutex mutex_;
  GroupPtr active_group_;
  std::vector<GroupPtr> inactive_groups_;

  std::string GetPathById(const std::string & service_id);
};
typedef boost::shared_ptr<Service> ServicePtr;

}
}

#endif // _XOA2_CLIENT_SERVICE_H_

