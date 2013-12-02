/*
 * =================================================================
 * 
 *       Author :   duo.zhang
 * Organization :   RenRen-INC
 *        Email :   duo.zhang@renren-inc.com
 *      Created :   2012-03-09  14:18:37
 *  Description :   
 *
 * =================================================================
 */
 
#ifndef _FEED_SOCIALWIKI_SOCIALWIKIBYUSERID_SRC_SOCIALWIKIBYUSERIDI_H_
#define _FEED_SOCIALWIKI_SOCIALWIKIBYUSERID_SRC_SOCIALWIKIBYUSERIDI_H_

#include "FeedSocialWiki.h"

using namespace MyUtil;

namespace xce {
namespace feed {

static const int WIKI_USER_CACHE_CATEGORY = 0;
static const int TASK_LEVEL_SOCIAL_WIKI = 102;
static const int TASK_LEVEL_ADD_OBJECT = 103;

class SocialWikiItemCompare 
{
public:
	bool operator()(const SocialWikiItem & a, const SocialWikiItem & b) {
		int d = a.time - b.time;
		if(d == 0)
			return a.feedid > b.feedid;
		return d > 0;
	}
};

class AddUserWikiTask : virtual public MyUtil::Task {
public:
	AddUserWikiTask(int userid, int wikiid) : Task(TASK_LEVEL_ADD_OBJECT), userid_(userid), wikiid_(wikiid) {
	}

protected:
	virtual void handle();

private:
	int userid_;
	int wikiid_;
};

class UserWikiFeed;
class LoadWikiFeedTask : virtual public MyUtil::Task {
	static const int WIKITABLECOUNT = 100;
public:
	LoadWikiFeedTask(int wikiid, int userid, SocialWikiItemSeq * seq = NULL) : Task(TASK_LEVEL_SOCIAL_WIKI), userwikiptr_(NULL), wikiid_(wikiid), userid_(userid), pSeq_(seq) {
	}
	~LoadWikiFeedTask() {}

protected:
	virtual void handle();

private:
	UserWikiFeed * userwikiptr_;
	int wikiid_;
	int userid_;
	SocialWikiItemSeq * pSeq_;
};

class UserWikiFeed : public ::IceUtil::RWRecMutex, public ::Ice::Object 
{
	typedef std::map<int, SocialWikiItemSeq> WikiToItemMap;
	friend class LoadWikiFeedTask;
public:
	UserWikiFeed(int userid) : userid_(userid) {
	}

	void add(const SocialWikiItem & item);
	void remove(const MyUtil::IntSeq & wikiids, long feedid);
	void get(int wikiid, const MyUtil::IntSeq types, SocialWikiItemSeq & res);

private:
	IceUtil::RWRecMutex mapmutex_;
	WikiToItemMap feeditem_;
	int userid_;
};
typedef ::IceUtil::Handle<UserWikiFeed> UserWikiFeedPtr;

class WikiUserFactoryI : public MyUtil::ServantFactory 
{
public:
	virtual Ice::ObjectPtr create(Ice::Int userid);
};

class SocialWikiByUseridI : virtual public SocialWikiByUserid,
		public ::MyUtil::Singleton<SocialWikiByUseridI>	
{
public:
	virtual void put(const ::xce::feed::SocialWikiItem&, const ::Ice::Current& = ::Ice::Current());
	virtual ::MyUtil::LongSeq gets(const ::MyUtil::IntSeq&, ::Ice::Int, ::Ice::Int, ::Ice::Int, const ::Ice::Current& = ::Ice::Current());
	virtual ::MyUtil::LongSeq getsWithType(const ::MyUtil::IntSeq&, ::Ice::Int, const ::MyUtil::IntSeq&, ::Ice::Int, ::Ice::Int, const ::Ice::Current& = ::Ice::Current());
	virtual void deleteFeed(::Ice::Int, const ::MyUtil::IntSeq&, ::Ice::Long, const ::Ice::Current& = ::Ice::Current());

private:
	::MyUtil::LongSeq productionResult(SocialWikiItemSeq & seq, int begin, int limit);
	IceUtil::Mutex mutex_;
};

} //namespace feed
} //namespace xce

#endif
