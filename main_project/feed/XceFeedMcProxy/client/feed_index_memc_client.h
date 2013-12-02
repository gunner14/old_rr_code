
#ifndef _FEED_NEW_XCE_FEED_INDEX_MEMC_CLIENT_H_
#define _FEED_NEW_XCE_FEED_INDEX_MEMC_CLIENT_H_

#include "RFeed.h"
#include "Singleton.h"
#include "MemcachedClientPool.h"


class TimeStatFIndex{
        timeval _begin, _end;
public:
        TimeStatFIndex(){
                reset();
        }
        void reset(){
                gettimeofday(&_begin, NULL);
        }
        float getTime(){
                gettimeofday(&_end, NULL);
                float timeuse=1000000*(_end.tv_sec-_begin.tv_sec)+_end.tv_usec
                                -_begin.tv_usec;
                timeuse/=1000;
                return timeuse;
        }
};

namespace xce {
namespace feedprx {

using namespace std;
using namespace MyUtil;

class FIndexMemcClient : public Singleton<FIndexMemcClient> {
public:
  ~FIndexMemcClient();
  FIndexMemcClient();


#pragma pack(4)
  struct MemcValueItem {
    Ice::Int actor;
    Ice::Long feedid;
    Ice::Long miniMerge;
  };
#pragma pack()

  typedef vector<MemcValueItem> FIValue;

  string GetKey(Ice::Long source, Ice::Int stype);

  FIValue GetFIndex(Ice::Long source, Ice::Int stype); 
  bool AppendFIndex(Ice::Long source, Ice::Int stype, Ice::Int actor, Ice::Long feedid, Ice::Long miniMerge);
  bool AddFIndex(Ice::Long source, Ice::Int stype, Ice::Int actor, Ice::Long feedid, Ice::Long miniMerge);
  bool RemoveFIndex(Ice::Long source, Ice::Int stype);

  static const size_t VALUE_ITEM_SIZE;
  static const size_t KEY_SIZE;
  static const size_t VALUE_LIMIT;
  static const size_t MUTEX_COUNT = 100;
private:
  void SetBuffer(void * buf, size_t off, MemcValueItem v);
  bool ParseBuffer(const void * buf, size_t len, vector<MemcValueItem> & v);
  bool AddFIndex(string key, char * buf,  MemcachedClient*);


  IceUtil::Mutex    mutexs_[MUTEX_COUNT];

protected:
  PoolManager pool_;
};

}
}
#endif //_XCE_FEED_INDEX_MEMC_CLIENT_H_
