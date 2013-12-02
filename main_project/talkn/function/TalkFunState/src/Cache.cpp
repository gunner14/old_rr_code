#include "Cache.h"
#include "TalkFunStateManagerI.h"
#include "XmppTools.h"
using namespace com::xiaonei::talk::fun::state;
using namespace com::xiaonei::talk::util;



void DoingCache::DealDelay(){
	ActionInfoSeq delays;
	{
		IceUtil::RWRecMutex::WLock lock(rwmutex_);
		delays.swap(delays_);
	}
	for(ActionInfoSeq::iterator it = delays.begin(); it != delays.end(); ++it){
			
		int actiontype = (*it).actiontype_;
		JidPtr jid = (*it).jid_;

    if( 0 == actiontype ){

      AvatarNPtr ava = new AvatarN();
      ava->jid = jid;
      ava->ext = "";
      ava->ver = "";
      ava->type=MemcGetOnlineType(jid->endpoint);
      ava->stat=MemcGetOnlineStat("");
      ava->hearttimestamp = time(NULL);

      TalkFunStateManagerI::instance().AvatarNOnline(ava);
    }else if ( 1 == actiontype){
      TalkFunStateManagerI::instance().AvatarNOffline(jid);
    }

    MCE_INFO("DoingCache::DealDelay-->jid:" << jidToString(jid) << " actiontype:" << actiontype);
  }

}


bool DoingCache::BeginWrite(const JidPtr& jid, int actiontype){
  return true;
	IceUtil::RWRecMutex::WLock lock(rwmutex_);

	int uid = jid->userId;
	bool check = Check(jid, actiontype); //检查成功，说明这个事件可以执行 检查结果当做返回结果
	//MCE_INFO("DoingCache::BeginWrite-->uid:" << uid << " actiontype:" << actiontype << " jid:" << jidToString(jid) <<  " uids_.size:" << uids_.size() << " reuse.size:" << delays_.size() << " checkok:" << check);

	
	if(check){
		uids_.insert(make_pair<int, int>(uid, actiontype));
	}else{
		MCE_INFO("DoingCache::BeginWrite-->push delays uid:" << uid << " actiontype:" << actiontype << " jid:" << jidToString(jid) <<  " uids_.size:" << uids_.size() << " reuse.size:" << delays_.size() << " checkok:" << check);
		delays_.push_back(ActionInfo(jid, actiontype));
	}
	return check;
}

void DoingCache::EndWrite(const JidPtr& jid, int actiontype){
  return;
	IceUtil::RWRecMutex::WLock lock(rwmutex_);
	int uid = jid->userId;
	Uid2Action::iterator it = uids_.find(uid); // 解除这个副本的事件
	if(it != uids_.end()){
		uids_.erase(it);
	}else{
		//WARN
	}
}

bool DoingCache::Check(const JidPtr& jid, int actiontype){ // 动作检查l
  return true;
	int uid = jid->userId;
	Uid2Action::iterator it = uids_.find(uid);
	if(it == uids_.end()){ // 如果这个用户没有任何行为,则允许操作;
		return true;
	}
	MCE_INFO("DoingCache::Check--> uid:" << uid << " actiontype:" << actiontype << " it->second:" << it->second);
	return false;
}


