#include "FeedInterfaceReplicaAdapter.h"

using namespace xce::feed;
using namespace xce::clusterstate;


FeedItemSeq FeedInterfaceReplicaAdapter::GetOwnFeeds(const MyUtil::IntSeq & userids, int begin, int limit, const MyUtil::IntSeq & types) {
  MyUtil::IntSeq userid_batch;
  FeedItemSeq result, temp;
  result.reserve(userids.size() * limit);
  
  for(MyUtil::IntSeq::const_iterator it = userids.begin(); it != userids.end();) {
    userid_batch.push_back(*it++);
    if (userid_batch.size() >= 200 || it == userids.end()) {
      try {
        temp = getProxy( *userid_batch.begin() )->GetOwnFeeds(userid_batch, begin, limit, types);
        result.insert(result.end(), temp.begin(), temp.end());
        userid_batch.clear();
      } catch (Ice::Exception & e) {
        userid_batch.clear();
        //std::cout << "FeedInterfaceReplicaAdapter::GetOwnFeeds. --> warn::" << e << endl;
        MCE_WARN("FeedInterfaceReplicaAdapter::GetOwnFeeds. --> warn:" << e);
      } 
    }
  }
//  result = getManagerTwoway()->GetOwnFeeds(userids, begin, limit, types);
  return result;
}

FeedContentDict FeedInterfaceReplicaAdapter::GetFeedDict(const MyUtil::LongSeq & feedids) {
  MyUtil::LongSeq feedid_batch;
  FeedContentDict result, temp;

  for(MyUtil::LongSeq::const_iterator it = feedids.begin(); it != feedids.end();) {
    feedid_batch.push_back(*it++);
    if (feedid_batch.size() >= 100 || it == feedids.end()) {
      try {
        temp = getProxy( *feedid_batch.begin() )->GetFeedDict(feedid_batch);
        result.insert(temp.begin(), temp.end());
        feedid_batch.clear();
      } catch (Ice::Exception & e) {
        feedid_batch.clear();
        //std::cout << "FeedInterfaceReplicaAdapter::GetFeedDict. --> warn::" << e << endl;
        //MCE_WARN("FeedInterfaceReplicaAdapter::GetFeedDict. --> warn:" << e);
      }
    }
  }
  return result;

}

void  FeedInterfaceReplicaAdapter::addFriendFeedToGuide(int userid,const MyUtil::LongSeq & feedids) {
  getManagerOneway(userid)->addFriendFeedToGuide(userid,feedids);
}

FeedInterfacePrx FeedInterfaceReplicaAdapter::getManager(Ice::Long id) {
    return getProxy(id);
}

FeedInterfacePrx FeedInterfaceReplicaAdapter::getManagerOneway(Ice::Long id) {
    return getProxyOneway(id);
}  
