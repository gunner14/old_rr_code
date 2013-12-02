#ifndef _IMONLINE_CACHE_CLIENT_H_
#define _IMONLINE_CACHE_CLIENT_H_

#include "Singleton.h"
#include "MemcachedClientPool.h"
#include "TalkCommon.h"
#include "XmppTools.h"

namespace xce {
namespace talk {
namespace online{
using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::talk::common;
using namespace com::xiaonei::talk::util;

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



class OnlineCenterClient: public Singleton<OnlineCenterClient> {
public:
  ~OnlineCenterClient();
  OnlineCenterClient();

  AvatarNSeq GetUser(int id);
  AvatarNMap GetUsers(const MyUtil::IntSeq& uids);
	void SetOnlineInfo(int userid, const AvatarNSeq& seq);
	void SetOnlineInfos(int userid, const AvatarNMap& map);
	void DeleteUser(int userid);
protected:
  bool SerializeAvatarNSeq(const AvatarNSeq& seq, string * serialized);
private:
  PoolManager pool_;
};

}
}
}
#endif // _XCE_TALK_CACHE_CLIENT_H_
