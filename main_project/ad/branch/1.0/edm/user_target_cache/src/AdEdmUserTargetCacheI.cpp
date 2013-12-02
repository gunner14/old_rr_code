/*
 * AdEdmUserTargetCacheI.cpp
 *
 *  Created on: 2012-2-23
 *      Author: yongjun.yin
 */

#include <ctime>
#include "AdEdmUserTargetCacheI.h"
#include "LogWrapper.h"
#include "Date.h"
#include "ServiceI.h"
#include "client/linux/handler/exception_handler.h"
#include "../../../util/src/AdCommonUtil.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "IceExt/src/Channel.h"
#include "boost/lexical_cast.hpp"

using namespace com::xiaonei::xce;
using namespace xce::ad;
using namespace MyUtil;
using namespace std;
using namespace boost;

bool MyCallback(const char *dump_path, const char *minidump_id, void *context, bool succeeded) {
  printf("got crash %s %s\n", dump_path, minidump_id);
  return true;
}

void MyUtil::initialize() {
  //google breakpad for dump
  srand(time(NULL));
  static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
  // service start
  ServiceI& service = ServiceI::instance();
  AdEdmUserTargetCacheI::instance().SetDB(service.getCommunicator()->getProperties()->getPropertyWithDefault("DbInstance", "adedm_db"));
  service.getAdapter()->add(&AdEdmUserTargetCacheI::instance(), service.createIdentity("ADEDM", ""));
  TaskManager::instance().execute(new InitTask());
  
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("AdEdmUserTargetCache.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("AdEdmUserTargetCache.Interval",5);
  xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerAdEdmUserTargetCache",&AdEdmUserTargetCacheI::instance(),mod,interval,new EdmChannel());
  MCE_INFO("MyUtil::initialize start ");
}

AdEdmUserTargetCacheI::AdEdmUserTargetCacheI() : initOK_(false){
  MCE_INFO("AdEdmUserTargetCacheI::The Engine started.")
}

void AdEdmUserTargetCacheI::Init(){
  TotalLoadAdGroup();
}

void AdEdmUserTargetCacheI::Reload() {
  IncLoadAdGroup();
}

void AdEdmUserTargetCacheI::TotalLoadAdGroup(){
  MCE_INFO("AdEdmUserTargetCacheI::TotalLoadAdGroup --> start ");
  Statement sql;

  sql << " SELECT    distinct adgroup_id "
      << " FROM      adgroup "
      << " WHERE     member_status = 1 "
      << " AND       am_status = 1 AND audit_status = 1 "
      << " AND       delete_flag = 1 AND daily_sys_status = 1 "
      << " AND       total_sys_status = 1 AND edm_cast_type = 2 AND display_type = 8 "
      << " ORDER BY  adgroup_id ";


  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(GetDB(), CDbRServer, "adgroup").store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN("AdEdmUserTargetCacheI::TotalLoadAdGroup : Ice Exception: " << e);
  } catch (std::exception& e) {
    MCE_WARN("AdEdmUserTargetCacheI::TotalLoadAdGroup : std expeption: " << e.what());
  }

  if (!res) {
    MCE_INFO("AdEdmUserTargetCacheI::TotalLoadAdGroup --> StoreQueryResult wrong");
    return;
  }
  if (res.empty()) {
    MCE_INFO("AdEdmUserTargetCacheI::TotalLoadAdGroup --> nothing in db ");
    return;
  }

  map<Ice::Long, UserIDIndexPtr> adgroupUserIDTempMap;
  //adgroupUserIDTempMap.resize(res.size());
  size_t size = res.size();
  size_t i = 0;
  for (i = 0; i < size; ++i) {
    Ice::Long adgroupID = lexical_cast<Ice::Long> ((mysqlpp::Null<string, mysqlpp::NullIsZero>) (res.at(i)["adgroup_id"]));
    UserIDIndexPtr userIDIndexPtr(new AdEdmUserIDIndex(adgroupID));
    adgroupUserIDTempMap[adgroupID] = userIDIndexPtr;
  }

  for (map<Ice::Long, UserIDIndexPtr>::iterator it = adgroupUserIDTempMap.begin(); it != adgroupUserIDTempMap.end(); ++it) {
    it->second->TotalLoadDB();
  }

  
  IceUtil::RWRecMutex::WLock lock(mutex_);
  {
    adgroupUserIDMap_.swap(adgroupUserIDTempMap);
  }
  MCE_INFO("AdEdmUserTargetCacheI::TotalLoadAdGroup --> done res.size(): "<< res.size());
}

void AdEdmUserTargetCacheI::IncLoadAdGroup(){
  MCE_INFO("AdEdmUserTargetCacheI::IncLoadAdGroup --> start ");
  Statement sql;

  sql << " SELECT    distinct adgroup_id "
      << " FROM      adgroup "
      << " WHERE     member_status = 1 "
      << " AND       am_status = 1 AND audit_status = 1 "
      << " AND       delete_flag = 1 AND daily_sys_status = 1 "
      << " AND       total_sys_status = 1 AND edm_cast_type = 2 AND display_type = 8 "
      << " ORDER BY  adgroup_id ";


  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(GetDB(), CDbRServer, "adgroup").store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN("AdEdmUserTargetCacheI::IncLoadAdGroup Ice Exception: " << e);
  } catch (std::exception& e) {
    MCE_WARN("AdEdmUserTargetCacheI::IncLoadAdGroup std expeption: " << e.what());
  }

  if (!res) {
    MCE_WARN("AdEdmUserTargetCacheI::IncLoadAdGroup --> StoreQueryResult wrong");
    return;
  }
  if (res.empty()) {
    MCE_WARN("AdEdmUserTargetCacheI::IncLoadAdGroup --> nothing in db ");
    return;
  }

  map<Ice::Long, UserIDIndexPtr> adgroupUserIDTempMap;
  
  MCE_INFO("AdEdmUserTargetCacheI::IncLoadAdGroup --> res.size(): "<< res.size());
  MCE_INFO("AdEdmUserTargetCacheI::IncLoadAdGroup --> oldmap.size: "<< adgroupUserIDMap_.size());
  for (size_t i = 0; i < res.size(); ++i) {
    Ice::Long adgroupID =  lexical_cast<Ice::Long> ((mysqlpp::Null<string, mysqlpp::NullIsZero>) (res.at(i)["adgroup_id"]));
    map<Ice::Long, UserIDIndexPtr>::iterator it = adgroupUserIDMap_.find(adgroupID);
    if ( it != adgroupUserIDMap_.end() ) {
      MCE_INFO("AdEdmUserTargetCacheI::IncLoadAdGroup --> found in oldmap: "<< adgroupID);
      adgroupUserIDTempMap[adgroupID] = it->second;
    } else {
      MCE_INFO("AdEdmUserTargetCacheI::IncLoadAdGroup --> add new adgroup: "<< adgroupID);
      UserIDIndexPtr userIDIndexPtr(new AdEdmUserIDIndex(adgroupID));
      adgroupUserIDTempMap[adgroupID] = userIDIndexPtr;
    }

    adgroupUserIDTempMap[adgroupID]->IncLoadDB();
  }
  
  IceUtil::RWRecMutex::WLock lock(mutex_);
  {
    adgroupUserIDMap_.swap(adgroupUserIDTempMap);
  }
  MCE_INFO("AdEdmUserTargetCacheI::IncLoadAdGroup --> done ");
}

//ICE接口 获取投放给用户userID 的 所有广告adgroupID list
AdgroupSeq AdEdmUserTargetCacheI::GetAdSeqByUserID(const int userID, const Ice::Current&) {
  AdgroupSeq adgroupSeq;
  if (!initOK_){
    return adgroupSeq;
  }

  IceUtil::RWRecMutex::RLock lock(mutex_);
  {
    map<Ice::Long, UserIDIndexPtr>::const_iterator mIter = adgroupUserIDMap_.begin();
    for (; mIter != adgroupUserIDMap_.end(); ++mIter){
      if (mIter->second->FindUserID(userID)){
        adgroupSeq.push_back(mIter->first);
        MCE_DEBUG("AdEdmUserTargetCacheI::GetAdSeqByUserID --> get gid:" << mIter->first);
      }	
    }
  }
  return adgroupSeq;
}

//ICE接口 删除指定adgroupID对指定userID的订单关系
bool AdEdmUserTargetCacheI::RmAdgroupUserID(const Ice::Long adgroupID, const int userID, const Ice::Current&) {
  if (!initOK_){
    return false;
  }
  
  IceUtil::RWRecMutex::RLock lock(mutex_);
  {
    if (adgroupUserIDMap_.find(adgroupID) != adgroupUserIDMap_.end()){
      adgroupUserIDMap_[adgroupID]->RmUserID(userID);
      return true;
      //bool result = false;
      //result = AdEdmUserTargetCacheAdapter::instance().RmEdm(adgroupID, userID);
      //return result;
    }
  }

  return false;
}

void InitTask::handle() {
  try {
    MCE_INFO("InitTask::handle --> AdEdmUserTargetCacheI::init start");
    AdEdmUserTargetCacheI::instance().Init();
    AdEdmUserTargetCacheI::instance().InitOK();
    MCE_INFO("InitTask::handle --> AdEdmUserTargetCacheI::init done");
  } catch (std::exception& e) {
    MCE_WARN("InitTask--> handle ERR  line:" << __LINE__ << " err:" << e.what());
  } catch (...) {
    MCE_WARN("InitTask--> handle ERR  line:" << __LINE__);
  }

  usleep(60 * 1000 * 1000);
  TaskManager::instance().execute(new ReloadTask());
}

void ReloadTask::handle() {
  try {
    while(true) {
      MCE_INFO("ReloadTask::handle --> AdEdmUserTargetCacheI::reload start");
      AdEdmUserTargetCacheI::instance().Reload();
      usleep(60 * 1000 * 1000);
      MCE_INFO("ReloadTask::handle --> AdEdmUserTargetCacheI::reload done");
    }
  } catch (std::exception& e) {
    MCE_WARN("ReloadTask--> handle ERR  line:" << __LINE__ << " err:" << e.what());
  } catch (...) {
    MCE_WARN("ReloadTask--> handle ERR  line:" << __LINE__);
  }
}

