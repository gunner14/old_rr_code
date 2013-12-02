/*
 * FeedDbWriterI.h
 */

#ifndef __FEEDDBWRITER_H__
#define __FEEDDBWRITER_H__

#include "QueryRunner.h"
#include "RFeed.h"
#include "Singleton.h"
#include "TaskManager.h"
#include <set.h>
#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>

namespace xce {
namespace feed {
using namespace MyUtil;

const std::string kFeedDbWriter = "FDBW";
const int kTaskDbWriterLevel    = 10001;
const int kTaskRemoveFeedLevel        = 10003;
class FeedDbWriterI: public FeedDbWriter, public MyUtil::Singleton<FeedDbWriterI> {
public:
  FeedDbWriterI() {
  }
  virtual void FeedDispatch2DB(const FeedSeedPtr &,const FeedReplyPtr & ,const Ice::Current& =Ice::Current());
  virtual void FeedDispatchEDM2DB(const FeedSeedPtr &,const Ice::Current & =Ice::Current());
  virtual void AddFeedSchool2DB(int schoolid,const FeedSeedPtr & seed,const Ice::Current & = Ice::Current());
  virtual void UpdateReply2DB(const MyUtil::LongSeq& ,const FeedReplyPtr &,const Ice::Current& = Ice::Current());
  virtual void RemoveFeedFromDB(const FeedIndexSeq & indexSeq,const Ice::Current & = Ice::Current());
  virtual void ReplaceFeedXML2DB(const MyUtil::LongSeq& ,const string & xml ,const Ice::Current& = Ice::Current());
};

class AddFeedTask:public MyUtil::Task{
public:
  AddFeedTask(const FeedSeedPtr & seed ):
  Task(kTaskDbWriterLevel),seed_(seed){
  }
  void handle();
private:
  FeedSeedPtr seed_;
};

class RemoveFeedTask: public Task{
public:
  RemoveFeedTask(const FeedIndexSeq & indexSeq):
    Task(kTaskRemoveFeedLevel),indexSeq_(indexSeq){
  }
  void handle();
private:
  FeedIndexSeq indexSeq_;

};

typedef IceUtil::Handle<RemoveFeedTask> RemoveFeedTaskPtr;
class FeedReplyBufferTimer: public Timer, public MyUtil::Singleton<FeedReplyBufferTimer> {
public:
  FeedReplyBufferTimer() :
     Timer(10000) {
  }
  void ChangeReply(const MyUtil::LongSeq & feeds, const FeedReplyPtr& reply);
  void handle();
private:
  map<Ice::Long, FeedReplyPtr> replys_;
  IceUtil::Mutex mutex_;
};

};
};
#endif 
