#include "FeedMarkAdapter.h"

using namespace MyUtil;
using namespace xce::feed;


void FeedMarkAdapter::MarkFeed(int uid, long fid, long source, int stype, int actor, bool mark) {
   GetManagerOneWay(uid)->MarkFeed(uid, fid, source, stype, actor, mark);
}

MarkInfoPtr FeedMarkAdapter::GetMarkInfo(int uid) {
  return GetManager(uid)->GetMarkInfo(uid);
}









