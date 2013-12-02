#include "TimeLineFeedReplicaAdapter.h"

using namespace MyUtil;
using namespace ::xce::feed::timeline;
using namespace xce::clusterstate;
using namespace std;

UidYear TimeLineFeedReplicaAdapter::Query(int uid) { //查询uid,哪年哪月有新鲜事
  return getProxy(uid)->Query(uid);
}
TimeLineFeedResult TimeLineFeedReplicaAdapter::QueryFeedInfo(int uid,int year,int month,int begin,int limit) {
  return getProxy(uid)->QueryFeedInfo(uid,year,month,begin,limit);
}
TimeLineFeedResult TimeLineFeedReplicaAdapter::QueryFeedInfoByStype(int uid,int year,int month,const MyUtil::IntSeq& types,int begin,int limit) 
{ 
  return getProxy(uid)->QueryFeedInfoByStype(uid,year,month,types,begin,limit);
}
int TimeLineFeedReplicaAdapter::QueryFeedSize(int uid,int year,int month) 
{
  return getProxy(uid)->QueryFeedSize(uid,year,month);
}
int TimeLineFeedReplicaAdapter::QueryFeedSizeByStype(int uid,int year,int month,const MyUtil::IntSeq& types) 
{
  return getProxy(uid)->QueryFeedSizeByStype(uid,year,month,types);
}
bool TimeLineFeedReplicaAdapter::SetWideScreen(int uid,int year,::Ice::Long feed,bool wide) 
{
  return getProxy(uid)->SetWideScreen(uid,year, feed,wide);
}
bool TimeLineFeedReplicaAdapter::IsWideScreen(int uid,int year,::Ice::Long feed) 
{
  return getProxy(uid)->IsWideScreen(uid,year,feed);
}
void TimeLineFeedReplicaAdapter::AddFeedMiniNotify(const FeedSeedPtr & seed,int type,::Ice::Long time)
{
  getProxyOneway(seed->actor)->AddFeedMiniNotify(seed,type,time);
}
void TimeLineFeedReplicaAdapter::Modify(int uid,int old_year,Ice::Long feed, int year,int month,int day) {
  getProxyOneway(uid)->Modify(uid,old_year,feed,year,month,day);
}
void TimeLineFeedReplicaAdapter::SetReadMini(int uid,int year, int stype, ::Ice::Long merge) {
  getProxy(uid)->SetReadMini(uid,year,stype,merge);
}
void TimeLineFeedReplicaAdapter::SetReadMiniByIds(int uid,int year,const MyUtil::LongSeq & feeds)
{
  getProxy(uid)->SetReadMiniByIds( uid,year,feeds);
}
void TimeLineFeedReplicaAdapter::SetReadMiniById(int uid,int year, ::Ice::Long feed)
{
  getProxyOneway(uid)->SetReadMiniById(uid,year,feed);
}
void TimeLineFeedReplicaAdapter::LoadFeedsToCache(int uid)
{
  getProxy(uid)->LoadFeedsToCache(uid);
}
std::string TimeLineFeedReplicaAdapter::Execute(int uid,const string & cmd)
{
  return getProxy(uid)->Execute(uid,cmd);
}

