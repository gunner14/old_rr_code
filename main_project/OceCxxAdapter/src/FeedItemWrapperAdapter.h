
#ifndef __FEEDITEMWRAPPERADAPTER_H_
#define __FEEDITEMWRAPPERADAPTER_H_

#include "Singleton.h"
#include "AdapterI.h"
#include "UserFeeds.h"

namespace xce {
namespace feed {

using namespace MyUtil;

class FeedItemWrapperAdapter: public MyUtil::AdapterI,
    public MyUtil::AdapterISingleton<MyUtil::XceFeedChannel, FeedItemWrapperAdapter> {
public:
    FeedItemWrapperAdapter() {
        _managers.resize(cluster() > 0 ? cluster() : 1);
        _managersOneway.resize(cluster() > 0 ? cluster() : 1);
    }

    FeedItemSeq getItemSeq(Ice::Int userid, Ice::Int begin, Ice::Int limit);
    FeedMetaSeq getLessThanFeedid(Ice::Int userid, Ice::Long feedid, Ice::Int limit);
    FeedMetaSeq get(int userid, int begin, int limit);
    FeedMetaSeq GetFeeds(int userid, int begin, int limit, const MyUtil::IntSeq& types, const MyUtil::IntSeq& ntypes);
    FeedMetaSeq GetFeedsByGroupName(int userid, const string& name, int begin, int limit, const MyUtil::IntSeq& types, const MyUtil::IntSeq& ntypes);
    FeedMetaSeq GetFeedsByFocus(int userid, int begin, int limit, const MyUtil::IntSeq& types, const MyUtil::IntSeq& ntypes);

    FeedItemWrapperPrx getManager(int id);
    FeedItemWrapperPrx getManagerOneway(int id);

protected:
    virtual string name() {
        //return "FeedNewsLoader";
        return "FeedItemWrapper";
    }
    virtual string endpoints() {
        //return "@FeedNewsLoader";
        return "@FeedItemWrapper";
    }
    virtual size_t cluster() {
        return 1;
    }

    vector<FeedItemWrapperPrx> _managersOneway;
    vector<FeedItemWrapperPrx> _managers;
};

}
;
}
;
#endif /* __FEEDFOCUSADAPTER_H_ */
