/* ------------------------------------------------------------------
 * FileName:      SocialWikiHotByWikiidReplicaAdapter.cpp
 * Desc:          
 * Author:        jin.zhang
 * Email:         jin.zhang@renren-inc.com
 * Create:        2012-03-12 12:14:10
 * LastChange:    2012-03-14 18:08:05
 * History:       
 *------------------------------------------------------------------ */

#include "SocialWikiHotByWikiidReplicaAdapter.h"
#include <map>

using namespace xce::feed;
using namespace MyUtil;
using namespace std;

SocialWikiHotByWikiidPrx SocialWikiHotByWikiidReplicaAdapter::getManager(Ice::Int wikiid) {
    return getProxy(wikiid);
}
SocialWikiHotByWikiidPrx SocialWikiHotByWikiidReplicaAdapter::getManagerOneway(Ice::Int wikiid) {
    return getProxyOneway(wikiid);
}

void SocialWikiHotByWikiidReplicaAdapter::put(const SocialWikiItem& item) {
    getManager(item.wikiid)->put(item);
}

void SocialWikiHotByWikiidReplicaAdapter::puts(const SocialWikiItemSeq& items) {
    Ice::Int cluster = getCluster();

    map<Ice::Int, SocialWikiItemSeq> mod2seq;
    for (SocialWikiItemSeq::const_iterator it = items.begin(); it != items.end(); it++) {
        Ice::Int mod = it->wikiid % cluster;
        if (mod2seq.find(mod) != mod2seq.end()) {
            mod2seq[mod].push_back(*it);
        } else {
            SocialWikiItemSeq iseq;
            iseq.push_back(*it);
            mod2seq.insert(make_pair<Ice::Int, SocialWikiItemSeq>(mod, iseq));
        }
    }

    for (map<Ice::Int, SocialWikiItemSeq>::iterator it = mod2seq.begin(); it != mod2seq.end(); it++) {
        getManagerOneway(it->first)->puts(it->second);
    }
}

LongSeq SocialWikiHotByWikiidReplicaAdapter::get(Ice::Int wikiid, Ice::Int begin, Ice::Int limit) {
    return getManager(wikiid)->get(wikiid, begin, limit);
}

LongSeq SocialWikiHotByWikiidReplicaAdapter::getByTypes(Ice::Int wikiid, Ice::Int begin, Ice::Int limit, const IntSeq& types) {
    return getManager(wikiid)->getByTypes(wikiid, begin, limit, types);
}

void SocialWikiHotByWikiidReplicaAdapter::remove(Ice::Int wikiid, Ice::Long feedid) {
    getManagerOneway(wikiid)->remove(wikiid, feedid);
}

void SocialWikiHotByWikiidReplicaAdapter::reload(Ice::Int wikiid) {
    getManagerOneway(wikiid)->reload(wikiid);
}

