
#ifndef __FEEDSOCIALWIKIHELP_H__
#define __FEEDSOCIALWIKIHELP_H__

#include "UserFeeds.h"
#include "QueryRunner.h"
#include "SocialWikiByUseridReplicaAdapter.h"
#include "SocialWikiNewsByWikiidReplicaAdapter.h"
//#include "SocialWikiHotByWikiidReplicaAdapter.h"

namespace xce { namespace feed {

	using namespace xce::feed;
	using namespace com::xiaonei::xce;

	//-----------------------------------------------------------------------------
	//常量定义
	const string    SW_INSTANCE_NAME = "feed_social_wiki";
	const string    SW_TABLE_PRE     = "feed_social_wiki_";
	const Ice::Int  SW_TABLE_COUNT   = 100;
	const string    SW_COL_FEEDID    = "feedid";
	const string    SW_COL_WIKIID    = "wikiid";
	const string    SW_COL_USERID    = "userid";
	const string    SW_COL_TYPE      = "type";
	const string    SW_COL_PRIVACY   = "privacy";
	const string    SW_COL_TIME      = "time";
	const string    SW_COL_MERGEID   = "mergeid";

	//-----------------------------------------------------------------------------
	// FeedSocialWikiHelper
	class FeedSocialWikiHelper: public Singleton<FeedSocialWikiHelper> {
		public:
			void put(const FeedMeta& fm, const MyUtil::Str2StrMap& ctx);
			//void del(const FeedMeta& fm, const MyUtil::Str2StrMap& ctx);
			void del(Ice::Int userid, Ice::Long feedid, const MyUtil::IntSeq& wikiids);
			//void changePrivacy(const FeedMeta& fm, const MyUtil::Str2StrMap& ctx);
			void changePrivacy(Ice::Int userid, Ice::Long feedid, Ice::Int privacy, const MyUtil::IntSeq& wikiids);

		private:
			inline bool feedmeta2wikiitem(const FeedMeta& fm, const MyUtil::Str2StrMap& ctx, SocialWikiItemSeq& items);
			inline bool checkPrivacy(const Ice::Int privacy);

			void puts2userid(const SocialWikiItemSeq& items);
			void puts2wikiid(const SocialWikiItemSeq& items);
			void delFromUserid(Ice::Int userid, const MyUtil::IntSeq& wikiids, Ice::Long feedid);
			void delFromWikiid(const MyUtil::IntSeq& wikiids, Ice::Long feedid);
	};

	//-----------------------------------------------------------------------------
	class AsynDBWriter: public MyUtil::Timer {
		public:
			AsynDBWriter(Ice::Int channel, string table): 
				Timer(3*1000, channel+10), table_(table){}

			void put(const SocialWikiItem& item);
			void handle();
		private:
			string table_;
			SocialWikiItemSeq items_;
			IceUtil::Mutex mutex_;
	};
	typedef IceUtil::Handle<AsynDBWriter> AsynDBWriterPtr;
	//-----------------------------------------------------------------------------
	// FeedSocialWikiDBHelper
	class FeedSocialWikiDBHelper: public Singleton<FeedSocialWikiDBHelper> {
		public:
			void put(const SocialWikiItem& item);
			void puts(const SocialWikiItemSeq& items);

			void put_asyn(const SocialWikiItem& item);
			void puts_asyn(const SocialWikiItemSeq& items);

			SocialWikiItem get(Ice::Int wikiid, Ice::Long feedid);
			void remove(Ice::Long feedid, const MyUtil::IntSeq& wikiids);
			void updatePrivacy(Ice::Long feedid, const MyUtil::IntSeq& wikiids, Ice::Int privacy);

		private:
			inline string getTable(Ice::Int wikiid);
			inline Statement getSql(const SocialWikiItem& item);
			inline AsynDBWriterPtr getWriter(Ice::Int wikiid);

			AsynDBWriterPtr writers_[SW_TABLE_COUNT];
			IceUtil::RWRecMutex mutex_[SW_TABLE_COUNT];
	};

}}  //namespace end
#endif

