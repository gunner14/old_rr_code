/*************************************************
 *
 *  @Copyright    Renren inc.
 *  @Brief        AdUseCache模块的服务接口层的实现，以及ICE服务的启动和定时加载
 *  @Filename     AdUserCacheLoaderI.cpp
 *  @Author       xiaoqiang.zhu
 *  @Email        xiaoqiang.zhu@renren-inc.com
 *  @Date         2012-08-24
 *  @History
 *
 *************************************************/

#include "AdUserCacheLoaderI.h"
#include "AdUserCacheDBAccess.h"
#include "AdUserDataWrapper.h"
#include "AdUserList.h"
//#include "FeedMemcProxy/client/user_profile.pb.h"
#include "FeedMemcProxy/client/user_profile_client.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>

#include "Date.h"
#include "ServiceI.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

using namespace std;
using namespace IceUtil;
using namespace xce::ad;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace ::xce::serverstate;


/*
 *  @Brief:  AdUserCache服务的初始化函数，涉及加载服务、启动定时任务
 */
void MyUtil::initialize() {
  MCE_INFO("Call MyUtil::initialize() in AdUserCacheLoader");

  //ICE Run Time
  ServiceI& service = ServiceI::instance();
  try {
    service.getAdapter()->add(&(AdUserCacheLoaderI::instance()),
        service.createIdentity("M", ""));
  } catch(std::exception& e) {
    MCE_WARN("MyUtil::initialize err:" << e.what());
  } catch(...) {
    MCE_WARN("MyUtil::initialize() ----> service.getAdapter() exception");
  }

  //启动定时更新AdUserPublicInfo的任务，延迟时间是24h
  try {
    int reset_interval_ =
      service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault(
          "Service." + ServiceI::instance().getName()
          + ".ADUSERPUBLICINFO_RESET_TIME_INTERVAL", 3600*24*1000);
    TaskManager::instance().scheduleRepeated(new AdUserPublicInfoResetTimer(
          reset_interval_));
  } catch(std::exception& e) {
    MCE_WARN("MyUtil::initialize err:" << e.what());
  } catch(...) {
    MCE_WARN("MyUtil::initialize() ----> TaskManager::instance().scheduleRepeated() exception");
  }
  int ret;
  try {
    ret = AdUserUpdate::instance().Init();
    if (ret == 0) {
      MCE_INFO("AdUserUpdate Init success");
    } else { 
      MCE_INFO("AdUserUpdate Init failed");
    }
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheLoaderI::initialize err:" << e.what());
  } catch(...) {
    MCE_WARN("AdUserCacheLoaderI::initialize ----> AdUserUpdate Init Fail");
  }
  //server initialize
  try {
    int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("AdUserCacheLoader.Mod");
    int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("AdUserCacheLoader.Interval", 5);
    ServerStateSubscriber::instance().initialize("ControllerAdUserCacheLoaderR",&AdUserCacheLoaderI::instance(),mod,interval, 
        new XceFeedControllerChannel);
    MCE_INFO("mod:" << mod << " interval:" << interval);
  } catch(std::exception& e) {
    MCE_WARN("MyUtil::initialize err:" << e.what());
  } catch(...) {
    MCE_WARN("MyUtil::initialize() ----> ServerStateSubscriber::instance().initialize exception");
  }
}


void AdUserPublicInfoResetTimer::handle() {
  MCE_INFO("AdUserPublicInfoRestTimer::handle ----> Reload all AdPublicInfo");
  AdUserCacheLoaderI::instance().initialize();
}

int AdUserCacheLoaderI::initialize() {
  int ret = 0;

  MCE_INFO("AdUserCacheLoaderI::initialize ----> Start Load AdUserPublicInfo");
  try {
    ret = AdUserPublicInfo::instance().Load();
    if (ret == 0) {
      MCE_INFO("Load AdUserPublicInfo success");
    } else {
      MCE_INFO("Load AdUserPublicInfo failed");
    }
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheLoaderI::initialize err:" << e.what());
  } catch(...) {
    MCE_WARN("AdUserCacheLoaderI::initialize ----> Load AdUserPublicInfo Fail");
  }

  MCE_INFO("AdUserCacheLoaderI::initialize ----> Start AdUserUpdate Init");

  return ret;
}

string AdUserCacheLoaderI::GetById(int userId, const Ice::Current& current) {
  MCE_DEBUG("AdUserCacheLoaderI::GetById ----> userId="<<userId);
  int ret = 0;
  timeval start_, end_;
  gettimeofday(&start_, NULL);  

  vector<int> userIds;
  userIds.push_back(userId);
  vector<int> missed_userIds;
  UserProfile upf;
  UserProfileMap upfMap;
  AdUserCacheDBAccess dba;

  //首先从二级缓存DB ad_user_info查询
  /*  try {
      ret = dba.GetFromAduserinfoDB(userIds, missed_userIds, upfMap); 
      if (ret != 0) MCE_WARN("AdUserCacheLoaderI::GetById  ----> GetFromAduserinfoDB exception");
      } catch(...) {
      MCE_WARN("AdUserCacheLoaderI::GetById  ----> GetFromAduserinfoDB exception");
      }

  //二级缓存查到了该用户信息,此时其id肯定为userId
  if (missed_userIds.empty()) {
  upf = upfMap[userId];

  //更新一级缓存memcache
  try {
  ret = dba.UpdateMemcache(upf);
  if (ret != 0) MCE_WARN("AdUserCacheLoaderI::GetById  ----> UpdateMemcache exception" );
  } catch(...) {
  MCE_WARN("AdUserCacheLoaderI::GetById  ----> UpdateMemcache exception" );
  }

  }
  //二级缓存miss掉了该用户的定向信息，则从直接返回一个缺省的userprofile，
  //并将该用户加入到查询队列，在队列里面异步地去查询源DB、推断，更新二级缓存和一级缓存
  else {*/
  //生成一个缺省值
  try {
    upf = AdUserDefaultData::instance().DefaultUserProfile(userId);
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheLoaderI::GetById err:" << e.what());
  } catch(...) {
    MCE_WARN("AdUserCacheLoaderI::GetById  ----> Get DefaultUserProfile exception");
  }

  //将该用户加入到查询队列
  try {
    AdUserUpdate::instance().PutId(userId);
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheLoaderI::GetById err:" << e.what());
  } catch(...) {
    MCE_WARN("AdUserCacheLoaderI::GetById  ----> AdUserUpdate::instance().PutId exception");
  }
  //  }

  //序列化并返回用户定向信息
  string upfStr;
  try {
    upf.SerializeToString(&upfStr);
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheLoaderI::GetById err:" << e.what());
  } catch(...) {
    upfStr = "";
    MCE_WARN("AdUserCacheLoaderI::GetById  ----> SerializeToString exception");
  }
  gettimeofday(&end_, NULL);
  float timeuse=1000000*(end_.tv_sec-start_.tv_sec)+end_.tv_usec-start_.tv_usec;
  timeuse/=1000;
  MCE_INFO("|" << userId << "|" << timeuse << "|");
  return upfStr;
}

int AdUserCacheLoaderI::LoadFromAduserinfoDBByIdSeq(const vector<int>& userIds, const Ice::Current& current) {
  MCE_INFO("AdUserCacheLoaderI::LoadFromAduserinfoDBByIdSeq  ----> size="<<userIds.size());
  int ret = 0;

  AdUserCacheDBAccess dba;
  vector<int> missed_userIds;
  UserProfileMap upfMap;

  //首先从二级缓存DB ad_user_info查询
  try {
    ret = dba.GetFromAduserinfoDB(userIds, missed_userIds, upfMap);
    if (ret != 0) MCE_WARN("AdUserCacheLoaderI::LoadFromAduserinfoDBByIdSeq  ----> GetFromAduserinfoDB exception" );
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheLoaderI::LoadFromAduserinfoDBByIdSeq err:" << e.what());
  } catch(...) {
    MCE_WARN("AdUserCacheLoaderI::LoadFromAduserinfoDBByIdSeq  ----> GetFromAduserinfoDB exception" );
  }  

  //二级缓存miss掉了部分用户的定向信息，则从源DB查询，并经过推断，获得其定向信息
  if (!missed_userIds.empty()) {
    AdUserBaseInfoMap ubiMap;
    try {
      ret = dba.GetBaseInfoFromSourceDB(missed_userIds, ubiMap);
      if (ret != 0) MCE_WARN("AdUserCacheLoaderI::LoadFromAduserinfoDBByIdSeq  ----> GetBaseInfoFromSourceDB exception" );
    } catch(std::exception& e) {
      MCE_WARN("AdUserCacheLoaderI::LoadFromAduserinfoDBByIdSeq err:" << e.what());
    } catch(...) {
      MCE_WARN("AdUserCacheLoaderI::LoadFromAduserinfoDBByIdSeq  ----> GetBaseInfoFromSourceDB exception" );
    }    

    //如果源DB也查不到，说明是空号用户，则返回
    if (ubiMap.empty()) {
      ostringstream oss;
      oss << "(";
      for (vector<int>::iterator it = missed_userIds.begin(); it != missed_userIds.end(); ++it ) {
        if (it !=  missed_userIds.begin()) {
          oss<<",";
        }
        oss<<(*it);
      }
      oss << ")";
      MCE_INFO("Not found in source DB  ----> size="<<missed_userIds.size()<<" userIds="<<oss.str());
    }

    //根据源DB获得的用户基础信息，推断其定向信息
    else {
      AdUserProfileInference upfInference; 
      UserProfile upf;

      for (AdUserBaseInfoMap::iterator it = ubiMap.begin(); it != ubiMap.end(); ++it) {
        //推断
        try {
          upf = upfInference.Inference(it->second);
        } catch(std::exception& e) {
          MCE_WARN("AdUserCacheLoaderI::LoadFromAduserinfoDBByIdSeq err:" << e.what());
        } catch(...) {
          MCE_WARN("AdUserCacheLoaderI::LoadFromAduserinfoDBByIdSeq  ----> Inference exception");
          continue;
        }
        upfMap[it->first] = upf;

        //更新二级缓存DB ad_user_info
        try {
          ret = dba.UpdateAduserinfoDB(upf);
          if (ret != 0) MCE_WARN("AdUserCacheLoaderI::LoadFromAduserinfoDBByIdSeq  ----> UpdateAduserinfoDB exception" );
        } catch(std::exception& e) {
          MCE_WARN("AdUserCacheLoaderI::LoadFromAduserinfoDBByIdSeq err:" << e.what());
        } catch(...) {
          MCE_WARN("AdUserCacheLoaderI::LoadFromAduserinfoDBByIdSeq  ----> UpdateAduserinfoDB exception" );
        }
      }	
    }
  }

  //更新一级缓存memcache
  for (UserProfileMap::iterator it = upfMap.begin(); it != upfMap.end(); ++it) {
    try {
      ret = dba.UpdateMemcache( it->second );
      if (ret != 0) MCE_WARN("AdUserCacheLoaderI::LoadFromAduserinfoDBByIdSeq  ----> UpdateMemcache exception" );
    } catch(std::exception& e) {
      MCE_WARN("AdUserCacheLoaderI::LoadFromAduserinfoDBByIdSeq err:" << e.what());
    } catch(...) {
      MCE_WARN("AdUserCacheLoaderI::LoadFromAduserinfoDBByIdSeq  ----> UpdateMemcache exception" );
    }    
  }  

  return ret;
}

int AdUserCacheLoaderI::LoadFromAduserinfoDBById(int userId, const Ice::Current& current) {
  MCE_INFO("AdUserCacheLoaderI::LoadFromAduserinfoDBById  ----> userId="<<userId);
  int ret = 0;

  vector<int> userIds;
  userIds.push_back(userId);  

  ret = LoadFromAduserinfoDBByIdSeq(userIds);

  return ret;
}

int AdUserCacheLoaderI::LoadFromSourceDBByIdSeq(const vector<int>& userIds, const Ice::Current& current) {
  MCE_INFO("AdUserCacheLoaderI::LoadFromSourceDBByIdSeq  ----> size="<<userIds.size());
  int ret = 0;

  UserProfile upf;
  AdUserBaseInfoMap ubiMap;
  AdUserProfileInference upfInference; 
  AdUserCacheDBAccess dba;

  //从源DB中查找用户的基础信息
  try {
    ret = dba.GetBaseInfoFromSourceDB(userIds, ubiMap);
    if (ret != 0) MCE_WARN("AdUserCacheLoaderI::LoadFromSourceDBByIdSeq  ----> GetBaseInfoFromSourceDB exception" );
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheLoaderI::LoadFromSourceDBByIdSeq err:" << e.what());
  } catch(...) {
    MCE_WARN("AdUserCacheLoaderI::LoadFromSourceDBByIdSeq  ----> GetBaseInfoFromSourceDB exception" );
  }

  //根据用户的基础信息，推断其定向信息
  for (AdUserBaseInfoMap::iterator it = ubiMap.begin(); it != ubiMap.end(); ++it) {
    //推断
    try {
      upf = upfInference.Inference(it->second);
    } catch(std::exception& e) {
      MCE_WARN("AdUserCacheLoaderI::LoadFromSourceDBByIdSeq err:" << e.what());
    } catch(...) {
      MCE_WARN("AdUserCacheLoaderI::LoadFromSourceDBByIdSeq  ----> Inference exception" );
    }

    //更新二级缓存DB ad_user_info
    try {
      ret = dba.UpdateAduserinfoDB(upf);
      if (ret != 0) MCE_WARN("AdUserCacheLoaderI::LoadFromSourceDBByIdSeq  ----> UpdateAduserinfoDB exception" );
    } catch(std::exception& e) {
      MCE_WARN("AdUserCacheLoaderI::LoadFromSourceDBByIdSeq err:" << e.what());
    } catch(...) {
      MCE_WARN("AdUserCacheLoaderI::LoadFromSourceDBByIdSeq  ----> UpdateAduserinfoDB exception" );
    }

    //更新一级缓存memcache
    try {
      ret = dba.UpdateMemcache(upf);
      if (ret != 0) MCE_WARN("AdUserCacheLoaderI::LoadFromSourceDBByIdSeq  ----> UpdateMemcache exception" );
    } catch(std::exception& e) {
      MCE_WARN("AdUserCacheLoaderI::LoadFromSourceDBByIdSeq err:" << e.what());
    } catch(...) {
      MCE_WARN("AdUserCacheLoaderI::LoadFromSourceDBByIdSeq  ----> UpdateMemcache exception" );
    }
  }  

  return ret;
}

int AdUserCacheLoaderI::LoadFromSourceDBById(int userId, const Ice::Current& current) {
  MCE_INFO("AdUserCacheLoaderI::LoadFromSourceDBById  ----> userId="<<userId);
  int ret = 0;

  vector<int> userIds;
  userIds.push_back(userId);  

  ret = LoadFromSourceDBByIdSeq(userIds);

  return ret;
}
/*
int AdUserCacheLoaderI::SetSeq(const vector<UserProfile>& upfseq){
  MCE_INFO("AdUserCacheLoaderI::SetSeq  ----> size="<< upfseq.size());
  int ret = 0;

  vector<int> userIds;
  AdUserCacheDBAccess dba;
  UserProfile upf;

  for (vector<UserProfile>::const_iterator it = upfseq.begin(); it != upfseq.end(); ++it) {
    upf = *it;
    //更新二级缓存DB ad_user_info
    try {
      ret = dba.UpdateAduserinfoDB(upf);
      if (ret != 0) MCE_WARN("AdUserCacheLoaderI::SetSeq  ----> UpdateAduserinfoDB exception" );
    } catch(...) {
      MCE_WARN("AdUserCacheLoaderI::SetSeq  ----> UpdateAduserinfoDB exception" );
    }

    //更新一级缓存memcache
    try {
      ret = dba.UpdateMemcache(upf);
      if (ret != 0) MCE_WARN("AdUserCacheLoaderI::SetSeq  ----> UpdateMemcache exception" );
    } catch(...) {
      MCE_WARN("AdUserCacheLoaderI::SetSeq  ----> UpdateMemcache exception" );
    }

    userIds.push_back(upf.id());
  }

  //更新Debug DB
  try {
    ret = dba.UpdateDebugDB(userIds);
    if (ret != 0) MCE_WARN("AdUserCacheLoaderI::SetSeq  ----> UpdateDebugDB exception" );
  } catch(...) {
    MCE_WARN("AdUserCacheLoaderI::SetSeq  ----> UpdateDebugDB exception" );
  }

  return ret;
}
*/

//int AdUserCacheLoaderI::Set(const string& userProfileString, const Ice::Current& current) {
int AdUserCacheLoaderI::Set(const UserInfo& userinfo, int &flag, const Ice::Current& current) {
  MCE_INFO("AdUserCacheLoaderI::Set ----> uid="<<userinfo.userid);
  int ret = 0;
  flag = -1;
  vector<int> userIds;
  AdUserBaseInfoMap ubiMap;
  AdUserCacheDBAccess dba;
  userIds.push_back(userinfo.userid);

  //查找源DB，判断用户是否是空号
  try {
    ret = dba.GetBaseInfoFromSourceDB(userIds, ubiMap);
    if (ret != 0) MCE_WARN("AdUserCacheLoaderI::Set  ----> GetBaseInfoFromSourceDB exception" );
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheLoaderI::Set err:" << e.what());
  } catch(...) {
    MCE_WARN("AdUserCacheLoaderI::Set  ----> GetBaseInfoFromSourceDB exception" );
  }
  if (ubiMap.empty()) {
    flag = 0;
    MCE_INFO("AdUserCacheLoaderI::Set ----> user="<<userinfo.userid<<"  is Null");
    return 1;
  } else {
    flag = 1;
  }

  UserProfile upf;
  upf.set_current_area(userinfo.currentArea);
  upf.set_home_area(userinfo.homeArea);
  upf.set_id(userinfo.userid);
  upf.set_stage(userinfo.stage);
  upf.set_gender(userinfo.gender);
  upf.set_age(userinfo.age);
  upf.set_school(userinfo.school);  
  upf.set_grade(userinfo.grade);
  upf.set_work_place(userinfo.workPlace);
  MCE_INFO("AdUserCacheLoaderI::Set  ----> userId="<<upf.id());

  vector<UserProfile> upfseq;
  try {
    ret = dba.UpdateAduserinfoDB(upf);
    if (ret != 0) MCE_WARN("AdUserCacheLoaderI::SetSeq  ----> UpdateAduserinfoDB exception" );
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheLoaderI::Set err:" << e.what());
  } catch(...) {
    MCE_WARN("AdUserCacheLoaderI::Set  ----> UpdateAduserinfoDB exception" );
  }

  //更新一级缓存memcache
  try {
    ret = dba.UpdateMemcache(upf);
    if (ret != 0) MCE_WARN("AdUserCacheLoaderI::Set  ----> UpdateMemcache exception" );
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheLoaderI::Set err:" << e.what());
  } catch(...) {
    MCE_WARN("AdUserCacheLoaderI::Set  ----> UpdateMemcache exception" );
  }
  try {
    ret = dba.UpdateDebugDB(userinfo.userid);
    if (ret != 0) MCE_WARN("AdUserCacheLoaderI::Set  ----> UpdateDebugDB exception" );
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheLoaderI::Set err:" << e.what());
  } catch(...) {
    MCE_WARN("AdUserCacheLoaderI::Set  ----> UpdateDebugDB exception" );
  }
  return ret;
}
/*
int AdUserCacheLoaderI::ResetSeq(const vector<int>& userIds) {
  MCE_INFO("AdUserCacheLoaderI::ResetSeq  ----> size="<<userIds.size());
  int ret = 0;

  //首先调用LoadFromSourceDBByIdSeq方法从源DB加载数据冲刷掉set值
  ret = LoadFromSourceDBByIdSeq(userIds);
  if (ret != 0) {
    MCE_WARN("AdUserCacheLoaderI::ResetSeq  ----> LoadFromSourceDBByIdSeq exception" ); 
    return ret;
  }

  //其次从Debug DB中擦除掉这些用户
  AdUserCacheDBAccess dba;
  try {
    ret = dba.DeleteDebugDB(userIds);
    if (ret != 0) MCE_WARN("AdUserCacheLoaderI::ResetSeq  ----> DeleteDebugDB exception" ); 
  } catch(...) {
    MCE_WARN("AdUserCacheLoaderI::ResetSeq  ----> DeleteDebugDB exception" ); 
  }

  return ret;
}*/

int AdUserCacheLoaderI::Reset(int userId, const Ice::Current& current) {
  MCE_INFO("AdUserCacheLoaderI::Reset  ----> userId="<<userId);
  int ret = 0;

  AdUserCacheDBAccess dba;

  ret = LoadFromSourceDBById(userId);
  if (ret != 0) {
    MCE_WARN("AdUserCacheLoaderI::Reset  ----> LoadFromSourceDBByIdSeq exception" ); 
    return ret;
  }

  //其次从Debug DB中擦除掉这些用户
  try {
    ret = dba.DeleteDebugDB(userId);
    if (ret != 0) MCE_WARN("AdUserCacheLoaderI::Reset  ----> DeleteDebugDB exception" ); 
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheLoaderI::Reset err:" << e.what());
  } catch(...) {
    MCE_WARN("AdUserCacheLoaderI::Reset  ----> DeleteDebugDB exception" ); 
  }

  return ret;
}

int AdUserCacheLoaderI::GetDebuggerList(vector<UserInfo>& debugList, const Ice::Current& current) {
  MCE_INFO("AdUserCacheLoaderI::GetDebuggerList");
  int ret = 0;
  vector<Ice::Int> userIds;
  vector<int> missed_userIds;
  UserProfileMap upfMap;
  AdUserCacheDBAccess dba;
  try {
    ret = dba.GetListFromDebugDB(userIds);
    if (ret != 0) {
      MCE_WARN("AdUserCacheLoaderI::GetDebuggerList  ----> GetListFromDebugDB exception" ); 
    }
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheLoaderI::GetDebuggerList err:" << e.what());
  } catch(...) {
    MCE_WARN("AdUserCacheLoaderI::GetDebuggerList  ----> GetListFromDebugDB exception" );  
  }

  //首先从二级缓存DB ad_user_info查询
  if (!userIds.empty()) {
    try {
      ret = dba.GetFromAduserinfoDB(userIds, missed_userIds, upfMap);
      if (ret != 0) MCE_WARN("AdUserCacheLoaderI::GetDebuggerList ----> GetFromAduserinfoDB exception" );
    } catch(std::exception& e) {
      MCE_WARN("AdUserCacheLoaderI::GetDebuggerList err:" << e.what());
    } catch(...) {
      MCE_WARN("AdUserCacheLoaderI::GetDebuggerList ----> GetFromAduserinfoDB exception" );
    }  
    for (UserProfileMap::iterator it = upfMap.begin(); it != upfMap.end(); ++it) {
      UserInfo userinfo;
      userinfo.userid = it->second.id();
      userinfo.gender = it->second.gender();
      userinfo.stage = it->second.stage();
      userinfo.grade = it->second.grade();
      userinfo.age = it->second.age();
      userinfo.school = it->second.school();
      userinfo.homeArea = it->second.home_area();
      userinfo.currentArea = it->second.current_area();
      userinfo.workPlace = it->second.work_place();
      debugList.push_back(userinfo);
    }
  }
  return ret;
}

int AdUserCacheLoaderI::GetByIdOffline(int userId, int& flag, UserInfo& userinfo, const Ice::Current& current) {
  MCE_INFO("AdUserCacheLoaderI::GetByIdOffline  ----> uid="<<userId);
  vector<int> uids; 
  AdUserBaseInfoMap res; 
  uids.push_back(userId); 
  vector<int> miss_ids; 
  UserProfileMap upfMap;  
  AdUserCacheDBAccess db_obj; 
  UserProfile upf; 
  flag = -1;
  //从ad_user_info中读取用户定向信息 
  if (db_obj.GetFromAduserinfoDB(uids, miss_ids, upfMap)) { 
    MCE_WARN("AdUserUpdate::GetByIdOffline --> GetFromAduserinfoDB error!"); 
  }  
 
  //二级缓存查到了该用户信息,此时其id肯定为userId 
  if (miss_ids.empty()) { 
    upf = upfMap[userId]; 
    flag = 1;
    //更新一级缓存memcache 
    if (db_obj.UpdateMemcache(upf)) { 
      MCE_WARN("AdUserUpdate::GetByIdOffline ----> UpdateMemcache error!" ); 
    }    
  } else { 
    //从源DB获取对应的用户基础信息 
    if (db_obj.GetBaseInfoFromSourceDB(uids, res)) { 
      MCE_WARN("AdUserUpdate::GetByIdOffline --> GetBaseInfoFromSourceDB error!"); 
    } 
    if (res.empty()) {
      flag = 0;
    } else {
      AdUserProfileInference infer_obj; 
      //根据用户基础信息生成定向信息 
      upf = infer_obj.Inference(res[userId]); 
      flag = 1;
      //更新二级缓存ad_user_info 
      if (db_obj.UpdateAduserinfoDB(upf)) { 
        MCE_WARN("AdUserUpdate::GetByIdOffline --> UpdateAduserinfoDB error!"); 
      } 
      //更新一级缓存memcache 
      if (db_obj.UpdateMemcache(upf)) { 
        MCE_WARN("AdUserUpdate::GetByIdOffline --> UpdateMemcache error!"); 
      }
    } 
  }
  if (flag == 1) {
    userinfo.userid = upf.id();
    userinfo.gender = upf.gender();
    userinfo.stage = upf.stage();
    userinfo.grade = upf.grade();
    userinfo.age = upf.age();
    userinfo.school = upf.school();
    userinfo.homeArea = upf.home_area();
    userinfo.currentArea = upf.current_area();
    userinfo.workPlace = upf.work_place();
  } else {
  }
  return 0;   
}  

int AdUserCacheLoaderI::OfflineInference(int userId, const Ice::Current& current) {
  MCE_INFO("AdUserCacheLoaderI::OfflineInference  ----> uid="<<userId);
  int ret =0;

  AdUserCacheDBAccess dba;
  stringstream oss;

  //缺省填充一些非法值，供后续判断
  UserProfile upf=AdUserDefaultData::instance().InvalidUserProfile();

  //设置用户id
  upf.set_id(userId);

  //从好友列表数据库中获取用户的好友列表
  vector<int> friend_ids;
  try {
    ret = dba.GetFriendListFromRelationDB(userId,friend_ids);
    if (ret != 0) MCE_WARN("AdUserCacheLoaderI::OfflineInference  ----> GetFriendListFromRelationDB exception"); 
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheLoaderI::OfflineInference err:" << e.what());
  } catch(...) {
    MCE_WARN("AdUserCacheLoaderI::OfflineInference  ----> GetFriendListFromRelationDB exception");
  }

  //如果好友个数太少，则推断是没有统计意义的，放弃推断，直接返回
  int threshold = 20;
  if (friend_ids.size() < threshold) {
    MCE_DEBUG("AdUserCacheLoaderI::OfflineInference ----> firend size="<<friend_ids.size()<<" too little, refuse inference");
    return ret;
  }

  //从源DB中查找好友的基础信息
  AdUserBaseInfoMap ubiMap;
  try {
    ret = dba.GetBaseInfoFromSourceDB(friend_ids, ubiMap);
    if (ret != 0) MCE_WARN("AdUserCacheLoaderI::OfflineInference  ----> GetBaseInfoFromSourceDB exception" );
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheLoaderI::OfflineInference err:" << e.what());
  } catch(...) {
    MCE_WARN("AdUserCacheLoaderI::OfflineInference  ----> GetBaseInfoFromSourceDB exception" );
  }

  //后续推断中公共使用的信息
  int cur_year = MyUtil::Date(time(NULL)).year();

  //年龄推断
  int friend_age[100]={0};
  const int min_age = 16;
  const int max_age = 65;

  for (AdUserBaseInfoMap::iterator it = ubiMap.begin(); it != ubiMap.end(); ++it) {
    AdUserBaseInfo bi = it->second;
    int age = cur_year - bi.birthyear_ + 1;
    if (age > max_age || age < min_age) {
      age = 0;
    }
    friend_age[age]++;
  }

  int user_age = min_age ;
  oss.clear();
  for (int i = min_age; i <= max_age; ++i) {
    if ( friend_age[user_age] < friend_age[i] ) {
      user_age = i;
    }
    if (friend_age[i] > 0) {
      oss<<i<<":"<<friend_age[i]<<" ";
    }
  }
  upf.set_age(user_age);
  MCE_DEBUG("AdUserCacheLoaderI::OfflineInference  ----> friend-age-distribution  "<<oss.str());
  MCE_DEBUG("AdUserCacheLoaderI::OfflineInference  ----> inferenced age="<<user_age);


  //性别推断
  //int user_gender = -1;
  //upf.set_gender(user_gender);


  //用户好友的阶段、年级与预处理
  map<int,int> friend_stage;
  map<int,int> friend_grade;
  for (AdUserBaseInfoMap::iterator it = ubiMap.begin(); it != ubiMap.end(); ++it) {
    int uid = it->first;
    AdUserBaseInfo bi = it->second;
    //处理白领
    if (bi.work_city_id_.length() && "0" != bi.work_city_id_) {
      friend_stage[uid] = 4;               //白领
      friend_grade[uid] = 0;
    }
    //处理大学生
    else if (bi.univ_id_ && -1 != bi.univ_id_){
      int grade = cur_year - bi.univ_enrollyear_ +1 ;
      if ("博士" == bi.type_of_course_) {
        if (grade >= 1 && grade <= 4) {
          friend_stage[uid] = 2;           //博士研究生
          friend_grade[uid] = 60 + grade;
        } else {
          friend_stage[uid] = 4;           //毕业了默认为工作
          friend_grade[uid] = 0;
        }
      } else if ("硕士" == bi.type_of_course_) {
        if (grade >= 1 && grade <= 4) {
          friend_stage[uid] = 2;           //硕士研究生
          friend_grade[uid] = 50 + grade;
        } else {
          friend_stage[uid] = 4;           //毕业了默认为工作
          friend_grade[uid] = 0;
        }
      } else if ("大学生" == bi.type_of_course_ || "本科生" == bi.type_of_course_) {
        if (grade >= 1 && grade <= 5) {
          friend_stage[uid] = 2;           //在校大学生
          friend_grade[uid] = 40 + grade;
        } else {
          friend_stage[uid] = 4;           //毕业了默认为工作
          friend_grade[uid] = 0;
        }
      } else {                             //如果缺失了type_of_course，则按照大学生来推
        if (grade >= 1 && grade <= 5) {
          friend_stage[uid] = 2;           //在校大学生
          friend_grade[uid] = 40 + grade;
        } else {
          friend_stage[uid] = 4;           //毕业了默认为工作
          friend_grade[uid] = 0;
        }
      }

    }
    //处理高中生
    else if (bi.high_school_id_ && -1 != bi.high_school_id_) {
      int grade = cur_year - bi.high_school_enrollyear_ + 1;
      if (grade >= 1 && grade <= 4) {
        friend_stage[uid] = 1;             //在校中学生
        friend_grade[uid] = 30 + grade;
      } else if(grade >= 5 && grade <= 9) {//高中毕业了，成为大学生
        friend_stage[uid] = 2;
        friend_grade[uid] = 40+grade-4;    
      } else {                             //其余情况默认为白领
        friend_stage[uid] = 4;
        friend_grade[uid] = 0;
      }
    }
    //处理初中生
    else if (bi.junior_school_id_ && -1!=bi.junior_school_id_) {
      int grade = cur_year - bi.junior_school_enrollyear_ +1;
      if (grade >= 1 && grade <= 4) {
        friend_stage[uid] = 1;             //在校中学生
        friend_grade[uid] = 20 + grade;
      } else if (grade >= 5 && grade <= 8) {  //初中毕业了，成为高中生
        friend_stage[uid] = 1;             
        friend_grade[uid] = 30 + grade -4;
      } else if (grade >= 9 && grade <= 13) { //高中毕业了，成为大学生
        friend_stage[uid] = 2;
        friend_grade[uid] = 40 + grade -8;
      } else {                             //其余情况默认为白领
        friend_stage[uid] = 4;
        friend_grade[uid] = 0;
      }
    }
    //缺省情况：白领
    else {
      friend_stage[uid] = 4;
      friend_grade[uid] = 0;
    }
  } 

  //阶段推断
  int user_stage = -1;
  int stage_distr[5]={0,0,0,0,0};//stage取值为[1 2 4]中的一个
  for (map<int,int>::iterator it = friend_stage.begin(); it != friend_stage.end(); ++it) {
    stage_distr[it->second]++;
  }
  user_stage = 1;
  oss.clear();
  for (int i=1; i<5; i++) {
    if (stage_distr[user_stage] < stage_distr[i]) {
      user_stage = i;
    }
    oss<<i<<":"<<stage_distr[i]<<" ";
  }
  upf.set_stage(user_stage);
  MCE_DEBUG("AdUserCacheLoaderI::OfflineInference  ----> friend-stage-distribution  "<<oss.str());
  MCE_DEBUG("AdUserCacheLoaderI::OfflineInference  ----> inferenced stage="<<user_stage);

  //年级推断
  //int user_grade = -1;
  //upf.set_grade(user_grade);

  //学校推断
  //long user_school_code=4000000000;
  //upf.set_school(user_school_code);

  //老家推断
  //string user_home_area = "0086000000000000";
  //upf.set_home_area(user_home_area);

  //当前所在地推断
  //string user_curr_area = "0086000000000000";
  //upf.set_current_area(user_curr_area);

  //工作地点推断
  //string user_work_place = "";
  //upf.set_work_place(user_work_place);

  //将推断结果写入Offline的Inference DB
  try {
    ret = dba.UpdateOfflineAduserinfoDB(upf);
    if (ret != 0) MCE_WARN("AdUserCacheLoaderI::OfflineInference  ----> UpdateOfflineAduserinfoDB exception" );
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheLoaderI::OfflineInference err:" << e.what());
  } catch(...) {
    MCE_WARN("AdUserCacheLoaderI::OfflineInference  ----> UpdateOfflineAduserinfoDB exception" );
  }

  return ret;
}


int AdUserCacheLoaderI::SetOfflineGender(int userId, int gender, const Ice::Current& current) {
  MCE_INFO("AdUserCacheLoaderI::SetOfflineGender");
  int ret = 0;
  if (gender !=0 || gender != 1) {
    MCE_WARN("AdUserCacheLoaderI::SetOfflineGender ----> gender must be 1(man) or 0(woman)");
    ret = -1;
    return ret;
  }

  vector<int> userIds;
  AdUserBaseInfoMap ubiMap;
  AdUserCacheDBAccess dba;
  userIds.push_back(userId);

  //查找源DB，判断用户是否是空号
  try {
    ret = dba.GetBaseInfoFromSourceDB(userIds, ubiMap);
    if (ret != 0) MCE_WARN("AdUserCacheLoaderI::SetOfflineGender ----> GetBaseInfoFromSourceDB exception" );
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheLoaderI::SetOfflineGender err:" << e.what());
  } catch(...) {
    MCE_WARN("AdUserCacheLoaderI::SetOfflineGender ----> GetBaseInfoFromSourceDB exception" );
  }
  if (ubiMap.empty()) {
    MCE_INFO("AdUserCacheLoaderI::SetOfflineGender ----> user="<<userId<<"  is Null");
    return -1;
  }

  //插入到OfflineDB中去
  try {
    ret = dba.SetGenderToOfflineAduserinfoDB(userId, gender);
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheLoaderI::SetOfflineGender err:" << e.what());
  } catch (...) {
    MCE_WARN("AdUserCacheLoaderI::SetOfflineGender ----> SetGenderToOfflineAduserinfoDB exception");
  }
  return ret;
}




