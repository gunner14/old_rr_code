#include "TalkFunPhoneStateManagerI.h"
#include "XmppTools.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "TaskManager.h"
#include "TalkFunPhoneStateAdapter.h"


using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk::fun::state;
using namespace MyUtil;
using namespace xce::serverstate;

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&TalkFunPhoneStateManagerI::instance(), service.createIdentity("M", ""));

	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
	ServerStateSubscriber::instance().initialize("ControllerTalkFunPhoneState", &TalkFunPhoneStateManagerI::instance(), mod, interval, new XceFeedControllerChannel());
	MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);

  TalkFunPhoneStateManagerI::instance().startSub(mod);

}

void TalkFunPhoneStateManagerI::startSub(int mod) {
  thread_ = new ControllerSub(mod);
  thread_->start(65535).detach();

}

void TalkFunPhoneStateManagerI::stat() {
  size_t cn = 0;
  const size_t up = 100;
  const size_t pl = 10;
  size_t zero = 0;
  size_t st_sz[up / pl] = {0};
  for (int i = 0; i < POOL_SIZE; ++i) {
    size_t tmp = pool_[i].size();
    cn += tmp;
    if (0 == tmp) {
      zero++;
    } else if (tmp >= up) {
      st_sz[sizeof(st_sz)/sizeof(st_sz[0])-1]++;
    } else {
      st_sz[tmp/pl]++;
    }

  }

	ostringstream os;
	os << "TalkFunPhoneStateManagerI::stat -->";
  os << " SIZE:" << cn;
  os << " zero:" << zero;
  for (size_t i = 0; i < sizeof(st_sz)/sizeof(st_sz[0]); ++i) {
    os << " le" << (i+1)*pl << ":" << st_sz[i];
  }
  MCE_INFO(os.str());

}
/* brief: 设置当前服务副本的status，status指的是该服务向controller注册时候使用的status
 * 修改status值会影响adapter的行为,服务启动时候status 默认值是0，此时该服务副本输入invaildproxy
 * 当为1时候，会成为vaildproxy。invalid时候该服务只能被写，用于热备的预热操作，当变为vaild时候
 * 该服务可读，预热完毕。
 * !!!注意，该接口调试使用，服务中已经加入了自动预热机制，不许要人为干预
 * st: status值， 0 or 1
 * return: void
 */
void TalkFunPhoneStateManagerI::setStatus(int st, const Ice::Current& current) {
  
  MCE_INFO("TalkFunPhoneStateManagerI::setStatus-->status" << st);
  ServerStateSubscriber::instance().setStatus(st);
}
/* brief: 获取存储的用户id，包括超时的。调试接口
 * return: 返回统计总值
 */
int TalkFunPhoneStateManagerI::sizeAll(const Ice::Current &current) {
  return (int)size();
}

size_t TalkFunPhoneStateManagerI::checkPool() {
  size_t sz = 0;
  for (int i = 0; i < POOL_SIZE; ++i) {
    sz += pool_[i].check_all();
  }

  return sz;

}
/* brief: 检测id池中超时用户，并删除之。调试接口
 * idx: 桶号
 * return: 返回合法id数目
 */
int TalkFunPhoneStateManagerI::checkAll(int idx, const Ice::Current &current) {
  return idx < POOL_SIZE && idx >= 0 ?
    (int)pool_[idx % POOL_SIZE].check_all()
    : -1;

}
/* brief: 获取一个桶中存储用户id，以及对应的时间戳
 * idx: 桶号
 * return: 桶中存储用户id，以及对应的时间戳
 */
map<int, int> TalkFunPhoneStateManagerI::getPoolUids(int idx, const Ice::Current &current) {
    map<int, int> uids;
    if (idx < POOL_SIZE && idx >= 0) {
        pool_[idx % POOL_SIZE].get_uids(uids);
    }
    return uids;
}

/* brief: 心跳接口，送过来的id序列对应的存储时间戳将被更新，如果不存在会创建新的
 * uids: 心跳id序列
 * return: void
 */
void TalkFunPhoneStateManagerI::HeartBeatPhone(const MyUtil::IntSeq& uids,const Ice::Current& current) {
	TimeStatN tsget;
  const char *fun = "TalkFunPhoneStateManagerI::HeartBeatPhone";


  for (vector<int>::const_iterator it = uids.begin(); it != uids.end(); ++it) {
    pool_[*it % POOL_SIZE].add(*it);
  }

  MCE_INFO(fun << "-->uids:" << uids.size() << " cost:" << tsget.getTime());

  FunStatManager::instance().StatPlus(current, fun, tsget.getTime(), (tsget.getTime() >= TIMEOUT));
}

/* brief: 下线，剔除相应id
 * uid: actor id
 * return: void
 */
void TalkFunPhoneStateManagerI::Offline(int uid, const Ice::Current& current) {
	TimeStatN tsget;
  const char *fun = "TalkFunPhoneStateManagerI::Offline";

  pool_[uid % POOL_SIZE].del(uid);

  FunStatManager::instance().StatPlus(current, fun, tsget.getTime(), (tsget.getTime() >= TIMEOUT));
}

/* brief: 上线，增加相应id
 * uid: actor id
 * return: void
 */
void TalkFunPhoneStateManagerI::Online(int uid, const Ice::Current& current) {
	TimeStatN tsget;
  const char *fun = "TalkFunPhoneStateManagerI::Online";

  pool_[uid % POOL_SIZE].add(uid);

  FunStatManager::instance().StatPlus(current, fun, tsget.getTime(), (tsget.getTime() >= TIMEOUT));

}
/* brief: 筛选给定id序列在线的用户
 * uids: 给定的id序列
 * return: 筛选结果
 */
MyUtil::IntSeq TalkFunPhoneStateManagerI::getUsersIds(const MyUtil::IntSeq& uids, const Ice::Current& current) {
	TimeStatN tsget;
  const char *fun = "TalkFunPhoneStateManagerI::getUsersIds";

  MyUtil::IntSeq res;
  for (vector<int>::const_iterator it = uids.begin(); it != uids.end(); ++it) {
    if (pool_[*it % POOL_SIZE].check(*it))
      res.push_back(*it);
  }

  FunStatManager::instance().StatPlus(current, fun, tsget.getTime(), (tsget.getTime() >= TIMEOUT));
  return res;

}

size_t TalkFunPhoneStateManagerI::size() {
  size_t cn = 0;
  for (int i = 0; i < POOL_SIZE; ++i) {
    cn += pool_[i].size();
  }
  return cn;
}

size_t TalkFunPhoneStateManagerI::check_all() {
  size_t cn = 0;
  for (int i = 0; i < POOL_SIZE; ++i) {
    cn += pool_[i].check_all();
  }
  return cn;

}


// ==================================================

size_t OnlineUids::add(int uid) {
  IceUtil::Mutex::Lock lock(mutex_);
  uids_[uid] = time(NULL);

  //MCE_DEBUG("OnlineUids::add-->uid:" << uid);
  return  uids_.size();
}

bool OnlineUids::del(int uid) {
  IceUtil::Mutex::Lock lock(mutex_);

  const char *fun = "OnlineUids::del";

  map<int, int>::iterator it = uids_.find(uid);

  bool rv = false;
  if (uids_.end() == it) {
    MCE_WARN(fun << "-->del err uid:" << uid);
  } else {
    int intv = time(NULL) - it->second;
    if (intv < ONLINE_TIMEOUT) {
      rv = true;
    } else {
      MCE_WARN(fun << "-->del timeout uid:" << uid << " stamp:" << it->second << " intv:" << intv);
    }
    uids_.erase(it);
  }

  return rv;
}

bool OnlineUids::check(int uid) {
  IceUtil::Mutex::Lock lock(mutex_);
  const char *fun = "OnlineUids::check";

  map<int, int>::iterator it = uids_.find(uid);

  bool rv = false;
  if (uids_.end() != it) {
    int intv = time(NULL) - it->second;
    if (intv < ONLINE_TIMEOUT) {
      rv = true;
    } else {
      MCE_WARN(fun << "-->timeout uid:" << uid << " stamp:" << it->second << " intv:" << intv);
      uids_.erase(it);
    }
  }

  return rv;
}

size_t OnlineUids::size() {
  IceUtil::Mutex::Lock lock(mutex_);
  return uids_.size();
}

void OnlineUids::get_uids(map<int, int> &uids) {
  IceUtil::Mutex::Lock lock(mutex_);
  uids = uids_;
}

size_t OnlineUids::check_all() {
  map<int, int> uids;
  {
    IceUtil::Mutex::Lock lock(mutex_);
    uids = uids_;
  }
  size_t cn = 0;
  for (map<int, int>::iterator it = uids.begin(); it != uids.end(); ++it) {
    if (check(it->first))
      ++cn;
  }

  return cn;
}

/*
void OnlineUids::set(const vector<int> &uids) {
  for (vector<int>::const_iterator it = uids.begin(); it != uids.end(); ++it) {
    add(*it);
  }
}

void OnlineUids::get(const vector<int> &uids, vector<int> &res) {
  for (vector<int>::const_iterator it = uids.begin(); it != uids.end(); ++it) {
    if (check(*it))
      res.push_back(*it);
  }
}
*/

// ==================================================

void ControllerSub::showState() {
  TalkFunPhoneStateAdapter &adapter = TalkFunPhoneStateAdapter::instance();
  const char *fun = "ControllerSub::showState";
  vector<TalkFunPhoneStateManagerPrx> get_seq;

  int cluster = adapter.getCluster();

  get_seq = adapter.getProxySeq(mod_);
  size_t sz0 = get_seq.size();

  get_seq = adapter.getValidProxySeq(mod_);
  size_t sz1 = get_seq.size();

  get_seq = adapter.getInvalidProxySeq(mod_);
  size_t sz2 = get_seq.size();

  get_seq = adapter.getAllProxySeq(mod_);
  size_t sz3 = get_seq.size();

  get_seq = adapter.getValidProxySeqOneway(mod_);
  size_t sz4 = get_seq.size();

  get_seq = adapter.getInvalidProxySeqOneway(mod_);
  size_t sz5 = get_seq.size();

  get_seq = adapter.getAllProxySeqOneway(mod_);
  size_t sz6 = get_seq.size();

  TalkFunPhoneStateManagerI::instance().stat();
  MCE_INFO(fun
           << "-->Cluster:" << cluster
           << " ProxySeq:" << sz0
           << " ValidProxySeq:" << sz1
           << " InvalidProxySeq:" << sz2
           << " AllProxySeq:" << sz3

           << " ValidProxySeqOneway:" << sz4
           << " InvalidProxySeqOneway:" << sz5
           << " AllProxySeqOneway:" << sz6);

}


void ControllerSub::showState2() {
  TalkFunPhoneStateAdapter &adapter = TalkFunPhoneStateAdapter::instance();

  const char *fun = "ControllerSub::showState2";

  vector<TalkFunPhoneStateManagerPrx> get_seq;
  // show service info

  int status = ServerStateSubscriber::instance().getStatus();
  MCE_INFO(fun << "-->getStatus:" << status);

    
  TalkFunPhoneStateManagerPrx prx = adapter.getProxy(mod_);
  string str_prx;
  if (0 == prx) {
    str_prx = "NULL";
  } else {
    str_prx = prx->ice_toString();
  }

  MCE_INFO(fun << "-->mod:" << mod_ << " getProxy:" << str_prx);


  get_seq = adapter.getProxySeq(mod_);
  MCE_INFO(fun << "-->mod:" << mod_ << " getProxySeq:" << get_seq.size());
  for (vector<TalkFunPhoneStateManagerPrx>::const_iterator it = get_seq.begin(); it != get_seq.end(); ++it)
    MCE_INFO(fun << "-->mod:" << mod_ << " getProxySeq:" << (*it)->ice_toString());

  get_seq = adapter.getValidProxySeq(mod_);
  MCE_INFO(fun << "-->mod:" << mod_ << " getValidProxySeq:" << get_seq.size());
  for (vector<TalkFunPhoneStateManagerPrx>::const_iterator it = get_seq.begin(); it != get_seq.end(); ++it)
    MCE_INFO(fun << "-->mod:" << mod_ << " getValidProxySeq:" << (*it)->ice_toString());


  get_seq = adapter.getInvalidProxySeq(mod_);
  MCE_INFO(fun << "-->mod:" << mod_ << " getInvalidProxySeq:" << get_seq.size());
  for (vector<TalkFunPhoneStateManagerPrx>::const_iterator it = get_seq.begin(); it != get_seq.end(); ++it)
    MCE_INFO(fun << "-->mod:" << mod_ << " getInvalidProxySeq:" << (*it)->ice_toString());


  get_seq = adapter.getAllProxySeq(mod_);
  MCE_INFO(fun << "-->mod:" << mod_ << " getAllProxySeq:" << get_seq.size());
  for (vector<TalkFunPhoneStateManagerPrx>::const_iterator it = get_seq.begin(); it != get_seq.end(); ++it)
    MCE_INFO(fun << "-->mod:" << mod_ << " getAllProxySeq:" << (*it)->ice_toString());

  // ====================
  prx = adapter.getProxyOneway(mod_);
  if (0 == prx) {
    str_prx = "NULL";
  } else {
    str_prx = prx->ice_toString();
  }

  MCE_INFO(fun << "-->mod:" << mod_ << " getProxyOneway:" << str_prx);


  get_seq = adapter.getValidProxySeqOneway(mod_);
  MCE_INFO(fun << "-->mod:" << mod_ << " getValidProxySeqOneway:" << get_seq.size());
  for (vector<TalkFunPhoneStateManagerPrx>::const_iterator it = get_seq.begin(); it != get_seq.end(); ++it)
    MCE_INFO(fun << "-->mod:" << mod_ << " getValidProxySeqOneway:" << (*it)->ice_toString());


  get_seq = adapter.getInvalidProxySeqOneway(mod_);
  MCE_INFO(fun << "-->mod:" << mod_ << " getInvalidProxySeqOneway:" << get_seq.size());
  for (vector<TalkFunPhoneStateManagerPrx>::const_iterator it = get_seq.begin(); it != get_seq.end(); ++it)
    MCE_INFO(fun << "-->mod:" << mod_ << " getInvalidProxySeqOneway:" << (*it)->ice_toString());


  get_seq = adapter.getAllProxySeqOneway(mod_);
  MCE_INFO(fun << "-->mod:" << mod_ << " getAllProxySeqOneway:" << get_seq.size());
  for (vector<TalkFunPhoneStateManagerPrx>::const_iterator it = get_seq.begin(); it != get_seq.end(); ++it)
    MCE_INFO(fun << "-->mod:" << mod_ << " getAllProxySeqOneway:" << (*it)->ice_toString());


}

void ControllerSub::run() {
  // 启动时候设置服务状态，用户在线预热，预热后修改之
  ServerStateSubscriber::instance().setStatus(0);
  const char *fun = "ControllerSub::run";

  //t_adapter adapter("ControllerTalkFunPhoneState", 120, 300, new XceFeedControllerChannel);

  for (;;) {
    // 达到预热时间时候，把服务设置为正常值，退出线程
    int intv = time(NULL)-start_time_;
    MCE_INFO(fun << "-->mod:" << mod_ << " intv:" << intv << " sub_get:" << SUB_CONTROLLER_GET);
    if (intv > SUB_CONTROLLER_GET) {
      MCE_INFO(fun << "-->mod:" << mod_ << " Set Valid");
      ServerStateSubscriber::instance().setStatus(1);
      break;
    }
    showState();
    sleep(10);

  }

  MCE_INFO(fun << "-->Sub Over");

  for (;;) {
    sleep(60);
    size_t sz = TalkFunPhoneStateManagerI::instance().checkPool();
    MCE_INFO(fun << "-->CHECK SIZE:" << sz);
    showState();
  }
}

