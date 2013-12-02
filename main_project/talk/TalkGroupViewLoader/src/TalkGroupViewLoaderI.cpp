#include "TalkGroupViewLoaderI.h"
#include "QueryRunner.h"
#include <sstream>
#include <time.h>
#include "util/cpp/String.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "XmppTools.h"
#include "DbOpe.h"
using namespace MyUtil;
using namespace xce::talk;
using namespace xce::talk::recent;
using namespace com::xiaonei::xce;
using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk::groupview::loader;

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&GroupViewLoaderManagerI::instance(), service.createIdentity("M", ""));

	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
	xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerTalkGroupViewLoader", &GroupViewLoaderManagerI::instance(), mod, interval, new XceFeedControllerChannel());
	MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);

}

TalkGroupViewPtr GroupViewLoaderManagerI::GenInfo(int uid, long sessionid, int isvisible, int hasread, int sessiontype, const string &updatetime, int affiliation, int saveonline, const string &notify, long msgid) {
	TalkGroupViewPtr item = new TalkGroupView();
	item->sessionId = sessionid;
	item->userId = uid;
	item->isVisible = isvisible;
	item->hasRead = hasread;
	item->sessionType = sessiontype;
	item->updateTime = updatetime;
  item->affiliation = affiliation;
  item->saveonline = saveonline;
  item->notify = notify;
  item->msgid = msgid;
	return item;
}

// 注意这个函数是没有cache的！！！！！
MyUtil::LongSeq GroupViewLoaderManagerI::GetCreatSessions(int creater, const Ice::Current &current){
	TimeStatN ts;
  LongSeq createSessionSeq;

  ostringstream sql;
  sql << "select session_id from " << DbOpe::GetDbTable(DB_PREF, TABLE_COUNT, creater)
      << " where user_id = " << creater
      << " and affiliation = 0"
			<< " limit 100;" ;

  mysqlpp::StoreQueryResult res;
  bool rv = DbOpe::SqlRead(DB_INSTANCE, DbOpe::GetDbRoute(DB_ROUTE, TABLE_COUNT, creater).c_str(), sql.str().c_str(), res);	
  if (!rv) {
    MCE_WARN("GroupViewLoaderManagerI::GetCreatSessions --> dosql error");
    FunStatManager::instance().StatPlus(current, "GroupViewLoaderManagerI::GetCreatSessions.err", ts.getTime(), (ts.getTime() > 200));
    return createSessionSeq;
  }


  for (size_t i = 0; i < res.num_rows(); ++i) {
    mysqlpp::Row row = res.at(i);
    long sid = (Ice::Long)row["session_id"];
    createSessionSeq.push_back(sid);
  }

	FunStatManager::instance().StatPlus(current, "GroupViewLoaderManagerI::GetCreatSessions.ok", ts.getTime(), (ts.getTime() > 200));
 	return createSessionSeq;
}


bool GroupViewLoaderManagerI::LocateUser(const MyUtil::IntSeq &idSeq, int type, TalkGroupViewSeqMap &gviews) {
  map<long, IntSeq> mods;
  for (MyUtil::IntSeq::const_iterator it = idSeq.begin(); it != idSeq.end(); ++it) {
    long m = *it % TABLE_COUNT;
    if (mods.find(m) == mods.end()) {
      mods[m] = IntSeq();
    }
    mods[m].push_back(*it);
  }
  MCE_INFO("GroupViewLoaderManagerI::LocateUser --> size:" << idSeq.size() << " mod size:" << mods.size());
  for (map<long, IntSeq>::const_iterator it = mods.begin(); it != mods.end(); ++it) {
    if (!LocateUserOneMod(it->first, it->second, type, gviews)) {
      return false;
    }
  }

  return true;

}

bool GroupViewLoaderManagerI::LocateUserOneMod(long mod, const MyUtil::IntSeq &idSeq, int type, TalkGroupViewSeqMap &gviews) {
  MCE_INFO("GroupViewLoaderManagerI::LocateUserOneMod --> mod:" << mod << " size:" << idSeq.size());
  ostringstream sql;

  sql << "select * from " 
      << DbOpe::GetDbTable(DB_PREF, TABLE_COUNT, mod) << " where user_id in (" ;
  
  for (MyUtil::IntSeq::const_iterator it = idSeq.begin(); it != idSeq.end(); ++it) {
    sql << *it;
    if (it != idSeq.end() - 1) {
      sql << ", ";
    }
  }

  sql << ") and session_type = " << type  << " order by update_time desc limit " <<
    (int)(type == TalkGroupViewClient::SOLID_GROUP ?LEN_SOLID_GROUP:(type == TalkGroupViewClient::DISCUSS_GROUP? LEN_DISCUSS_GROUP : LEN_CONVERSATION_GROUP));

  mysqlpp::StoreQueryResult res;
  bool rv = DbOpe::SqlRead(DB_INSTANCE, DbOpe::GetDbRoute(DB_ROUTE, TABLE_COUNT, mod).c_str(), sql.str().c_str(), res);	
  if (!rv) {
    MCE_WARN("GroupViewLoaderManagerI::LocateUser --> dosql error");
    return false;
  }
  // 空数据也要cache，避免重复db操作
  for (MyUtil::IntSeq::const_iterator it = idSeq.begin(); it != idSeq.end(); ++it) {
    if (gviews.find(*it) == gviews.end()) {
      gviews[*it] = TalkGroupViewSeq();
    }
  }
  
  MCE_INFO("GroupViewLoaderManagerI::LocateUserOneMod --> row.size:" << res.num_rows());
  for (size_t i = 0; i < res.num_rows(); ++i) {
    mysqlpp::Row row = res.at(i);
    int uid = (int)row["user_id"];
    if (gviews.find(uid) == gviews.end()) {
      gviews[uid] = TalkGroupViewSeq();
    }

    gviews[uid].push_back(GenInfo(
                                  (int)row["user_id"],
                                  (long)row["session_id"],
                                  (int)row["is_visible"],
                                  (int)row["has_read"],
                                  (int)row["session_type"],
                                  (string)row["update_time"],
                                  (int)row["affiliation"],
                                  (int)row["saveonline"],
                                  (string)row["notify"],
                                  (long)row["msgid"]
                                  ));

  }
  for (TalkGroupViewSeqMap::const_iterator it = gviews.begin(); it != gviews.end(); ++it) {
    TalkGroupViewClient::instance().SetRecentGroup(it->first, it->second, type);
  }

  return true;

}

TalkGroupViewPtr GroupViewLoaderManagerI::LocateOneItem(int uid, long sessionid) {
  MCE_INFO("GroupViewLoaderManagerI::LocateOneItem --> uid:" << uid << " sid:" << sessionid);
  ostringstream sql;

  sql << "select * from " 
      << DbOpe::GetDbTable(DB_PREF, TABLE_COUNT, uid) << " where user_id = "  << uid
      << " and session_id = " << sessionid;;

  mysqlpp::StoreQueryResult res;
  bool rv = DbOpe::SqlRead(DB_INSTANCE, DbOpe::GetDbRoute(DB_ROUTE, TABLE_COUNT, uid).c_str(), sql.str().c_str(), res);	
  if (!rv) {
    MCE_WARN("GroupViewLoaderManagerI::LocateOneItem --> dosql error");
    return 0;
  }

  
  MCE_INFO("GroupViewLoaderManagerI::LocateOneItem --> row.size:" << res.num_rows());
  for (size_t i = 0; i < res.num_rows(); ++i) {
    mysqlpp::Row row = res.at(i);

    return GenInfo(
                   (int)row["user_id"],
                   (long)row["session_id"],
                   (int)row["is_visible"],
                   (int)row["has_read"],
                   (int)row["session_type"],
                   (string)row["update_time"],
                   (int)row["affiliation"],
                   (int)row["saveonline"],
                   (string)row["notify"],
                   (long)row["msgid"]
                   );

  }
  return 0;

}
/*
TalkGroupViewSeq GroupViewLoaderManagerI::LoadById(int owner, int type, const Ice::Current &current) {
  return TalkGroupViewSeq();
}
*/

TalkGroupViewSeqMap GroupViewLoaderManagerI::LoadBySeq(const MyUtil::IntSeq &owners, int type, const Ice::Current &current ) {
	TimeStatN ts;
  TalkGroupViewSeqMap seqmap;
  bool rv = LocateUser(owners, type, seqmap);
	FunStatManager::instance().StatPlus(current, "GroupViewLoaderManagerI::LoadBySeq", ts.getTime(), (ts.getTime() > 200));
  if (rv) {
    return seqmap;
  } else {
    MCE_WARN("GroupViewLoaderManagerI::LocateBySeq --> load error");
    return TalkGroupViewSeqMap();
  }


}

bool GroupViewLoaderManagerI::UpdateItem(int userid, long sid, const Str2StrMap &props, const Ice::Current &current) {
	TimeStatN ts;
  bool rv = UpdateItem_(userid, sid, props);
	FunStatManager::instance().StatPlus(current, "GroupViewLoaderManagerI::UpdateItem", ts.getTime(), (ts.getTime() > 200));
  return rv;

}

void GroupViewLoaderManagerI::UpdateItemAsyn(int userid, long sid, const Str2StrMap &props, const Ice::Current &current) {
	TimeStatN ts;
  UpdateItem_(userid, sid, props);
	FunStatManager::instance().StatPlus(current, "GroupViewLoaderManagerI::UpdateItemAsyn", ts.getTime(), (ts.getTime() > 200));

}

bool GroupViewLoaderManagerI::Update(int actor, const TalkGroupViewPtr &groupview, const Ice::Current & current) {
	TimeStatN ts;
  bool rv = Update_(actor, groupview);
	FunStatManager::instance().StatPlus(current, "GroupViewLoaderManagerI::Update", ts.getTime(), (ts.getTime() > 200));
  return rv;
}

bool GroupViewLoaderManagerI::Delete(int uid, long sessionid, int type, const Ice::Current &current) {
	TimeStatN ts;
  bool rv = Delete_(uid, sessionid, type);
	FunStatManager::instance().StatPlus(current, "GroupViewLoaderManagerI::Delete", ts.getTime(), (ts.getTime() > 200));
  return rv;
}

bool GroupViewLoaderManagerI::UpdateMsgid(int uid, long sessionid, int type, long msgid, const Ice::Current &current) {
	TimeStatN ts;
  bool rv = UpdateMsgid_(uid, sessionid, type, msgid);
	FunStatManager::instance().StatPlus(current, "GroupViewLoaderManagerI::UpdateMsgid", ts.getTime(), (ts.getTime() > 200));
  return rv;

}

void GroupViewLoaderManagerI::UpdateAsyn(int actor, const TalkGroupViewPtr &groupview, const Ice::Current &current) {
	TimeStatN ts;
  Update_(actor, groupview);
	FunStatManager::instance().StatPlus(current, "GroupViewLoaderManagerI::UpdateAsyn", ts.getTime(), (ts.getTime() > 200));
}

void GroupViewLoaderManagerI::DeleteAsyn(int uid, long sessionid, int type, const Ice::Current &current) {
	TimeStatN ts;
  Delete_(uid, sessionid, type);
	FunStatManager::instance().StatPlus(current, "GroupViewLoaderManagerI::DeleteAsyn", ts.getTime(), (ts.getTime() > 200));
}

TalkGroupViewPtr GroupViewLoaderManagerI::MergeNewView(int userid, long sid, const Str2StrMap &props) {
  ostringstream sql;
  sql << "select * from " 
      << DbOpe::GetDbTable(DB_PREF, TABLE_COUNT, userid)
      << " where user_id = " << userid << " and session_id = " << sid;
  mysqlpp::StoreQueryResult res;
  bool rv = DbOpe::SqlRead(DB_INSTANCE, DbOpe::GetDbRoute(DB_ROUTE, TABLE_COUNT, userid).c_str(), sql.str().c_str(), res);	
  if (!rv) {
    MCE_WARN("GroupViewLoaderManagerI::MergeNewView --> dosql error");
    return 0;
  }
  

  TalkGroupViewPtr gv;
  for (size_t i = 0; i < res.num_rows(); ++i) {
    mysqlpp::Row row = res.at(i);
    int is_visible = (int)row["is_visible"];
    int has_read = (int)row["has_read"];
    int session_type = (int)row["session_type"];
    string update_time = (string)row["update_time"];
    int affiliation = (int)row["affiliation"];
    int saveonline = (int)row["saveonline"];
    string notify = (string)row["notify"];
    long msgid = (long)row["msgid"];

    Str2StrMap::const_iterator it = props.find("is_visible");
    if (it != props.end()) {
      is_visible = boost::lexical_cast<int>(it->second);
    }
    it = props.find("has_read");
    if (it != props.end()) {
      has_read = boost::lexical_cast<int>(it->second);
    }
    it = props.find("session_type");
    if (it != props.end()) {
      session_type = boost::lexical_cast<int>(it->second);
    }
    it = props.find("update_time");
    if (it != props.end()) {
      update_time = boost::lexical_cast<string>(it->second);
    }
    it = props.find("affiliation");
    if (it != props.end()) {
      affiliation = boost::lexical_cast<int>(it->second);
    }
    it = props.find("saveonline");
    if (it != props.end()) {
      saveonline = boost::lexical_cast<int>(it->second);
    }
    it = props.find("notify");
    if (it != props.end()) {
      notify = it->second;
    }
    it = props.find("msgid");
    if (it != props.end()) {
      msgid = boost::lexical_cast<long>(it->second);
    }

    gv = GenInfo(
                 userid, sid,
                 is_visible,
                 has_read,
                 session_type,
                 update_time,
                 affiliation,
                 saveonline,
                 notify,
                 msgid
                 );
    break;

  }
  return gv;
  
}

bool GroupViewLoaderManagerI::UpdateItem_(int userid, long sid, const Str2StrMap &props) {
	MCE_INFO("GroupViewLoaderManagerI::UpdateItem_-->uid " << userid << 
			" sid " << sid );
  TalkGroupViewPtr gv = MergeNewView(userid, sid, props);
  if (0 == gv) {
    return false;
  }

  return Update_(userid, gv);
  /*
  ostringstream sql;
  sql << "update " << DbOpe::GetDbTable(DB_PREF, TABLE_COUNT, userid) << " set ";

  size_t count = 0;
  for(MyUtil::Str2StrMap::const_iterator it = props.begin(); it != props.end(); ++it, ++count){
    sql << it->first << " = " << it->second;	
    if (count < props.size() - 1) {
      sql << ",";
    }
  }
  sql << " where user_id = " << userid << " and session_id = " << sid;
  bool rv = DbOpe::SqlWrite(DB_INSTANCE, DbOpe::GetDbRoute(DB_ROUTE, TABLE_COUNT, userid).c_str(), sql.str().c_str());	
  if (!rv) {
    MCE_WARN("GroupViewLoaderManagerI::UpdateItem_ --> dosql error");
    return false;
  }


  return true;
  */
}

bool GroupViewLoaderManagerI::Update_(int actor, const TalkGroupViewPtr &groupview) {
	if (0 == groupview) {
		MCE_WARN("GroupViewLoaderManagerI::Update-->groupinfo null actor " << actor);
		return false;
	}

	MCE_INFO("GroupViewLoaderManagerI::Update-->do actor id " << actor << 
			" sessionid " << groupview->sessionId );

  ostringstream sql;
  sql << "insert into " << DbOpe::GetDbTable(DB_PREF, TABLE_COUNT, groupview->userId) << " (user_id, session_id, is_visible, has_read, session_type, affiliation, saveonline, notify, msgid, update_time) values ("
      << groupview->userId << ", "
      << groupview->sessionId << ", "
      << groupview->isVisible << ", "
      << groupview->hasRead << ", "
      << groupview->sessionType << ", "
      << groupview->affiliation << ", "
      << groupview->saveonline << ", "
      << "\"" << groupview->notify << "\"" << ", "
      << groupview->msgid << ", "
      << "\""<<groupview->updateTime << "\")"
      << " on duplicate key update "
      << " user_id = " << groupview->userId
      << " , session_id = " << groupview->sessionId
      <<" , is_visible = " << groupview->isVisible
      << " , has_read = " << groupview->hasRead
      << " , session_type = " << groupview->sessionType
      << " , affiliation = " << groupview->affiliation
      << " , saveonline = " << groupview->saveonline
      << " , notify = " << "\"" << groupview->notify << "\""
      << " , msgid = " << groupview->msgid
      << " , update_time = \""  << groupview->updateTime << "\"";

  bool rv = DbOpe::SqlWrite(DB_INSTANCE, DbOpe::GetDbRoute(DB_ROUTE, TABLE_COUNT, groupview->userId).c_str(), sql.str().c_str());	
  if (!rv) {
    MCE_WARN("GroupViewLoaderManagerI::Update_ --> dosql error");
    return false;
  }


	TalkGroupViewSeq rseq;
  TalkGroupViewClient::instance().GetRecentGroupTypeByIdWithLoad(actor, groupview->sessionType, rseq);
	MCE_INFO("GroupViewLoaderManagerI::Update-->rseq.size " << rseq.size());

	TalkGroupViewSeq::iterator it = rseq.begin();
	for (; it != rseq.end(); ++it) {
		if ((*it)->userId == groupview->userId && (*it)->sessionId == groupview->sessionId) {
			break;
		}
	}

	int GroupLen[] = { GroupViewLoaderManagerI::LEN_SOLID_GROUP, GroupViewLoaderManagerI::LEN_DISCUSS_GROUP, GroupViewLoaderManagerI::LEN_CONVERSATION_GROUP};

	if (it != rseq.end()) {
    rseq.erase(it);
	} else if ((int)rseq.size() >= GroupLen[groupview->sessionType]) {
    rseq.pop_back();
	}
  rseq.insert(rseq.begin(), groupview);

	MCE_INFO("GroupViewLoaderManagerI::Update-->rseq.size " << rseq.size());
	TalkGroupViewClient::instance().SetRecentGroup(actor, rseq, groupview->sessionType);

  return true;

  
}

bool GroupViewLoaderManagerI::UpdateMsgid_(int uid, long sessionid, int type, long msgid) {

	MCE_INFO("GroupViewLoaderManagerI::UpdateMsgid_-->do actor id " << uid << 
			" sessionid " << sessionid );

  ostringstream sql;
  sql << "update " << DbOpe::GetDbTable(DB_PREF, TABLE_COUNT, uid) << " set msgid = " << msgid
      << " where user_id = " << uid << " and session_id = " << sessionid
      << " and session_type = " << type;

  bool rv = DbOpe::SqlWrite(DB_INSTANCE, DbOpe::GetDbRoute(DB_ROUTE, TABLE_COUNT, uid).c_str(), sql.str().c_str());	
  if (!rv) {
    MCE_WARN("GroupViewLoaderManagerI::UpdateMsgid_ --> dosql error");
    return false;
  }


	TalkGroupViewSeq rseq;
  TalkGroupViewClient::instance().GetRecentGroupTypeByIdWithLoad(uid, type, rseq);
	MCE_INFO("GroupViewLoaderManagerI::UpdateMsgid_-->rseq.size " << rseq.size());

	TalkGroupViewSeq::iterator it = rseq.begin();
	for (; it != rseq.end(); ++it) {
		if ((*it)->userId == uid && (*it)->sessionId == sessionid) {
			break;
		}
	}

	int GroupLen[] = { GroupViewLoaderManagerI::LEN_SOLID_GROUP, GroupViewLoaderManagerI::LEN_DISCUSS_GROUP, GroupViewLoaderManagerI::LEN_CONVERSATION_GROUP};
  TalkGroupViewPtr groupview;
	if (it != rseq.end()) {
    groupview = *it;
    groupview->msgid = msgid;
    rseq.erase(it);

	} else {
    groupview = LocateOneItem(uid, sessionid);
  }
  if (groupview != 0) {
    rseq.insert(rseq.begin(), groupview);    
  }
     
  if ((int)rseq.size() >= GroupLen[type]) {
    rseq.pop_back();
	}


	MCE_INFO("GroupViewLoaderManagerI::UpdateMsgid_-->rseq.size " << rseq.size());
	TalkGroupViewClient::instance().SetRecentGroup(uid, rseq, type);

  return true;

}

bool GroupViewLoaderManagerI::Delete_(int uid, long sessionid, int type) {

  ostringstream sql;
  sql << "delete from " << DbOpe::GetDbTable(DB_PREF, TABLE_COUNT, uid) << " where session_id = " << sessionid << " and user_id = "  << uid ;

  bool rv = DbOpe::SqlWrite(DB_INSTANCE, DbOpe::GetDbRoute(DB_ROUTE, TABLE_COUNT, uid).c_str(), sql.str().c_str());	
  if (!rv) {
    return false;
  }


	TalkGroupViewSeq rseq;
  TalkGroupViewClient::instance().GetRecentGroupTypeByIdWithLoad(uid, type, rseq);
	MCE_INFO("GroupViewLoaderManagerI::Delete_-->rseq.size " << rseq.size());

	TalkGroupViewSeq::iterator it = rseq.begin();
	for (; it != rseq.end(); ++it) {
		if ((*it)->userId == uid && (*it)->sessionId == sessionid) {
			break;
		}
	}

	int GroupLen[] = { GroupViewLoaderManagerI::LEN_SOLID_GROUP, GroupViewLoaderManagerI::LEN_DISCUSS_GROUP, GroupViewLoaderManagerI::LEN_CONVERSATION_GROUP};

	if (it != rseq.end()) {
    rseq.erase(it);
	}

	MCE_INFO("GroupViewLoaderManagerI::Delete_-->rseq.size " << rseq.size());
	TalkGroupViewClient::instance().SetRecentGroup(uid, rseq, type);


  return true;

}



