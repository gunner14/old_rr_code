#ifndef NET_CONFIG_CLIENT_H
#define NET_CONFIG_CLIENT_H
#include <stdint.h>
#include <vector>
#include <string>
#include <memory>
#include <zookeeper/zookeeper.h>

namespace net {

//  typedef struct _zhandle zhandle_t;
//typedef struct {
//  int64_t client_id;
//  char passwd[16];
//} clientid_t;

class ConfigClient {
 public:
  ConfigClient(const std::string &addr, const std::string &path);
  bool GetIpGroup(std::vector<std::pair<std::string, int> > *v);
  ~ConfigClient();
  std::auto_ptr<zhandle_t> zh_;
  std::auto_ptr<clientid_t> myid_;
 private:
  std::string addr_;
  //DISALLOW_COPY_AND_ASSIGN(ConfigClient);
  ConfigClient(const ConfigClient &cc);
  ConfigClient& operator = (const ConfigClient &cc);
  std::string path_;

};

} // namespace

#endif

