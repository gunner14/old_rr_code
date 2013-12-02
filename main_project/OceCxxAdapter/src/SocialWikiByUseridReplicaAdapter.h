/*
 * =================================================================
 * 
 *       Author :   duo.zhang
 * Organization :   RenRen-INC
 *        Email :   duo.zhang@renren-inc.com
 *      Created :   2012-03-09  14:18:37
 *  Description :   
 *
 * =================================================================
 */
 
#ifndef _FEED_SOCIALWIKI_SOCIALWIKIBYUSERID_SRC_SOCIALWIKIBYUSERIDREPLICAADAPTER_H_
#define _FEED_SOCIALWIKI_SOCIALWIKIBYUSERID_SRC_SOCIALWIKIBYUSERIDREPLICAADAPTER_H_

#include "AdapterI.h"
#include "FeedSocialWiki.h"

using namespace xce::feed;
using namespace MyUtil;

class SocialWikiByUseridReplicaAdapter : public MyUtil::ReplicatedClusterAdapterI<SocialWikiByUseridPrx>, public Singleton<SocialWikiByUseridReplicaAdapter>
{
public:
	SocialWikiByUseridReplicaAdapter(): MyUtil::ReplicatedClusterAdapterI <SocialWikiByUseridPrx> ("ControllerSocialWikiByUserid", 120, 300, new XceFeedControllerChannel()) { 
	}
	~SocialWikiByUseridReplicaAdapter() {}

	void put(const SocialWikiItemSeq & );
	LongSeq gets(const IntSeq &, int , int , int);
	LongSeq getsWithType(const IntSeq &, int , const IntSeq & , int , int);
	void deleteFeed(int, const IntSeq &, long);

private:
	SocialWikiByUseridPrx getManager(int id);
	SocialWikiByUseridPrx getManagerOneway(int id);
};

#endif
