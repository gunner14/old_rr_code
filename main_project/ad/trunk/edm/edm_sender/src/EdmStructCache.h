/*
 * EdmStructCache.h
 *
 *  Created on: 2012-2-21
 *      Author: sunzz
 */

#ifndef EDMSTRUCTCACHE_H_
#define EDMSTRUCTCACHE_H_
#include "EdmStruct.h"

const static int zone_reload_timestamp = 1 * 60 * 1000;
using namespace com::xiaonei::xce;
using namespace xce::ad;


class AdMemberPool : public MyUtil::Singleton<AdMemberPool> {
public:
  void Init();
  AdMemberPtr GetAdMember(Ice::Long member_id);
  class UpdateCheckTimer : public MyUtil::Timer {
  public:
    UpdateCheckTimer() :
      Timer(1 * 60 * 1000) {
    }
    virtual void handle();
  };
private:
  map<Ice::Long, AdMemberPtr> member_pool_;
  IceUtil::RWRecMutex mutex_;
};

class AdCampaignPool : public MyUtil::Singleton<AdCampaignPool> {
public:
  void Init();
  AdCampaignPtr GetAdCampaign(Ice::Long campaign_id);
  class UpdateCheckTimer : public MyUtil::Timer {
  public:
    UpdateCheckTimer() :
      Timer(1 * 60 * 1000) {
    }
    ;
    virtual void handle();
  };
private:
  map<Ice::Long, AdCampaignPtr> campaign_pool_;
  IceUtil::RWRecMutex mutex_;

};

class AdGroupPool : public MyUtil::Singleton<AdGroupPool> {
public:
  void Init();
  AdGroupPtr GetAdGroup(Ice::Long group_id);
  set<AdGroupPtr> GetAdGroupsByIds(const vector<Ice::Long>& gids);

  //void UpdateCampainStatus();
  class UpdateCheckTimer : public MyUtil::Timer {
  public:
    UpdateCheckTimer() :
      Timer(10 * 60 * 1000) {
    }
    virtual void handle();
  };
  /*class UpdateCampaignStatusTimer : public MyUtil::Timer {
  public:
    UpdateCampaignStatusTimer() :
      Timer(1 * 60 * 1000) {
    }
    virtual void handle();
  };*/
private:
  hash_map<Ice::Long, AdGroupPtr> group_pool_;

  IceUtil::RWRecMutex mutex_;

  void LoadAdGroup(hash_map<Ice::Long, AdGroupPtr> & group_pool);
  void LoadCreative(hash_map<Ice::Long, AdGroupPtr> & group_pool);
  void CheckIfHaveCreative(hash_map<Ice::Long, AdGroupPtr> & group_pool); // adgroup's has creative check
};

class SchoolPool : public MyUtil::Singleton<SchoolPool> {
public:
  void Init();
  uint64_t GetSchoolAreaCode(Ice::Long school);
private:
  map<Ice::Long, uint64_t> pool_;
  IceUtil::RWRecMutex mutex_;
};



/***********************************************************************************************************************/
class PointsChecker : public MyUtil::Singleton<PointsChecker> {
public:
  bool Check(const string & day_points, const string & hour_points);
private:
  bool CheckHour(const string & day_points);
  bool CheckDay(const string & hour_points);
};

/***********************************************************************************************************************/



#endif /* EDMSTRUCTCACHE_H_ */
