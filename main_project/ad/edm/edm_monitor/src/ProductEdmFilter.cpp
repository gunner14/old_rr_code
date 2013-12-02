#include "../include/ProductEdmFilter.h"

USE_NS(std)
USE_NS(xce::edm)

bool ProductEdmFilter::IsOffline(const EdmInfo& edmInfo){
  Ice::Long id = edmInfo.campaignID;
  double ratio = GetSendRatio(id);
  MCE_DEBUG("ProductEdmFilter::IsOffline "<<id<<"-->ratio:"<<ratio);
  return SelectByRand(ratio);
}

void ProductEdmFilter::CalcSendRatio(const std::map<Ice::Long, DummyRecordPtr> & records){
  MCE_DEBUG("ProductEdmQuotaLoader::CalcSendRatio size:"<<records.size());
  std::map<Ice::Long, double>  results;
  double ProductDailyConsumeBudgetLimit = EdmMonitorConfig::instance().GetValue(PRODUCT_DAILY_BUDGET_CONSUME_LIMIT);
  double ProductDailyThreshold = EdmMonitorConfig::instance().GetValue(PRODUCT_DAILY_THRESHOLD);
  double ProductDailyLowerThresholdRatio = EdmMonitorConfig::instance().GetValue(PRODUCT_DAILY_RATIO_LOWER_THRESHOLD);
  double ProductBalanceThreshold = EdmMonitorConfig::instance().GetValue(PRODUCT_BALANCE_THRESHOLD);
  double ProductBalanceLowerThresholdRatio = EdmMonitorConfig::instance().GetValue(PRODUCT_BALANCE_LOWER_RATIO);
  double ProductMinSendRatio = EdmMonitorConfig::instance().GetValue(PRODUCT_MIN_SEND_RATIO);
  double ProductNoQuatoDataRatio = EdmMonitorConfig::instance().GetValue(PRODUCT_NO_QUATODATA_RATIO);

  if(ProductDailyConsumeBudgetLimit == FAILED
		  || ProductDailyThreshold == FAILED
		  || ProductDailyLowerThresholdRatio == FAILED
		  || ProductBalanceThreshold == FAILED
		  || ProductBalanceLowerThresholdRatio == FAILED
		  || ProductMinSendRatio == FAILED
		  || ProductNoQuatoDataRatio == FAILED){
	MCE_WARN("ProductEdmFilter::CalcSendRatio ,finally ratio,Config Data Error,not Config some Data or Config Value is -1 or Error");
	if(ProductNoQuatoDataRatio == FAILED){
	  ProductNoQuatoDataRatio = 0;
	}
  }
  std::map<Ice::Long, DummyRecordPtr>::const_iterator its = records.begin();
  for(; its!=records.end(); its++){
	  CampaignRecord* campaignRecordPtr = dynamic_cast<CampaignRecord*>(its->second.get());
		if(campaignRecordPtr == NULL){
		  continue;
		}
	  double ratio = 1.0;
	  double ratio1 = GetMinTotalRatio(campaignRecordPtr->dailyQuota_, campaignRecordPtr->dailyAmount_, ProductDailyConsumeBudgetLimit);
	  MCE_DEBUG("ProductEdmFilter::CalcSendRatio Amount/Quota < limit ratio:"<<its->first<<" : "<<ratio1);
	  if(ratio > ratio1){
	    ratio = ratio1;
	  }

	  ratio1 = GetMinDailyRatio(campaignRecordPtr->dailyQuota_, campaignRecordPtr->dailyAmount_, ProductDailyThreshold, ProductDailyLowerThresholdRatio);
	  MCE_DEBUG("ProductEdmFilter::CalcSendRatio Quota-Amount < threshold, Ratio:"<<its->first<<" : "<<ratio1);
	  if(ratio > ratio1){
	    ratio = ratio1;
	  }

  	ratio1 = GetMemberBalanceRatio(campaignRecordPtr->memberBalance_, ProductBalanceThreshold, ProductBalanceLowerThresholdRatio);
  	MCE_DEBUG("ProductEdmFilter::CalcSendRatio member Balance, Ratio:"<<its->first<<" : "<<ratio1);
  	if(ratio > ratio1){
  	  ratio = ratio1;
  	}

  	MCE_DEBUG("ProductEdmFilter::CalcSendRatio min send ratio,"<<its->first<<" : "<<ratio<<" "<<ProductMinSendRatio);
  	if(ratio < ProductMinSendRatio){
  	  ratio = ProductMinSendRatio;
  	}
	  results[its->first] = ratio;
	  MCE_INFO("ProductEdmFilter::CalcSendRatio ,finally ratio:"<<its->first<<" : "<<results[its->first]);
  }
  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    campaignID2Ratio_.swap(results);
  }
}

double ProductEdmFilter::GetSendRatio(const Ice::Long & campaignID){
	{
    IceUtil::RWRecMutex::RLock lock(mutex_);
    std::map<Ice::Long, double>::iterator it = campaignID2Ratio_.find(campaignID);
    if(it != campaignID2Ratio_.end()){
      return it->second;
    }
	}
  double ratio = 1.0;
  ratio = EdmMonitorConfig::instance().GetValue(PRODUCT_NO_QUATODATA_RATIO);
	if(ratio == -1){
	  ratio = 0.2;
	}
  return  ratio;
}
