/*
 * =====================================================================================
 *
 *       Filename:  FeedItemWrapperI.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011年11月10日 12时10分56秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  guanghe.ge (ggh), guanghe.ge@opi-corp.com
 *        Company:  XiaoNei
 *
 * =====================================================================================
 */

#include "FeedItemWrapperI.h"
#include "ServiceI.h"
#include "IceLogger.h"
#include "util/cpp/TimeCost.h"
#include "util/cpp/InvokeMonitor.h"
#include "BuddyByOnlineTimeReplicaAdapter.h"
#include "FeedItemCacheByUseridReplicaAdapter.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

#include "BuddyGroupAdapter.h"
#include "FeedFocusAdapter.h"

using namespace xce::feed;
using namespace xce::serverstate;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace mop::hi::svc::adapter;
using namespace xce::buddy::adapter; 

void MyUtil::initialize() {
    ServiceI& service = ServiceI::instance();
    service.getAdapter()->add(&FeedItemWrapperI::instance(), service.createIdentity("M", ""));

    //zhi.lin register cache
    service.registerObjectCache(PAGEMINISITE_CATEGORY, "PageMinisite", new PageMinisiteObjectFactoryI, true);

    //controller
    int mod = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FeedItemWrapper.Mod", 0);
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FeedItemWrapper.Interval", 5);
    //string controller = "ControllerFeedItemWrapper";
    string controller = service.getCommunicator()->getProperties()->getPropertyWithDefault(
            "FeedItemWrapper.Controller", "ControllerFeedItemWrapper");
    //ServerStateSubscriber::instance().initialize(controller, &FeedItemWrapperI::instance(), mod, interval);
    xce::serverstate::ServerStateSubscriber::instance().initialize(controller, &FeedItemWrapperI::instance(), 
            mod, interval, new XceFeedControllerChannel());
}

Ice::ObjectPtr PageMinisiteObjectFactoryI::create(Ice::Int userid) 
{
    ostringstream pattern;
    mysqlpp::StoreQueryResult  res;
    Statement sql;
    pattern << "fans_pages_" << userid % 100;
    sql << "SELECT page_id FROM fans_pages_" << userid % 100<< " WHERE user_id = " << userid;
    try {
      res = QueryRunner("fans_pages", CDbRServer, pattern.str()).store(sql);
    } catch (Ice::Exception& e) {
      MCE_WARN("PagesObjectFactoryI::create --> error: " << e);
    } catch (std::exception& e) {
      MCE_WARN("PagesObjectFactoryI::create --> error: " << e.what());    
    }
    if (!res) {
      MCE_WARN("PagesObjectFactoryI::create --> StoryQueryWrong !! ");     
    }
    if (res.empty()) {
      MCE_INFO("PagesObjectFactoryI::create --> no funs_pages_id!! ");     
    }
    PageMinisiteObjectPtr obj=new PageMinisiteObject;
    
    for (size_t i = 0; i < res.size(); ++i) {
        obj->pages_.push_back(res.at(i)["page_id"]);
    }
    sql.clear();
    pattern.str("");    // clear
    pattern << "minisite_follower_site_" << (userid % 100);
    sql << "SELECT site_id FROM minisite_follower_site_" << (userid % 100) << " WHERE follower_id = " << userid;
    try {
        res = QueryRunner("minisite_follower_site", CDbRServer, pattern.str()).store(sql);
    } catch (Ice::Exception& e) {
      MCE_WARN("MinisiteObjectFactoryI::create --> error: " << e);
    } catch (std::exception& e) {
      MCE_WARN("MinisiteObjectFactoryI::create --> error: " << e.what());    
    }
    if (!res) {
      MCE_WARN("MinisiteObjectFactoryI::create --> StoryQueryWrong !! ");     
    }
    if (res.empty()) {
      MCE_INFO("MinisiteObjectFactoryI::create --> no minisite_id!! ");     
    }
    for (size_t i = 0; i < res.size(); ++i) {
        obj->minisites_.push_back(res.at(i)["site_id"]);
    }
    return obj;
}
void ReloadTask::put(Ice::Int uid) {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(mutex_);
    if (sids_.find(uid) != sids_.end()) {
        return;
    }
    sids_.insert(uid);
    if (sids_.size() >= batchSize_) {
        mutex_.notify();
        return;
    }
}
void ReloadTask::run() {
    while(true) {
        try{
            set<Ice::Int> tmp;
            {
                IceUtil::Monitor<IceUtil::Mutex>::Lock lock(mutex_);
                if (sids_.size() < batchSize_)
                    mutex_.timedWait(IceUtil::Time::seconds(batchTime_));
                sids_.swap(tmp);
            }
            if (tmp.empty()) continue;
            MCE_INFO("ReloadTask::run. reload size: " << tmp.size());
            for (set<Ice::Int>::iterator it=tmp.begin();it!=tmp.end(); ++it) {
                ServiceI::instance().reloadObject(PAGEMINISITE_CATEGORY,*it);
            }
        } catch (const Ice::Exception& e) {
            MCE_WARN("ReloadTask has exception! " << e.what());
        } catch (...) {
            MCE_WARN("ReloadTask has exception: UNKNOWN!");
        }
    }
}

FeedItemWrapperI::FeedItemWrapperI() {
    ReloadTask::instance().start().detach();
}
FeedItemWrapperI::~FeedItemWrapperI() {
}
FeedItemSeq FeedItemWrapperI::getItemSeq(::Ice::Int userid, ::Ice::Int begin, ::Ice::Int limit, const ::Ice::Current& current) {
  FeedItemSeq result;
  TimeStat ts;
  float step1;
  try {
    FeedMetaSeq metas = get(userid, begin, limit);
    step1 = ts.getTime();
    result.reserve(metas.size());

    for (FeedMetaSeq::iterator it = metas.begin();
        it != metas.end(); ++it) {
      FeedItem item;
      item.feed = it->feedid;
      item.merge = it->mergeid;
      item.type = it->type;
      item.time = it->time;
      item.weight = it->weight & 0x00ffffff;//根据经纬要求，高8位清零
      item.actor = it->userid;

      result.push_back(item);
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedItemWrapperI::get, userid: " << userid << " " << e);
  } catch (...) {
    MCE_WARN("FeedItemWrapperI::get, userid: " << userid << " UNKNOWN EXCEPTIOIN!");
  }
  MCE_INFO("FeedItemWrapperI::getItemSeq-->time statistics userid:"<<userid<<" get:"<<step1<<" Total:"<<ts.getTime());
  return result;
}

FeedMetaSeq FeedItemWrapperI::getLessThanFeedid(::Ice::Int userid, ::Ice::Long feedid, ::Ice::Int limit, 
        const ::Ice::Current& current) {
    return getLessThanFeedidByTypes(userid, feedid, limit, MyUtil::IntSeq(), current);
}
FeedMetaSeq FeedItemWrapperI::getLessThanFeedidByTypes(::Ice::Int userid, ::Ice::Long feedid, ::Ice::Int limit, 
        const MyUtil::IntSeq& types, const ::Ice::Current& current) {
    MCE_INFO("FeedItemWrapperI::getLessThanFeedidByTypes, userid: " << userid << ", feedid: " << feedid << ", limit: " << limit);

    FeedMetaSeq result;
    ostringstream oss;
    TimeStat ts;
    
    if (userid <=0 || feedid <= 0 || limit <=0) {
        return result;
    }
    try {
        oss << "FeedItemWrapperI::getLessThanFeedidByTypes-->time statistics userid:"<< userid<<" getFilterAndMaxReadFeedid:";

        StrSeq filters;
        Ice::Long maxReadFeedid = 0;
        getFilterAndMaxReadFeedid(userid, filters, maxReadFeedid);
        float step1 = ts.getTime();
        oss << step1<<" getFrientList:";
        
        if (maxReadFeedid >= feedid) {
            MCE_DEBUG("FeedItemWrapperI::getLessThanFeedidByTypes, maxReadFeedid:" << maxReadFeedid 
                   << " >= lessFeedid: " << feedid);
            return result;
        }
        IntSeq friends = xce::buddy::adapter::BuddyByOnlineTimeCacheAdapter::instance().getFriendList(userid, -1);
    /*
        mysqlpp::StoreQueryResult res;
        
        res = getPages(userid);
        for (size_t i = 0; i < res.size(); ++i) {
            int target = res.at(i)["page_id"];
            friends.push_back(target);
        }
        MCE_INFO(" FeedItemWrapperI::getLessThanFeedid --> friends size: " << friends.size() << "page size:" << res.size());     

        res = getMinisite(userid);
        for (size_t i = 0; i < res.size(); ++i) {
            int target = res.at(i)["site_id"];
            friends.push_back(target);
        }
        MCE_INFO(" FeedItemWrapperI::getLessThanFeedid --> friends size: " << friends.size() << "minisite size:" << res.size());     
    */
        float step2 = ts.getTime() - step1;
        oss<< step2<<" FeedItemCacheByUserid::getLessThanFeedid:";

        
        MyUtil::IntSeq types_filter = types;
        if (types_filter.empty()) {
            types_filter = getFixedType();
        }
        //result = FeedItemCacheByUseridReplicaAdapter::instance().getLessThanFeedid(friends, feedid, limit, types_fliter, filters);
        //返回值是按时间从大到小,时间相等按feedid从大到小排,所以得遍历取出比maxReadFeedid的FeedMeta
        FeedMetaSeq tmp = FeedItemCacheByUseridReplicaAdapter::instance().getLessThanFeedid(friends, feedid, limit, types_filter, filters);
        float step3 = ts.getTime() - step1 - step2;
        MCE_INFO("FeedItemWrapperI::getLessThanFeedidByTypes --> FeedItemCacheByUseridReplicaAdapter::getLessThanFeedid, userid: " << userid
                << " feedid:" << feedid << " limit:" << limit << " get:" << tmp.size());
        oss<<step3;
        //for (FeedMetaSeq::reverse_iterator it = result.rbegin(); 
        //        it != result.rend() && it->feedid <= maxReadFeedid; it++) {
        //    result.pop_back();
        //}
        for (FeedMetaSeq::iterator it = tmp.begin(); it != tmp.end(); it++){
            if (it->feedid > maxReadFeedid){
                result.push_back(*it);
            }
        }

  } catch (Ice::Exception& e) {
    MCE_WARN("FeedItemWrapperI::getLessThanFeedidByTypes, userid: " << userid << " " << e);
  } catch (...) {
    MCE_WARN("FeedItemWrapperI::getLessThanFeedidByTypes, userid: " << userid << " UNKNOWN EXCEPTIOIN!");
  }
    oss<<" Total:"<<ts.getTime();
    MCE_INFO(oss.str());
    return result;
}

FeedMetaSeq FeedItemWrapperI::get(::Ice::Int userid, ::Ice::Int begin, ::Ice::Int limit, const ::Ice::Current& current) {
    return getByTypes(userid, begin, limit, MyUtil::IntSeq(), current);
}
FeedMetaSeq FeedItemWrapperI::getByTypes(::Ice::Int userid, ::Ice::Int begin, ::Ice::Int limit, 
        const MyUtil::IntSeq& types, const ::Ice::Current& current) {
    ostringstream oss;
    oss << "userid: " << userid << " begin: " << begin << " limit: " << limit;
    InvokeClient id = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);
    FeedMetaSeq result;
    if (userid <=0 || limit <=0 || begin<0) {
        MCE_WARN("FeedItemWrapperI::get. error: userid<=0 || limit<=0 || begin<0. userid: " << userid
                << ", begin: " << begin << ", limit: " << limit);
        return result;
    }
    try {
        //ostringstream oss;
        //TimeStat ts;
        //oss << "FeedItemWrapperI::getByTypes-->time statistics userid:"<< userid<<" getFriendList:";
        
        IntSeq friends = xce::buddy::adapter::BuddyByOnlineTimeCacheAdapter::instance().getFriendList(userid, -1);
        //float step1 = ts.getTime();
        //oss <<step1<<" getPages:";

        //zhi.lin.begin 取好友关注的主页和小站
        std::vector<int> pages,minisites;
        getPages(userid,pages);getMinisites(userid,minisites);
        friends.reserve(friends.size()+pages.size()+minisites.size());//reverse
        copy(pages.begin(),pages.end(),back_inserter(friends));
        copy(minisites.begin(),minisites.end(),back_inserter(friends));
        //zhi.lin.end

        MCE_INFO(" FeedItemWrapperI::getByTypes --> friends size: " << friends.size() << " minisite size: " << minisites.size() << " page size: " << pages.size());     

        GetFeedMetasImpl(userid, friends, begin, limit, types, result);

        //oss<<" Total:"<<ts.getTime();
        //MCE_INFO(oss.str());
    } catch (Ice::Exception& e) {
      MCE_WARN("FeedItemWrapperI::getByTypes, userid: "<< userid << " begin: " << begin << " limit: " << limit << " err:" << e.what());
    } catch (...) {
      MCE_WARN("FeedItemWrapperI::getByTypes, userid: "<< userid << " begin: " << begin << " limit: " << limit << " throw Exception!");
    }
    return result;
}

FeedMetaSeq FeedItemWrapperI::getByTypesWithFriends(::Ice::Int userid, const MyUtil::IntSeq& friends, ::Ice::Int begin, ::Ice::Int limit,
    const MyUtil::IntSeq& types, const ::Ice::Current& current) {
  FeedMetaSeq result;
  try {
    GetFeedMetasImpl(userid, friends, begin, limit, types, result);

    MCE_INFO("FeedItemWrapperI::getByTypesWithFriends,  friend size: " 
        << friends.size() << " res size: "<< result.size() << " success!");
  } catch (...){
    MCE_WARN("FeedItemWrapperI::getByTypesWithFriends, userid: "<< userid << " begin: " 
        << begin << " limit: " << limit << " throw Exception!");
  }
  return result;
}

FeedMetaSeq FeedItemWrapperI::GetFeeds(::Ice::Int userid, ::Ice::Int begin, ::Ice::Int limit,
    const MyUtil::IntSeq& types, const ::Ice::Current& current) {
  FeedMetaSeq result;
  //ostringstream oss;
  //TimeStat ts;
  try {

    IntSeq friends = BuddyByOnlineTimeCacheAdapter::instance().getFriendList(userid, -1);

    GetFeedMetasImpl(userid, friends, begin, limit, types, result);

    MCE_INFO("FeedItemWrapperI::GetFeeds,  friend size: " 
        << friends.size() << " res size: "<< result.size() << " success!");
  } catch (...){
    MCE_WARN("FeedItemWrapperI::GetFeeds, userid: "<< userid << " begin: " 
        << begin << " limit: " << limit << " throw Exception!");
  }
  //oss<<" Total:"<<ts.getTime();
  //MCE_INFO(oss.str());
  return result;
}

FeedMetaSeq FeedItemWrapperI::GetFeedsByGroupId(::Ice::Int userid, ::Ice::Int group, 
    ::Ice::Int begin, ::Ice::Int limit, const IntSeq& types, const ::Ice::Current&) {
  FeedMetaSeq result;
  try {
    IntSeq friends = BuddyGroupAdapter::instance().getBuddyList(userid, group, 0, -1);

    GetFeedMetasImpl(userid, friends, begin, limit, types, result);

  } catch (...){
    MCE_WARN("FeedItemWrapperI::GetFeedByGroupId, userid: " << userid << " begin: " 
      << begin << " limit: " << limit << " throw Exception!");  
  }
  return result;
}

FeedMetaSeq FeedItemWrapperI::GetFeedsByGroupName(::Ice::Int userid, const string& name, 
    ::Ice::Int begin, ::Ice::Int limit, const IntSeq& types, const ::Ice::Current&) {
  FeedMetaSeq result;
  try {
    IntSeq friends = BuddyGroupAdapter::instance().getBuddyList(userid, name, 0, -1);

    GetFeedMetasImpl(userid, friends, begin, limit, types, result);

  } catch (...){
    MCE_WARN("FeedItemWrapperI::GetFeedByGroupName, userid: " << userid << " begin: " 
      << begin << " limit: " << limit << " throw Exception!");  
  }
  return result;
}

FeedMetaSeq FeedItemWrapperI::GetFeedsByFocus(int userid, int begin, int limit, 
    const IntSeq& types, const ::Ice::Current& current) {
  FeedMetaSeq result;
  try {
    IntSeq friends = FeedFocusAdapter::instance().GetFocuses(userid);

    GetFeedMetasImpl(userid, friends, begin, limit, types, result);

  } catch (...){
    MCE_WARN("FeedItemWrapperI::GetFeedsByFocus, userid: "<< userid << " begin: " 
      << begin << " limit: " << limit << " throw Exception!");  
  }
  return result;
}

LongSeq FeedItemWrapperI::GetFriendsFeedsList(int userid, const ::Ice::Current& current) {
  static const int FRIENDORIGTPYE[] = {601, 701, 709, 8200};
  ostringstream oss;
  oss << "userid: " << userid;
  InvokeClient id = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

  FeedMetaSeq result;
  IntSeq types;
  int type_size = sizeof(FRIENDORIGTPYE)/sizeof(FRIENDORIGTPYE[0]);
  for (int i = 0; i < type_size; i++) {
    types.push_back(FRIENDORIGTPYE[i]);
  }

  try {
    IntSeq friends = BuddyByOnlineTimeCacheAdapter::instance().getFriendList(userid, -1);
    GetFeedMetasImplSimple(userid, friends, 0, 1, types, result);
  } catch (...) {
    MCE_WARN("FeedItemWrapperI::GetUserFeedsFriendsList, userid: "<< userid << " throw Exception!");  
  }

  LongSeq feedids;
  feedids.reserve(result.size());
  for(FeedMetaSeq::iterator it = result.begin(); it != result.end(); ++it) {
    feedids.push_back(it->feedid);
  }

  return feedids;
}

void FeedItemWrapperI::GetFeedMetasImpl(int userid, const IntSeq & friends, int begin, int limit, 
    const IntSeq & types, FeedMetaSeq & result) {
  StrSeq filters;
  Ice::Long maxReadFeedid = 0;
  getFilterAndMaxReadFeedid(userid, filters, maxReadFeedid);

  MyUtil::IntSeq types_filter = types;
  if (types_filter.empty()) {
    types_filter = getFixedType();
  }

  if (maxReadFeedid > 0) {    //如果设置过全部标记为已读
    result = FeedItemCacheByUseridReplicaAdapter::instance().getBiggerThanFeedid(friends, maxReadFeedid, begin, limit, types_filter, filters);
    MCE_INFO("FeedItemWrapperI::GetFeedMetasImpl --> FeedItemCacheByUseridReplicaAdapter::getBiggerThanFeedid userid:" << userid
      << " max read feedid:" << maxReadFeedid << " limit:" << limit);
  }
  else {
    result = FeedItemCacheByUseridReplicaAdapter::instance().getByTypesWithFilter(friends, begin, limit, types_filter, filters);
    MCE_INFO("FeedItemWrapperI::GetFeedMetasImpl --> FeedItemCacheByUseridReplicaAdapter::getByTypesWithFilter, userid: " << userid
      << ", begin: " << begin << ", limit: " << limit);
  }
}

void FeedItemWrapperI::GetFeedMetasImplSimple(int userid, const IntSeq & friends, int begin, int limit, 
    const IntSeq & types, FeedMetaSeq & result) {
  StrSeq filters;
  Ice::Long maxReadFeedid = 0;
  getFilterAndMaxReadFeedid(userid, filters, maxReadFeedid);

  MyUtil::IntSeq types_filter = types;
  if (types_filter.empty()) {
    types_filter = getFixedType();
  }

  if (maxReadFeedid > 0) {    //如果设置过全部标记为已读
    result = FeedItemCacheByUseridReplicaAdapter::instance().getBiggerThanFeedidSimple(friends, maxReadFeedid, begin, limit, types_filter, filters);
    MCE_INFO("FeedItemWrapperI::GetFeedMetasImplSimple --> FeedItemCacheByUseridReplicaAdapter::getBiggerThanFeedidSimple userid:" << userid
      << " max read feedid:" << maxReadFeedid << " limit:" << limit);
  }
  else {
    result = FeedItemCacheByUseridReplicaAdapter::instance().getByTypesWithFilterSimple(friends, begin, limit, types_filter, filters);
    MCE_INFO("FeedItemWrapperI::GetFeedMetasImplSimple --> FeedItemCacheByUseridReplicaAdapter::getByTypesWithFilterSimple, userid: " << userid
      << ", begin: " << begin << ", limit: " << limit);
  }
}

void FeedItemWrapperI::getPages(int userid,vector<int> & v) {
  PageMinisiteObjectPtr ptr=ServiceI::instance().locateObject<PageMinisiteObjectPtr>(PAGEMINISITE_CATEGORY,userid);
  ptr->getPages(v);
  
  if( ptr->getCreateTime() <= time(NULL) - EXPIRED_TIME )
      ReloadTask::instance().put(userid);
}

void FeedItemWrapperI::getMinisites(int userid, vector<int> & v) {
  PageMinisiteObjectPtr ptr=ServiceI::instance().locateObject<PageMinisiteObjectPtr>(PAGEMINISITE_CATEGORY,userid);
  ptr->getMinisites(v);
  if( ptr->getCreateTime() <= time(NULL) - EXPIRED_TIME )
      ReloadTask::instance().put(userid);
}

MyUtil::StrSeq FeedItemWrapperI::createFilter(int userid, IntSeq ntypes) {

  StrSeq filters;

  //获取用户屏蔽的信息
  UserBlockInfoPtr block_infos = FeedItemBlockCacheReplicaAdapter::instance().getAll(userid);
  if (!block_infos) {
    block_infos = new UserBlockInfo;
    block_infos->maxReadFeedid = 0;
  }

  //IntSeq types = FeedItemBlockCacheReplicaAdapter::instance().getUserBlockTypes(userid);
  IntSeq types = block_infos->types;
  for (IntSeq::iterator it = types.begin();
      it != types.end(); ++it) {
    ostringstream oss;
    oss << TypePrefix << ":" << (long)(*it);
    filters.push_back(oss.str());
  }

  for (IntSeq::iterator it = ntypes.begin();
      it != ntypes.end(); ++it) {
    ostringstream oss;
    oss << TypePrefix << ":" << (long)(*it);
    filters.push_back(oss.str());
  }

  //IntSeq userids = FeedItemBlockCacheReplicaAdapter::instance().getUserBlockUsers(userid);
  IntSeq userids = block_infos->userids;
  for (IntSeq::iterator it = userids.begin();
      it != userids.end(); ++it) {
    ostringstream oss;
    oss << UseridPrefix << ":" << (long)(*it);
    filters.push_back(oss.str());
  }

  LongSeq feedids = block_infos->feedids;
  for (LongSeq::iterator it = feedids.begin();
      it != feedids.end(); ++it) {
    ostringstream oss;
    oss << FeedidPrefix << ":" << (long)(*it);
    filters.push_back(oss.str());
  }

  return filters; 
}
void FeedItemWrapperI::getFilterAndMaxReadFeedid(Ice::Int userid, StrSeq& filters, Ice::Long& maxReadFeedid) {
  filters.clear();
  //获取用户屏蔽的信息
  UserBlockInfoPtr block_infos;
  try {
    block_infos = FeedItemBlockCacheReplicaAdapter::instance().getAll(userid);
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedItemWrapperI::getFilterAndMaxReadFeedid --> "
        << "FeedItemBlockCacheReplicaAdapter::getAll, userid: " << userid << ", error:" << e.what());
  } catch (...) {
    MCE_WARN("FeedItemWrapperI::getFilterAndMaxReadFeedid, userid: " << userid << " UNKNOWN EXCEPTION!");
  }
  if (!block_infos) {
    block_infos = new UserBlockInfo;
    block_infos->maxReadFeedid = 0;
  }

  IntSeq types = block_infos->types;
  for (IntSeq::iterator it = types.begin();
      it != types.end(); ++it) {
    ostringstream oss;
    oss << TypePrefix << ":" << (long)(*it);
    filters.push_back(oss.str());
  }

  IntSeq userids = block_infos->userids;
  for (IntSeq::iterator it = userids.begin();
      it != userids.end(); ++it) {
    ostringstream oss;
    oss << UseridPrefix << ":" << (long)(*it);
    filters.push_back(oss.str());
  }

  LongSeq feedids = block_infos->feedids;
  for (LongSeq::iterator it = feedids.begin();
      it != feedids.end(); ++it) {
    ostringstream oss;
    oss << FeedidPrefix << ":" << (long)(*it);
    filters.push_back(oss.str());
  }

  maxReadFeedid = block_infos->maxReadFeedid;

  MCE_INFO("FeedItemWrapperI::getFilterAndMaxReadFeedid, userid: " << userid
      << " block types size: " << types.size()
      << ", block users size: " << userids.size() << ", block feeds size: " << feedids.size()
      << ", max read feedid: " << maxReadFeedid);
}


MyUtil::IntSeq FeedItemWrapperI::getFixedType() {//all kinds of types:page group....
  IntSeq result;
  int type_size = sizeof(TYPE)/sizeof(TYPE[0]);
  result.reserve(type_size);
  for (int i = 0; i < type_size; i++) {
    result.push_back(TYPE[i]);
  }
  return result;
}

