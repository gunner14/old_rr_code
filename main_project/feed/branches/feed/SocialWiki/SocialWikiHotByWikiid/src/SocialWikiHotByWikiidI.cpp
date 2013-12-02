/* ------------------------------------------------------------------
 * FileName:      SocialWikiHotByWikiidI.cpp
 * Desc:          
 * Author:        jin.zhang
 * Email:         jin.zhang@renren-inc.com
 * Create:        2012-03-13 14:09:23
 * LastChange:    2012-03-16 10:52:49
 * History:       
 *------------------------------------------------------------------ */

#include "SocialWikiHotByWikiidI.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/InvokeMonitor.h"
#include "util/cpp/TimeCost.h"
#include "../../Filter/TypesFilter.h"

using namespace MyUtil;
using namespace xce::feed;
using namespace com::xiaonei::xce;

//-----------------------------------------------------------------------------
//initialize
void MyUtil::initialize() {
    ServiceI& service = ServiceI::instance();
    service.getAdapter()->add(&SocialWikiHotByWikiidI::instance(), 
            service.createIdentity("M", ""));

    service.registerObjectCache(CATEGORY_SW, CATEGORY_SW_NAME, new SocialWikiFactoryI(), true);

    int mod = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("SocialWikiHotByWikiid.Mod", 0);
    int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("SocialWikiHotByWikiid.Interval", 5); 
    string controller = service.getCommunicator()->getProperties()->getPropertyWithDefault(
            "SocialWikiHotByWikiid.Controller", "ControllerSocialWikiHotByWikiid");
     xce::serverstate::ServerStateSubscriber::instance().initialize(controller,
             &SocialWikiHotByWikiidI::instance(), mod, interval, new XceFeedControllerChannel());
}

//-----------------------------------------------------------------------------
//SocialWiki
void SocialWiki::put(const SocialWikiCacheItem& item) {
    IceUtil::RWRecMutex::WLock lock(*this);
    feeds_.push_front(item);
}

std::vector<SocialWikiCacheItem> SocialWiki::get() {
    IceUtil::RWRecMutex::RLock lock(*this);
    return std::vector<SocialWikiCacheItem>(feeds_.begin(), feeds_.end());
}

void SocialWiki::del(const Ice::Long feedid) {
    IceUtil::RWRecMutex::WLock lock(*this);
    for (CBItems::iterator it = feeds_.begin(); it != feeds_.end(); it++) {
        if (feedid == it->feedid){
            feeds_.erase(it);
            break;
        }
    }
}

size_t SocialWiki::size() const {
    IceUtil::RWRecMutex::RLock lock(*this);
    return feeds_.size();
}
Ice::Int SocialWiki::getBirthtime() const {
    IceUtil::RWRecMutex::RLock lock(*this);
    return birthtime_;
}

//-----------------------------------------------------------------------------
//SocialWikiFactoryI::create
Ice::ObjectPtr SocialWikiFactoryI::create(Ice::Int wikiid) {
    ostringstream oss;
    oss << "SocialWikiFactoryI::create. wikiid: " << wikiid;
    TimeCost tm = TimeCost::create(oss.str());

    stringstream table;
    table << TABLE_PRE << (wikiid % TABLE_COUNT);

    Statement sql;
    sql << "SELECT * FROM " << table.str()
        << " WHERE " << SW_COL_WIKIID << "= " << wikiid
        << " AND UNIX_TIMESTAMP(" << SW_COL_UPDATETIME << ")>(UNIX_TIMESTAMP()-" <<  DURATION_TIME << ")"
        << " ORDER BY " << SW_COL_POSITION << " LIMIT " << LOAD_LIMIT;

    mysqlpp::StoreQueryResult res;
    try {
        res = QueryRunner(INSTANCE_NAME, CDbRServer, table.str()).store(sql);
    } catch (mysqlpp::Exception& e) {
        MCE_WARN("SocialWikiFactoryI::create, wikiid: " << wikiid << e.what()); 
    } catch (...) {
        MCE_WARN("SocialWikiFactoryI::create, wikiid: " << wikiid << " UNKNOWN EXCEPTION!");
    }

    SocialWikiPtr swiki = new SocialWiki;
    for (mysqlpp::StoreQueryResult::reverse_iterator it = res.rbegin();
            it != res.rend(); ++it) {
        SocialWikiCacheItem item(
                (Ice::Long)(*it)[SW_COL_FEEDID.c_str()],
                (Ice::Int)(*it)[SW_COL_POSITION.c_str()],
                (Ice::Int)(*it)[SW_COL_UPDATETIME.c_str()],
                (Ice::Int)(*it)[SW_COL_TYPE.c_str()]);
        swiki->put(item);
    }
    MCE_DEBUG("SocialWikiFactoryI::create, wikiid: " << wikiid << " size: " << swiki->size());

    return swiki;
}

//-----------------------------------------------------------------------------
//ReloadTask
void ReloadTask::put(Ice::Int wikiid) {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(mutex_);
    if (sids_.find(wikiid) != sids_.end()) {
        return;
    }
    ids_.push_back(wikiid);
    if (ids_.size() >= batchSize_) {
        mutex_.notify();
        return;
    }
    sids_.insert(wikiid);
}
void ReloadTask::run() {
    while(true) {
        try{
            vector<Ice::Int> tmp;
            {
                IceUtil::Monitor<IceUtil::Mutex>::Lock lock(mutex_);
                if (ids_.size() < batchSize_)
                    mutex_.timedWait(IceUtil::Time::seconds(batchTime_));
                ids_.swap(tmp);
                sids_.clear();
            }

            if (tmp.empty()) continue;
            MCE_DEBUG("ReloadTask::run. reload size: " << tmp.size());
            for (size_t i = 0; i < tmp.size(); i++) {
                ServiceI::instance().reloadObject(CATEGORY_SW, tmp[i]);
            }
        } catch (const Ice::Exception& e) {
            MCE_WARN("ReloadTask has exception! " << e.what());
        } catch (...) {
            MCE_WARN("ReloadTask has exception: UNKNOWN!");
        }
    }
}
//-----------------------------------------------------------------------------
//SocialWikiHotByWikiidI
SocialWikiHotByWikiidI::SocialWikiHotByWikiidI() {
    ReloadTask::instance().start().detach();
}
SocialWikiHotByWikiidI::~SocialWikiHotByWikiidI() {}

LongSeq SocialWikiHotByWikiidI::get(Ice::Int wikiid, Ice::Int begin, Ice::Int limit, 
        const Ice::Current& cur) {
    ostringstream oss;
    oss << "wikiid: " << wikiid << " begin: " << begin << " limit: " << limit;
    InvokeClient id = InvokeClient::create(cur, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

    ostringstream oss_err;
    oss_err << "SocialWikiHotByWikiidI::get. wikiid: " << wikiid << " begin: " << 
            begin << " limit: " << limit << " get size: 0. error"; 

    if (begin < 0 || limit <= 0) {
        MCE_DEBUG(oss_err.str() << " --> begin<0 || limit<=0");
        return LongSeq();
    }
    
    SocialWikiPtr obj = ServiceI::instance().locateObject<SocialWikiPtr>(CATEGORY_SW, wikiid);
    if (obj) {
        std::vector<SocialWikiCacheItem> items = obj->get();
        if (begin >= (int)items.size()) {
            MCE_DEBUG(oss_err.str() << " --> begin>=size");
            return LongSeq();
        }

        Ice::Int limittmp = limit;
        if (limittmp > (int)(items.size()-begin))
            limittmp = items.size() - begin;

        LongSeq res;
        for (std::vector<SocialWikiCacheItem>::iterator it = items.begin() + begin;
                it != items.end() && limittmp; it++, limittmp--) {
            res.push_back(it->feedid);
        }
        MCE_DEBUG("SocialWikiHotByWikiidI::get. wikiid: " << wikiid << " begin: " << 
                begin << " limit: " << limit << " get size: " << res.size());

        if (obj->getBirthtime() <= (time(NULL) - EXPIRED_TIME)) {
            MCE_DEBUG("SocialWikiHotByWikiidI::get. expired wikiid: " << wikiid);
            ReloadTask::instance().put(wikiid);
        }
        return res;
    }
    else {
        MCE_DEBUG(oss_err.str() << " --> cannot locate object.");
    }
    return LongSeq();
}

LongSeq SocialWikiHotByWikiidI::getByTypes(Ice::Int wikiid, Ice::Int begin, Ice::Int limit, const MyUtil::IntSeq& types, 
        const Ice::Current& cur) {
    ostringstream oss;
    oss << "wikiid: " << wikiid << " begin: " << begin << " limit: " << limit;
    InvokeClient id = InvokeClient::create(cur, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

    ostringstream oss_err;
    oss_err << "SocialWikiHotByWikiidI::getByTypes. wikiid: " << wikiid << " begin: " << 
            begin << " limit: " << limit << " get size: 0. error"; 

    if (begin < 0 || limit <= 0) {
        MCE_DEBUG(oss_err.str() << " --> begin<0 || limit<=0"); return LongSeq();
    }
    
    SocialWikiPtr obj = ServiceI::instance().locateObject<SocialWikiPtr>(CATEGORY_SW, wikiid);
    if (obj) {
        std::vector<SocialWikiCacheItem> items = obj->get();
        if (begin >= (int)items.size()) {
            MCE_DEBUG(oss_err.str() << " --> begin>=size");
            return LongSeq();
        }

        Ice::Int limittmp = limit;
        if (limittmp > (int)(items.size()-begin))
            limittmp = items.size() - begin;
        LongSeq res;
        TypesFilter type_filter(types);
        for (std::vector<SocialWikiCacheItem>::iterator it = items.begin() + begin;
                (it != items.end()) && limittmp; it++) {
            if (type_filter(it->type)) {
                res.push_back(it->feedid);
                limittmp--;
            }
        }
        MCE_DEBUG("SocialWikiHotByWikiidI::getByTypes. wikiid: " << wikiid << " begin: " << 
                begin << " limit: " << limit << " get size: " << res.size());

        if (obj->getBirthtime() <= (time(NULL) - EXPIRED_TIME)) {
            MCE_DEBUG("SocialWikiHotByWikiidI::getByTypes. expired wikiid: " << wikiid);
            ReloadTask::instance().put(wikiid);
        }
        return res;
    }
    else {
        MCE_DEBUG(oss_err.str() << " --> cannot locate object.");
    }
    return LongSeq();
}

void SocialWikiHotByWikiidI::put(const SocialWikiItem& item, const Ice::Current& cur) {
    if (item.privacy != PrivacyPublic) {
        MCE_DEBUG("SocialWikiHotByWikiidI::put. cannot put into cache, wikiid: " << item.wikiid
                << " feedid: " << item.feedid << " userid: " << item.userid << " privacy: " << item.privacy);
        return;
    }
    SocialWikiPtr obj = ServiceI::instance().findObject<SocialWikiPtr>(CATEGORY_SW, item.wikiid);
    if (obj) {
        SocialWikiCacheItem cacheitem(item);
        obj->put(cacheitem);
        MCE_DEBUG("SocialWikiHotByWikiidI::put. wikiid: " << item.wikiid << " feedid: " << item.feedid
                << " userid: " << item.userid << " privacy: " << item.privacy);
    }
    else {
        MCE_WARN("SocialWikiHotByWikiidI::put. not find object --> reload wikiid: " << item.wikiid);
        ReloadTask::instance().put(item.wikiid);
    }
}

void SocialWikiHotByWikiidI::puts(const SocialWikiItemSeq& items, const Ice::Current& cur) {
    Ice::Int cnt = 0;
    for (SocialWikiItemSeq::const_iterator it = items.begin(); it != items.end(); it++) {
        if (it->privacy != PrivacyPublic) continue;
        SocialWikiPtr obj = ServiceI::instance().findObject<SocialWikiPtr>(CATEGORY_SW, it->wikiid);
        if (obj) {
            SocialWikiCacheItem cacheitem(*it);
            obj->put(cacheitem);
            cnt++;
        }
        else {
            ReloadTask::instance().put(it->wikiid);
        }
    }
    MCE_DEBUG("SocialWikiNewsBywikiidI::puts. put: " << cnt << " reload: " << (items.size() - cnt));
}

void SocialWikiHotByWikiidI::remove(Ice::Int wikiid, Ice::Long feedid, const Ice::Current& cur) {
    SocialWikiPtr obj = ServiceI::instance().findObject<SocialWikiPtr>(CATEGORY_SW, wikiid);
    if (obj) {
        obj->del(feedid);
        MCE_DEBUG("SocialWikiHotByWikiidI::remove. wikiid: " << wikiid << " feedid: " << feedid);
    }
    else {
        MCE_DEBUG("SocialWikiHotByWikiidI::remove. wikiid: " << wikiid << " feedid: " << feedid
                << " cannot locate object.");
    }
}

void SocialWikiHotByWikiidI::reload(Ice::Int wikiid, const Ice::Current& cur) {
    MCE_DEBUG("SocialWikiHotByWikiidI::reload. wikiid: " << wikiid);
    ServiceI::instance().reloadObject(CATEGORY_SW, wikiid);
}

void SocialWikiHotByWikiidI::delWiki(Ice::Int wikiid) {
    ServiceI::instance().removeObject(CATEGORY_SW, wikiid);
    MCE_DEBUG("SocialWikiHotByWikiidI::delWiki. wikiid: " << wikiid);
}

