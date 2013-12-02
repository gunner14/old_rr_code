#ifndef FEED_INTERFACE_ADAPTER_H
#define FEED_INTERFACE_ADAPTER_H

#include "RFeed.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce{
namespace feed{

using namespace MyUtil;

class FeedInterfaceAdapter : public AdapterI,
	public AdapterISingleton<MyUtil::XceFeedChannel, FeedInterfaceAdapter>
{

protected:
	virtual string name() {
		return "FeedInterface";
	}
	virtual string endpoints() {
		return "@FeedInterface";
	}
	virtual size_t cluster() {
		return 1;
	}

public:
	FeedInterfaceAdapter();
	void removeFeedById(Ice::Long feedId);
	void removeFeedBySource(Ice::Long source, int stype);
	void removeFeedBySourceHS(Ice::Long source, int stype);
	void removeFeedByParent(Ice::Long parent, int ptype);
	void removeFeed(Ice::Long source, int stype, int actor);


	void removeFeedReply(Ice::Long source, int stype, int actor, Ice::Long removeReplyId, const FeedReplyPtr& reply);
  void removeFeedReplyById(int stype, Ice::Long feedid, Ice::Long removeReplyId, const FeedReplyPtr& reply);




	void readNews(int user, Ice::Long feed);
	void readAllNews(int user);
	void readMini(int user, Ice::Long feedid);
	void readAllMini(int user);

	void dispatch(const FeedSeedPtr& seed);

	void dispatchWithReply(const FeedSeedPtr& seed, const FeedReplyPtr& reply);
	void dispatchEDM(const FeedSeedPtr& seed);

	void addFeedReply(Ice::Long source, int stype, int actor, const FeedReplyPtr & reply);
  void addFeedReplyById(int stype, Ice::Long feedid,const FeedReplyPtr& reply);
  void addFeedReplyWithConfig(Ice::Long source, Ice::Int stype, Ice::Int actor, const FeedReplyPtr& reply, const map<string, string> & config);
  void dispatchAgain(Ice::Long source, int stype, int actor, int toid);
  void ReplaceXML(Ice::Long source, int stype, int actor, const string & xml);
  void DirectedSend(Ice::Long source, int stype, int actor, const vector<int> & targets);


//	Ice::Long createFeedId();

  void MarkFeed(int uid, Ice::Long fid, Ice::Long source, int stype, int actor, bool mark);

  FeedItemSeq GetOwnFeeds(const IntSeq & userids, int begin, int limit, const IntSeq & types);
  FeedContentDict GetFeedDict(const LongSeq & feedIds);
  
private:

	FeedInterfacePrx getManager();
	FeedInterfacePrx getManagerOneway();
	FeedInterfacePrx getManagerTwoway();

	vector<FeedInterfacePrx> _managersTwoway;
	vector<FeedInterfacePrx> _managersOneway;
	vector<FeedInterfacePrx> _managers;
};


};
};
#endif
