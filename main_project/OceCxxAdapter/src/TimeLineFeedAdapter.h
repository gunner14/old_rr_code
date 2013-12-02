
#include "AdapterI.h"
#include "TimeLineFeed.h"
#include "Channel.h"
 
namespace xce {
namespace feed {
namespace timeline {

class TimeLineFeedAdapter: public MyUtil::AdapterI,
    public MyUtil::AdapterISingleton<MyUtil::XceFeedChannel, TimeLineFeedAdapter>
{
public:
    ::xce::feed::timeline::UidYear Query(int uid);
    ::xce::feed::timeline::TimeLineFeedResult QueryFeedInfoByStype(int uid,int year,int month,const MyUtil::IntSeq& types,int begin,int limit);
    ::xce::feed::timeline::TimeLineFeedResult QueryFeedInfo(int uid,int year,int month,int begin,int limit);
    int QueryFeedSize(int uid,int year,int month);
    int QueryFeedSizeByStype(int uid,int year,int month,const MyUtil::IntSeq& types);
    bool IsWideScreen(int uid,int year, ::Ice::Long feed);
    bool SetWideScreen(int uid,int year, ::Ice::Long feed, bool wide);
    void AddFeedMiniNotify(const FeedSeedPtr & seed,int type,::Ice::Long time);
    void Modify(int uid,int old_year, Ice::Long feed, int year,int month,int day);
    void SetReadMini(int uid,int year, int stype, ::Ice::Long merge);
    void SetReadMiniByIds(int uid,int year,const MyUtil::LongSeq & feeds);
    void SetReadMiniById(int uid,int year, ::Ice::Long feed);
    void LoadFeedsToCache(int uid);
    std::string Execute(int uid,const string & cmd);


   ::xce::feed::timeline::TimeLineFeedPrx getManager(int id);
   ::xce::feed::timeline::TimeLineFeedPrx getManagerOneway(int id);


protected:
   virtual string name() {
      return "TimeLineFeed";
   }
   virtual string endpoints() {
      return "@TimeLineFeed";
   }
   virtual size_t cluster() {
      return 1;
   }


  vector< ::xce::feed::timeline::TimeLineFeedPrx> _managersOneway;
  vector< ::xce::feed::timeline::TimeLineFeedPrx> _managers;
};
}
}
}

