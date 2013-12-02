#ifndef _XOA2_CLIENT_GROUP_H_
#define _XOA2_CLIENT_GROUP_H_

#include "xoa2/client/shard.h"
#include "xoa2/client/zk_client.h"

namespace xoa2 {
namespace client {

class Group : public ZkEventListener {
 public:
  Group(const std::string & path);

  virtual void HandleNodeEvent(const char * path, const std::string & value);
  virtual void HandleChildEvent(const char * path, const std::vector<std::string> & children);

  int priority() const {
    return priority_;
  }
  bool Load();
  std::string ShardServerAddr(size_t shard) const;

 private:
  std::string path_;
  int id_;
  int priority_;
  int shard_factor_;

  mutable boost::shared_mutex shards_mutex_;
  std::map<int, ShardPtr> shards_;
};
typedef boost::shared_ptr<Group> GroupPtr;

}
}

#endif // _XOA2_CLIENT_GROUP_H_


