
#ifndef _XCE_FEED_INDEX_REDIS_CLIENT_H_
#define _XCE_FEED_INDEX_REDIS_CLIENT_H_

#include "RFeed.h"
#include "Singleton.h"
//#include "feed/FeedRedisClient/src/ClientPool.h"
//#include "feed/FeedRedisClient/src/FeedRedisClient.h"
#include "feed/FeedRedisClient/src/FeedCacheClient.h"


namespace xce {
namespace feed {

//using namespace std;
//using namespace MyUtil;

class FeedIndexRedisClient : public MyUtil::Singleton<FeedIndexRedisClient>, public FeedCacheClient {
public:
  ~FeedIndexRedisClient();
  FeedIndexRedisClient();


  struct IndexItem {
    Ice::Int actor;
    Ice::Int time;
    Ice::Int stype;
    Ice::Int ptype;
    Ice::Long feedid;
    Ice::Long source;
    Ice::Long psource;
    Ice::Long miniMerge;
    Ice::Long newsMerge;
  };

  string GetKey(Ice::Long source, Ice::Int stype);

  bool GetIndex(Ice::Long source, Ice::Int stype, std::vector<FeedIndexRedisClient::IndexItem>& vec); 
  bool AddIndex(FeedIndexPtr index);
  bool RemoveIndex(Ice::Long source, Ice::Int stype);

  static const size_t LIST_ITEM_SIZE;
  static const size_t KEY_SIZE;
  static const int RETRY = 3;
  static const size_t LIST_LIMIT;

private:
  void Encode(void * buf, const IndexItem& item);

};

}
}
#endif 
