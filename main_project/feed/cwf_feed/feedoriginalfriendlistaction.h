#ifndef XCE_FEED_ORIGINAL_FRIEND_LIST_ACTION_H__
#define XCE_FEED_ORIGINAL_FRIEND_LIST_ACTION_H__
#include "RFeed.h"
#include "FeedContentAdapter.h"
#include "feed/feedview/feedxml.h"
#include <string>

#include "feed/cwf_feed/action.h"
using namespace xce::feed;
namespace xce {

struct FeedOriginalFriendListAction : public cwf::BaseAction {
  bool Match(const std::string& url) const;
  void CheckLoad(const vector<Ice::Long> & fids, FeedContentDict & dict, int uid);
  void GetDataByIds(const vector<Ice::Long> & fids, feed::FeedDataSeq & feed_seq, int uid);
  cwf::HttpStatusCode Process(cwf::Request*, cwf::Response*);

#if 0
  virtual cwf::User* Authorize(cwf::Request*);
  virtual const std::string& template_name() const {
    return "";
  }
  virtual bool Match(const std::string& url) const;
  virtual cwf::HttpStatusCode GenerateHeader(cwf::Request* request, cwf::Header* header);
  virtual ctemplate::TemplateDictionary* Process(cwf::Request* request);

  FeedTypeAction() : template_name_("replylist.tpl") {}
#endif
};

}
#endif // XCE_FEED_ORIGINAL_FRIEND_LIST_ACTION_H__
