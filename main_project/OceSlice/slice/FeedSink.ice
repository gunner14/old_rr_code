#ifndef FEED_SINK_ICE
#define FEED_SINK_ICE

#include <RFeed.ice>

module xce {
module feed {


class FeedSink{
    void AddFeed(FeedItem item, FeedConfig config, MyUtil::Int2IntMap id2weight);
	void SetRead(int uid, int stype, long merge);
	void SetReadById(int uid,long feedId);
	void SetReadAll(int uid);
	void ReloadWhitelist();
	void GetFeedData(int uid, bool hot, bool bystype,MyUtil::IntSeq stypes, int begin, int limit);
	void MarkFeed(int uid,long fid,long source,int stype,int actor,bool mark);
};

};
};
#endif