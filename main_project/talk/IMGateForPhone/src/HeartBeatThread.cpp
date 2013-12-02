#include "HeartBeatThread.h"
#include "TalkFunStateAdapter.h"
#include "TalkFunPhoneStateAdapter.h"
#include "IMStormAdapter.h"

/*说明：线程sleep一段时间，开始与IM的在线进行同步
 */
void PhoneHeartBeatThread::run() {
  IntSeq seq;
  int try_times = 0;
  for (;;) {
    if (get_users(seq, try_times)) {
      heartbeat(seq);
      seq.clear();
      try_times = 0;
    } else {
      if (try_times > TRY_SWAP) {
        try_times = 0;
      } else {
        try_times += 1;
      }
    }

    usleep(autointv_);
  }

}

/*说明：计算线程sleep时间，依据当前同步的任务量
 *参数1：当前sleep时间
 *参数2：需要同步的任务队列的长度
 *返回值：新的sleep时间
 */
int PhoneHeartBeatThread::auto_interv(int intv, size_t len) {
  if (len < (2 * ACCUM_THRESH / 5)) {
    intv += ONE_ZOOM;
    if (intv > MAX_CHECK_INTV) {
      intv = MAX_CHECK_INTV;
    }
    MCE_INFO("PhoneHeartBeatThread::auto_interv--> up len " << len << " intv " << intv);
  } else if (len > (4 * ACCUM_THRESH / 5)) {
    intv -= ONE_ZOOM;
    if (intv < MIN_CHECK_INTV) {
      intv = MIN_CHECK_INTV;
    }
    MCE_INFO("PhoneHeartBeatThread::auto_interv--> down len " << len << " intv " << intv);
  }


  return intv;
}

/*说明：获取需要同步的id列表
 *参数1：id列表容器
 *参数2：重试次数
 *返回值：是否执行成功
 */
bool PhoneHeartBeatThread::get_users(IntSeq &seq, int try_times) {
  IceUtil::Mutex::Lock lock(heart_mutex_);
  //check_intver_ = auto_interv(check_intver_, heart_seq_.size());
  if (heart_seq_.size() >= (size_t)max_heart_users_ || (!heart_seq_.empty() && try_times > TRY_SWAP)) {
    //
    IntSeq::iterator it = heart_seq_.begin();
    size_t newsize = 0;
    if (heart_seq_.size() > ONE_GET_USERS) {
      it = heart_seq_.end() - ONE_GET_USERS;
      newsize = heart_seq_.size() - ONE_GET_USERS;
    }

    seq.insert(seq.begin(), it, heart_seq_.end());
    heart_seq_.resize(newsize);
    MCE_INFO("PhoneHeartBeatThread::get_users-->size " << heart_seq_.size());
    autointv_ = auto_interv(autointv_, heart_seq_.size());
    return true;
  } else {
    MCE_INFO("PhoneHeartBeatThread::get_users-->less size " << heart_seq_.size() << " try " << try_times);
    autointv_ = auto_interv(autointv_, heart_seq_.size());
    return false;

  }
}

/*说明：将待同步id放入线程工作队列中
 *参数1：线程待同步id
 */
void PhoneHeartBeatThread::add_heart_seq(const IntSeq &seq) {
  IceUtil::Mutex::Lock lock(heart_mutex_);
  if (heart_seq_.size() > ACCUM_THRESH) {
    MCE_WARN("PhoneHeartBeatThread::add_heart_seq-->accum overflow " << heart_seq_.size());
  } else {
    heart_seq_.insert(heart_seq_.begin(), seq.begin(), seq.end());
    //MCE_INFO("PhoneHeartBeatThread::add_heart_seq-->size " << heart_seq_.size());
  }

  autointv_ = auto_interv(autointv_, heart_seq_.size());
  MCE_INFO("PhoneHeartBeatThread::add_heart_seq-->size " << heart_seq_.size() << " autointv " << autointv_);

}

JidPtr PhoneHeartBeatThread::get_jid(int uid) {
  JidPtr jid = new Jid();
  jid->userId = uid;
	ostringstream os;
	os << "PIMStorm" << uid % xce::talk::IMStormAdapter::instance().getCluster();
	jid->endpoint = os.str();
	jid->index = 2;

  return jid;

}

/*说明：同步talkfunstate 和 talkfunphonestat
 *参数1：线程待同步id
 */
void PhoneHeartBeatThread::heartbeat(const IntSeq &seq) {

  JidSeq jids;
  IntSeq uids;
  int count = 0;
  for (IntSeq::const_iterator it = seq.begin(); it != seq.end(); ++it) {
    jids.push_back(get_jid(*it));
    uids.push_back(*it);
    // 够一次就通知一次
    if (jids.size() >= (size_t)max_heart_users_) {
      /*
      try {
        fun::state::TalkFunStateAdapter::instance().HeartBeat(jids);
      } catch (Ice::Exception& e) {
        MCE_WARN("PhoneHeartBeatThread::heartbeat-->TalkFunStateAdapter::HeartBeat err:"
                 <<e<< "  file:"<<__FILE__<<"  line:"<<__LINE__);
      }
      */

      jids.clear();


      try {
        fun::state::TalkFunPhoneStateAdapter::instance().HeartBeatPhone(uids);
      } catch (Ice::Exception& e) {
        MCE_WARN("PhoneHeartBeatThread::heartbeat-->TalkFunPhoneStateAdapter.HeartBeatPhone err:"
                 <<e<< "  file:"<<__FILE__<<"  line:"<<__LINE__);
      }

      uids.clear();
      jids.clear();



      count++;
      //usleep(INTV_CALLONLINE);
    }
  }

  if (!jids.empty()) {
    /*
    try {
      fun::state::TalkFunStateAdapter::instance().HeartBeat(jids);
    } catch (Ice::Exception& e) {
      MCE_WARN("PhoneHeartBeatThread::heartbeat-->TalkFunStateAdapter::HeartBeat err:"
               <<e<< "  file:"<<__FILE__<<"  line:"<<__LINE__);
    }
    */

    try {
      fun::state::TalkFunPhoneStateAdapter::instance().HeartBeatPhone(uids);
    } catch (Ice::Exception& e) {
      MCE_WARN("PhoneHeartBeatThread::heartbeat-->TalkFunPhoneStateAdapter.HeartBeatPhone err:"
               <<e<< "  file:"<<__FILE__<<"  line:"<<__LINE__);
    }

    count++;
  }


  MCE_INFO("PhoneHeartBeatThread::heartbeat-->call online count " << count << " seq.size " << seq.size());

}

size_t PhoneHeartBeatThread::size() {
  IceUtil::Mutex::Lock lock(heart_mutex_);
  return heart_seq_.size();
}
void PhoneHeartBeatThread::clear() {
  IceUtil::Mutex::Lock lock(heart_mutex_);
  heart_seq_.clear();
  MCE_WARN("PhoneHeartBeatThread::clear-->clear seq");
}
