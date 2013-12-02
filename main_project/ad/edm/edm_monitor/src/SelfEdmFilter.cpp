#include "../include/SelfEdmFilter.h"

USE_NS(std)
USE_NS(xce::edm)

bool SelfEdmFilter::IsOffline(const EdmInfo& edmInfo){
  Ice::Long id = edmInfo.campaignID;
  double ratio = GetSendRatio(id);
  MCE_DEBUG("SelfEdmFilter::IsOffline "<<id<<"-->ratio:"<<ratio);
  return SelectByRand(ratio);
}

void SelfEdmFilter::CalcSendRatio(const std::map<Ice::Long, DummyRecordPtr> & records){
  MCE_DEBUG("SelfEdmFilter::CalcSendRatio size:"<<records.size());
  std::map<Ice::Long, double>  results;
  double SelfDailyConsumeBudgetLimit = EdmMonitorConfig::instance().GetValue(SELF_DAILY_BUDGET_CONSUME_LIMIT);
  double SelfDailyThreshold = EdmMonitorConfig::instance().GetValue(SELF_DAILY_THRESHOLD);
  double SelfDailyLowerThresholdRatio = EdmMonitorConfig::instance().GetValue(SELF_DAILY_RATIO_LOWER_THRESHOLD);
  double SelfBalanceThreshold = EdmMonitorConfig::instance().GetValue(SELF_BALANCE_THRESHOLD);
  double SelfBalanceLowerThresholdRatio = EdmMonitorConfig::instance().GetValue(SELF_BALANCE_LOWER_RATIO);
  double SelfMinSendRatio = EdmMonitorConfig::instance().GetValue(SELF_MIN_SEND_RATIO);
  double SelfNoQuatoDataRatio = EdmMonitorConfig::instance().GetValue(SELF_NO_QUATODATA_RATIO);

  if(SelfDailyConsumeBudgetLimit == FAILED
		  || SelfDailyThreshold == FAILED
		  || SelfDailyLowerThresholdRatio == FAILED
		  || SelfBalanceThreshold == FAILED
		  || SelfBalanceLowerThresholdRatio == FAILED
		  || SelfMinSendRatio == FAILED
		  || SelfNoQuatoDataRatio == FAILED){
    MCE_WARN("SelfEdmFilter::CalcSendRatio ,finally ratio,Config Data Error,not Config some Data or Config Value = -1 or Error");
	if(SelfNoQuatoDataRatio == FAILED){
	  SelfNoQuatoDataRatio = 0;
	}
  }
  
  std::map<Ice::Long, DummyRecordPtr>::const_iterator its = records.begin();
  for(; its!=records.end(); its++){
    CampaignRecord* campaignRecordPtr = dynamic_cast<CampaignRecord*>(its->second.get());
		if(campaignRecordPtr == NULL){
		  continue;
		}
	  double ratio = 1.0;
	  double ratio1 = GetMinTotalRatio(campaignRecordPtr->dailyQuota_, campaignRecordPtr->dailyAmount_, SelfDailyConsumeBudgetLimit);
	  MCE_DEBUG("SelfEdmFilter::CalcSendRatio (Amount / Quota  >  limit) ,  Ratio:"<<its->first<<" : "<<ratio1);
	  if(ratio > ratio1){
	    ratio = ratio1;
	  }

	  ratio1 = GetMinDailyRatio(campaignRecordPtr->dailyQuota_, campaignRecordPtr->dailyAmount_, SelfDailyThreshold, SelfDailyLowerThresholdRatio);
	  MCE_DEBUG("SelfEdmFilter::CalcSendRatio (Quato - Amount) < threshold, Ratio:"<<its->first<<" : "<<ratio1);
	  if(ratio > ratio1){
	    ratio = ratio1;
	  }

	  ratio1 = GetMemberBalanceRatio(campaignRecordPtr->memberBalance_, SelfBalanceThreshold, SelfBalanceLowerThresholdRatio);
	  MCE_DEBUG("SelfEdmFilter::CalcSendRatio, member Balance  < threshold,Ratio:"<<its->first<<" : "<<ratio1);
	  if(ratio > ratio1){
	    ratio = ratio1;
	  }

    MCE_DEBUG("SelfEdmFilter::CalcSendRatio "<<its->first<<" current:"<<ratio<<" min:"<<SelfMinSendRatio);
	  if(ratio < SelfMinSendRatio){
	    ratio = SelfMinSendRatio;
	  }
	  results[its->first] = ratio;
	  MCE_INFO("SelfEdmFilter::CalcSendRatio ,finally Ratio: "<<its->first<<" : "<<results[its->first]);
  }
  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
  	campaignID2Ratio_.swap(results);
  }
}

double SelfEdmFilter::GetSendRatio(const Ice::Long & campaignID){
  {
    IceUtil::RWRecMutex::RLock lock(mutex_);
    std::map<Ice::Long, double>::iterator it = campaignID2Ratio_.find(campaignID);
    if(it != campaignID2Ratio_.end()){
  	  return it->second;
    }
	}
  double ratio = 0.2;
  ratio = EdmMonitorConfig::instance().GetValue(SELF_NO_QUATODATA_RATIO);
  if(ratio == -1){
	  ratio = 0.2;
	}
  return  ratio;
}
