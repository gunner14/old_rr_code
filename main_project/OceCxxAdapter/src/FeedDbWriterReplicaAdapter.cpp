#include "FeedDbWriterReplicaAdapter.h"

using namespace xce::feed;
using namespace MyUtil;
void FeedDbWriterReplicaAdapter::FeedDispatch2DB(const FeedSeedPtr& seed, const FeedReplyPtr& reply)
{
	getManagerOneway(seed->actor)->FeedDispatch2DB(seed,reply);
}
void FeedDbWriterReplicaAdapter::FeedDispatchEDM2DB(const FeedSeedPtr& seed)
{
	getManagerOneway(seed->actor)->FeedDispatchEDM2DB(seed);
}
void FeedDbWriterReplicaAdapter::AddFeedSchool2DB(int schoolid,const FeedSeedPtr & seed) {
  getManagerOneway(seed->actor)->AddFeedSchool2DB(schoolid,seed);
}
void FeedDbWriterReplicaAdapter::UpdateReply2DB(const MyUtil::LongSeq & feeds, const FeedReplyPtr & reply ) {
  if (feeds.empty()) 
    return;
  getManagerOneway(feeds[0])->UpdateReply2DB(feeds,reply);
}
void FeedDbWriterReplicaAdapter::RemoveFeedFromDB(const FeedIndexSeq indexSeq) {
  if(indexSeq.empty())
    return;
  FeedIndexSeq::const_iterator it = indexSeq.begin();
  getManagerOneway((*it)->actor)->RemoveFeedFromDB(indexSeq); 
}
void FeedDbWriterReplicaAdapter::ReplaceFeedXML2DB(const MyUtil::LongSeq & feedIds,const string & xml){
  if(feedIds.empty())
    return;
  getManagerOneway(feedIds[0])->ReplaceFeedXML2DB(feedIds,xml);
}

FeedDbWriterPrx FeedDbWriterReplicaAdapter::getManager(Ice::Long id) {
	//return locate<FeedDbWriterPrx> (_managers, "FDBW", id, TWO_WAY);
  return getProxy(id);
}

FeedDbWriterPrx FeedDbWriterReplicaAdapter::getManagerOneway(Ice::Long id) {
	//return locate<FeedDbWriterPrx> (_managersOneway, "FDBW", id, ONE_WAY);
  return getProxyOneway(id);
}
