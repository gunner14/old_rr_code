#include "TalkRecentLoaderI.h"
#include "QueryRunner.h"
#include <sstream>
#include <time.h>
#include "util/cpp/String.h"
#include "XmppTools.h"
#include "FeedMemcProxy/client/talk_cache_client.h"

#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
using namespace MyUtil;
using namespace xce::talk;
using namespace xce::talk::recent;
using namespace com::xiaonei::xce;
using namespace com::xiaonei::talk::recent::loader;

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&RecentLoaderManagerI::instance(), service.createIdentity("M", ""));

	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
	xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerTalkRecentLoader", &RecentLoaderManagerI::instance(), mod, interval, new TalkChannel());
	MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);

}

TalkRecentUserPtr RecentLoaderManagerI::GenInfo(int id, int type, const string &subject, const string &info) {
  TalkRecentUserPtr item = new TalkRecentUser();
  item->id = id;
  item->type = type;

  item->subject = subject;
  string tinyurl;
  if (type == 1) {
    // 单聊的
    //item->info = info;
    TalkUserPtr uPtr = TalkCacheClient::instance().GetUserByIdWithLoad(id);
    item->subject = uPtr->name;
    tinyurl = buildHeadUrl(uPtr->tinyurl, true);
  }


  ostringstream jn;
  string tname = item->subject;
  // 处理json转义
  StrUtil::string_replace(tname, "\\", "\\\\");
  StrUtil::string_replace(tname, "/", "\\/");
  StrUtil::string_replace(tname, "\"", "\\\"");
  jn << "[{\"id\":" << item->id 
     << ",\"type\":" << type 
     << ",\"name\":\"" << tname 
     << "\",\"head\":\"" << tinyurl 
     << "\"}]";

  item->info = jn.str();

  return item;
}

TalkRecentUserPtr RecentLoaderManagerI::LocateItem(int owner, int id, int type) {
  MCE_INFO("RecentLoaderManagerI::LocateItem-->Call");
  try {
    ostringstream queryname; // 和session_view 散表方式一样的
    queryname << "session_view_" << (owner % 100);

    Statement sql;
    //select * from latest_contact_session_90 limit 1 ;
    sql << "select * from latest_contact_session_" << owner % 100 << " where owner_id = " << owner
        << " and session_id = " << id << " and type = " << type << " limit 1";
    
    mysqlpp::StoreQueryResult res = QueryRunner(DB_INSTANCE, CDbRServer, queryname.str()).store(sql);
    if (!res) {
      MCE_WARN("RecentLoaderManagerI::LocateItem --> load name error id="<< owner  << "  return NULL");
      return 0;
    }

    if (res.num_rows() > 0) {
      mysqlpp::Row row = res.at(0);
      return GenInfo((int)row["session_id"], (int)row["type"],
                            (string)row["subject"], (string)row["info"]);
      
    } else {
      MCE_WARN("RecentLoaderManagerI::LocateItem -->empty res owner " << owner << " id " << id << " type " << type);
    }


  } catch(Ice::Exception &e) {
    MCE_WARN(e << " at " << __FILE__ << ":" << __LINE__);
  } catch (std::exception &e) {
    MCE_WARN(e.what() << " at " << __FILE__ << ":" << __LINE__);
  } catch (...) {
    MCE_WARN("Unkown exception at " << __FILE__ << ":" << __LINE__);
  }

  return 0;

}

bool RecentLoaderManagerI::LocateUser(int owner, TalkRecentUserSeq &seq) {
  MCE_INFO("RecentLoaderManagerI::LocateUser-->Call");
  seq.clear();
  try {

    ostringstream queryname; // 和session_view 散表方式一样的
    queryname << "session_view_" << (owner % 100);

    Statement sql;
    //select * from latest_contact_session_90 limit 1 ;
    sql << "select * from latest_contact_session_" << owner % 100 << " where owner_id = " << owner 
        << " order by last_update_time desc limit " << (int)LEN_RECENT;
    
    mysqlpp::StoreQueryResult res = QueryRunner(DB_INSTANCE, CDbRServer, queryname.str()).store(sql);
    if (!res) {
      MCE_WARN("RecentLoaderManagerI::LocateUser --> load name error id="<< owner  << "  return false");
      return false;
    }

    for (size_t i = 0; i < res.num_rows(); ++i) {
      mysqlpp::Row row = res.at(i);

      seq.push_back(GenInfo((int)row["session_id"], (int)row["type"],
                            (string)row["subject"], (string)row["info"]));
      
    }
    MCE_INFO("RecentLoaderManagerI::LocateUser-->owner " << owner << " seq.size " << seq.size());
    TalkRecentClient::instance().SetRecentUser(owner, seq);
    return true;

  } catch(Ice::Exception &e) {
    MCE_WARN(e << " at " << __FILE__ << ":" << __LINE__);
  } catch (std::exception &e) {
    MCE_WARN(e.what() << " at " << __FILE__ << ":" << __LINE__);
  } catch (...) {
    MCE_WARN("Unkown exception at " << __FILE__ << ":" << __LINE__);
  }

  return false;

}
/* brief: 获取指定用户最近联系信息
 * owner: 目标用户
 * return: 最近联系信息
 */
TalkRecentUserSeq RecentLoaderManagerI::LoadById(int owner, const Ice::Current &current) {
  MCE_INFO("RecentLoaderManagerI::LoadById-->Call");
  TalkRecentUserSeq seq;
  if (LocateUser(owner, seq)) {
    return seq;
  } else {
    MCE_WARN("RecentLoaderManagerI::LoadById --> load error owner "<< owner);
    return TalkRecentUserSeq();
  }
}
/* brief: 获取指定用户序列最近联系信息
 * owners: 目标用户序列
 * return: 最近联系信息map
 */
TalkRecentUserSeqMap RecentLoaderManagerI::LoadBySeq(const MyUtil::IntSeq &owners, const Ice::Current &current ) {
  MCE_INFO("RecentLoaderManagerI::LoadBySeq-->Call owners.size " << owners.size());
  TalkRecentUserSeqMap seqmap;
  for (IntSeq::const_iterator it = owners.begin(); it != owners.end(); ++it) {
    seqmap[*it] = TalkRecentUserSeq();
    LocateUser(*it, seqmap[*it]);
  }
  return seqmap;
}
/* brief: 更新cache中最近联系信息
 * actor: 目标用户
 * groupinfo: 数据信息
 * return: void
 */
void RecentLoaderManagerI::Update(int actor, const TalkRecentUserPtr &groupinfo, const Ice::Current &) {
  MCE_INFO("RecentLoaderManagerI::Update-->Call actor " << actor);
  if (groupinfo != 0) {
    TaskManager::instance().execute(new UpdateTask(actor, groupinfo));
  } else {
    MCE_WARN("RecentLoaderManagerI::Update-->Call groupinfo null actor " << actor);
  }
}
/* brief: 删除目标用户最近联系cache中指定的用户
 * actor: 目标用户
 * id: 删除的用户id
 * type: 聊天类型，1为单聊 2为群聊（老私信的配置，最后群没有上线 type为2也就废弃了）
 * return: void
 */
void RecentLoaderManagerI::Delete(int actor, int id, int type, const Ice::Current &) {
  MCE_INFO("RecentLoaderManagerI::Delete-->Call actor " << actor << " id "<< id <<  " type " << type);
  TaskManager::instance().execute(new DeleteTask(actor, id, type));

}


// ==================================================
void UpdateTask::handle() {
  if (0 == _groupinfo) {
      MCE_WARN("UpdateTask::handle-->groupinfo null actor " << _actor);
      return;
  }

  MCE_INFO("UpdateTask::handle-->do id " << _groupinfo->id << 
           " type " << _groupinfo->type);
           //<< " subject " << _groupinfo->subject << " info " << _groupinfo->info);
  TalkRecentUserSeq rseq = TalkRecentClient::instance().GetRecentUserByIdWithLoad(_actor);
  MCE_INFO("UpdateTask::handle-->before update rseq.size " << rseq.size());
  TalkRecentUserSeq::iterator it = rseq.begin();
  for (; it != rseq.end(); ++it) {
    if ((*it)->id == _groupinfo->id && (*it)->type == _groupinfo->type) {
      break;
    }
  }



  if (it != rseq.end()) {
    // 如果是第一个就不换顺序了
    if (rseq.begin() != it) {
      TalkRecentUserPtr tmp = *it;
      rseq.erase(it);
      //rseq.insert(rseq.begin(), RecentLoaderManagerI::instance().GenInfo(tmp->id, tmp->type, tmp->subject, tmp->info));
      rseq.insert(rseq.begin(), tmp);
    }

  } else {
    // 没有找到，从db里面获取把
    TalkRecentUserPtr newit = RecentLoaderManagerI::instance().LocateItem(_actor, _groupinfo->id, _groupinfo->type);
    if (0 != newit) {
      if (rseq.size() >= RecentLoaderManagerI::LEN_RECENT) {
        rseq.pop_back();
      }
      rseq.insert(rseq.begin(), newit);
    } else {
      MCE_WARN("UpdateTask::handle-->db locate fault actor "
               << _actor << " id " << _groupinfo->id << " type " << _groupinfo->type);
    }
    
  }



  MCE_INFO("UpdateTask::handle-->after update rseq.size " << rseq.size());
  TalkRecentClient::instance().SetRecentUser(_actor, rseq);

}



void DeleteTask::handle() {
  MCE_INFO("DeleteTask::handle-->actor " << _actor << " id " << _id << " type " << _type);

  TalkRecentUserSeq rseq = TalkRecentClient::instance().GetRecentUserByIdWithLoad(_actor);
  MCE_INFO("DeleteTask::handle-->before delete rseq.size " << rseq.size());
  TalkRecentUserSeq::iterator it = rseq.begin();
  for (; it != rseq.end(); ++it) {
    if ((*it)->id == _id && (*it)->type == _type) {
      break;
    }
  }


  if (it != rseq.end()) {
    rseq.erase(it);
    TalkRecentClient::instance().SetRecentUser(_actor, rseq);    
  } else {
    MCE_WARN("DeleteTask::handle-->can't find actor " << _actor << " id " << _id << " type " << _type);
  }


  MCE_INFO("DeleteTask::handle-->after delete rseq.size " << rseq.size());


}

