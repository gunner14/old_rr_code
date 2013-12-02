#include "../include/member_compute_ratio.h"

using namespace std;
using namespace xce::ad;

double MemberComputeRatio::Compute(map<string, double>& data_map){
  double balance_threshold = LowerConfig::instance().GetValue(MEMBER_BALANCE_THRESHOLD);
  double balance_ratio = LowerConfig::instance().GetValue(LOW_BALANCE_RATIO);
  if ((balance_threshold < 0)
      ||(balance_ratio < 0)){
    return 1.0;
  }

  double balance = data_map[MEMBER_BALANCE]; 
  //MCE_DEBUG("MemberComputeRatio::Compute --> balance_threshold: " << balance_threshold << " balance: " << balance);
  if ((balance < balance_threshold)
      && (balance_threshold > 0)
     ){
      //MCE_DEBUG("MemberComputeRatio::Compute --> member ratio: " << balance / balance_threshold * balance_ratio);
      return balance / balance_threshold  * balance_ratio;
     }
  return 1.0;
}
