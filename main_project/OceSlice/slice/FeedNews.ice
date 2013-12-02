#ifndef __FEEDNEWS_ICE__
#define __FEEDNEWS_ICE__

#include <Util.ice>
#include <RFeed.ice>

module xce
{
module feed
{

class MultiFeed {
  int type;
  MyUtil::LongSeq showfeeds;
  MyUtil::LongSeq hidefeeds;
};

sequence<MultiFeed> MultiFeedSeq;

class MultiFeedsRes {
  int flag;
  MultiFeedSeq feeds;
};

const string JsonFirstVersion = "0";
const string JsonMultiFeedType = "type";
const string JsonMultiFeedShowData = "sd";
const string JsonMultiFeedHideData = "hd";
const string JsonFeedsHasOld = "hasold";
const string JsonFeedFeedidKey = "fid";

class FeedNews {
  string GetLiveFeedData(int uid, int begin, int limit);
  string GetLiveFeedDataByStype(int uid, MyUtil::IntSeq stypes, int begin, int limit);
  string GetLiveFeedDataByType(int uid, MyUtil::IntSeq stypes, int begin, int limit);
  
  string GetHotFeedData(int uid, int begin, int limit);
  string GetHotFeedDataByStype(int uid, MyUtil::IntSeq stypes, int begin, int limit);
  string GetHotFeedDataByType(int uid, MyUtil::IntSeq stypes, int begin, int limit);
    
  //FeedDataSeq GetFeedDataByIds(MyUtil::LongSeq fids);
  //FeedDataSeq GetFeedDataByIdsWithUid(int uid,MyUtil::LongSeq fids,long first,int stype);
    
  string GetFriendOriginal(int uid, MyUtil::IntSeq friends, int begin, int limit); //好友原创
  string GetFeedDataByGroupName(int uid, string name, int begin, int limit); //好友分组
  string GetFeedDataByGroupId(int uid, int id, int begin, int limit); //好友分组

  FeedItemSeq GetOriginalFeedData(int uid);

  int GetUnviewedCount(int uid, int type);
  MyUtil::Int2IntMap  GetCountMapByStype(int uid, MyUtil::IntSeq stypes, bool unviewed);

  void AddFeed(FeedItem item, FeedConfig config, MyUtil::Int2IntMap id2weight);
  void SetRead(int uid, int stype, long merge);
  void SetReadById(int uid,long feedId);
  void SetReadAll(int uid);

  //void load(int userid);
  void EraseUser(int uid);  
};


};
};

#endif
