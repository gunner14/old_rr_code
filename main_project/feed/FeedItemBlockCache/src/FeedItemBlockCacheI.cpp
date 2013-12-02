#include "FeedItemBlockCacheI.h"
#include "util/cpp/TimeCost.h"
#include "util/cpp/InvokeMonitor.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "ServiceI.h"
#include "DbWriter.h"

using namespace xce::feed;
using namespace MyUtil;
using namespace com::xiaonei::xce;

void MyUtil::initialize() {
    ServiceI& service = ServiceI::instance();
    service.getAdapter()->add(&FeedItemBlockCacheI::instance(), 
            service.createIdentity("M", ""));

    service.registerObjectCache(USER_BLOCK_TYPE, "UBT", new UserBlockTypeFactoryI, false);
    service.registerObjectCache(USER_BLOCK_USER, "UBU", new UserBlockUserFactoryI, false);

    service.getCommunicator()->getProperties()->setProperty("Service.ObjectCache.UBF.CacheSize", "1500000");
    service.registerObjectCache(USER_BLOCK_FEED, "UBF", new UserBlockFeedFactoryI, true);

    //controller
    int mod = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FeedItemBlockCache.Mod", 0);
	  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FeedItemBlockCache.Interval", 5);
    string controller = service.getCommunicator()->getProperties()->getPropertyWithDefault(
            "FeedItemBlockCache.Controller", "ControllerFeedItemBlockCache");
    TaskManager::instance().scheduleRepeated(&FeedItemBlockCacheI::HeatingCacheTimer::instance());
    xce::serverstate::ServerStateSubscriber::instance().initialize(controller, &FeedItemBlockCacheI::instance(), 
            mod, interval, new XceFeedControllerChannel());


}

//-----------------------------------------------------------------------------
// create
Ice::ObjectPtr UserBlockTypeFactoryI::create(Ice::Int userid)
{
    Statement sql;
    sql << "SELECT feed_type FROM feed_recv_config WHERE uid=" << userid;
    mysqlpp::StoreQueryResult res;
    try {
        res = QueryRunner("feed_db", CDbRServer, "feed_recv_config").store(sql);
    } catch (mysqlpp::Exception& e) {
        MCE_WARN("UserBlockTypeFactoryI::create, userid: " << userid << e.what()); 
    } catch (...) {
        MCE_WARN("UserBlockTypeFactoryI::create, userid: " << userid << " UNKNOWN EXCEPTION!");
    }

    UserBlockTypePtr ubtypes = new UserBlockType;
    for (mysqlpp::StoreQueryResult::iterator it = res.begin();
            it != res.end(); ++it) {
        int type = (int)(*it)["feed_type"];
        ubtypes->put(type);
    }
    return ubtypes;
}

Ice::ObjectPtr UserBlockUserFactoryI::create(Ice::Int userid) 
{
    Statement sql;
    sql << "SELECT target FROM relation_feed_low WHERE source= " << userid;

    mysqlpp::StoreQueryResult res;
    try {
        res = QueryRunner("feedwb", CDbRServer).store(sql);
    } catch (mysqlpp::Exception& e) {
        MCE_WARN("UserBlockUserFactoryI::create, userid: " << userid << e.what());
    } catch (...) {
        MCE_WARN("UserBlockUserFactoryI::create, userid: " << userid << " UNKNOWN EXCEPTION!");
    }
	
    UserBlockUserPtr userids = new UserBlockUser;
    for (mysqlpp::StoreQueryResult::iterator it = res.begin();
            it != res.end(); ++it) {
        int id = (int)(*it)["target"];
        userids->put(id);
    }
    return userids;
}

Ice::ObjectPtr UserBlockFeedFactoryI::create(Ice::Int userid)
{
    ostringstream oss;
    oss << "UserBlockFeedFactoryI::create. userid: " << userid;
    InvokeClient id = InvokeClient::create(Ice::Current(), oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

    TimeStat ts;
    Statement sql;
    sql << "SELECT " << UBF_COLUMN_BLOCKFEED 
        << " FROM " << TableUserBlockFeedPre << (userid % TableUserBlockFeedCount)
        << " WHERE " << UBF_COLUMN_USERID << "=" << userid 
        << " AND " << UBF_COLUMN_TIME << ">=" << (time(NULL) - DURATION_TIME)
        << " ORDER BY " << UBF_COLUMN_BLOCKFEED << " DESC LIMIT " << UBF_LOAD_LIMIT;
        //by time desc
        //<< " ORDER BY " << UBF_COLUMN_TIME << " DESC LIMIT " << UBF_LOAD_LIMIT;
    mysqlpp::StoreQueryResult res;
    stringstream pattern;
    pattern << TableUserBlockFeedPre << (userid % TableUserBlockFeedCount);
    
    try {
        res = QueryRunner(INSTANCE_NAME, CDbRServer, pattern.str()).store(sql);
    } catch (mysqlpp::Exception& e) {
        MCE_WARN("UserBlockFeedFactoryI::create, userid: " << userid << e.what());
    } catch (...) {
        MCE_WARN("UserBlockFeedFactoryI::create, userid: " << userid << " UNKNOWN EXCEPTION!");
    }
    MCE_INFO("UserBlockFeedFactoryI::create ["<<userid<<"] sql time cost:"<<ts.getTime());
    
    UserBlockFeedPtr feedids = new UserBlockFeed;
    if (!feedids)
        return 0;
    for (mysqlpp::StoreQueryResult::iterator it = res.begin();
            it != res.end(); ++it) {
        Ice::Long fid = (Ice::Long)(*it)[UBF_COLUMN_BLOCKFEED.c_str()];
        feedids->put(fid, false);
    }

    //load max read feedid
    if (feedids->getMaxReadFeed() || (int)feedids->size() < UBF_LOAD_LIMIT)
        return feedids;

    sql.clear();
    sql << "SELECT " << UBF_COLUMN_BLOCKFEED 
        << " FROM " << TableUserBlockFeedPre << (userid % TableUserBlockFeedCount)
        << " WHERE " << UBF_COLUMN_USERID << "=" << userid 
        << " AND " << UBF_COLUMN_BLOCKFEED << "<0 LIMIT 1";

    try {
        res = QueryRunner(INSTANCE_NAME, CDbRServer, pattern.str()).store(sql);
    } catch (mysqlpp::Exception& e) {
        MCE_WARN("UserBlockFeedFactoryI::create, userid: " << userid << ", load max read feedid from db." << e.what());
    } catch (...) {
        MCE_WARN("UserBlockFeedFactoryI::create, userid: " << userid << ", load max read feedid from db, UNKNOWN EXCEPTION!");
    }
    if (res.begin() != res.end()) {
        Ice::Long fid = (Ice::Long)(*res.begin())[UBF_COLUMN_BLOCKFEED.c_str()];
        if (fid < 0)
            feedids->put(fid, false);
    }
    return feedids;
}

//-----------------------------------------------------------------------------

FeedItemBlockCacheI::FeedItemBlockCacheI()
{
    ReloadTask::instance().start().detach();
}

FeedItemBlockCacheI::~FeedItemBlockCacheI() 
{
}

//-----------------------------------------------------------------------------
// UserBlockType
IntSeq FeedItemBlockCacheI::getUserBlockTypes(Ice::Int userid, 
        const Ice::Current & current)
{
    ostringstream oss;
    oss << "FeedItemBlockCacheI::getUserBlockTypes. userid: " << userid;
    InvokeClient id = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

    UserBlockTypePtr object = ServiceI::instance().locateObject<UserBlockTypePtr>(USER_BLOCK_TYPE, userid);
    if (object) {
        MCE_DEBUG("FeedItemBlockCache::getUserBlockTypes, userid: " << userid);
        return object->get();
    }
    else {
        MCE_DEBUG("FeedItemBlockCache::getUserBlockTypes, userid: " << userid << ", cannot locate object");
    }
    return IntSeq();
}

void FeedItemBlockCacheI::addUserBlockType(Ice::Int userid, Ice::Int type, 
        const Ice::Current& current)
{
    UserBlockTypePtr object = ServiceI::instance().locateObject<UserBlockTypePtr>(USER_BLOCK_TYPE, userid);
    if (object) {
        object->put(type);
        MCE_DEBUG("FeedItemBlockCache::addUserBlockType, userid: " << userid);
    }
    else {
        MCE_DEBUG("FeedItemBlockCache::addUserBlockType, userid: " << userid << ", cannot loacte object");
    }
}

void FeedItemBlockCacheI::removeUserBlockType(Ice::Int userid, Ice::Int type, 
        const Ice::Current & current)
{
    UserBlockTypePtr object = ServiceI::instance().locateObject<UserBlockTypePtr>(USER_BLOCK_TYPE, userid);
    if (object) {
        object->del(type);
        MCE_DEBUG("FeedItemBlockCache::removeUserBlockType, userid: " << userid << ", type: " << type);
    }
    else {
        MCE_DEBUG("FeedItemBlockCache::removeUserBlockType, userid: " << userid << ", cannot loacte object");
    }
}

bool FeedItemBlockCacheI::hasUserBlockType(Ice::Int userid, Ice::Int type, const Ice::Current & current)
{
    bool res = false;
    UserBlockTypePtr object = ServiceI::instance().findObject<UserBlockTypePtr>(USER_BLOCK_TYPE, userid);
    if (!object) {
        object = ServiceI::instance().locateObject<UserBlockTypePtr>(USER_BLOCK_TYPE, userid);
    }
    if (object) {
        res = object->has(type);
    }
    MCE_DEBUG("FeedItemBlockCache::hasUserBlockType, userid: " << userid << ", type: " << type << ", result: " << res);
    return res;
} 


//-----------------------------------------------------------------------------
// UserBlockUser
IntSeq FeedItemBlockCacheI::getUserBlockUsers(Ice::Int userid, const Ice::Current& current)
{
    ostringstream oss;
    oss << "FeedItemBlockCacheI::getUserBlockUsers. userid: " << userid;
    InvokeClient id = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

    UserBlockUserPtr object = ServiceI::instance().locateObject<UserBlockUserPtr>(USER_BLOCK_USER, userid);
    if (object) {
        MCE_DEBUG("FeedItemBlockCache::getUserBlockUsers, userid: " << userid);
        return object->get();
    }
    else {
        MCE_DEBUG("FeedItemBlockCache::getUserBlockUsers, userid: " << userid << ", cannot locate object");
    }
    return IntSeq();
}

void FeedItemBlockCacheI::addUserBlockUser(Ice::Int userid, Ice::Int shield, 
        const Ice::Current& current)
{
    UserBlockUserPtr object = ServiceI::instance().locateObject<UserBlockUserPtr>(USER_BLOCK_USER, userid);
    if (object) {
        object->put(shield);
        MCE_DEBUG("FeedItemBlockCache::addUserBlockUser, userid: " << userid << ", shieldid: " << shield);
    }
    else {
        MCE_DEBUG("FeedItemBlockCache::addUserBlockUser, userid: " << userid << ", cannot locate object");
    }
}

void FeedItemBlockCacheI::removeUserBlockUser(Ice::Int userid, Ice::Int shield, 
        const Ice::Current& current)
{
    UserBlockUserPtr object = ServiceI::instance().locateObject<UserBlockUserPtr>(USER_BLOCK_USER, userid);
    if (object) {
        object->del(shield);
        MCE_DEBUG("FeedItemBlockCache::removeUserBlockUser, userid: " << userid << ", shieldid: " << shield);
    }
    else {
        MCE_DEBUG("FeedItemBlockCache::removeUserBlockUser, userid: " << userid << ", cannot locate object");
    }
}

bool FeedItemBlockCacheI::hasUserBlockUser(Ice::Int userid, Ice::Int shield, 
        const Ice::Current& current)
{
    bool res = false;
    UserBlockUserPtr object = ServiceI::instance().findObject<UserBlockUserPtr>(USER_BLOCK_USER, userid);
    if (!object) {
        object = ServiceI::instance().locateObject<UserBlockUserPtr>(USER_BLOCK_USER, userid);
    }
    if (object) {
        res = object->has(shield);
    }
    MCE_DEBUG("FeedItemBlockCache::hasUserBlockUser, userid: " << userid << ", shieldid: " << shield << ", result: " << res);
    return res;
}

//-----------------------------------------------------------------------------
// UserBlockFeed
LongSeq FeedItemBlockCacheI::getUserBlockFeeds(Ice::Int userid, 
        const Ice::Current & current)
{
    TimeStat ts;
    ostringstream oss;
    oss << "FeedItemBlockCacheI::getUserBlockFeeds. userid: " << userid;
    InvokeClient id = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);
    try {
      UserBlockFeedPtr object = ServiceI::instance().locateObject<UserBlockFeedPtr>(USER_BLOCK_FEED, userid);
      return object->get();
    } catch(Ice::Exception & e) {
      MCE_WARN("FeedItemBlockCache::getUserBlockFeed: userid["<<userid<<"] object created failed");
    }
    MCE_INFO("FeedItemBlockCache::getUserBlockFeed: userid[" << userid<<"] get time cost: "<<ts.getTime()<<" ms");
    return LongSeq(); 
}
void FeedItemBlockCacheI::addUserBlockFeed(Ice::Int userid, Ice::Long feedid,  
    const Ice::Current& current)
{
  TimeStat ts;
  ostringstream oss;
  oss << "FeedItemBlockCacheI::addUserBlockFeed. userid: " << userid << " feedid: " << feedid;
  InvokeClient id = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);
  if (!feedid) {
    MCE_DEBUG("FeedItemBlockCache::addUserBlockFeed: userid " << userid << ", feedId " << feedid << ", don't process.");
    return;
  }
  UserBlockFeedPtr object = ServiceI::instance().findObject<UserBlockFeedPtr>(USER_BLOCK_FEED,userid);
  if (object) { 
    if (!object->has(feedid)) { 
      long evict_feedid = object->put(feedid, true);
      if (0 != evict_feedid) {
        MCE_DEBUG("FeedItemBlockCache::addUserBlockFeed: userid " << userid << ", feedId " << feedid << " evict_feedid: " << evict_feedid);
        FeedItemBlockCacheI::instance().setMaxReadFeed(userid, evict_feedid);
      }
    } else {
      MCE_DEBUG("FeedItemBlockCache::addUserBlockFeed: userid " << userid << ", feedId " << feedid
         << ", already existed.");
      return;
    }
  } else {
    //锁在函数体里加的
    HeatingCacheTimer::instance().put(userid);
  }
  // 使用批写入策略
  DbWriter::instance().addBlockFeedUseBatch(userid, feedid);
  MCE_INFO("FeedItemBlockCache::addUserBlockFeed: userid["<<userid<<"]add time cost: "<<ts.getTime()<<" ms");
}
void FeedItemBlockCacheI::removeUserBlockFeed(Ice::Int userid, Ice::Long feedid, 
    const Ice::Current & current)
{
  DbWriter::instance().removeBlockFeed(userid, feedid);

  //UserBlockFeedPtr object = ServiceI::instance().locateObject<UserBlockFeedPtr>(USER_BLOCK_FEED, userid);
  UserBlockFeedPtr object = ServiceI::instance().findObject<UserBlockFeedPtr>(USER_BLOCK_FEED, userid);
  if (object) {
    object->del(feedid);
    MCE_DEBUG("FeedItemBlockCache::removeUserBlockFeed: userid " << userid << ", feedId " << feedid);
  }
  else {
    MCE_DEBUG("FeedItemBlockCache::removeUserBlockFeed: userid " << userid << ", cannot locate object.");
  }
}

bool FeedItemBlockCacheI::hasUserBlockFeed(Ice::Int userid, Ice::Long feedid, 
    const Ice::Current & current)
{
  bool res = false;
  UserBlockFeedPtr object = ServiceI::instance().findObject<UserBlockFeedPtr>(USER_BLOCK_FEED, userid);
  if (!object) {
    object = ServiceI::instance().locateObject<UserBlockFeedPtr>(USER_BLOCK_FEED, userid);
  }
  if (object) {
    res = object->has(feedid);
  }
  MCE_DEBUG("FeedItemBlockCache::hasUserBlockFeed: userid " << userid << ", feeId " << feedid << ", result: " << res);
  return res;
} 


//-----------------------------------------------------------------------------
// max read feed
void FeedItemBlockCacheI::setMaxReadFeed(Ice::Int userid, Ice::Long feedid,  
    const Ice::Current & current) 
{
  if (feedid <= 0) {
    MCE_DEBUG("FeedItemBlockCacheI::setMaxReadFeed: userid " << userid << ", max read feedid: " << feedid
        << ", feedid<=0 error!");
    return;
  }

  UserBlockFeedPtr object = ServiceI::instance().locateObject<UserBlockFeedPtr>(USER_BLOCK_FEED, userid);
  if (object) {
    //if (object->getMaxReadFeed() != feedid) {
    if (object->getMaxReadFeed() < feedid) {
      MCE_DEBUG("FeedItemBlockCacheI::setMaxReadFeed: userid " << userid << ", max read feedid: " << feedid);
      object->updateMaxReadFeed(feedid);
      DbWriter::instance().updateMaxReadFeed(userid, feedid);
    }
    else {
      MCE_DEBUG("FeedItemBlockCacheI::setMaxReadFeed: userid " << userid << ", max read feedid: " << feedid
          << ", already existed.");
    }
  } else {
    MCE_DEBUG("FeedItemBlockCacheI::setMaxReadFeed: userid " << userid << ", cannot locate object.");
  }

}

Ice::Long FeedItemBlockCacheI::getMaxReadFeed(Ice::Int userid, 
     const Ice::Current & current)
{
  //UserBlockFeedPtr object = ServiceI::instance().locateObject<UserBlockFeedPtr>(USER_BLOCK_FEED, userid);
  UserBlockFeedPtr object = ServiceI::instance().findObject<UserBlockFeedPtr>(USER_BLOCK_FEED, userid);
  if (object) {
    Ice::Long feedid = object->getMaxReadFeed();
    MCE_DEBUG("FeedItemBlockCacheI::getMaxReadFeed: userid " << userid << ", maxReadFeed: " << feedid);
    return feedid;
  }
  MCE_DEBUG("FeedItemBlockCacheI::getMaxReadFeed: userid " << userid << ", cannot loacte object.");
  return 0;
}

//-----------------------------------------------------------------------------
// getAll
UserBlockInfoPtr FeedItemBlockCacheI::getAll(Ice::Int userid, const Ice::Current& current)
{
  ostringstream oss;
  oss << "FeedItemBlockCacheI::getAll. userid: " << userid;
  InvokeClient id = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

  UserBlockInfoPtr result = new UserBlockInfo;
  if (!result) {
    MCE_DEBUG("FeedItemBlockCacheI::getAll new error:");
    return 0;
  }

  result->types = getUserBlockTypes(userid);
  result->userids = getUserBlockUsers(userid);
  result->feedids = getUserBlockFeeds(userid); 
  result->maxReadFeedid = getMaxReadFeed(userid);
  return result;
}

//-----------------------------------------------------------------------------
// reload

void FeedItemBlockCacheI::reloadUserBlockTypes(Ice::Int userid,
    const Ice::Current & current)
{
  MCE_DEBUG("FeedItemBlockCacheI::reloadUserBlockTypes, userid: " << userid);
  ServiceI::instance().reloadObject(USER_BLOCK_TYPE, userid);
}

void FeedItemBlockCacheI::reloadUserBlockUsers(Ice::Int userid, 
    const Ice::Current & current)
{
  MCE_DEBUG("FeedItemBlockCacheI::reloadUserBlockUsers, userid: " << userid);
  ServiceI::instance().reloadObject(USER_BLOCK_USER, userid);
}

void FeedItemBlockCacheI::reloadUserBlockFeeds(Ice::Int userid, 
    const Ice::Current & current)
{
  MCE_DEBUG("FeedItemBlockCacheI::reloadUserBlockFeeds, userid: " << userid);
  ServiceI::instance().reloadObject(USER_BLOCK_FEED, userid);
}

void FeedItemBlockCacheI::reloadAll(Ice::Int userid,
    const Ice::Current & current)
{
  ostringstream oss;
  oss << "FeedItemBlockCacheI::getAll. userid: " << userid;
  InvokeClient id = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

  MCE_DEBUG("FeedItemBlockCacheI::reloadAll, userid: " << userid);
  ServiceI::instance().reloadObject(USER_BLOCK_TYPE, userid);
  ServiceI::instance().reloadObject(USER_BLOCK_USER, userid);
  ServiceI::instance().reloadObject(USER_BLOCK_FEED, userid);
}

void FeedItemBlockCacheI::HeatingCacheTimer::put(int uid) {
  IceUtil::Mutex::Lock lock(mutex_);
  if (users.size() >= maxLoadSize)
    return;
  users.insert(uid);
}
void FeedItemBlockCacheI::HeatingCacheTimer::handle() {
  TimeStat ts;
  set<int> usersTmp;
  {
    IceUtil::Mutex::Lock lock(mutex_);
    users.swap(usersTmp);
  }
  for (set<int>::iterator ir = usersTmp.begin() ; ir != usersTmp.end(); ++ir) {
    try {
      UserBlockFeedPtr object = ServiceI::instance().locateObject<UserBlockFeedPtr>(USER_BLOCK_FEED,*ir);
    }catch (Ice::Exception &e) {
      MCE_DEBUG("FeedItemBlockCache::HeatingCacheTimer::handle create UserBlockFeed object by \"new\"failed!");
    }
  }
  MCE_INFO("FeedItemBlockCache::HeatingCacheTimer::handle: users size: "<<usersTmp.size()
      <<" ,time cost: "<<ts.getTime()<<" ms");
}


