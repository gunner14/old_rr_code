#ifndef _HEART_BEAT_THREAD_
#define _HEART_BEAT_THREAD_

#include <IceUtil/IceUtil.h>
#include <IceUtil/Thread.h>
#include <Ice/Ice.h>

#include "Singleton.h"
#include "TalkCommon.h"

using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::common;
using namespace MyUtil;

/*说明：此线程用于同步3G和IM 的在线状态.
 * 由3G主动发起，IM依据3G提供的在线用户列表进行同步
 */
class PhoneHeartBeatThread : virtual public IceUtil::Thread,
virtual public Singleton<PhoneHeartBeatThread> {
  enum {
    //DEFAULT_CHECK_INTV = 5, //5s
    DEFAULT_CHECK_INTV = 1, //1s

    ONE_HEART_USERS = 1000, //一次调用后面心跳接口，心跳人数
    TRY_SWAP = 2,

    // 单位心跳间隔，最多通知的人数
    MAX_HEART_USERS_UNIT = 2500000,

    //ACCUM_THRESH = ((MAX_HEART_USERS_UNIT/100) / 5) * DEFAULT_CHECK_INTV,
    ACCUM_THRESH = MAX_HEART_USERS_UNIT / 5,
    ONE_GET_USERS = ONE_HEART_USERS * 5,

    MAX_CHECK_INTV = 3 * 1000000, // 3s
    MIN_CHECK_INTV = 1 * 1000000, // 1s
    ONE_ZOOM = 1000000 / 5,
  };


	IceUtil::Mutex heart_mutex_;
	IntSeq heart_seq_;
  int max_heart_users_;
  int autointv_;

  bool get_users(IntSeq &seq, int try_times);
  void heartbeat(const IntSeq &seq);
  JidPtr get_jid(int uid);

 public:
 PhoneHeartBeatThread() : max_heart_users_(ONE_HEART_USERS)
    {
      autointv_ = (MIN_CHECK_INTV + MAX_CHECK_INTV)/2;
      start(65535).detach();
    }
  virtual void run();

  void add_heart_seq(const IntSeq &seq);
  size_t size();
  void clear();
  int auto_interv(int intv, size_t len);

};



#endif
