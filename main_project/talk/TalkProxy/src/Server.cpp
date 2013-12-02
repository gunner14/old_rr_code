#include "Server.h"
#include "Manager.h"
#include "XmppTools.h"
#include <sys/resource.h>
#include "TalkCacheAdapter.h"

#include "PresenceAdapter.h"
#include "OnlineCenterAdapter.h"
#include "TalkLogAdapter.h"
#include "Action.h"

#include "TalkFunStateAdapter.h"
using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::util;
using namespace mop::hi::svc::adapter;
using namespace MyUtil;
using namespace talk::online::adapter;
using namespace xiaonei::talk;
//---------------------------------------------------------------------------
void MyUtil::initialize() {
	MCE_DEBUG("begin initialize...");
	//set openfiles
	struct rlimit limit;
	if (getrlimit(RLIMIT_NOFILE, &limit) < 0) {
		MCE_WARN("no right to get open files");
	} else {
		MCE_INFO("openfile  cur:"<<limit.rlim_cur<<"  max:"<<limit.rlim_max);
	}
	limit.rlim_cur = limit.rlim_max = RLIM_INFINITY;
	if (setrlimit(RLIMIT_NOFILE, &limit) < 0) {
		MCE_WARN("no right to set open files");
	} else {
		MCE_INFO("openfile  cur:"<<limit.rlim_cur<<"  max:"<<limit.rlim_max);
	}

	ServiceI& service = ServiceI::instance();
	Ice::PropertiesPtr props = service.getCommunicator()->getProperties();

	//	Manager::instance().setLogic(LogicManagerPrx::uncheckedCast(service.getCommunicator()->stringToProxy(props->getPropertyWithDefault("Service."
	//			+ service.getName() + ".TalkLogic", "M@TalkLogic"))));

	service.getAdapter()->add(&Manager::instance(), service.createIdentity(PROXY_MANAGER,
			""));

	MCE_DEBUG("Ready to start Server");
	MCE_INFO("LINUX_VERSION_CODE:"<< LINUX_VERSION_CODE
			<<"   epoll basic level:"<< KERNEL_VERSION(2, 5, 45));

	Server::instance().start(props);
	try{
		OnlineCenterAdapter::instance().closeProxyServer(service.getName());
	}catch(Ice::Exception& e){
		MCE_WARN("initialize --> call close proxy err:"<<e<< "  file:"<<__FILE__<<"  line:"<<__LINE__);
	}catch(std::exception& e){
		MCE_WARN("initialize --> call close proxy err:"<<e.what()<< "  file:"<<__FILE__<<"  line:"<<__LINE__);
	}

	TaskManager::instance().scheduleRepeated(new OnlineSizeStater(60*1000));
	TaskManager::instance().scheduleRepeated(new OnlineSizeMoniter(1000));
	TaskManager::instance().scheduleRepeated(&LoginProcedureStat::instance());
	//	service.registerXceObserver("TalkProxy", new ServiceSchema);
	MCE_DEBUG("finish initialize...");
}

//----------------------------------------------------------------------
void Server::start(const Ice::PropertiesPtr& props) {
	int port =
			props->getPropertyAsIntWithDefault("Service.Acceptor.Port", 5222);
	int serverId = props->getPropertyAsIntWithDefault("Service.ServerIndex", -1);
	string ssl_key = props->getPropertyWithDefault("Service.SSLKey","/opt/XNTalk/etc/server.pem");
	if(serverId == -1){
		MCE_WARN("Server::start --> not config server index err");
		return;
	}
	_renren = (bool)props->getPropertyAsIntWithDefault("Service.Renren", 0);
	//::xce::nio::ConnectionPool::instance().initialize(serverId,new SessionFactoryI);
	_refactor = new Refactor(port,serverId,new SessionFactoryI, 20, ssl_key, 500000);
	_refactor->start();
	MCE_DEBUG("Server::started");
}

void Server::stop() {
//	for (set<SessionPtr>::iterator it = _applies.begin(); it != _applies.end(); ++it) {
//		(*it)->shutdown();
//	}
//	for (SessionMap::iterator it = _sessions.begin(); it != _sessions.end(); ++it) {
//		it->second->shutdown();
//	}
//	_refactor->stop();
//	_refactor = 0;
}

//void Server::apply(const SessionPtr& session) {
//	IceUtil::Mutex::Lock lock(_mutex);
//
//	if (session->getConnectionState() == ASIO_SHUTDOWN) {
//		MCE_WARN("Server::apply --> connection stat shutdown");
//		return;
//	}
//
//	_applies.insert(session);
//	MCE_INFO("Server::apply -> " << session.get() << ", applies: "
//			<< _applies.size() << ", size: " << _sessions.size());
//
//}

//void Server::join(const SessionPtr& session) {
//	IceUtil::Mutex::Lock lock(_mutex);
//
//	if (session->getConnectionState() == ASIO_SHUTDOWN) {
//		return;
//	}
//
//	_applies.erase(session);
//	if (session->jid()) {
//		_sessions[session->jid()] = session;
//
//	}
//	MCE_INFO("Server::join -> " << session.get() << ", applies: "
//			<< _applies.size() << ", size: " << _sessions.size());
//
//}
//
//void Server::leave(const SessionPtr& session) {
//	IceUtil::Mutex::Lock lock(_mutex);
//
//	_applies.erase(session);
//	if (session->jid()) {
//		_sessions.erase(session->jid());
//
//	}
//	MCE_INFO("Server::leave -> " << session.get() <<", applies: "
//			<<_applies.size()<<", size: "<<_sessions.size());
//
//
//}
//
//void Server::leave(const JidPtr& jid) {
//	SessionPtr session;
//	{
//		IceUtil::Mutex::Lock lock(_mutex);
//
//		SessionMap::iterator pos = _sessions.find(jid);
//		if (pos != _sessions.end()) {
//			session = pos->second;
//		}
//	}
//	if (session.get()) {
//		session->shutdown();
//	}
//}

bool Server::verify(const JidPtr& jid, const string& ticket) {
	IceUtil::Mutex::Lock lock(_mutex);
	MCE_DEBUG("call Server::checkTicket --> jid:"<<jid->userId
			<<"@talk.xiaonei.com/"<<jid->endpoint<<"_"<<jid->index
			<<"   ticket:"<<ticket);

	//SessionMap::iterator it = _sessions.find(jid);
	::xce::nio::ConnectionPtr conn = _refactor->getConnection(jid->index);
	if(conn){
		AsioChannelPtr channel = conn->channel();
		SessionPtr session = SessionPtr::dynamicCast(channel);
		if(session){
			if(session->ticket() == ticket){
				return true;
			}
		}
	}
	return false;
//	if (it == _sessions.end()) {
//		MCE_DEBUG("Server::checkTicket --> not find jid");
//		return false;
//	}
//	MCE_DEBUG("Server::checkTicket --> ticket:"<<it->second->ticket() <<"   t:"
//			<<ticket);
//	if (it->second->ticket() == ticket) {
//		return true;
//	}
//	return false;
}

string Server::GetTicket(const JidPtr& jid) {

  MCE_DEBUG("call Server::GetTicket --> jid:"<<jid->userId
      <<"@talk.xiaonei.com/"<<jid->endpoint<<"_"<<jid->index);

  IceUtil::Mutex::Lock lock(_mutex);
  //SessionMap::iterator it = _sessions.find(jid);
  ::xce::nio::ConnectionPtr conn = _refactor->getConnection(jid->index);
  if(conn){
    AsioChannelPtr channel = conn->channel();
    SessionPtr session = SessionPtr::dynamicCast(channel);
    if(session){
      return session->ticket();
    }
  }
  return "";
}

bool Server::deliver(const JidPtr& jid, const string& message) {
//	SessionPtr session;
//	{
//		IceUtil::Mutex::Lock lock(_mutex);
//		SessionMap::iterator pos = _sessions.find(jid);
//		if (pos != _sessions.end()) {
//			session = pos->second;
//		}
//	}
//	if (session.get()) {
//		if (session->deliver(message)) {
//			return true;
//		} else {
//			MCE_WARN("Server::deliver -> " << session.get() << "->"
//					<< jidToString(jid)
//					<< " should be destroyed, but it exists");
//			return false;
//		}
//	}
//	return false;
	if(_renren){
		changeDomain(const_cast<string&>(message), "xiaonei.com", "renren.com");
	}else{
		changeDomain(const_cast<string&>(message), "renren.com", "xiaonei.com");
	}
	return _refactor->deliver(jid->index,message);
}

int Server::size(bool apply) {
	IceUtil::Mutex::Lock lock(_mutex);
	//	if (apply) {
	//		ostringstream os;
	//		for (set<SessionPtr>::iterator it = _applies.begin(); it
	//				!= _applies.end(); ++it) {
	//			os <<"apply_session_ptr:"<<*(it->get()) <<"\t";
	//		}
	//		MCE_INFO(os.str());
	//	}
	_refactor->connectionPoolSize();
	if(apply){
		return getJoinCount() - getOnlineUserCount();
	}
	return getOnlineUserCount();
}

bool Server::indexExist(Ice::Long index){
	return (_refactor->getConnection(index) != 0);
}

/*void Server::sendLog(const JidPtr& from, const JidPtr& to, const string& msg){
	if(!_sendLog){
		return;
	}
	try{
		TalkLogAdapter::instance().feed(from, to, msg);
	}catch(...){
		MCE_WARN("Invoke TalkLog error");
	}
}
*/

void changeDomain(string& data, const string& src, const string& dst){
//校内改名人人
	if(data.empty() || src.empty()){
		return;
	}
	stringstream tmp;
	bool isInside = false;
	for(int i=0; i<data.size(); ){
		switch(data[i]){
			case '<' :
				isInside = true;
				break;
			case '>' :
				isInside = false;
				break;
			/*case '@':
				if(isInside && (data.size() - i) > 5 && strncmp(data.c_str()+i+1, "talk.", 5) == 0) {
					tmp << "@talk.";
					i += 6;
					tmp << data[i];
					i++;
					continue;
				}*/
			default:
				if(isInside && data[i]==src[0] && (data.size()-i)>src.size() &&
				(strncmp(data.c_str()+i+1, src.c_str()+1, src.size()-1) == 0)){
					tmp << dst;
					i += src.size();
					continue;
				}
				break;
		}
		tmp << data[i];
		i++;
	}
	data = tmp.str();
}

void Server::kick(const JidPtr& jid){
	_refactor->connectionShutdown(jid->index, false);
}


// 心跳--------------------------------------------------
size_t SessionHeartBeat::count() {
  IceUtil::Mutex::Lock lock(mutex_);

  ListIndex &seqidx = session_collects.get<list_tag>();

  return seqidx.size();

}

void SessionHeartBeat::insert(const SessionPtr &s) {
  if (0 == s || 0 == s->jid()) {
    return;
  }

  IceUtil::Mutex::Lock lock(mutex_);

  ListIndex &seqidx = session_collects.get<list_tag>();
  seqidx.push_back(new t_session(s, time(NULL)));

  MCE_DEBUG("SessionHeartBeat::insert-->session " << *seqidx.back()->session() << " stamp " << seqidx.back()->stamp());
}

void SessionHeartBeat::del(const SessionPtr &s) {
  if (0 == s || 0 == s->jid()) {
    return;
  }

  IceUtil::Mutex::Lock lock(mutex_);

  SidIndex &sididx = session_collects.get<sid_tag>();
  SidIndex::iterator it = sididx.find(s->jid()->index);
  if (sididx.end() != it) {
    MCE_DEBUG("SessionHeartBeat::del-->session " << *(*it)->session() << " stamp " << (*it)->stamp());
    sididx.erase(it);
  } else {
    MCE_WARN("SessionHeartBeat::del-->session " << *s << " inexistence");
  }
}

bool SessionHeartBeat::get_old_sessions(vector<SessionPtr> &sessions, const int one_heart_inv, const int max_num) {
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
      MCE_WARN("SessionHeartBeat::get_old_sessions-->HEART_BEAT TOO SLOW! inv " << tmp_inv << " session " << *seqidx.front()->session());
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

void SessionHeartBeatPool::insert(const SessionPtr &s) {
  if (0 == s || 0 == s->jid()) {
    MCE_WARN("SessionHeartBeatPool::insert-->session null");
    return;
  }

  MCE_DEBUG("SessionHeartBeatPool::insert-->session " << *s);
  int uid = s->jid()->userId;
  pool_[uid % POOL_SIZE].insert(s);
  
}

void SessionHeartBeatPool::del(const SessionPtr &s) {
  if (0 == s || 0 == s->jid()) {
    MCE_WARN("SessionHeartBeatPool::del-->session null");
    return;
  }
  MCE_DEBUG("SessionHeartBeatPool::del-->session " << *s);
  int uid = s->jid()->userId;
  pool_[uid % POOL_SIZE].del(s);

}

int SessionHeartBeatPool::get_old_sessions(vector<SessionPtr> &sessions, const int scan_begin_idx) {
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

void SessionHeartBeatPool::do_heart(const vector<SessionPtr> &sessions, int cn) {
  //    MCE_INFO("SessionHeartBeatPool::do_heart-->session.size " << sessions.size() <<
  //          " deb_count " << cn);
    JidSeq jids;
    int count = 0;
    for (vector<SessionPtr>::const_iterator it = sessions.begin(); it != sessions.end(); ++it) {
      jids.push_back((*it)->jid());
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
  vector<SessionPtr> sessions;
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
