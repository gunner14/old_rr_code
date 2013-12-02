#include "XmppTools.h"
#include "SessionHeartBeat.h"
#include "TalkFunStateAdapter.h"



using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk;
//---------------------------------------------------------------------------

// 心跳--------------------------------------------------
size_t SessionHeartBeat::count() {
  IceUtil::Mutex::Lock lock(mutex_);

  ListIndex &seqidx = session_collects.get<list_tag>();

  return seqidx.size();

}

void SessionHeartBeat::insert(const JidPtr &s) {
  if (0 == s) {
    return;
  }

  IceUtil::Mutex::Lock lock(mutex_);

  ListIndex &seqidx = session_collects.get<list_tag>();
  seqidx.push_back(new t_session(s, time(NULL)));

  //  MCE_DEBUG("SessionHeartBeat::insert-->session " << jidToString(seqidx.back()->session()) << " stamp " << seqidx.back()->stamp());
}

void SessionHeartBeat::del(const JidPtr &s) {
  if (0 == s) {
    return;
  }

  IceUtil::Mutex::Lock lock(mutex_);

  SidIndex &sididx = session_collects.get<sid_tag>();
  SidIndex::iterator it = sididx.find(s->index);
  if (sididx.end() != it) {
    //MCE_DEBUG("SessionHeartBeat::del-->session " << jidToString((*it)->session()) << " stamp " << (*it)->stamp());
    sididx.erase(it);
  } else {
    MCE_WARN("SessionHeartBeat::del-->session " << jidToString(s) << " inexistence");
  }
}

bool SessionHeartBeat::get_old_sessions(vector<JidPtr> &sessions, const int one_heart_inv, const int max_num) {
  IceUtil::Mutex::Lock lock(mutex_);
  ListIndex &seqidx = session_collects.get<list_tag>();
  time_t now = time(NULL);
  bool ishave = false;
  if (seqidx.empty())
    return ishave;

  for (; sessions.size() < (size_t)max_num; ) {
    //MCE_INFO("SessionHeartBeat::get_old_sessions-->session befront" << *seqidx.front()->session() << " stamp " << seqidx.front()->stamp());
    time_t tmp_inv = now - seqidx.front()->stamp();
    const int secsplit = 1000000;
    const int ceilval = SessionHeartBeatPool::instance().one_beat_intv()*secsplit
      +SessionHeartBeatPool::instance().max_intv()
      +5*secsplit;
    //MCE_INFO("SessionHeartBeat::get_old_sessions-->tmp_inv " << tmp_inv*secsplit << " ceilval " << ceilval << " befront "
    //         << *seqidx.front()->session());
    if (tmp_inv*secsplit > ceilval) {
      MCE_WARN("SessionHeartBeat::get_old_sessions-->HEART_BEAT TOO SLOW! inv " << tmp_inv << " session " << jidToString(seqidx.front()->session()));
    }
    if (tmp_inv >= one_heart_inv) {
      sessions.push_back(seqidx.front()->session());
      seqidx.front()->change_stamp(now);
      seqidx.relocate(seqidx.end(), seqidx.begin());
      ishave = true;
      //MCE_INFO("SessionHeartBeat::get_old_sessions-->session affront" << *seqidx.front()->session() << " stamp " << seqidx.front()->stamp());
      //MCE_INFO("SessionHeartBeat::get_old_sessions-->session back" << *seqidx.back()->session() << " stamp " << seqidx.back()->stamp());
    } else {
      break;
    }
  }

  return ishave;
}



void SessionHeartBeatPool::start_thread() {
  // 心跳发送任务启动运行，最多启动一个
  MCE_INFO("SessionHeartBeatPool::start_thread-->call");
  IceUtil::Mutex::Lock lock(mutex_thread_run_);
  if (!flag_run_) {
    MCE_INFO("SessionHeartBeatPool::start_thread-->thread start");
    run_thread_ = true; // 标记可运行

    thread_ = new SessionHeartBeatThread;
    thread_->start(65535).detach();
    flag_run_ = true;
  }

}

void SessionHeartBeatPool::reset_runflag() {
  MCE_INFO("SessionHeartBeatPool::reset_runflag-->call");
  IceUtil::Mutex::Lock lock(mutex_thread_run_);
  flag_run_ = false;

}

void SessionHeartBeatPool::stop_thread() {
  MCE_WARN("SessionHeartBeatPool::stop_thread-->call");
  run_thread_ = false; // 标记不可运行
  
}

HeartBeatConfPtr SessionHeartBeatPool::get_thread_conf() {
  MCE_INFO("SessionHeartBeatPool::get_thread_conf-->oneBeatIntv " << one_beat_intv_
           << " maxIntvLoop " << max_intv_ << " minIntvLoop " << min_intv_
           << " ceilOneCheckGet " << ceil_one_get_ << " maxHeartUsers " << max_heart_users_
           << " floorBeatMoreGet " << get_more_floor_intv_);
  HeartBeatConfPtr conf = new HeartBeatConf;
  conf->oneBeatIntv = one_beat_intv_;
  conf->floorBeatMoreGet = get_more_floor_intv_;
  conf->maxIntvLoop = max_intv_;
  conf->minIntvLoop = min_intv_;
  conf->maxHeartUsers = max_heart_users_;
  conf->ceilOneCheckGet = ceil_one_get_;

  return conf;

}

void SessionHeartBeatPool::set_heartbeat_conf(const HeartBeatConfPtr &conf) {
  if (conf->oneBeatIntv >= 1
      && conf->floorBeatMoreGet <= conf->oneBeatIntv
      && conf->maxIntvLoop >= 1000 * 1000
      && conf->minIntvLoop >= 100 * 1000
      && conf->maxHeartUsers >= 1
      && conf->ceilOneCheckGet > conf->maxHeartUsers) {
    one_beat_intv_ = conf->oneBeatIntv;
    get_more_floor_intv_ = conf->floorBeatMoreGet;
    max_intv_ = conf->maxIntvLoop;
    min_intv_ = conf->minIntvLoop;
    max_heart_users_ = conf->maxHeartUsers;
    ceil_one_get_ = conf->ceilOneCheckGet;

  }
  MCE_WARN("SessionHeartBeatPool::set_heartbeat_conf-->oneBeatIntv " << one_beat_intv_
           << " maxIntvLoop " << max_intv_ << " minIntvLoop " << min_intv_
           << " ceilOneCheckGet " << ceil_one_get_ << " maxHeartUsers " << max_heart_users_
           << " floorBeatMoreGet " << get_more_floor_intv_);


}

size_t SessionHeartBeatPool::pool_count() {
  size_t cn = 0;
  for (size_t i = 0; i < POOL_SIZE; i++) {
    cn += pool_[i].count();
  }

  return cn;
}

void SessionHeartBeatPool::insert(const JidPtr &s) {
  if (0 == s) {
    MCE_WARN("SessionHeartBeatPool::insert-->session null");
    return;
  }

  MCE_DEBUG("SessionHeartBeatPool::insert-->session " << jidToString(s));
  int uid = s->userId;
  pool_[uid % POOL_SIZE].insert(s);
  
}

void SessionHeartBeatPool::del(const JidPtr &s) {
  if (0 == s) {
    MCE_WARN("SessionHeartBeatPool::del-->session null");
    return;
  }
  MCE_DEBUG("SessionHeartBeatPool::del-->session " << jidToString(s));
  int uid = s->userId;
  pool_[uid % POOL_SIZE].del(s);

}

int SessionHeartBeatPool::get_old_sessions(vector<JidPtr> &sessions, const int scan_begin_idx) {
  sessions.clear();
  const int end_idx = scan_begin_idx % POOL_SIZE;
  int max_num = ceil_one_get_;
  int i = end_idx;
  int bt_intv = one_beat_intv_;
  for ( ; sessions.size() < (size_t)max_heart_users_ && bt_intv >= get_more_floor_intv_; ) {
    do {
      bool ishave = pool_[i].get_old_sessions(sessions, bt_intv, max_num);

      if (ishave) {
        /*
          MCE_INFO("SessionHeartBeatPool::get_old_sessions-->i " << i << " session size " << sessions.size()
                   << " max_num " << max_num << " scan_begin_idx " << scan_begin_idx);
        */
      }

      if (sessions.size() >= (size_t)max_num) {
        break;
      }
      i++;
      i %= POOL_SIZE;
    } while (i != end_idx);
    // 如果一次连设置的每次通知心跳人数个数都不够，那么就在把心跳时间放短，接着取
    bt_intv--;
    max_num = max_heart_users_; // 如果正常的取不够，压缩取，只取够一次调用talkfun的
  }
  // 调节检测间隔值
  if (bt_intv < one_beat_intv_-ZOOM_INTV_THRESH_SCOPE) {
    interv_ += ONE_CHG_HEART_INTV;
  } else if ((int)sessions.size() > max_heart_users_*ZOOM_INTV_THRESH_TIME) {
    interv_ -= ONE_CHG_HEART_INTV;
  }
  if (interv_ > max_intv_) interv_ = max_intv_;
  if (interv_ < min_intv_) interv_ = min_intv_;

  if (sessions.size() >= (size_t)ceil_one_get_) {
    MCE_WARN("SessionHeartBeatPool::get_old_sessions-->ALREADY MIN INTERV HEARTBEAT MAY SLOW! intv " << interv_
             << " ceil get " << ceil_one_get_ << " sessions.size " << sessions.size());

  }


  MCE_INFO("SessionHeartBeatPool::get_old_sessions-->session size " << sessions.size()
           << " one_beat_intv " << one_beat_intv_ << " bt_intv " << bt_intv << " interv " << interv_);


  //  MCE_INFO("SessionHeartBeatPool::get_old_sessions-->i " << i << " session size " << sessions.size());
  return i;
}

void SessionHeartBeatPool::do_heart(const vector<JidPtr> &sessions, int cn) {
  //    MCE_INFO("SessionHeartBeatPool::do_heart-->session.size " << sessions.size() <<
  //          " deb_count " << cn);
    JidSeq jids;
    int count = 0;
    for (vector<JidPtr>::const_iterator it = sessions.begin(); it != sessions.end(); ++it) {
      jids.push_back(*it);
      // 够一次就通知一次
      if (jids.size() >= (size_t)max_heart_users_) {
        try {
          fun::state::TalkFunStateAdapter::instance().HeartBeat(jids);
        } catch (Ice::Exception& e) {
          MCE_WARN("SessionHeartBeatPool::do_heart-->TalkFunStateAdapter::HeartBeat err:"
                   <<e<< "  file:"<<__FILE__<<"  line:"<<__LINE__);
        } catch (std::exception& e) {
          MCE_WARN("SessionHeartBeatPool::do_heart-->TalkFunStateAdapter::HeartBeat err:"
                   <<e.what()<< "  file:"<<__FILE__<<"  line:"<<__LINE__);
        }
        jids.clear();
        count++;
        usleep(INTV_CALLONLINE);
      }
    }

    if (!jids.empty()) {
        try {
          fun::state::TalkFunStateAdapter::instance().HeartBeat(jids);
        } catch (Ice::Exception& e) {
          MCE_WARN("SessionHeartBeatPool::do_heart-->TalkFunStateAdapter::HeartBeat err:"
                   <<e<< "  file:"<<__FILE__<<"  line:"<<__LINE__);
        } catch (std::exception& e) {
          MCE_WARN("SessionHeartBeatPool::do_heart-->TalkFunStateAdapter::HeartBeat err:"
                   <<e.what()<< "  file:"<<__FILE__<<"  line:"<<__LINE__);
        }
        count++;
      }


    MCE_INFO("SessionHeartBeatPool::do_heart-->call online count " << count << " session.size " << sessions.size());



}


void SessionHeartBeatPool::run() {
  vector<JidPtr> sessions;
  int scan_begin_idx = 0;

  int deb_count = 0;

  MCE_INFO("HeartBeatTask::run-->begin");
  // 128
  //  MCE_INFO("HeartBeatTask::run-->sizeof(SessionHeartBeat)=" << sizeof(SessionHeartBeat));
  // 32
  //MCE_INFO("HeartBeatTask::run-->sizeof(t_session)=" << sizeof(t_session_size));

  for ( ;run_thread_; ) {

    scan_begin_idx = get_old_sessions(sessions, scan_begin_idx);
    //    MCE_INFO("HeartBeatTask::run-->heartbeat session size " << sessions.size() << " pool count " << pool_count());
    
    if (!sessions.empty()) {
      MCE_INFO("HeartBeatTask::run-->heartbeat session size " << sessions.size()
               << " pool count " << pool_count());
      do_heart(sessions, deb_count++);
    }


    usleep(interv_);
  }

  reset_runflag();
}

void  SessionHeartBeatThread::run() {
    SessionHeartBeatPool::instance().run();
}
