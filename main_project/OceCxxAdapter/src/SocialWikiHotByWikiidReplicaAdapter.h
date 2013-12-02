/* ------------------------------------------------------------------
 * FileName:      SocialWikiHotByWikiidReplicaAdapter.h
 * Desc:          
 * Author:        jin.zhang
 * Email:         jin.zhang@renren-inc.com
 * Create:        2012-03-12 12:14:20
 * LastChange:    2012-03-14 18:05:43
 * History:       
 *------------------------------------------------------------------ */

#ifndef __SOCIALWIKIHOT_BYWIKIID_REPLICAADAPTER_H__
#define __SOCIALWIKIHOT_BYWIKIID_REPLICAADAPTER_H__

#include "Singleton.h"
#include "AdapterI.h"
#include "Channel.h"
#include "ReplicaCluster/ClusterState/src/client/cxx/ClientInterface.h"
#include "FeedSocialWiki.h"

namespace xce { namespace feed {
using namespace MyUtil;

class SocialWikiHotByWikiidReplicaAdapter : public MyUtil::ReplicatedClusterAdapterI<SocialWikiHotByWikiidPrx>,
    public MyUtil::Singleton<SocialWikiHotByWikiidReplicaAdapter> 
{
public:
    SocialWikiHotByWikiidReplicaAdapter() : 
        MyUtil::ReplicatedClusterAdapterI<SocialWikiHotByWikiidPrx>(
                "ControllerSocialWikiHotByWikiid", 120, 300, new XceFeedControllerChannel())
    {}

    void put(const SocialWikiItem& item);
    void puts(const SocialWikiItemSeq& items);

    LongSeq get(Ice::Int wikiid, Ice::Int begin, Ice::Int limit);
    LongSeq getByTypes(Ice::Int wikiid, Ice::Int begin, Ice::Int limit, const IntSeq& types);

    void remove(Ice::Int wikiid, Ice::Long feedid);

    void reload(Ice::Int wikiid);
protected:
    inline SocialWikiHotByWikiidPrx getManager(Ice::Int wikiid);
    inline SocialWikiHotByWikiidPrx getManagerOneway(Ice::Int wikiid);
};

}}

#endif  //__SOCIALWIKIHOT_BYWIKKIID__REPLICAADAPTER_H__

