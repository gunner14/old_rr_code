#ifndef __XCE_EDM_MONITOR_CONSTANT_H_
#define __XCE_EDM_MONITOR_CONSTANT_H_

#include <set>
#include <string>
#include <vector>

namespace xce{
namespace ad{

const int UNIT = 100;
const int FAILED = -1;
const unsigned int ZERO = 0;
const unsigned int THREE_CLOCK = 3;
const unsigned int ONE_MINIUTE = 65;
const int CAMPAIGN_ARRAY_SIZE = 3;
const int MEMBER_ARRAY_SIZE = 2;
const int CONFIG_ARRAY_SIZE = 10;
const unsigned int ONE_DAY = 24 * 60 * 60;

const std::string UPDATE_TIME = "update_time";
const std::string CAMPAIGN_ID = "campaignId";
const std::string CAMPAIGN_DAILY_BUDGET = "dailyBudget";
const std::string CAMPAIGN_DAILY_CONSUME = "dailyConsume";
const std::string LOW_CAMPAIGN_RATIO = "low_campaign_ratio";
const std::string CAMPAIGN_INIT_RATIO = "campaign_init_ratio";
const std::string CAMPAIGN_DEFAULT_RATIO = "campaign_default_ratio";
const std::string CAMPAIGN_DAILY_RATIO_THRESHOLD = "campaign_daily_ratio_threshold";
const std::string CAMPAIGN_DAILY_BUDGET_THRESHOLD = "campaign_daily_budget_threshold";

const static std::string CAMPAIGN_ARRAY[CAMPAIGN_ARRAY_SIZE] =  {CAMPAIGN_ID, CAMPAIGN_DAILY_BUDGET, CAMPAIGN_DAILY_CONSUME};
const static std::vector<std::string> CAMPAIGN_VEC(CAMPAIGN_ARRAY, CAMPAIGN_ARRAY + CAMPAIGN_ARRAY_SIZE);

const std::string MEMBER_ID = "memberId";
const std::string MEMBER_BALANCE = "balance";
const std::string LOW_BALANCE_RATIO = "low_balance_ratio";
const std::string MEMBER_INIT_RATIO = "member_init_ratio";
const std::string MEMBER_LIMIT_RATIO = "member_limit_ratio";
const std::string MEMBER_DEFAULT_RATIO = "member_default_ratio";
const std::string MEMBER_BALANCE_THRESHOLD = "member_balance_threshold";

const static std::string MEMBER_ARRAY[MEMBER_ARRAY_SIZE] = {MEMBER_ID, MEMBER_BALANCE};
const static std::vector<std::string> MEMBER_VEC(MEMBER_ARRAY, MEMBER_ARRAY + MEMBER_ARRAY_SIZE);

const std::string LEAST_RATIO = "least_ratio";

const static std::string CONFIG_ARRAY[CONFIG_ARRAY_SIZE] = {CAMPAIGN_DEFAULT_RATIO, LOW_CAMPAIGN_RATIO, CAMPAIGN_DAILY_RATIO_THRESHOLD, CAMPAIGN_DAILY_BUDGET_THRESHOLD, LOW_BALANCE_RATIO, MEMBER_DEFAULT_RATIO, MEMBER_BALANCE_THRESHOLD, LEAST_RATIO, CAMPAIGN_INIT_RATIO, MEMBER_INIT_RATIO};
const static std::set<std::string> CONFIG_SET(CONFIG_ARRAY, CONFIG_ARRAY + CONFIG_ARRAY_SIZE);

const std::string MEMBER_URL = "http://snipe.jebe.renren.com/snipe/account/members/";
const std::string CAMPAIGN_URL = "http://snipe.jebe.renren.com/snipe/account/adgroup/campaignids/";

const std::string CONFIG_TABLE_NAME = "ad_send_speed_lower_config";
const std::string AD_MEMBER_BALANCE_TABLE_NAME = "ad_member_balance";
const std::string AD_CAMPAIGN_BALANCE_TABLE_NAME = "ad_campaign_balance";

}//end of namespace ad
}//end of namespace xce

#endif
