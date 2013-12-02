/*
 * EdmStruct.cpp
 *
 *  Created on: 2012-2-20
 *      Author: sunzz
 */

#include "EdmStruct.h"
#include "Date.h"


#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/regex.hpp>

namespace xce {
namespace ad {

using namespace MyUtil;
using namespace std;
using namespace boost;



/*********************************************************************************************/
/***********************************************************************************************************************/
bool AdMember::Available(int display_type) {
bool tmp_val = is_formal_ && (validity_ == 1) && no_exceed_max_
    && am_online_status_ && ma_online_status_ && (audit_status_ == 1) && delete_flag_;
if (display_type != 1) {
  tmp_val = have_money_ && tmp_val;
}
//MCE_DEBUG("AdMember::Available --> RESULT = " << tmp_val << " member_id = " << member_id_ << " is_formal_ = " << is_formal_ << " validity_ = " <<
// validity_ << " no_exceed_max_ = " << no_exceed_max_ << " am_online_status_ = " << am_online_status_ << " ma_online_status_ = " <<
// ma_online_status_ << " delete_flag_ = " << delete_flag_ << " have_money_ = " << have_money_ << " display_type = " << display_type);
return tmp_val;
}
/***********************************************************************************/

bool AdCampaign::Available() {
  Date date = Date(time(NULL));
  time_t timestamp = date.time();
  bool time_check = timestamp >= begin_time_ && (!end_time_ || timestamp <= end_time_);
  bool result = delete_flag_ && (audit_status_ == 1) && time_check
      && (member_status_ == 1) && (am_status_ == 1) && (sys_status_ == 1);

  return result;
}

/***********************************************************************************************************************/

bool AdGroup::Available() {
  //MCE_DEBUG("AdGroup::Available -->group_id:"<< group_id_<<"member_table_status_: " << member_table_status_ 
  //        <<", campaign_status_:" << campaign_status_ << ",point_valid_: " << point_valid_);

  return member_table_status_ && campaign_status_ && point_valid_;
      //&& (member_id_ == 999 || max_price_ >= EngineConfig::instance().sys_min_price());
}

int AdGroup::edm_member_type(){//类型转换，根据member_category_推断客户类型。1是品牌 ；8是产品； 其他是自助
  if(1 == member_category_){
    return Brand_ad;
  } else if(8 == member_category_) {
    return Product_ad;
  } else {
    return Self_ad;
  }
}

bool AdGroup::HasCreatives() {
  return !(creative_seq_.empty());
}

/*void AdGroup::UpdateCampainStatus() {
  AdCampaignPtr campaign_ptr = AdCampaignPool::instance().GetAdCampaign(campaign_id_);
  if (campaign_ptr) {
    campaign_status_ = campaign_ptr->Available();
  }
}*/


/***********************************************************************************************************************/



} // end namespace ad
} // end namespace xce


