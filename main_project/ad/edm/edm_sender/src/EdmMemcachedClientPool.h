#ifndef EDM_MEMCPROXY_CLIENT_MEMCACHEDCLIENTPOOL
#define EDM_MEMCPROXY_CLIENT_MEMCACHEDCLIENTPOOL

#include "EdmMemcachedClient.h"
#include "IceLogger.h"
#include <set>
#include <map>
#include <string>
#include "TaskManager.h"

using namespace std;

namespace xce{
 namespace ad{

class EdmMemcachedClientPool{
public:
  EdmMemcachedClientPool(int index, const string& ip, int port, int timeout, int recv_count_limit):
                    index_(index), ip_(ip), port_(port), timeout_(timeout), recv_count_limit_(recv_count_limit){}
  EdmMemcachedClient*  GetClient();
  void              ReleaseClient(EdmMemcachedClient* cli);
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
  list<EdmMemcachedClient*>  free_client_;
};

class EdmPoolManager : public MyUtil::Timer{
public:
  EdmPoolManager():Timer(10000, 111){index_=0;timeout_=0;recv_count_limit_=0;}
  pair<int, EdmMemcachedClient*>  GetClient(Ice::Long index);
  void              CheckValid();
  void              ReleaseClient(Ice::Long index, EdmMemcachedClient* cli);
  void              PrintStatInfo();
  bool              Init(int timeout, int recv_count_limit=0);
  void              handle();
private:
  map<int, EdmMemcachedClientPool*> pool_;
  vector<int> valid_;
  int timeout_;
  int recv_count_limit_;
  IceUtil::Mutex mutex_;
  Ice::Long index_;
};
};
};
#endif
