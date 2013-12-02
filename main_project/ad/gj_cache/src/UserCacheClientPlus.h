#ifndef __USERCACHE_CLIENT_PLUS_H__
#define __USERCACHE_CLIENT_PLUS_H__
#include <string>
#include <vector>
#include "UserCacheService.h"
#include "TaskManager.h"
#include "Singleton.h"
#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>
#include <IceUtil/RWRecMutex.h>
#include <IceUtil/Mutex.h>

#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>
#include <protocol/TCompactProtocol.h>

namespace xce {
namespace ad {
using namespace std;
const int CONN_SIZE = 20;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

class UserCacheClientPlus : public MyUtil::Singleton<UserCacheClientPlus>{
public:
  UserCacheClientPlus();
  ~UserCacheClientPlus();
  void CheckConnValid();
  void GetFollower(vector<int64_t> & follower, const int64_t uid, const int limit);
  void GetFans(vector<int64_t> & fans, const int64_t uid, const int limit);
  void GetFansByIndex(vector<int64_t> & fans, const int64_t uid, const int64_t index, const int limit);
  void GetTopicFollower(vector<int64_t> & follower, const int64_t top_id, const int limit);
  void GetTopicFollowerByIndex(vector<int64_t> & follower, const int64_t top_id, const int64_t index, const int limit);
  void GetAlbumFans(vector<int64_t> & fans, const int64_t album_id, const int64_t index, const int limit);
private:
  string server_list[2];
  int port_;
  int threshold_;

  //boost::shared_ptr<FollowerConn> conn_list[CONN_SIZE];
  IceUtil::RWRecMutex mutex_;

  list<UserCacheServiceClient *> conn_pool;
  list<UserCacheServiceClient *> conn_invalid_pool;
  //vector< pair <UserCacheServiceClient,bool> > conn_pool;

  UserCacheServiceClient* GetClient();
  void ReleaseClient(UserCacheServiceClient*);
  UserCacheServiceClient* NewClient();
  UserCacheServiceClient* NewClientInternal(const string &name, int port);

};

class FailLoadTask : virtual public MyUtil::Task{
public:
  FailLoadTask(){}
  virtual void handle();
};

}
}

#endif //__USERCACHE_CLIENT_PLUS_H__