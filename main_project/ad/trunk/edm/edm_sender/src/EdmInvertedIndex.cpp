/*
 *  EdmInvertedIndex.cpp
 *  @Author: jieyu.zeng
 *  @Date: 2012-02-24
 *  @Description: Edm 索引
 *
 */

#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <QueryRunner.h>
#include <DbDescriptor.h>
#include "EdmInvertedIndex.h"
#include "EdmSendManagerI.h"
#include "ParametersController.h"
#include "LogWrapper.h"
#include "EdmTranslator.h"
#include "MatchHandler.h"
#include "AdEdmUserTargetCacheAdapter.h"
#include "EdmStructCache.h"
//#include "EdmConfig.h"

using namespace xce::ad;
using namespace com::xiaonei::xce;
using namespace boost;

namespace xce {
namespace ad {

bool IntIndex::Add(const int key, AdGroupPtr group) {
  IceUtil::RWRecMutex::WLock lock(mutex_);
  MCE_DEBUG("[Add] before add: size = " << index_[key].size());
  index_[key].insert(group);
  MCE_DEBUG("[Add] after add: size = " << index_[key].size());
  return true;
}

bool IntIndex::Add(vector<int>& keys, AdGroupPtr group) {
  IceUtil::RWRecMutex::WLock lock(mutex_);
  for(vector<int>::const_iterator it = keys.begin(); it != keys.end(); ++it) {
    index_[*it].insert(group);
  }
  return true;
}

bool IntIndex::Del(vector<int>& keys, AdGroupPtr group) {
  IceUtil::RWRecMutex::WLock lock(mutex_);
  for(vector<int>::const_iterator it = keys.begin(); it != keys.end(); ++it) {
    index_[*it].erase(group);
  }
  return true;
}

set<AdGroupPtr> IntIndex::Get(const int key) {
  IceUtil::RWRecMutex::RLock lock(mutex_);
  hash_map<int, set<AdGroupPtr> >::const_iterator it = index_.find(key);
  if(it != index_.end()) {
    return it->second;
  }
  return set<AdGroupPtr>();
}

set<AdGroupPtr> IntIndex::GetWithAll(const int key) {
  set<AdGroupPtr> result;
  IceUtil::RWRecMutex::RLock lock(mutex_);
  hash_map<int, set<AdGroupPtr> >::const_iterator it = index_.find(key);
  if(it != index_.end()) {
    const set<AdGroupPtr>& key_result = it->second;
    result.insert(key_result.begin(), key_result.end());
  }
  it = index_.find(-1);
  if(it != index_.end()) {
    const set<AdGroupPtr>& key_result = it->second;
    result.insert(key_result.begin(), key_result.end());
  }
  return result;
}

bool UInt64Index::Add(const uint64_t key, AdGroupPtr group) {
  IceUtil::RWRecMutex::WLock lock(mutex_);
  index_[key].insert(group);
  return true;
}

bool UInt64Index::Add(vector<uint64_t>& keys, AdGroupPtr group) {
  IceUtil::RWRecMutex::WLock lock(mutex_);
  for(vector<uint64_t>::const_iterator it = keys.begin(); it != keys.end(); ++it) {
    index_[*it].insert(group);
  }
  return true;
}

bool UInt64Index::Del(vector<uint64_t>& keys, AdGroupPtr group) {
  IceUtil::RWRecMutex::WLock lock(mutex_);
  for(vector<uint64_t>::const_iterator it = keys.begin(); it != keys.end(); ++it) {
    index_[*it].erase(group);
  }
  return true;
}

set<AdGroupPtr> UInt64Index::Get(const uint64_t key) {
  IceUtil::RWRecMutex::RLock lock(mutex_);
  hash_map<uint64_t, set<AdGroupPtr> >::const_iterator it = index_.find(key);
  if(it != index_.end()) {
    return it->second;
  }
  return set<AdGroupPtr>();
}

set<AdGroupPtr> UInt64Index::GetWithAll(const uint64_t key) {
  set<AdGroupPtr> result;
  IceUtil::RWRecMutex::RLock lock(mutex_);
  hash_map<uint64_t, set<AdGroupPtr> >::iterator it = index_.find(key);
  if(it != index_.end()) {
    set<AdGroupPtr>& key_result =it->second;
    result.insert(key_result.begin(), key_result.end());
  }
  it = index_.find(0);
  if(it != index_.end()) {
    set<AdGroupPtr>& key_result =it->second;
    result.insert(key_result.begin(), key_result.end());
  }
  return result;
}

set<AdGroupPtr> AreaUInt64Index::GetWithAll(const uint64_t key) {
  uint64_t province = (key/10000000000)*10000000000;
  uint64_t COUNTRY = 86000000000000;
  
  set<AdGroupPtr> result;
  IceUtil::RWRecMutex::RLock lock(mutex_);
  hash_map<uint64_t, set<AdGroupPtr> >::iterator it = index_.find(key);
  if(it != index_.end()) {
    set<AdGroupPtr>& key_result =it->second;
    result.insert(key_result.begin(), key_result.end());
  }
  if(province != key) {
    it = index_.find(province);
    if(it != index_.end()) {
      set<AdGroupPtr>& key_result =it->second;
      result.insert(key_result.begin(), key_result.end());
    }
  }
  if(province != COUNTRY) {
    it = index_.find(COUNTRY);
    if(it != index_.end()) {
      set<AdGroupPtr>& key_result =it->second;
      result.insert(key_result.begin(), key_result.end());
    }
  }
  return result;
}
/*****************************************
 * 对外获取定向接口
 */
bool EdmInvertedIndex::GetBidGroups(const UserProfile& profile, PubType edm_member_type, set<AdGroupPtr>& result_set) {
  Get(profile, edm_member_type, result_set);
  if(Brand_ad == edm_member_type){////获取定向用户id的广告列表
    vector<Ice::Long> target_uid_gids;
    try {
      target_uid_gids = AdEdmUserTargetCacheAdapter::instance().GetAdSeqByUserID(profile.id());
    } catch (Ice::Exception& e) {
      MCE_WARN("call AdEdmUserTargetCacheAdapter error -> "<<e);
    } catch (std::exception& ex) {
      MCE_WARN("call AdEdmUserTargetCacheAdapter exception error -> "<<ex.what());
    } catch (...) {
      MCE_WARN("call AdEdmUserTargetCacheAdapter failed");
    }
    set<AdGroupPtr> target_groups = AdGroupPool::instance().GetAdGroupsByIds(target_uid_gids);
    result_set.insert(target_groups.begin(), target_groups.end());
  }
  return true;
}

/***********************************************8
* 取定向的EDM
* profile : 用户信息
* edm_member_type : edm类别：产品、品牌、中小
* result_set : 结果集合
* return : 成功与否
*/
bool EdmInvertedIndex::Get(const UserProfile& profile, int edm_member_type, set<AdGroupPtr>& result_set) {
  MCE_DEBUG("EdmInvertedIndex::Get --> uid=" << profile.id() << " gender=" << profile.gender() << " age=" << profile.age() << " stage=" << profile.stage() << " school=" << profile.school() << " grade=" << profile.grade() << " current_area=" << profile.current_area() << " ip_area=" << profile.ip_area());

  vector<AdGroupPtr> result;
  
  DimIndexPtr dim_index;
  {
    IceUtil::RWRecMutex::RLock lock(mutex_);
    hash_map<int, DimIndexPtr>::const_iterator it = edm_index_.find(edm_member_type);
    if (it != edm_index_.end()) {
      dim_index = it->second;
    } else {
      dim_index = NULL;  
    } 
  }
  if (!dim_index) {
    return false;
  }

  int user_stage = profile.stage(); 
  // 符合stage的edm
  const set<AdGroupPtr>& stage = dim_index->stage_index_->Get(user_stage);
  // 符合gender的edm
  const set<AdGroupPtr>& gender = dim_index->gender_index_->Get(profile.gender());

  // 求gender 和 stage 交集
  result.resize(min(gender.size(),stage.size()), NULL);
  vector<AdGroupPtr>::iterator it = set_intersection(gender.begin(), gender.end(), stage.begin(), stage.end(), result.begin());
  result_set.insert(result.begin(), it);

  // 符合age的edm
  const set<AdGroupPtr>& age = dim_index->age_index_->GetWithAll(profile.age());
  // 之前结果和age求交集
  it = set_intersection(result_set.begin(), result_set.end(), age.begin(), age.end(), result.begin());

  result_set.clear();
  result_set.insert(result.begin(), it);

  // 符合地域的广告 : 大学生按school/current_area, 高中生/白领按ip_area.
  if (1 == user_stage || 4 == user_stage) {
    uint64_t ip_area = lexical_cast<uint64_t> (profile.ip_area());
    const set<AdGroupPtr>& area = dim_index->area_index_->GetWithAll(ip_area);
    it = set_intersection(result_set.begin(), result_set.end(), area.begin(), area.end(), result.begin());
    result_set.clear();
    result_set.insert(result.begin(), it);
  } else { // 大学生
    const set<AdGroupPtr>& grade = dim_index->grade_index_->GetWithAll(profile.grade());
    it = set_intersection(result_set.begin(), result_set.end(), grade.begin(), grade.end(), result.begin());
    result_set.clear();
    result_set.insert(result.begin(), it);
    uint64_t school_id = lexical_cast<uint64_t>(profile.school());
    set<AdGroupPtr> school = dim_index->school_index_->GetWithAll(school_id);
    uint64_t school_scope = lexical_cast<uint64_t> (profile.current_area());
    const set<AdGroupPtr>& school_area = dim_index->school_area_index_->GetWithAll(school_scope);
    uint64_t ip_area = lexical_cast<uint64_t> (profile.ip_area());
    const set<AdGroupPtr>& area = dim_index->area_index_->GetWithAll(ip_area);
    //并集
    school.insert(school_area.begin(), school_area.end());
    school.insert(area.begin(), area.end());
    it = set_intersection(result_set.begin(), result_set.end(), school.begin(), school.end(), result.begin());
    result_set.clear();
    result_set.insert(result.begin(), it);
  }

  if (result_set.empty()) {
    MCE_DEBUG("Get no ad: uid=" << profile.id() << " gender=" << profile.gender() << " age=" << profile.age() << " stage=" 
        << profile.stage() << " school=" << profile.school() << " grade=" << profile.grade() << " current_area=" 
        << profile.current_area() << " ip_area=" << profile.ip_area());
  }

  return true;
}

/***********************************************
* 更新
*  
*/
bool EdmInvertedIndex::Load(hash_map<Ice::Long, AdGroupPtr>& groups) {
  TotalLoad(groups); 
  //load_all_ = false; //现在全部全量更新
  //  IncLoad(groups);
  return true;
}
/***********************************************
* 全量更新索引
*  
*/
bool EdmInvertedIndex::TotalLoad(hash_map<Ice::Long, AdGroupPtr>& groups) {
  MCE_INFO("EdmInvertedIndex::TotalLoad() --> START!  groups.size() = " << groups.size());
  Statement sql;
  sql << "select * from bid_unit where delete_flag = 1";
  mysqlpp::StoreQueryResult res;
  const string DB_INSTANCE = EngineConfig::instance().db_source_ads();
  try {
    res = QueryRunner(DB_INSTANCE, CDbRServer,  "bid_unit").store(sql);//TODO
  } catch (Ice::Exception& e) {
    MCE_WARN("EdmInvertedIndex::ReadBidUnitDb() error: " << e);
    return false;
  } catch (std::exception& e) {
    MCE_WARN("EdmInvertedIndex::ReadBidUnitDb() std::exception: " << e.what());
    return false;
  } catch (...) {
    MCE_WARN("EdmInvertedIndex::ReadBidUnitDb() unknown error");
    return false;
  }
  if (!res) {
    MCE_INFO("EdmInvertedIndex::TotalLoad() --> StoryQueryResult wrong");
    return false;
  }
  if (res.empty()) {
    MCE_INFO("EdmInvertedIndex::TotalLoad() --> no ads in bid_unit ");
    return false;
  }

  MCE_INFO("EdmInvertedIndex::TotalLoad() --> sql query res.size = " << res.size());
  int valid_edm = 0;
  hash_map<int, DimIndexPtr> edm_index;
  for(size_t i = 0; i < res.size(); ++i) {
    Ice::Long gid = lexical_cast<Ice::Long> ((mysqlpp::Null<string, mysqlpp::NullIsZero>) res.at(i)["adgroup_id"]);
    Ice::Long bid_id = lexical_cast<Ice::Long> ((mysqlpp::Null<string, mysqlpp::NullIsZero>) res.at(i)["bid_unit_id"]);
    Ice::Long member_id = lexical_cast<Ice::Long> ((mysqlpp::Null<string, mysqlpp::NullIsZero>) res.at(i)["member_id"]);
    int gender = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>) res.at(i)["cast_gender"]);
    int stage = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>) res.at(i)["cast_stage"]);
    string grade = (mysqlpp::Null<string, mysqlpp::NullIsBlank>) (res.at(i)["cast_grade"]);
    string age = (mysqlpp::Null<string, mysqlpp::NullIsBlank>) (res.at(i)["cast_age"]);
    string school = (mysqlpp::Null<string, mysqlpp::NullIsBlank>) (res.at(i)["cast_school"]);
    string area = (mysqlpp::Null<string, mysqlpp::NullIsBlank>) (res.at(i)["cast_area"]);
    if (member_id == 999) { // Filter default EDM adgroup TODO: 这个还需要么？
      continue;
    }
    hash_map<Ice::Long, AdGroupPtr>::iterator mit = groups.find(gid);
    if(mit == groups.end()) { //Filter 无效 EDM adgroup
      continue;
    }
    AdGroupPtr group = mit->second;
    if (8 == group->display_type() && 2 != group->edm_cast_type()) { // This adgroup is EDM adgroup . 且不是uid定向
      MCE_DEBUG("EdmInvertedIndex::TotalLoad() --> adgroup_id=" << gid << " gender " << gender << " stage " << stage 
                << " grade " << grade << " age " << age << " school " << school << " area " << area );
      ++valid_edm;
      group->set_bid_unit_id(bid_id);
      
      vector<int> int_keys;
      vector<uint64_t> uint64_keys;
      int type = group->edm_member_type();;  
      if (edm_index.find(type) == edm_index.end()) {
        edm_index.insert(make_pair(type, new DimIndex()));
      }
      DimIndexPtr dim_index = edm_index[type];
     
      if(Translator::ProcessGender(gender, int_keys)) {
        dim_index->gender_index_->Add(int_keys, group);
        int_keys.clear();
      }
      if(Translator::ProcessStage(stage, int_keys)) {
        dim_index->stage_index_->Add(int_keys, group);
        int_keys.clear();
      }
      if(Translator::ProcessAge(age, int_keys)) {
        dim_index->age_index_->Add(int_keys, group);
        int_keys.clear();
      }
     
      if(2 == stage) {//只有stage == 2时才允许指定学校投放
        vector<uint64_t> uint64_keys2;
        if(Translator::ProcessSchool(school, uint64_keys, uint64_keys2)) {
          dim_index->school_area_index_->Add(uint64_keys, group);
          dim_index->school_index_->Add(uint64_keys2, group);
          uint64_keys.clear();
          uint64_keys2.clear();
        }
        if(Translator::ProcessGrade(grade, int_keys)) {
          dim_index->grade_index_->Add(int_keys, group);
          int_keys.clear();
        }
      } else if((4 == stage) && Translator::ProcessArea(area, uint64_keys)) {
        dim_index->area_index_->Add(uint64_keys, group);
        uint64_keys.clear();
      } else if(6 == stage && Translator::ProcessArea(area, uint64_keys)) {
        dim_index->area_index_->Add(uint64_keys, group);
        uint64_keys.clear();
        int_keys.push_back(-1);
        dim_index->grade_index_->Add(int_keys, group);
        int_keys.clear();
      }
    }
  }

  MCE_INFO("EdmInvertedIndex::TotalLoad() --> valid edm count = " << valid_edm);
  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    edm_index_.swap(edm_index);
    MCE_INFO("EdmInvertedIndex::TotalLoad() --> DONE!");
  }
  return true;
}
/***********************************************
* 增量更新索引
*  
*/
bool EdmInvertedIndex::IncLoad(hash_map<Ice::Long, AdGroupPtr>& groups) {
  
  return true;
}


}
}
