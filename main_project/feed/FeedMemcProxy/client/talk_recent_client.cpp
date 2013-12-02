#include "talk_recent_client.h"
#include <boost/lexical_cast.hpp>
#include "TalkRecent.pb.h"
#include "TalkRecentLoaderAdapter.h"
#include "ServiceI.h"
#include "LogWrapper.h"

namespace xce {
namespace talk {
namespace recent{

using namespace MyUtil;
using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::recent::contact;


static char * kEmptyRecent = "EMPTY_RECENT";

static void GetUserKey(Ice::Long fid, char * key, size_t len) {
  snprintf(key, len, "IMRECENT#%ld", fid);  
  key[len - 1] = 0;
}
Ice::Long GetUserId(const char * key, size_t len) {
  if (len < 9 || strncmp("IMRECENT#", key, 9) != 0) {
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

static void MemcRecentContactToTalkRecentUserSeq(const McRecentContact& memc_contact, TalkRecentUserSeq& talk_user_seq) {
	for(int i = 0; i < memc_contact.recentcontact_size(); ++i){
		McRecentUser memc_user = memc_contact.recentcontact(i);
		TalkRecentUserPtr talk_user = new TalkRecentUser();
		talk_user->id = memc_user.id();
		talk_user->subject= memc_user.subject();
		talk_user->info= memc_user.info();
		talk_user->type = memc_user.type();
		talk_user_seq.push_back(talk_user);
	}
}

TalkRecentClient::TalkRecentClient() {
  pool_.Init(100);
}

TalkRecentClient::~TalkRecentClient() {
}

TalkRecentUserSeqMap TalkRecentClient::GetRecentUsers(const MyUtil::IntSeq& ids){
  float cost_mem;
  float cost_decom;
  vector<string> uids;
  TalkRecentUserSeqMap dict;
	if(ids.empty()){
		return TalkRecentUserSeqMap();
	}
  for(size_t i=0; i<ids.size(); i++){
    char data[64] = {0};
    snprintf(data, 64, "IMRECENT#%d", ids.at(i));
    uids.push_back(data);
  }
  pair<int, MemcachedClient*> cli = pool_.GetClient(ids.at(0)); 
  if(!cli.second){
    return TalkRecentUserSeqMap();
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
		if(kEmptyRecent == it->second){
			dict.insert(make_pair(uid, TalkRecentUserSeq()));
			//MCE_INFO("TalkRecentClient::GetUsers --> id:" << uid << " kEmptyRecent");
			continue;
		}
		//MCE_INFO("TalkRecentClient::GetUsers --> id:" << uid << " v.size:" << it->second.size() << " value:" << it->second);
		McRecentContact recent_contact;
    recent_contact.ParseFromArray(it->second.c_str(), it->second.size());
		TalkRecentUserSeq seq;
		MemcRecentContactToTalkRecentUserSeq(recent_contact,seq);
		dict.insert(make_pair(uid, seq));
  }
  cost_decom = ts.getTime();
  pool_.ReleaseClient(cli.first, cli.second);
  MCE_INFO("TalkRecentClient::GetUsers --> ids:" << ids.size() << " res:" << dict.size() << " cost_mem:" << cost_mem << " cost_par:" << cost_decom << " cost_total:" << (cost_mem+cost_decom));
  return dict;
}

TalkRecentUserSeq TalkRecentClient::GetRecentUser(int user_id) {
  MyUtil::IntSeq ids;
  ids.push_back(user_id);
  TalkRecentUserSeqMap dict = GetRecentUsers(ids);
	if(dict.find(user_id) != dict.end()){
		return dict[user_id];
	}
	return TalkRecentUserSeq();
}

// 返回值, 标识是否压缩了
bool TalkRecentClient::SerializeTalkRecentUserSeq(const TalkRecentUserSeq& aseq, string * serialized) {
  if (aseq.empty()) {
    return false;
  }

	McRecentContact memc_recent_contact;
 	
	for(TalkRecentUserSeq::const_iterator sit = aseq.begin(); sit != aseq.end(); ++sit){
		McRecentUser* memc_recent_user = memc_recent_contact.add_recentcontact();	
		if(!memc_recent_user){
			continue;
		}
		TalkRecentUserPtr talk_user = *sit;
		memc_recent_user->set_id(talk_user->id);
		memc_recent_user->set_subject(talk_user->subject);
		memc_recent_user->set_info(talk_user->info);
		memc_recent_user->set_type(talk_user->type);
	}
  memc_recent_contact.SerializeToString(serialized);
  return true;
}

void TalkRecentClient::SetEmptyRecent(int userid){
  char key[32];
  GetUserKey(userid, key, 32);
  pair<int, MemcachedClient*> cli = pool_.GetClient(userid); 
  if(!cli.second){
    return ;
  }
  bool res = cli.second->Set(key, kEmptyRecent, 0);
  pool_.ReleaseClient(cli.first, cli.second);


  //  MCE_INFO("TalkRecentClient::SetEmptyRecent --> res" << res << " uid " << userid);
  //return res;
}
void TalkRecentClient::SetRecentUser(int userid, const TalkRecentUserSeq& seq) {
	if(seq.empty()){
		SetEmptyRecent(userid);
    //MCE_INFO("TalkRecentClient::SetRecentUser -->uid " << userid);
		return;
	}

  char key[32];
  GetUserKey(userid, key, 32);
  string value;
  bool b = SerializeTalkRecentUserSeq(seq, &value);
  pair<int, MemcachedClient*> cli = pool_.GetClient(userid); 
  if(!cli.second){
    return;
  }
  bool res = cli.second->Set(key, value, 0);
  pool_.ReleaseClient(cli.first, cli.second);
  //return res;
}

TalkRecentUserSeq TalkRecentClient::GetRecentUserByIdWithLoad(int userid){
	MyUtil::IntSeq uids;
	uids.push_back(userid);
	TalkRecentUserSeqMap mp = GetRecentUsers(uids);
	TalkRecentUserSeq ans;
	TalkRecentUserSeqMap::iterator it = mp.find(userid);
	if(it == mp.end()){
		try{
			ans = TalkRecentLoaderAdapter::instance().LoadById(userid);
		}
		catch(std::exception& e){
			MCE_WARN("TalkRecentClient::GetUserByIdWithLoad --> CALL TalkRecentLoader.LoadById err" << e.what());
		}catch(...){
			MCE_WARN("TalkRecentCacheClient::GetUserByIdWithLoad --> CALL TalkRecentLoader.LoadById err");
		}
	}else{
	      ans = it->second;
	}
	return ans;
}
TalkRecentUserSeqMap TalkRecentClient::GetRecentUserBySeqWithLoad(int actor, const MyUtil::IntSeq& ids){
	TalkRecentUserSeqMap ans;
	if(ids.empty()){
		return ans;
	}
	ans = GetRecentUsers(ids);
	MCE_INFO("TalkRecentClient::GetUserBySeqWithLoad --> " << actor<< " ids:" << ids.size() << " " << ans.size());
	if(ids.size() == ans.size()){
    return ans;
	}
	TalkRecentUserSeqMap loadans;
	IntSeq loadids;
	TalkRecentUserSeqMap::iterator dit;
	for(MyUtil::IntSeq::const_iterator idit = ids.begin(); idit != ids.end(); ++idit){
		dit = ans.find((*idit));
		if(dit == ans.end()){
			loadids.push_back((*idit));
		}
	}
	if(!loadids.empty()){
		try{
			loadans = TalkRecentLoaderAdapter::instance().LoadBySeq(loadids);
		}
		catch(std::exception& e){
			MCE_WARN("TalkRecentClient::GetUserBySeqWithLoad --> CALL TalkRecentLoader.LoadBySeq err" << e.what());
		}catch(...){
			MCE_WARN("TalkRecentClient::GetUserBySeqWithLoad --> CALL TalkRecentLoader.LoadBySeq err");
		}
		if(!loadans.empty()){
			ans.insert(loadans.begin(), loadans.end());
		}
	}
  return ans;
}

}
}
}

