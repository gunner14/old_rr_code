#include "TalkFunStateManagerI.h"
#include "XmppTools.h"
#include "FeedMemcProxy/client/online_cache_client.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "CommonTask.h"
#include "client/linux/handler/exception_handler.h"
#include "TaskManager.h"
#include "TalkFunStateAdapter.h"
#include "Cache.h"
using namespace com::xiaonei::talk::util;
using namespace xce::talk::online;
using namespace com::xiaonei::talk::fun::state;
using namespace MyUtil;


bool MyCallback(const char *dump_path, const char *minidump_id, void *context, bool succeeded) {
	printf("got crash %s %s\n", dump_path, minidump_id);
	return true;
}




void MyUtil::initialize() {
	static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);

	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&TalkFunStateManagerI::instance(), service.createIdentity("M", ""));

	TalkFunStateManagerI::instance().usebackup_ = false;
	TalkFunStateManagerI::instance().InitDoingPool();
	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
	xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerTalkFunState", &TalkFunStateManagerI::instance(), mod, interval, new XceFeedControllerChannel());
	MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);
	DoingThread::instance();
}

bool TalkFunStateManagerI::addHeartBeat(const JidSeq &seq, JidSeq &cache) {
  IceUtil::Mutex::Lock lock(heart_mutex_);
  heart_seq_.insert(heart_seq_.end(), seq.begin(), seq.end());
  //MCE_DEBUG("TalkFunStateManagerI::addHeartBeat-->heart_seq_ size:" << heart_seq_.size());

  time_t nw = time(NULL);
  if (nw - heart_stamp_ > 10) {
    cache.swap(heart_seq_);
    heart_seq_.clear();
    heart_stamp_ = nw;
    return true;
  } else {

    return false;
  }

}

void TalkFunStateManagerI::heartInfo(const JidSeq& jidseq, const Ice::Current& current) {
  if (!jidseq.empty()) {
    const string eptype = CheckEndpointType(jidseq.front()->endpoint);
    MCE_INFO("heartInfo --> type:" << eptype << " jseq size:" << jidseq.size() << " remote:" << GetRemoteAdress(current));
  }

}

/* brief: 心跳接口，被跳的用户对应的在线时间戳会更新，不存在会创建
   jidseq: 给定的心跳用户序列
   return: void
 */
void TalkFunStateManagerI::HeartBeat(const JidSeq& jidseq, const Ice::Current& current){
	TimeStatN tsget;
  heartInfo(jidseq, current);
  HeartBeatNotSplit(jidseq);
  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::HeartBeat", tsget.getTime(), (tsget.getTime() >= TIMEOUT));
  return;

  JidSeq jseq;
  if (!addHeartBeat(jidseq, jseq)) {
    FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::HeartBeat.addcache", tsget.getTime(), (tsget.getTime() >= TIMEOUT));
    return;
  }
  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::HeartBeat.split", tsget.getTime(), (tsget.getTime() >= TIMEOUT));
  MCE_INFO("TalkFunStateManagerI::HeartBeat--> jseq size:" << jseq.size());


	JidSeq seq;
	
	int mod = -1;
	mod = ServiceI::instance().getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
	int cluster = TalkFunStateAdapter::instance().Cluster();

	JidSeqMap jmp;
	if(mod <= 100 && mod >= 0){
		for(JidSeq::const_iterator it = jseq.begin(); it != jseq.end(); ++it){
			JidPtr jid = *it;
			int idmod = jid->userId % cluster;
			if(idmod == mod){
				seq.push_back(jid);
			}else{
				JidSeqMap::iterator mit = jmp.find(idmod);
				if(mit == jmp.end()){
					jmp[idmod] = JidSeq();
				}
				jmp[idmod].push_back(jid);
			}
		}
		for(JidSeqMap::iterator mit = jmp.begin(); mit != jmp.end(); ++mit){
			MCE_INFO("TalkFunStateManagerI::HeartBeat--> broadcast size :" << jmp.size() << " jid.size:" << mit->second.size());
      try {
        TalkFunStateAdapter::instance().HeartBeatNotSplit(mit->second);

      } catch (Ice::Exception& e){
        MCE_WARN("TalkFunStateManagerI::HeartBeat-->TalkFunStateAdapter::HeartBeatNotSplit-->size:" << mit->second.size() << " err:" << e);
      }

		}
	} else {
    seq = jseq;
    MCE_WARN("TalkFunStateManagerI::HeartBeat-->TalkFunStateAdapter::HeartBeatNotSplit-->cold repli size:" << seq.size());
  }


  HeartBeatNotSplit(seq);

}

/* brief: 废弃的rpc，接口
 */
void TalkFunStateManagerI::HeartBeatNotSplit(const JidSeq &jseq, const Ice::Current &current) {
	JidSeq seq = jseq;
	for(JidSeq::iterator it = seq.begin(); it != seq.end();){
		DoingCachePtr cacheptr = doingpool_[(*it)->userId % DOINGPOOLSIZE];
		bool ok = cacheptr->BeginWrite(*it, 0);
		if(ok == false){
			it = seq.erase(it);
		}else{
			++it;
		}
	}
	




	TimeStatN ts;
	set<int> distuids;
	IntSeq uids;
	for(JidSeq::const_iterator it = seq.begin(); it != seq.end(); ++it){
		JidPtr jid = *it;	
		if(distuids.find(jid->userId) == distuids.end()){
			uids.push_back(jid->userId);
		}
		distuids.insert(jid->userId);
	}

	TimeStatN tsget;
	AvatarNSeqMap ans;
  if (!OnlineCenterClient::instance().GetUsers(ans, uids, usebackup_)) {
    MCE_WARN("TalkFunStateManagerI::HeartBeatNotSplit-->get memc error" << " seq.size:" << seq.size() << " uids.size:" << uids.size() << " ans.size:" << ans.size());
  }

  FunStatManager::instance().Stat("TalkFunStateManagerI::HeartBeatNotSplit.GetUsers", tsget.getTime(), (tsget.getTime() >= TIMEOUT));
	MCE_INFO("TalkFunStateManagerI::HeartBeatNotSplit-->" << " seq.size:" << seq.size() << " uids.size:" << uids.size() << " ans.size:" << ans.size());
	TimeStatN tsdeal;
	for(JidSeq::const_iterator jit = seq.begin(); jit != seq.end(); ++jit){
		JidPtr jid = *jit;
		int userid = jid->userId;
		AvatarNSeqMap::iterator mit = ans.find(userid);
		bool hasfindjid = false;
		if(mit != ans.end()){
			AvatarNSeq avaseq = mit->second;
			for(AvatarNSeq::iterator sit = avaseq.begin(); sit != avaseq.end(); ++sit){
				if(equal(jid, (*sit)->jid)){
					(*sit)->hearttimestamp = time(NULL);
					hasfindjid = true;
					break;
				}
			}
		}else{
			AvatarNSeq newseq;
			ans[userid] = newseq;
		}
		//MCE_INFO("TalkFunStateManagerI::HeartBeatNotSplit-->" << " jid:" << jidToString(jid) << " has avatars seq:" << hasfindjid);
		if(!hasfindjid){
			AvatarNSeqMap::iterator mit = ans.find(userid);
			if(mit != ans.end()){
				AvatarNPtr newavatar = new AvatarN();
				newavatar->jid = jid;
				newavatar->type = MemcGetOnlineType(jid->endpoint);
				newavatar->stat = MemcGetOnlineStat("");
				newavatar->hearttimestamp = time(NULL);
				newavatar->ver = "";
				newavatar->ext = "";
				mit->second.push_back(newavatar);
				MCE_INFO("TalkFunStateManagerI::HeartBeatNotSplit-->" << " jid:" << jidToString(jid) << " has avatars seq:" << hasfindjid << " new jid and push it, seq.size:" << mit->second.size());
			}
		}
	}
  FunStatManager::instance().Stat("TalkFunStateManagerI::HeartBeatNotSplit.Deal", tsdeal.getTime(), (tsdeal.getTime() >= TIMEOUT));
	TimeStatN tsset;
	OnlineCenterClient::instance().SetOnlineInfos(ans);
  FunStatManager::instance().Stat("TalkFunStateManagerI::HeartBeatNotSplit.Set", tsset.getTime(), (tsset.getTime() >= TIMEOUT));
  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::HeartBeatNotSplit", ts.getTime(), (ts.getTime() >= TIMEOUT));




	for(JidSeq::iterator it = seq.begin(); it != seq.end();++it){
		DoingCachePtr cacheptr = doingpool_[(*it)->userId % DOINGPOOLSIZE];
		cacheptr->EndWrite(*it, 0);
	}

}


/*
bool TalkFunStateManagerI::GetRightHeartUser(AvatarNSeq& ans){
	int now = time(NULL);
	size_t size = ans.size();
	bool haschange = false;
	int userid = 0;
	if(size){
		userid = ans.at(0)->jid->userId;
	}
	for(AvatarNSeq::iterator sit = ans.begin(); sit != ans.end();){
		if(((now - (*sit)->hearttimestamp) + 0.0) >= GAMEOVERTIME && ( !(*sit)->jid->endpoint.empty() ) && ('T' == (*sit)->jid->endpoint.at(0)) ){
			sit =  ans.erase(sit);
			haschange = true;
		MCE_INFO("TalkFunStateManagerI::GetRightHeartUser--> userid: " << userid << " size.befor:" << size << " size.after:" << ans.size());
		}else{
			++sit;
		}
	}
	if(size != ans.size()){
		MCE_INFO("TalkFunStateManagerI::GetRightHeartUser--> userid: " << userid << " size.befor:" << size << " size.after:" << ans.size());
	}
	return haschange;
}
*/

/* brief: 同GetUserOnlineInfo ，批量接口，接口已经废弃
 */
AvatarNSeqMap TalkFunStateManagerI::GetUsersOnlineInfo(const MyUtil::IntSeq& uids, bool checkheartbeat, const Ice::Current& current){
	TimeStatN ts;
	AvatarNSeqMap ans;
  if (!OnlineCenterClient::instance().GetUsers(ans, uids, usebackup_)) {
    MCE_WARN("TalkFunStateManagerI::GetUsersOnlineInfo --> get memc error size " << uids.size() << " bak " << usebackup_);
  }

  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::GetUsersOnlineInfo", ts.getTime(), (ts.getTime() >= TIMEOUT));
	return ans;
}
/* brief: 获取给定id的在线信息，该信息是新版的在线结构，直接存储在memcached中，该接口已经废弃
 * uids: 给定id
 * checkheartbeat: 废弃参数
 * return: 对应id的所有在线副本
 */
AvatarNSeq TalkFunStateManagerI::GetUserOnlineInfo(int userid, bool checkheartbeat, const Ice::Current& current){
	TimeStatN ts;
	AvatarNSeq ans;
	if (!OnlineCenterClient::instance().GetUser(ans, userid, usebackup_)) {
    MCE_WARN("TalkFunStateManagerI::GetUserOnlineInfo --> get memc error uid " << userid << " bak " << usebackup_);
  }

	MCE_INFO("OnlineCenterClient--> GetUser : ans.size:" << ans.size());

  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::GetUserOnlineInfo", ts.getTime(), (ts.getTime() >= TIMEOUT));
	return ans;
}

/* brief: 同GetUserOnlineInfo ，接口已经废弃
 */
int TalkFunStateManagerI::GetUserOnlineType(int userid, const Ice::Current& current){
	TimeStatN ts;
	AvatarNSeq ans;
  if (!OnlineCenterClient::instance().GetUser(ans, userid, usebackup_)) {
    MCE_WARN("TalkFunStateManagerI::GetUserOnlineType --> get memc error uid " << userid << " bak " << usebackup_);
  }

  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::GetUserOnlineType", ts.getTime(), (ts.getTime() >= TIMEOUT));
	return 1;
}

/* brief: 同GetUserOnlineInfo ，接口已经废弃
 */
int TalkFunStateManagerI::GetUserOnlineStat(int userid, const Ice::Current& current){
	TimeStatN ts;
	AvatarNSeq ans;
  if (!OnlineCenterClient::instance().GetUser(ans, userid, usebackup_)) {
    MCE_WARN("TalkFunStateManagerI::GetUserOnlineStat --> get memc error uid " << userid << " bak " << usebackup_);
  }

  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::GetUserOnlineStat", ts.getTime(), (ts.getTime() >= TIMEOUT));
	return 1;
}
/* brief: rpc接口已经废弃，有的都是内部调用
 */
void TalkFunStateManagerI::SetUserOnlineInfo(int userid, const AvatarNSeq& seq, const Ice::Current& current){
	if(seq.empty()){
		DeleteUser(userid);
		return;
	}
	TimeStatN ts;
	OnlineCenterClient::instance().SetOnlineInfo(userid, seq);
  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::SetUserOnlineInfo", ts.getTime(), (ts.getTime() >= TIMEOUT));
}

void TalkFunStateManagerI::DeleteUser(int userid){
	TimeStatN ts;
	MCE_INFO("TalkFunStateManagerI::DeleteUser-->userid:" << userid);
	OnlineCenterClient::instance().DeleteUser(userid);
  FunStatManager::instance().Stat("TalkFunStateManagerI::DeleteUser", ts.getTime(), (ts.getTime() >= TIMEOUT));
}

void TalkFunStateManagerI::DeleteUserJid(const JidPtr &jid) {
	TimeStatN ts;
	MCE_INFO("TalkFunStateManagerI::DeleteUserJid-->jid:" << jidToString(jid));
	if (!OnlineCenterClient::instance().DeleteUser(jid->userId)) {
    MCE_WARN("TalkFunStateManagerI::DeleteUserJid--> jid:" << jidToString(jid));
  }
  FunStatManager::instance().Stat("TalkFunStateManagerI::DeleteUserJid", ts.getTime(), (ts.getTime() >= TIMEOUT));

}
/* brief: 用户上线接口，会写对应用户的memacached，用户有上线动作时候会调用
 * avatarn: 在线副本
 * return void
 */
void TalkFunStateManagerI::AvatarNOnline(const AvatarNPtr& avatarn, const Ice::Current& current){

	
	TimeStatN ts;
  
	if("PIMStorm" == CheckEndpointType(avatarn->jid->endpoint)) {
		return;
	}
  
	
	
	DoingCachePtr cacheptr = doingpool_[avatarn->jid->userId % DOINGPOOLSIZE];
	bool ok = cacheptr->BeginWrite(avatarn->jid, 0);
	if(!ok){
		return;
	}



	AvatarNSeq seq;
	if (!OnlineCenterClient::instance().GetUser(seq, avatarn->jid->userId, usebackup_)) {
    MCE_WARN("TalkFunStateManagerI::AvatarNOnline --> get memc error uid " << avatarn->jid->userId << " bak " << usebackup_);
  }

	//MCE_INFO("TalkFunStateManagerI::AvatarNOnline--> userid:" << avatarn->jid->userId << " seq.size:" << seq.size());
	for(AvatarNSeq::iterator it = seq.begin(); it != seq.end();){
		if( equal ((*(it))->jid, avatarn->jid) ){
			avatarn->hearttimestamp = (*(it))->hearttimestamp;//还原时间戳
			//MCE_INFO("TalkFunStateManagerI::AvatarNOnline-->  equal it:" << jidToString((*it)->jid) << " avatarn->jid:" << jidToString(avatarn->jid));
      // 如果新的online没有带这些信息，则使用老的
      if (avatarn->ver.empty()) {
        avatarn->ver = (*(it))->ver;
      }
      if (avatarn->ext.empty()) {
        avatarn->ext = (*(it))->ext;
      }
			seq.erase(it);
			break;
		}else{
			//MCE_INFO("TalkFunStateManagerI::AvatarNOnline--> diff it:" << jidToString((*it)->jid) << " avatarn->jid:" << jidToString(avatarn->jid));
			++it;
		}
	}
	seq.push_back(avatarn);
	SetUserOnlineInfo(avatarn->jid->userId, seq);
	MCE_INFO("TalkFunStateManagerI::AvatarNOnline--> jid:" << jidToString(avatarn->jid) << " seq.size():" << seq.size());
  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::AvatarNOnline", ts.getTime(), (ts.getTime() >= TIMEOUT));
  StatEndpoint("ONLINE", avatarn->jid);

	if(ok){
		cacheptr->EndWrite(avatarn->jid, 0);
	}


}
void TalkFunStateManagerI::StatEndpoint(const char *fun, const JidPtr &jid) {
  char buff[100];

  const string eptype = CheckEndpointType(jid->endpoint);
  const char *endpoint = eptype.c_str();
  
  snprintf(buff, sizeof(buff), "StatEndpoint::%s@%s", fun, endpoint);
  FunStatManager::instance().Stat(buff, 0, false);
}
/* brief: 用户下线接口，会写对应用户的memacached，用户有下线动作时候会调用
 * avatarn: 用户在线jid
 * return void
 */
void TalkFunStateManagerI::AvatarNOffline(const JidPtr& jid, const Ice::Current& current){

	if("PIMStorm" == CheckEndpointType(jid->endpoint)) {
		return;
	}


	DoingCachePtr cacheptr = doingpool_[jid->userId % DOINGPOOLSIZE];
	bool ok = cacheptr->BeginWrite(jid, 1);
	if(!ok){
		return;
	}


	TimeStatN ts;
	AvatarNSeq seq;
	if (!OnlineCenterClient::instance().GetUser(seq, jid->userId, usebackup_)) {
    MCE_WARN("TalkFunStateManagerI::AvatarNOffline --> get memc error uid " << jid->userId << " bak " << usebackup_);
  }

	for(AvatarNSeq::iterator it = seq.begin(); it != seq.end();){
		if( equal((*(it))->jid, jid) ){
			seq.erase(it);
			break;
		}else{
			++it;
		}
	}
	if(!seq.empty()){
		SetUserOnlineInfo(jid->userId, seq);
	}
	else{
		DeleteUserJid(jid);
	}
  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::AvatarNOffline", ts.getTime(), (ts.getTime() >= TIMEOUT));
  StatEndpoint("OFFLINE", jid);
	MCE_INFO("TalkFunStateManagerI::AvatarNOffline--> jid:" << jidToString(jid) << " seq.size():" << seq.size());

	cacheptr->EndWrite(jid, 1);

}



/* brief: 废弃的rpc接口
 */
void TalkFunStateManagerI::ClearUserOnlineInfo(int userid, const Ice::Current& current){
	TimeStatN ts;
	DeleteUser(userid);
  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::ClearUserOnlineInfo", ts.getTime(), (ts.getTime() >= TIMEOUT));
}

