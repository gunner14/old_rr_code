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
 
#include "SocialWikiByUseridReplicaAdapter.h"

SocialWikiByUseridPrx SocialWikiByUseridReplicaAdapter::getManager(int id) {
	return getProxy(id);
}

SocialWikiByUseridPrx SocialWikiByUseridReplicaAdapter::getManagerOneway(int id) {
	return getProxyOneway(id);
}

void SocialWikiByUseridReplicaAdapter::put(const SocialWikiItemSeq & feeditems) {
	for(SocialWikiItemSeq::const_iterator it = feeditems.begin(); it != feeditems.end(); ++it)
		getManagerOneway(0)->put(*it);
}

LongSeq SocialWikiByUseridReplicaAdapter::gets(const IntSeq & userids, int wikiid, int begin, int limit) {
	return getManager(0)->gets(userids, wikiid, begin, limit);
}

LongSeq SocialWikiByUseridReplicaAdapter::getsWithType(const IntSeq & userids, int wikiid, const IntSeq & types, int begin, int limit) {
	return getManager(0)->getsWithType(userids, wikiid, types, begin, limit);
}

void SocialWikiByUseridReplicaAdapter::deleteFeed(int userid, const IntSeq & wikiids, long feedid) {
	getManagerOneway(0)->deleteFeed(userid, wikiids, feedid);
}
