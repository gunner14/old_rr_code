#include "FeedDispatcherAdapter.h"

using namespace xce::feed;
using namespace MyUtil;

/*void
FeedDispatcherAdapter::deliverSupply(int supplier, int accepter, const Ice::Context& ctx){
	_managerOneway->deliverSupply(supplier, accepter, ctx);
}
*/

void FeedDispatcherAdapter::dispatch(const FeedSeedPtr& seed, const FeedConfigPtr& config)
{
	getManager(seed->actor)->dispatch(seed,config);
}


void FeedDispatcherAdapter::dispatchReply( const ReplyDataPtr& replyData,const FeedConfigPtr& config){
	getManager(replyData->actor)->dispatchReply(replyData,config);
}


void FeedDispatcherAdapter::addBuddyNotify(int host, int guest){
	getManagerOneway(host)->addBuddyNotify(host,guest);
}
        
void FeedDispatcherAdapter::changeBuddyNotify(int host, int guest){
	getManagerOneway(host)->Reload(Friend, host);
	getManagerOneway(guest)->Reload(Friend, guest);
}

void FeedDispatcherAdapter::buddyApplyNotify(int host, int guest){
	getManagerOneway(guest)->buddyApplyNotify(host, guest);
}

void FeedDispatcherAdapter::joinPageNotify(int page, int fans){
	getManagerOneway(page)->joinPageNotify(page,fans);
}

void FeedDispatcherAdapter::addMiniSiteFeed(int siteid, int userid){
	getManagerOneway(userid)->addMiniSiteFeed(siteid, userid);
}
void FeedDispatcherAdapter::addMiniSiteFeedToGuides(long source,int type,int siteid, const vector<int> &guides)
{
  map<int,vector<int> > vmap;
  for (int i=0; i<guides.size(); ++i) {
    vmap[guides[i]%cluster()].push_back(guides[i]);
  }
  for (map<int,vector<int> >::iterator it = vmap.begin(); it != vmap.end(); ++it) {
    getManagerOneway(it->first)->addMiniSiteFeedToGuides(source,type,siteid,it->second);
  }  
}
void FeedDispatcherAdapter::Reload(LoadType type, int id){
  for(unsigned  i = 0; i != cluster(); ++i){
    getManagerOneway(i)->Reload(type,id);
  }
}
void FeedDispatcherAdapter::Reload2(LoadType type, long id){
  for(unsigned  i = 0; i != cluster(); ++i){
    getManagerOneway(i)->Reload2(type,id);
  }
}
void FeedDispatcherAdapter::ReloadBatch(LoadType type, const vector<int> & ids){
  for(unsigned  i = 0; i != cluster(); ++i){
    getManagerOneway(i)->ReloadBatch(type,ids);
  }
}

FeedDispatcherPrx FeedDispatcherAdapter::getManager(int id) {
	return locate<FeedDispatcherPrx> (_managers, "FD", id, TWO_WAY);
}

FeedDispatcherPrx FeedDispatcherAdapter::getManagerOneway(int id) {
	return locate<FeedDispatcherPrx> (_managersOneway, "FD", id, ONE_WAY);
}
