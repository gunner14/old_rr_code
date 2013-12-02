#ifndef _FEED_NEW_MEMCPROXY_CLIENT_MEMCACHEDCLIENTPOOL_
#define _FEED_NEW_MEMCPROXY_CLIENT_MEMCACHEDCLIENTPOOL_

#include "MemcachedClient.h"
#include "IceLogger.h"
#include <set>
#include <map>
#include <string>
#include "TaskManager.h"

using namespace std;


namespace xce {
namespace feedprx {

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
  PoolManager():Timer(10000, 111){
		index_=0;
		timeout_=0;
		recv_count_limit_=0;
		business_="feed";
		
	}
  pair<int, MemcachedClient*>  GetClient(Ice::Long index = -1);
  void              CheckValid();
  void              ReleaseClient(Ice::Long index, MemcachedClient* cli, bool isErr = false);
  void              PrintStatInfo();
  bool              Init(int timeout, const string& business = "feed", int recv_count_limit=0);
  void              handle();

private:
set<int> CheckErrProxy(map<int, MemcachedClient* > clients);

private:
  map<int, MemcachedClientPool*> pool_;
  vector<int> valid_;
	///////////////// error detection
  set<int> err_proxy_set_; //error detection
	map<int, int> valid_proxy_map_; //proxy to error num
	vector<int> valid_proxy_vec_; //proxy to error num.
	static const int kErrThreshold = 20;
	static const int kRetryThreshold = 5;
	static const double kLowestCntR;
	static int begin_index_ ;
	///////////////

  int timeout_;
  int recv_count_limit_;
  IceUtil::Mutex mutex_;
  Ice::Long index_;
  Ice::Long edi_; //error detection index
	string business_;
};

}}
#endif
