#include "FeedItemBlockCacheAdapter.h"

using namespace xce::feed;

FeedItemBlockCacheAdapter::FeedItemBlockCacheAdapter()
{
    _managersOneway.resize(cluster() > 0 ? cluster() : 1);
    _managers.resize(cluster() > 0 ? cluster() : 1);
}

FeedItemBlockCachePrx FeedItemBlockCacheAdapter::getManager(Ice::Int userid)
{
    return locate<FeedItemBlockCachePrx>(_managers, "M", userid, TWO_WAY);
}

FeedItemBlockCachePrx FeedItemBlockCacheAdapter::getManagerOneway(Ice::Int userid)
{
    return locate<FeedItemBlockCachePrx>(_managersOneway, "M", userid, ONE_WAY);
}

//----------------------------------------------------------------------------
//
IntSeq FeedItemBlockCacheAdapter::getUserBlockTypes(Ice::Int userid)
{
    return getManager(userid)->getUserBlockTypes(userid);
}

void FeedItemBlockCacheAdapter::addUserBlockType(Ice::Int userid, Ice::Int type)
{
    getManager(userid)->addUserBlockType(userid, type);
}

void FeedItemBlockCacheAdapter::removeUserBlockType(Ice::Int userid, Ice::Int type)
{
    getManager(userid)->removeUserBlockType(userid, type);
}

bool FeedItemBlockCacheAdapter::hasUserBlockType(Ice::Int userid, Ice::Int type)
{
    return getManager(userid)->hasUserBlockType(userid, type);
}

//----------------------------------------------------------------------------
//
IntSeq FeedItemBlockCacheAdapter::getUserBlockUsers(Ice::Int userid)
{
    return getManager(userid)->getUserBlockUsers(userid);
}

void FeedItemBlockCacheAdapter::addUserBlockUser(Ice::Int userid, Ice::Int block_userid)
{
    getManager(userid)->addUserBlockUser(userid, block_userid);
}

void FeedItemBlockCacheAdapter::removeUserBlockUser(Ice::Int userid, Ice::Int block_userid)
{
    getManager(userid)->removeUserBlockUser(userid, block_userid);
}

bool FeedItemBlockCacheAdapter::hasUserBlockUser(Ice::Int userid, Ice::Int block_userid)
{
    return getManager(userid)->hasUserBlockUser(userid, block_userid);
}

//----------------------------------------------------------------------------
//
LongSeq FeedItemBlockCacheAdapter::getUserBlockFeeds(Ice::Int userid)
{
    return getManager(userid)->getUserBlockFeeds(userid);
}

void FeedItemBlockCacheAdapter::addUserBlockFeed(Ice::Int userid, Ice::Long feedid)
{
    getManager(userid)->addUserBlockFeed(userid, feedid);
}

void FeedItemBlockCacheAdapter::removeUserBlockFeed(Ice::Int userid, Ice::Long feedid)
{
    getManager(userid)->removeUserBlockFeed(userid, feedid);
}

bool FeedItemBlockCacheAdapter::hasUserBlockFeed(Ice::Int userid, Ice::Long feedid)
{
    return getManager(userid)->hasUserBlockFeed(userid, feedid);
}

//-----------------------------------------------------------------------------
UserBlockInfoPtr FeedItemBlockCacheAdapter::getAll(Ice::Int userid)
{
    return getManager(userid)->getAll(userid);
}
