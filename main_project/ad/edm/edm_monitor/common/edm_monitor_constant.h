#ifndef __XCE_EDM_MONITOR_CONSTANT_H_
#define __XCE_EDM_MONITOR_CONSTANT_H_

#include <string>
#include <Ice/Config.h>
#include "ns_common.h"

NS_BEGIN(xce, edm)

const unsigned int ZERO = 0;
const int FAILED = -1;
const Ice::Long INIT_VALUE = -1;
const unsigned int LIMIT_SIZE = 20;
const unsigned int ONE_DAY = 24 * 60 * 60;

const std::string CAMPAIGN_DAILY_BUDGET = "least_campaign_daily_budget";
const std::string ADGROUP_DAILY_QUOTA = "least_adgroup_daily_quota";
const std::string ADGROUP_TOTAL_QUOTA = "least_adgroup_total_quota";
const std::string CAMPAIGN_DAILY_RATIO = "least_campaign_daily_ratio";
const std::string ADGROUP_DAILY_RATIO = "least_adgroup_daily_ratio";
const std::string ADGROUP_TOTAL_RATIO = "least_adgroup_total_ratio";
const std::string CAMPAIGN_DAILY_LIMIT = "least_campaign_daily_ratio";
const std::string ADGROUP_DAILY_LIMIT = "least_adgroup_daily_ratio";
const std::string ADGROUP_TOTAL_LIMIT = "least_adgroup_total_ratio";
const std::string CAMPAIGN_DEFAULT_RATIO = "campaign_default_ratio";
const std::string ADGROUP_DEFAULT_RATIO = "adgroup_default_ratio";

//品牌广告相关参数
const std::string BRAND_DAILY_THRESHOLD = "brand_daily_threshold";//品牌广告日预投量-日累计投放量的阈值
const std::string BRAND_DAILY_LIMIT = "brand_daily_limit"; //日预投量-日累计量 < 阈值时的概率参数
const std::string BRAND_DAILY_TOTAL_THRESHOLD = "brand_daily_total_threshold"; //总预投量-总累计量的阈值
const std::string BRAND_DAILY_TOTAL_LIMIT = "brand_daily_total_limit";//总预投量-总累计量 < 阈值时的概率参数
const std::string BRAND_DAILY_RATIO_THRESHOLD = "brand_daily_ratio_threshold";//日累积量/日预投量比例阈值
const std::string BRAND_TOTAL_RATIO_THRESHOLD = "brand_total_ratio_threshold";//总累积量/总预投量比例阈值
const std::string BRAND_MEMBER_BALANCE_THRESHOLD = "brand_member_balance_threshold"; //账户余额阈值
const std::string BRAND_MEMBER_BALANCE_LIMIT = "brand_member_balance_limit";//账户余额<阈值时的概率参数
const std::string BRAND_MIN_SEND_LIMIT = "brand_min_send_limit"; //品牌广告最低发送概率
const std::string BRAND_NO_QUATODATA_RATIO = "brand_no_quatodata_ratio"; //没有业务数据时的发送概率

//自助广告相关参数
const std::string SELF_DAILY_BUDGET_CONSUME_LIMIT = "self_daily_budget_consume_limit";//日累计花费找日预投量比例的阈值
const std::string SELF_DAILY_THRESHOLD = "self_daily_threshold";//日预售-日已花费的阈值
const std::string SELF_DAILY_RATIO_LOWER_THRESHOLD = "self_daily_ratio_lower_threshold";//日预算-日已花费低于阈值时的发送概率参数
const std::string SELF_BALANCE_THRESHOLD = "self_balance_threshold"; //账户余额阈值
const std::string SELF_BALANCE_LOWER_RATIO = "self_balance_lower_ratio";//账户余额小于阈值时发送概率参数
const std::string SELF_MIN_SEND_RATIO = "self_min_send_ratio";//自助广告的最低发送概率
const std::string SELF_NO_QUATODATA_RATIO = "self_no_quatodata_ratio"; //没有业务数据时发送概率

//推左广告相关参数
const std::string LEFT_AD_DAILY_BUDGET_CONSUME_LIMIT = "left_ad_daily_budget_consume_limit";//日累计花费与日预投量比例的阈值
const std::string LEFT_AD_DAILY_THRESHOLD = "left_ad_daily_threshold";//日余额的阈值
const std::string LEFT_AD_DAILY_RATIO_LOWER_THRESHOLD = "left_ad_daily_ratio_lower_threshold";//日余额低于阈值时的发送概率参数
const std::string LEFT_AD_BALANCE_THRESHOLD = "left_ad_balance_threshold";//账户余额的阈值
const std::string LEFT_AD_BALANCE_LOWER_RATIO = "left_balance_lower_ratio";//账户余额小于阈值时发送概率
const std::string LEFT_AD_MIN_SEND_RATIO = "left_ad_min_send_ratio";//推左广告的最低发送概率
const std::string LEFT_AD_NO_QUATODATA_RATIO = "left_ad_default_ratio";

//产品相关参数
const std::string PRODUCT_DAILY_BUDGET_CONSUME_LIMIT = "product_daily_budget_consume_limit";//日累计花费找日预投量比例的阈值
const std::string PRODUCT_DAILY_THRESHOLD = "product_daily_threshold";//日预售-日已花费的阈值
const std::string PRODUCT_DAILY_RATIO_LOWER_THRESHOLD = "product_daily_ratio_lower_threshold";//日预算-日已花费低于阈值时的发送概率参数
const std::string PRODUCT_BALANCE_THRESHOLD = "product_balance_threshold"; //账户余额阈值
const std::string PRODUCT_BALANCE_LOWER_RATIO = "product_balance_lower_ratio";//账户余额小于阈值时发送概率参数
const std::string PRODUCT_MIN_SEND_RATIO = "product_min_send_ratio";//产品广告的最低发送概率
const std::string PRODUCT_NO_QUATODATA_RATIO = "product_no_quatodata_ratio"; //没有业务数据时发送概率

//http相关
const std::string CAMPAIGN_ID = "campaignId";
const std::string DAILY_BUDGET = "dailyBudget";
const std::string DAILY_CONSUME = "dailyConsume";
const std::string ADGROUP_ID = "adgroupId";
const std::string DAILY_QUOTA = "dailyObject"; // Daily Quota
const std::string DAILY_AMOUNT = "dailyCount"; // Daily Amount
const std::string TOTAL_QUOTA = "totalObject"; // Total Quota
const std::string TOTAL_AMOUNT = "totalCount"; // Total Amount
const std::string MEMBER_ID = "memberId";
const std::string MEMBER_BALANCE = "balance"; //Member Balance

NS_END

#endif
