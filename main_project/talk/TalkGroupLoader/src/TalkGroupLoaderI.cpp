#include <sstream>
#include <time.h>
#include "util/cpp/String.h"
#include "QueryRunner.h"
#include "TalkGroupLoaderI.h"
#include "FeedMemcProxy/client/talk_cache_client.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include <boost/lexical_cast.hpp>
#include "XmppTools.h"
#include "DbOpe.h"
using namespace com::xiaonei::talk::util;
using namespace MyUtil;
using namespace xce::talk;
using namespace com::xiaonei::xce;
using namespace com::xiaonei::talk::group::loader;

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&TalkGroupLoaderManagerI::instance(), service.createIdentity("M", ""));

	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
	xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerTalkGroupLoader", &TalkGroupLoaderManagerI::instance(), mod, interval, new MyUtil::XceFeedControllerChannel());
	MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);

}

TalkGroupLoaderManagerI::TalkGroupLoaderManagerI(){
	_prop.insert("session_id");
	_prop.insert("session_name");
	_prop.insert("session_head");
	_prop.insert("session_creater");
	_prop.insert("session_create_time");
	_prop.insert("session_intro");
	_prop.insert("session_type");
	_prop.insert("session_last_content");
	_prop.insert("session_last_time");
}

GroupInfoPtr TalkGroupLoaderManagerI::GenInfo(const Ice::Long session_id, const string& session_name,  
		const string& session_head, int session_creater,  const string& create_time, 
		const string& session_intro, int session_type, const string& session_last_content, const  string& session_last_time,
		const string& announcement, const string& authentication, const string& notify,const int level,
    const int toplimit, const int saveonline) {
	GroupInfoPtr info_cont = new GroupInfo();
	info_cont->id = session_id;
	info_cont->name = session_name;
	info_cont->head = session_head;
	info_cont->creator = session_creater;
	info_cont->createTime = create_time;
	info_cont->introduction = session_intro;
	info_cont->type = session_type;
	info_cont->lastContent = session_last_content;
	info_cont->lastTime = session_last_time;
	info_cont->announcement = announcement;
	info_cont->authentication = authentication;
	info_cont->notify = notify;
	info_cont->level = level;
	info_cont->toplimit = toplimit;
	info_cont->saveonline = saveonline;
	return info_cont;
}

Ice::Long TalkGroupLoaderManagerI::identifyGroupInfo(MyUtil::Str2StrMap props){
	Ice::Long judge_id = 0;  // guoqing.liu   用 Ice::Long  不能用 long 
	for(MyUtil::Str2StrMap::iterator iter= props.begin(); iter != props.end(); ++iter){
		if(!strcmp("session_id",iter->first.c_str())){
			judge_id = boost::lexical_cast<Ice::Long>(iter->second);
		}
		if(_prop.find(iter->first) == _prop.end()){
			MCE_WARN("TalkGroupLoaderManagerI::identifyGroupInfo WARN props key" << iter->first)
				return 0;
		}
	}
	if(judge_id){
		return judge_id;
	}
	return 0;
}

bool TalkGroupLoaderManagerI::getInfoFromDb(const MyUtil::LongSeq& idSeq, GroupInfoDict& infos) {
  map<long, LongSeq> mods;
  for (MyUtil::LongSeq::const_iterator it = idSeq.begin(); it != idSeq.end(); ++it) {
    long m = *it % TABLE_COUNT;
    if (mods.find(m) == mods.end()) {
      mods[m] = LongSeq();
    }
    mods[m].push_back(*it);
  }
  MCE_INFO("TalkGroupLoaderManagerI::getInfoFromDb --> size:" << idSeq.size() << " mod size:" << mods.size());
  for (map<long, LongSeq>::const_iterator it = mods.begin(); it != mods.end(); ++it) {
    if (!getInfoFromDbOneMod(it->first, it->second, infos)) {
      return false;
    }
  }

  return true;

}

bool TalkGroupLoaderManagerI::getInfoFromDbOneMod(long mod, const MyUtil::LongSeq& idSeq, GroupInfoDict& infos) {
  MCE_INFO("TalkGroupLoaderManagerI::getInfoFromDbOneMod --> mod:" << mod << " size:" << idSeq.size());
  ostringstream sql;

  sql << "select * from " << DbOpe::GetDbTable(DB_PREF, TABLE_COUNT, mod) << " where session_id in (";
  for (MyUtil::LongSeq::const_iterator it = idSeq.begin(); it != idSeq.end(); ++it) {
    sql << *it;
    if (it != idSeq.end() - 1) {
      sql << ", ";
    }
  }
  sql << ")";
  mysqlpp::StoreQueryResult res;
  bool rv = DbOpe::SqlRead(DB_INSTANCE, DbOpe::GetDbRoute(DB_ROUTE, TABLE_COUNT, mod).c_str(), sql.str().c_str(), res);	
  if (!rv) {
    MCE_WARN("TalkGroupLoaderManagerI::getInfoFromDb --> dosql error");
    return false;
  }

  GroupInfoPtr info;
  for (size_t i = 0; i < res.num_rows(); ++i) {
    mysqlpp::Row row = res.at(i);
    info = GenInfo((Ice::Long)row["session_id"], (string)row["session_name"],(string)row["session_head"],(int)row["session_creater"],(string)row["create_time"],(string)row["session_intro"], (int)row["session_type"],(string)row["session_last_content"],(string)row["session_last_time"],(string)row["announcement"],(string)row["authentication"],(string)row["notify"],(int)row["level"],(int)row["toplimit"],(int)row["saveonline"]);

    TalkGroupInfoClient::instance().SetGroupInfo(info);

    infos[(Ice::Long)row["session_id"]] = info;
  }



  return true;

}




GroupInfoDict TalkGroupLoaderManagerI::LoadGroupInfo(const MyUtil::LongSeq& idSeq, const Ice::Current &current){ //  guoqing.liu 用批量的接口
	TimeStatN ts;
	GroupInfoDict infoDict;
  bool rv = getInfoFromDb(idSeq, infoDict);
	FunStatManager::instance().StatPlus(current, "TalkGroupLoaderManagerI::LoadGroupInfo", ts.getTime(), (ts.getTime() > 200));
  if (rv) {
    return infoDict;
  } else {
    MCE_WARN("TalkGroupLoaderManagerI::LoadGroupInfo --> load error");
    return GroupInfoDict();
  }


}
/*
// 注意这个函数是没有cache的！！！！！
MyUtil::LongSeq TalkGroupLoaderManagerI::GetCreatSessions(int creater, const Ice::Current &current){
	TimeStatN ts;
  LongSeq createSessionSeq;

  ostringstream sql;
  sql << "select session_id from session_info"  << " where session_creater = " << creater
			<< " limit 100;" ;

  mysqlpp::StoreQueryResult res;
  bool rv = DbOpe::SqlRead(DB_INSTANCE, DB_ROUTE, sql.str().c_str(), res);	
  if (!rv) {
    MCE_WARN("TalkGroupLoaderManagerI::GetCreatSessions --> dosql error");
    FunStatManager::instance().StatPlus(current, "TalkGroupLoaderManagerI::GetCreatSessions.err", ts.getTime(), (ts.getTime() > 200));
    return createSessionSeq;
  }


  for (size_t i = 0; i < res.num_rows(); ++i) {
    mysqlpp::Row row = res.at(i);
    long sid = (Ice::Long)row["session_id"];
    createSessionSeq.push_back(sid);
  }

	FunStatManager::instance().StatPlus(current, "TalkGroupLoaderManagerI::GetCreatSessions.ok", ts.getTime(), (ts.getTime() > 200));
 	return createSessionSeq;
}
*/
bool TalkGroupLoaderManagerI::SetGroupInfo_(const GroupInfoPtr& groupInfo) {
  /*
  if (groupInfo == 0) {
    return false;
  }
  ostringstream sql;

  sql << "update session_info set  (session_id,session_name,session_head,session_creater,session_intro,session_type,session_last_content,session_last_time, announcement,authentication, notify, level, toplimit, saveonline ) values(" << groupInfo->id << ",'" << groupInfo->name << "','" << groupInfo->head << "'," << groupInfo->creator << ",'" << groupInfo->introduction << "'," << groupInfo->type << ",'" << groupInfo->lastContent << "','" << groupInfo->lastTime <<"','" << groupInfo->announcement <<"','"<< groupInfo->authentication <<"','"<< groupInfo->notify <<"','"<< groupInfo->level <<"','"<< groupInfo->toplimit <<"','"<< groupInfo->saveonline<<"');";


  bool rv = DbOpe::SqlWrite(DB_INSTANCE.c_str(), "session_info", sql.str().c_str());	
  if (!rv) {
    return false;
  }
  TalkGroupInfoClient::instance().SetGroupInfo(groupInfo);
  */
  return true;

}

bool TalkGroupLoaderManagerI::AddGroupInfo_(const GroupInfoPtr& groupInfo) {
  if (groupInfo == 0) {
    return false;
  }
  ostringstream sql;

  sql << "insert into " << DbOpe::GetDbTable(DB_PREF, TABLE_COUNT, groupInfo->id) << " (session_id,session_name,session_head,session_creater,session_intro,session_type,session_last_content,session_last_time, announcement,authentication, notify, level, toplimit, saveonline ) values(" << groupInfo->id << ",'" << EscapeString(groupInfo->name) << "','" << groupInfo->head << "'," << groupInfo->creator << ",'" << EscapeString(groupInfo->introduction) << "'," << groupInfo->type << ",'" << groupInfo->lastContent << "','" << groupInfo->lastTime <<"','" << EscapeString(groupInfo->announcement) <<"','"<< groupInfo->authentication <<"','"<< groupInfo->notify <<"','"<< groupInfo->level <<"','"<< groupInfo->toplimit <<"','"<< groupInfo->saveonline<<"');";


  bool rv = DbOpe::SqlWrite(DB_INSTANCE, DbOpe::GetDbRoute(DB_ROUTE, TABLE_COUNT, groupInfo->id).c_str(), sql.str().c_str());	
  if (!rv) {
    MCE_WARN("TalkGroupLoaderManagerI::AddGroupInfo_ --> dosql error");
    return false;
  }
  TalkGroupInfoClient::instance().SetGroupInfo(groupInfo);

  return true;

}

bool TalkGroupLoaderManagerI::DeleteGroupInfo_(const MyUtil::LongSeq& idSeq) {
  map<long, LongSeq> mods;
  for (MyUtil::LongSeq::const_iterator it = idSeq.begin(); it != idSeq.end(); ++it) {
    long m = *it % TABLE_COUNT;
    if (mods.find(m) == mods.end()) {
      mods[m] = LongSeq();
    }
    mods[m].push_back(*it);
  }
  MCE_INFO("TalkGroupLoaderManagerI::DeleteGroupInfo_ --> size:" << idSeq.size() << " mod size:" << mods.size());
  for (map<long, LongSeq>::const_iterator it = mods.begin(); it != mods.end(); ++it) {
    if (!DeleteGroupInfoOneMod(it->first, it->second)) {
      return false;
    }
  }

  return true;

}

bool TalkGroupLoaderManagerI::DeleteGroupInfoOneMod(long mod, const MyUtil::LongSeq& idSeq) {
  MCE_INFO("TalkGroupLoaderManagerI::DeleteGroupInfoOneMod --> mod:" << mod << " size:" << idSeq.size());
  ostringstream sql;

  sql << "delete from " << DbOpe::GetDbTable(DB_PREF, TABLE_COUNT, mod) << " where session_id in (";
  for (MyUtil::LongSeq::const_iterator it = idSeq.begin(); it != idSeq.end(); ++it) {
    sql << *it;
    if (it != idSeq.end() - 1) {
      sql << ", ";
    }
  }
  sql << ")";

  bool rv = DbOpe::SqlWrite(DB_INSTANCE, DbOpe::GetDbRoute(DB_ROUTE, TABLE_COUNT, mod).c_str(), sql.str().c_str());
  if (!rv) {
    MCE_WARN("TalkGroupLoaderManagerI::DeleteGroupInfo_ --> dosql error");
    return false;
  }


  for (MyUtil::LongSeq::const_iterator it = idSeq.begin(); it != idSeq.end(); ++it) {
    TalkGroupInfoClient::instance().DeleteGroupInfo(*it);
  }
  

  return true;

}



bool TalkGroupLoaderManagerI::UpdateLastContent_(long sid, const string &lastcontent, const string &lasttime) {
  ostringstream sql;
  sql << "update " << DbOpe::GetDbTable(DB_PREF, TABLE_COUNT, sid) << " set ";
  sql << " session_last_content = \"" << EscapeString(lastcontent) << "\","
      << " session_last_time = \"" << lasttime << "\" ";
  sql << " where session_id = " << sid;
  bool rv = DbOpe::SqlWrite(DB_INSTANCE, DbOpe::GetDbRoute(DB_ROUTE, TABLE_COUNT, sid).c_str(), sql.str().c_str());	
  if (!rv) {
    MCE_WARN("TalkGroupLoaderManagerI::UpdateLastContent_ --> dosql error");
    return false;
  }

  GroupInfoPtr info;
  bool rvinfo = TalkGroupInfoClient::instance().GetGroupInfoByIdWithLoad(sid, info);
  if (!rvinfo) {
    LongSeq idSeq;
    GroupInfoDict infDict;
    idSeq.push_back(sid);
    if (getInfoFromDb(idSeq, infDict) && !infDict.empty()) {
      info = infDict.begin()->second;
    } else {
      return false;
    }

  } else {
    info->lastContent = lastcontent;
    info->lastTime = lasttime;
  }
  TalkGroupInfoClient::instance().SetGroupInfo(info);


  return true;

}

bool TalkGroupLoaderManagerI::UpdateGroupInfo_(long sid, const MyUtil::Str2StrMap& props) {
  ostringstream sql;
  sql << "update " << DbOpe::GetDbTable(DB_PREF, TABLE_COUNT, sid) << " set ";
  size_t count = 0;
  for(MyUtil::Str2StrMap::const_iterator it = props.begin(); it != props.end(); ++it, ++count){
    sql << it->first << " = " << EscapeString(it->second, 1);	
    if (count < props.size() - 1) {
      sql << ",";
    }
  }
  sql << " where session_id = " << sid;
  bool rv = DbOpe::SqlWrite(DB_INSTANCE, DbOpe::GetDbRoute(DB_ROUTE, TABLE_COUNT, sid).c_str(), sql.str().c_str());	
  if (!rv) {
    MCE_WARN("TalkGroupLoaderManagerI::UpdateGroupInfo_ --> dosql error");
    return false;
  }

  LongSeq idSeq;
	GroupInfoDict infDict;
  idSeq.push_back(sid);
  if (getInfoFromDb(idSeq, infDict) && !infDict.empty()) {
    TalkGroupInfoClient::instance().SetGroupInfo(infDict.begin()->second);
  } else {
    return false;
  }

  return true;

}

bool TalkGroupLoaderManagerI::UpdateLastContent(long sid, const string &lastcontent, const string &lasttime, const Ice::Current &current) {
	TimeStatN ts;

  bool rv = UpdateLastContent_(sid, lastcontent, lasttime);
	FunStatManager::instance().StatPlus(current, "TalkGroupLoaderManagerI::UpdateLastContent", ts.getTime(), (ts.getTime() > 200));
  return rv;

}

bool TalkGroupLoaderManagerI::SetGroupInfo(const GroupInfoPtr &groupInfo, const Ice::Current &current){
	TimeStatN ts;
  bool rv = SetGroupInfo_(groupInfo);
	FunStatManager::instance().StatPlus(current, "TalkGroupLoaderManagerI::SetGroupInfo", ts.getTime(), (ts.getTime() > 200));
  return rv;
}

bool TalkGroupLoaderManagerI::AddGroupInfo(const GroupInfoPtr &groupInfo, const Ice::Current &current){
	TimeStatN ts;
  bool rv = AddGroupInfo_(groupInfo);
	FunStatManager::instance().StatPlus(current, "TalkGroupLoaderManagerI::AddGroupInfo", ts.getTime(), (ts.getTime() > 200));
  return rv;
}

bool TalkGroupLoaderManagerI::DeleteGroupInfo(const MyUtil::LongSeq& idSeq, const Ice::Current  &current){
	TimeStatN ts;
  bool rv = DeleteGroupInfo_(idSeq);
	FunStatManager::instance().StatPlus(current, "TalkGroupLoaderManagerI::DeleteGroupInfo", ts.getTime(), (ts.getTime() > 200));
  return rv;
}

bool TalkGroupLoaderManagerI::UpdateGroupInfo(long sid, const MyUtil::Str2StrMap& props, const Ice::Current  &current){
	TimeStatN ts;
  bool rv = UpdateGroupInfo_(sid, props);
	FunStatManager::instance().StatPlus(current, "TalkGroupLoaderManagerI::UpdateGroupInfo", ts.getTime(), (ts.getTime() > 200));
  return rv;
}


void TalkGroupLoaderManagerI::SetGroupInfoAsyn(const GroupInfoPtr& groupInfo, const Ice::Current  &current) {
	TimeStatN ts;
  SetGroupInfo_(groupInfo);
  FunStatManager::instance().StatPlus(current, "TalkGroupLoaderManagerI::SetGroupInfoAsyn", ts.getTime(), (ts.getTime() > 200));
}

void TalkGroupLoaderManagerI::AddGroupInfoAsyn(const GroupInfoPtr& groupInfo, const Ice::Current  &current) {
	TimeStatN ts;
  AddGroupInfo_(groupInfo);
	FunStatManager::instance().StatPlus(current, "TalkGroupLoaderManagerI::AddGroupInfoAsyn", ts.getTime(), (ts.getTime() > 200));
}
 
void TalkGroupLoaderManagerI::DeleteGroupInfoAsyn(const MyUtil::LongSeq& idSeq, const Ice::Current  &current) {
	TimeStatN ts;
  DeleteGroupInfo_(idSeq);
	FunStatManager::instance().StatPlus(current, "TalkGroupLoaderManagerI::DeleteGroupInfoAsyn", ts.getTime(), (ts.getTime() > 200));
}
 
void TalkGroupLoaderManagerI::UpdateGroupInfoAsyn(long sid, const MyUtil::Str2StrMap& props, const Ice::Current  &current) {
	TimeStatN ts;
  UpdateGroupInfo_(sid, props);
	FunStatManager::instance().StatPlus(current, "TalkGroupLoaderManagerI::UpdateGroupInfoAsyn", ts.getTime(), (ts.getTime() > 200));
}

string TalkGroupLoaderManagerI::EscapeString(const string &raw_string, const bool mode) {
  int begin = 0;
  int end = raw_string.length();
  if (mode == 1) {
    begin = 1;
    end -= 1;
  }
  ostringstream os;
  if (mode == 1)
    os << raw_string.substr(0, begin);
  for (size_t i=begin; i<end; ++i) {
    if (raw_string[i]=='\'' ||raw_string[i]=='%' ||raw_string[i]=='\"' ) {
      os << '\\';
    }
    os << raw_string[i];
  }
  if (mode == 1)
    os << raw_string.substr(end,raw_string.length()-end);
  return os.str();
}
