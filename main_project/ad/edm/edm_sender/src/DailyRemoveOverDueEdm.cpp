/*
 * FrequencyCheck.cpp
 *
 * module role : 每天定时删除前一天过期的推左Edm
 * Created on: 2012-09-28
 * Author: ping.zhang 
 */

#include <algorithm>
#include "DailyRemoveOverDueEdm.h"
#include "QueryRunner.h"
#include "LogWrapper.h"
#include <boost/lexical_cast.hpp>
#include <string>
#include "Date.h"
#include "ServiceI.h"
#include "ParametersController.h"
#include "FeedAssistantReplicaAdapter.h"
#include "EdmStructCache.h"
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/tokenizer.hpp>

using namespace boost;
using namespace xce::feed;
namespace xce {
namespace ad {


bool DailyRemoveOverDueEdmTimer::RemoveEdm() {
  Date date = Date(time(NULL));
  Statement sql;
  map<Ice::Long, AdCampaignPtr> campaign_pool;
  sql
    << "select campaign_id, end_time from campaign where sys_status = 1 and member_status = 1 and am_status = 1 and delete_flag = 1 and audit_status = 1";
  const string DB_INSTANCE = EngineConfig::instance().db_source_ads();
  try{
    mysqlpp::StoreQueryResult res = QueryRunner(DB_INSTANCE, CDbRServer, "campaign").store(sql);
    if (res && res.num_rows() > 0) {
      for (size_t i = 0; i < res.num_rows(); ++i) {
        mysqlpp::Row row = res.at(i);
        Ice::Long campaign_id = lexical_cast<Ice::Long> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["campaign_id"]);
        string end_time =
          (mysqlpp::Null<string, mysqlpp::NullIsBlank>) row["end_time"];
        if (!end_time.empty()) {
          Date date_end_time = Date(end_time);
          //过期时间为今天0点
          if(0 == ((date.time() - date_end_time.time())/86400)) {
            overdue_campaigns_.insert(campaign_id);
          }
        }
      }
    }
  } catch(std::exception& e) {
    MCE_WARN("DailyRemoveOverDueEdmTimer::RemoveEdm---> std::exception error : " << e.what());
  }  catch(...) {
    MCE_WARN("RemoveEdm--> err:" <<",line:" << __LINE__)
  }
  if(overdue_campaigns_.empty()) {
    MCE_INFO("DailyRemoveOverDueEdmTimer::RemoveEdm---> no overdue campaign.");
    return true;
  } else {
    ostringstream outcampaigns;
    copy(overdue_campaigns_.begin(),overdue_campaigns_.end(),ostream_iterator<Ice::Long>(outcampaigns,","));
    Statement sql;
    sql
      << " SELECT adgroup_id campaign_id FROM adgroup "
      << " WHERE total_sys_status = 1 AND daily_sys_status = 1 "
      << " AND member_status = 1 AND am_status = 1 AND audit_status = 1 "
      << " AND delete_flag = 1 AND display_type = 4 AND trans_type = 10 "
      << " AND member_id != 999 AND product_type & 5 = 5 and trans_type = 10 "
      << " AND campaign_id in (" << outcampaigns.str() << ")";
    const string DB_INSTANCE = EngineConfig::instance().db_source_ads();

    try {
      mysqlpp::StoreQueryResult res = QueryRunner(DB_INSTANCE, CDbRServer, 
          "adgroup").store(sql);

      if (res && res.num_rows() > 0) {
        for (size_t i = 0; i < res.num_rows(); ++i) {
          Ice::Long group_id = 0;
          mysqlpp::Row row = res.at(i);
          group_id = lexical_cast<Ice::Long> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["adgroup_id"]);
          if (0 != group_id) {
            //删除过期的推左EDM，推左的EDM是图文，stype：8002
            FeedAssistantReplicaAdapter::instance().removeFeedBySource(group_id,8002);
            MCE_INFO("Remove OverDue Edm: adgroup_id = " << group_id);
          }
        }
      } else {
        MCE_INFO("AdGroupPool::Init --> no data in adgroup");
      } 
    } catch(std::exception& e) {
      MCE_WARN("AdGroupPool::LoadAdGroup std::exception error : " << e.what());
    } catch(...) {
      MCE_WARN("AdGroupPool::LoadAdGroup unknown error.");
    }
  }
  return true; 
}


void DailyRemoveOverDueEdmTimer::handle() {
  //如果是凌晨2点，就删除过期edm
  Date date = Date(time(NULL));
  if(2 == date.hour()) {
    RemoveEdm();
  }
}


}
}
