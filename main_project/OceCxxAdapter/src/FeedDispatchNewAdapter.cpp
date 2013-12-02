#include "FeedDispatchNewAdapter.h"

using namespace xce::feed;
using namespace MyUtil;

void FeedDispatchNewAdapter::AddBuddyNotify(int host, int guest){
	getManagerOneway(host)->AddBuddyNotify(host,guest);
}
        
void FeedDispatchNewAdapter::BuddyApplyNotify(int host, int guest){
	getManagerOneway(guest)->BuddyApplyNotify(host, guest);
}

void FeedDispatchNewAdapter::JoinPageNotify(int page, int fans){
	getManagerOneway(page)->JoinPageNotify(page,fans);
}
void FeedDispatchNewAdapter::GuideBuddyReplyNotify(int newUser,int buddy) {
  getManagerOneway(buddy)->GuideBuddyReplyNotify(newUser,buddy);
}
void FeedDispatchNewAdapter::AddMiniSiteFeed(int siteid, int userid){
	getManagerOneway(userid)->AddMiniSiteFeed(siteid, userid);
}
void FeedDispatchNewAdapter::AddMiniSiteFeedToGuides(long source,int type,int siteid, const vector<int> &guides)
{
  map<int,vector<int> > vmap;
  for (int i=0; i<guides.size(); ++i) {
    vmap[guides[i]%cluster()].push_back(guides[i]);
  }
  for (map<int,vector<int> >::iterator it = vmap.begin(); it != vmap.end(); ++it) {
    getManagerOneway(it->first)->AddMiniSiteFeedToGuides(source,type,siteid,it->second);
  }  
}
void FeedDispatchNewAdapter::Reload(LoadType type, int id){
  for(unsigned  i = 0; i != cluster(); ++i){
    getManagerOneway(i)->Reload(type,id);
  }
}
void FeedDispatchNewAdapter::Reload2(LoadType type, long id){
  for(unsigned  i = 0; i != cluster(); ++i){
    getManagerOneway(i)->Reload2(type,id);
  }
}
void FeedDispatchNewAdapter::ReloadBatch(LoadType type, const vector<int> & ids){
  for(unsigned  i = 0; i != cluster(); ++i){
    getManagerOneway(i)->ReloadBatch(type,ids);
  }
}
void FeedDispatchNewAdapter::RemoveFeedByTime(Ice::Long source, Ice::Int stype, Ice::Int actor, Ice::Int time) {
	getManagerOneway(source)->RemoveFeedByTime(source, stype, actor, time);
}

void FeedDispatchNewAdapter::RemoveFeedById(Ice::Long feedId)
{
	getManagerOneway(feedId)->RemoveFeedById(feedId);
}

void FeedDispatchNewAdapter::RemoveFeedBySource(Ice::Long source, int stype)
{
	getManagerOneway(source)->RemoveFeedBySource(source, stype);
}

void FeedDispatchNewAdapter::RemoveFeedBySourceHS(Ice::Long source, int stype)
{
	getManagerOneway(source)->RemoveFeedBySourceHS(source, stype);
}

void FeedDispatchNewAdapter::RemoveFeedByParent(Ice::Long parent, int ptype)
{
	getManagerOneway(parent)->RemoveFeedByParent(parent, ptype);
}

void FeedDispatchNewAdapter::RemoveFeed(Ice::Long source, int stype, int actor)
{
	getManagerOneway(source)->RemoveFeed(source, stype, actor);
}


void FeedDispatchNewAdapter::RemoveFeedReply(Ice::Long source, int stype, int actor, Ice::Long removeReplyId, const FeedReplyPtr& reply)
{
	getManagerOneway(source)->RemoveFeedReply(source, stype, actor, removeReplyId, reply);
}

void FeedDispatchNewAdapter::RemoveFeedReplyById(int stype, Ice::Long feedid, Ice::Long removeReplyId, const FeedReplyPtr& reply)
{
	getManagerOneway(feedid)->RemoveFeedReplyById(stype, feedid, removeReplyId, reply);
}

void FeedDispatchNewAdapter::Dispatch(const FeedSeedPtr& seed)
{
	//getManagerOneway(seed->type & 0x00ffffff)->dispatch(seed);

	getManagerOneway(seed->source)->Dispatch(seed);
}

void FeedDispatchNewAdapter::DispatchEDM(const FeedSeedPtr& seed)
{
	//getManagerOneway(seed->type & 0x00ffffff)->dispatchEDM(seed);
	getManagerOneway(seed->source)->DispatchEDM(seed);
}

void FeedDispatchNewAdapter::DispatchWithReply(const FeedSeedPtr& seed, const FeedReplyPtr& reply){
	//getManagerOneway(seed->type & 0x00ffffff)->dispatchWithReply(seed,reply);
	getManagerOneway(seed->source)->DispatchWithReply(seed,reply);
}


void FeedDispatchNewAdapter::AddFeedReply(Ice::Long source, int stype, int actor, const FeedReplyPtr & reply)
{
	getManagerOneway(source)->AddFeedReply(source, stype, actor,  reply);
}

void FeedDispatchNewAdapter::AddFeedReplyById(int stype, Ice::Long feedid, const FeedReplyPtr & reply)
{
	getManagerOneway(feedid)->AddFeedReplyById(stype, feedid,  reply);
}
void FeedDispatchNewAdapter::AddFeedReplyWithConfig(Ice::Long source, int stype, int actor, const FeedReplyPtr & reply,
    const map<string,string> & reply_config)
{
  getManagerOneway(source)->AddFeedReplyWithConfig(source, stype, actor,  reply,reply_config);
}

void FeedDispatchNewAdapter::CacheSync(Ice::Long source, int stype, int actor){
	getManagerOneway(source)->CacheSync(source,stype,actor);
}

Ice::Long FeedDispatchNewAdapter::GetFeedId(Ice::Long source, int stype, int actor){
  return getManager(source)->GetFeedId(source, stype, actor);
}

void FeedDispatchNewAdapter::DispatchAgain(Ice::Long source, int stype, int actor, int toid){
  getManagerOneway(source)->DispatchAgain(source,stype,actor,toid);
}
void FeedDispatchNewAdapter::ReplaceXML(Ice::Long source, int stype, int actor,const string & xml ){
	getManagerOneway(source)->ReplaceXML(source,stype,actor,xml);
}
void FeedDispatchNewAdapter::DirectedSend(Ice::Long source, int stype, int actor,const vector<int> & targets){
  getManagerOneway(source)->DirectedSend(source,stype,actor,targets);
}

FeedDispatchNewPrx FeedDispatchNewAdapter::getManager(Ice::Long id) {
	return locate<FeedDispatchNewPrx> (_managers, "FDNW", id, TWO_WAY);
}

FeedDispatchNewPrx FeedDispatchNewAdapter::getManagerOneway(Ice::Long id) {
  id = id / 10;
	return locate<FeedDispatchNewPrx> (_managersOneway, "FDNW", id, ONE_WAY);
}
