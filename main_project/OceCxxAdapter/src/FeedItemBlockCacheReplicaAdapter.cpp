
#include "FeedItemBlockCacheReplicaAdapter.h"

using namespace xce::feed;
using namespace MyUtil;


FeedItemBlockCachePrx FeedItemBlockCacheReplicaAdapter::getManager(Ice::Int userid)
{
    return getProxy(userid);
}

FeedItemBlockCachePrx FeedItemBlockCacheReplicaAdapter::getManagerOneway(Ice::Int userid)
{
    return getProxyOneway(userid);
}

//----------------------------------------------------------------------------
//
MyUtil::IntSeq FeedItemBlockCacheReplicaAdapter::getUserBlockTypes(Ice::Int userid)
{
    return getManager(userid)->getUserBlockTypes(userid);
}

void FeedItemBlockCacheReplicaAdapter::addUserBlockType(Ice::Int userid, Ice::Int type)
{
    getManagerOneway(userid)->addUserBlockType(userid, type);
}

void FeedItemBlockCacheReplicaAdapter::removeUserBlockType(Ice::Int userid, Ice::Int type)
{
    getManagerOneway(userid)->removeUserBlockType(userid, type);
}

bool FeedItemBlockCacheReplicaAdapter::hasUserBlockType(Ice::Int userid, Ice::Int type)
{
    return getManager(userid)->hasUserBlockType(userid, type);
}

//----------------------------------------------------------------------------
//
IntSeq FeedItemBlockCacheReplicaAdapter::getUserBlockUsers(Ice::Int userid)
{
    return getManager(userid)->getUserBlockUsers(userid);
}

void FeedItemBlockCacheReplicaAdapter::addUserBlockUser(Ice::Int userid, Ice::Int block_userid)
{
    getManagerOneway(userid)->addUserBlockUser(userid, block_userid);
}

void FeedItemBlockCacheReplicaAdapter::removeUserBlockUser(Ice::Int userid, Ice::Int block_userid)
{
    getManagerOneway(userid)->removeUserBlockUser(userid, block_userid);
}

bool FeedItemBlockCacheReplicaAdapter::hasUserBlockUser(Ice::Int userid, Ice::Int block_userid)
{
    return getManager(userid)->hasUserBlockUser(userid, block_userid);
}

//----------------------------------------------------------------------------
//
LongSeq FeedItemBlockCacheReplicaAdapter::getUserBlockFeeds(Ice::Int userid)
{
    return getManager(userid)->getUserBlockFeeds(userid);
}

void FeedItemBlockCacheReplicaAdapter::addUserBlockFeed(Ice::Int userid, Ice::Long feedid)
{
    //getManager(userid)->addUserBlockFeed(userid, feedid);
    getManagerOneway(userid)->addUserBlockFeed(userid, feedid);
}

void FeedItemBlockCacheReplicaAdapter::removeUserBlockFeed(Ice::Int userid, Ice::Long feedid)
{
    getManagerOneway(userid)->removeUserBlockFeed(userid, feedid);
}

bool FeedItemBlockCacheReplicaAdapter::hasUserBlockFeed(Ice::Int userid, Ice::Long feedid)
{
    return getManager(userid)->hasUserBlockFeed(userid, feedid);
}


//-----------------------------------------------------------------------------
// readAllFeed
void FeedItemBlockCacheReplicaAdapter::readAllFeeds(Ice::Int userid)
{
    MyUtil::IntSeq friends = 
        xce::buddy::adapter::BuddyByOnlineTimeCacheAdapter::instance().getFriendList(userid, -1);

    FeedMetaSeq feeds = FeedItemCacheByUseridReplicaAdapter::instance().get(friends, 0, 1);

    if (!feeds.empty()) {
        getManagerOneway(userid)->setMaxReadFeed(userid, feeds[0].feedid);
    }
}

//-----------------------------------------------------------------------------
UserBlockInfoPtr FeedItemBlockCacheReplicaAdapter::getAll(Ice::Int userid)
{
    return getManager(userid)->getAll(userid);
}


//-----------------------------------------------------------------------------
// reload
void FeedItemBlockCacheReplicaAdapter::reloadUserBlockTypes(Ice::Int userid)
{
    getManagerOneway(userid)->reloadUserBlockTypes(userid);
}

void FeedItemBlockCacheReplicaAdapter::reloadUserBlockUsers(Ice::Int userid)
{
    getManagerOneway(userid)->reloadUserBlockUsers(userid);
}

void FeedItemBlockCacheReplicaAdapter::reloadUserBlockFeeds(Ice::Int userid)
{
    getManagerOneway(userid)->reloadUserBlockFeeds(userid);
}

void FeedItemBlockCacheReplicaAdapter::reloadAll(Ice::Int userid)
{
    getManagerOneway(userid)->reloadAll(userid);
}
