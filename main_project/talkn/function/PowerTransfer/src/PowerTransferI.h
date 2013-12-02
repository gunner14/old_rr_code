#ifndef __POWER_TRANSFER__ 
#define __POWER_TRANSFER__

#include "RFeed.h"
#include "ServiceI.h"
#include "PowerTransfer.h"
#include "ReceiversHandler.h"
#include "TalkCommon.h"
#include "Common.h"
#include <Ice/Ice.h>
#include "IMStormAdapter.h"
#include "IMStorm.h"

namespace com{
namespace xiaonei{
namespace talk{
namespace function{
namespace transfer{

using namespace ::xce::feed;
using namespace ::xce::talk;

class PowerTransferI : public PowerTransfer, public MyUtil::Singleton<PowerTransferI>{
public:
  virtual void notify(const Str2StrMap& paras, const Ice::Current& = Ice::Current());
  virtual void deliverFeedReply(const ReplyDataPtr& data, const MyUtil::IntSeq& target,
    const Ice::Current& = Ice::Current());
	virtual void deliverFeedWithFocusfriend(const FeedSeedPtr& feed, const MyUtil::IntSeq& target, 
    const MyUtil::IntSeq& focusfriendSeq, const Ice::Current& = Ice::Current());
  void FeedSend2PIMStorm(const FeedSeedPtr& feed, const MyUtil::IntSeq& target,
    const MyUtil::IntSeq& focusfriendSeq);

  void DeliverTypeSetInit() {
    int tmp[] = {101, 102, 103, 104, 106, 107, 110, 502, 601, 701, 
      702, 709 ,801, 808, 8301, 2401, 211, 210, 209, 213,2002,2012,2032,2038,2013,2008,2035,2036,2005,2006,504}; 
    for(size_t i = 0; i < sizeof(tmp) / sizeof(int); ++i)
      deliver_type_set_.insert(tmp[i]);
    int tmp_p[] = {102, 103, 104, 502, 601, 701};
    for(size_t i = 0; i < sizeof(tmp_p) / sizeof(int); ++i)
      deliver_ptalkproxy_set_.insert(tmp_p[i]);
  }

  bool IsDeliverType(int stype) {
    return (deliver_type_set_.find(stype) != deliver_type_set_.end());
  }
  
  bool IsDeliver2PTalkproxy(int stype) {
    return (deliver_ptalkproxy_set_.find(stype) != deliver_ptalkproxy_set_.end());
  }

private:
  set<int> deliver_type_set_;
  set<int> deliver_ptalkproxy_set_;
  IceUtil::Mutex _promutex;
  map<string, string> _properties;
};

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
