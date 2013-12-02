#include "FeedDispatchNewReplicaAdapter.h"

using namespace xce::feed;
using namespace MyUtil;

void FeedDispatchNewReplicaAdapter::AddBuddyNotify(int host, int guest){
	getManagerOneway(host)->AddBuddyNotify(host,guest);
}
        
void FeedDispatchNewReplicaAdapter::BuddyApplyNotify(int host, int guest){
	getManagerOneway(guest)->BuddyApplyNotify(host, guest);
}

void FeedDispatchNewReplicaAdapter::JoinPageNotify(int page, int fans){
	getManagerOneway(page)->JoinPageNotify(page,fans);
}
void FeedDispatchNewReplicaAdapter::GuideBuddyReplyNotify(int newUser,int buddy) {
  getManagerOneway(buddy)->GuideBuddyReplyNotify(newUser,buddy);
}
void FeedDispatchNewReplicaAdapter::AddMiniSiteFeed(int siteid, int userid){
	getManagerOneway(userid)->AddMiniSiteFeed(siteid, userid);
}
void FeedDispatchNewReplicaAdapter::AddMiniSiteFeedToGuides(long source,int type,int siteid, const vector<int> &guides)
{
  map<int,vector<int> > vmap;
  for (int i=0; i<guides.size(); ++i) {
    vmap[guides[i]%getCluster()].push_back(guides[i]);
  }
  for (map<int,vector<int> >::iterator it = vmap.begin(); it != vmap.end(); ++it) {
    getManagerOneway(it->first)->AddMiniSiteFeedToGuides(source,type,siteid,it->second);
  }  
}
void FeedDispatchNewReplicaAdapter::Reload(LoadType type, int id){
  for(unsigned  i = 0; i != getCluster(); ++i){
    getProxyOneway(i)->Reload(type,id);
  }
}
void FeedDispatchNewReplicaAdapter::Reload2(LoadType type, long id){
  for(unsigned  i = 0; i != getCluster(); ++i){
    getProxyOneway(i)->Reload2(type,id);
  }
}
void FeedDispatchNewReplicaAdapter::ReloadBatch(LoadType type, const vector<int> & ids){
  for(unsigned  i = 0; i != getCluster(); ++i){
    getProxyOneway(i)->ReloadBatch(type,ids);
  }
}
void FeedDispatchNewReplicaAdapter::RemoveFeedByTime(Ice::Long source, Ice::Int stype, Ice::Int actor, Ice::Int time) {
	getManagerOneway(source)->RemoveFeedByTime(source, stype, actor, time);
}

void FeedDispatchNewReplicaAdapter::RemoveFeedById(Ice::Long feedId)
{
	getManagerOneway(feedId)->RemoveFeedById(feedId);
}

void FeedDispatchNewReplicaAdapter::RemoveFeedBySource(Ice::Long source, int stype)
{
	getManagerOneway(source)->RemoveFeedBySource(source, stype);
}

void FeedDispatchNewReplicaAdapter::RemoveFeedBySourceHS(Ice::Long source, int stype)
{
	getManagerOneway(source)->RemoveFeedBySourceHS(source, stype);
}

void FeedDispatchNewReplicaAdapter::RemoveFeedByParent(Ice::Long parent, int ptype)
{
	getManagerOneway(parent)->RemoveFeedByParent(parent, ptype);
}

void FeedDispatchNewReplicaAdapter::RemoveFeed(Ice::Long source, int stype, int actor)
{
	getManagerOneway(source)->RemoveFeed(source, stype, actor);
}


void FeedDispatchNewReplicaAdapter::RemoveFeedReply(Ice::Long source, int stype, int actor, Ice::Long removeReplyId, const FeedReplyPtr& reply)
{
  if (!reply) {
    return;
  }
	getManagerOneway(source)->RemoveFeedReply(source, stype, actor, removeReplyId, reply);
}

void FeedDispatchNewReplicaAdapter::RemoveFeedReplyById(int stype, Ice::Long feedid, Ice::Long removeReplyId, const FeedReplyPtr& reply)
{
  if (!reply) {
    return;
  }
	getManagerOneway(feedid)->RemoveFeedReplyById(stype, feedid, removeReplyId, reply);
}

void FeedDispatchNewReplicaAdapter::Dispatch(const FeedSeedPtr& seed)
{
  if (!seed) {
    return;
  }
	getManagerOneway(seed->source)->Dispatch(seed);
}

void FeedDispatchNewReplicaAdapter::DispatchEDM(const FeedSeedPtr& seed)
{
  if (!seed) {
    return;
  }
	getManagerOneway(seed->actor)->DispatchEDM(seed);
}

void FeedDispatchNewReplicaAdapter::DispatchWithReply(const FeedSeedPtr& seed, const FeedReplyPtr& reply){
  if (!seed ) {
    return;
  }
	getManagerOneway(seed->source)->DispatchWithReply(seed,reply);
}


void FeedDispatchNewReplicaAdapter::AddFeedReply(Ice::Long source, int stype, int actor, const FeedReplyPtr & reply)
{
  if (!reply) {
    return;
  }
	getManagerOneway(source)->AddFeedReply(source, stype, actor,  reply);
}

void FeedDispatchNewReplicaAdapter::AddFeedReplyById(int stype, Ice::Long feedid, const FeedReplyPtr & reply)
{
  if (!reply) {
    return;
  }
	getManagerOneway(feedid)->AddFeedReplyById(stype, feedid,  reply);
}
void FeedDispatchNewReplicaAdapter::AddFeedReplyWithConfig(Ice::Long source, int stype, int actor, const FeedReplyPtr & reply,
    const map<string,string> & reply_config)
{
  if (!reply) {
    return;
  }
  getManagerOneway(source)->AddFeedReplyWithConfig(source, stype, actor,  reply,reply_config);
}

void FeedDispatchNewReplicaAdapter::CacheSync(Ice::Long source, int stype, int actor){
	getManagerOneway(source)->CacheSync(source,stype,actor);
}

Ice::Long FeedDispatchNewReplicaAdapter::GetFeedId(Ice::Long source, int stype, int actor){
  return getManager(source)->GetFeedId(source, stype, actor);
}

void FeedDispatchNewReplicaAdapter::DispatchAgain(Ice::Long source, int stype, int actor, int toid){
  getManagerOneway(source)->DispatchAgain(source,stype,actor,toid);
}
void FeedDispatchNewReplicaAdapter::ReplaceXML(Ice::Long source, int stype, int actor,const string & xml ){
	getManagerOneway(source)->ReplaceXML(source,stype,actor,xml);
}
void FeedDispatchNewReplicaAdapter::DirectedSend(Ice::Long source, int stype, int actor,const vector<int> & targets){
  getManagerOneway(source)->DirectedSend(source,stype,actor,targets);
}

FeedDispatchNewPrx FeedDispatchNewReplicaAdapter::getManager(Ice::Long id) {
	//return locate<FeedDispatchNewPrx> (_managers, "FDNW", id, TWO_WAY);
  return getProxy(id);
}

FeedDispatchNewPrx FeedDispatchNewReplicaAdapter::getManagerOneway(Ice::Long id) {
	//return locate<FeedDispatchNewPrx> (_managersOneway, "FDNW", id, ONE_WAY);
  id = id / 10;
  return getProxyOneway(id);
}
