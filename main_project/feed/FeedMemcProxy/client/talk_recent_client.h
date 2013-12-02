#ifndef _TALK_RECENT_CLIENT_H_
#define _TALK_RECENT_CLIENT_H_

#include "Singleton.h"
#include "MemcachedClientPool.h"
#include "TalkCommon.h"
#include "XmppTools.h"
#include "TalkRecentLoader.h"
namespace xce{
namespace talk {
namespace recent{
using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::talk::common;
using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk::recent::loader;

class TimeStat{
  timeval _begin, _end;
  public:
  TimeStat(){
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



class TalkRecentClient: public Singleton<TalkRecentClient> {
public:
  ~TalkRecentClient();
  TalkRecentClient();
	

  TalkRecentUserSeq GetRecentUser(int id);
  TalkRecentUserSeqMap GetRecentUsers(const MyUtil::IntSeq& ids);
  TalkRecentUserSeq GetRecentUserByIdWithLoad(int userid);
  TalkRecentUserSeqMap GetRecentUserBySeqWithLoad(int actor, const MyUtil::IntSeq& ids);
  void SetRecentUser(int userid, const TalkRecentUserSeq& seq);
  void SetEmptyRecent(int userid);
protected:
  bool SerializeTalkRecentUserSeq(const TalkRecentUserSeq& aseq, string * serialized);
private:
  PoolManager pool_;
};

}
}
}
#endif // _XCE_TALK_CACHE_CLIENT_H_
