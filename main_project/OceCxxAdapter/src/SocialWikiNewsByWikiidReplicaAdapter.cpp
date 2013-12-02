/* ------------------------------------------------------------------
 * FileName:      SocialWikiNewsByWikiidReplicaAdapter.cpp
 * Desc:          
 * Author:        jin.zhang
 * Email:         jin.zhang@renren-inc.com
 * Create:        2012-03-12 12:14:10
 * LastChange:    2012-03-12 19:00:18
 * History:       
 *------------------------------------------------------------------ */

#include "SocialWikiNewsByWikiidReplicaAdapter.h"
#include <map>

using namespace xce::feed;
using namespace MyUtil;
using namespace std;

SocialWikiNewsByWikiidPrx SocialWikiNewsByWikiidReplicaAdapter::getManager(Ice::Int wikiid) {
    return getProxy(wikiid);
}
SocialWikiNewsByWikiidPrx SocialWikiNewsByWikiidReplicaAdapter::getManagerOneway(Ice::Int wikiid) {
    return getProxyOneway(wikiid);
}

void SocialWikiNewsByWikiidReplicaAdapter::put(const SocialWikiItem& item) {
    getManagerOneway(item.wikiid)->put(item);
}

void SocialWikiNewsByWikiidReplicaAdapter::puts(const SocialWikiItemSeq& items) {
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

LongSeq SocialWikiNewsByWikiidReplicaAdapter::get(Ice::Int wikiid, Ice::Int begin, Ice::Int limit) {
    return getManager(wikiid)->get(wikiid, begin, limit);
}

LongSeq SocialWikiNewsByWikiidReplicaAdapter::getByTypes(Ice::Int wikiid, Ice::Int begin, Ice::Int limit, const IntSeq& types) {
    return getManager(wikiid)->getByTypes(wikiid, begin, limit, types);
}

void SocialWikiNewsByWikiidReplicaAdapter::remove(Ice::Int wikiid, Ice::Long feedid) {
    getManagerOneway(wikiid)->remove(wikiid, feedid);
}

void SocialWikiNewsByWikiidReplicaAdapter::reload(Ice::Int wikiid) {
    getManagerOneway(wikiid)->reload(wikiid);
}

