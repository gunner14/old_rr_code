#include "FeedInterfaceAdapter.h"

using namespace xce::feed;


FeedInterfaceAdapter::FeedInterfaceAdapter()
{
	_managers.resize(cluster() > 0 ? cluster() : 1);
	_managersOneway.resize(cluster() > 0 ? cluster() : 1);
	_managersTwoway.resize(cluster() > 0 ? cluster() : 1);
}

FeedInterfacePrx FeedInterfaceAdapter::getManager()
{
	return locate<FeedInterfacePrx> (_managers, "M", 0, DATAGRAM);
}

//FeedInterfacePrx FeedInterfaceAdapter::getManager()
//{
//	return locate<FeedInterfacePrx> (_managersOneway, "M", 0, ONE_WAY);
//}

FeedInterfacePrx FeedInterfaceAdapter::getManagerTwoway()
{
	return locate<FeedInterfacePrx> (_managersTwoway, "M", 0, TWO_WAY);
}



void FeedInterfaceAdapter::removeFeedById(Ice::Long feedId)
{
	getManager()->removeFeedById(feedId);
}

void FeedInterfaceAdapter::removeFeedBySource(Ice::Long source, int stype)
{
	getManager()->removeFeedBySource(source, stype);
}

void FeedInterfaceAdapter::removeFeedBySourceHS(Ice::Long source, int stype)
{
	getManager()->removeFeedBySourceHS(source, stype);
}

void FeedInterfaceAdapter::removeFeedByParent(Ice::Long parent, int ptype)
{
	getManager()->removeFeedByParent(parent, ptype);
}

void FeedInterfaceAdapter::removeFeed(Ice::Long source, int stype, int actor)
{
	getManager()->removeFeed(source, stype, actor);
}


void FeedInterfaceAdapter::removeFeedReply(Ice::Long source, int stype, int actor, Ice::Long removeReplyId, const FeedReplyPtr& reply)
{

	getManager()->removeFeedReply(source, stype, actor, removeReplyId, reply);

}

void FeedInterfaceAdapter::readNews(int user,Ice::Long feed)
{

	getManager()->readNews(user,feed);

}

void FeedInterfaceAdapter::readAllNews(int user)
{
	getManager()->readAllNews(user);
}

void FeedInterfaceAdapter::dispatch(const FeedSeedPtr& seed)
{
	getManager()->dispatch(seed);
}

void FeedInterfaceAdapter::dispatchWithReply(const FeedSeedPtr& seed, const FeedReplyPtr& reply){
	getManager()->dispatchWithReply(seed,reply);
}

void FeedInterfaceAdapter::dispatchEDM(const FeedSeedPtr& seed)
{
	getManager()->dispatchEDM(seed);
}
//Ice::Long FeedInterfaceAdapter::createFeedId(){
//	return getManager()->createFeedId();
//}

void FeedInterfaceAdapter::addFeedReply(Ice::Long source, int stype, int actor, const FeedReplyPtr & reply)
{
	getManager()->addFeedReply(source, stype, actor,  reply);
}

void FeedInterfaceAdapter::MarkFeed(int uid, Ice::Long fid, Ice::Long source, int stype, int actor, bool mark) 
{
  getManager()->MarkFeed(uid, fid, source, stype, actor, mark);
}


void FeedInterfaceAdapter::addFeedReplyById(int stype, Ice::Long feedid,const FeedReplyPtr& reply) {
  getManager()->addFeedReplyById(stype, feedid, reply);
}

void FeedInterfaceAdapter::addFeedReplyWithConfig(Ice::Long source, Ice::Int stype, Ice::Int actor, const FeedReplyPtr& reply, const map<string, string> & config) {
  getManager()->addFeedReplyWithConfig(source, stype, actor, reply, config);
}
void FeedInterfaceAdapter::dispatchAgain(Ice::Long source, int stype, int actor, int toid) {
  getManager()->dispatchAgain(source, stype, actor, toid);
}
void FeedInterfaceAdapter::ReplaceXML(Ice::Long source, int stype, int actor, const string & xml) {
  getManager()->ReplaceXML(source, stype, actor, xml);
}
void FeedInterfaceAdapter::DirectedSend(Ice::Long source, int stype, int actor, const vector<int> & targets) {
  getManager()->DirectedSend(source, stype, actor, targets);
}

void FeedInterfaceAdapter::readMini(int user, Ice::Long feedid) {
  getManager()->readMini(user, feedid);
}
void FeedInterfaceAdapter::readAllMini(int user) {
  getManager()->readAllMini(user);
}
void FeedInterfaceAdapter::removeFeedReplyById(int stype, Ice::Long feedid, Ice::Long removeReplyId, const FeedReplyPtr& reply) {
  getManager()->removeFeedReplyById(stype, feedid, removeReplyId, reply);
}

FeedItemSeq FeedInterfaceAdapter::GetOwnFeeds(const IntSeq & userids, int begin, int limit, const IntSeq & types) {
  IntSeq userid_batch;
  FeedItemSeq result, temp;
  result.reserve(userids.size() * limit);
  
  for(IntSeq::const_iterator it = userids.begin(); it != userids.end();) {
    userid_batch.push_back(*it++);
    if (userid_batch.size() >= 200 || it == userids.end()) {
      try {
        temp = getManagerTwoway()->GetOwnFeeds(userid_batch, begin, limit, types);
        result.insert(result.end(), temp.begin(), temp.end());
        userid_batch.clear();
      } catch (Ice::Exception & e) {
        userid_batch.clear();
        MCE_WARN("FeedInterfaceAdapter::GetOwnFeeds. --> warn:" << e);
      } 
    }
  }
//  result = getManagerTwoway()->GetOwnFeeds(userids, begin, limit, types);
  return result;
}

FeedContentDict FeedInterfaceAdapter::GetFeedDict(const LongSeq & feedids) {
  LongSeq feedid_batch;
  FeedContentDict result, temp;

  for(LongSeq::const_iterator it = feedids.begin(); it != feedids.end();) {
    feedid_batch.push_back(*it++);
    if (feedid_batch.size() >= 250 || it == feedids.end()) {
      try {
        temp = getManagerTwoway()->GetFeedDict(feedid_batch);
        result.insert(temp.begin(), temp.end());
        feedid_batch.clear();
      } catch (Ice::Exception & e) {
        feedid_batch.clear();
        MCE_WARN("FeedInterfaceAdapter::GetFeedDict. --> warn:" << e);
      }
    }
  }
  return result;


}


