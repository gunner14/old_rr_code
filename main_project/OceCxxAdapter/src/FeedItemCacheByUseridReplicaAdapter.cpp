/*
 * =====================================================================================
 *
 *       Filename:  FeedItemCacheByUseridReplicaAdapter.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011年11月21日 15时42分33秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  guanghe.ge (ggh), guanghe.ge@opi-corp.com
 *        Company:  XiaoNei
 *
 * =====================================================================================
 */

#include "FeedItemCacheByUseridReplicaAdapter.h"

using namespace std;
using namespace MyUtil;
using namespace xce::feed;
using namespace xce::clusterstate;

void FeedItemCacheByUseridReplicaAdapter::put(const FeedMeta& meta) {
  vector<UserFeedsPrx> prxs = getAllProxySeq(meta.userid);

  for (vector<UserFeedsPrx>::iterator it = prxs.begin();
      it != prxs.end(); ++it) {
    try {
      (*it)->put(meta);
    } catch (Ice::Exception& e) {
      MCE_WARN("FeedItemCacheByUseridReplicaAdapter::put, userid: " << meta.userid << " feedid: " << meta.feedid << " proxy: " << *it << " Exception: " << e.what());
    } catch (...) {
      MCE_WARN("FeedItemCacheByUseridReplicaAdapter::put, userid: " << meta.userid << " feedid: " << meta.feedid << " proxy: " << *it << " Exception!");
    }
  }
}
FeedMetaSeq FeedItemCacheByUseridReplicaAdapter::get(const MyUtil::IntSeq& ids, int begin, int limit) {
  UserFeedsPrx prx = getProxy(0);
	try {
		FeedMetaSeq result = prx->get(ids, begin, limit);
		return result;
	} catch (Ice::Exception& e) {
		MCE_WARN("FeedItemCacheByUseridReplicaAdapter::get prx: " << prx << " Exception: " << e.what());
	}

	return FeedMetaSeq();
}

FeedMetaSeq FeedItemCacheByUseridReplicaAdapter::getByTypes(const MyUtil::IntSeq& ids, int begin, int limit, const MyUtil::IntSeq& types) {
  UserFeedsPrx prx = getProxy(0);
	try {
		FeedMetaSeq result = prx->getByTypes(ids, begin, limit, types);
		return result;
	} catch (Ice::Exception& e) {
		MCE_WARN("FeedItemCacheByUseridReplicaAdapter::getByTypes prx: " << prx << " Exception: " << e.what());
	}

	return FeedMetaSeq();
}

MyUtil::Int2IntMap FeedItemCacheByUseridReplicaAdapter::getItemCountMapByTypes(const MyUtil::IntSeq& ids, const MyUtil::IntSeq& types) {
  UserFeedsPrx prx = getProxy(0);
  try {
    std::cout << "start Map" << endl;
    Int2IntMap result = prx->getItemCountMapByTypes(ids,types);
    return result;  
  } catch (Ice::Exception& e){
    MCE_WARN("FeedItemCacheByUseridReplicaAdapter::getItemCountMapByTypes prx: " << prx << " Exception: " << e.what());
  }
  return Int2IntMap();
}
int FeedItemCacheByUseridReplicaAdapter::getItemCountByTypes(const MyUtil::IntSeq& ids, const MyUtil::IntSeq& types) {
	int result=-1;
  UserFeedsPrx prx = getProxy(0);
  try {
		result = prx->getItemCountByTypes(ids, types);
		return result;
	} catch (Ice::Exception& e) {
		MCE_WARN("FeedItemCacheByUseridReplicaAdapter::getItemCountByTypes prx: " << prx << " Exception: " << e.what());
	}

	return result;
}
FeedMetaSeq FeedItemCacheByUseridReplicaAdapter::getWithFilter(const MyUtil::IntSeq& ids, int begin, int limit, const MyUtil::StrSeq& filter) {
  UserFeedsPrx prx = getProxy(0);
	try {
		FeedMetaSeq result = prx->getWithFilter(ids, begin, limit, filter);
		return result;
	} catch (Ice::Exception& e) {
		MCE_WARN("FeedItemCacheByUseridReplicaAdapter::getWithFilter prx: " << prx << " Exception: " << e.what());
	}

	return FeedMetaSeq();
}

FeedMetaSeq FeedItemCacheByUseridReplicaAdapter::getByTypesWithFilter(const MyUtil::IntSeq& ids, int begin, int limit, const MyUtil::IntSeq& types, const MyUtil::StrSeq& filter) {
  UserFeedsPrx prx = getProxy(0);
	try {
		FeedMetaSeq result = prx->getByTypesWithFilter(ids, begin, limit, types, filter);
		return result;
	} catch (Ice::Exception& e) {
		MCE_WARN("FeedItemCacheByUseridReplicaAdapter::getByTypesWithFilter prx: " << prx << " Exception: " << e.what());
	}

	return FeedMetaSeq();
}

FeedMetaSeq FeedItemCacheByUseridReplicaAdapter::getByTypesWithFilterSimple(const MyUtil::IntSeq& ids, int begin, int limit, const MyUtil::IntSeq& types, const MyUtil::StrSeq& filter) {
  UserFeedsPrx prx = getProxy(0);
	try {
		FeedMetaSeq result = prx->getByTypesWithFilterSimple(ids, begin, limit, types, filter);
		return result;
	} catch (Ice::Exception& e) {
		MCE_WARN("FeedItemCacheByUseridReplicaAdapter::getByTypesWithFilterSimple prx: " << prx << " Exception: " << e.what());
	}

	return FeedMetaSeq();
}

FeedMetaSeq FeedItemCacheByUseridReplicaAdapter::getBiggerThanFeedid(const MyUtil::IntSeq& ids, long feedid, int begin, int limit, const MyUtil::IntSeq& types, const MyUtil::StrSeq& filter) {
  UserFeedsPrx prx = getProxy(0);
	try {
		FeedMetaSeq result = prx->getBiggerThanFeedid(ids, feedid, begin, limit, types, filter);
		return result;
	} catch (Ice::Exception& e) {
		MCE_WARN("FeedItemCacheByUseridReplicaAdapter::getBiggerThanFeedid prx: " << prx << " Exception: " << e.what());
	}

	return FeedMetaSeq();
}

FeedMetaSeq FeedItemCacheByUseridReplicaAdapter::getBiggerThanFeedidSimple(const MyUtil::IntSeq& ids, long feedid, int begin, int limit, const MyUtil::IntSeq& types, const MyUtil::StrSeq& filter) {
  UserFeedsPrx prx = getProxy(0);
	try {
		FeedMetaSeq result = prx->getBiggerThanFeedidSimple(ids, feedid, begin, limit, types, filter);
		return result;
	} catch (Ice::Exception& e) {
		MCE_WARN("FeedItemCacheByUseridReplicaAdapter::getBiggerThanFeedidSimple prx: " << prx << " Exception: " << e.what());
	}

	return FeedMetaSeq();
}

FeedMetaSeq FeedItemCacheByUseridReplicaAdapter::getLessThanFeedid(const MyUtil::IntSeq& ids, Ice::Long feedid, Ice::Int limit, const MyUtil::IntSeq& types, const MyUtil::StrSeq& filter) {
  UserFeedsPrx prx = getProxy(0);
	try {
		FeedMetaSeq result = prx->getLessThanFeedidByTypesWithFilter(ids, feedid, limit, types, filter);
		return result;
	} catch (Ice::Exception& e) {
		MCE_WARN("FeedItemCacheByUseridReplicaAdapter::getLessThanFeedid prx: " << prx << " Exception: " << e.what());
	}

	return FeedMetaSeq();
}

int FeedItemCacheByUseridReplicaAdapter::getItemCount(const MyUtil::IntSeq& ids) {
  UserFeedsPrx prx = getProxy(0);
	try {
		int count = prx->getItemCount(ids);
		return count;
	} catch (Ice::Exception& e) {
		MCE_WARN("FeedItemCacheByUseridReplicaAdapter::getItemCount prx: " << prx << " Exception: " << e.what());
	}

	return 0;
}

void FeedItemCacheByUseridReplicaAdapter::deleteFeed(int userid, long feedid) {
  vector<UserFeedsPrx> prxs = getAllProxySeqOneway(userid);

  for (vector<UserFeedsPrx>::iterator it = prxs.begin();
      it != prxs.end(); ++it) {
    try {
      (*it)->deleteFeed(userid, feedid);
    } catch (Ice::Exception & e) {
      MCE_WARN("FeedItemCacheReplicaAdapter::deleteFeed userid: " << userid << " feedid: " << feedid << " proxy: " << *it << " Exception: " << e.what());
    }
  }
}
