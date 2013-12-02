#ifndef _FEED_ITEM_BLOCK_CACHE_H_
#define _FEED_ITEM_BLOCK_CACHE_H_

#include <ObjectCache.ice>
#include <Util.ice>
module xce {
module feed {
    class UserBlockInfo {
        MyUtil::IntSeq  types;
        MyUtil::IntSeq  userids;
        MyUtil::LongSeq feedids;
        long            maxReadFeedid;
    };

    interface FeedItemBlockCache {
        MyUtil::IntSeq getUserBlockTypes(int userid);  //如果不存在,则创建新的
        void addUserBlockType(int userid, int type);
        void removeUserBlockType(int userid, int type);
        bool hasUserBlockType(int userid, int type);

        MyUtil::IntSeq getUserBlockUsers(int userid);
        void addUserBlockUser(int userid, int blockUserid);
        void removeUserBlockUser(int userid, int blockUserid);
        bool hasUserBlockUser(int userid, int blockUserid);

        MyUtil::LongSeq getUserBlockFeeds(int userid);
        void addUserBlockFeed(int userid, long feedid);
        void removeUserBlockFeed(int userid, long feedid);
        bool hasUserBlockFeed(int userid, long feedid);

        void setMaxReadFeed(int userid, long feedid);
        long getMaxReadFeed(int userid);

        UserBlockInfo getAll(int userid);

        // reload
        void reloadUserBlockTypes(int userid);
        void reloadUserBlockUsers(int userid);
        void reloadUserBlockFeeds(int userid);
        void reloadAll(int userid);
    };   
};
};
#endif
