#include "FeedDbWriterAdapter.h"

using namespace xce::feed;
using namespace MyUtil;
void FeedDbWriterAdapter::FeedDispatch2DB(const FeedSeedPtr& seed, const FeedReplyPtr& reply)
{
	getManagerOneway(seed->actor)->FeedDispatch2DB(seed,reply);
}
void FeedDbWriterAdapter::FeedDispatchEDM2DB(const FeedSeedPtr& seed)
{
	getManagerOneway(seed->actor)->FeedDispatchEDM2DB(seed);
}
void FeedDbWriterAdapter::AddFeedSchool2DB(int schoolid,const FeedSeedPtr & seed) {
  getManagerOneway(seed->actor)->AddFeedSchool2DB(schoolid,seed);
}
void FeedDbWriterAdapter::UpdateReply2DB(const MyUtil::LongSeq & feeds, const FeedReplyPtr & reply ) {
  if (feeds.empty()) 
    return;
  getManagerOneway(feeds[0]%cluster())->UpdateReply2DB(feeds,reply);
}
void FeedDbWriterAdapter::RemoveFeedFromDB(const FeedIndexSeq indexSeq) {
  if(indexSeq.empty())
    return;
  FeedIndexSeq::const_iterator it = indexSeq.begin();
  getManagerOneway((*it)->source)->RemoveFeedFromDB(indexSeq); 
}
void FeedDbWriterAdapter::ReplaceFeedXML2DB(const MyUtil::LongSeq & feedIds,const string & xml){
  if(feedIds.empty())
    return;
  getManagerOneway(feedIds[0]%cluster())->ReplaceFeedXML2DB(feedIds,xml);
}

FeedDbWriterPrx FeedDbWriterAdapter::getManager(Ice::Long id) {
	return locate<FeedDbWriterPrx> (_managers, "FDBW", id, TWO_WAY);
}

FeedDbWriterPrx FeedDbWriterAdapter::getManagerOneway(Ice::Long id) {
	return locate<FeedDbWriterPrx> (_managersOneway, "FDBW", id, ONE_WAY);
}
