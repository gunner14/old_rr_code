#include <vector>
#include "XmppTools.h"

#include "feed/FeedMemcProxy/client/talk_group_client.h"
#include "feed/FeedMemcProxy/client/compress_wrap.h"

#include "TalkGroupLoaderAdapter.h"

namespace xce {
namespace talk {
using namespace std;
using namespace xce::feed;
using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk::group::loader;
static const char* const kTalkGroupKeyPrefix = "TALK_GROUP_INFO#";
const int TalkGroupInfoClient::kKeyBufLen = 64;

TalkGroupInfoClient::TalkGroupInfoClient() { 
  pool_.Init(50, "im");
}

TalkGroupInfoClient::~TalkGroupInfoClient() {
}

void TalkGroupInfoClient::GetGroupMemcKey(int64_t sid, char* key_buff,
    size_t max_len) {
	  snprintf(key_buff, max_len, "%s%ld", kTalkGroupKeyPrefix, sid);
}

int64_t TalkGroupInfoClient::GetSessionId(const char* key_str_buff,
    size_t key_str_len) {
	  size_t key_prefix_len = strlen(kTalkGroupKeyPrefix);
		if (key_str_len < key_prefix_len) {
		  //MCE_WARN("NotifyMemcClient::GetNotifyContId key length error:"
		  //  << key_prefix_len);
		  return -1;
	  }
		if (strncmp(key_str_buff, kTalkGroupKeyPrefix, key_prefix_len) != 0) {
			 MCE_WARN("NotifyMemcClient::GetNotifyContId key error" << key_str_buff);
			   return -2;
			 }
		int64_t session_id;
		try {
			string session_id_str(key_str_buff + key_prefix_len);
			session_id = boost::lexical_cast<int64_t>(session_id_str);
			} catch (boost::bad_lexical_cast& exp) {
				MCE_WARN("NotifyMemcClient::GetNotifyContId lexical_cast error"
				<< " key_str_buff:" << key_str_buff);
				return -3;
				}
				return session_id;
}




void TalkGroupInfoClient::PbTalkGroupInfo(const GroupChatGroupInfo& pb_content,
     GroupInfoPtr& info_cont) {
	if(info_cont){
		info_cont->id = pb_content.id();
		info_cont->name = pb_content.name();
		info_cont->head = pb_content.head();
		info_cont->creator = pb_content.creator();
		info_cont->createTime = pb_content.create_time();
		info_cont->introduction = pb_content.introduction();
		info_cont->type = pb_content.type();
		info_cont->lastContent = pb_content.last_content();
		info_cont->lastTime= pb_content.last_time();
		info_cont->announcement= pb_content.announcement();
		info_cont->authentication= pb_content.authentication();
		info_cont->notify= pb_content.notify();
		info_cont->saveonline= pb_content.saveonline();
		info_cont->level= pb_content.level();
		info_cont->toplimit= pb_content.toplimit();
	}
}

bool TalkGroupInfoClient::GetGroupInfoByIdWithLoad(int64_t sid, GroupInfoPtr &info) {
  std::vector<int64_t> session_ids;
  session_ids.push_back(sid);
  GroupInfoDict gd;
  if (GetGroupInfosBySeqWithLoad(session_ids, gd)) {
    if (!gd.empty())
      info.swap(gd.begin()->second);
    return true;
  } else {
    return false;
  }

}

bool TalkGroupInfoClient::GetGroupInfosBySeqWithLoad(const std::vector<int64_t> &session_ids, GroupInfoDict &info_dict) {
  if (!GetGroupInfos(session_ids, info_dict)) {
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
      GroupInfoDict loadans;
      TalkGroupLoaderAdapter::instance().LoadGroupInfo(loadids, loadans);
      if (!loadans.empty()) {
        info_dict.insert(loadans.begin(), loadans.end());
      } else {
        MCE_WARN("TalkGroupInfoClient::GetGroupInfosBySeqWithLoad get DB empty");
      }

		} catch (std::exception& e) {
			MCE_WARN("TalkGroupInfoClient::GetGroupInfosBySeqWithLoad --> CALL TalkRecentLoader.LoadBySeq err" << e.what());
		}
  }

  return true;


}

bool TalkGroupInfoClient::GetGroupInfo(int64_t sid,GroupInfoPtr& info) { 
	vector<int64_t> session_ids;
	session_ids.push_back(sid);
	GroupInfoDict info_dict;
	GetGroupInfos(session_ids, info_dict);
	MCE_DEBUG("TalkGroupInfoClient::GetGroupInfo info_dict.size" << info_dict.size());
	if (info_dict.empty() || info_dict.find(sid) == info_dict.end()) {
		MCE_WARN("TalkGroupInfoClient::GetNotifyContent empty nid[" << sid << "]");
		return 0;
	}
	info = info_dict[sid];   
	return 1;
}
bool TalkGroupInfoClient::GetGroupInfos(const vector<int64_t>& session_ids,
    GroupInfoDict& info_dict) {
  if (session_ids.empty()) {
    MCE_WARN("TalkGroupInfoClient::GetNotifyContents notify_ids empty");
    return false;
  }
  int session_ids_size = session_ids.size(); 
  vector<string> info_keys;
  char buffer[kKeyBufLen] = {0};
  for (int i = 0; i < session_ids_size; ++i) {
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, sizeof(buffer), "%s%ld", kTalkGroupKeyPrefix, //guoqing.liu  GetGroupMemcKey  不是有现成的函数么...
		session_ids.at(i));
		info_keys.push_back(string(buffer));
		MCE_DEBUG("TalkGroupInfoClient::GetGroupInfos buffer" << buffer);
	}

  pair<int, MemcachedClient*> memc_cli = pool_.GetClient(session_ids.at(0));
  if (!memc_cli.second) {
    MCE_WARN("TalkGroupInfoClient::GetNotifyContents GetClient failed");
    return false;
  }
  map<string, string> result;
  map<string, int> flags;

  TimeStatN ts;
  if(!memc_cli.second->Get(info_keys, result, flags)) {
    MCE_WARN("TalkGroupInfoClient::GetNotifyContents:memc client get failed"
        << " content_keys.size[" << info_keys.size() << "]");
    pool_.ReleaseClient(memc_cli.first, memc_cli.second);
    return false;
  }
  pool_.ReleaseClient(memc_cli.first, memc_cli.second);
  double memc_get_cost = ts.getTime();
  ts.reset();
  map<string, string>::iterator it = result.begin();
  for (; it != result.end(); ++it) {
    int64_t session_id = GetSessionId(it->first.c_str(), it->first.size());
    GroupChatGroupInfo pb_info_cont;
    int flag = flags[it->first];
    if (flag & 0x01) {
      string decompressed;
      CompressWrap::instance().Decompress(it->second.c_str(), &decompressed);
      pb_info_cont.ParseFromArray(decompressed.c_str(), decompressed.size());
    } else {
      pb_info_cont.ParseFromArray(it->second.c_str(), it->second.size());
    }
    GroupInfoPtr info_cont = new GroupInfo();
    PbTalkGroupInfo(pb_info_cont, info_cont);
    info_dict.insert(make_pair(session_id, info_cont));
  }
  double parse_cost = ts.getTime();

  MCE_INFO("TalkGroupInfoClient::GetNotifyContents:nid_size[" 
      << session_ids.size() << "] dict_size[" << info_dict.size()
      << "] memc_get_cost[" << memc_get_cost << "] parse_cost[" 
      << parse_cost << "]");
  return true;
}
bool TalkGroupInfoClient::SerializeContent2Str(const GroupInfoPtr& info_cont,
    string* pb_info_str){
	if(info_cont){
    GroupChatGroupInfo pb_group_info;
    pb_group_info.set_id(info_cont->id);
    pb_group_info.set_name(info_cont->name);
    pb_group_info.set_head(info_cont->head);
    pb_group_info.set_creator(info_cont->creator);
    pb_group_info.set_create_time(info_cont->createTime);
    pb_group_info.set_introduction(info_cont->introduction);
    pb_group_info.set_type(info_cont->type);
    pb_group_info.set_last_content(info_cont->lastContent);
    pb_group_info.set_last_time(info_cont->lastTime);
    pb_group_info.set_announcement(info_cont->announcement);
    pb_group_info.set_authentication(info_cont->authentication);
    pb_group_info.set_notify(info_cont->notify);
    pb_group_info.set_saveonline(info_cont->saveonline);
    pb_group_info.set_level(info_cont->level);
    pb_group_info.set_toplimit(info_cont->toplimit);
    return pb_group_info.SerializeToString(pb_info_str);
	}
	return false;
}


bool TalkGroupInfoClient::SetGroupInfo(const GroupInfoPtr& info_content) {
  string pb_info_str;
  char key_str_buff[kKeyBufLen] = {0};
 	GetGroupMemcKey(info_content->id, key_str_buff, sizeof(key_str_buff));
	MCE_DEBUG("TalkGroupInfoClient::SetGroupInfo " << key_str_buff);
  if (!SerializeContent2Str(info_content, &pb_info_str)) {
    MCE_WARN("TalkGroupInfoClient SerializeContent2Str failed");
    return false;
  }

  MCE_INFO("TalkGroupInfoClient::SetGroupInfo " << pb_info_str);
  pair<int, MemcachedClient*> memc_cli = pool_.GetClient(
      info_content->id);								
	if(!memc_cli.second){
		return false;
	}
  bool ret = memc_cli.second->Set(key_str_buff, pb_info_str, 0);
  pool_.ReleaseClient(memc_cli.first, memc_cli.second);
  if (!ret) {
    MCE_WARN("TalkGroupInfoClient::SetNotifyContent memcached set failed");
	} 
  return ret;
}
bool TalkGroupInfoClient::DeleteGroupInfo(const long sid){ 	
	char key_str_buff[kKeyBufLen] = {0};
	GetGroupMemcKey(sid, key_str_buff, sizeof(key_str_buff));
	pair<int, MemcachedClient*> memc_cli = pool_.GetClient(sid);
	if(!memc_cli.second){
		MCE_WARN("TalkGroupInfoClient::DeleteGroupInfo --> MemcachedClient ptr is NULL!  source:" << sid);
		return false;
	}

	if(!memc_cli.second->Delete(key_str_buff)) {
		MCE_WARN("TalkGroupInfoClient::DeleteGroupInfo -->  remove failed!  key:" << key_str_buff);
		pool_.ReleaseClient(memc_cli.first, memc_cli.second);
		return false;
	}
	pool_.ReleaseClient(memc_cli.first, memc_cli.second);
	return true;

}
}
}

