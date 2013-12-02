/* ------------------------------------------------------------------
 * FileName:      SocialWikiNewsByWikiidI.h
 * Desc:          
 * Author:        jin.zhang
 * Email:         jin.zhang@renren-inc.com
 * Create:        2012-03-09 16:07:43
 * LastChange:    2012-03-16 18:05:57
 * History:       
 *------------------------------------------------------------------ */
#ifndef __SOCIALWIKI_NEWSBYWIKIIDI_H__
#define __SOCIALWIKI_NEWSBYWIKIIDI_H__

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

const string    INSTANCE_NAME       = "feed_social_wiki";
const string    TABLE_PRE           = "feed_social_wiki_";
const Ice::Int  TABLE_COUNT         = 100;
const Ice::Int  LOAD_LIMIT          = 1000;
const string    SW_COL_WIKIID       = "wikiid";
const string    SW_COL_FEEDID       = "feedid";
const string    SW_COL_TYPE         = "type";
const string    SW_COL_PRIVACY      = "privacy";
const string    SW_COL_TIME         = "time";

//const Ice::Int  PRIVACY_PUBLIC      = 0;
const Ice::Int  DURATION_TIME       = 15 * 24 * 60 * 60;


//-----------------------------------------------------------------------------
//缓存结构
struct SocialWikiCacheItem {
    Ice::Long feedid;
    Ice::Int type;
    Ice::Int time;

    SocialWikiCacheItem(const SocialWikiItem& item):
        feedid(item.feedid), type(item.type), time(item.time) {}
    SocialWikiCacheItem(const Ice::Long fid, const Ice::Int type, const Ice::Int time):
        feedid(fid), type(type), time(time) {}

    bool operator< (const SocialWikiCacheItem& res) const {
        if (res.time == time)
            return feedid > res.feedid;
        return time > res.time;
    }
};
class SocialWiki: public ::IceUtil::Mutex, public ::Ice::Object {
private:
    const static size_t BUFFER_SIZE = 1000;
    typedef boost::circular_buffer<SocialWikiCacheItem> CBItems;
    CBItems feeds_;
public:
    SocialWiki(): feeds_(BUFFER_SIZE)
    {}

    void    put(const SocialWikiCacheItem& item);
    std::vector<SocialWikiCacheItem> get();
    void    del(const Ice::Long feedid);

    inline size_t size() const;
};
typedef IceUtil::Handle<SocialWiki> SocialWikiPtr;

class SocialWikiFactoryI: public MyUtil::ServantFactory {
public:
    SocialWikiFactoryI() {}
    virtual ~SocialWikiFactoryI(){}
    
    virtual Ice::ObjectPtr create(Ice::Int wikiid);
};

//-----------------------------------------------------------------------------
//SocialWikiNewsByWikiidI
class SocialWikiNewsByWikiidI: public SocialWikiNewsByWikiid, 
    public Singleton<SocialWikiNewsByWikiidI> {
public:
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

#endif  //__SOCIALWIKI_NEWSBYWIKIIDI_H__

