#include "../include/LeftAdEdmFilter.h"

USE_NS(std)
USE_NS(xce::edm)

bool LeftAdEdmFilter::IsOffline(const EdmInfo& edmInfo){
  Ice::Long id = edmInfo.campaignID;
  double ratio = GetSendRatio(id);
  MCE_DEBUG("LeftAdEdmFilter::IsOffline "<<id<<"-->ratio:"<<ratio);
  double LeftAdMinSendRatio = EdmMonitorConfig::instance().GetValue(LEFT_AD_MIN_SEND_RATIO);
  if (ratio < LeftAdMinSendRatio){
    return true;
  }
  return SelectByRand(ratio);
}

void LeftAdEdmFilter::CalcSendRatio(const std::map<Ice::Long, DummyRecordPtr> & records){
  MCE_DEBUG("LeftAdEdmFilter::CalcSendRatio size:"<<records.size());
  std::map<Ice::Long, double>  results;
  double LeftAdDailyConsumeBudgetLimit = EdmMonitorConfig::instance().GetValue(LEFT_AD_DAILY_BUDGET_CONSUME_LIMIT);
  double LeftAdDailyThreshold = EdmMonitorConfig::instance().GetValue(LEFT_AD_DAILY_THRESHOLD);
  double LeftAdDailyLowerThresholdRatio = EdmMonitorConfig::instance().GetValue(LEFT_AD_DAILY_RATIO_LOWER_THRESHOLD);
  double LeftAdBalanceThreshold = EdmMonitorConfig::instance().GetValue(LEFT_AD_BALANCE_THRESHOLD);
  double LeftAdBalanceLowerThresholdRatio = EdmMonitorConfig::instance().GetValue(LEFT_AD_BALANCE_LOWER_RATIO);

  if(LeftAdDailyConsumeBudgetLimit == FAILED
		  || LeftAdDailyThreshold == FAILED
		  || LeftAdDailyLowerThresholdRatio == FAILED
		  || LeftAdBalanceThreshold == FAILED
		  || LeftAdBalanceLowerThresholdRatio == FAILED){
    MCE_WARN("LeftAdEdmFilter::CalcSendRatio ,finally ratio,Config Data Error,not Config some Data or Config Value = -1 or Error");
  }
  
  std::map<Ice::Long, DummyRecordPtr>::const_iterator its = records.begin();
  for(; its!=records.end(); its++){
    CampaignRecord* campaignRecordPtr = dynamic_cast<CampaignRecord*>(its->second.get());
		if(campaignRecordPtr == NULL){
		  continue;
		}
	  double ratio = 1.0;
	  double ratio1 = GetMinTotalRatio(campaignRecordPtr->dailyQuota_, campaignRecordPtr->dailyAmount_, LeftAdDailyConsumeBudgetLimit);
	  MCE_DEBUG("LeftAdEdmFilter::CalcSendRatio (Amount / Quota  >  limit) ,  Ratio:"<<its->first<<" : "<<ratio1);
	  if(ratio > ratio1){
	    ratio = ratio1;
	  }

	  ratio1 = GetMinDailyRatio(campaignRecordPtr->dailyQuota_, campaignRecordPtr->dailyAmount_, LeftAdDailyThreshold, LeftAdDailyLowerThresholdRatio);
	  MCE_DEBUG("LeftAdEdmFilter::CalcSendRatio (Quato - Amount) < threshold, Ratio:"<<its->first<<" : "<<ratio1);
	  if(ratio > ratio1){
	    ratio = ratio1;
	  }

	  ratio1 = GetMemberBalanceRatio(campaignRecordPtr->memberBalance_, LeftAdBalanceThreshold, LeftAdBalanceLowerThresholdRatio);
	  MCE_DEBUG("LeftAdEdmFilter::CalcSendRatio, member Balance  < threshold,Ratio:"<<its->first<<" : "<<ratio1);
	  if(ratio > ratio1){
	    ratio = ratio1;
	  }

	  results[its->first] = ratio;
	  MCE_INFO("LeftAdEdmFilter::CalcSendRatio ,finally Ratio: "<<its->first<<" : "<<results[its->first]);
  }
  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
  	campaignID2Ratio_.swap(results);
  }
}

double LeftAdEdmFilter::GetSendRatio(const Ice::Long & campaignID){
  {
    IceUtil::RWRecMutex::RLock lock(mutex_);
    std::map<Ice::Long, double>::iterator it = campaignID2Ratio_.find(campaignID);
    if(it != campaignID2Ratio_.end()){
  	  return it->second;
    }
	}
  double ratio = EdmMonitorConfig::instance().GetValue(LEFT_AD_NO_QUATODATA_RATIO); 
  if(ratio == -1){
	  ratio = 0.2;
	}
  return  ratio;
}
