/*
 * =====================================================================================
 *
 *       Filename:  FeedItemWrapperI.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011年11月10日 12时04分52秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  guanghe.ge (ggh), guanghe.ge@opi-corp.com
 *        Company:  XiaoNei
 *
 * =====================================================================================
 */

#ifndef __FEEDITEMWRAPPER_I_H__
#define __FEEDITEMWRAPPER_I_H__

#include <set>
#include <vector>
#include "UserFeeds.h"
#include "Singleton.h"
#include <TaskManager.h>
#include "QueryRunner.h"
#include "ObjectCacheI.h"
#include "IceUtil/IceUtil.h"
#include "IceUtil/Mutex.h"
#include "FeedItemBlockCacheReplicaAdapter.h"

namespace xce {
namespace feed {

using namespace xce::feed;
using namespace std;

//zhi.lin.begin
const Ice::Int  EXPIRED_TIME    = 1 * 24 * 60 * 60;

// Wrapper cache
const Ice::Int PAGEMINISITE_CATEGORY = 1;
class PageMinisiteObject : public ::Ice::Object 
{
public:
    PageMinisiteObject():time_(time(NULL)){}
    void getPages(std::vector<int> & v){
        v=pages_;
    }
    void setPages(std::vector<int> & v) {
        pages_=v;
    }
    void getMinisites(std::vector<int> & v) {
        v=minisites_;
    }
    void setMinisites(std::vector<int> & v) {
        minisites_=v;
    }
    int getCreateTime() {
        return time_;
    }
    void setCreateTime(int i) {
        time_=i;
    }
private:
    int time_;
    std::vector<int> pages_;
    std::vector<int> minisites_;
    friend class PageMinisiteObjectFactoryI;
};
typedef IceUtil::Handle<PageMinisiteObject> PageMinisiteObjectPtr;
class PageMinisiteObjectFactoryI : public MyUtil::ServantFactory {
  public:
    PageMinisiteObjectFactoryI() {}
    virtual ~PageMinisiteObjectFactoryI() {}
    virtual Ice::ObjectPtr create(Ice::Int userid);
};

//-----------------------------------------------------------------------------
// ReloadTask
class ReloadTask: public IceUtil::Thread, public Singleton<ReloadTask> {
public:
    void put(Ice::Int uid);
    virtual void run();

private:
    set<Ice::Int> sids_;

    IceUtil::Monitor<IceUtil::Mutex> mutex_;
    const static size_t batchSize_ = 20;
    const static time_t batchTime_ = 2;
};
//zhi.lin.end


//const int TYPE[] = {502, 103, 102, 110, 701, 702, 2002, 104, 708, 2401, 8190, 709, 601, 107};
const int TYPE[] = {100, 500, 600, 700, 800, 900, 1800, 1900, 2000, 2400, 3700, 8100, 8200, 8600};

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

class FeedItemWrapperI : public FeedItemWrapper, public MyUtil::Singleton<FeedItemWrapperI> {
  public:
    FeedItemWrapperI();
    ~FeedItemWrapperI();

    virtual FeedItemSeq getItemSeq(::Ice::Int userid, ::Ice::Int begin, ::Ice::Int limit, const ::Ice::Current& = ::Ice::Current());

    virtual FeedMetaSeq getLessThanFeedid(::Ice::Int userid, ::Ice::Long feedid, ::Ice::Int limit, const ::Ice::Current& = ::Ice::Current());
    virtual FeedMetaSeq get(::Ice::Int userid, ::Ice::Int begin, ::Ice::Int limit, const ::Ice::Current& = ::Ice::Current());

    virtual FeedMetaSeq getLessThanFeedidByTypes(::Ice::Int userid, ::Ice::Long feedid, ::Ice::Int limit, 
            const MyUtil::IntSeq& types, const ::Ice::Current& = ::Ice::Current());
    virtual FeedMetaSeq getByTypes(::Ice::Int userid, ::Ice::Int begin, ::Ice::Int limit, 
            const MyUtil::IntSeq& types, const ::Ice::Current& = ::Ice::Current());
    virtual FeedMetaSeq getByTypesWithFriends(::Ice::Int userid, const MyUtil::IntSeq& friends, ::Ice::Int begin, ::Ice::Int limit, 
            const MyUtil::IntSeq& types, const ::Ice::Current& = ::Ice::Current());

    virtual FeedMetaSeq GetFeeds(::Ice::Int userid, ::Ice::Int begin, ::Ice::Int limit, const MyUtil::IntSeq& types, const ::Ice::Current& = ::Ice::Current());
    virtual FeedMetaSeq GetFeedsByGroupId(int userid, int group, int begin, int limit, const MyUtil::IntSeq& types, const ::Ice::Current& = ::Ice::Current());
    virtual FeedMetaSeq GetFeedsByGroupName(int userid, const string& name, int begin, int limit, const MyUtil::IntSeq& types, const ::Ice::Current& = ::Ice::Current());
    virtual FeedMetaSeq GetFeedsByFocus(int userid, int begin, int limit, const MyUtil::IntSeq& types, const ::Ice::Current& = ::Ice::Current());

    virtual LongSeq GetFriendsFeedsList(int userid, const ::Ice::Current& = ::Ice::Current());

  private:
    void GetFeedMetasImpl(int userid, const IntSeq & friends, int begin, int limit, const IntSeq & types, FeedMetaSeq & result);
    void GetFeedMetasImplSimple(int userid, const IntSeq & friends, int begin, int limit, const IntSeq & types, FeedMetaSeq & result);

    MyUtil::IntSeq getFixedType();

    void getPages(int user, vector<int> & v);
    void getMinisites(int user, vector<int> & v);
    MyUtil::StrSeq createFilter(int user, const MyUtil::IntSeq ntypes);
    void getFilterAndMaxReadFeedid(Ice::Int userid, StrSeq& filters, Ice::Long& maxReadFeedid);
};

}
}

#endif
