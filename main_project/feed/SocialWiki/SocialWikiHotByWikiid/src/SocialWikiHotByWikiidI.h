/* ------------------------------------------------------------------
 * FileName:      SocialWikiHotByWikiidI.h
 * Desc:          
 * Author:        jin.zhang
 * Email:         jin.zhang@renren-inc.com
 * Create:        2012-03-13 11:50:52
 * LastChange:    2012-03-16 10:52:36
 * History:       
 *------------------------------------------------------------------ */
#ifndef __SOCIALWIKI_HOTBYWIKIIDI_H__
#define __SOCIALWIKI_HOTBYWIKIIDI_H__

#include "FeedSocialWiki.h"
#include "ServiceI.h"
#include "QueryRunner.h"
#include <boost/circular_buffer.hpp>
#include <boost/foreach.hpp>
#include <string>

namespace xce { namespace feed {

using namespace MyUtil;
using namespace std;

//-----------------------------------------------------------------------------
//常量定义
const Ice::Int  CATEGORY_SW         = 0;
const string    CATEGORY_SW_NAME    = "SW";

const string    INSTANCE_NAME       = "social_wiki";
const string    TABLE_PRE           = "wiki_hot_feed_";
const Ice::Int  TABLE_COUNT         = 100;
const Ice::Int  LOAD_LIMIT          = 1000;
const string    SW_COL_WIKIID       = "wiki_id";
const string    SW_COL_FEEDID       = "feed_id";
const string    SW_COL_POSITION     = "position";
const string    SW_COL_UPDATETIME  = "update_time";
const string    SW_COL_TYPE         = "feed_type";

const Ice::Int  DURATION_TIME       = 30 * 24 * 60 * 60;    //30 days
const Ice::Int  EXPIRED_TIME        = 1 * 60 * 60;


//-----------------------------------------------------------------------------
//缓存结构
struct SocialWikiCacheItem {    //成员待修改
    Ice::Long feedid;
    Ice::Int position;
    Ice::Int update_time;
    Ice::Int type;

    SocialWikiCacheItem(const SocialWikiItem& item):
        feedid(item.feedid), position(0), update_time(item.time), type(item.type){}

    SocialWikiCacheItem(const Ice::Long fid, const Ice::Int pos, const Ice::Int time, const Ice::Int type):
        feedid(fid), position(pos), update_time(time), type(type) {}

    //待修改
    bool operator< (const SocialWikiCacheItem& res) const {
        return position < res.position;
    }
};
class SocialWiki: public ::IceUtil::RWRecMutex, public ::Ice::Object {
private:
    const static Ice::Int BUFFER_SIZE = 1000;
    typedef boost::circular_buffer<SocialWikiCacheItem> CBItems;
    CBItems feeds_;
    Ice::Int birthtime_;
public:
    SocialWiki(): feeds_(BUFFER_SIZE), birthtime_(time(NULL))
    {}

    void    put(const SocialWikiCacheItem& item);
    std::vector<SocialWikiCacheItem> get();
    void    del(const Ice::Long feedid);

    inline size_t   size() const;
    inline Ice::Int getBirthtime() const;
};
typedef IceUtil::Handle<SocialWiki> SocialWikiPtr;

class SocialWikiFactoryI: public MyUtil::ServantFactory {
public:
    SocialWikiFactoryI() {}
    virtual ~SocialWikiFactoryI(){}
    
    virtual Ice::ObjectPtr create(Ice::Int wikiid);
};

//-----------------------------------------------------------------------------
// ReloadTask
class ReloadTask: public IceUtil::Thread, public Singleton<ReloadTask> {
public:
    void put(Ice::Int wikiid);
    virtual void run();

private:
    vector<Ice::Int> ids_;
    set<Ice::Int> sids_;

    IceUtil::Monitor<IceUtil::Mutex> mutex_;
    const static size_t batchSize_ = 20;
    const static time_t batchTime_ = 2;
};

//-----------------------------------------------------------------------------
//SocialWikiHotByWikiidI
class SocialWikiHotByWikiidI: public SocialWikiHotByWikiid, 
    public Singleton<SocialWikiHotByWikiidI> {
public:
    SocialWikiHotByWikiidI();
    ~SocialWikiHotByWikiidI();

    virtual void put(const SocialWikiItem& item, const Ice::Current& cur = Ice::Current());
    virtual void puts(const SocialWikiItemSeq& items, const Ice::Current& cur = Ice::Current());

    virtual LongSeq get(Ice::Int wikiid, Ice::Int begin, Ice::Int limit, 
            const Ice::Current& cur = Ice::Current());
    virtual LongSeq getByTypes(Ice::Int wikiid, Ice::Int begin, Ice::Int limit, const MyUtil::IntSeq& types,
            const Ice::Current& cur = Ice::Current());

    virtual void remove(Ice::Int wikiid, Ice::Long feedid, const Ice::Current& cur = Ice::Current());

    virtual void reload(Ice::Int wikiid, const Ice::Current& cur = Ice::Current());
    
protected:
    void delWiki(Ice::Int wikiid);
};

}}//namespace end

#endif  //__SOCIALWIKI_HOTBYWIKIIDI_H__

