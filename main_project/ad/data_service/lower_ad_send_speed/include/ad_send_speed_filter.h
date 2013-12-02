/*
 * =====================================================================================
 *
 *       Filename:  AdSendSpeedLower.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年08月23日
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lei.yu1 
 *        Company:  renren
 *
 * =====================================================================================
 */

#ifndef __XCE_AD_SEND_SPEED_FILTER__
#define __XCE_AD_SEND_SPEED_FILTER__

#include <map>
#include <vector>
#include "ServiceI.h"
#include "Singleton.h"
#include "LogWrapper.h"
#include "../common/struct.h"
#include "../common/constant.h"
#include "compute_ratio.h"
#include "campaign_compute_ratio.h"
#include "member_compute_ratio.h"
#include <IceUtil/RWRecMutex.h>
#include <IceUtil/Mutex.h>
#include "db_quota_loader.h"
#include "http_quota_loader.h"
#include "ad/indexService/common/src/indexTimer.h"
#include "ad/indexService/common/src/index.h"

namespace xce{
namespace ad{

class AdSendSpeedLower : public MyUtil::Singleton<AdSendSpeedLower>{
public:
  AdSendSpeedLower() : member_last_update_time_(0), campaign_last_update_time_(0), last_clear_time_(0), init_(false){}

  bool IsOffline(AdGroupPtr adgroup_ptr);

  void UpdateData();
  bool Clear();
  
  void Init(const std::string& db); 
protected:
  void InitCampaignIDList(std::vector<Ice::Long>& campaign_id_vec);
  void InitMemberIDList(std::vector<Ice::Long>& member_id_vec);

  double GetValue(std::map<Ice::Long, DummyRecordPtr>& ratio, const Ice::Long id, const std::string& default_value_name);

  bool SelectByRand(double ratio);

  void Copy(const std::map<Ice::Long, DummyRecordPtr>& record, std::map<Ice::Long, DummyRecordPtr>& temp_record);
private:
  std::map<Ice::Long, DummyRecordPtr> campaign_ratio_;
  std::map<Ice::Long, DummyRecordPtr> member_ratio_;
  time_t member_last_update_time_;
  time_t campaign_last_update_time_;
  time_t last_clear_time_;
  IceUtil::RWRecMutex mutex_;
  bool init_;
};//end of class AdSendSpeedLower 

class AdSendSpeedLowerTimer : public IndexTimer{
public:
  AdSendSpeedLowerTimer(void *instance): IndexTimer(instance){}
  virtual void process();
  virtual int interval() const;
};//end of class AdSendSpeedLowerTimer

}//end of namespace ad
}//end of namespace xce

#endif
