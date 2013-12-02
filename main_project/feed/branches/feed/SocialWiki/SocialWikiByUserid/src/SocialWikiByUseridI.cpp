/*
 * =================================================================
 * 
 *       Author :   duo.zhang
 * Organization :   RenRen-INC
 *        Email :   duo.zhang@renren-inc.com
 *      Created :   2012-03-09  14:18:37
 *  Description :   
 *
 * =================================================================
 */
 
#include "ServiceI.h"
#include "SocialWikiByUseridI.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/InvokeMonitor.h"
#include "QueryRunner.h"

using namespace xce::feed;
using namespace MyUtil;
using namespace com::xiaonei::xce;

static const int WIKI_USER_CACHE_CATEGORY = 0;
void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&SocialWikiByUseridI::instance(), service.createIdentity("M", ""));
	service.getCommunicator()->getProperties()->setProperty("Service.ObjectCache.WUC.CacheSize", "500000");
	service.registerObjectCache(WIKI_USER_CACHE_CATEGORY, "WUC", new WikiUserFactoryI);
	TaskManager::instance().config(TASK_LEVEL_SOCIAL_WIKI, MyUtil::ThreadPoolConfig(2,20));
	TaskManager::instance().config(TASK_LEVEL_ADD_OBJECT, MyUtil::ThreadPoolConfig(5,50));

	int mod = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("SocialWikiByUserid.Mod", 0);
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("SocialWikiByUserid.Interval", 5);
	string controller = service.getCommunicator()->getProperties()->getPropertyWithDefault("SocialWikiByUserid.Controller", "ControllerSocialWikiByUserid");
	try {
		xce::serverstate::ServerStateSubscriber::instance().initialize(controller, &SocialWikiByUseridI::instance(), mod, interval, new XceFeedControllerChannel());
	} catch(Ice::Exception e) {
		MCE_DEBUG("initialize: "<<e.what());
	}
}

Ice::ObjectPtr WikiUserFactoryI::create(Ice::Int userid) {
	UserWikiFeedPtr object = new UserWikiFeed(userid);
	
	return object;
}

void AddUserWikiTask::handle() {
	UserWikiFeedPtr object = new UserWikiFeed(userid_);

	ServiceI::instance().addObject(object, WIKI_USER_CACHE_CATEGORY, userid_);

	TaskManager::instance().execute(new LoadWikiFeedTask(wikiid_, userid_, *object));
}

void LoadWikiFeedTask::handle() {
	if (NULL == pSeq_) {
		std::pair<UserWikiFeed::WikiToItemMap::iterator, bool> p;
		IceUtil::RWRecMutex::WLock lock(userwiki_.mapmutex_);
		UserWikiFeed::WikiToItemMap::iterator findit = userwiki_.feeditem_.find(wikiid_);
		if (findit != userwiki_.feeditem_.end()) {
			return ;
		} else {
			p = userwiki_.feeditem_.insert(std::make_pair(wikiid_, SocialWikiItemSeq()));
			pSeq_ = &(p.first->second);
		}
	}
	Statement sql;
	ostringstream tablename;
	tablename << "feed_social_wiki_" << wikiid_%WIKITABLECOUNT;
	sql << "SELECT * FROM " << tablename.str() << " WHERE wikiid=" << wikiid_ << " and userid=" << userid_;

	mysqlpp::StoreQueryResult res;

	try {
		res = QueryRunner("feed_social_wiki", CDbRServer, tablename.str()).store(sql);
	} catch (mysqlpp::Exception & e) {
		MCE_WARN("LoadWIkiFeedTask, wikiid: " << wikiid_ << ", userid: " << userid_ << ", Exception: " << e.what());
	} catch (...) {
		MCE_WARN("LoadWIkiFeedTask, wikiid: " << wikiid_ << ", userid: " << userid_ << ", Unknown Exception");
	}

	if (res.empty()) {
		return ;
	}
	SocialWikiItemSeq tempseq;
	tempseq.reserve(res.size());

	for(mysqlpp::StoreQueryResult::iterator it = res.begin(); it != res.end(); ++it) {
		tempseq.push_back(SocialWikiItem());
		SocialWikiItem & tempitem = tempseq.back();
		mysqlpp::Row row = *it;
		tempitem.userid = row["userid"];
		tempitem.wikiid = row["wikiid"];
		tempitem.feedid = row["feedid"];
		tempitem.time = (time_t)mysqlpp::DateTime(row["time"]);
		tempitem.type = row["type"];
		tempitem.mergeid = row["mergeid"];
		tempitem.privacy = row["privacy"];
	}

	IceUtil::RWRecMutex::WLock lock(userwiki_.mapmutex_);
	pSeq_->swap(tempseq);
	pSeq_->insert(pSeq_->end(), tempseq.begin(), tempseq.end());
}

void UserWikiFeed::add(const SocialWikiItem & item) {
	int wikiid = item.wikiid;
	SocialWikiItemSeq * seq = NULL;
	{
		IceUtil::RWRecMutex::RLock lock(mapmutex_);
		WikiToItemMap::iterator findit = feeditem_.find(wikiid);
		if (findit != feeditem_.end()) {
			seq = &(findit->second);
		}
	}

	std::pair<WikiToItemMap::iterator, bool> p;
	IceUtil::RWRecMutex::WLock lock(mapmutex_);
	if (seq != NULL) {
		seq->push_back(item);
		return ;
	}
	WikiToItemMap::iterator findit = feeditem_.find(wikiid);
	if (findit != feeditem_.end()) {
		findit->second.push_back(item);
	} else {
		p = feeditem_.insert(std::make_pair(wikiid, SocialWikiItemSeq()));
		p.first->second.push_back(item);
		TaskManager::instance().execute(new LoadWikiFeedTask(wikiid, userid_, *this, &(p.first->second)));
	}
}

void UserWikiFeed::get(int wikiid, const MyUtil::IntSeq types, SocialWikiItemSeq & res) {
	const SocialWikiItemSeq * seq = NULL;
	bool isUseType = types.empty() ? false : true;
	IceUtil::RWRecMutex::RLock lock(mapmutex_);
	WikiToItemMap::iterator findit = feeditem_.find(wikiid);
	if (feeditem_.end() != findit) {
		seq = &(findit->second);
	} else {
		TaskManager::instance().execute(new LoadWikiFeedTask(wikiid, userid_, *this));
		return ;
	}
	for(SocialWikiItemSeq::const_iterator it = seq->begin(); it != seq->end(); ++it) {
		if (!isUseType) {
			res.push_back(*it);
		} else {
			IntSeq::const_iterator findit = std::find(types.begin(), types.end(), it->type);
			if (findit != types.end()) {
				res.push_back(*it);
			}
		}
	}
}

void UserWikiFeed::remove(const MyUtil::IntSeq & wikiids, long feedid) {
	SocialWikiItemSeq * seq = NULL;
	for(IntSeq::const_iterator it = wikiids.begin(); it != wikiids.end(); ++it) {
		{
			IceUtil::RWRecMutex::RLock lock(mapmutex_);
			WikiToItemMap::iterator findit = feeditem_.find(*it);
			if (findit != feeditem_.end()) {
				seq = &(findit->second);
			}
		}
		if (NULL == seq) {
			continue ;
		}
		IceUtil::RWRecMutex::WLock lock(mapmutex_);
		for(SocialWikiItemSeq::iterator seqit = seq->begin(); seqit != seq->end(); ) {
			if (seqit->feedid == feedid){
				seqit = seq->erase(seqit);
			} else {
				++seqit;
			}
		}
	}
}

void SocialWikiByUseridI::put(const ::xce::feed::SocialWikiItem & feeditem, const ::Ice::Current& current) {
	ostringstream oss;
	oss << "id: " << feeditem.userid << " feedid: " << feeditem.feedid << " type: " << feeditem.type << " time: " << feeditem.time;
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

	UserWikiFeedPtr objptr = ServiceI::instance().locateObject<UserWikiFeedPtr>(WIKI_USER_CACHE_CATEGORY, feeditem.userid);

	if (objptr) {
		objptr->add(feeditem);
	} else {
		MCE_WARN("SocialWikiByUseridI::put, userid: " << feeditem.userid << ", cannot locate object");
	}
}

::MyUtil::LongSeq SocialWikiByUseridI::gets(const ::MyUtil::IntSeq & userids, ::Ice::Int wikiid, ::Ice::Int begin, ::Ice::Int limit, const ::Ice::Current& current) {
	SocialWikiItemSeq feedseq;
	ostringstream oss;
	oss << " wikiid: " << wikiid << " limit: " << limit;
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

	for(IntSeq::const_iterator it = userids.begin(); it != userids.end(); ++it) {
		UserWikiFeedPtr objptr = ServiceI::instance().findObject<UserWikiFeedPtr>(WIKI_USER_CACHE_CATEGORY, *it);
		if (objptr) {
			objptr->get(wikiid, IntSeq(), feedseq);
		} else {
			TaskManager::instance().execute(new AddUserWikiTask(*it, wikiid));
		}
	}

	return productionResult(feedseq, begin, limit);
} 

::MyUtil::LongSeq SocialWikiByUseridI::getsWithType(const ::MyUtil::IntSeq & userids, ::Ice::Int wikiid, const ::MyUtil::IntSeq & types, ::Ice::Int begin, ::Ice::Int limit, const ::Ice::Current& current) {
	SocialWikiItemSeq feedseq;

	ostringstream oss;
	oss << " wikiid: " << wikiid << " limit: " << limit;
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

	for(IntSeq::const_iterator it = userids.begin(); it != userids.end(); ++it) {
		UserWikiFeedPtr objptr = ServiceI::instance().findObject<UserWikiFeedPtr>(WIKI_USER_CACHE_CATEGORY, *it);
		if (objptr) {
			objptr->get(wikiid, types, feedseq);
		} else {
			TaskManager::instance().execute(new AddUserWikiTask(*it, wikiid));
		}
	}

	return productionResult(feedseq, begin, limit);
}

void SocialWikiByUseridI::deleteFeed(::Ice::Int userid, const ::MyUtil::IntSeq & wikiids, ::Ice::Long feedid, const ::Ice::Current& current) {
	ostringstream oss;
	oss << " userid: " << userid << " feedid: " << feedid;
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

	UserWikiFeedPtr objptr = ServiceI::instance().findObject<UserWikiFeedPtr>(WIKI_USER_CACHE_CATEGORY, userid);
	if (objptr) {
		objptr->remove(wikiids, feedid);
	} else {
		MCE_DEBUG("SocialWikiByUseridI::deleteFeed, userid: " << userid << ", cannot locate object");
	}
}

::MyUtil::LongSeq SocialWikiByUseridI::productionResult(SocialWikiItemSeq & seq, int begin, int limit) {
	int seqsize = seq.size();
	
	if (begin > seqsize || limit == 0) {
		return MyUtil::LongSeq();
	}
	if ((limit > 0 && begin + limit > seqsize) || limit < 0)
		limit = seqsize - begin;
	
	SocialWikiItemSeq::iterator it = seq.begin();
	std::nth_element(it, it + (begin + limit) - 1, seq.end(), SocialWikiItemCompare());
	if(limit > 1)
		std::nth_element(it, it + begin, it + (begin + limit) - 1, SocialWikiItemCompare());
	std::sort(it + begin, it + begin + limit, SocialWikiItemCompare());

	LongSeq res;
	res.reserve(limit);

	for(SocialWikiItemSeq::const_iterator longit = it + begin; longit != it + begin + limit; ++longit) {
		if (longit->feedid != res.back()) {
			res.push_back(longit->feedid);
		}
	}
	return res;
}

