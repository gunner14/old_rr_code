/*
 * =====================================================================================
 *
 *       Filename:  FeedItemWrapperI.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011年11月10日 12时04分52秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  guanghe.ge (ggh), guanghe.ge@opi-corp.com
 *        Company:  XiaoNei
 *
 * =====================================================================================
 */

#ifndef __FEEDITEMWRAPPER_I_H__
#define __FEEDITEMWRAPPER_I_H__

#include <set>
#include <vector>
#include "UserFeeds.h"
#include "Singleton.h"
#include <TaskManager.h>
#include "QueryRunner.h"
#include "ObjectCacheI.h"
#include "IceUtil/IceUtil.h"
#include "IceUtil/Mutex.h"
#include "FeedItemBlockCacheReplicaAdapter.h"

namespace xce {
namespace feed {

using namespace xce::feed;
using namespace std;

//const int TYPE[] = {502, 103, 102, 110, 701, 702, 2002, 104, 708, 2401, 8190, 709, 601, 107};
const int TYPE[] = {100, 500, 600, 700, 800, 900, 1800, 1900, 2000, 2400, 3700, 8100, 8200, 8600};

class FeedItemWrapperI : public FeedItemWrapper, public MyUtil::Singleton<FeedItemWrapperI> {
public:
    FeedItemWrapperI();
    ~FeedItemWrapperI();
    
    virtual FeedItemSeq getItemSeq(::Ice::Int userid, ::Ice::Int begin, ::Ice::Int limit, const ::Ice::Current& = ::Ice::Current());
    virtual FeedMetaSeq getLessThanFeedid(::Ice::Int userid, ::Ice::Long feedid, ::Ice::Int limit, const ::Ice::Current& = ::Ice::Current());
    virtual FeedMetaSeq get(::Ice::Int userid, ::Ice::Int begin, ::Ice::Int limit, const ::Ice::Current& = ::Ice::Current());
    virtual FeedMetaSeq GetFeeds(::Ice::Int userid, ::Ice::Int begin, ::Ice::Int limit, const MyUtil::IntSeq& types, const MyUtil::IntSeq& ntypes, const ::Ice::Current& = ::Ice::Current());
    virtual FeedMetaSeq GetFeedsByGroupId(int userid, int group, int begin, int limit, const MyUtil::IntSeq& types, const MyUtil::IntSeq& ntypes, const ::Ice::Current& = ::Ice::Current());
    virtual FeedMetaSeq GetFeedsByGroupName(int userid, const string& name, int begin, int limit, const MyUtil::IntSeq& types, const MyUtil::IntSeq& ntypes, const ::Ice::Current& = ::Ice::Current());
    virtual FeedMetaSeq GetFeedsByFocus(int userid, int begin, int limit, const MyUtil::IntSeq& types, const MyUtil::IntSeq& ntypes, const ::Ice::Current& = ::Ice::Current());
private:
    MyUtil::IntSeq getFixedType();
    
    mysqlpp::StoreQueryResult getPages(int user);
    mysqlpp::StoreQueryResult getMinisite(int user);
    MyUtil::StrSeq createFilter(int user, const MyUtil::IntSeq ntypes);
    void getFilterAndMaxReadFeedid(Ice::Int userid, StrSeq& filters, Ice::Long& maxReadFeedid);
};

}
}

#endif
