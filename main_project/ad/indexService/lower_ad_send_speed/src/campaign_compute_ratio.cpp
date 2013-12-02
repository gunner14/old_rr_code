#include <algorithm>
#include "../include/campaign_compute_ratio.h"

using namespace std;
using namespace xce::ad;

double CampaignComputeRatio::Compute(map<string, double>& data_map){
  double ratio = 1.0;
  double budget_threshold = LowerConfig::instance().GetValue(CAMPAIGN_DAILY_BUDGET_THRESHOLD);
  double daily_ratio_threshold = LowerConfig::instance().GetValue(CAMPAIGN_DAILY_RATIO_THRESHOLD);
  double low_campaign_ratio = LowerConfig::instance().GetValue(LOW_CAMPAIGN_RATIO);
  if ((budget_threshold < 0)
      || (daily_ratio_threshold < 0)){
    return ratio;
  }

  double daily_budget = data_map[CAMPAIGN_DAILY_BUDGET];
  double daily_consume = data_map[CAMPAIGN_DAILY_CONSUME]; 

  if (((daily_budget - daily_consume) <= budget_threshold)
        && (budget_threshold > 0)
     ){
       ratio = (daily_budget - daily_consume) / budget_threshold * low_campaign_ratio;
     }
  
  //MCE_DEBUG("CampaignComputeRatio::Compute --> after threshold judge ratio: " << ratio << " daily_budget: " << daily_budget << " daily_consume: " << daily_consume);
  if ((daily_budget > 0)
      && ((daily_consume / daily_budget) 
	    > daily_ratio_threshold)
     ){
       ratio = min((daily_budget - daily_consume) / daily_budget, ratio);
     }
  //MCE_DEBUG("CampaignComputeRatio::Compute --> after speed judget ratio: " << ratio);

  return ratio;
}
