
#ifndef __FEEDITEMBLCOKCACHEI_H__
#define __FEEDITEMBLOCKCACHEI_H__

#include <set>
#include <vector>
#include <asm/atomic.h>
#include "ServiceI.h"
#include "Singleton.h"
#include <TaskManager.h>
#include "QueryRunner.h"
#include "ObjectCacheI.h"
#include "IceUtil/IceUtil.h"
#include "IceUtil/Mutex.h"
#include "FeedItemBlockCache.h"

namespace xce {
namespace feed {

using namespace xce::feed;
using namespace std;
using namespace MyUtil;

const Ice::Int USER_BLOCK_TYPE = 0;
const Ice::Int USER_BLOCK_USER = 1;
const Ice::Int USER_BLOCK_FEED = 2;
const Ice::Int MAX_READ_FEED   = 3;

const string INSTANCE_NAME = "user_block_feed";

const string TableUserBlockFeedPre = "user_block_feed_";

const int TableUserBlockFeedCount = 100;

const string UBF_COLUMN_USERID     = "userid";
const string UBF_COLUMN_BLOCKFEED  = "feedid";
const string UBF_COLUMN_TIME       = "time"; 

//const int DURATION_TIME = 30 * 24 * 60 * 60;
//15 days
const int DURATION_TIME = 15 * 24 * 60 * 60;
const int UBF_LOAD_LIMIT = 1000;
//const int UBF_LOAD_LIMIT = 10;
const int UBF_LOAD_EVICT = 100;



//-------------------------------------------------------------------------------------
// TimeStat
class TimeStat{
  timeval _begin, _end;
  bool _log;
  public:
  TimeStat(bool log=false){
    _log = log;
    reset();
  }
  ~TimeStat(){
    if(_log){
      MCE_INFO("######### " << getTime());
    }
  }
  void reset(){
    gettimeofday(&_begin, NULL);
  }
  float getTime(){
    gettimeofday(&_end, NULL);
    float timeuse=1000000*(_end.tv_sec-_begin.tv_sec)+_end.tv_usec
      -_begin.tv_usec;
    timeuse/=1000;
    return timeuse;
  }
};
//-------------------------------------------------------------------------------------
// UserBlockType
class UserBlockType : public ::IceUtil::Mutex ,
  public ::Ice::Object {
    public:
      void put(int type) {
        IceUtil::Mutex::Lock lock(*this);
        types_.insert(type);
      }

      void del(int type) {
        IceUtil::Mutex::Lock lock(*this);
        types_.erase(type);
      }

      bool has(int type) {
        IceUtil::Mutex::Lock lock(*this);
        return types_.find(type) != types_.end();
      }

      std::vector<int> get() {
        IceUtil::Mutex::Lock lock(*this);
        std::vector<int> result(types_.begin(), types_.end());
        return result;
      }
    private:
      std::set<int> types_;
  };
typedef IceUtil::Handle<UserBlockType> UserBlockTypePtr;

class UserBlockTypeFactoryI : public MyUtil::ServantFactory {
  public:
    UserBlockTypeFactoryI() {}
    virtual ~UserBlockTypeFactoryI() {}

    virtual Ice::ObjectPtr create(Ice::Int userid);
};


//-------------------------------------------------------------------------------------
// UserBlockUser
class UserBlockUser : public ::IceUtil::Mutex ,
  public ::Ice::Object {
    public:
      void put(int id) {
        IceUtil::Mutex::Lock lock(*this);
        ids_.insert(id);
      }

      void del(int id) {
        IceUtil::Mutex::Lock lock(*this);
        ids_.erase(id);
      }

      bool has(int id) {
        IceUtil::Mutex::Lock lock(*this);
        return ids_.find(id) != ids_.end();
      } 
      std::vector<int> get() {
        IceUtil::Mutex::Lock lock(*this);
        std::vector<int> result(ids_.begin(), ids_.end());
        return result;
      }
    private:

      std::set<int> ids_;
  };
typedef IceUtil::Handle<UserBlockUser> UserBlockUserPtr;

class UserBlockUserFactoryI : public MyUtil::ServantFactory {
  public:
    UserBlockUserFactoryI() {}
    virtual ~UserBlockUserFactoryI() {}

    virtual Ice::ObjectPtr create(Ice::Int userid);
};


//-------------------------------------------------------------------------------------
// UserBlockFeed
// 若设置过全部已读，则集合的首元素存储的是已读的最大feedid(负数表示)
class UserBlockFeed : public ::IceUtil::Mutex, 
  public ::Ice::Object {
    public:
      long put(Ice::Long feedid, bool is_evict) {
        IceUtil::Mutex::Lock lock(*this);

        long result = evict(is_evict);
        feedids_.insert(feedid);
        //MCE_DEBUG("UserBlockFeed::put, feedid: " << feedid);

        //for (std::set<long>::const_iterator it = feedids_.begin();
        //		it != feedids_.end();++it) {
        //	MCE_DEBUG("put remain key: " << *it);
        //}
        return result;
      } 

      void del(Ice::Long feedid) {
        IceUtil::Mutex::Lock lock(*this);
        feedids_.erase(feedid);
      }

      bool has(Ice::Long feedid) {
        IceUtil::Mutex::Lock lock(*this);
        return feedids_.find(feedid) != feedids_.end();
      }

      Ice::Long getMaxReadFeed() {
        IceUtil::Mutex::Lock lock(*this);
        if (!feedids_.empty() && *(feedids_.begin()) < 0) 
          return -*(feedids_.begin());
        return 0;
      }

      void updateMaxReadFeed(Ice::Long feedid) {
        if (feedid <= 0) return;
        IceUtil::Mutex::Lock lock(*this);

        while (!feedids_.empty() && *(feedids_.begin()) <= feedid) {
          feedids_.erase(feedids_.begin());
        }

        if (feedid) {
          feedids_.insert(feedids_.begin(), feedid > 0 ? -feedid : feedid);
        }
      }

      std::vector<Ice::Long> get() {
        IceUtil::Mutex::Lock lock(*this);
        //for (std::set<long>::const_iterator it = feedids_.begin();
        //		it != feedids_.end();++it) {
        //	MCE_DEBUG("get remain key: " << *it);
        //}

        if (!feedids_.empty() && *(feedids_.begin()) <= 0)
          return (std::vector<Ice::Long>(++feedids_.begin(), feedids_.end()));
        else
          return (std::vector<Ice::Long>(feedids_.begin(), feedids_.end()));
      }

      size_t size() const {
        return feedids_.size();
      }
    private:
      inline long evict(bool is_evict) {
        if (feedids_.size() >= MaxSize_ && is_evict) {
          long max_read_feedid = 0;
          int count = 0;

          std::set<long>::const_iterator it;
          for (it = feedids_.begin();it != feedids_.end() && count < UBF_LOAD_EVICT; ++it) {
            max_read_feedid = std::max(max_read_feedid, abs(*it));
            count ++;
          }

          feedids_.erase(feedids_.begin(), --it);

          //for (std::set<long>::const_iterator it = feedids_.begin();
          //		it != feedids_.end();++it) {
          //	MCE_DEBUG("evict remain key: " << *it);
          //}

          return max_read_feedid;
        }


        /*
           while (feedids_.size() >= MaxSize_) {
           long max_read_feedid = 0;

           if (*feedids_.begin() <= 0) {

           long tmp = -(*(feedids_.begin()));
           feedids_.erase(feedids_.begin());
           max_read_feedid = std::max(tmp, *(feedids_.begin()));
           feedids_.erase(feedids_.begin());

           for (std::set<long>::const_iterator it = feedids_.begin();
           it != feedids_.end();++it) {
           MCE_DEBUG("evict remain key: " << *it);
           }
           } else {
           max_read_feedid = *(feedids_.begin());
           feedids_.erase(feedids_.begin());
           MCE_DEBUG("evict > 0, begin: " << *(feedids_.begin()) << " max_read_feedid: " << max_read_feedid);
           }
           return max_read_feedid;
           }*/
        return 0;
      }
      std::set<Ice::Long> feedids_;
      static const size_t MaxSize_ = UBF_LOAD_LIMIT;
  };
typedef IceUtil::Handle<UserBlockFeed> UserBlockFeedPtr;

class UserBlockFeedFactoryI : public MyUtil::ServantFactory {
  public:
    UserBlockFeedFactoryI() {}
    ~UserBlockFeedFactoryI() {}

    virtual Ice::ObjectPtr create(Ice::Int userid);
};

//-------------------------------------------------------------------------------------
// ReloadTask
class ReloadTask : public IceUtil::Thread, public Singleton<ReloadTask> {
  public:
    void put(Ice::Int userid) {
      IceUtil::Monitor<IceUtil::Mutex>::Lock lock(mutex_);
      if (suserids_.find(userid) != suserids_.end()) {
        return;
      }
      userids_.push_back(userid);

      if (userids_.size() >= batchSize_) {
        mutex_.notify();
        return;
      }

      suserids_.insert(userid);
    }

    void run() {
      while (true) {
        try {
          vector<Ice::Int> tmp;
          {
            IceUtil::Monitor<IceUtil::Mutex>::Lock lock(mutex_);
            if (userids_.size() < batchSize_)
              mutex_.timedWait(IceUtil::Time::seconds(batchTime_)); 
            userids_.swap(tmp);
            suserids_.clear();
          }

          if (!tmp.empty()) {
            MCE_DEBUG("ReloadTask::run, reload size: " << tmp.size());
            for (size_t i = 0; i < tmp.size(); i++) {
              ServiceI::instance().reloadObject(USER_BLOCK_FEED, tmp[i]);
            }
          }
        } catch(...) {
          MCE_WARN("BlockFeedSaver has exception!");
        }
      }
    }
  private:
    vector<Ice::Int> userids_;
    set<Ice::Int>   suserids_;

    IceUtil::Monitor<IceUtil::Mutex> mutex_;
    const static size_t batchSize_ = 10;
    const static size_t batchTime_ = 1;
};

//-------------------------------------------------------------------------------------
// FeedItemBlockCacheI
class FeedItemBlockCacheI;
typedef IceUtil::Handle<FeedItemBlockCacheI> FeedItemBlockCacheIPtr;


class FeedItemBlockCacheI : public FeedItemBlockCache,
  public MyUtil::Singleton<FeedItemBlockCacheI>{
    public:
      FeedItemBlockCacheI();
      ~FeedItemBlockCacheI();

      virtual IntSeq getUserBlockTypes(Ice::Int userid, const Ice::Current& current=::Ice::Current());
      virtual void addUserBlockType(Ice::Int userid, Ice::Int block_type, const Ice::Current & current=::Ice::Current());
      virtual void removeUserBlockType(Ice::Int userid, Ice::Int block_type, const Ice::Current & current=::Ice::Current());
      virtual bool hasUserBlockType(Ice::Int userid, Ice::Int block_type, const Ice::Current & current=::Ice::Current());

      virtual MyUtil::IntSeq getUserBlockUsers(Ice::Int userid, const Ice::Current& current=::Ice::Current());
      virtual void addUserBlockUser(Ice::Int userid, Ice::Int block_userid, const Ice::Current& current=::Ice::Current());
      virtual void removeUserBlockUser(Ice::Int userid, Ice::Int block_userid, const Ice::Current& current=::Ice::Current());
      virtual bool hasUserBlockUser(Ice::Int userid, Ice::Int block_userid, const Ice::Current& current=::Ice::Current());

      virtual MyUtil::LongSeq getUserBlockFeeds(Ice::Int userid, const Ice::Current & current=::Ice::Current());
      virtual void addUserBlockFeed(Ice::Int userid, Ice::Long block_feedid, const Ice::Current& current=::Ice::Current());
      virtual void removeUserBlockFeed(Ice::Int userid, Ice::Long block_feedid, const Ice::Current& current=::Ice::Current());
      virtual bool hasUserBlockFeed(Ice::Int userid, Ice::Long block_feedid, const Ice::Current& current=::Ice::Current());

      virtual void        setMaxReadFeed(Ice::Int userid, Ice::Long feedid, const Ice::Current& current=::Ice::Current());
      virtual Ice::Long   getMaxReadFeed(Ice::Int userid, const Ice::Current& current=::Ice::Current());

      virtual UserBlockInfoPtr getAll(Ice::Int userid, const Ice::Current& current=::Ice::Current());

      // reload
      virtual void reloadUserBlockTypes(Ice::Int userid, const Ice::Current& current=::Ice::Current());
      virtual void reloadUserBlockUsers(Ice::Int userid, const Ice::Current& current=::Ice::Current());
      virtual void reloadUserBlockFeeds(Ice::Int userid, const Ice::Current& current=::Ice::Current());
      virtual void reloadAll(Ice::Int userid, const Ice::Current& current=::Ice::Current());
      //create  object for ObjectCache use by getUserBlockFeeds
      UserBlockFeedPtr createObjectAndInsertToObjectCache(Ice::Int userid);
    private:
      set<int> noBlockUsers;
      IceUtil::RWRecMutex rwMutex_;
  };
}
}
#endif
