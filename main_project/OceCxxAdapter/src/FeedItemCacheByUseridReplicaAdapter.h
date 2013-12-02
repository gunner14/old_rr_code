/*
 * =====================================================================================
 *
 *       Filename:  FeedItemCacheByUseridReplicaAdapter.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011年11月21日 14时22分19秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  guanghe.ge (ggh), guanghe.ge@opi-corp.com
 *        Company:  XiaoNei
 *
 * =====================================================================================
 */

#ifndef __FEEDITEMCACHE_BYUSERID_REPLICA_ADAPTER_H__
#define __FEEDITEMCACHE_BYUSERID_REPLICA_ADAPTER_H__

#include "UserFeeds.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"
#include "ReplicaCluster/ClusterState/src/client/cxx/ClientInterface.h"

namespace xce {
namespace feed {

using namespace MyUtil;

class FeedItemCacheByUseridReplicaAdapter : public MyUtil::ReplicatedClusterAdapterI<UserFeedsPrx>, public MyUtil::Singleton<FeedItemCacheByUseridReplicaAdapter> {
public:
	FeedItemCacheByUseridReplicaAdapter() : 
		MyUtil::ReplicatedClusterAdapterI<UserFeedsPrx> ("ControllerFeedItemCacheByUserid", 120, 300, new XceFeedControllerChannel()){
	}

	void put(const FeedMeta& d);
	FeedMetaSeq get(const MyUtil::IntSeq& uids, int begin, int limit);
	FeedMetaSeq getByTypes(const MyUtil::IntSeq& uids, int begin, int limit, const MyUtil::IntSeq& types);
	FeedMetaSeq getWithFilter(const MyUtil::IntSeq& uids, int begin, int limit, const StrSeq& filter);
	FeedMetaSeq getByTypesWithFilter(const MyUtil::IntSeq& uids, int begin, int limit, const MyUtil::IntSeq& types, const StrSeq& filter);
	FeedMetaSeq getByTypesWithFilterSimple(const MyUtil::IntSeq& uids, int begin, int limit, const MyUtil::IntSeq& types, const StrSeq& filter);

	FeedMetaSeq getBiggerThanFeedid(const MyUtil::IntSeq& uids, long feedid, int begin, int limit, const MyUtil::IntSeq& types, const StrSeq& filter);
	FeedMetaSeq getBiggerThanFeedidSimple(const MyUtil::IntSeq& uids, long feedid, int begin, int limit, const MyUtil::IntSeq& types, const StrSeq& filter);
  int getItemCountByTypes(const MyUtil::IntSeq& uids,const MyUtil::IntSeq& types);
  MyUtil::Int2IntMap getItemCountMapByTypes(const MyUtil::IntSeq& userids, const MyUtil::IntSeq& types);
	FeedMetaSeq getLessThanFeedid(const MyUtil::IntSeq& uids, ::Ice::Long feedid, ::Ice::Int limit, const MyUtil::IntSeq& types, const StrSeq& filter);

	int getItemCount(const MyUtil::IntSeq& uids);
	void deleteFeed(int userid, long feedid);
};//end FeedItemCacheByUseridReplicaAdapter

};
};

#endif
