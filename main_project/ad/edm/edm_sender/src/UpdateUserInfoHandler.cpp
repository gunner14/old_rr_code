/*
 * UpdateUserInfoHandler.cpp
 *
 *  Created on: 2012-2-27
 *      Author: zhizhao.sun
 */

#include"UpdateUserInfoHandler.h"
#include "FrequencyCheck.h"
#include "ValidateCheck.h"
#include "ParametersController.h"

namespace xce {
namespace ad {

//static const string DB_INSTANCE = EngineConfig::instance().db_source();
//static const string DB_INSTANCE = "ads_db";
static const string DB_INSTANCE = "edmsend_db";
static const std::string kWeeklySendedTableName = "ad_edm_sended_weekly";
static const std::string kDailySendedTableName = "ad_edm_sended_daily";

bool UpdateUserInfoHandler::handle(MatchHandlerParamter& para) {//更新用户状态信息 、删除脚印
  //user_id及edm_type从para中获取
  int user_id = para.user_id();
  set<SelectedAdGroupPtr> select_groups = para.selected_groups();
  set<SelectedAdGroupPtr>::iterator it;
  for (it = select_groups.begin(); it != select_groups.end(); ++it) {
    UpdateUserInfo(user_id,(*it)->group_id(),(*it)->send_type());
  }
  //删除用户登录脚印
  ValidateCheckCache::instance().RemoveUserFootPrint(user_id);
  return true;
}

void UpdateUserInfoHandler::UpdateUserInfo(int user_id, Ice::Long group_id, SendType send_type) { //用户信息更新
  FrequencyCache::instance().Add(user_id, send_type);
  if (kWeekly == send_type) {    //暂定 1 代表商业edm
    UpdateDbForWeeklySendEdm(user_id,group_id);
  } else {
    UpdateDbForDailySendEdm(user_id,group_id);
  }
  /*
  //待加 ：对 用户定向的edm 发送后 用户id的删除
  */
}

void UpdateUserInfoHandler::UpdateDbForWeeklySendEdm(int user_id, Ice::Long group_id) {
  com::xiaonei::xce::Statement sql;
  time_t now_time = time(NULL);
  sql << "insert into " << kWeeklySendedTableName << "(adgroup_id,userid,send_time_int) values (" << group_id << "," << user_id << "," << now_time << ")";
  try {
    com::xiaonei::xce::QueryRunner(DB_INSTANCE, com::xiaonei::xce::CDbWServer,kWeeklySendedTableName).store(sql);  
  } catch (Ice::Exception& e) {
    MCE_WARN("insert send of edm:" << group_id << " user_id:" << user_id << " in table:" << kWeeklySendedTableName << " failed, err->" << e);
  } catch (std::exception& ex) {
    MCE_WARN("insert send of edm:" << group_id << " user_id:" << user_id << " in table:" << kWeeklySendedTableName << " failed, std err->" << ex.what());
  } catch (...) {
    MCE_WARN("insert send of edm:" << group_id << " user_id:" << user_id << " in table:" << kWeeklySendedTableName);
  }
  /*
  待加 数据库中发送量的更改
  */
}

void UpdateUserInfoHandler::UpdateDbForDailySendEdm(int user_id, Ice::Long group_id) {
  com::xiaonei::xce::Statement sql;
  time_t now_time = time(NULL);
  sql << "insert into " << kDailySendedTableName << "(adgroup_id,userid,send_time_int) values (" << group_id << "," << user_id << "," << now_time << ")";
  try {
    com::xiaonei::xce::QueryRunner(DB_INSTANCE, com::xiaonei::xce::CDbWServer,kDailySendedTableName).store(sql);  
  } catch (Ice::Exception& e) {
    MCE_WARN("insert send of edm:" << group_id << " user_id:" << user_id << " in table:" << kDailySendedTableName << " failed, err->" << e);
  } catch (std::exception& ex) {
    MCE_WARN("insert send of edm:" << group_id << " user_id:" << user_id << " in table:" << kDailySendedTableName << " failed, std err->" << ex.what());
  } catch (...) {
    MCE_WARN("insert send of edm:" << group_id << " user_id:" << user_id << " in table:" << kDailySendedTableName);
  }
  /*
  待加 数据库中发送量的更改
  */
}

}
}
