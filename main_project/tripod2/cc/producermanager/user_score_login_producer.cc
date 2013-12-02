/*
 * =====================================================================================
 *
 *       Filename:  user_score_login_producer.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月16日 12时03分48秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  xiaoguang.yang (), xiaoguang.yang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#include "OceProto/proto/user_score.pb.h"
#include "tripod2/cc/producermanager/user_score_login_producer.h" 
#include <algorithm>
#include "boost/lexical_cast.hpp"

namespace xce {
namespace tripod {

using namespace com::xiaonei::xce;
using namespace xce::tripoddata::userscore;

bool UserScoreLoginProducer::Create(const std::string& key, ByteSeq* value, bool use_master) {
  if (!value) {
    return false;
  }
  
  MCE_DEBUG("UserScoreLoginProducer::Create(), key = " << key << ", use_master = " << use_master);
  long user_id = boost::lexical_cast<long>(key);
  Statement sql;
  sql << "SELECT user_id, history_days, continue_days, active_days, UNIX_TIMESTAMP(last_time) AS sec_time, login_type FROM score_login_"
    << user_id % table_mod << " WHERE user_id = " << user_id;
  
  ostringstream pattern;
  pattern << "user_score_" << (user_id % table_mod);
  ScoreLogin query_result; 
  int count = QueryRunner("user_score", use_master?CDbWServer:CDbRServer, pattern.str()).query(sql, ScoreLoginResultHandler(query_result));
  if (!count) {
    MCE_WARN("UserScoreLoginProducer::Create(), miss user_id = " << user_id);
    return false;
  } else {
    PbScoreLogin * message = new PbScoreLogin(); 
    message->set_user_id(user_id);
    message->set_history_days(query_result.history_login_days_);
    message->set_continue_days(query_result.continue_login_days_);
    message->set_active_days(query_result.active_days_);
    message->set_last_time(query_result.last_login_time_);
    message->set_login_type(query_result.login_type_);
    std::string str;
    message->SerializeToString(&str);
    value->insert(value->begin(), str.begin(), str.end());
    delete message;
  }
  return true;
}

bool ScoreLoginResultHandler::handle(mysqlpp::Row& row) const {
  int history_login_days = (int) row["history_days"];
  int continue_login_days = (int) row["continue_days"];
  long last_login_time = (long) row["sec_time"];
  last_login_time *= 1000;
  int active_days = (int) row["active_days"];
  int login_type = (int) row["login_type"];

  result_->history_login_days_ = history_login_days <= 0 ? 1 : history_login_days;
  result_->continue_login_days_ = continue_login_days <= 0 ? 1 : continue_login_days;
  result_->last_login_time_ = last_login_time < 1000 ? 1000 : last_login_time;
  result_->login_type_ = login_type == 0 ? 4 : login_type;
  if (active_days == 0) {
    int abs_count = 2;
    for(int i = 1; i <= continue_login_days; ++i) {
      active_days = active_days | abs_count;
      abs_count *= 2;
      if(i == 30) break;
    }
  }
  result_->active_days_ = active_days;
}

}} //xce::tripod
