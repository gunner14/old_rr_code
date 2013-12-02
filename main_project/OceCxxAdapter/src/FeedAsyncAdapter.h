#ifndef FEED_ASYNC_ADAPTER_H
#define FEED_ASYNC_ADAPTER_H

#include "FeedAsync.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce{
namespace feed{

using namespace MyUtil;

class FeedAsyncAdapter : public AdapterI,
	public AdapterISingleton<MyUtil::XceFeedChannel, FeedAsyncAdapter>
{
public:

   bool AddFeedIndex(int index,const FeedIndexPtr & idx);
   bool RemoveFeedIndex(int index,const MyUtil::LongSeq& feeds);
   bool AddFeedContent(int index,const FeedSeedPtr & seed);
   bool RemoveFeedContent(int index,const MyUtil::LongSeq& feeds);

   bool AddFeedMini(int index,const FeedSeedPtr& seed);
   bool RemoveFeedMini(int index,int user, int stype, Ice::Long miniMerge);
   bool RemoveAllFeedMini(int index,int user);

   bool AddFeedSchool(int index,int schoolid, const FeedSeedPtr& seed);
   bool AddFeedGroup(int index,int groupid, const FeedSeedPtr& seed);

   bool ReplaceXML(int index,const  MyUtil::LongSeq & fids , const string & xml );
    bool UpdateReply(int index,const map<Ice::Long, FeedReplyPtr>& buffer);
    bool CopyFeedContent(int index,Ice::Long old_feed_id, Ice::Long new_feed_id, int new_type, const std::string& new_props);

protected:
	 string name() {
		return "FeedAsync";
	}
	 string endpoints() {
		return "@FeedAsync";
	}
	 size_t cluster() {
		return 1;
	}

public:
	FeedAsyncAdapter();
	~FeedAsyncAdapter();

//	int Push(int index,const SqlInfoPtr & sqlinfo);
//	void TestPush(int index,const SqlInfoPtr & sqlinfo);
private:

	FeedAsyncPrx getManager(int id);
	FeedAsyncPrx getManagerOneway(int id);

	vector<FeedAsyncPrx> _managersOneway;
	vector<FeedAsyncPrx> _managers;
};


};
};
#endif
