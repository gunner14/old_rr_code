/*
 * AdEdmUserIDIndex.cpp
 *
 *  Created on: 2012-2-23
 *      Author: yongjun.yin
 */

#include "AdEdmUserIDIndex.h"
#include <iostream>
#include <sstream>
#include <QueryRunner.h>
#include <DbDescriptor.h>
#include "../../../util/src/AdCommonUtil.h"
#include "client/linux/handler/exception_handler.h"
#include "AdEdmUserTargetCacheI.h"
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace IceUtil;
using namespace xce::ad;
using namespace com::xiaonei::xce;
using namespace MyUtil;
using namespace boost;

bool AdEdmUserIDIndex::FindUserID(int userID) {
  if (userIDSet_.find(userID) != userIDSet_.end()){
    return true;
  }
  return false;
}

void AdEdmUserIDIndex::RmUserID(int userID) {
  userIDSet_.erase(userID);
}

void AdEdmUserIDIndex::ParseUserIDSet(const mysqlpp::Row::value_type &strUserIDs, set<uint32_t>& userIDSet){
  stringstream inRaw;
  for (size_t i = 0; i < strUserIDs.size(); i++){
    inRaw.put(strUserIDs.data()[i]);
  }

  UserList* userListPtr = new UserList();
  if (userListPtr->ParseFromIstream(&inRaw)) {
    for (int i = 0; i < userListPtr->user_size(); i++){
      User* user = userListPtr->mutable_user(i);
      userIDSet.insert(user->uid());
    }
  } else {
    MCE_WARN("AdEdmUserIDIndex::ParseUidSet std exception");
  }
  delete userListPtr;
}

bool AdEdmUserIDIndex::TotalLoadDB() {
  Statement sql;
  sql << " SELECT adgroup_id, uid_set, uid_count, update_time  "
      << " FROM   edm_user_target "
      << " WHERE  flag = 1 AND adgroup_id = " << adgroupID_;
  mysqlpp::StoreQueryResult res;
  try{
    res = QueryRunner(AdEdmUserTargetCacheI::instance().GetDB(), CDbRServer, "edm_user_target").store(sql);
  } catch (Ice::Exception& e){
    MCE_WARN("AdEdmUserIDIndex::TotalLoadDB Ice Exception: " << e);
  } catch (std::exception& e){
    MCE_WARN("AdEdmUserIDIndex::TotalLoadDB std exception: " << e.what());
  }

  if (!res){
    MCE_INFO("AdEdmUserIDIndex::TotalLoadDB --> StoreQueryResult wrong");
    return false;
  }
  if (res.empty()){
    MCE_INFO("AdEdmUserIDIndex::TotalLoadDB --> nothing in db");
    return false;
  }

  for (size_t i = 0; i < res.size(); ++i){
    userCount_ += lexical_cast<Ice::Long> ((mysqlpp::Null<string, mysqlpp::NullIsZero>) res.at(i)["uid_count"]);
    updateTime_ = lexical_cast<Ice::Long> ((mysqlpp::Null<string, mysqlpp::NullIsZero>) res.at(i)["update_time"]);
    ParseUserIDSet(res.at(i)["uid_set"], userIDSet_);
  }
  return true;
}

bool AdEdmUserIDIndex::IncLoadDB() {
  Statement sql;
  sql << " SELECT adgroup_id, uid_set, uid_count, update_time  "
      << " FROM   edm_user_target "
      << " WHERE  flag = 1 AND adgroup_id = " << adgroupID_ 
      << " AND update_time > " << updateTime_;

  mysqlpp::StoreQueryResult res;
  try{
    res = QueryRunner(AdEdmUserTargetCacheI::instance().GetDB(), CDbRServer, "edm_user_target").store(sql);
  } catch (Ice::Exception& e){
    MCE_WARN("AdEdmUserIDIndex::IncLoadDB Ice Exception: " << e);
  } catch (std::exception& e){
    MCE_WARN("AdEdmUserIDIndex::IncLoadDB std exception: " << e.what());
  }

  if (!res){
    MCE_INFO("AdEdmUserIDIndex::IncLoadDB --> StoreQueryResult wrong");
    return false;
  }
  if (res.empty()){
    MCE_INFO("AdEdmUserIDIndex::IncLoadDB --> nothing in db");
    return false;
  }
  
  for (size_t i = 0; i < res.size(); ++i){
    userCount_ += lexical_cast<Ice::Long> ((mysqlpp::Null<string, mysqlpp::NullIsZero>) res.at(i)["uid_count"]);
    Ice::Long updateTime = lexical_cast<Ice::Long> ((mysqlpp::Null<string, mysqlpp::NullIsZero>) res.at(i)["update_time"]);
    updateTime_ = ((updateTime_ > updateTime)?updateTime_ : updateTime);
    ParseUserIDSet(res.at(i)["uid_set"], userIDSet_);
  }

  return true;
}
