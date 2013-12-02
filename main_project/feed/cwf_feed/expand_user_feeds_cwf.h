#ifndef __EXPAND_USER_FEEDS_CWF__
#define __EXPAND_USER_FEEDS_CWF__
#include "RFeed.h"
#include "feed/cwf_feed/action.h"
namespace xce {
int ExpandUserFeedsCwf(int owner, int uid, const std::string& name,
    int render, feed::FeedDataSeq& feed_seq, int from, cwf::Response * res, std::string& stat,
    bool isAdmin = false, int view = 0);
}
#endif
