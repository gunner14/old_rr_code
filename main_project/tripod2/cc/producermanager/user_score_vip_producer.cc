/*
 * =====================================================================================
 *
 *       Filename:  user_score_vip_producer.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月09日 16时01分16秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  xiaoguang.yang (), xiaoguang.yang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#include "OceProto/proto/user_score.pb.h"
#include "tripod2/cc/producermanager/user_score_vip_producer.h" 
#include <algorithm>
#include "boost/lexical_cast.hpp"

namespace xce {
namespace tripod {

using namespace com::xiaonei::xce;
using namespace xce::tripoddata::userscore;

bool UserScoreVipProducer::Create(const std::string& key, ByteSeq* value, bool use_master) {
  if (!value) {
    return false;
  }
  MCE_DEBUG("UserScoreVipProducer::Create(), key = " << key << ", use_master = " << use_master);
  int user_id = boost::lexical_cast<int>(key);
  Statement sql;
  sql << "SELECT id, state, level FROM user_state WHERE id =" << user_id;
  int result = 0;
  int count = QueryRunner("user_state", use_master?CDbWServer:CDbRServer).query(sql, ScoreVipResultHandler(result));
  if( !count ){
    MCE_WARN("UserScoreVipProducer::Create(), miss user_id = " << user_id);
    return false;
  } else {
    PbScoreVip *message = new PbScoreVip();
    message->set_user_id(user_id);
    message->set_vip(result);
    std::string str;
    message->SerializeToString(&str);
    value->insert(value->begin(), str.begin(), str.end());
    delete message;
  }
  return true;
}

bool ScoreVipResultHandler::handle(mysqlpp::Row& row) const {
  long state = (long) row["state"];
  int level = (int) row["level"];
  if (level == 0) {
      level = 1;
  }
  *result_ = (0!=(state&(1<<1))) ? level : -level;
  return true;
}


}}
