/*
 * =====================================================================================
 *
 *       Filename:  FeedItemFacadeReplicaAdapter.cpp
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

#include "FeedItemFacadeReplicaAdapter.h"

using namespace std;
using namespace MyUtil;
using namespace xce::feed;
using namespace xce::clusterstate;

//void FeedItemFacadeReplicaAdapter::put(const FeedSeedPtr& seed, const ::Ice::Current& current) {
//    try {
//        getProxyOneway(seed->actor)->put(seed,current.ctx);
//
//    } catch (Ice::Exception& e) {
//        MCE_WARN("FeedItemFacadeReplicaAdapter::put, userid: " << seed->actor << " feedid: " << seed->feed << e.what());
//    } catch (...) {
//        MCE_WARN("FeedItemFacadeReplicaAdapter::put, userid: " << seed->actor << " feedid: " << seed->feed << " Exception!");
//    }
//}

void FeedItemFacadeReplicaAdapter::put(const FeedSeedPtr& seed) {
    try {
        getProxyOneway(seed->actor)->put(seed);
    } catch (Ice::Exception& e) {
        MCE_WARN("FeedItemFacadeReplicaAdapter::put, userid: " << seed->actor << " feedid: " << seed->feed << e.what());
    } catch (...) {
        MCE_WARN("FeedItemFacadeReplicaAdapter::put, userid: " << seed->actor << " feedid: " << seed->feed << " Exception!");
    }
}

void FeedItemFacadeReplicaAdapter::deleteFeed(Ice::Int userid, Ice::Long feedid) {
    try {
        getProxyOneway(userid)->deleteFeed(userid, feedid);
    } catch (Ice::Exception& e) {
        MCE_WARN("FeedItemFacadeReplicaAdapter::deleteFeed, userid: " << userid << " feedid: " << feedid);
    } catch (...) {
        MCE_WARN("FeedItemFacadeReplicaAdapter::deleteFeed, userid: " << userid << " feedid: " << feedid << " Exception!");
    }
}

void FeedItemFacadeReplicaAdapter::delWikiFeed(Ice::Int userid, Ice::Long source, Ice::Int stype) {
    try {
        getProxyOneway(userid)->delWikiFeed(userid, source, stype);
    } catch (Ice::Exception& e) {
        MCE_WARN("FeedItemFacadeReplicaAdapter::delWikiFeed, userid: " << userid << " source: " << source << " stype: " << stype << e.what());
    } catch (...) {
        MCE_WARN("FeedItemFacadeReplicaAdapter::delWikiFeed, userid: " << userid << " source: " << source << " stype: " << stype << " Exception!");
    }
}

void FeedItemFacadeReplicaAdapter::changeWikiFeedPrivacy(Ice::Int userid, Ice::Long source, Ice::Int stype, Ice::Int privacy) {
    try {
        getProxyOneway(userid)->changeWikiFeedPrivacy(userid, source, stype, privacy);
    } catch (Ice::Exception& e) {
        MCE_WARN("FeedItemFacadeReplicaAdapter::changeWikiFeedPrivacy, userid: " << userid << " source: " << source << " stype: " << stype << e.what());
    } catch (...) {
        MCE_WARN("FeedItemFacadeReplicaAdapter::changeWikiFeedPrivacy, userid: " << userid << " source: " << source << " stype: " << stype << " Exception!");
    }
}


