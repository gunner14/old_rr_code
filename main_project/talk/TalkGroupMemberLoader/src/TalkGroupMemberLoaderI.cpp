#include <sstream>
#include <time.h>
#include "util/cpp/String.h"
#include "QueryRunner.h"
#include "TalkGroupMemberLoaderI.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include <boost/lexical_cast.hpp>
#include "XmppTools.h"
#include "DbOpe.h"
using namespace MyUtil;
using namespace xce::talk;
using namespace com::xiaonei::xce;
using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk::group::memberloader;

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&TalkGroupMemberLoaderManagerI::instance(), service.createIdentity("M", ""));

	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
	xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerTalkGroupMemberLoader", &TalkGroupMemberLoaderManagerI::instance(), mod, interval, new XceFeedControllerChannel());
	MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);

}
TalkGroupMemberLoaderManagerI::TalkGroupMemberLoaderManagerI(){
}


void TalkGroupMemberLoaderManagerI::strSplit(const string &list_str,GroupMemberSeq& group_member) {
  group_member.clear();
  set<int> uids; // 去重使用
	vector<string> tokens;
	boost::split(tokens, list_str, boost::is_any_of(","));
	for (size_t i = 0; i < tokens.size(); ++ i) {
		vector<string> token;
		GroupMemberPtr member = new GroupMember();
		boost::split(token, tokens[i], boost::is_any_of(":"));
		if(token.size() == 2){
			try{
				member->uid = boost::lexical_cast<int>(token[0]);
				member->level = boost::lexical_cast<int>(token[1]);
        // 过滤重复的
        if (uids.end() == uids.find(member->uid)) {
          uids.insert(member->uid);
          group_member.push_back(member);
        } else {
          MCE_WARN("TalkGroupMemberLoaderManagerI::strSplit --> duplicate uid:" << member->uid);
        }

			}catch(boost::bad_lexical_cast& exp){
				MCE_WARN("TalkGroupMemberLoaderManagerI::strSplit --> boost err");
			}
		}
	}
}


void TalkGroupMemberLoaderManagerI::SerializeMembers(const GroupMemberSeq& members,
		string &info_str){
  info_str.clear();
  set<int> uids; // 去重使用
	const GroupMemberSeq &MList = members;

	for(GroupMemberSeq::const_iterator iter = MList.begin(); iter != MList.end(); ++iter) {
    if (uids.end() == uids.find((*iter)->uid)) {
      if (!info_str.empty()) {
        info_str += ",";
      }
      
      info_str += boost::lexical_cast<string>((*iter)->uid) + ":" + boost::lexical_cast<string>((*iter)->level);
      uids.insert((*iter)->uid);
    } else {
      MCE_WARN("TalkGroupMemberLoaderManagerI::SerializeMembers --> duplicate uid:" << (*iter)->uid);
    }
	}

}


bool TalkGroupMemberLoaderManagerI::getGroupMemberFromDb(const MyUtil::LongSeq& idSeq, GroupMemberDict& member_list){
  map<long, LongSeq> mods;
  for (MyUtil::LongSeq::const_iterator it = idSeq.begin(); it != idSeq.end(); ++it) {
    long m = *it % TABLE_COUNT;
    if (mods.find(m) == mods.end()) {
      mods[m] = LongSeq();
    }
    mods[m].push_back(*it);
  }
  MCE_INFO("TalkGroupMemberLoaderManagerI::getGroupMemberFromDb --> size:" << idSeq.size() << " mod size:" << mods.size());
  for (map<long, LongSeq>::const_iterator it = mods.begin(); it != mods.end(); ++it) {
    if (!getGroupMemberFromDbOneMod(it->first, it->second, member_list)) {
      return false;
    }
  }

  return true;

}

bool TalkGroupMemberLoaderManagerI::getGroupMemberFromDbOneMod(long mod, const MyUtil::LongSeq& idSeq, GroupMemberDict& member_list){
  MCE_INFO("TalkGroupMemberLoaderManagerI::getGroupMemberFromDbOneMod --> mod:" << mod << " size:" << idSeq.size());

  GroupMemberSeq list_str;
  ostringstream sql;

  sql << "select * from " << DbOpe::GetDbTable(DB_PREF, TABLE_COUNT, mod) << " where session_id in (" ;
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
    MCE_WARN("TalkGroupMemberLoaderManagerI::getGroupMemberFromDbOneMod --> dosql error");
    return false;
  }


  for (size_t i = 0; i < res.num_rows(); ++i) {
    mysqlpp::Row row = res.at(i);
    strSplit((string)row["session_member"],list_str);
    long sid = (long)row["session_id"];
    MCE_DEBUG("TalkGroupMemberLoaderManagerI::getGroupMemberFromDbOneMod --> sid:" << sid << " member:" << (string)row["session_member"]);
    member_list.insert(pair<long, GroupMemberSeq>(sid, list_str));
  }

  TalkGroupMemberClient::instance().SetGroupMember(member_list);

  return true;
}


GroupMemberDict TalkGroupMemberLoaderManagerI::LoadGroupMember(const MyUtil::LongSeq& idSeq, const Ice::Current &current){ //  guoqing.liu 用批量的接口
	TimeStatN ts;
	GroupMemberDict memberDict;
  bool rv = getGroupMemberFromDb(idSeq, memberDict);
	FunStatManager::instance().StatPlus(current, "TalkGroupMemberLoaderManagerI::LoadGroupMember", ts.getTime(), (ts.getTime() > 200));
  if (rv) {
    return memberDict;
  } else {
    MCE_WARN("TalkGroupMemberLoaderManagerI::LoadGroupMember --> load error");
    return GroupMemberDict();
  }


}

bool TalkGroupMemberLoaderManagerI::DeleteGroup_(const MyUtil::LongSeq &idSeq){
  map<long, LongSeq> mods;
  for (MyUtil::LongSeq::const_iterator it = idSeq.begin(); it != idSeq.end(); ++it) {
    long m = *it % TABLE_COUNT;
    if (mods.find(m) == mods.end()) {
      mods[m] = LongSeq();
    }
    mods[m].push_back(*it);
  }
  MCE_INFO("TalkGroupMemberLoaderManagerI::DeleteGroup_ --> size:" << idSeq.size() << " mod size:" << mods.size());
  for (map<long, LongSeq>::const_iterator it = mods.begin(); it != mods.end(); ++it) {
    if (!DeleteGroupOneMod(it->first, it->second)) {
      return false;
    }
  }

  return true;

}

bool TalkGroupMemberLoaderManagerI::DeleteGroupOneMod(long mod, const MyUtil::LongSeq &idSeq){
  MCE_INFO("TalkGroupMemberLoaderManagerI::getGroupMemberFromDbOneMod --> mod:" << mod << " size:" << idSeq.size());
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
    MCE_WARN("TalkGroupMemberLoaderManagerI::DeleteGroup_ --> dosql error");
    return false;
  }
    
  TalkGroupMemberClient::instance().DeleteGroup(idSeq);
  return true;

}

bool TalkGroupMemberLoaderManagerI::updateMembers(const GroupMemberDict& memberdict) {
  map<long, GroupMemberDict> mods;
	for(GroupMemberDict::const_iterator it = memberdict.begin(); it != memberdict.end(); ++it) {
    long m = it->first % TABLE_COUNT;
    if (mods.find(m) == mods.end()) {
      mods[m] = GroupMemberDict();
    }
    mods[m][it->first] = it->second;
  }
  MCE_INFO("TalkGroupMemberLoaderManagerI::updateMembers --> size:" << memberdict.size() << " mod size:" << mods.size());
  for (map<long, GroupMemberDict>::const_iterator it = mods.begin(); it != mods.end(); ++it) {
    if (!updateMembersOneMod(it->first, it->second)) {
      return false;
    }
  }

  return true;


}

bool TalkGroupMemberLoaderManagerI::updateMembersOneMod(long mod, const GroupMemberDict& memberdict) {
  MCE_INFO("TalkGroupMemberLoaderManagerI::getGroupMemberFromDbOneMod --> mod:" << mod << " size:" << memberdict.size());
  ostringstream sql;
  string list_str;

  // 批量更新
  // insert into session_member_str  (session_id, session_member) values (33, '12:1'), (55, '12:1') on duplicate key update session_member = values(session_member)
  sql << "insert into " << DbOpe::GetDbTable(DB_PREF, TABLE_COUNT, mod) << " (session_id, session_member) values ";
  size_t count = 0;
	for(GroupMemberDict::const_iterator iter = memberdict.begin(); iter != memberdict.end(); ++iter, ++count) {
    SerializeMembers(iter->second, list_str);
    sql << "(" << iter->first << ", '" << list_str << "')";

    if (count < memberdict.size() - 1) {
      sql << ",";
    }
  }
  sql << " on duplicate key update session_member = values(session_member)";


  bool rv = DbOpe::SqlWrite(DB_INSTANCE, DbOpe::GetDbRoute(DB_ROUTE, TABLE_COUNT, mod).c_str(), sql.str().c_str());

  if (!rv) {
    MCE_WARN("TalkGroupMemberLoaderManagerI::updateMembers --> dosql error");
    return false;
  }

  TalkGroupMemberClient::instance().SetGroupMember(memberdict);

  return true;

}


bool TalkGroupMemberLoaderManagerI::AddNewMember_(const long sid, const GroupMemberSeq &members){
  MyUtil::LongSeq session_ids;
  session_ids.push_back(sid);
  GroupMemberDict gm;
  if (!getGroupMemberFromDb(session_ids, gm) || gm.empty()) {
    MCE_WARN("TalkGroupMemberLoaderManagerI::AddNewMember_ --> getgroup error sid:" << sid );
    return false;

  }
  GroupMemberSeq &oldmem = gm.begin()->second;
  oldmem.insert(oldmem.end(), members.begin(), members.end());
  return updateMembers(gm);
}

bool TalkGroupMemberLoaderManagerI::RemoveMember_(const long sid, const MyUtil::IntSeq &idSeq){
  MyUtil::LongSeq session_ids;
  session_ids.push_back(sid);
  GroupMemberDict gm;
  if (!getGroupMemberFromDb(session_ids, gm) || gm.empty()) {
    MCE_WARN("TalkGroupMemberLoaderManagerI::RemoveMember_ --> getgroup error sid:" << sid );
    return false;
  }


  GroupMemberSeq &oldmem = gm.begin()->second;

  set<int> delids;
  for (MyUtil::IntSeq::const_iterator it = idSeq.begin(); it != idSeq.end(); ++it) {
    delids.insert(*it);
  }


  for (GroupMemberSeq::iterator it = oldmem.begin(); it != oldmem.end();) {
    if (delids.find((*it)->uid) != delids.end()) {
      it = oldmem.erase(it);
    } else {
      ++it;
    }
    
  }

  return updateMembers(gm);

}

bool TalkGroupMemberLoaderManagerI::SetMember_(const GroupMemberDict& memberdict) {
  MyUtil::LongSeq session_ids;
	for(GroupMemberDict::const_iterator iter = memberdict.begin(); iter != memberdict.end(); ++iter) {
    session_ids.push_back(iter->first);
  }

  GroupMemberDict gm;
  if (!getGroupMemberFromDb(session_ids, gm) || gm.empty()) {
    MCE_WARN("TalkGroupMemberLoaderManagerI::SetMember_ --> getgroup error" );
    return false;
  }

  // 这里代码非常丑陋，但是怎么办呢？应该数据量不大，没有必要建立索引的查询方式把
	for(GroupMemberDict::const_iterator iter = memberdict.begin(); iter != memberdict.end(); ++iter) {
    // 先找到对应的老的sid对应的列表
    GroupMemberDict::iterator it = gm.find(iter->first);
    if (it == gm.end()) {
      continue;
    }
    // 循环老的列表，并对比新的member数据，并修改到新的值
    for (GroupMemberSeq::iterator oit = it->second.begin(); oit != it->second.end(); ++oit) {
      // 新的可能设置多个，只需要找到需要的uid进行设置即可
      for (GroupMemberSeq::const_iterator nit = iter->second.begin(); nit != iter->second.end(); ++nit) {
        if ((*nit)->uid == (*oit)->uid) {
          (*oit)->level = (*nit)->level;
          break;
        }
      }

    }

  }


  return updateMembers(gm);
}




// rpc 接口
bool TalkGroupMemberLoaderManagerI::AddNewGroup(const GroupMemberDict& memberdict, const Ice::Current  &current) {
	TimeStatN ts;
  bool rv = updateMembers(memberdict);
	FunStatManager::instance().StatPlus(current, "TalkGroupMemberLoaderManagerI::AddNewGroup", ts.getTime(), (ts.getTime() > 200));
  return rv;
}

bool TalkGroupMemberLoaderManagerI::DeleteGroup(const MyUtil::LongSeq &idSeq, const Ice::Current  &current) {
	TimeStatN ts;
  bool rv = DeleteGroup_(idSeq);
	FunStatManager::instance().StatPlus(current, "TalkGroupMemberLoaderManagerI::DeleteGroup", ts.getTime(), (ts.getTime() > 200));
  return rv;
}

bool TalkGroupMemberLoaderManagerI::AddNewMember(const long sid, const GroupMemberSeq &members, const Ice::Current &current) {
	TimeStatN ts;
  bool rv = AddNewMember_(sid, members);
	FunStatManager::instance().StatPlus(current, "TalkGroupMemberLoaderManagerI::AddNewMember", ts.getTime(), (ts.getTime() > 200));
  return rv;
}

bool TalkGroupMemberLoaderManagerI::RemoveMember(const long sid, const MyUtil::IntSeq &idSeq, const Ice::Current  &current) {
	TimeStatN ts;
  bool rv = RemoveMember_(sid, idSeq);
	FunStatManager::instance().StatPlus(current, "TalkGroupMemberLoaderManagerI::RemoveMember", ts.getTime(), (ts.getTime() > 200));
  return rv;
}

bool TalkGroupMemberLoaderManagerI::SetMember(const GroupMemberDict& memberdict, const Ice::Current  &current) {
	TimeStatN ts;
  bool rv = SetMember_(memberdict);
	FunStatManager::instance().StatPlus(current, "TalkGroupMemberLoaderManagerI::SetMember", ts.getTime(), (ts.getTime() > 200));
  return rv;
}

void TalkGroupMemberLoaderManagerI::AddNewGroupAsyn(const GroupMemberDict& memberdict, const Ice::Current  &current) {
	TimeStatN ts;
  updateMembers(memberdict);
	FunStatManager::instance().StatPlus(current, "TalkGroupMemberLoaderManagerI::AddNewGroupAsyn", ts.getTime(), (ts.getTime() > 200));
}

void TalkGroupMemberLoaderManagerI::DeleteGroupAsyn(const MyUtil::LongSeq &idSeq, const Ice::Current  &current) {
	TimeStatN ts;
  DeleteGroup_(idSeq);
	FunStatManager::instance().StatPlus(current, "TalkGroupMemberLoaderManagerI::DeleteGroupAsyn", ts.getTime(), (ts.getTime() > 200));
}

void TalkGroupMemberLoaderManagerI::AddNewMemberAsyn(const long sid, const GroupMemberSeq &members, const Ice::Current& current) {
	TimeStatN ts;
  AddNewMember_(sid, members);
	FunStatManager::instance().StatPlus(current, "TalkGroupMemberLoaderManagerI::AddNewMemberAsyn", ts.getTime(), (ts.getTime() > 200));
}

void TalkGroupMemberLoaderManagerI::RemoveMemberAsyn(const long sid, const MyUtil::IntSeq &idSeq, const Ice::Current  &current) {
	TimeStatN ts;
  RemoveMember_(sid, idSeq);
	FunStatManager::instance().StatPlus(current, "TalkGroupMemberLoaderManagerI::RemoveMemberAsyn", ts.getTime(), (ts.getTime() > 200));
}

void TalkGroupMemberLoaderManagerI::SetMemberAsyn(const GroupMemberDict& memberdict, const Ice::Current  &current) {
	TimeStatN ts;
  SetMember_(memberdict);
	FunStatManager::instance().StatPlus(current, "TalkGroupMemberLoaderManagerI::SetMemberAsyn", ts.getTime(), (ts.getTime() > 200));
}

