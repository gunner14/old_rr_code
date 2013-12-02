#include "MemcachedClient.h"
#include "feed_user_property.h"
#include <boost/lexical_cast.hpp>

#include "LogWrapper.h"

using namespace xce::feedprx;

namespace xce {
namespace feedprx{


void GetUserKey(Ice::Int uid, char * key, size_t len) {
  snprintf(key, len, "FUP#%d", uid);  
  key[len - 1] = 0;
}

FeedUserPropertyClient::FeedUserPropertyClient() {
  pool_.Init(50);
}

FeedUserPropertyClient::~FeedUserPropertyClient() {
}


FeedUserPropertyData FeedUserPropertyClient::Get(Ice::Int uid){
  char key[32];
  GetUserKey(uid, key, 32);
  pair<int, MemcachedClient*> cli = pool_.GetClient(uid); 
  if(!cli.second){
    return FeedUserPropertyData();
  }
	TimeStat1 ts;
  vector<string> keys;
  keys.push_back(key);
  map<string, string> res;
  map<string, int> flags;
  cli.second->Get(keys, res, flags);
	float cost_mem = ts.getTime();
	string ip = cli.second->GetIp();
  pool_.ReleaseClient(cli.first, cli.second);
  if(res.size()!=1){
    return FeedUserPropertyData();
  }
	if( cost_mem < 160) {
		MCE_INFO("FeedUserPropertyClient::Get --> uid:" << uid << " cost_total:" << cost_mem << " ip:" << ip);
	} else {
		MCE_WARN("FeedUserPropertyClient::Get --> uid:" << uid << " cost_total:" << cost_mem << " ip:" << ip);
	}
  return FeedUserPropertyData(res[key]);
}

bool FeedUserPropertyClient::Set(int uid, FeedUserPropertyData& data){
  char key[32];
  GetUserKey(uid, key, 32);
  pair<int, MemcachedClient*> cli = pool_.GetClient(uid); 
	if(!cli.second)
		return false;
	TimeStat1 ts;
  bool res = cli.second->Set(key, data.GetOriginalData(), 0);
	float cost_mem = ts.getTime();
	string ip = cli.second->GetIp();
  pool_.ReleaseClient(cli.first, cli.second);
	if( cost_mem < 160) {
		MCE_INFO("FeedUserPropertyClient::set --> uid:" << uid << " cost_total:" << cost_mem << " ip:" << ip);
	} else {
		MCE_WARN("FeedUserPropertyClient::set --> uid:" << uid << " cost_total:" << cost_mem << " ip:" << ip);
	}
  return res;
}

}
}
/*int main(){
  char data[128] = {0};
  *(Ice::Long*)data = 1;
  *(Ice::Long*)(data+8) = 2;
  //*(Ice::Long*)(data+16) = 3;

  string s;
  s.insert(s.begin(), data, data+16);
  FeedUserPropertyData t(s);
  printf("%d %d %d\n", t.GetMaxFavFeedId(), t.GetMaxFeedId(), t.GetActiveFriendCount());
  t.SetActiveFriendCount(1321);
  printf("%d %d %d\n", t.GetMaxFavFeedId(), t.GetMaxFeedId(), t.GetActiveFriendCount());
}*/


