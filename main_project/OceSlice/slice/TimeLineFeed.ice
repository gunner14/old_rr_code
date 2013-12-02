#ifndef _Time_Line_Feed_
#define _Time_Line_Feed_

#include <Util.ice>
#include <RFeed.ice>

module xce
{
  module feed
  {
    module timeline
    {
      dictionary<int,MyUtil::IntSeq> UidYear;

      struct TimeLineFeedInfo  {
          MyUtil::LongSeq feeds;
          int flag;
      };
      sequence<TimeLineFeedInfo> TimeLineFeedInfoSeq;

      struct TimeLineFeedResult {
          int size;
          TimeLineFeedInfoSeq feeds;
      };
      interface TimeLineFeed
      {
         UidYear Query(int uid);
         TimeLineFeedResult QueryFeedInfoByStype(int uid,int year,int month,MyUtil::IntSeq types,int begin,int limit);
         TimeLineFeedResult QueryFeedInfo(int uid,int year,int month,int begin,int limit);
         int QueryFeedSize(int uid,int year,int month);
         int QueryFeedSizeByStype(int uid,int year,int month, MyUtil::IntSeq types);
         bool IsWideScreen(int uid,int year, long feed);
         bool SetWideScreen(int uid,int year, long feed, bool wide);
         void Modify(int uid,int oldYear, long feed, int year,int month,int day);
         void AddFeedMiniNotify(FeedSeed seed,int type,long time);
         void AddFeedMini(FeedSeed seed);
         void SetReadMini(int uid,int year, int stype, long merge);
         void SetReadMiniByIds(int uid,int year, MyUtil::LongSeq feeds);
         void SetReadMiniById(int uid,int year, long feed);
         void LoadFeedsToCache(int uid);
         string Execute(int uid,string cmd);
      };
    };
  };
};


#endif
