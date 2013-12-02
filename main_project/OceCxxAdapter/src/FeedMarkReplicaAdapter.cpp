#include "FeedMarkReplicaAdapter.h"

using namespace MyUtil;
using namespace xce::feed;
using namespace xce::clusterstate;

void FeedMarkReplicaAdapter::MarkFeed(int uid, long fid, long source, int stype, int actor, bool mark) {
  try {
    getProxy(uid)->MarkFeed(uid, fid, source, stype, actor, mark);
  } catch (Ice::Exception & e) {
    MCE_WARN("FeedMarkReplicaAdapter:: MarkFeed wrong" << "uid " << uid 
            << " fid "<<fid<<" source "<<source<<" stype " <<stype
            << " actor "<<actor<<" mark "<<mark << " err " << e);
  }
  
}

MarkInfoPtr FeedMarkReplicaAdapter::GetMarkInfo(int uid) {
  try {
    return getProxy(uid)->GetMarkInfo(uid);
  } catch (Ice::Exception & e) {
    MCE_WARN("FeedMarkReplicaAdapter:: GetMarkInfo wrong --> " <<"uid " << uid<<" err "<<e);
    return new MarkInfo();
  }
}









