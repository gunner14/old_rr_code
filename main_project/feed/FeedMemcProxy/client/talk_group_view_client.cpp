#include "talk_group_view_client.h"
#include <boost/lexical_cast.hpp>
#include "TalkGroupViewLoaderAdapter.h"
#include "ServiceI.h"
#include "LogWrapper.h"


namespace xce {
namespace talk {
namespace recent{

using namespace MyUtil;
using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::groupview;
using namespace com::xiaonei::talk::groupview::loader;

  //static char * kEmptyRecent = "EMPTY_GROUPVIEW";

static void GetGroupViewKey(Ice::Long fid, char * key, size_t len) {
  snprintf(key, len, "TALKGROUPVIEW#%ld", fid);  
  key[len - 1] = 0;
}
Ice::Long GetUserId(const char * key, size_t len) {
  if (len < 14 || strncmp("TALKGROUPVIEW#", key, 14) != 0) {
    return -1;
  }

  Ice::Long id = -1;
  try {
    id = boost::lexical_cast<Ice::Long>(string(key + 14, len - 14));
  } catch(boost::bad_lexical_cast &) {
    return -1;
  }
  return id;
}

#define GET_GROUPVIEW_SEQ(mcgroups, groupname, group_seq)  \
	for(int i = 0; i < mcgroups.groupname##_size(); i++)\
	{\
		McGroupView memc_group = mcgroups.groupname(i);\
		TalkGroupViewPtr talk_group = new TalkGroupView();\
                talk_group->userId = memc_group.userid();\
                talk_group->sessionId = memc_group.sessionid();\
                talk_group->isVisible = memc_group.isvisible();\
                talk_group->hasRead = memc_group.hasread();\
                talk_group->sessionType = memc_group.sessiontype();\
                talk_group->affiliation = memc_group.affiliation();\
                talk_group->updateTime = memc_group.updatetime();\
                talk_group->saveonline = memc_group.saveonline();\
                talk_group->notify = memc_group.notify();\
                talk_group->msgid = memc_group.msgid(); \
                group_seq.push_back(talk_group);\
	}


class GroupCompare
{
public:
	bool operator()(const IceInternal::Handle<com::xiaonei::talk::groupview::loader::TalkGroupView>&a, const IceInternal::Handle<com::xiaonei::talk::groupview::loader::TalkGroupView>& b)
	{
		return a->updateTime > b->updateTime;
	}		
};

static void MemcGroupsToTalkGroupViewSeq(const McGroups& memc_groups, TalkGroupViewSeq& talk_group_seq/*取到组类型->组列表的映射*/) {
	//将固定群，讨论组，两人聊天进行混合后排序取前20条
	GET_GROUPVIEW_SEQ(memc_groups, solidgroup, talk_group_seq)
	GET_GROUPVIEW_SEQ(memc_groups, discussgroup, talk_group_seq)
	GET_GROUPVIEW_SEQ(memc_groups, conversationgroup, talk_group_seq)
	
	sort(talk_group_seq.begin(), talk_group_seq.end(), GroupCompare());
	if(talk_group_seq.size() <= 20)
		return;
	else
		talk_group_seq.erase(talk_group_seq.begin()+20, talk_group_seq.end());
	return;
}

static void MemcGroupsToTalkGroupViewSeqByType(const McGroups& memc_groups, int type, TalkGroupViewSeq& talk_group_seq/*取到组类型->组列表的映射*/) {
	if(type == TalkGroupViewClient::SOLID_GROUP)
	{
		GET_GROUPVIEW_SEQ(memc_groups, solidgroup, talk_group_seq)
	}
	else if( type == TalkGroupViewClient::DISCUSS_GROUP)
	{
		GET_GROUPVIEW_SEQ(memc_groups, discussgroup, talk_group_seq)
	}
	else if( type == TalkGroupViewClient::CONVERSATION_GROUP)
	{
		GET_GROUPVIEW_SEQ(memc_groups, conversationgroup, talk_group_seq)
	}
	return;
}

TalkGroupViewClient::TalkGroupViewClient() {
  pool_.Init(100, "im");
}

TalkGroupViewClient::~TalkGroupViewClient() {
}

bool TalkGroupViewClient::GetRecentGroups(const MyUtil::IntSeq& ids, TalkGroupViewSeqMap &groupviews){
	float cost_mem;
	float cost_decom;
	vector<string> uids;
	TalkGroupViewSeqMap &dict = groupviews;
	if(ids.empty()){
		return true;
	}
	for(size_t i=0; i<ids.size(); i++){
		char data[64] = {0};
		snprintf(data, 64, "TALKGROUPVIEW#%d", ids.at(i));
		uids.push_back(data);
	}
	pair<int, MemcachedClient*> cli = pool_.GetClient(ids.at(0)); 
	if(!cli.second){
		return false;
	}
	map<string, string> res;
	map<string, int> flags;
	TimeStat ts;
	bool succ = cli.second->Get(uids, res, flags);//cli 为memcache客户端, 第三个参数干啥的？
	pool_.ReleaseClient(cli.first, cli.second);

  if(!succ) {
    MCE_WARN("TalkGroupViewClient::GetRecentGroups memc client get failed"
        << " content_keys.size[" << uids.size() << "]");
    return false;
  }

  
	cost_mem = ts.getTime();
	ts.reset();
	map<string, string>::iterator it = res.begin();
	for(; it!=res.end(); ++it){
		int uid = GetUserId(it->first.c_str(), it->first.size());

		MCE_INFO("TalkGroupViewClient::GetUsers --> id:" << uid << " v.size:" << it->second.size());
		McGroups mc_groups;
		mc_groups.ParseFromString(it->second);
		TalkGroupViewSeq seq;
		MemcGroupsToTalkGroupViewSeq(mc_groups, seq);
		dict.insert(make_pair(uid, seq));
	}
	cost_decom = ts.getTime();

	MCE_INFO("TalkGroupViewClient::GetRecentGroups --> ids:" << ids.size() << " res:" << dict.size() << " cost_mem:" << cost_mem << " cost_par:" << cost_decom << " cost_total:" << (cost_mem+cost_decom));
  
  return true;
}

bool TalkGroupViewClient::GetRecentGroup(int user_id, TalkGroupViewSeq &groupviews) {
  MyUtil::IntSeq ids;
  ids.push_back(user_id);
  TalkGroupViewSeqMap dict;
  if (GetRecentGroups(ids, dict) && !dict.empty()) {
    groupviews.swap(dict.begin()->second);
    return true;
  } else {
    return false;
  }
}

bool TalkGroupViewClient::GetRecentGroupsByType(const MyUtil::IntSeq& ids, int type, TalkGroupViewSeqMap &groupviews){
	float cost_mem;
	float cost_decom;
	vector<string> uids;
	TalkGroupViewSeqMap &dict = groupviews;
	if(ids.empty()){
		return true;
	}
	for(size_t i=0; i<ids.size(); i++){
		char data[64] = {0};
		snprintf(data, 64, "TALKGROUPVIEW#%d", ids.at(i));
		uids.push_back(data);
	}
	pair<int, MemcachedClient*> cli = pool_.GetClient(ids.at(0)); 
	if(!cli.second){
    return false;
	}
	map<string, string> res;
	map<string, int> flags;
	TimeStat ts;
	bool succ = cli.second->Get(uids, res, flags);//cli 为memcache客户端, 第三个参数干啥的？
	pool_.ReleaseClient(cli.first, cli.second);
  if(!succ) {
    MCE_WARN("TalkGroupViewClient:::GetRecentGroupsByType memc client get failed"
        << " content_keys.size[" << uids.size() << "]");
    return false;
  }

	cost_mem = ts.getTime();
	ts.reset();
	map<string, string>::iterator it = res.begin();
	for(; it!=res.end(); ++it){
		int uid = GetUserId(it->first.c_str(), it->first.size());
		MCE_DEBUG("TalkGroupViewClient::GetRecentGroupsByType -->  type" << type <<"  key:" << it->first << " value.size:" << it->second.size());
		McGroups mc_groups;
		mc_groups.ParseFromString(it->second);
		TalkGroupViewSeq seq;
		MemcGroupsToTalkGroupViewSeqByType(mc_groups, type, seq);
		dict.insert(make_pair(uid, seq));
	}
	cost_decom = ts.getTime();

	MCE_INFO("TalkGroupViewClient::GetRecentGroupsByType --> ids:" << ids.size() << " res:" << dict.size() << " cost_mem:" << cost_mem << " cost_par:" << cost_decom << " cost_total:" << (cost_mem+cost_decom));
	return true;
}

bool TalkGroupViewClient::GetRecentGroupByType(int user_id, int type, TalkGroupViewSeq &groupviews) {
  MyUtil::IntSeq ids;
  ids.push_back(user_id);
  TalkGroupViewSeqMap dict;
  ids.push_back(user_id);

  if (GetRecentGroupsByType(ids, type, dict) && !dict.empty()) {
    groupviews.swap(dict.begin()->second);
    return true;
  } else {
    return false;
  }

}

#define SET_GROUP_VIEW_SEQ(mcgroups, groupname, groupseq) \
	mcgroups.clear_##groupname();\
        for(TalkGroupViewSeq::const_iterator sit = groupseq.begin(); sit != groupseq.end(); ++sit){ \
                McGroupView * memc_group_view = mc_groups.add_##groupname(); \
                if(!memc_group_view){\
                        continue;\
                }\
                TalkGroupViewPtr talk_group = *sit;\
                memc_group_view->set_userid(talk_group->userId);\
                memc_group_view->set_sessionid(talk_group->sessionId);\
                memc_group_view->set_isvisible(talk_group->isVisible);\
                memc_group_view->set_hasread(talk_group->hasRead);\
                memc_group_view->set_sessiontype(talk_group->sessionType);\
                memc_group_view->set_affiliation(talk_group->affiliation);\
                memc_group_view->set_updatetime(talk_group->updateTime);\
                memc_group_view->set_saveonline(talk_group->saveonline);\
                memc_group_view->set_notify(talk_group->notify);\
                memc_group_view->set_msgid(talk_group->msgid);\
        }


bool TalkGroupViewClient::SerializeTalkGroupViewSeq(McGroups& mc_groups, const TalkGroupViewSeq& aseq, int type, string * serialized) {
	if (aseq.empty()) {
		return false;
	}
	if(type == TalkGroupViewClient::SOLID_GROUP)
	{
		SET_GROUP_VIEW_SEQ(mc_groups, solidgroup, aseq)
	}
	else if( type == TalkGroupViewClient::DISCUSS_GROUP)
	{
		SET_GROUP_VIEW_SEQ(mc_groups, discussgroup, aseq)
	}
	else if( type == TalkGroupViewClient::CONVERSATION_GROUP)
	{
		SET_GROUP_VIEW_SEQ(mc_groups, conversationgroup, aseq)
	}
	
	mc_groups.SerializeToString(serialized);
	return true;
}

void TalkGroupViewClient::SetRecentGroup(int userid, const TalkGroupViewSeq& seq, int type) {
  MCE_INFO("TalkGroupViewClient::SetRecentGroup --> uid:" << userid << " type:" << type << " seq.size:" << seq.size());
	char key[32];
	GetGroupViewKey(userid, key, 32);
	pair<int, MemcachedClient*> cli = pool_.GetClient(userid);
	if(!cli.second){
		return;
	}
	vector<string> uids;
	uids.push_back(key);
  map<string, string> res;
  map<string, int> flags;
	string value;

  McGroups  mc_groups;
  bool b = SerializeTalkGroupViewSeq(mc_groups, seq, type, &value);
  MCE_DEBUG("TalkGroupViewClient::SetRecentGroup --> value:" << value);
	bool result = cli.second->Set(key, value, 0);
  MCE_DEBUG("TalkGroupViewClient::SetRecentGroup --> res:" << result << " key:" << key << " value.size:" << value.size());
	pool_.ReleaseClient(cli.first, cli.second);
	//return res;
}

bool TalkGroupViewClient::GetRecentGroupByIdWithLoad(int uid, TalkGroupViewSeq &groupviews) {
  MyUtil::IntSeq ids;
  ids.push_back(uid);
  TalkGroupViewSeqMap dict;

  if (GetRecentGroupsBySeqWithLoad(uid, ids, dict, false)) {
    if (!dict.empty())
      groupviews.swap(dict.begin()->second);
    return true;
  }
  return false;
}


bool TalkGroupViewClient::GetAllGroups(int uid, TalkGroupViewSeq &groupviews) {
  MCE_DEBUG("TalkGroupViewClient::GetAllGroups  uid:" << uid);
  MyUtil::IntSeq ids;
  ids.push_back(uid);
  TalkGroupViewSeqMap dict;

  if (GetRecentGroupsBySeqWithLoad(uid, ids, dict, true)) {
    if (!dict.empty())
      groupviews.swap(dict.begin()->second);
    return true;
  }
  return false;
}


bool TalkGroupViewClient::GetRecentGroupsBySeqWithLoad(int actor, const MyUtil::IntSeq& ids, TalkGroupViewSeqMap &groupviews, bool is_get_all_groups) {
  bool rv;
  TalkGroupViewSeqMap gv0;
  rv = GetRecentGroupsTypeBySeqWithLoad(actor, TalkGroupViewClient::SOLID_GROUP, ids, gv0);

  /*
  TalkGroupViewSeqMap gv1;
  if (rv)
    rv = GetRecentGroupsTypeBySeqWithLoad(actor, TalkGroupViewClient::DISCUSS_GROUP, ids, gv1);
  TalkGroupViewSeqMap gv2;
  if (rv)
    rv = GetRecentGroupsTypeBySeqWithLoad(actor, TalkGroupViewClient::CONVERSATION_GROUP, ids, gv2);
  */
  if (rv) {
    for (MyUtil::IntSeq::const_iterator id_iter=ids.begin(); id_iter!=ids.end(); id_iter++) {
      TalkGroupViewSeq talk_group_view;
      TalkGroupViewSeqMap::const_iterator view_iter;
      view_iter = gv0.find(*id_iter);
      if (view_iter != gv0.end())
        talk_group_view.insert(talk_group_view.end(), (view_iter->second).begin(), (view_iter->second).end());
      /*
      view_iter = gv1.find(*id_iter);
      if (view_iter != gv1.end())
        talk_group_view.insert(talk_group_view.end(), (view_iter->second).begin(), (view_iter->second).end());
      view_iter = gv2.find(*id_iter);
      if (view_iter != gv2.end())
        talk_group_view.insert(talk_group_view.end(), (view_iter->second).begin(), (view_iter->second).end());
        */
      if (!is_get_all_groups) {
	      sort(talk_group_view.begin(), talk_group_view.end(), GroupCompare());
	      if(talk_group_view.size() > 20)
	      	talk_group_view.erase(talk_group_view.begin()+20, talk_group_view.end());
      }
      groupviews[*id_iter] = talk_group_view;
    }
  }
  // 这里可能还要优化！！！！先实现了
  // 上面是确认是否需要loaddb，下面从cache取，也就是两次取了cache，是不是可以优化成一次呢
//  if (rv)
//    rv = GetRecentGroups(ids, groupviews);
  MCE_INFO("TalkGroupViewClient::GetRecentGroupsBySeqWithLoad --> size " << groupviews.size())

  return rv;
}

bool TalkGroupViewClient::GetRecentGroupTypeByIdWithLoad(int userid, int type, TalkGroupViewSeq &groupviews) {
  MyUtil::IntSeq ids;
  ids.push_back(userid);
  TalkGroupViewSeqMap dict;
  if (GetRecentGroupsTypeBySeqWithLoad(userid, type, ids, dict) && !dict.empty()) {
    groupviews.swap(dict.begin()->second);
    return true;
  } else {
    return false;
  }

}

bool TalkGroupViewClient::GetRecentGroupsTypeBySeqWithLoad(int actor,int type, const MyUtil::IntSeq& ids, TalkGroupViewSeqMap &groupviews){
	TalkGroupViewSeqMap &ans = groupviews;
	if(!GetRecentGroupsByType(ids, type, ans)){
		return false;
	}

	MCE_INFO("TalkGroupViewClient::GetRecentGroupsTypeBySeqWithLoad --> " << actor<< " ids:" << ids.size() << " " << ans.size() << " type:" << type);
	if(ids.size() == ans.size()){
		return true;
	}

	TalkGroupViewSeqMap loadans;
	IntSeq loadids;
	TalkGroupViewSeqMap::iterator dit;
	for(MyUtil::IntSeq::const_iterator idit = ids.begin(); idit != ids.end(); ++idit){
		dit = ans.find((*idit));
		if(dit == ans.end() || dit->second.size() == 0){
			loadids.push_back((*idit));
		}
	}
	if(!loadids.empty()){
		try{
			TalkGroupViewLoaderAdapter::instance().LoadBySeq(loadids, type, loadans);
			MCE_INFO("TalkGroupViewClient::GetRecentGroupsTypeBySeqWithLoad --> CALL TalkRecentLoader.LoadBySeq actor:" << actor
               << " loadids.size:" << loadids.size() << " loadans.size:" << loadans.size());
		}
		catch(std::exception& e){
			MCE_WARN("TalkGroupViewClient::GetRecentGroupsTypeBySeqWithLoad --> CALL TalkRecentLoader.LoadBySeq err" << e.what());
		}catch(...){
			MCE_WARN("TalkGroupViewClient::GetRecentGroupsTypeBySeqWithLoad --> CALL TalkRecentLoader.LoadBySeq err");
		}
		if(!loadans.empty()){
			ans.insert(loadans.begin(), loadans.end());
		}
	}
	return true;
}

}
}
}

