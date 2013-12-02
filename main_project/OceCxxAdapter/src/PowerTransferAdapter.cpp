#include "PowerTransferAdapter.h"



namespace com {
namespace xiaonei {
namespace talk{
namespace function{
namespace transfer{
PowerTransferPrx PowerTransferAdapter::getManagerOneway(int id) {
        return getProxyOneway(id);
}
PowerTransferPrx PowerTransferAdapter::getManager(int id) {
        return getProxy(id);
}
void PowerTransferAdapter::deliverFeedWithFocusfriend(const ::xce::feed::FeedSeedPtr& feed, const MyUtil::IntSeq& target, const MyUtil::IntSeq& focusfriendSeq) {
        //getManagerOneway(feed->actor)->deliverFeed(feed, target);
        getManagerOneway(0)->deliverFeedWithFocusfriend(feed, target, focusfriendSeq);
}



}
}
}
}
}
