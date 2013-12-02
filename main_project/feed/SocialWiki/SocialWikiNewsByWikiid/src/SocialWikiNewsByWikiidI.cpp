/* ------------------------------------------------------------------
 * FileName:      SocialWikiNewsByWikiidI.cpp
 * Desc:          
 * Author:        jin.zhang
 * Email:         jin.zhang@renren-inc.com
 * Create:        2012-03-09 17:21:13
 * LastChange:    2012-03-12 10:46:06
 * History:       
 *------------------------------------------------------------------ */

#include "SocialWikiNewsByWikiidI.h"
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
    service.getAdapter()->add(&SocialWikiNewsByWikiidI::instance(), 
            service.createIdentity("M", ""));

    service.registerObjectCache(CATEGORY_SW, CATEGORY_SW_NAME, new SocialWikiFactoryI(), true);

    int mod = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("SocialWikiNewsByWikiid.Mod", 0);
    int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("SocialWikiNewsByWikiid.Interval", 5); 
    string controller = service.getCommunicator()->getProperties()->getPropertyWithDefault(
            "SocialWikiNewsByWikiid.Controller", "ControllerSocialWikiNewsByWikiid");
     xce::serverstate::ServerStateSubscriber::instance().initialize(controller,
             &SocialWikiNewsByWikiidI::instance(), mod, interval, new XceFeedControllerChannel());
}

//-----------------------------------------------------------------------------
//SocialWiki
void SocialWiki::put(const SocialWikiCacheItem& item) {
    IceUtil::Mutex::Lock lock(*this);
    feeds_.push_front(item);
}

std::vector<SocialWikiCacheItem> SocialWiki::get() {
    IceUtil::Mutex::Lock lock(*this);
    return std::vector<SocialWikiCacheItem>(feeds_.begin(), feeds_.end());
}

void SocialWiki::del(const Ice::Long feedid) {
    IceUtil::Mutex::Lock lock(*this);
    for (CBItems::iterator it = feeds_.begin(); it != feeds_.end(); it++) {
        if (feedid == it->feedid){
            feeds_.erase(it);
            break;
        }
    }
}

size_t SocialWiki::size() const {
    IceUtil::Mutex::Lock lock(*this);
    return feeds_.size();
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
        << " WHERE " << SW_COL_WIKIID << "=" << wikiid
        << " AND " << SW_COL_PRIVACY << "=" << PrivacyPublic
        << " AND " << SW_COL_TIME << ">" << (time(NULL) - DURATION_TIME)
        << " ORDER BY " << SW_COL_TIME << " DESC LIMIT " << LOAD_LIMIT;
        //<< " ORDER BY " << SW_COL_FEEDID << " DESC LIMIT " << LOAD_LIMIT;

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
                (Ice::Int)(*it)[SW_COL_TYPE.c_str()], 
                (Ice::Int)(*it)[SW_COL_TIME.c_str()]);
        swiki->put(item);
    }
    MCE_DEBUG("SocialWikiFactoryI::create, wikiid: " << wikiid << " size: " << swiki->size());

    return swiki;
}

//-----------------------------------------------------------------------------
//SocialWikiNewsByWikiidI
LongSeq SocialWikiNewsByWikiidI::get(Ice::Int wikiid, Ice::Int begin, Ice::Int limit, 
        const Ice::Current& cur) {
    ostringstream oss;
    oss << "wikiid: " << wikiid << " begin: " << begin << " limit: " << limit;
    InvokeClient id = InvokeClient::create(cur, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

    ostringstream oss_err;
    oss_err << "SocialWikiNewsByWikiidI::get. wikiid: " << wikiid << " begin: " << 
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
        MCE_DEBUG("SocialWikiNewsByWikiidI::get. wikiid: " << wikiid << " begin: " << 
                begin << " limit: " << limit << " get size: " << res.size());
        return res;
    }
    else {
        MCE_DEBUG(oss_err.str() << " --> cannot locate object.");
    }
    return LongSeq();
}

LongSeq SocialWikiNewsByWikiidI::getByTypes(Ice::Int wikiid, Ice::Int begin, Ice::Int limit, const MyUtil::IntSeq& types, 
        const Ice::Current& cur) {
    ostringstream oss;
    oss << "wikiid: " << wikiid << " begin: " << begin << " limit: " << limit;
    InvokeClient id = InvokeClient::create(cur, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

    ostringstream oss_err;
    oss_err << "SocialWikiNewsByWikiidI::getByTypes. wikiid: " << wikiid << " begin: " << 
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

        int begintmp = begin;
        LongSeq res;
        TypesFilter type_filter(types);
        for (std::vector<SocialWikiCacheItem>::iterator it = items.begin();
                (it != items.end()) && limittmp; it++) {
            if (type_filter(it->type)) {
              if (begintmp > 0) {
                begintmp--;
                continue;
              }
                res.push_back(it->feedid);
                limittmp--;
            }
        }
        MCE_DEBUG("SocialWikiNewsByWikiidI::getByTypes. wikiid: " << wikiid << " begin: " << 
                begin << " limit: " << limit << " get size: " << res.size());
        return res;
    }
    else {
        MCE_DEBUG(oss_err.str() << " --> cannot locate object.");
    }
    return LongSeq();
}

void SocialWikiNewsByWikiidI::put(const SocialWikiItem& item, const Ice::Current& cur) {
    ostringstream oss;
    oss << "wikiid: " << item.wikiid << " feedid: " << item.feedid << " privacy: " << item.privacy;
    InvokeClient id = InvokeClient::create(cur, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

    if (item.privacy != PrivacyPublic) {
        MCE_DEBUG("SocialWikiNewsByWikiidI::put. cannot put into cache, wikiid: " << item.wikiid
                << " feedid: " << item.feedid << " userid: " << item.userid << " privacy: " << item.privacy);
        return;
    }
    SocialWikiPtr obj = ServiceI::instance().locateObject<SocialWikiPtr>(CATEGORY_SW, item.wikiid);
    if (obj) {
        SocialWikiCacheItem cacheitem(item);
        obj->put(cacheitem);
        MCE_DEBUG("SocialWikiNewsByWikiidI::put. wikiid: " << item.wikiid << " feedid: " << item.feedid
                << " userid: " << item.userid << " privacy: " << item.privacy);
    }
    else {
        MCE_WARN("SocialWikiNewsByWikiidI::put. cannot locate object.");
    }
}

void SocialWikiNewsByWikiidI::puts(const SocialWikiItemSeq& items, const Ice::Current& cur) {
    ostringstream oss;
    oss << "size: " << items.size();
    InvokeClient id = InvokeClient::create(cur, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);
    
    Ice::Int cnt = 0;
    for (SocialWikiItemSeq::const_iterator it = items.begin(); it != items.end(); it++) {
        if (it->privacy != PrivacyPublic) continue;
        SocialWikiPtr obj = ServiceI::instance().locateObject<SocialWikiPtr>(CATEGORY_SW, it->wikiid);
        if (obj) {
            SocialWikiCacheItem cacheitem(*it);
            obj->put(cacheitem);
            cnt++;
        }
    }
    MCE_DEBUG("SocialWikiNewsBywikiidI::puts. put: " << cnt << " lost: " << (items.size() - cnt));
}

void SocialWikiNewsByWikiidI::remove(Ice::Int wikiid, Ice::Long feedid, const Ice::Current& cur) {
    SocialWikiPtr obj = ServiceI::instance().findObject<SocialWikiPtr>(CATEGORY_SW, wikiid);
    if (obj) {
        obj->del(feedid);
        MCE_DEBUG("SocialWikiNewsByWikiidI::remove. wikiid: " << wikiid << " feedid: " << feedid);
    }
    else {
        MCE_DEBUG("SocialWikiNewsByWikiidI::remove. wikiid: " << wikiid << " feedid: " << feedid
                << " cannot locate object.");
    }
}

void SocialWikiNewsByWikiidI::reload(Ice::Int wikiid, const Ice::Current& cur) {
    MCE_DEBUG("SocialWikiNewsByWikiidI::reload. wikiid: " << wikiid);
    ServiceI::instance().reloadObject(CATEGORY_SW, wikiid);
}

void SocialWikiNewsByWikiidI::delWiki(Ice::Int wikiid) {
    MCE_DEBUG("SocialWikiNewsByWikiidI::delWiki. wikiid: " << wikiid);
    ServiceI::instance().removeObject(CATEGORY_SW, wikiid);
}

