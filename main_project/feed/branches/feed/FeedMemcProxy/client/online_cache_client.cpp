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

Ice::Long GetUserId(const char * key, size_t len) {
  if (len < 9 || strncmp("IMONLINE#", key, 9) != 0) {
    return -1;
  }

  Ice::Long id = -1;
  try {
    id = boost::lexical_cast<Ice::Long>(string(key + 9, len - 9));
  } catch(boost::bad_lexical_cast &) {
    return -1;
  }
  return id;
}


static void MemcOnlineInfoToAvatarN(const McOnlineInfo& memc_online_info, AvatarNSeq& aseq) {
	int size= memc_online_info.avatar_size();
	for(int i = 0; i < size ; ++i){
		AvatarNPtr avatarn = new AvatarN();
		McAvatar mcavatar = memc_online_info.avatar(i);
		McJid mcjid = mcavatar.jid();
		JidPtr jid = new Jid();
		jid->userId = mcjid.userid();
		jid->index = mcjid.index();
		jid->endpoint = mcjid.endpoint();
		avatarn->jid = jid;
		avatarn->stat = mcavatar.state();
		avatarn->type = mcavatar.type();
		avatarn->ver = mcavatar.ver();
		avatarn->ext = mcavatar.ext();
		avatarn->hearttimestamp = mcavatar.hearttimestamp();
		aseq.push_back(avatarn);
	}
}

OnlineCenterClient::OnlineCenterClient() {
  pool_.Init(100);
}

OnlineCenterClient::~OnlineCenterClient() {
}

AvatarNMap OnlineCenterClient::GetUsers(const MyUtil::IntSeq& ids){
  float cost_mem;
  float cost_decom;
  vector<string> uids;
  AvatarNMap dict;
	if(ids.empty()){
		return AvatarNMap();
	}
  for(size_t i=0; i<ids.size(); i++){
    char data[64] = {0};
    snprintf(data, 64, "IMONLINE#%d", ids.at(i));
    uids.push_back(data);
  }
  pair<int, MemcachedClient*> cli = pool_.GetClient(ids.at(0)); 
  if(!cli.second){
    return AvatarNMap();
  }
  map<string, string> res;
  map<string, int> flags;
  TimeStat ts;
  bool succ = cli.second->Get(uids, res, flags);
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
  pool_.ReleaseClient(cli.first, cli.second);
  MCE_INFO("MemcOnlineCenter::GetUsers --> ids:" << ids.size() << " res:" << dict.size() << " cost_mem:" << cost_mem << " cost_par:" << cost_decom << " cost_total:" << (cost_mem+cost_decom));
  return dict;
}


AvatarNSeq OnlineCenterClient::GetUser(int user_id) {
  MyUtil::IntSeq ids;
  ids.push_back(user_id);
  AvatarNMap dict = GetUsers(ids);
	if(dict.find(user_id) != dict.end()){
		return dict[user_id];
	}
	return AvatarNSeq();
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
		mcavatar->set_hearttimestamp(time(NULL));
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
  GetUserKey(userid, key, 32);
  string value;
  bool b = SerializeAvatarNSeq(aseq, &value);
  //if(!b){
   // return false;
  //}
  pair<int, MemcachedClient*> cli = pool_.GetClient(userid); 
  if(!cli.second){
    return;
  }
  bool res = cli.second->Set(key, value, 0);
  pool_.ReleaseClient(cli.first, cli.second);
  //return res;
}
void OnlineCenterClient::DeleteUser(int userid){
  char key[32];
  GetUserKey(userid, key, 32);
  pair<int, MemcachedClient*> cli = pool_.GetClient(userid); 
  if(!cli.second){
    return ;
  }
  bool res = cli.second->Delete(key);
  pool_.ReleaseClient(cli.first, cli.second);
  //return res;
}



}
}
}

