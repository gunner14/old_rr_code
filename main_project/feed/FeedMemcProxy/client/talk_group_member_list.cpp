#include "feed/FeedMemcProxy/client/talk_group_member_list.h"
#include "feed/FeedMemcProxy/client/compress_wrap.h"
#include <boost/algorithm/string.hpp>
#include <vector>

#include "XmppTools.h"
#include "TalkGroupMemberLoaderAdapter.h"
namespace xce {
namespace talk {
using namespace std;
using namespace xce::feed;
using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk::group::memberloader;

static const char* const kTalkGroupKeyPrefix = "TALK_GROUP_MEMBER#";
const int TalkGroupMemberClient::kKeyBufLen = 64;

TalkGroupMemberClient::TalkGroupMemberClient() { 
  pool_.Init(50, "im");
}

TalkGroupMemberClient::~TalkGroupMemberClient() {
}

void TalkGroupMemberClient::GetGroupMemcKey(int64_t sid, char* key_buff,
    size_t max_len) {
	  snprintf(key_buff, max_len, "%s%ld", kTalkGroupKeyPrefix, sid);
}

int64_t TalkGroupMemberClient::GetSessionId(const char* key_str_buff,
		size_t key_str_len) {
	size_t key_prefix_len = strlen(kTalkGroupKeyPrefix);
	if (key_str_len < key_prefix_len) {

		return -1;
	}
	if (strncmp(key_str_buff, kTalkGroupKeyPrefix, key_prefix_len) != 0) {
		MCE_WARN("TalkGroupMemberClient::GetSessionId key error" << key_str_buff);
		return -2;
	}
	int64_t session_id;
	try {
		string session_id_str(key_str_buff + key_prefix_len);
		session_id = boost::lexical_cast<int64_t>(session_id_str);
	} catch (boost::bad_lexical_cast& exp) {
		MCE_WARN("TalkGroupMemberClient::GetSessionId lexical_cast error"
				<< " key_str_buff:" << key_str_buff);
		return -3;
	}
	return session_id;
}




void TalkGroupMemberClient::PbTalkGroupMember(const PbGroupMember& pb_group_member,
		GroupMemberSeq& group_member) {
	//		group_member->sessionId = pb_group_member.session_id();
	string member_list  = pb_group_member.member_list();
	vector<string> tokens;
	MCE_INFO("TalkGroupMemberClient::PbTalkGroupMember----" << member_list);
	boost::split(tokens, member_list, boost::is_any_of(","));
	if(tokens.size() == 0){
		vector<string> token;
		GroupMemberPtr member = new GroupMember();
		boost::split(token, member_list, boost::is_any_of(":"));
		if(token.size() == 2){
			try{
				member->uid = boost::lexical_cast<int>(token[0]);
				member->level = boost::lexical_cast<int>(token[1]);
				group_member.push_back(member);
			}catch(boost::bad_lexical_cast& exp){
				MCE_WARN("boost err");
			}
		}
	}
	for (size_t i = 0; i < tokens.size(); ++ i) {
		vector<string> token;
		GroupMemberPtr member = new GroupMember();
		boost::split(token, tokens[i], boost::is_any_of(":"));
		if(token.size() == 2){
			try{
				member->uid = boost::lexical_cast<int>(token[0]);
				member->level = boost::lexical_cast<int>(token[1]);
				group_member.push_back(member);
			}catch(boost::bad_lexical_cast& exp){
				MCE_WARN("boost err");	
			}
		}
	}
}
//  这函数假设了session_ids没有重复！！应该用set比较好
bool TalkGroupMemberClient::GetGroupMembersBySeqWithLoad(const std::vector<int64_t> &session_ids, GroupMemberDict &info_dict) {
  if (!GetGroupMembers(session_ids, info_dict)) {
    return false;
  }

  // 如果取的不够，从db里面取
  if (info_dict.size() < session_ids.size()) {
    std::vector<int64_t> loadids;
    for(std::vector<int64_t>::const_iterator idit = session_ids.begin(); idit != session_ids.end(); ++idit){
      if (info_dict.find(*idit) == info_dict.end()) {
        loadids.push_back((*idit));
      }
    }
		try {
      GroupMemberDict loadans;
      TalkGroupMemberLoaderAdapter::instance().LoadGroupMember(loadids, loadans);
      if (!loadans.empty()) {
        info_dict.insert(loadans.begin(), loadans.end());
      } else {
        MCE_WARN("TalkGroupMemberClient::GetGroupMembersBySeqWithLoad get DB empty");
      }

		} catch (std::exception& e) {
			MCE_WARN("TalkGroupMemberClient::GetGroupMembersBySeqWithLoad --> CALL TalkRecentLoader.LoadBySeq err" << e.what());
		}
  }

  return true;
  
}

bool TalkGroupMemberClient::GetGroupMembersByIdWithLoad(int64_t sid, GroupMemberSeq &member_list) {
  std::vector<int64_t> session_ids;
  session_ids.push_back(sid);
  GroupMemberDict gd;
  if (GetGroupMembersBySeqWithLoad(session_ids, gd)) {
    if (!gd.empty())
      member_list.swap(gd.begin()->second);
    return true;
  } else {
    return false;
  }
  
}


bool TalkGroupMemberClient::GetGroupMember(int64_t sid,GroupMemberDict& member_list) { 
	vector<int64_t> session_ids;
	session_ids.push_back(sid);
	GetGroupMembers(session_ids, member_list);
	if (member_list.empty() || member_list.find(sid) != member_list.end()) {
		MCE_WARN("TalkGroupMemberClient::GetGroupMember empty nid[" << sid << "]");
		return 0;
	}
	return 1;
}


bool TalkGroupMemberClient::GetGroupMembers(const vector<int64_t>& session_ids,
    GroupMemberDict& member_list) {
  member_list.clear();
  if (session_ids.empty()) {
    return true;
  }
  int session_ids_size = session_ids.size(); 
  vector<string> info_keys;
  char buffer[kKeyBufLen] = {0};
  for (int i = 0; i < session_ids_size; ++i) {
    GetGroupMemcKey(session_ids.at(i), buffer, sizeof(buffer));
		info_keys.push_back(string(buffer));
	}
  pair<int, MemcachedClient*> memc_cli = pool_.GetClient(session_ids.at(0));
  if (!memc_cli.second) {
    MCE_WARN("TalkGroupMemberClient::GetGroupMembers GetClient failed");
    return false;
  }
  map<string, string> result;
  map<string, int> flags;
  TimeStatN ts;
  bool succ = memc_cli.second->Get(info_keys, result, flags);
  pool_.ReleaseClient(memc_cli.first, memc_cli.second); // 不管是不是成功都要释放
  if(!succ) {
    MCE_WARN("TalkGroupMemberClient::GetGroupMembers memc client get failed"
        << " content_keys.size[" << info_keys.size() << "]");
    return false;
  }

  double memc_get_cost = ts.getTime();
  ts.reset();
  map<string, string>::iterator it = result.begin();
  for (; it != result.end(); ++it) {
    int64_t session_id = GetSessionId(it->first.c_str(), it->first.size());
    PbGroupMember pb_info_cont;
    int flag = flags[it->first];
    if (flag & 0x01) {
      string decompressed;
      CompressWrap::instance().Decompress(it->second.c_str(), &decompressed);
      pb_info_cont.ParseFromArray(decompressed.c_str(), decompressed.size());
    } else {
      pb_info_cont.ParseFromArray(it->second.c_str(), it->second.size());
    }
		GroupMemberSeq member_seq;
    PbTalkGroupMember(pb_info_cont, member_seq);
    member_list.insert(make_pair(session_id, member_seq));
  }


  double parse_cost = ts.getTime();



  MCE_INFO("TalkGroupMemberClient::GetGroupMembers nid_size[" 
      << session_ids.size() << "] dict_size[" << member_list.size()
      << "] memc_get_cost[" << memc_get_cost << "] parse_cost[" 
      << parse_cost << "]");
  return true;
}
bool TalkGroupMemberClient::SerializeContent2Str(long sid, const GroupMemberSeq& members,
    string* pb_info_str){
  PbGroupMember pb_group_member;
  pb_group_member.set_session_id(sid);
	const GroupMemberSeq &MList = members;
	int MCnt = MList.size();
	int flag = 0;
  string member_str;
	for(GroupMemberSeq::const_iterator iter = MList.begin(); iter != MList.end(); ++iter){
		  flag ++;
			if(flag != MCnt)
	    	member_str += boost::lexical_cast<string>((*iter)->uid) + ":" + boost::lexical_cast<string>((*iter)->level) + ","; 	
			else
				member_str += boost::lexical_cast<string>((*iter)->uid) + ":" + boost::lexical_cast<string>((*iter)->level);
		}
	MCE_INFO("TalkGroupMemberClient::SerializeContent2Str " << member_str);
  pb_group_member.set_member_list(member_str);
  return pb_group_member.SerializeToString(pb_info_str);

}


bool TalkGroupMemberClient::SetGroupMember(const GroupMemberDict& group_member) {
	string pb_info_str;
	char key_str_buff[kKeyBufLen] = {0};
	for(GroupMemberDict::const_iterator iter = group_member.begin(); iter != group_member.end(); ++iter){
		GetGroupMemcKey(iter->first, key_str_buff, sizeof(key_str_buff));
		if (!SerializeContent2Str(iter->first, iter->second, &pb_info_str)) {
			MCE_WARN("TalkGroupMemberClient SerializeContent2Str failed");
			return false;
		}
		pair<int, MemcachedClient*> memc_cli = pool_.GetClient(iter->first);								
		if(!memc_cli.second){
			return false;
		}
		bool ret = memc_cli.second->Set(key_str_buff, pb_info_str, 0);
		pool_.ReleaseClient(memc_cli.first, memc_cli.second);
		if (!ret) {
			MCE_WARN("TalkGroupMemberClient::SetGroupMember memcached set failed");
		} 
	}
	return true;
}

bool TalkGroupMemberClient::DeleteOneGroup(int64_t sid) {
	char key_str_buff[kKeyBufLen] = {0};
	GetGroupMemcKey(sid, key_str_buff, sizeof(key_str_buff));
	pair<int, MemcachedClient*> memc_cli = pool_.GetClient(sid);
	if(!memc_cli.second){
		MCE_WARN("TalkGroupMemberClient::DeleteGroup --> MemcachedClient ptr is NULL!  source:" << sid);
		return false;
	}

	if(!memc_cli.second->Delete(key_str_buff)) {
		MCE_WARN("TalkGroupMemberClient::DeleteGroup -->  remove failed!  key:" << key_str_buff);
		pool_.ReleaseClient(memc_cli.first, memc_cli.second);
		return false;
	}
	pool_.ReleaseClient(memc_cli.first, memc_cli.second);
	return true;

}

void TalkGroupMemberClient::DeleteGroup(const MyUtil::LongSeq &idSeq){


  for (MyUtil::LongSeq::const_iterator it = idSeq.begin(); it != idSeq.end(); ++it) {
    DeleteOneGroup(*it);
  }
}
}
}

