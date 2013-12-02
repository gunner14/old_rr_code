/*
 * =====================================================================================
 *
 *       Filename:  FeedItemFacadeI.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011年10月27日 18时47分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  guanghe.ge (ggh), guanghe.ge@opi-corp.com
 *        Company:  XiaoNei
 *
 * =====================================================================================
 */

#include "FeedItemFacadeI.h"
#include "FacadeHelper.hpp"
#include "ServiceI.h"
#include "IceLogger.h"
#include "util/cpp/TimeCost.h"
#include "util/cpp/InvokeMonitor.h"
#include "Monitor/client/service/ServiceMonitor.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "FeedItemCacheByUseridReplicaAdapter.h"
#include "FeedItemCacheByAppidReplicaAdapter.h"
#include "FeedSocialWikiHelper.h"
#include "FeedMemcProxy/client/feed_index_memc_client.h"

using namespace MyUtil;
using namespace xce::feed;
using namespace xce::serverstate;

void MyUtil::initialize() {
    ServiceI& service = ServiceI::instance();
    service.getAdapter()->add(&FeedItemFacadeI::instance(), service.createIdentity("M", ""));

		int puttaskmin = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FeedItemFacade.PutFeedTaskConfig.Min", 2);
		int puttaskmax = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FeedItemFacade.PutFeedTaskConfig.Max", 100);
		TaskManager::instance().config(PUT_FEED_TASK_LEVEL, MyUtil::ThreadPoolConfig(puttaskmin, puttaskmax));

    //controller
    int interval = 5;
    int mod = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FeedItemFacade.Mod", 0);
    string controller = service.getCommunicator()->getProperties()->getPropertyWithDefault("FeedItemFacade.Controller", "ControllerFeedItemFacade");
    ServerStateSubscriber::instance().initialize(controller, &FeedItemFacadeI::instance(), mod, interval, new XceFeedControllerChannel());

}

void PutFeedTask::handle() {

    ostringstream oss;
    oss << "PutFeedTask::handle uid: " << seed_->actor << " feedid: " << seed_->feed << " type: " << seed_->type << " time: " << seed_->time;
		TimeCost tm = TimeCost::create(oss.str(), TimeCost::LOG_LEVEL_INFO);

    std::map<string, string> context = seed_->extraProps;

    FeedMeta fm;
    fm.userid = seed_->actor;
    fm.feedid = seed_->feed;
    fm.type = seed_->type;
    fm.time = seed_->time;
    fm.weight = seed_->baseWeight;
    fm.mergeid = seed_->newsMerge;

    //dispatch ByUser
    try {
        FeedItemCacheByUseridReplicaAdapter::instance().put(fm);
    } catch (Ice::Exception& e) {
        MCE_WARN("FeedItemFacadeI::put FeedItemCacheByUserid, feedid: " << fm.feedid << e.what());
    } catch (...) {
        MCE_WARN("FeedItemFacadeI::put FeedItemCacheByUserid, feedid: " << fm.feedid << " UNKNOWN Exception!");
    }

    if (FacadeHelper::is_exist(context, AppidContextPrefix)) {
        //dispatch ByAppid
        MCE_DEBUG("dispatch to ByAppId");
        try {
            FeedItemCacheByAppidReplicaAdapter::instance().put(fm, context);
        } catch (Ice::Exception& e) {
            MCE_WARN("FeedItemFacadeI::put FeedItemCacheByAppid, feedid: " << fm.feedid << e.what());
        } catch (...) {
            MCE_WARN("FeedItemFacadeI::put FeedItemCacheByAppid, feedid: " << fm.feedid << " UNKNOWN Exception!");
        }
    }

    if (FacadeHelper::is_exist(context, WikiidContextPrefix)) {
        MCE_DEBUG("dispatch to SocialWiki");
        FeedSocialWikiHelper::instance().put(fm, context);
    }

    //write DB.
    FacadeHelper::write_db(fm, context);
}

FeedItemFacadeI::FeedItemFacadeI() {
}

FeedItemFacadeI::~FeedItemFacadeI() {
}

void FeedItemFacadeI::put(const FeedSeedPtr& seed, const ::Ice::Current& current) {
	ostringstream oss;
	oss << "FeedItemFacadeI::put uid: " << seed->actor << " feedid: " << seed->feed << " type: " << seed->type << " time: " << seed->time;
	MCE_INFO(oss.str());

	TaskManager::instance().execute(new PutFeedTask(seed));
}

void FeedItemFacadeI::deleteFeed(Ice::Int userid, Ice::Long feedid, const ::Ice::Current& current) {

    ostringstream oss;
    oss << "uid: " << userid << " feedid: " << feedid;
    InvokeClient ic = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

    //TODO into del cache.
		MyUtil::IntSeq wikiids;
		FeedItemCacheByUseridReplicaAdapter::instance().deleteFeed(userid, feedid);

		wikiids.clear();
		Feedid2Wikiids(feedid, wikiids);
		FeedSocialWikiHelper::instance().del(userid, feedid, wikiids);

		FacadeHelper::delete_db(feedid);
}

void FeedItemFacadeI::delWikiFeed(Ice::Int userid, Ice::Long source, Ice::Int stype,
        const ::Ice::Current& current) {
    ostringstream oss;
    oss << "uid: " << userid << " source: " << source;
    InvokeClient ic = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

		MyUtil::IntSeq wikiids;
		MyUtil::LongSeq feedids;
		SourceAndStype2Feedids(source, stype, feedids);
		for(MyUtil::LongSeq::const_iterator it = feedids.begin(); it != feedids.end(); ++it) {
			wikiids.clear();
			Feedid2Wikiids(*it, wikiids);
			FeedSocialWikiHelper::instance().del(userid, *it, wikiids);
		}
}


void FeedItemFacadeI::changeWikiFeedPrivacy(Ice::Int userid, Ice::Long source, Ice::Int stype, Ice::Int privacy, const ::Ice::Current& current) {
    ostringstream oss;
    oss << "uid: " << userid << " source: " << source;
    InvokeClient ic = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

		MyUtil::IntSeq wikiids;
		MyUtil::LongSeq feedids;
		SourceAndStype2Feedids(source, stype, feedids);
		for(MyUtil::LongSeq::const_iterator it = feedids.begin(); it != feedids.end(); ++it) {
			wikiids.clear();
			Feedid2Wikiids(*it, wikiids);
			FeedSocialWikiHelper::instance().changePrivacy(userid, *it, privacy, wikiids);
		}
}

void FeedItemFacadeI::SourceAndStype2Feedids(Ice::Long source, Ice::Int stype, MyUtil::LongSeq & feedids) {
	FIndexMemcClient::FIValue feedinfo;
	try {
		feedinfo = FIndexMemcClient::instance().GetFIndex(source, stype);
	} catch (std::exception& e) {
		MCE_WARN("FeedItemFacadeI::SourceAndStype2Feedid --> call Memc err." << e.what());
	} catch (...) {
		MCE_WARN("FeedItemFacadeI::SourceAndStype2Feedid --> call Memc err, unknown");
	}

	if (feedinfo.empty()) {
		MCE_WARN("FeedItemFacadeI::SourceAndStype2Feedid, source = " << source << " Cannot Find Source and stype!");
		feedids.clear();
		return ;
	}

	for(FIndexMemcClient::FIValue::const_iterator it = feedinfo.begin(); it != feedinfo.end(); ++it) {
		feedids.push_back(it->feedid);
	}
}

void FeedItemFacadeI::Feedid2Wikiids(Ice::Long feedid, MyUtil::IntSeq & wikiids) {
	ostringstream table;
	mysqlpp::StoreQueryResult res;
	table << DB_TABLE_PREFIX << feedid % DB_TABLE_COUNT;
	Statement sql;
	sql << "select " << DB_COLUMN_CONTEXT << " from " << table.str() << " where " << DB_COLUMN_FEEDID << " = " << feedid;
	try {
		res = QueryRunner(DB_INSTANCE_NAME, CDbWServer, table.str()).store(sql);
	} catch (mysqlpp::Exception& e) {
		MCE_WARN("FeedItemFacadeI::Feedid2Wikiids, feedid = " << feedid << e.what());
	} catch (...) {
		MCE_WARN("FeedItemFacadeI::Feedid2Wikiids, feedid = " << feedid << " UNKNOWN EXCEPTION!")
	}
	if (res.empty()) {
		MCE_WARN("FeedItemFacadeI::Feedid2Wikiids, feedid = " << feedid << " Cannt Find Feed!");
		wikiids.clear();
		return ;
	}

	string context = res.at(0)[DB_COLUMN_CONTEXT.c_str()].c_str();

	int begin = 0;
	int end = 0;
	string temp;
	begin = context.find(WikiidContextPrefix);
	begin = context.find('=', begin);
	if (begin != std::string::npos) {
		begin++;
	}
	end = context.find(';', begin);
	temp = context.substr(begin, end - begin);

	begin = 0;
	while(begin != std::string::npos && begin < temp.size()) {
		if (temp.at(begin) == ' ') {
			begin++;
			continue;
		}
		end = temp.find(' ', begin);
		int wikiid = atoi(temp.substr(begin, end - begin).c_str());
		if (wikiid <= 0) {
			MCE_WARN("FeedItemFacadeI::Feedid2Wikiids FOUND Error Context Data. feedid = " << feedid << " Context: " << context);
			wikiids.clear();
			return ;
		}
		wikiids.push_back(wikiid);
		begin = end;
	}
}

