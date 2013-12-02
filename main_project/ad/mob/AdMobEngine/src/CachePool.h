/*
 *  CachePool.h
 *
 *   Created on: 2012-3-31
 *       Author: yongjun.yin
 *  Description: 定义 数据缓存基类 CachePool,以及引擎中业务需要的Pool
 */

#ifndef __AD_MOB_CACHE_POOL_H__
#define __AD_MOB_CACHE_POOL_H__

#include <map>
#include <string>
#include <boost/lexical_cast.hpp>
#include <IceUtil/Mutex.h>
#include <IceUtil/RWRecMutex.h>

#include "Struct.h"
#include "QueryRunner.h"
#include "DbDescriptor.h"
#include "Singleton.h"
#include "TaskManager.h"

using namespace std;
using namespace boost;
using namespace xce::ad;

namespace xce{
namespace ad{

const long ONE_MINUTE = 60*1000*1000;		 //1min

class CachePool {
public:
  virtual ~CachePool(){}
  virtual void Initialize(const string& DbInstance, const Ice::Long updateInterval);
  //virtual AdStructPtr GetValue(Ice::Long)=0;
  virtual size_t Size() {
		return 0;
  }
  virtual string Print(int size) {
		return "Empty";
  }
protected:
  virtual void TotalLoad(){}
  virtual void IncLoad() { 
		TotalLoad();
  }
  void SetDB(string DbInstance) {
    DbInstance_ = DbInstance;
  }
  string GetDB() {
    return DbInstance_;
  }
  bool Is_Init_Ok() {
    return init_ok_;
  }

  class InitTask : virtual public MyUtil::Task {
  public:  
    InitTask(CachePool* parent, Ice::Long updateInterval = ONE_MINUTE ) : updateInterval_(updateInterval) {
      parent_ = parent;
    }
    virtual void handle();
  private:
    Ice::Long updateInterval_;
    CachePool* parent_;
  };

  class ReloadTask : virtual public MyUtil::Task {
  public:
    ReloadTask(CachePool* parent, Ice::Long updateInterval = ONE_MINUTE ) : updateInterval_(updateInterval) {
      parent_ = parent;
    }
    virtual void handle();
  private:
    Ice::Long updateInterval_;
    CachePool* parent_;
  };

  string DbInstance_;
  Ice::Long updateInterval_;
  IceUtil::RWRecMutex mutex_;
  bool init_ok_;

};

class MemberPool : public CachePool, public MyUtil::Singleton<MemberPool> {
public:
  AdMemberPtr GetAdMember(Ice::Long member_id);
  size_t Size();
  string Print(const int size);
private:
  void TotalLoad();
  map<Ice::Long, AdMemberPtr> pool_;
};

class CampaignPool : public CachePool, public MyUtil::Singleton<CampaignPool> {
public:
  AdCampaignPtr GetAdCampaign(Ice::Long campaign_id);
  size_t Size();
  string Print(const int size);
private:
  void TotalLoad();
  map<Ice::Long, AdCampaignPtr> pool_;
};

class GroupPool : public CachePool, public MyUtil::Singleton<GroupPool> {
public:
  AdGroupPtr GetAdGroup(Ice::Long group_id);
  map<Ice::Long, vector<AdGroupPtr> > GetRotateAds();
  size_t Size();
  string Print(const int size);
private:
  class PointsChecker {
  public:
    static bool Check(const string & day_points, const string & hour_points);
    static bool CheckHour(const string & hour_point_str);
    static bool CheckDay(const string & day_point_str);
  };
  void TotalLoad();
  
  void TotalLoadAdGroup(map<Ice::Long, AdGroupPtr>& local_pool);
  void TotalLoadAdCreative(map<Ice::Long, AdGroupPtr>& local_pool);
  void FilterNoCreativeAdGroups(map<Ice::Long, AdGroupPtr>& local_pool);
  
  void TotalLoadAdGroupAdZoneRef();

  map<Ice::Long, AdGroupPtr> pool_;
  map<Ice::Long, vector<AdGroupPtr> > zone2rotate_;
};

class ZonePool : public CachePool, public MyUtil::Singleton<ZonePool> {
public:
  AdZonePtr GetAdZone(Ice::Long);
  bool IsValid(Ice::Long);
  size_t Size();
  string Print(const int size);
private:
  inline void FillRotateGroups();
  void TotalLoad();
  map<Ice::Long, AdZonePtr> pool_;
};

class UserBindPool : public CachePool, public MyUtil::Singleton<UserBindPool> {
public:
  AdUserBindPtr GetUserBind(const int user_id, const Ice::Long zone_id);
	size_t Size();
	string Print(const int size);
private:
  void TotalLoad();
  set<AdUserBindPtr> pool_;
};

class DiscountPool : public CachePool, public MyUtil::Singleton<DiscountPool> {
public:
	double GetDiscountRatio(const Ice::Long& member_id);
	string Print(const int size);
	size_t Size();
private:
	void TotalLoad();
  map<Ice::Long, double> pool_;
};

}
}
#endif
