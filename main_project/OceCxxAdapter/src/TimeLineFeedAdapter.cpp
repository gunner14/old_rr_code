 #include "TimeLineFeedAdapter.h"

using namespace MyUtil;

using namespace ::xce::feed::timeline;

TimeLineFeedPrx TimeLineFeedAdapter::getManager(int id) {
    return locate<TimeLineFeedPrx> (_managers, "TimeLineFeed", id, TWO_WAY);
}

TimeLineFeedPrx TimeLineFeedAdapter::getManagerOneway(int id) {
    return locate<TimeLineFeedPrx> (_managersOneway, "TimeLineFeed", id, ONE_WAY);
}
UidYear TimeLineFeedAdapter::Query(int uid)
{
    return getManager(0)->Query(uid);
}
TimeLineFeedResult TimeLineFeedAdapter::QueryFeedInfoByStype(int uid,int year,int month,const MyUtil::IntSeq& types,int begin,int limit)
{
    return getManager(0)->QueryFeedInfoByStype(uid,year,month,types,begin,limit);
}

TimeLineFeedResult TimeLineFeedAdapter::QueryFeedInfo(int uid,int year,int month,int begin,int limit)
{
    return getManager(0)->QueryFeedInfo( uid, year, month, begin,limit);
}

int TimeLineFeedAdapter::QueryFeedSize(int uid,int year,int month)
{
    return getManager(0)->QueryFeedSize( uid, year, month);
}
int TimeLineFeedAdapter::QueryFeedSizeByStype(int uid,int year,int month,const MyUtil::IntSeq& types)
{
    return getManager(0)->QueryFeedSizeByStype( uid, year, month,types);
}
bool TimeLineFeedAdapter::IsWideScreen(int uid,int year, ::Ice::Long feed)
{
    return getManager(0)->IsWideScreen( uid,year, feed);
}
bool TimeLineFeedAdapter::SetWideScreen(int uid,int year, ::Ice::Long feed, bool wide)
{
    return getManager(0)->SetWideScreen( uid,year,feed, wide);
}
void TimeLineFeedAdapter::AddFeedMiniNotify(const FeedSeedPtr & seed,int type,::Ice::Long time)
{
    return getManager(0)->AddFeedMiniNotify(seed,type,time);
}
void TimeLineFeedAdapter::Modify(int uid,int old_year, Ice::Long feed, int year,int month,int day)
{
    return getManager(0)->Modify( uid, old_year,feed, year,month,day);
}
void TimeLineFeedAdapter::SetReadMini(int uid,int year, int stype, ::Ice::Long merge)
{
    return getManager(0)->SetReadMini(uid,year,stype, merge);
}
void TimeLineFeedAdapter::SetReadMiniByIds(int uid,int year,const MyUtil::LongSeq & feeds)
{
    return getManager(0)->SetReadMiniByIds( uid,year, feeds);
}
void TimeLineFeedAdapter::SetReadMiniById(int uid,int year, ::Ice::Long feed)
{
    return getManager(0)->SetReadMiniById( uid,year, feed);
}
void TimeLineFeedAdapter::LoadFeedsToCache(int uid)
{
    return getManager(0)->LoadFeedsToCache( uid);
}
string TimeLineFeedAdapter::Execute(int uid,const string & cmd)
{
    return getManager(0)->Execute(uid,cmd);
}
