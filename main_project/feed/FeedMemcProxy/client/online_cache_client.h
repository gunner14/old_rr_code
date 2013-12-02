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



class OnlineCenterClient: public Singleton<OnlineCenterClient> { // client 不能暴露太多的细节接口，否则将来升级更新会比较麻烦.
public:
  ~OnlineCenterClient();
  OnlineCenterClient();


  bool GetUser(AvatarNSeq &ans, int id, bool backup = false);
  bool GetUsers(AvatarNSeqMap &dict, const MyUtil::IntSeq& uids, bool backup = false);


	
	void SetOnlineInfo(int userid, const AvatarNSeq& seq);
	void SetOnlineInfos(const AvatarNSeqMap& map);
	bool DeleteUser(int userid);
protected:
  bool SerializeAvatarNSeq(const AvatarNSeq& seq, string * serialized);
private:
  PoolManager pool_;
};

}
}
}
#endif // _XCE_TALK_CACHE_CLIENT_H_
