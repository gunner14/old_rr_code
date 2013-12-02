#ifndef _TimeLineFeedAdapter_h_
#define _TimeLineFeedAdapter_h_

#include "TimeLineFeed.h"
#include "Singleton.h"
#include "AdapterI.h"
#include "ReplicaCluster/ClusterState/src/client/cxx/ClientInterface.h"
#include "Channel.h"

namespace xce {
namespace feed {
namespace timeline {
class TimeLineFeedReplicaAdapter : public MyUtil::ReplicatedClusterAdapterI<TimeLineFeedPrx>
                                   ,public MyUtil::Singleton<TimeLineFeedReplicaAdapter> {
  public:
    TimeLineFeedReplicaAdapter():
      MyUtil::ReplicatedClusterAdapterI<TimeLineFeedPrx> ("ControllerTimeLineFeed", 120, 300, new MyUtil::XceFeedControllerChannel()) { 
    }
    UidYear Query(int uid);
    TimeLineFeedResult QueryFeedInfo(int uid,int year,int month,int begin,int limit);
    TimeLineFeedResult QueryFeedInfoByStype(int uid,int year,int month,const MyUtil::IntSeq& types,int begin,int limit);
    int QueryFeedSize(int uid,int year,int month);
    int QueryFeedSizeByStype(int uid,int year,int month,const MyUtil::IntSeq& types);
    bool IsWideScreen(int uid,int year, ::Ice::Long feed);
    bool SetWideScreen(int uid,int year, ::Ice::Long feed, bool wide);
    void AddFeedMiniNotify(const FeedSeedPtr &,int type,::Ice::Long time);
    void Modify(int uid,int old_year, Ice::Long feed, int year,int month,int day);
    void SetReadMini(int uid,int year, int stype, ::Ice::Long merge);
    void SetReadMiniByIds(int uid,int year, const MyUtil::LongSeq & feeds);
    void SetReadMiniById(int uid,int year, ::Ice::Long feed);
    void LoadFeedsToCache(int uid);
    std::string Execute(int uid, const std::string & cmd);

};
}
}
}

#endif
