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

FeedItemWrapperI::FeedItemWrapperI() {
}

FeedItemWrapperI::~FeedItemWrapperI() {
}

FeedItemSeq FeedItemWrapperI::getItemSeq(::Ice::Int userid, ::Ice::Int begin, ::Ice::Int limit, const ::Ice::Current& current) {
	FeedItemSeq result;
	try {
		FeedMetaSeq metas = get(userid, begin, limit);
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
	return result;
}
FeedMetaSeq FeedItemWrapperI::getLessThanFeedid(::Ice::Int userid, ::Ice::Long feedid, ::Ice::Int limit, const ::Ice::Current& current) {
    MCE_INFO("FeedItemWrapperI::getLessThanFeedid, userid: " << userid << ", feedid: " << feedid << ", limit: " << limit);

    FeedMetaSeq result;
    if (userid <=0 || feedid <= 0 || limit <=0) {
        return result;
    }
    try {
        ostringstream oss;
        oss << "FeedItemWrapperI::getLessThanFeedid, userid: " << userid;
        TimeCost tm = TimeCost::create(oss.str());

        StrSeq filters;
        Ice::Long maxReadFeedid = 0;
        getFilterAndMaxReadFeedid(userid, filters, maxReadFeedid);

        ostringstream oss1;
        oss1 << "step 1: get filters: " << filters.size();
        tm.step(oss1.str());
        
        if (maxReadFeedid >= feedid) {
            MCE_DEBUG("FeedItemWrapperI::getLessThanFeedid, maxReadFeedid:" << maxReadFeedid 
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
        ostringstream oss2;
        oss2 << "step 2: get friends size: " << friends.size();
        tm.step(oss2.str());

        IntSeq type_filters = getFixedType();

        //result = FeedItemCacheByUseridReplicaAdapter::instance().getLessThanFeedid(friends, feedid, limit, type_filters, filters);
        //返回值是按时间从大到小,时间相等按feedid从大到小排,所以得遍历取出比maxReadFeedid的FeedMeta
        FeedMetaSeq tmp = FeedItemCacheByUseridReplicaAdapter::instance().getLessThanFeedid(friends, feedid, limit, type_filters, filters);
        MCE_DEBUG("FeedItemWrapperI::getLessThanFeedid --> FeedItemCacheByUseridReplicaAdapter::getLessThanFeedid, userid: " << userid
                << ", feedid: " << feedid << ", limit: " << limit << ", get: " << tmp.size());
		
        //for (FeedMetaSeq::reverse_iterator it = result.rbegin(); 
        //        it != result.rend() && it->feedid <= maxReadFeedid; it++) {
        //    result.pop_back();
        //}
        for (FeedMetaSeq::iterator it = tmp.begin(); it != tmp.end(); it++){
            if (it->feedid > maxReadFeedid){
                result.push_back(*it);
            }
        }
	
        ostringstream oss3;
        oss3 << "step 3: get FeedItem size: " << result.size();
        tm.step(oss3.str());

	} catch (Ice::Exception& e) {
		MCE_WARN("FeedItemWrapperI::getLessThanFeedid, userid: " << userid << " " << e);
	} catch (...) {
		MCE_WARN("FeedItemWrapperI::getLessThanFeedid, userid: " << userid << " UNKNOWN EXCEPTIOIN!");
	}
    
    return result;
}

FeedMetaSeq FeedItemWrapperI::get(::Ice::Int userid, ::Ice::Int begin, ::Ice::Int limit, const ::Ice::Current& current) {
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
        ostringstream oss;
        oss << "FeedItemWrapperI::get, userid: " << userid;
        TimeCost tm = TimeCost::create(oss.str());
        
        IntSeq friends = xce::buddy::adapter::BuddyByOnlineTimeCacheAdapter::instance().getFriendList(userid, -1);

		
        mysqlpp::StoreQueryResult res;
        
        res = getPages(userid);
        for (size_t i = 0; i < res.size(); ++i) {
            int target = res.at(i)["page_id"];

			if (target > 0) {
	            friends.push_back(target);
			}
        }
        MCE_INFO(" FeedItemWrapperI::get --> friends size: " << friends.size() << "page size:" << res.size());     

        res = getMinisite(userid);
        for (size_t i = 0; i < res.size(); ++i) {
            int target = res.at(i)["site_id"];

			if (target > 0) {
	            friends.push_back(target);
			}
        }
        MCE_INFO(" FeedItemWrapperI::get --> friends size: " << friends.size() << "minisite size:" << res.size());     
		

        ostringstream oss1;
        oss1 << "step 1: get friends size: " << friends.size();
        tm.step(oss1.str());

        StrSeq filters;
        Ice::Long maxReadFeedid = 0;
        getFilterAndMaxReadFeedid(userid, filters, maxReadFeedid);



        IntSeq type_filters = getFixedType();
        ostringstream oss5;
        oss5 << "step 2: get max read feedid: " << maxReadFeedid;
        tm.step(oss5.str());

        if (maxReadFeedid > 0) {    //如果设置过全部标记为已读
            result = FeedItemCacheByUseridReplicaAdapter::instance().getBiggerThanFeedid(friends, maxReadFeedid, limit, type_filters, filters);
            MCE_DEBUG("FeedItemWrapperI::get --> FeedItemCacheByUseridReplicaAdapter::getBiggerThanFeedid, userid: " << userid
                    << ", max read feedid: " << maxReadFeedid << ", limit: " << limit);
        }
        else {
            result = FeedItemCacheByUseridReplicaAdapter::instance().getByTypesWithFilter(friends, begin, limit, type_filters, filters);
            MCE_DEBUG("FeedItemWrapperI::get --> FeedItemCacheByUseridReplicaAdapter::getByTypesWithFilter, userid: " << userid
                    << ", begin: " << begin << ", limit: " << limit);
        }

        ostringstream oss6;
        oss6 << "step 3: get FeedItem size: " << result.size();
        tm.step(oss6.str());
    } catch (Ice::Exception& e) {
        MCE_WARN("FeedItemWrapperI::get, userid: "<< userid << " begin: " << begin << " limit: " << limit << " err:" << e.what());
    } catch (...) {
        MCE_WARN("FeedItemWrapperI::get, userid: "<< userid << " begin: " << begin << " limit: " << limit << " throw Exception!");
    }
    return result;
}

FeedMetaSeq FeedItemWrapperI::GetFeeds(::Ice::Int userid, ::Ice::Int begin, ::Ice::Int limit,
         const MyUtil::IntSeq& types, const MyUtil::IntSeq& ntypes, const ::Ice::Current& current) {
    FeedMetaSeq result;
    try {
        IntSeq friends = BuddyByOnlineTimeCacheAdapter::instance().getFriendList(userid, -1);
 		friends.push_back(userid);		
        
        mysqlpp::StoreQueryResult res;
        res = getPages(userid);
        for (size_t i = 0; i < res.size(); ++i) {
            int target = res.at(i)["page_id"];
            friends.push_back(target);
        }
 
        res = getMinisite(userid);
        for (size_t i = 0; i < res.size(); ++i) {
            int target = res.at(i)["site_id"];
            friends.push_back(target);
        }

        MCE_INFO(" FeedItemWrapperI::getfeeds --> friends size: " << friends.size() 
                << "page size:" << res.size() << " userid: " << userid);     
        
        StrSeq filters = createFilter(userid, ntypes);
        if (types.empty()) { // if types is empty, put in all fixedtypes
            result = FeedItemCacheByUseridReplicaAdapter::instance().getWithFilter(friends, begin, limit, filters);
            MCE_INFO(" FeedItemWrapperI::GetFeedd: getfeeds only by filters");
        } else {
            result = FeedItemCacheByUseridReplicaAdapter::instance().getByTypesWithFilter(friends, begin, limit, types, filters);
        }
        
        MCE_INFO("FeedItemWrapperI::GetFeeds,  friend size:" 
                << friends.size() << " res size: "<< result.size() << " success!");
    } catch (...){
        MCE_WARN("FeedItemWrapperI::GetFeeds, userid: "<< userid << " begin: " 
                << begin << " limit: " << limit << " throw Exception!");
    }
    return result;
}

FeedMetaSeq FeedItemWrapperI::GetFeedsByGroupId(::Ice::Int userid, ::Ice::Int group, 
        ::Ice::Int begin, ::Ice::Int limit, const IntSeq& types, const IntSeq& ntypes, const ::Ice::Current&) {
//TODO
    MCE_WARN(" FeedItemWrapperI::GetFeedsByGroupId: did not support for the momet! also see GetFeedsByGroupName");
    return FeedMetaSeq();
}

FeedMetaSeq FeedItemWrapperI::GetFeedsByGroupName(::Ice::Int userid, const string& name, 
        ::Ice::Int begin, ::Ice::Int limit, const IntSeq& types, const IntSeq& ntypes, const ::Ice::Current&) {
    FeedMetaSeq result;
    try {
        IntSeq friends = BuddyGroupAdapter::instance().getBuddyList(userid, name, begin, -1);
        StrSeq filters = createFilter(userid, ntypes);
        if (types.empty()) { // if types is empty, put in all fixedtypes
            result = FeedItemCacheByUseridReplicaAdapter::instance().getWithFilter(friends, begin, limit, filters);
            MCE_INFO(" FeedItemWrapperI::GetFeedByGruopName: getfeeds only by filters");
        } else {
            result = FeedItemCacheByUseridReplicaAdapter::instance().getByTypesWithFilter(friends, begin, limit, types, filters);
        }

        MCE_INFO("FeedItemWrapperI::GetFeedByGroupName, friens size: "<< friends.size() 
                <<"  result size: " << result.size() << " success!");

    } catch (...){
        MCE_WARN("FeedItemWrapperI::GetFeedByGroupName, userid: "<< userid << " begin: " 
                << begin << " limit: " << limit << " throw Exception!");  
    }
    return result;
}

FeedMetaSeq FeedItemWrapperI::GetFeedsByFocus(int userid, int begin, int limit, const IntSeq& types, 
      const IntSeq& ntypes, const ::Ice::Current& current) {
    FeedMetaSeq result;
    try {
        IntSeq friends = FeedFocusAdapter::instance().GetFocuses(userid);

        StrSeq filters = createFilter(userid, ntypes);

        if (types.empty()) { // if types is empty, put in all fixedtypes
            result = FeedItemCacheByUseridReplicaAdapter::instance().getWithFilter(friends, begin, limit, filters);
            MCE_INFO(" FeedItemWrapperI::GetFeedByFocus: getfeeds only by filters");
        } else {
            result = FeedItemCacheByUseridReplicaAdapter::instance().getByTypesWithFilter(friends, begin, limit, types, filters);
        }

        result = FeedItemCacheByUseridReplicaAdapter::instance().getByTypesWithFilter(friends, begin, limit, types, filters);

        MCE_INFO("FeedItemWrapperI::GetFeedsByFocus, friend size:" 
                << friends.size() << "<< res size: "<< result.size() << " success!");

    } catch (...){
        MCE_WARN("FeedItemWrapperI::GetFeedsByFocus, userid: "<< userid << " begin: " 
                << begin << " limit: " << limit << " throw Exception!");  
    }
    return result;
}

mysqlpp::StoreQueryResult FeedItemWrapperI::getPages(int userid) {

    ostringstream pattern;
    mysqlpp::StoreQueryResult  res;
    Statement sql;

    pattern << "fans_pages_" << userid % 100;

    sql << "SELECT page_id FROM fans_pages_" << userid % 100
        << " WHERE user_id = " << userid;

    try {
        res = QueryRunner("fans_pages", CDbRServer, pattern.str()).store(sql);
    } catch (Ice::Exception& e) {
        MCE_WARN(" FeedItemWrapperI::getPages --> error: " << e);
    } catch (std::exception& e) {
        MCE_WARN(" FeedItemWrapperI::getPages --> error: " << e.what());    
    }

    if (!res) {
        MCE_WARN(" FeedItemWrapperI::getPages --> StoryQueryWrong  @1 !! ");     
    }
    if (res.empty()) {
        MCE_WARN(" FeedItemWrapperI::getPages --> no funs_pages_id!! ");     
    }

    return res;
}

mysqlpp::StoreQueryResult FeedItemWrapperI::getMinisite(int userid) {

    mysqlpp::StoreQueryResult  res;
    Statement sql;
    sql << "SELECT site_id FROM minisite_follower"
        << " WHERE follower_id = " << userid;

    try {
        res = QueryRunner("minisite", CDbRServer).store(sql);
    } catch (Ice::Exception& e) {
        MCE_WARN(" FeedItemWrapperI::getMinisite --> error: " << e);
    } catch (std::exception& e) {
        MCE_WARN(" FeedItemWrapperI::getMinisite --> error: " << e.what());    
    }

    if (!res) {
        MCE_WARN(" FeedItemWrapperI::getMinisite --> StoryQueryWrong  @2 !! ");     
    }
    if (res.empty()) {
        MCE_WARN(" FeedItemWrapperI::getMinisite --> no minisite_id!! ");     
    }

    return res;
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

