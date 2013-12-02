#ifndef __FEED_REPLY_TRANSFER__ 
#define __FEED_REPLY_TRANSFER__

#include "RFeed.h"
#include "TalkCommon.h"
#include "Common.h"

namespace com{
namespace xiaonei{
namespace talk{
namespace function{
namespace transfer{

using namespace ::xce::feed;

class DeliverFeedReplyTask : public Task {
public:
  DeliverFeedReplyTask(const ReplyDataPtr& replyData, const MyUtil::IntSeq &target)
    : Task(110), reply_data_(replyData), target_(target) {} 
  virtual void handle();
private:
  ReplyDataPtr reply_data_;
  MyUtil::IntSeq target_;
};

};
};
};
};
};

#endif
