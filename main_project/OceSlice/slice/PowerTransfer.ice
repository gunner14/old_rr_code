#ifndef __POWER_TRANSFER_ICE__
#define __POWER_TRANSFER_ICE__
#include <RFeed.ice>
#include <TalkCommon.ice>
#include <Notify.ice>
#include <TalkFunction.ice>

module com
{
module xiaonei
{
module talk
{
module function
{
module transfer
{
	
	class PowerTransfer extends TalkFunBase{
	//newsfeed relate
	//realtime:
	void deliverFeedWithFocusfriend(xce::feed::FeedSeed seed, MyUtil::IntSeq target, MyUtil::IntSeq focusfriend);
	void deliverFeedReply( xce::feed::ReplyData data, MyUtil::IntSeq target);
	
	
	
	//notify relate
	void deliverNotifyWebpager(MyUtil::IntSeq toids, string content);
	void deliverNotifyClient(MyUtil::IntSeq toids, string content);
	void deliverNotifyPhone(MyUtil::IntSeq toids, string content);

	//passivity answer Iq protocol
	
	//MyUtil::Long2IntMap getFeedReplyCount(MyUtil::LongSeq feeds);not use
	//void deliverFeedReplyCount(com::xiaonei::talk::common::Jid jid, MyUtil::LongSeq feeds, string iqid, string xmlns);
	//void loadOfflineFeedReply(com::xiaonei::talk::common::Jid jid, int stype, int actor, long source, string iqId);

	//xiaozu relate
	//void loadXiaozuOfflineFeedReply(com::xiaonei::talk::common::Jid jid, int stype, long xiaozuid, long threadid, int page, string  iqId);
        //void loadXiaozuContent(com::xiaonei::talk::common::Jid jid, long xiaozuid, long threadid, string iqId);

	//offline
	//void loadOfflineFeed(com::xiaonei::talk::common::Jid jid, long feedIndex, long replyIndex, int iqId);
	//void notify(MyUtil::Str2StrMap paras);
};
};
};
};
};
};

#endif
