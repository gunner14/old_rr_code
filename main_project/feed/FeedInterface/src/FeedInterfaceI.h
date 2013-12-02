#ifndef FEEDINTERFACEI_H_
#define FEEDINTERFACEI_H_
#include "ServiceI.h"
#include "Singleton.h"
#include "RFeed.h"
#include "QueryRunner.h"

namespace xce {
namespace feed {

using namespace MyUtil;

const int TYPE[] = {100, 500, 600, 700, 800, 900, 1800, 1900, 2000, 2400, 3700, 8100, 8200, 8600};
const int kCallCountMax = 5;

class FeedInterfaceI: virtual public FeedInterface, public MyUtil::Singleton<FeedInterfaceI> {

public:

	FeedInterfaceI() :getownfeeds_callcount_(0), getfeeddict_callcount_(0) {
	}
	virtual void removeFeedById(Ice::Long feedId, const Ice::Current& current =
			Ice::Current());
	virtual void removeFeedBySource(Ice::Long source, Ice::Int stype,
			const Ice::Current& current = Ice::Current());
	virtual void removeFeedBySourceHS(Ice::Long source, Ice::Int stype,
			const Ice::Current& current = Ice::Current());
	virtual void removeFeedByParent(Ice::Long parent, Ice::Int ptype,
			const Ice::Current& current = Ice::Current());
	virtual void removeFeed(Ice::Long source, Ice::Int stype, Ice::Int actor,
			const Ice::Current& current = Ice::Current());

	virtual void removeFeedReply(Ice::Long source, Ice::Int stype,
			Ice::Int actor, Ice::Long removeReplyId, const FeedReplyPtr& reply,
			const Ice::Current& current = Ice::Current());
	virtual void removeFeedReplyById(int stype, Ice::Long feedid,
			Ice::Long removeReplyId, const FeedReplyPtr& reply,
			const Ice::Current& current = Ice::Current());
	virtual void readNews(Ice::Int user, Ice::Long feed,
			const Ice::Current& current = Ice::Current());
	virtual void readAllNews(Ice::Int user, const Ice::Current& current =
			Ice::Current());

	virtual void readMini(Ice::Int user, Ice::Long feed,
			const Ice::Current& current = Ice::Current());
	virtual void readAllMini(Ice::Int user, const Ice::Current& current =
			Ice::Current());

	virtual void dispatch(const FeedSeedPtr& seed, const Ice::Current& current =
			Ice::Current());
	virtual void dispatchEDM(const FeedSeedPtr& seed,
			const Ice::Current& current = Ice::Current());
	virtual void addFeedReplyById(int stype, Ice::Long feedid,
			const FeedReplyPtr& reply, const Ice::Current& current =
					Ice::Current());

	virtual void addFeedReplyWithConfig(Ice::Long source, int stype,
			Ice::Int actor, const FeedReplyPtr& reply,
			const map<string, string> & config, const Ice::Current& current =
					Ice::Current());

	virtual void dispatchWithReply(const FeedSeedPtr& seed,
			const FeedReplyPtr& reply, const Ice::Current& current =
					Ice::Current());
	virtual void addFeedReply(Ice::Long source, Ice::Int stype, Ice::Int actor,
			const FeedReplyPtr& reply, const Ice::Current& current =
					Ice::Current());

	virtual void dispatchAgain(Ice::Long source, int stype, int actor, int toid, const Ice::Current& current = Ice::Current());

	virtual void ReplaceXML(Ice::Long source, int stype, int actor,
			const string & xml, const Ice::Current& = Ice::Current());//TODO

  virtual Ice::Long getFeedId(Ice::Long, Ice::Int, Ice::Int, const Ice::Current& = Ice::Current()) { return 0l;}
  virtual void cacheSync(Ice::Long, Ice::Int, Ice::Int, const Ice::Current& = Ice::Current()) {}    

	virtual void DirectedSend(Ice::Long source, int stype, int actor,
			const vector<int> & targets, const Ice::Current& = Ice::Current());

  void MarkFeed(int uid, Ice::Long fid, Ice::Long source, int stype, 
      int actor, bool mark, const Ice::Current& = Ice::Current());

  virtual  FeedItemSeq GetOwnFeeds(const MyUtil::IntSeq & userids, int begin, int limit, const MyUtil::IntSeq & types, const Ice::Current& = Ice::Current() );
  virtual  FeedContentDict GetFeedDict(const MyUtil::LongSeq & feedIds, const Ice::Current& = Ice::Current() );

  virtual  void ResetCallCount(int count, const Ice::Current& = Ice::Current()) {
    IceUtil::Mutex::Lock lock(mutex_);
    getownfeeds_callcount_ = count;
    getfeeddict_callcount_ = count;
  }
private:
  MyUtil::IntSeq getFixedType();
  int getownfeeds_callcount_;
  int getfeeddict_callcount_;
  IceUtil::Mutex mutex_;
};


}
;
}
;

#endif /* FEEDINTERFACEI_H_ */
