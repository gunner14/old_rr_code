#ifndef __USERCACHE_CLIENT_H__
#define __USERCACHE_CLIENT_H__
#include <string>
#include <vector>
#include "UserCacheService.h"
namespace xce {
namespace ad {
using namespace std;

class UserCacheClient {
public:
  UserCacheClient(const string& server_name = "localhost", int port = 9032, int threshold = 10)
            : server_name_(server_name),
              port_(port),
              threshold_(threshold){}; 
  void GetFollower(vector<int64_t> & follower, const int64_t uid , const int limit);

private:
  string server_name_;
  int port_;
  int threshold_;
};

}
}

#endif //__USERCACHE_CLIENT_H__
