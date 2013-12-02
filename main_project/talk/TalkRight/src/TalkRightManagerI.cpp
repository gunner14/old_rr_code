#include "TalkRightManagerI.h"
#include "BuddyCoreAdapter.h"
#include "QueryRunner.h"
#include "TalkRightAdapter.h"
#include "TalkFunStateAdapter.h"
#include "MessageType.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "talk/SpamClient/src/OzeinFilterAdapter.h"
#include "talk/SpamClient/src/OzeinActivityAdapter.h"
#include "ValidCodeAdapter.h"
#include "XmppTools.h"
#include <IceUtil/RWRecMutex.h>

using namespace com::xiaonei::ozein::content::generation;
using namespace com::xiaonei::ozein::activity::generation;
using namespace com::xiaonei::icode::xoa;
using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk::common;
using namespace com::xiaonei::service::buddy;
using namespace com::xiaonei::xce;
using namespace MyUtil;
using namespace com::xiaonei::talk::adapter;
using namespace com::xiaonei::talk::fun::state;

static const int UB = 0;
static const int SMB = 1;
static const int US = 2;

void MyUtil::initialize() {

	ServiceI& service = ServiceI::instance();

	service.registerObjectCache(UB, "UB", new UserBlockSetFactoryI);
	service.registerObjectCache(SMB, "SMB", new SysMsgBlockSetFactoryI);
	service.registerObjectCache(US, "US", new UserStatusFactoryI);

	service.getAdapter()->add(&RightManagerI::instance(), service.createIdentity("M", ""));

	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
	xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerTalkRight", &RightManagerI::instance(), mod, interval, new XceFeedControllerChannel());
	MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);

  // 加载白名单
  RightManagerI::instance().ReloadWhiteList();
}

Ice::ObjectPtr UserBlockSetFactoryI::create(Ice::Int id) {
	try {
		MyUtil::IntSeq ids = BuddyCoreAdapter::instance().getBlockSeq(id,0,-1);
		UserBlockSetPtr ptr = new BlockSet<int>();
		for(size_t i = 0; i < ids.size(); ++i) {
			ptr->add(ids.at(i));
		}

		return ptr;
	} catch(...) {
		MCE_WARN("UserBlockSetFactoryI::create err -> id="<<id);
	}
	return 0;
}
ObjectResultPtr UserBlockSetFactoryI::create(const MyUtil::IntSeq& ids) {
	ObjectResultPtr result = new ObjectResult;
	for (size_t i = 0; i < ids.size(); ++i) {

		Ice::ObjectPtr obj = create(ids.at(i));
		if (obj) {
			result->data[ids.at(i)] = obj;
		}
	}
	return result;
}

Ice::ObjectPtr SysMsgBlockSetFactoryI::create(Ice::Int id) {

//	Statement sql;
	ConnectionHolder conn("im", CDbRServer," ");
	mysqlpp::Query query = conn.query();
	query
			<< "select userid,message_type from system_message_block where userid = "
			<< id;

//	DefResSetPtr drs;
//	ConnectionQuery conn("im", CDbRServer);

	try {
		SysMsgBlockSetPtr ptr = new BlockSet<string>();

#ifndef NEWARCH
		mysqlpp::Result res = query.store();
#else
		mysqlpp::StoreQueryResult res = query.store();
		if(!res){
			return 0;
		}
#endif
//		ResSetPtr rs = conn.doSelect(sql);
//		 = DefResSetPtr::dynamicCast(rs);

#ifndef NEWARCH
		for (size_t i= 0; i<res.size(); ++i) {
#else
		for (size_t i = 0; i<res.num_rows(); ++i) {
#endif

#ifndef NEWARCH
			string type = res[i]["MESSAGE_TYPE"].get_string();//ptr->link;
#else
			string type = res[i]["MESSAGE_TYPE"].c_str();//ptr->link;
#endif
			ptr->add(type);
		}

		return ptr;
	} catch(Ice::Exception& e) {
		MCE_WARN(e << " at " << __FILE__ << ":" << __LINE__);
	} catch (std::exception& e) {
		MCE_WARN(e.what() << " at " << __FILE__ << ":" << __LINE__);
	} catch (...) {
		MCE_WARN("Unkown exception at " << __FILE__ << ":" << __LINE__);
	}

	return 0;
}

ObjectResultPtr SysMsgBlockSetFactoryI::create(const MyUtil::IntSeq& ids) {
	ObjectResultPtr result = new ObjectResult;
	for (size_t i = 0; i < ids.size(); ++i) {

		Ice::ObjectPtr obj = create(ids.at(i));
		if (obj) {
			result->data[ids.at(i)] = obj;
		}
	}
	return result;
}

Ice::ObjectPtr UserStatusFactoryI::create(Ice::Int id) {
	try {
		UserStatusPtr ptr = new UserStatus();
		Statement sql;
                sql << "select * from user_state where id = " << id;
                mysqlpp::StoreQueryResult res = QueryRunner("user_state", CDbRServer).store(sql);
                if (!res || res.num_rows()<=0) {
                        MCE_WARN("load state error id=" << id);
                        return 0;
                }
                mysqlpp::Row row = res.at(0);
		int state = (int)row["state"];
		ptr->selected = state&1;

                sql.clear();
                sql << "select * from user_passport where id=" << id;
                res = QueryRunner("user_passport", CDbRServer).store(sql);
                if (!res || res.num_rows()<=0) {
                        MCE_WARN("load status error id="<<id);
                        return 0;
                }
                row = res.at(0);
                ptr->status = (int)row["status"];
		return ptr;
	} catch(Ice::Exception& e) {
		MCE_WARN(e << " at " << __FILE__ << ":" << __LINE__);
	} catch (std::exception& e) {
		MCE_WARN(e.what() << " at " << __FILE__ << ":" << __LINE__);
	} catch (...) {
		MCE_WARN("Unkown exception at " << __FILE__ << ":" << __LINE__);
	}
	return 0;
}

ObjectResultPtr UserStatusFactoryI::create(const MyUtil::IntSeq& ids) {
	ObjectResultPtr result = new ObjectResult;
	for (size_t i = 0; i < ids.size(); ++i) {

		Ice::ObjectPtr obj = create(ids.at(i));
		if (obj) {
			result->data[ids.at(i)] = obj;
		}
	}
	return result;
}
//bool RightManagerI::verify(Ice::Int from, Ice::Int to, Ice::Int type,
//		const Ice::Current&) {
//	if(type == SYSMSG){
//		Ice::ObjectPtr obj = ServiceI::instance().findObject(SMB,to);
//		if(obj){
//			SysMsgBlockSetPtr ptr = SysMsgBlockSetPtr::dynamicCast(obj);
//			return ptr->has();
//		}
//	}
//
//
//}
/* brief : 过滤消息
 * 参数1：消息列表
 * return: 返回过滤完的消息 
 */
MyUtil::IntSeq RightManagerI::batchVerify(const VerifySeq& seq,
		const Ice::Current&) {
	MyUtil::IntSeq res;

	for (size_t i = 0; i < seq.size(); ++i) {
    // 白名单用户
    if(InWhiteList(seq.at(i)->from)) {
		  res.push_back(seq.at(i)->index);
      continue;
    }

		if (seq.at(i)->type == SYSTEM_MESSAGE) {
			SysMsgBlockSetPtr ptr = ServiceI::instance().locateObject<SysMsgBlockSetPtr>(SMB, seq.at(i)->to);
			if (ptr) {
				if (ptr->has(seq.at(i)->sysMsgType)) {
					continue;
				}
			}
		} else {
			// check message
			if (seq.at(i)->type == MESSAGE || seq.at(i)->type == OFFLINE_MESSAGE) {

				Relationship r;
				r.from = seq.at(i)->from;
				r.to = seq.at(i)->to;

				BuddyDesc bd = BuddyCoreAdapter::instance().getRelation(r, 1000);
				MCE_DEBUG("relation:"<<bd<<"  Friend:"<<Friend);
				if (Block == bd || Blocked == bd || BiBlock == bd || Self == bd) {
					continue;
				}

			}
			// block
			UserBlockSetPtr ptr = ServiceI::instance().locateObject<UserBlockSetPtr>(UB, seq.at(i)->to);
			if (ptr) {
				if (ptr->has(seq.at(i)->from)) {
					MCE_INFO("from:"<<seq.at(i)->from <<" is blocked by to:"<<seq.at(i)->to);
					continue;
				}
			}

			// hidden
			if (seq.at(i)->type == PRESENCE_COMMON) {
				if (seq.at(i)->from > 0 && seq.at(i)->to > 0) {
					int cluster = TalkRightAdapter::instance().getCluster();
					if ((seq.at(i)->from % cluster) == (seq.at(i)->to % cluster)) {
						if (isHiddenUser(seq.at(i)->from)) {
							continue;
						}
					} else {
						bool flag = 0;
						try {
							flag = TalkRightAdapter::instance().isHiddenUser(seq.at(i)->from);
						} catch(...) {
							MCE_WARN("check hidden user err");
						}
						if (flag) {
							continue;
						}
					}
				}
			}

		}

		res.push_back(seq.at(i)->index);
	}
	return res;
}

bool RightManagerI::BuddyRelationVerify(BuddyDesc &bd, int from, int to) {
  Relationship r;
  r.from = from;
  r.to = to;
  // 关系判断
  bd = BuddyCoreAdapter::instance().getRelation(r, 1000);
  if (Block == bd || Blocked == bd || BiBlock == bd || Self == bd) {
    return false;
  } else {
    return true;
  }

}

bool RightManagerI::UserBlockVerify(int from, int to) {
  UserBlockSetPtr ptr = ServiceI::instance().locateObject<UserBlockSetPtr>(UB, to);
  if (ptr) {
    if (ptr->has(from)) {
      MCE_INFO("RightManagerI::UserBlockVerify-->from:"<<from <<" is blocked by to:"<<to);
      return false;
    }
  }

  return true;

}

bool RightManagerI::SpamFilterVerify(int from, const string &msg) {
  TimeStat ts_spam; 
  vector<string> v;
  bool rv = true;
  v.push_back(msg);
  try {
    CheckResultMore spamobj = OzeinFilterClient::instance().checkerAndFilter(from, -1, 108, v, ""); 
    if (spamobj.getFlag() == 30) {
      rv = false;
      MCE_INFO("RightManagerI::SpamFilterVerify-->spam from " << from << " msg " << msg);
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("RightManagerI::SpamFilterVerify-->"
             "call OzeinFilterClient.checkerAndFilter ERR  line:"<<__LINE__<<" err:"<<e);
  } catch (std::exception& e) {
    MCE_WARN("RightManagerI::SpamFilterVerify-->"
             "call OzeinFilterClient.checkerAndFilter ERR  line:"<<__LINE__<<" err:"<<e.what());
  }
  FunStatManager::instance().Stat("TalkRight.SpamFilterVerify", ts_spam.getTime(), (ts_spam.getTime() >= 250.0));	

  return rv;


}

bool RightManagerI::SpamActivityVerify(int from, int to, const string &msg) {
  TimeStat ts_spam;
  bool rv = true;
  try {
    if (OzeinActivityAdapter::instance().checkActivity(from, to, 108, msg) > 0) {
      rv = false;
      MCE_INFO("RightManagerI::SpamActivityVerify-->spam from " << from 
               << " to " << to << " msg " << msg);
    }

  } catch (Ice::Exception& e) {
    MCE_WARN("RightManagerI::SpamActivityVerify-->"
             "call OzeinActivityAdapter.checkActivity ERR  line:"<<__LINE__<<" err:"<<e);
  } catch (std::exception& e) {
    MCE_WARN("RightManagerI::SpamActivityVerify-->"
             "call OzeinActivityAdapter.checkActivity ERR  line:"<<__LINE__<<" err:"<<e.what());
  }
  FunStatManager::instance().Stat("TalkRight.SpamActivityVerify", ts_spam.getTime(), (ts_spam.getTime() >= 250.0));	

  return rv;
}

// 关于错误值含义，请参考TalkRight.ice MsgVerify接口前的注释
int RightManagerI::MsgVerify(const VerifyMsgPtr &vmsg, const Ice::Current&) {
  
  if (0 == vmsg)
    return 501;

  MCE_INFO("RightManagerI::MsgVerify-->from " << vmsg->from << " to " << vmsg->to
           << " type " << vmsg->type << " code " << vmsg->code << " endpoint " << vmsg->endpoint)

  // 白名单用户不进行验证
  if(InWhiteList(vmsg->from)) {
    MCE_INFO("RightManagerI::MsgVerify-->from is in white list: from " 
      << vmsg->from << " to " << vmsg->to);
    return 0;
  }

  // 首先验证是否带了验证码
  if (!vmsg->code.empty()) {
    // 掉安全的验证吗接口
    bool isvalid = true;
    bool iserror = true;
    try {
      isvalid = ValidCodeAdapter::instance().validCode(vmsg->code, "privmessage", boost::lexical_cast<string>(vmsg->from));
      iserror = false;

    } catch (TTransportException& e) {
      MCE_WARN("RightManagerI::MsgVerify-->call ValidCodeAdapter.validCode TTransportException " << e.what() << " from " << vmsg->from << " to " << vmsg->to);
    } catch (std::exception& e) {
      MCE_WARN("RightManagerI::MsgVerify-->call ValidCodeAdapter.validCode " << e.what() << " from " << vmsg->from << " to " << vmsg->to);
    }

    if (isvalid) {
      // 没有调用错误的时候才写cache
      if (!iserror) {
        _verifycache[RecentValidStranterCache::get_small_id(vmsg->from, vmsg->to)
                     % RECENT_CACHE_POOL_SIZE].update(vmsg->from, vmsg->to);
      }
    } else {
      return 102;
    }
  }
  
  // 好友关系判断
  // 单聊才走好友这套东西
  if (vmsg->type == 1) {
    BuddyDesc bd;
    if (!BuddyRelationVerify(bd, vmsg->from, vmsg->to)) {
      switch (bd) {
      case Block:
        return 2;
      case Blocked:
        return 1;
      case BiBlock:
        return 3;
      case Self:
        return 4;
      default:
        break;
      }
    }
    // block判断
    if (!UserBlockVerify(vmsg->from, vmsg->to))
      return 5;

    bool ispager = false;
    if (!vmsg->endpoint.empty() && vmsg->endpoint.at(0) == 'W') {
      ispager = true;
    }
    // 验证码逻辑，不是好友的走判断
    if (bd != Friend && ispager
        && !_verifycache[RecentValidStranterCache::get_small_id(vmsg->from, vmsg->to)
                         % RECENT_CACHE_POOL_SIZE].lookup(vmsg->from, vmsg->to))
      return 101;

  }

  // spam 消息过滤
  if (!SpamFilterVerify(vmsg->from, vmsg->msg))
    return 201;

  // spam 消息频率过滤，只过滤单聊的消息
  if (vmsg->type == 1 && !SpamActivityVerify(vmsg->from, vmsg->to, vmsg->msg))
    return 202;


  return 0;
}

bool RightManagerI::isHiddenUser(Ice::Int userId, const Ice::Current&) {
	return _hiddenUsers->has(userId);
}
void RightManagerI::addHiddenUser(Ice::Int userId, const Ice::Current&) {
	_hiddenUsers->add(userId);
}
void RightManagerI::removeHiddenUser(Ice::Int userId, const Ice::Current&) {
	_hiddenUsers->remove(userId);
}
void RightManagerI::reloadBlockUser(Ice::Int userId, const Ice::Current&) {
	MCE_DEBUG("RightManagerI::reloadBlockUser --> userid:"<<userId);
	ServiceI::instance().removeObject(UB, userId);
}

UserStatusPtr RightManagerI::getUserStatus(Ice::Int userId, const Ice::Current&) {
	return ServiceI::instance().locateObject<UserStatusPtr>(US, userId);
}

void RightManagerI::reloadUserStatus(Ice::Int userId, const Ice::Current&) {
	ServiceI::instance().removeObject(US, userId);
}
/*
 * brief : 修改用户屏蔽类型
 *
 */
void RightManagerI::modifySystemMessageBlock(Ice::Int userId,
		const MyUtil::StrSeq& typeSeq, const Ice::Current&) {

	Statement sql;

	sql << "delete from system_message_block where userid = "<<userId;

//	ConnectionQuery conn("im", CDbWServer);
//	conn.doDelete(sql);
	QueryRunner("im", CDbWServer).store(sql);

	if (!typeSeq.empty()) {

		sql.clear();
		sql << "insert into system_message_block (userid,message_type) values ";
		for (size_t i = 0; i < typeSeq.size(); ++i) {
			if (i) {
				sql<<",";
			}
			sql<<"("<<userId<<",'"<<typeSeq.at(i)<<"')";
		}
		//ConnectionQuery conn("im_common", CDbWServer);
		QueryRunner("im", CDbWServer).store(sql);
	}

	ServiceI::instance().removeObject(SMB, userId);

}
//void RightManagerI::removeSystemMessageBlock(Ice::Int userId,
//		const string& type, const Ice::Current&) {
//	Statement sql;
//	sql << "delete from system_message_block where userid = "<<userId << " and message_type='"<<type<<"'";
//
//	ConnectionQuery conn("im_common", CDbWServer);
//	conn.doInsert(sql);
//
//	ServiceI::instance().removeObject(SMB, userId);
//}
/*
 * brief :获取用户屏蔽类型
 */
MyUtil::StrSeq RightManagerI::getSystemMessageBlock(Ice::Int userId,
		const Ice::Current&) {

	SysMsgBlockSetPtr ptr = ServiceI::instance().locateObject<SysMsgBlockSetPtr>(SMB, userId);
	if (ptr) {
		return ptr->getSeq();
	}
	return MyUtil::StrSeq();
}

IntSeq RightManagerI::getRecentBufferSet(const Ice::Current&) {
  IntSeq rv;

  rv.push_back(_verifycache[0].cache_size());
  rv.push_back(_verifycache[0].time_out());

  return rv;

}

void RightManagerI::setRecentBufferSet(int cachesize, int timeout, const Ice::Current&) {
  MCE_INFO("RightManagerI::setRecentBufferSet-->cachesize " << cachesize << " timeout " << timeout);

  for (int i = 0; i < RECENT_CACHE_POOL_SIZE; ++i) {
    _verifycache[i].set_cache_size(cachesize);
    _verifycache[i].set_time_out(timeout);
  }
}

void RightManagerI::ReloadWhiteList(const Ice::Current&) {
  string line;
  int id;
  ifstream ifile(WHITE_LIST_FILE.c_str());
  set<int> tmp_set;

  if (!ifile) {
    MCE_WARN("RightManager::ReloadWhiteList --> open file error");
    return;
  }
  while(getline(ifile, line)) {
    if (line=="")
      continue;
    try {
      id = boost::lexical_cast<int>(line);
    } catch (boost::bad_lexical_cast& e) {
      MCE_WARN("RightManager::ReloadWhiteList --> lexical " << line << e.what());
      if(!ifile)
        ifile.close();
      return;
    }
    tmp_set.insert(id);
  }

  ostringstream os;
  for(set<int>::iterator i= tmp_set.begin(); i != tmp_set.end(); ++i)
    os << *i << "\n";
  MCE_INFO("RightManager::ReloadWhiteList --> white list: size is " << tmp_set.size()
     << "\n" << os.str().c_str());
  {
    IceUtil::RWRecMutex::WLock lock(white_list_mutex_);
    white_list_.swap(tmp_set);
  }

  if(!ifile)
    ifile.close();
}

bool RightManagerI::InWhiteList(int uid) {
  IceUtil::RWRecMutex::RLock lock(white_list_mutex_);
  return (white_list_.find(uid) != white_list_.end());
}

ProxyManagerPrx RightManagerI::getProxyServerOneway(const JidPtr& ptr) {
	IceUtil::Mutex::Lock lock(_mutex_prx);

	map<string,ProxyManagerPrx>::iterator it =
			_managerOneway.find(ptr->endpoint);
	if (it != _managerOneway.end()) {
		return it->second;
	}
	Ice::ObjectPrx prx = ServiceI::instance().getAdapter()->getCommunicator()->stringToProxy("M@"+ptr->endpoint);
  ProxyManagerPrx dPrx =
		ProxyManagerPrx::uncheckedCast(prx->ice_oneway());

	_managerOneway[ptr->endpoint] = dPrx;
  
	return dPrx;
}
/*
 * brief : 人人桌面用户踢下线
 *
 */

void RightManagerI::kickDesk(Ice::Int userId, const Ice::Current &current) {
	TimeStatN ts;
  JidSeq jseq;
  MCE_INFO("RightManagerI::kickDesk-->uid:" << userId);
  try{
    jseq = TalkFunStateAdapter::instance().getUserJids(userId);
  }
  catch(Ice::Exception& e){
    MCE_WARN("RightManagerI::kickDesk-->TalkFunStateAdapter::getUserJids-->" << e << " uid:" << userId);
  }

  for (JidSeq::const_iterator it = jseq.begin(); it != jseq.end(); ++it) {
    if (CheckEndpointType((*it)->endpoint) == "TalkProxy") {
      MCE_INFO("RightManagerI::kickDesk-->jid:" << jidToString(*it));
      // kick this user
      try {
        getProxyServerOneway(*it)->kick(*it);
      } catch(Ice::Exception& e) {
        MCE_WARN("RightManagerI::kickDesk -->TalkProxyAdapter::kick "<<e<<"  to endpoint:"<< jidToString(*it));
      }

    }
  }
  FunStatManager::instance().StatPlus(current, "RightManagerI::kickDesk", ts.getTime(), (ts.getTime() >= 200));


}

// ==================================================
void RecentValidStranterCache::update(int from, int to) {
  IceUtil::Mutex::Lock lock(mutex_);
  string users;
  RecentValidStranterCache::get_key(from, to, users);
  ListIndex &seqidx = cache_.get<tag_seq>();

  t_idpairPtr pu(new t_idpair(users));
  std::pair<ListIndex::iterator, bool> p = cache_.push_front(pu);
  if (!p.second) {
    //    cache_.replace(p.first, pu);
    (*p.first)->set_stamp(time(NULL));
    cache_.relocate(cache_.begin(), p.first);
    MCE_INFO("RecentValidStranterCache::update--> relocate size " << seqidx.size()
             << " from " << from << " to " << to);

  }

  if ((int)seqidx.size() > cache_size_) {
    MCE_INFO("RecentValidStranterCache::update-->bigsize size " << seqidx.size()
             << " from " << from << " to " << to << " stamp " << seqidx.back()->stamp());
    seqidx.pop_back();
  }

  MCE_DEBUG("RecentValidStranterCache::update-->show size " << seqidx.size()
            << " from " << from << " to " << to);


}

bool RecentValidStranterCache::lookup(int from, int to) {
  IceUtil::Mutex::Lock lock(mutex_);

  string users;
  RecentValidStranterCache::get_key(from, to, users);
  ListIndex &seqidx = cache_.get<tag_seq>();
  SetIndex &setidx = cache_.get<tag_users>();

  SetIndex::iterator it = setidx.find(users);
  if (setidx.end() == it) {
    return false;
  } else {
    if (time(NULL) - (*it)->stamp() > timeout_) {
      MCE_INFO("RecentValidStranterCache::lookup-->timeout size " << seqidx.size()
               << " stamp " << (*it)->stamp()
               << " from " << from << " to " << to);
      setidx.erase(it);
      return false;
    } else {
      (*it)->set_stamp(time(NULL));
      MCE_DEBUG("RecentValidStranterCache::lookup-->relook size " << seqidx.size()
                << " stamp " << (*it)->stamp()
                << " from " << from << " to " << to);
      cache_.relocate(seqidx.begin(), cache_.project<tag_seq>(it));
      return true;
    }

  }

}

