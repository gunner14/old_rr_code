#include "online_cache_client.h"
#include <boost/lexical_cast.hpp>
#include "online.pb.h"

#include "ServiceI.h"
#include "LogWrapper.h"

namespace xce {
namespace talk {
namespace online{

using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::online;

static void GetUserKey(Ice::Long fid, char * key, size_t len) {
  snprintf(key, len, "IMONLINE#%ld", fid);  
  key[len - 1] = 0;
}

static void GetUserBackupKey(Ice::Long fid, char * key, size_t len){
  snprintf(key, len, "IMONLINEBACKUP#%ld", fid);  
  key[len - 1] = 0;
}

Ice::Long GetUserId(const char * key, size_t len) {
	bool backup = false;
	if(len >= 9 && strncmp("IMONLINE#", key, 9) == 0 ){
		backup = false;
	}else if (len >= 15 && strncmp("IMONLINEBACKUP#", key, 15) == 0){
		backup = true;
	}else{
		return -1;
	}
	//if(len == >= 9 && strncmp("IMONLINE#", key, 9) == 0 )
  //if (len < 9 || strncmp("IMONLINE#", key, 9) != 0) {
	//	if(len < 15 || strncmp("IMONLINEBACKUP#", key, 15) != 0){
	//		return -1;
	//	}
	//	backup = true;
  //}
	
  Ice::Long id = -1;
  try {
		if(!backup){
			id = boost::lexical_cast<Ice::Long>(string(key + 9, len - 9));
		}
		else{
			id = boost::lexical_cast<Ice::Long>(string(key + 15, len - 15));
		}
  } catch(boost::bad_lexical_cast &) {
    return -1;
  }
  return id;
}


static void MemcOnlineInfoToAvatarN(const McOnlineInfo& memc_online_info, AvatarNSeq& aseq) {
	int size= memc_online_info.avatar_size();
  int tm_now = time(NULL);
	for(int i = 0; i < size ; ++i){
		McAvatar mcavatar = memc_online_info.avatar(i);
		McJid mcjid = mcavatar.jid();

    if (tm_now - mcavatar.hearttimestamp() > 600) {
      MCE_INFO("MemcOnlineInfoToAvatarN --> timeout intv:" << tm_now - mcavatar.hearttimestamp() 
               << " heart:" << mcavatar.hearttimestamp()
               << " uid:" << mcjid.userid() << " index:" << mcjid.index() << " endpoint:" << mcjid.endpoint());
      continue;
    }

		AvatarNPtr avatarn = new AvatarN();

		JidPtr jid = new Jid();
		jid->userId = mcjid.userid();
		jid->index = mcjid.index();
		jid->endpoint = mcjid.endpoint();
    // 过滤PIMStorm的endpoint
    if (CheckEndpointType(jid->endpoint) == "PIMStorm")
      continue;
		avatarn->jid = jid;
		avatarn->stat = mcavatar.state();
		avatarn->type = mcavatar.type();
		avatarn->ver = mcavatar.ver();
		avatarn->ext = mcavatar.ext();
		avatarn->hearttimestamp = mcavatar.hearttimestamp();
		aseq.push_back(avatarn);
	}
  if (aseq.size() != 0 && size != aseq.size()) {
    MCE_INFO("MemcOnlineInfoToAvatarN --> size before:" << size << " after:" << aseq.size());
  }

}

OnlineCenterClient::OnlineCenterClient() {
  pool_.Init(250, "im");
}

OnlineCenterClient::~OnlineCenterClient() {
}

bool OnlineCenterClient::GetUsers(AvatarNSeqMap &dict, const MyUtil::IntSeq& ids, bool backup) {
  float cost_mem;
  float cost_decom;
  vector<string> uids;
	if(ids.empty()){
		return true;
	}
  for(size_t i=0; i<ids.size(); i++){
    char data[64] = {0};
		if(!backup){
			snprintf(data, 64, "IMONLINE#%d", ids.at(i));
		}else{
			snprintf(data, 64, "IMONLINEBACKUP#%d", ids.at(i));
		}
    uids.push_back(data);
  }
  pair<int, MemcachedClient*> cli = pool_.GetClient(ids.at(0)); 
  if(!cli.second) {
    MCE_WARN("OnlineCenterClient::GetUsers-->get memc error size:" << ids.size());
    return false;
  }
  map<string, string> res;
  map<string, int> flags;
  TimeStat ts;
  bool succ = cli.second->Get(uids, res, flags);
  pool_.ReleaseClient(cli.first, cli.second);
  if (!succ) {
    MCE_WARN("OnlineCenterClient::GetUsers-->get memc data error size:" << ids.size());
    return false;
  }
  cost_mem = ts.getTime();
  ts.reset();
  map<string, string>::iterator it = res.begin();
  for(; it!=res.end(); ++it){
		int uid = GetUserId(it->first.c_str(), it->first.size());
		McOnlineInfo memc_online_info;
    memc_online_info.ParseFromArray(it->second.c_str(), it->second.size());
		AvatarNSeq aseq;
    MemcOnlineInfoToAvatarN(memc_online_info, aseq);
		if(!aseq.empty()){
    	dict.insert(make_pair(uid, aseq));
		}
  }
  cost_decom = ts.getTime();

  MCE_INFO("MemcOnlineCenter::GetUsers --> ids:" << ids.size() << " res:" << dict.size() << " cost_mem:" << cost_mem << " cost_par:" << cost_decom << " cost_total:" << (cost_mem+cost_decom) << " backup:" << backup);
  return true;

}



bool OnlineCenterClient::GetUser(AvatarNSeq &ans, int user_id, bool backup) {
  MyUtil::IntSeq ids;
  ids.push_back(user_id);
  AvatarNSeqMap dict;
  if (!GetUsers(dict, ids, backup)) {
    return false;
  }
	if(dict.find(user_id) != dict.end()){
		ans.swap(dict[user_id]);
	} else {
    ans.clear();
  }
  return true;
}



// 返回值, 标识是否压缩了
bool OnlineCenterClient::SerializeAvatarNSeq(const AvatarNSeq& aseq, string * serialized) {
  if (aseq.empty()) {
    return false;
  }

	McOnlineInfo memc_online_info;

	for(AvatarNSeq::const_iterator sit = aseq.begin(); sit != aseq.end(); ++sit){
		McAvatar* mcavatar = memc_online_info.add_avatar();	
		if(!mcavatar){
			continue;
		}
		McJid* mcjid = mcavatar->mutable_jid();
		if(!mcjid){
			continue;
		}
		AvatarNPtr avatarn = *sit;
		mcjid->set_userid(avatarn->jid->userId);
		mcjid->set_index(avatarn->jid->index);
		mcjid->set_endpoint(avatarn->jid->endpoint);
		mcavatar->set_hearttimestamp(avatarn->hearttimestamp);
		mcavatar->set_type(avatarn->type);
		mcavatar->set_state(avatarn->stat);
		mcavatar->set_ver(avatarn->ver);
		mcavatar->set_ext(avatarn->ext);
	}
  memc_online_info.SerializeToString(serialized);
  return true;
}

void OnlineCenterClient::SetOnlineInfo(int userid, const AvatarNSeq& aseq) {
  char key[32];
  char keybu[32];
  GetUserKey(userid, key, 32);
  GetUserBackupKey(userid, keybu, 32);
  string value;
  bool b = SerializeAvatarNSeq(aseq, &value);
  pair<int, MemcachedClient*> cli = pool_.GetClient(userid); 
  if(!cli.second){
    MCE_WARN("OnlineCenterClient::SetOnlineInfo-->get memc error uid:" << userid);
    return;
  }
  bool res = cli.second->Set(key, value, 0);
  pool_.ReleaseClient(cli.first, cli.second);

	
	
  pair<int, MemcachedClient*> clibu = pool_.GetClient(userid); 
  if(!clibu.second){
    MCE_WARN("OnlineCenterClient::SetOnlineInfo-->get bak memc error uid:" << userid);
    return;
  }
	bool res2 = clibu.second->Set(keybu, value, 0);
  pool_.ReleaseClient(clibu.first, clibu.second);
	MCE_DEBUG("OnlineCenterClient::SetOnlineInfo-->userid: " << userid <<" res1:" << res << " res2:" << res2);
  //return res;

}
void OnlineCenterClient::SetOnlineInfos(const AvatarNSeqMap& map){
	for(AvatarNSeqMap::const_iterator it = map.begin(); it != map.end(); ++it){
		SetOnlineInfo(it->first, it->second);
	}
}
bool OnlineCenterClient::DeleteUser(int userid){
  char key[32];
  char keybu[32];
  GetUserKey(userid, key, 32);
  GetUserBackupKey(userid, keybu, 32);
  pair<int, MemcachedClient*> cli = pool_.GetClient(userid); 
  if(!cli.second){
    MCE_WARN("OnlineCenterClient::DeleteUser --> memc get error:" << key);
    return false;
  }
  bool res = cli.second->Delete(key);
  if (!res) {
    MCE_WARN("OnlineCenterClient::DeleteUser --> Delete error key:" << key);
  }

  res = cli.second->Delete(keybu);
  if (!res) {
    MCE_WARN("OnlineCenterClient::DeleteUser --> bakup Delete error keybu:" << keybu);
  }
  pool_.ReleaseClient(cli.first, cli.second);
  return res;

/*
  pair<int, MemcachedClient*> clibu = pool_.GetClient(userid); 
  if(!clibu.second){
    return;
  }
  res = clibu.second->Delete(keybu);
  pool_.ReleaseClient(clibu.first, clibu.second);
	*/
}



}
}
}

