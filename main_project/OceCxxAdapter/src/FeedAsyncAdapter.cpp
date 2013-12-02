#include "FeedAsyncAdapter.h"

using namespace xce::feed;

FeedAsyncAdapter::FeedAsyncAdapter() {
  _managers.resize(cluster() > 0 ? cluster() : 1);
  _managersOneway.resize(cluster() > 0 ? cluster() : 1);
}

FeedAsyncAdapter::~FeedAsyncAdapter() {

}

FeedAsyncPrx FeedAsyncAdapter::getManager(int id) {
  return locate<FeedAsyncPrx> (_managers, "M", id, TWO_WAY);
}

FeedAsyncPrx FeedAsyncAdapter::getManagerOneway(int id) {
  return locate<FeedAsyncPrx> (_managersOneway, "M", id, ONE_WAY);
}

bool FeedAsyncAdapter::AddFeedIndex(int index,const FeedIndexPtr & idx){
  return getManager(index)->AddFeedIndex(idx);
}
bool FeedAsyncAdapter::RemoveFeedIndex(int index,const MyUtil::LongSeq& feeds){
  return getManager(index)->RemoveFeedIndex(feeds);
}
bool FeedAsyncAdapter::AddFeedContent(int index,const FeedSeedPtr & seed){
  return getManager(index)->AddFeedContent(seed);
}
bool FeedAsyncAdapter::RemoveFeedContent(int index,const MyUtil::LongSeq& feeds){
  return getManager(index)->RemoveFeedContent(feeds);
}

bool FeedAsyncAdapter::AddFeedMini(int index,const FeedSeedPtr& seed){
  return getManager(index)->AddFeedMini(seed);
}
bool FeedAsyncAdapter::RemoveFeedMini(int index,int user, int stype, Ice::Long miniMerge){
  return getManager(index)->RemoveFeedMini(user,stype,miniMerge);
}
bool FeedAsyncAdapter::RemoveAllFeedMini(int index,int user){
  return getManager(index)->RemoveAllFeedMini(user);
}

bool FeedAsyncAdapter::AddFeedSchool(int index,int schoolid, const FeedSeedPtr& seed){
  return getManager(index)->AddFeedSchool(schoolid,seed);
}

bool FeedAsyncAdapter::AddFeedGroup(int index,int groupid, const FeedSeedPtr& seed){
  return getManager(index)->AddFeedGroup(groupid,seed);
}

bool FeedAsyncAdapter::ReplaceXML(int index,const  MyUtil::LongSeq & fids , const string & xml ){
  return getManager(index)->ReplaceXML(fids,xml);

}
bool FeedAsyncAdapter::UpdateReply(int index,const map<Ice::Long, FeedReplyPtr>& buffer){
  return getManager(index)->UpdateReply(buffer);
}
bool FeedAsyncAdapter::CopyFeedContent(int index,Ice::Long old_feed_id, Ice::Long new_feed_id, int new_type, const std::string& new_props){
  return getManager(index)->CopyFeedContent(old_feed_id,new_feed_id,new_type,new_props);

}

//void FeedAsyncAdapter::TestPush(int index,int index, const SqlInfoPtr & sqlinfo) {
//
//  MCE_INFO("FeedAsyncAdapter::TestPush. index:" << index);
//  FeedQueuePrx::uncheckedCast(getManagerOneway(index)->ice_datagram())->TestPush(
//      sqlinfo);
//}
