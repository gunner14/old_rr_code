/*
 * =====================================================================================
 *
 *       Filename:  FeedItemFacadeI.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011年10月27日 18时46分38秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  guanghe.ge (ggh), guanghe.ge@opi-corp.com
 *        Company:  XiaoNei
 *
 * =====================================================================================
 */

#ifndef __FEEDITEMFACADE_I_H__
#define __FEEDITEMFACADE_I_H__

#include <UserFeeds.h>
#include "TaskManager.h"
#include "Singleton.h"

namespace xce {
namespace feed {

static const int PUT_FEED_TASK_LEVEL = 102;
class PutFeedTask : public  MyUtil::Task {
public:
	PutFeedTask(FeedSeedPtr seed) : MyUtil::Task(PUT_FEED_TASK_LEVEL), seed_(seed) {
	}

	virtual void handle();
private:
	FeedSeedPtr seed_;
};

class FeedItemFacadeI : public FeedItemFacade, public MyUtil::Singleton<FeedItemFacadeI> {
public:
    FeedItemFacadeI();
    virtual ~FeedItemFacadeI();

    virtual void put(const FeedSeedPtr& seed, const ::Ice::Current& = ::Ice::Current());
    virtual void deleteFeed(Ice::Int userid, Ice::Long feedid, const ::Ice::Current& = ::Ice::Current());
    virtual void delWikiFeed(Ice::Int userid, Ice::Long source, Ice::Int stype, const ::Ice::Current& = ::Ice::Current());
    virtual void changeWikiFeedPrivacy(Ice::Int usreid, Ice::Long source, Ice::Int stype, Ice::Int privacy, const ::Ice::Current& = ::Ice::Current());

private:
		void SourceAndStype2Feedids(Ice::Long source, Ice::Int stype, MyUtil::LongSeq & feedids);
		void Feedid2Wikiids(Ice::Long feedid, MyUtil::IntSeq & wikiids);
};

}
}

#endif
