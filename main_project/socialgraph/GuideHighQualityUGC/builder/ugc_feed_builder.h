#ifndef  UGC_FEED_BUILDER_H_
#define  UGC_FEED_BUILDER_H_

#include <boost/shared_ptr.hpp>
#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace socialgraph {
namespace guideugc {

/*
*the UGC feeds for one guide
*/
const static int  MAX_UGC_FEED_SIZE = 2*3*31; 

class  GuideUserUGCFeed {
public:
       GuideUserUGCFeed(int userid): _userid(userid){ _day = _next = -1; }
      ~GuideUserUGCFeed() { }
       int    PushUGCfeeds(int limit);
private:
       void   LoadUGCFeeds(int limit);
       int    SendStrategy(int limit);
private:
       int    _userid;
       int    _next;
       int    _day;
       MyUtil::LongSeq  _feeds;       
};


/*
* the mgr of feeds
*/
class   GuideUserUGCFeedMgr: public MyUtil::Singleton<GuideUserUGCFeedMgr> {
 public:
        int   PushUGCFeeds(int userid, int limit = 3);
 protected:
        bool  IsNeedReset();
        void  Update(); 
 private:
	GuideUserUGCFeedMgr() { _limit = MAX_UGC_FEED_SIZE; _year = _month = -1; }
       ~GuideUserUGCFeedMgr() { }
        int    _limit;
        int    _year;
        int    _month;	
        IceUtil::RWRecMutex  _monitor;
        std::map< int, GuideUserUGCFeed >  _guideugcfeeds;
       
        friend class MyUtil::Singleton<GuideUserUGCFeedMgr>;
};

}}}

#endif
