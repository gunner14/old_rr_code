#ifndef MEMCPROXY_CLIENT_MEMCACHEDCLIENTPOOL
#define MEMCPROXY_CLIENT_MEMCACHEDCLIENTPOOL

#include "MemcachedClient.h"
#include "IceLogger.h"
#include <set>
#include <map>
#include <string>
#include "TaskManager.h"

using namespace std;

namespace xce{
 namespace ad{

class MemcachedClientPool{
public:
  MemcachedClientPool(int index, const string& ip, int port, int timeout, int recv_count_limit):
                    index_(index), ip_(ip), port_(port), timeout_(timeout), recv_count_limit_(recv_count_limit){}
  MemcachedClient*  GetClient();
  void              ReleaseClient(MemcachedClient* cli);
  void              Clear();
  int FreeCount(){
    return free_client_.size();
  }
  string GetIp(){
    return ip_;
  }
  int    GetPort(){
    return port_;
  }
private:
  int     index_;
  string  ip_;
  int     port_;
  int     timeout_;
  int     recv_count_limit_;
  //IceUtil::Mutex mutex_;
  list<MemcachedClient*>  free_client_;
};

class PoolManager : public MyUtil::Timer{
public:
  PoolManager():Timer(10000, 111){index_=0;timeout_=0;recv_count_limit_=0;}
  pair<int, MemcachedClient*>  GetClient(Ice::Long index);
  void              CheckValid();
  void              ReleaseClient(Ice::Long index, MemcachedClient* cli);
  void              PrintStatInfo();
  bool              Init(int timeout, int recv_count_limit=0);
  void              handle();
private:
  map<int, MemcachedClientPool*> pool_;
  vector<int> valid_;
  int timeout_;
  int recv_count_limit_;
  IceUtil::Mutex mutex_;
  Ice::Long index_;
};
};
};
#endif
