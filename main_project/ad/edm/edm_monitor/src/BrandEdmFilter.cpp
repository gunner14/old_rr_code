#include "../include/BrandEdmFilter.h"

USE_NS(std)
USE_NS(xce::edm)

//bool BrandEdmFilter::IsOffline(DummyRecordPtr record){
bool BrandEdmFilter::IsOffline(const EdmInfo& edmInfo){
  Ice::Long id = edmInfo.adGroupID;
  double ratio = GetSendRatio(id);
  MCE_DEBUG("BrandEdmFilter::IsOffline "<<id<<"-->ratio:"<<ratio);
  return SelectByRand(ratio);
}

void BrandEdmFilter::CalcSendRatio(const std::map<Ice::Long, DummyRecordPtr> & records){
  MCE_DEBUG("BrandEdmQuotaLoader::CalcSendRatio size:"<<records.size());
  std::map<Ice::Long, double>  results;
  double DailyThreshold         = EdmMonitorConfig::instance().GetValue(BRAND_DAILY_THRESHOLD);
  double DailyLimit             = EdmMonitorConfig::instance().GetValue(BRAND_DAILY_LIMIT);
  double DailyTotalThreshold    = EdmMonitorConfig::instance().GetValue(BRAND_DAILY_TOTAL_THRESHOLD);
  double DailyTotalLimit        = EdmMonitorConfig::instance().GetValue(BRAND_DAILY_TOTAL_LIMIT);
  double DailyRatioThreshold    = EdmMonitorConfig::instance().GetValue(BRAND_DAILY_RATIO_THRESHOLD);
  double TotalRatioThreshold    = EdmMonitorConfig::instance().GetValue(BRAND_TOTAL_RATIO_THRESHOLD);
  double MemberBalanceThreshold = EdmMonitorConfig::instance().GetValue(BRAND_MEMBER_BALANCE_THRESHOLD);
  double MemberBalanceLimit     = EdmMonitorConfig::instance().GetValue(BRAND_MEMBER_BALANCE_LIMIT);
  double BrandMinSendRatio      = EdmMonitorConfig::instance().GetValue(BRAND_MIN_SEND_LIMIT);
  double BrandNoQuatoDataRatio  = EdmMonitorConfig::instance().GetValue(BRAND_NO_QUATODATA_RATIO);

  if(DailyThreshold == FAILED
		 || DailyLimit== FAILED
     || DailyTotalThreshold == FAILED
  	 || DailyTotalLimit == FAILED
 		 || DailyRatioThreshold == FAILED
  	 || TotalRatioThreshold	== FAILED
		 || MemberBalanceThreshold == FAILED
		 || MemberBalanceLimit == FAILED
		 || BrandMinSendRatio == FAILED
		 || BrandNoQuatoDataRatio == FAILED){
    MCE_WARN("BrandEdmFilter::CalcSendRatio ,finally ratio,Config Data Error,not Config some Data or Config Value = -1 or Error");
	if(BrandNoQuatoDataRatio == FAILED){
	  BrandNoQuatoDataRatio = 0.2;
	}
  }
	  
  std::map<Ice::Long, DummyRecordPtr>::const_iterator its = records.begin();
  for(; its!=records.end(); ++its) {
	AdGroupRecord * adgrouprecordPtr = dynamic_cast<AdGroupRecord *>(its->second.get());
	if(adgrouprecordPtr == NULL){
	  continue;
	}
	double minratio = 1.0;
	double ratio1 = GetMinDailyRatio(adgrouprecordPtr->dailyQuota_, adgrouprecordPtr->dailyAmount_, DailyThreshold, DailyLimit);
    MCE_DEBUG("BrandEdmFilter::CalcSendRatio ( Quato -  Amount)     <        Threshold: "<<its->first<<" : "<<ratio1);
	if(minratio > ratio1){
      minratio = ratio1;
	}
	ratio1 = GetMinDailyRatio(adgrouprecordPtr->totalQuota_, adgrouprecordPtr->totalAmount_, DailyTotalThreshold, DailyTotalLimit);
    MCE_DEBUG("BrandEdmFilter::CalcSendRatio (TotalQuato-TotalAmount) < TotalThreshold:"<<its->first<<" : "<<ratio1);
	if(minratio > ratio1){
	  minratio = ratio1;
	}
	ratio1 = GetMinTotalRatio(adgrouprecordPtr->dailyQuota_, adgrouprecordPtr->dailyAmount_, DailyRatioThreshold);
    MCE_DEBUG("BrandEdmFilter::CalcSendRatio (Amount   /   Quato)     <         ratio:"<<its->first<<" : "<<ratio1);
	if(minratio > ratio1){
	  minratio = ratio1;
	}
	ratio1 = GetMinTotalRatio(adgrouprecordPtr->totalQuota_, adgrouprecordPtr->totalAmount_, TotalRatioThreshold);
    MCE_DEBUG("BrandEdmFilter::CalcSendRatio (TotalAmount / TotalQuato) <  TotalRatio:"<<its->first<<" : "<<ratio1);
	if(minratio > ratio1){
	  minratio = ratio1;
	}
	ratio1 = GetMemberBalanceRatio(adgrouprecordPtr->memberBalance_, MemberBalanceThreshold, MemberBalanceLimit);
    MCE_DEBUG("BrandEdmFilter::CalcSendRatio memberBalance  <  memberBalanceThreshold:"<<its->first<<" : "<<ratio1);
	if(minratio > ratio1){
	  minratio = ratio1;
	}


    MCE_DEBUG("BrandEdmFilter::CalcSendRatio "<<its->first<<" current:"<<minratio<<" min:"<<BrandMinSendRatio);
	if(minratio < BrandMinSendRatio){
	  minratio = BrandMinSendRatio;
	}
	results[its->first] = minratio;
  MCE_INFO("BrandEdmFilter::CalcSendRatio Failly Ratio:"<<its->first<<" : "<<results[its->first]);
  }
  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    adGroups2Ratio_.swap(results);
  }
}

double BrandEdmFilter::GetSendRatio(const Ice::Long & adgroupid){
	{
    IceUtil::RWRecMutex::RLock lock(mutex_);
    std::map<Ice::Long, double>::iterator it = adGroups2Ratio_.find(adgroupid);
    if(it != adGroups2Ratio_.end()){
      return it->second; 
    }
	}
  double ratio = 0.2;
  ratio = EdmMonitorConfig::instance().GetValue(BRAND_NO_QUATODATA_RATIO);
	if(ratio == -1 ) {
	  ratio = 0.2;  
	}
  return  ratio;
}
