#ifndef __FEED_TRANSFER__
#define __FEED_TRANSFER__

#include "RFeed.h"
#include "ServiceI.h"
#include "PowerTransfer.h"
#include "ReceiversHandler.h"
#include "TalkCommon.h"
#include "Common.h"
#include <Ice/Ice.h>
#include "IMStormAdapter.h"
#include "IMStorm.h"

using namespace ::xce::feed;
using namespace ::xce::talk;

namespace com{
namespace xiaonei{
namespace talk{
namespace function{
namespace transfer{


class DeliverFeedTask : public Task {
public:
  DeliverFeedTask(const FeedSeedPtr& feed, const MyUtil::IntSeq &target, const MyUtil::IntSeq focusfriendSeq) 
    : Task(100), feed_(feed), target_(target) {
    small_type_ = boost::lexical_cast<std::string>(feed->type & 0xFFFF);
    big_type_ = boost::lexical_cast<std::string>((feed->type & 0xFFFF) / 100 );
    source_ = boost::lexical_cast<std::string>(feed->source);
    actor_ = boost::lexical_cast<std::string>(feed->actor);
    time_ = boost::lexical_cast<std::string>(feed->time);
    focus_friends_.insert(focusfriendSeq.begin(), focusfriendSeq.end());
  } 
  virtual void handle();
private:
  void DeliverFeedInit();
  bool XmlParas(); 

  FeedSeedPtr feed_;
  string small_type_;
  string big_type_;
  string source_;
  string actor_;
  string time_;
  MyUtil::IntSeq target_;
  set<int> focus_friends_;
  Str2StrMap msg2client_;
  Str2StrMap msg2webpager_unfocus_;
  Str2StrMap msg2webpager_focus_;
  Str2StrMap msg2phone_;
  vector<string> client_jids_;
  vector<string> webpager_focus_jids_;
  vector<string> webpager_unfocus_jids_;
  vector<string> phone_jids_;
};

};
};
};
};
};

#endif
