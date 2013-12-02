#ifndef WEBTRANSITSADAPTER_H_
#define WEBTRANSITSADAPTER_H_


#include "AdapterI.h"
#include "Singleton.h"
#include "WTransit.h"
#include "RFeed.h"

namespace talk{
namespace adapter {

using namespace talk::wtransit;
using namespace xce::feed;
using namespace MyUtil;
using namespace com::xiaonei::talk::common;

//class WTransitAdapter : public AdapterI,
//	public AdapterISingleton<MyUtil::TalkChannel, WTransitAdapter> {
class WTransitAdapter : 
 public MyUtil::ReplicatedClusterAdapterI<WTransitManagerPrx>, public MyUtil::Singleton<WTransitAdapter>  {

public:
   //	WTransitAdapter() {
	WTransitAdapter() : 
   MyUtil::ReplicatedClusterAdapterI<WTransitManagerPrx> ("ControllerWTransit",120,300,new XceFeedControllerChannel) {
     //		_managersOneway.resize(cluster() > 0 ? cluster() : 1);
	}
	//void deliverNotify(Ice::Int userid,const string& title, const string& msg);
	void deliverFeed(const FeedSeedPtr& feed, const MyUtil::IntSeq& target);
	void deliverFeedWithFocusfriend(const FeedSeedPtr& feed, const MyUtil::IntSeq& target, const MyUtil::IntSeq& focusfrinedSeq);
	void deliverFeedReply( const ReplyDataPtr& replyData, const MyUtil::IntSeq& target);
	void deliverImoldFeed(const JidPtr& jid, int id, Ice::Long source, int actor, int shareType, int notifyType, int feedType);
	void loadOfflineFeed(const JidPtr& jid, Ice::Long feedIndex, Ice::Long replyIndex, int iqId);
	//void deliverReply(const mop::hi::svc::dispatch::NotePtr& note);
	void deliverNotifyWebpager(const MyUtil::IntSeq& toids, const string& content);
	void deliverNotifyClient(const MyUtil::IntSeq& toids, const string& content);
	void deliverNotifyPhone(const MyUtil::IntSeq& toids, const string& content);


	void deliverGroupNotifyPager(const MyUtil::IntSeq& groupids, const MyUtil::IntSeq& touids, const MyUtil::IntSeq& blockuids, const string& content);
	void deliverGroupNotifyClient(const MyUtil::IntSeq& groupids, const MyUtil::IntSeq& touids, const MyUtil::IntSeq& blockuids, const string& content);
	void deliverGroupNotifyPhone(const MyUtil::IntSeq& groupids, const MyUtil::IntSeq& touids, const MyUtil::IntSeq& blockuids, const string& content);

	void CallFeedCache(const JidPtr& jid, long feedIndex, long replyIndex, int iqId);
	void CallFeedCacheAndContent(const JidPtr& jid, long feedIndex, long replyIndex, int iqId);
	void CallNotifyGate(const JidPtr& jid, long feedIndex, long replyIndex, int iqId);


	void deliverAppNotice(int toid, int appid, const string& appName, const string& appIcon, const string& msg);
	
	void loadOfflineFeedReply(const JidPtr& jid, int stype, int owner, Ice::Long source, const string& iqId);
	map<Ice::Long, int> getFeedReplyCount(int userid, const vector<Ice::Long>& feeds);
  void deliverFeedReplyCount(const JidPtr& jid, const vector<Ice::Long>& feeds, const string& iqid, const string& xmlns);
protected:

	WTransitManagerPrx getManagerOneway(int id, bool first15 = true);
	WTransitManagerPrx getManager(int id, bool first15 = true);

  /*
	virtual string name() {
		return "WTransit";
	}
	virtual string endpoints() {
		return "@WTransit";
	}
	virtual size_t cluster() {
		return 20;  
	}
	vector<WTransitManagerPrx> _managersOneway;
	vector<WTransitManagerPrx> _managers;
  */

};
}
;
}
;



#endif /*WEBTRANSITSADAPTER_H_*/
