
#include "FeedItemWrapperReplicaAdapter.h"

using namespace MyUtil;
using namespace xce::feed;


FeedItemWrapperPrx FeedItemWrapperReplicaAdapter::getManager(int userid) 
{
    return getProxy(userid);
}

FeedItemWrapperPrx FeedItemWrapperReplicaAdapter::getManagerOneway(int userid)
{
    return getProxyOneway(userid);
}

FeedMetaSeq FeedItemWrapperReplicaAdapter::get(int userid, int begin, int limit)
{
    return getManager(userid)->get(userid, begin, limit);
}

FeedMetaSeq FeedItemWrapperReplicaAdapter::getByTypes(int userid, int begin, int limit, const MyUtil::IntSeq& types)
{
    return getManager(userid)->getByTypes(userid, begin, limit, types);
}

FeedMetaSeq FeedItemWrapperReplicaAdapter::getByTypesWithFriends(int userid, const MyUtil::IntSeq & friends, int begin, int limit, const MyUtil::IntSeq& types)
{
    return getManager(userid)->getByTypesWithFriends(userid, friends, begin, limit, types);
}

FeedItemSeq FeedItemWrapperReplicaAdapter::getItemSeq(Ice::Int userid, Ice::Int begin, Ice::Int limit)
{
    return getManager(userid)->getItemSeq(userid, begin, limit);
}

FeedMetaSeq FeedItemWrapperReplicaAdapter::getLessThanFeedid(Ice::Int userid, Ice::Long feedid, Ice::Int limit)
{
    return getManager(userid)->getLessThanFeedid(userid, feedid, limit);
}
FeedMetaSeq FeedItemWrapperReplicaAdapter::getLessThanFeedidByTypes(Ice::Int userid, Ice::Long feedid, Ice::Int limit, const MyUtil::IntSeq& types)
{
    return getManager(userid)->getLessThanFeedidByTypes(userid, feedid, limit, types);
}

FeedMetaSeq FeedItemWrapperReplicaAdapter::GetFeeds(int userid, 
        int begin, int limit, const MyUtil::IntSeq& types)
{
    return getManager(userid)->GetFeeds(userid, begin, limit, types);
}

FeedMetaSeq FeedItemWrapperReplicaAdapter::GetFeedsByGroupId(int userid, int id, 
        int begin, int limit, const MyUtil::IntSeq& types) 
{
    return getManager(userid)->GetFeedsByGroupId(userid, id, begin, limit, types);
}

FeedMetaSeq FeedItemWrapperReplicaAdapter::GetFeedsByGroupName(int userid, const string& name, 
        int begin, int limit, const MyUtil::IntSeq& types) 
{
    return getManager(userid)->GetFeedsByGroupName(userid, name, begin, limit, types);
}

FeedMetaSeq FeedItemWrapperReplicaAdapter::GetFeedsByFocus(int userid, 
        int begin, int limit, const MyUtil::IntSeq& types) 
{ 
    return getManager(userid)->GetFeedsByFocus(userid, begin, limit, types);
}

MyUtil::LongSeq FeedItemWrapperReplicaAdapter::GetFriendsFeedsList(int userid) {
  return getManager(userid)->GetFriendsFeedsList(userid);
}
