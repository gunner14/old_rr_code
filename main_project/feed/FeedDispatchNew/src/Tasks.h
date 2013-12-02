#ifndef TASKS_H_
#define TASKS_H_

#include "TaskManager.h"
#include "RFeed.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>

namespace xce {
namespace feed {

const int WRITEMEMC_TASK_LEVEL = 10001;
const int REPLY_TASK_LEVEL = 10002;
const int REMOVE_TASK_LEVEL = 10003;
const int REMOVE_BY_TIME_TASK_LEVEL = 10005;
const int DISPATCH_TASK_LEVEL = 10006;
const int DISPATCH_BIG_FANST_FEED_ASK_LEVEL = 10007;

using namespace MyUtil;
using boost::multi_index_container;
using namespace boost::multi_index;

class ConfigCache: public MyUtil::Singleton<ConfigCache> {
public:
  ConfigCache() :
    kSize_(100000) {
  }
  void Add(const Ice::Long feed_id, const map<string, string> & config);
  map<string, string> Get(const Ice::Long feed_id);
  size_t Size();
private:
  void Kick();
  string PrintHelper();

  /////////////////////////////////////////////////////////////////////////
  struct Item: public IceUtil::Shared {
    Ice::Long feed_id;
    map<string, string> config;
  };
  typedef IceUtil::Handle<Item> ItemPtr;

  struct seqtag {
  };
  struct feedtag {
  };

  typedef multi_index_container<ItemPtr, indexed_by<sequenced<tag<seqtag> > ,
      hashed_unique<tag<feedtag> ,
          BOOST_MULTI_INDEX_MEMBER(Item, Ice::Long,feed_id)> > > ConfigPool;

  typedef ConfigPool::index<seqtag>::type SIndex;
  typedef ConfigPool::index<feedtag>::type FIndex;
  /////////////////////////////////////////////////////////////////////////

  ConfigPool pool_;
  IceUtil::RWRecMutex mutex_;
  const int kSize_; //cache��С
};

class InteractionUploader : public MyUtil::Singleton<InteractionUploader> {
public :
  //void UploadReply(long fid, int r);      
	void UploadReply(const FeedIndexPtr & idx,int r);
  void UploadReply(long fid,int r); 
  void UploadRange(long fig,int r);
}; 



////////////////////////////////////////////////////////////////////////
class RemoveFeedByTimeTask: public MyUtil::Task {
public:
  RemoveFeedByTimeTask(Ice::Long source, int stype, Ice::Int actor, Ice::Int time) :
    Task(REMOVE_BY_TIME_TASK_LEVEL),  source_(source), stype_(stype), actor_(actor), _time(time) {
  }
  ;

  virtual void handle();
private:
  Ice::Long source_;
  Ice::Int stype_;
  Ice::Int actor_;
  Ice::Int _time;
};

class RemoveFeedByIdTask: public MyUtil::Task {
public:
  RemoveFeedByIdTask(Ice::Long feedId) :
    Task(REMOVE_TASK_LEVEL), _feedId(feedId) {
  }
  ;

  virtual void handle();
private:
  Ice::Long _feedId;
};

class RemoveFeedBySourceStypeTask: public MyUtil::Task {
public:
  RemoveFeedBySourceStypeTask(Ice::Long source, int stype) :
    Task(REMOVE_TASK_LEVEL), source_(source), stype_(stype) {
  }
  ;
  virtual void handle();
private:
  Ice::Long source_;
  int stype_;
};

class RemoveFeedBySourceStypeActorTask: public MyUtil::Task {
public:
  RemoveFeedBySourceStypeActorTask(Ice::Long source, int stype, int actor) :
    Task(REMOVE_TASK_LEVEL), source_(source), stype_(stype), actor_(actor) {
  }
  ;
  virtual void handle();

private:
  Ice::Long source_;
  int stype_, actor_;
};

class RemoveFeedByParentTask: public MyUtil::Task {
public: RemoveFeedByParentTask(Ice::Long psource, int ptype) :
    Task(REMOVE_TASK_LEVEL), psource_(psource), ptype_(ptype) {
  }
  ;
  virtual void handle();

private:
  Ice::Long psource_;
  int ptype_;
};

class  FeedDispatchTask: public MyUtil::Task {
public:
  FeedDispatchTask(const FeedSeedPtr & seed,const FeedReplyPtr & reply = 0,const FeedConfigPtr & config = 0):
    Task(DISPATCH_TASK_LEVEL),seed_(seed),reply_(reply),config_(config) {
    }
  void WriteDataToMemc();
  void CheckSend2School(const FeedConfigPtr& config);
  void MakeFeedConfig(FeedConfigPtr & config);
  void MakeFeedIndex(FeedIndexPtr & index);
  void MakeFeedData(const FeedConfigPtr& config,FeedDataPtr & data);
  void Deliver2Group(const FeedConfigPtr &config);
  void UpdateReply2Memc(ReplyDataPtr & replyData);
  void HandlePhoto();
  void DispatchReply();
  void DispatchFeed();
  int GetAppId();
  map<int,int> GetBayesScores(int actor,const set<int> &target,int stype);
  virtual void handle();
private:
  FeedSeedPtr seed_;
  FeedReplyPtr reply_;  
  FeedConfigPtr config_;
};
class DispatchBigFansSizeFeedTask: public Task {
public:
  DispatchBigFansSizeFeedTask(FeedSeedPtr seed,FeedConfigPtr config,
      const set<int> & target):
    Task(DISPATCH_BIG_FANST_FEED_ASK_LEVEL),seed_(seed),config_(config),target_(target) {
    
    }
  void handle();
private:
  FeedSeedPtr seed_;
  FeedConfigPtr config_;
  set<int> target_;
};

class DispatchReplyToIM {
public:
  DispatchReplyToIM(const ReplyDataPtr & reply,const FeedConfigPtr & config):
  reply_(reply), config_(config){   }
  void handle();
private:
  ReplyDataPtr reply_;
  FeedConfigPtr config_;
};

class ReplyDelayDispatchTimer: public MyUtil::Timer {
public:
  ReplyDelayDispatchTimer(const ReplyDataPtr& reply,
      const FeedConfigPtr& config) :
    Timer(2000), reply_(reply), config_(config) {
  }
  ;
  virtual void handle();

private:
  ReplyDataPtr reply_;
  FeedConfigPtr config_;
};

class ReplyDispatchTask: public MyUtil::Task {
public:
  ReplyDispatchTask(Ice::Long source, int stype, int actor,
      const FeedReplyPtr& reply) :
    Task(REPLY_TASK_LEVEL), source_(source), stype_(stype), actor_(actor), reply_(reply) {
  }
  ;
  virtual void handle();
  bool SetSomeConfig(FeedIndexPtr & idx,FeedConfigPtr & config,map<string, string> & props);
private:
  Ice::Long source_;
  int stype_, actor_;
  FeedReplyPtr reply_;
};

class ReplaceXMLTask: public MyUtil::Task {
public:
  ReplaceXMLTask(Ice::Long source, int stype, int actor, const string & xml) :
    Task(), source_(source), stype_(stype), actor_(actor), _xml(xml) {
  }
  ;
  virtual void handle();
private:
  Ice::Long source_;
  int stype_, actor_;
  string _xml;
};

}
}

#endif /* TASKS_H_ */
