#ifndef __FEEDGUIDE_ICE__
#define __FEEDGUIDE_ICE__

#include <RFeed.ice>

module xce {
module feed {

  class FeedGuideManager {
    FeedDataRes GetGuideFeedData(int uid, int begin, int limit);
    FeedDataRes GetGuideFeedDataByStype(int uid, MyUtil::IntSeq stypes, int begin, int limit);
//    FeedDataRes GetGuideLiveFeedData(int uid, int begin, int limit);
//    FeedDataRes GetGuideLiveFeedDataByStype(int uid, MyUtil::IntSeq stypes, int begin, int limit);
//    FeedDataRes GetGuideLiveFeedDataByType(int uid, MyUtil::IntSeq stypes, int begin, int limit);
//
//    FeedDataRes GetGuideHotFeedData(int uid, int begin, int limit);
//    FeedDataRes GetGuideHotFeedDataByStype(int uid, MyUtil::IntSeq stypes, int begin, int limit);
//    FeedDataRes GetGuideHotFeedDataByType(int uid, MyUtil::IntSeq stypes, int begin, int limit);
//
    FeedDataSeq GetFeedDataByIds(MyUtil::LongSeq fids);
    FeedDataSeq GetFeedDataByIdsWithUid(int uid,MyUtil::LongSeq fids,long first,int stype);

    FeedItemSeq GetOriginalFeedData(int uid);

    int         GetUnviewedCount(int uid, int type);
    MyUtil::Int2IntMap	GetCountMapByStype(int uid, MyUtil::IntSeq stypes, bool unviewed);

    void AddFeed(FeedItem item, FeedConfig config, MyUtil::Int2IntMap id2weight);
    void SetRead(int uid, int stype, long merge);
    void SetReadById(int uid,long feedId);
    void SetReadAll(int uid);
    bool HasFeed(int uid);
    void load(int userid);
    FeedItemSeqSeq GetFeedItemSeqSeqByUsers( MyUtil::IntSeq uids); 
    void MarkFeed(int uid,long fid,long source,int stype,int actor,bool mark);
    void EraseUser(int uid);	
    void AddFeedReply(FeedIndex idx, int replycnt);

    void AddWhiteList(int userid);
    void DelWhiteList(int userid);
    MyUtil::IntSeq GetWhiteList();
    void ClearWhiteList();
  }; 

};
};




#endif
