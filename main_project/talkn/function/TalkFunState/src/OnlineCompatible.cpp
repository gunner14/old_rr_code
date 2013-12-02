#include "TalkFunStateManagerI.h"
#include "XmppTools.h"
#include "FeedMemcProxy/client/online_cache_client.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "CommonTask.h"
#include "TaskManager.h"
#include "BuddyByIdReplicaAdapter.h"
#include "IMStormAdapter.h"
#include "TalkFunPhoneStateAdapter.h"

//using namespace com::xiaonei::talk::common;
using namespace xce::buddy::adapter;
using namespace com::xiaonei::talk::util;
using namespace xce::talk::online;
using namespace com::xiaonei::talk::fun::state;
using namespace xce::talk;
using namespace MyUtil;

static void AvatarN2Avatar(const AvatarNPtr& avatarn, const AvatarPtr& ans){
	if(avatarn && ans){
		ans->jid = avatarn->jid;
		OnlineStat stat;
		if(avatarn->stat == 1){
			ans->stat = ONLINE;
		}
		else if(avatarn->stat == 2){
			ans->stat = AWAY;
		}
		else if(avatarn->stat == 3){
			ans->stat = BUSY;
		}
		else if(avatarn->stat == 4){
			ans->stat = NOTAVAILABLE;
		}
		else if(avatarn->stat == 5){
			ans->stat = INVISIABLE;
		}
		else {
			ans->stat = ONLINE; 
		}
		ans->ver = avatarn->ver;
		ans->ext = avatarn->ext;
		ans->stamp = time(NULL); // TODO 下线时间戳，是否要兼容
	}
}
int CalculateType(AvatarNSeq & ans) {
  int onlineType = 0;
  AvatarPtr res = new Avatar();
  int onlineStat_tmp = ONLINE;
  for (AvatarNSeq::iterator sit = ans.begin();sit!=ans.end();++sit) {
    AvatarN2Avatar((*sit),res);
    char tmp = res->jid->endpoint.at(0);
    switch (tmp) {
    case 'W':
      onlineType |= WP_ONLINE;
      break;
    case 'H':
      onlineType |= WP_ONLINE;
      break;
    case 'T':
      onlineType |= IM_ONLINE;
      break;
    case 'P':
      onlineType |= PHONE_ONLINE;
      break;

    default:
      break;
    }
    if (res->stat < onlineStat_tmp) {
      onlineStat_tmp = res->stat;
    }
  }
  if (onlineStat_tmp == INVISIABLE)
    onlineType = UNKNOWTYPE;
  return onlineType;
}
/* brief: 过滤给定id序列在线的用户，并同时返回在线类型
 * ids: 给定对id序列
 * return: 过滤结果，以及用户的在线类型
 */
UserOnlineTypeSeq TalkFunStateManagerI::getOnlineUsers(const MyUtil::IntSeq& ids,const Ice::Current& current) {
  TimeStatN ts;
  AvatarNSeqMap ans;
  if (!OnlineCenterClient::instance().GetUsers(ans, ids, usebackup_)) {
    MCE_WARN("TalkFunStateManagerI::getOnlineUsers--> get memc error size:"<<ids.size() << "bak:"<<usebackup_);
  }
  AvatarNSeq aseq;
  get3GAvatarNs(ids, aseq);
  for (AvatarNSeq::iterator it = aseq.begin(); it != aseq.end(); ++it) {
    if (ans.find((*it)->jid->userId) == ans.end()) {
      ans[(*it)->jid->userId] = AvatarNSeq();
    }
    ans[(*it)->jid->userId].push_back(*it);
  }

  UserOnlineTypeSeq res;
  for (AvatarNSeqMap::iterator sit = ans.begin();sit!=ans.end();++sit) {
    int tmp = CalculateType(sit->second);
    if (tmp>0) {
      UserOnlineTypePtr info = new UserOnlineType();
      info->uid = sit->first;
      info->type = tmp;
      res.push_back(info);
    }
  }
  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::getOnlineUsers", ts.getTime(), (ts.getTime() >= TIMEOUT));
  return res;
}


static AvatarPtr get_old_avatar(const AvatarNPtr& avatarn) {
  AvatarPtr av = new Avatar();
  AvatarN2Avatar(avatarn, av);
  return av;
}


// 对pager在线的暂时先进行特殊处理一下
static JidPtr get_pager_jid(const JidPtr &jid) {

  if (jid
      && !jid->endpoint.empty()
      && jid->endpoint.at(0) == 'H') {

    char buff[20];
    JidPtr wjid = new Jid();
    wjid->userId = jid->userId;
    wjid->index = 0;
    snprintf(buff, sizeof(buff), "WTalkProxy%d", jid->userId % 10);
    wjid->endpoint = buff;
    return wjid;
  } else {
    return 0;
  }

}
// 获取和老版本兼容的jid，主要处理wtalkproxy问题
static void get_old_jid(JidPtr &jid) {
  if (jid
      && !jid->endpoint.empty()
      && jid->endpoint.at(0) == 'H') {
    jid = get_pager_jid(jid);
  }

}
void TalkFunStateManagerI::get3GAvatarNs(const MyUtil::IntSeq& uids, AvatarNSeq &ans) {
  JidSeq jseq;
  get3GJids(uids, jseq);

  for (JidSeq::iterator it = jseq.begin(); it != jseq.end(); ++it) {
    AvatarNPtr ava = new AvatarN();
		ava->jid = *it;
		ava->ext = "phone";
		ava->ver = "";
		ava->stat = 1;
		ava->type = 8;
		ava->hearttimestamp = time(NULL);
    ans.push_back(ava);
  }

}

void TalkFunStateManagerI::get3GAvatars(const MyUtil::IntSeq& uids, AvatarSeq &ans) {
  JidSeq jseq;
  get3GJids(uids, jseq);

  for (JidSeq::iterator it = jseq.begin(); it != jseq.end(); ++it) {
    AvatarPtr ava = new Avatar();
		ava->jid = *it;
		ava->ext = "phone";
		ava->ver = "";
		ava->stat = ONLINE;

    ans.push_back(ava);
  }


}

void TalkFunStateManagerI::get3GJids(const MyUtil::IntSeq& uids, JidSeq &ans) {
	TimeStatN ts;
  const char *fun = "TalkFunStateManagerI::get3GJids";
  IntSeq res;

  try {
    TalkFunPhoneStateAdapter::instance().getUsersIds(uids, res);
  } catch (Ice::Exception& e) {
    MCE_WARN(fun << "-->TalkFunPhoneStateAdapter.instance uids:"<< uids.size());
  }

  char buff[20];
  for (IntSeq::const_iterator it = res.begin(); it != res.end(); ++it) {
    JidPtr j = new Jid();
    j->userId = *it;
    snprintf(buff, sizeof(buff), "PIMStorm%d", *it % IMStormAdapter::instance().getCluster());
    j->endpoint = buff;
    j->index = 2;
    ans.push_back(j);
  }
  FunStatManager::instance().Stat(fun, ts.getTime(), (ts.getTime() >= TIMEOUT));
}


bool TalkFunStateManagerI::getUserAvatars_(int userId, int onlineType, AvatarSeq &ans) {
	AvatarNSeq seq;
	if (!OnlineCenterClient::instance().GetUser(seq, userId, usebackup_)) {
    MCE_WARN("TalkFunStateManagerI::getUserAvatars_ --> get memc error uid " << userId << " bak " << usebackup_);
    return false;
  }
  // 提取指定类型的
  bool isnot_have_wtalk = true; // one uid in one WTalkProxy only!
	for (AvatarNSeq::iterator it = seq.begin(); it != seq.end(); ++it) {
    string ckend = CheckEndpointType((*it)->jid->endpoint);
		if(((*it)->type & onlineType)){
			ans.push_back(get_old_avatar(*it));
		} else if ((16 & onlineType) && ckend == "PTalkProxy") {
      ans.push_back(get_old_avatar(*it));
    }

    else if (isnot_have_wtalk && (2 & onlineType)) {
      JidPtr jid = get_pager_jid((*it)->jid);
      if (jid) {
        (*it)->jid = jid;
        ans.push_back(get_old_avatar(*it));
        isnot_have_wtalk = false;
      }
    }

  }

  if ((8 & onlineType) || (32 & onlineType)) {
    AvatarSeq aseq;
    IntSeq ids;
    ids.push_back(userId);
    get3GAvatars(ids, aseq);
    ans.insert(ans.end(), aseq.begin(), aseq.end());

  }

  return true;
}

bool TalkFunStateManagerI::getUsersAvatars_(const MyUtil::IntSeq& ids, int onlineType, AvatarMap &ans) {
	AvatarNSeqMap seq;
  if (!OnlineCenterClient::instance().GetUsers(seq, ids, usebackup_)) {
    MCE_WARN("TalkFunStateManagerI::getUsersAvatars_ --> get memc error size " << ids.size() << " bak " << usebackup_);
    return false;
  }

  bool isnot_have_wtalk = true;
	for(AvatarNSeqMap::iterator it = seq.begin(); it != seq.end(); ++it){
    AvatarSeq aseq;
    isnot_have_wtalk = true;
		for(AvatarNSeq::iterator sit = it->second.begin(); sit != it->second.end(); ++sit){
      string ckend = CheckEndpointType((*sit)->jid->endpoint);
			if(((*sit)->type & onlineType)){
				aseq.push_back(get_old_avatar(*sit));
			} else if ((16 & onlineType) && ckend == "PTalkProxy") {
        aseq.push_back(get_old_avatar(*sit));
      }

      else if (isnot_have_wtalk && (2 & onlineType)) {
        JidPtr jid = get_pager_jid((*sit)->jid);
        if (jid) {
          (*sit)->jid = jid;
          aseq.push_back(get_old_avatar(*sit));
          isnot_have_wtalk = false;
        }
      }

		}
		ans[it->first] = aseq;
	}

  if ((8 & onlineType) || (32 & onlineType)) {
    AvatarSeq aseq;
    get3GAvatars(ids, aseq);
    for (AvatarSeq::iterator it = aseq.begin(); it != aseq.end(); ++it) {
      if (ans.find((*it)->jid->userId) == ans.end()) {
        ans[(*it)->jid->userId] = AvatarSeq();
      }
      ans[(*it)->jid->userId].push_back(*it);
    }
  }

  return true;

}

bool TalkFunStateManagerI::getUserJids_(int userId, int onlineType, JidSeq &ans) {
	AvatarNSeq seq;
	if (!OnlineCenterClient::instance().GetUser(seq, userId, usebackup_)) {
    MCE_WARN("TalkFunStateManagerI::getUserJids_ --> get memc error uid " << userId << " bak " << usebackup_);
    return false;
  }

  bool isnot_have_wtalk = true;
	for(AvatarNSeq::iterator it = seq.begin(); it != seq.end(); ++it){
    string ckend = CheckEndpointType((*it)->jid->endpoint);
		if(((*it)->type & onlineType)){
			ans.push_back((*it)->jid);
		} else if ((16 & onlineType) && ckend == "PTalkProxy") {
      ans.push_back((*it)->jid);
    }


    else if (isnot_have_wtalk && (2 & onlineType)) {
      JidPtr jid = get_pager_jid((*it)->jid);
      if (jid) {
        ans.push_back(jid);
        isnot_have_wtalk = false;
      }
    }
    
	}
  if ((8 & onlineType) || (32 & onlineType)) {
    JidSeq jseq;
    IntSeq ids;
    ids.push_back(userId);
    get3GJids(ids, jseq);
    ans.insert(ans.end(), jseq.begin(), jseq.end());

  }


  return true;

}

/////////////////////////兼容老接口////////////////////////////////////
/* brief: 获取给定用户，对应在线类型下的在线副本
 * userId: 给定用户
 * onlineType: 获取的在线类型
 * return: 符合过滤条件的在线副本
 */
JidSeq TalkFunStateManagerI::getUserJids(int userId, int onlineType, const Ice::Current& current){
	TimeStatN ts;
	JidSeq ans;
  getUserJids_(userId, onlineType, ans);
  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::getUserJids", ts.getTime(), (ts.getTime() >= TIMEOUT));
	return ans;
}

bool TalkFunStateManagerI::getUsersJids_(const MyUtil::IntSeq& ids, int onlineType, JidSeqMap &ans) {
	AvatarNSeqMap seq;
  if (!OnlineCenterClient::instance().GetUsers(seq, ids, usebackup_)) {
    MCE_WARN("TalkFunStateManagerI::getUsersJids_ --> get memc error size " << ids.size() << " bak " << usebackup_);
    return false;
  }

  bool isnot_have_wtalk = true;
	for(AvatarNSeqMap::iterator it = seq.begin(); it != seq.end(); ++it){
		JidSeq jseq;
    isnot_have_wtalk = true;
		for(AvatarNSeq::iterator sit = it->second.begin(); sit != it->second.end(); ++sit){
      string ckend = CheckEndpointType((*sit)->jid->endpoint);
			if(((*sit)->type & onlineType)){
				jseq.push_back((*sit)->jid);
			} else if ((16 & onlineType) && ckend == "PTalkProxy") {
				jseq.push_back((*sit)->jid);
      }

      else if (isnot_have_wtalk && (2 & onlineType)) {
        JidPtr jid = get_pager_jid((*sit)->jid);
        if (jid) {
          jseq.push_back(jid);
          isnot_have_wtalk = false;
        }
      }

		}
		ans[it->first] = jseq;
	}

  if ((8 & onlineType) || (32 & onlineType)) {
    JidSeq jseq;
    get3GJids(ids, jseq);
    for (JidSeq::iterator it = jseq.begin(); it != jseq.end(); ++it) {
      if (ans.find((*it)->userId) == ans.end()) {
        ans[(*it)->userId] = JidSeq();
      }
      ans[(*it)->userId].push_back(*it);
    }
  }

  return true;

}



/* brief: 获取给定用户，对应在线类型下的在线副本
 * ids: 给定用户序列
 * onlineType: 获取的在线类型
 * return: 符合过滤条件的在线副本map
 */
JidSeqMap TalkFunStateManagerI::getUsersJids(const MyUtil::IntSeq& ids, int onlineType, const Ice::Current& current){
	TimeStatN ts;
	JidSeqMap ans;
  getUsersJids_(ids, onlineType, ans);
  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::getUsersJids", ts.getTime(), (ts.getTime() >= TIMEOUT));
	return ans;
}

/* brief: 获取符合jid的avatar
 * jid: 给定jid
 * return: 符合该jid的avatar信息
 */
AvatarPtr TalkFunStateManagerI::getAvatar(const JidPtr& jid, const Ice::Current& current){
	TimeStatN ts;
	AvatarPtr ans = new Avatar();
	AvatarNSeq seq;
	if (!OnlineCenterClient::instance().GetUser(seq, jid->userId, usebackup_)) {
    MCE_WARN("TalkFunStateManagerI::getAvatar --> get memc error uid " << jid->userId << " bak " << usebackup_);
  }

	AvatarNPtr avatarn;
	for(AvatarNSeq::iterator it = seq.begin(); it != seq.end(); ++it){
    get_old_jid((*it)->jid);
		if( equal((*it)->jid, jid)){
			avatarn = *it;
			break;
		}
	}
	if(avatarn){
		AvatarN2Avatar(avatarn, ans);
	}
  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::getAvatar", ts.getTime(), (ts.getTime() >= TIMEOUT));
	return ans;
}


/* brief: 同getAvatar，批量接口
 */
AvatarSeq TalkFunStateManagerI::getAvatars(const JidSeq& jids, const Ice::Current& current){
	TimeStatN ts;
	AvatarSeq ans;
	if(jids.empty()){
		return ans;
	}
	IntSeq ids;
	for(JidSeq::const_iterator it = jids.begin(); it != jids.end(); ++it){
		ids.push_back((*it)->userId);
	}

	AvatarNSeqMap seq;
  if (!OnlineCenterClient::instance().GetUsers(seq, ids, usebackup_)) {
    MCE_WARN("TalkFunStateManagerI::getAvatars --> get memc error size " << ids.size() << " bak " << usebackup_);
  }


	for(JidSeq::const_iterator it = jids.begin(); it != jids.end(); ++it){
    //MCE_DEBUG("TalkFunStateManagerI::getAvatars --> check jid:" << jidToString(*it));
		int uid = (*it)->userId;
		AvatarNSeqMap::iterator mit = seq.find(uid);
		if(mit != seq.end()){
			AvatarPtr tmp_ans;
			AvatarNPtr avatarn_ptr;
			for(AvatarNSeq::iterator sit = mit->second.begin(); sit != mit->second.end(); ++sit){
        //MCE_DEBUG("TalkFunStateManagerI::getAvatars --> new jid:" << jidToString((*sit)->jid));
        get_old_jid((*sit)->jid);
        //MCE_DEBUG("TalkFunStateManagerI::getAvatars --> old jid:" << jidToString((*sit)->jid));
				if( equal((*it), (*sit)->jid) ){

					avatarn_ptr = *sit;
					break;
				}
			}
			if(avatarn_ptr){
        tmp_ans = get_old_avatar(avatarn_ptr);
				if(tmp_ans){
					ans.push_back(tmp_ans);
				}
			}
		}
	}
  //MCE_DEBUG("TalkFunStateManagerI::getAvatars --> seq.size:" << seq.size() << " ans.size:" << ans.size() << " jids.size:" << jids.size() << " bak " << usebackup_);
  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::getAvatars", ts.getTime(), (ts.getTime() >= TIMEOUT));
	return ans;

}

/* brief: 同getUserJids，只是取的是avatar，不是jid
 */
AvatarSeq TalkFunStateManagerI::getUserAvatars(int userId, int onlineType, const Ice::Current& current){
	TimeStatN ts;
	AvatarSeq ans;
  getUserAvatars_(userId, onlineType, ans);
  /*
	AvatarNSeq seq;
	if (!OnlineCenterClient::instance().GetUser(seq, userId, usebackup_)) {
    MCE_WARN("TalkFunStateManagerI::getUserAvatars --> get memc error uid " << userId << " bak " << usebackup_);
  }

	for(AvatarNSeq::iterator it = seq.begin(); it != seq.end(); ++it){
		AvatarPtr tmp_ans = new Avatar();
		AvatarN2Avatar((*it), tmp_ans);
		if(tmp_ans){
			ans.push_back(tmp_ans);
		}
	}
  */
  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::getUserAvatars", ts.getTime(), (ts.getTime() >= TIMEOUT));
	return ans;
}

/* brief: 同getUsersJids，只是取的是avatar，不是jid
 */
AvatarMap TalkFunStateManagerI::getUsersAvatars(const MyUtil::IntSeq& ids, int onlineType, const Ice::Current& current){
	TimeStatN ts;
	AvatarMap ans;
  getUsersAvatars_(ids, onlineType, ans);
  /*
	AvatarNSeqMap seq;
  if (!OnlineCenterClient::instance().GetUsers(seq, ids, usebackup_)) {
    MCE_WARN("TalkFunStateManagerI::getUsersAvatars --> get memc error size " << ids.size() << " bak " << usebackup_);
  }

	
	for(AvatarNSeqMap::iterator mit = seq.begin(); mit != seq.end(); ++mit){
		AvatarSeq tmp_ans;
		for(AvatarNSeq::iterator sit = mit->second.begin(); sit != mit->second.end(); ++sit){
			AvatarPtr ptr = new Avatar();
			AvatarN2Avatar( (*sit), ptr);
			if(ptr){
				tmp_ans.push_back(ptr);
			}
		}
		ans[mit->first] = tmp_ans;
	}
  */
  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::getUsersAvatars", ts.getTime(), (ts.getTime() >= TIMEOUT));
	return ans;
}

/* brief: 类似getUserJids，只是取的是好友的和自己jid信息
 * buddyOnlineType: 需要获取好友的在线类型条件
 * selfOnlineType： 需要获取自己的在线类型条件
 * userId: actor
 * return: 同getUserJids
 */
JidSeqMap TalkFunStateManagerI::getBuddyJidsAndSelf(int userId, int buddyOnlineType, int selfOnlineType, const Ice::Current& current){
	TimeStatN ts;
	IntSeq ids;
	ids = BuddyByIdCacheAdapter::instance().getFriendListAsc(userId, 5000);
	if(ids.empty()){
		return JidSeqMap();
	}

	JidSeqMap ans;
  getUsersJids_(ids, buddyOnlineType, ans);	
	JidSeq self_ans;
  getUserJids_(userId, selfOnlineType, self_ans);

	if(!self_ans.empty()){
		ans[userId] = self_ans;
	}
  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::getBuddyJidsAndSelf", ts.getTime(), (ts.getTime() >= TIMEOUT));
	return ans;

}
/* brief: 类似getUsersJids，只是id序列不是给定的，而是直接获取的actor的好友
 * onlineType: 在线类型过滤条件
 * userId: actor
 * return: 同getUserJids
 */
JidSeqMap TalkFunStateManagerI::getBuddyJids(int userId, int onlineType, const Ice::Current& current){
	TimeStatN ts;
	IntSeq ids;
	ids = BuddyByIdCacheAdapter::instance().getFriendListAsc(userId, 5000);
	if(ids.empty()){
		return JidSeqMap();
	}

	JidSeqMap ans;
  getUsersJids_(ids, onlineType, ans);
  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::getBuddyJids", ts.getTime(), (ts.getTime() >= TIMEOUT));
	return ans;	
}

/* brief: 类似getBuddyJids,只是获取的avatars信息
 */
AvatarMap TalkFunStateManagerI::getBuddyAvatars(int userId, int onlineType,const Ice::Current& current){
	TimeStatN ts;
	IntSeq ids;
	ids = BuddyByIdCacheAdapter::instance().getFriendListAsc(userId, 5000);
	if(ids.empty()){
		return AvatarMap();
	}

	AvatarMap ans;
  getUsersAvatars_(ids, onlineType, ans);	
  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::getBuddyAvatars", ts.getTime(), (ts.getTime() >= TIMEOUT));
	return ans;	
	
}

/* brief: 类似getBuddyJidsAndSelf,只是获取的avatars信息
 */
AvatarMap TalkFunStateManagerI::getBuddyAvatarsAndSelf(int userId, int buddyOnlineType, int selfOnlineType, const Ice::Current& current){
	TimeStatN ts;
	IntSeq ids;
	ids = BuddyByIdCacheAdapter::instance().getFriendListAsc(userId, 5000);
	if(ids.empty()){
		return AvatarMap();
	}

	AvatarMap ans;
  getUsersAvatars_(ids, buddyOnlineType, ans);
	AvatarSeq self_ans;
  getUserAvatars_(userId, selfOnlineType, self_ans);

	if(!self_ans.empty()){
		ans[userId] = self_ans;
	}
  FunStatManager::instance().StatPlus(current, "TalkFunStateManagerI::getBuddyAvatarsAndSelf", ts.getTime(), (ts.getTime() >= TIMEOUT));
	return ans;
}





