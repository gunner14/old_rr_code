/*
 * =====================================================================================
 *
 *       Filename:  sql.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年04月18日 14时17分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef __INDEX_SERVICE_SQL_H__
#define __INDEX_SERVICE_SQL_H__

#include <string>

namespace xce{
	namespace ad{

		const std::string MEMBER_QUERY =   std::string("SELECT member_id,category,industry")
                                   + " FROM member"
                                   + " WHERE is_formal != 0 AND no_exceed_max != 0 and am_online_status != 0"
                                   + " AND have_money != 0 AND ma_online_status != 0 AND delete_flag != 0"
                                   + " AND audit_status = 1 AND validity = 1";

		const std::string CAMPAIGN_QUERY = std::string("SELECT  campaign_id, member_id")
                                     + " FROM campaign " 
                                     + " WHERE delete_flag != 0 AND audit_status = 1 AND am_status = 1"
                                     + " AND sys_status = 1 AND member_status = 1"
                                     + " AND now() > begin_time AND ((end_time is NULL) OR (now() < end_time))";

		const std::string GROUP_QUERY =  std::string("SELECT adgroup_id, campaign_id, member_id, display_type, trans_type,")
                                  + " max_price,day_points,hour_points" 
                                  + " FROM adgroup" 
                                  + " WHERE display_type = 4 AND member_id != 999"
                                  + " AND member_status = 1 AND am_status = 1 AND audit_status = 1 AND delete_flag = 1"
                                  + " AND daily_sys_status = 1 AND total_sys_status = 1";


		const std::string CREATIVE_QUERY = std::string("SELECT creative_id, adgroup_id, widget_id")
                                     + " FROM creative"
                                     + " WHERE member_status = 1 AND am_status = 1 AND audit_status = 1 AND delete_flag = 1";

		const std::string ADZONE_GROUP_REF_QUERY = std::string("SELECT adzone_id, adgroup_id, max_price")
                                            + " FROM adgroup_adzone_ref"
                                            + " WHERE member_status = 1 AND am_status =1 AND delete_flag =1 AND show_flag = 1";

		const std::string BID_UNIT_QUERY = std::string("SELECT bid_unit_id, adgroup_id, cast_gender, cast_stage, cast_grade,")
                                     + " cast_age, cast_school, cast_area, cast_company, cast_interest"
                                     + " FROM bid_unit"
                                     + " WHERE delete_flag = 1 AND member_id != 999";
	}
}

#endif
