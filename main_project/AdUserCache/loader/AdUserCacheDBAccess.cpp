/*************************************************
 *
 *  @Copyright    Renren inc.
 *  @Brief        AdUseCacheLoader模块的数据访问接口层实现
 *  @Filename     AdUserCacheDBAccess.cpp
 *  @Author       chaojie.liu
 *  @Email        chaojie.liu@renren-inc.com
 *  @Date         2012-09-08
 *  @History
 *
 *************************************************/

#include "Date.h"
#include "ServiceI.h"
#include "AdUserCacheDBAccess.h"
#include <boost/lexical_cast.hpp>
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
using namespace std;
using namespace IceUtil;
using namespace xce::ad;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace ::xce::serverstate;
using namespace mysqlpp;
using namespace boost;

void UserBornDBAccess::handle(){
  mysqlpp::StoreQueryResult res = QueryRunner(DB_USER_BORN, CDbRServer).store(sql_);
  if (res && res.num_rows() > 0) {
    for (size_t i = 0; i < res.num_rows(); ++i) {
      mysqlpp::Row row = res.at(i);
      try {
        int id = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["id"]);
//        string gender = row["gender"].data();
//        string home_city = row["home_city"].data();
        string gender = (mysqlpp::Null<string, mysqlpp::NullIsBlank>)row["gender"];
        string home_city = (mysqlpp::Null<string, mysqlpp::NullIsBlank>)row["home_city"];
        int birth_year = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["birth_year"]);
        int birth_month = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["birth_month"]);
        int birth_day = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["birth_day"]);
        MCE_INFO("UserBornDBAccess::handle --> userid = " << id << " gender = " << gender << " home_city = " << home_city << " birth_year = " << birth_year << " birth_month = " << birth_month << " birth_day = " << birth_day);
        AdUserBaseInfoMap::iterator it = result_.find(id);
        if ( it == result_.end() ) {
          AdUserBaseInfo obj;
          obj.SetUserId(id);
          obj.SetUserBornProperties(gender, birth_year, birth_month, birth_day, home_city);
          result_[id] = obj;
        } else {
          (it->second).SetUserBornProperties(gender, birth_year, birth_month, birth_day, home_city);
        }
      } catch (...) {
        MCE_WARN("UserBornDBAccess data loading error!");
        continue;
      }
    }
  }
  MCE_DEBUG("UserBornDBAccess::handle --> SetUserBornProperties");
}

void UserStageDBAccess::handle(){
  mysqlpp::StoreQueryResult res = QueryRunner(DB_USER_STAGE, CDbRServer).store(sql_);
  if (res && res.num_rows() > 0) {
    for (size_t i = 0; i < res.num_rows(); ++i) {
      mysqlpp::Row row = res.at(i);
      try {
        int id = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["id"]);
        int stage = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["stage"]);
        MCE_INFO("UserStageDBAccess::handle --> userid = " << id << " stage = " << stage);
        AdUserBaseInfoMap::iterator it = result_.find(id);
        if ( it == result_.end() ) {
          AdUserBaseInfo obj;
          obj.SetUserId(id);
          obj.SetUserStageProperties(stage);
          result_[id] = obj;
        } else {
          (it->second).SetUserStageProperties(stage);
        }
      } catch (...) {
        MCE_WARN("UserStageDBAccess data loading error!");
        continue;
      }
    }
  }
  MCE_DEBUG("UserStageDBAccess::handle --> SetUserStageProperties");
}

void UniversityDBAccess::handle(){
  mysqlpp::StoreQueryResult res = QueryRunner(DB_UNIVERSITY_INFO, CDbRServer).store(sql_);
  if (res && res.num_rows() > 0) {
    for (size_t i = 0; i < res.num_rows(); ++i) {
      mysqlpp::Row row = res.at(i);
     try {
      int id = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["userid"]);
      Ice::Long univ_id = lexical_cast<Ice::Long> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["university_id"]);
      string type_of_course = (mysqlpp::Null<string, mysqlpp::NullIsBlank>)row["type_of_course"];
      int enroll_year = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["enroll_year"]);
      MCE_INFO("UniversityDBAccess::handle --> userid = " << id << " univ_id = " << univ_id << " enroll_year = " << enroll_year << " type_of_course = " << type_of_course);
      AdUserBaseInfoMap::iterator it = result_.find(id);
      if ( it == result_.end() ) {
        AdUserBaseInfo obj;
        obj.SetUserId(id);
        obj.SetUniversityProperties(univ_id, type_of_course,enroll_year);
        result_[id] = obj;
      } else {
        (it->second).SetUniversityProperties(univ_id, type_of_course,enroll_year);
      }
     } catch (...) {
       MCE_WARN("UniversityDBAccess data loading error!");
       continue;
     }
    }
  }
  MCE_DEBUG("UniversityDBAccess::handle --> SetUniversityProperties");
}

void WorkplaceDBAccess::handle(){
  mysqlpp::StoreQueryResult res = QueryRunner(DB_WORKPLACE_INFO, CDbRServer).store(sql_);
  if (res && res.num_rows() > 0) {
    for (size_t i = 0; i < res.num_rows(); ++i) {
      mysqlpp::Row row = res.at(i);
      try {
        int id = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["userid"]);
        string city_id = (mysqlpp::Null<string, mysqlpp::NullIsBlank>)row["city_id"];
        string workplace_name = (mysqlpp::Null<string, mysqlpp::NullIsBlank>)row["workplace_name"];
        MCE_INFO("WorkplaceDBAccess::handle --> userid = " << id << " city_id = " << city_id << " work_place = " << workplace_name);
        AdUserBaseInfoMap::iterator it = result_.find(id);
        if ( it == result_.end() ) {
          AdUserBaseInfo obj;
          obj.SetUserId(id);
          obj.SetWorkPlaceProperties(city_id,workplace_name);
          result_[id] = obj;
        } else {
          if ((it->second).WorkplaceSize() < 5) {
            (it->second).SetWorkPlaceProperties(city_id,workplace_name);
          } 
        }
      } catch (...) {
        MCE_WARN("WorkplaceDBAccess data loading error!");
        continue;
      }
    }
  }
  MCE_DEBUG("WorkplaceDBAccess::handle --> SetWorkPlaceProperties");
}

void JuniorDBAccess::handle(){
  mysqlpp::StoreQueryResult res = QueryRunner(DB_JUNIOR_HIGH_SCHOOL_INFO, CDbRServer).store(sql_);
  if (res && res.num_rows() > 0) {
    for (size_t i = 0; i < res.num_rows(); ++i) {
      mysqlpp::Row row = res.at(i);
      try {
        int id = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["userid"]);
        Ice::Long junior_high_school_id = lexical_cast<Ice::Long> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["junior_high_school_id"]);
        int junior_high_school_year = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["junior_high_school_year"]);
        MCE_INFO("JuniorDBAccess::handle --> userid = " << id << " junior_high_school_id = " << junior_high_school_id << " junior_high_school_year = " << junior_high_school_year);
        AdUserBaseInfoMap::iterator it = result_.find(id);
        if ( it == result_.end() ) {
          AdUserBaseInfo obj;
          obj.SetUserId(id);
          obj.SetJuniorProperties(junior_high_school_id, junior_high_school_year);
          result_[id] = obj;
        } else {
          (it->second).SetJuniorProperties(junior_high_school_id, junior_high_school_year);
        }
      } catch (...) {
        MCE_WARN("JuniorDBAccess data loading error!");
        continue;
      }
    }
  }
  MCE_DEBUG("JuniorDBAccess::handle --> SetJuniorProperties");
}

void HighSchoolDBAccess::handle(){
  mysqlpp::StoreQueryResult res = QueryRunner(DB_HIGH_SCHOOL_INFO, CDbRServer).store(sql_);
  if (res && res.num_rows() > 0) {
    for (size_t i = 0; i < res.num_rows(); ++i) {
      mysqlpp::Row row = res.at(i);
      try {
        int id = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["userid"]);
        Ice::Long high_school_id = lexical_cast<Ice::Long> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["high_school_id"]);
        int enroll_year = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["enroll_year"]);
        MCE_INFO("HighSchoolDBAccess::handle --> userid = " << id << " high_school_id = " << high_school_id << " high_school_enroll_year = " << enroll_year);
        AdUserBaseInfoMap::iterator it = result_.find(id);
        if ( it == result_.end() ) {
          AdUserBaseInfo obj;
          obj.SetUserId(id);
          obj.SetHighSchoolProperties(high_school_id, enroll_year);
          result_[id] = obj;
        } else {
          (it->second).SetHighSchoolProperties(high_school_id, enroll_year);
        }
      } catch (...) {
        MCE_WARN("HighSchoolDBAccess data loading error!");
        continue;
      }
    }
  }
  MCE_DEBUG("HighSchoolDBAccess::handle --> SetHighSchoolProperties");
}

//更新一级缓存memcache的用户定向信息
int AdUserCacheDBAccess::UpdateMemcache(const UserProfile &upf) {
  MCE_DEBUG("AdUserCacheDBAccess::UpdateMemcache");

  try{
    UserProfileClient::instance().Set(upf);
  } catch(...) {
    MCE_WARN("AdUserCacheDBAccess::UpdateMemcache --> UserProfileClient::instance().Set(upf)  exception");
    return 1;
  }   
  return 0;
}

//从二级缓存db ad_user_info获取用户定向信息 
int AdUserCacheDBAccess::GetFromAduserinfoDB(const vector<int> &user_ids, vector<int> &miss_ids, UserProfileMap &result) {
  MCE_DEBUG("AdUserCacheDBAccess::GetFromAduserinfoDB --> size = " << user_ids.size());
  set<int> ids_set;
  set<int> hit_ids;
  com::xiaonei::xce::Statement sql;
//  stringstream sql;
  sql << "select userid, stage, gender, age, current_area, home_area, schoolid, grade, work_place from ad_user_info where userid in";
  stringstream oss;
  oss << "(";
  for (vector<int>::const_iterator it = user_ids.begin(); it != user_ids.end(); ++it) {
    ids_set.insert(*it);
    if (it != user_ids.begin()) {
      oss << ",";
    }
    oss << (*it);
  }
  oss << ")";
  sql << oss.str();
  try {
    mysqlpp::StoreQueryResult res = QueryRunner(DB_AD_USER_INFO, CDbRServer).store(sql);
//  const char* DB_HOST = "10.22.206.99";
//  const char* DB_NAME = "test";
//  const char* DB_USER = "ad";
//  const char* DB_PASSWORD = "adjb###";

//  Connection conn(DB_NAME, DB_HOST, DB_USER, DB_PASSWORD);
//  Query query(&conn, true, sql.str().c_str());
//  StoreQueryResult res = query.store(); 
    if (res && res.num_rows() > 0) {
      for (size_t i = 0; i < res.num_rows(); ++i) {
        mysqlpp::Row row = res.at(i);
        int userid = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["userid"]);
        int stage = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["stage"]);
        int gender = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["gender"]);
        int age = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["age"]);
        string current_area = (mysqlpp::Null<string, mysqlpp::NullIsBlank>)row["current_area"];
        string home_area = (mysqlpp::Null<string, mysqlpp::NullIsBlank>)row["home_area"];
        Ice::Long school = lexical_cast<Ice::Long> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["schoolid"]);
        int grade = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["grade"]);
        string work_place = (mysqlpp::Null<string, mysqlpp::NullIsBlank>)row["work_place"];
        UserProfile upf;
        upf.set_current_area(current_area);
        upf.set_home_area(home_area);
        upf.set_id(userid);
        upf.set_stage(stage);
        upf.set_gender(gender);
        upf.set_age(age);
        upf.set_school(school);  
        upf.set_grade(grade);
        upf.set_work_place(work_place);
        MCE_INFO("AdUserCacheDBAccess::GetFromAduserinfoDB --> RESULT : userid = " << userid << " gender = " << gender << " stage = " << stage << " age = " << age <<  " current_area = " << current_area << " home_area = " << home_area << " school = " << school << " grade = " << grade << " work_place = " << work_place);
        result[userid] = upf;
        ids_set.erase(userid);
        hit_ids.insert(userid);
      }
    }
  } catch (std::exception& e) {
    MCE_WARN("AdUserCacheDBAccess::GetFromAduserinfoDB err:" << e.what());
    return 1;
  } catch (...) {
    MCE_WARN("AdUserCacheDBAccess::GetFromAduserinfoDB --> data loading error!");
    return 1;
  } 
  for (::std::set<int>::iterator it = ids_set.begin(); it != ids_set.end(); ++it) {
    miss_ids.push_back(*it);
  }
  //在二级缓存中命中的用户同时将线下推断库中该用户对应的标志位置为1
  if (!hit_ids.empty()) {  
    int flag = 1;
    for (set<int>::iterator its = hit_ids.begin(); its != hit_ids.end(); ++its) {
      if (1 == SetFlagToOfflineAduserinfoDB(*its,flag)) {
        MCE_WARN("AdUserCacheDBAccess::GetFromAduserinfoDB --> SetFlagToOfflineAduserinfoDB error!");
      }
    }
  }
  return 0;
}

//更新二级缓存db ad_user_info中的用户定向信息
int AdUserCacheDBAccess::UpdateAduserinfoDB(const UserProfile &upf) {
  MCE_DEBUG("AdUserCacheDBAccess::UpdateAduserinfoDB");
  int userid = upf.id();
  int gender = upf.gender();
  int stage = upf.stage();
  string current_area = upf.current_area();
  string home_area = upf.home_area();
  Ice::Long school = upf.school();
  int grade = upf.grade();
  int age = upf.age();
  string work_place = upf.work_place(); 
  MCE_INFO("AdUserCacheDBAccess::UpdateAduserinfoDB -- > RESULT : userid = " << userid << " gender = " << gender << " stage = " << stage << " age = " << age << " home_area = " << home_area << " current_area = " << current_area << " school = " << school << " grade = " << grade << " work_place = " << work_place);
  com::xiaonei::xce::Statement sql;
//  stringstream sql;
  sql << "insert into ad_user_info (userid, stage, gender, age, current_area, home_area, schoolid, grade, work_place) values"
      << "(" << userid<< ", " << stage << ", " << gender << ", " << age << ", " << mysqlpp::quote << current_area << ", " << mysqlpp::quote << home_area << ", " << school << ", " << grade << ", " << "\'" << work_place << "\'" << ")"
      << " on duplicate key update stage = values(stage), gender = values(gender), age = values(age), current_area = values(current_area), home_area = values(home_area), schoolid = values(schoolid), grade = values(grade), work_place = values(work_place)";  
  try{
    QueryRunner(DB_AD_USER_INFO, CDbWServer).store(sql);
//        const char* DB_HOST = "10.22.206.99";
//        const char* DB_NAME = "test";
//        const char* DB_USER = "ad";
//        const char* DB_PASSWORD = "adjb###";

//        Connection conn(DB_NAME, DB_HOST, DB_USER, DB_PASSWORD);
//        Query query(&conn, true, sql.str().c_str());
//        query.store();
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheDBAccess::UpdateAduserinfoDB --> insert into ad_user_info exception : " << e.what());
    return 1;
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheDBAccess::UpdateAduserinfoDB --> data loading error!");
    return 1;
  } 
  return 0;
}

//从源DB获取公共信息
int AdUserCacheDBAccess::GetPublicInfoFromSourceDB(map<string, string> &cityname_areacode, map<Ice::Long, string> &universityid_areacode, map<Ice::Long, string> &highschoolid_areacode, map<Ice::Long, string> &juniorschoolid_areacode) {
  MCE_INFO("AdUserCacheDBAccess::GetPublicInfoFromSourceDB : ");
  com::xiaonei::xce::Statement sql;
  ::std::map<int, string> regionid_areacode;
  //获取“地区编号-地区编码”辅助信息  
  sql << "select region_id, area_code from ad_china_area where region_id != '';";
  mysqlpp::StoreQueryResult res = QueryRunner(DB_AD_CHINA_AREA, CDbRServer).store(sql);
  if (res && res.num_rows() > 0) {
    for (size_t i = 0; i < res.num_rows(); ++i) {
      try {
        mysqlpp::Row row = res.at(i);
        string areacode = (mysqlpp::Null<string, mysqlpp::NullIsBlank>)row["area_code"];
        int regionid = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["region_id"]);
        regionid_areacode[regionid] = areacode;
      } catch (...) {
        mysqlpp::Row row = res.at(i);
        MCE_WARN("AdUserCacheDBAccess::GetPublicInfoFromSourceDB --> select ad_china_area error!   areacode="<<row["area_code"].data()<<"   regionid="<<row["region_id"]);
        continue;
      }
    }   
  }

  sql.clear();
  //获取“城市名称-城市编码”信息
  sql <<"select id, name from region;";
  res = QueryRunner(DB_UNIVERSITY, CDbRServer).store(sql);
  if (res && res.num_rows() > 0) {
    for (size_t i = 0; i < res.num_rows(); ++i){
      try {
      mysqlpp::Row row = res.at(i);
      string city_name = (mysqlpp::Null<string, mysqlpp::NullIsBlank>)row["name"];
      int region_id = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["id"]);
      map<int, string> :: iterator rait = regionid_areacode.find(region_id);
      if(rait != regionid_areacode.end()){
        cityname_areacode[city_name] = rait->second;
          //MCE_DEBUG("user_city_cache_ :  city_name = " << city_name << "   area_code = " << r2ait->second);
      }
         
      } catch (...) {
        MCE_WARN("AdUserCacheDBAccess::GetPublicInfoFromSourceDB --> select region error!");
        continue;
      }
    }   
  }   

  sql.clear();
  //获取“高中编号-高中编码”信息
  sql <<"select id, code from high_school where code != '';";
  res = QueryRunner(DB_UNIVERSITY, CDbRServer).store(sql);
  if (res && res.num_rows() > 0) {
    for (size_t i = 0; i < res.num_rows(); ++i){
      try {
      mysqlpp::Row row = res.at(i);
      Ice::Long high_school_id = lexical_cast<Ice::Long> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["id"]);
      string area_code = (mysqlpp::Null<string, mysqlpp::NullIsBlank>)row["code"];
      highschoolid_areacode[high_school_id] = area_code;
      //MCE_DEBUG("user_highschool_cache_ :  high_school_id = " << high_school_id << "   area_code = " << area_code);
      } catch (...) {
        MCE_WARN("AdUserCacheDBAccess::GetPublicInfoFromSourceDB --> select high_school error!");
        continue;
      }
    }   
  } 

  sql.clear();
  //获取“初中编号-初中编码”信息
  sql <<"select id, code from junior_school where code != '';";
  res = QueryRunner(DB_UNIVERSITY, CDbRServer).store(sql);
  if (res && res.num_rows() > 0) {
    for (size_t i = 0; i < res.num_rows(); ++i){
      try {
      mysqlpp::Row row = res.at(i);
      Ice::Long junior_school_id = lexical_cast<Ice::Long> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["id"]);
      string area_code = (mysqlpp::Null<string, mysqlpp::NullIsBlank>)row["code"];
      juniorschoolid_areacode[junior_school_id] = area_code;
        //MCE_DEBUG("user_juniorschool_cache_ :  junior_school_id = " << junior_school_id << "   area_code = " << area_code);
      } catch (...) {
        MCE_WARN("AdUserCacheDBAccess::GetPublicInfoFromSourceDB --> select junior_school error!");
        continue;
      }
    }
  }  

  sql.clear();
  //获取“大学编号-大学编码”信息
  sql << "select id, region_id from university;";
  res = QueryRunner(DB_UNIVERSITY, CDbRServer).store(sql);
  if (res && res.num_rows() > 0) {
    for (size_t i = 0; i < res.num_rows(); ++i) {
      try {
      mysqlpp::Row row = res.at(i);
      Ice::Long universityid = lexical_cast<Ice::Long> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["id"]);
      int regionid = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["region_id"]);
      map<int, string> :: iterator rait = regionid_areacode.find(regionid);
      if(rait != regionid_areacode.end()){
        universityid_areacode[universityid] = rait->second;
          //MCE_DEBUG("user_city_cache_ :  city_name = " << city_name << "   area_code = " << r2ait->second);
      }   
      } catch (...) {
        MCE_WARN("AdUserCacheDBAccess::GetPublicInfoFromSourceDB --> select university error!");
        continue;
      }
    }
  }
  return 0;
}

//从源DB获取用户基础信息
int AdUserCacheDBAccess::GetBaseInfoFromSourceDB(const vector<int> &uid_seq, AdUserBaseInfoMap &res) {
  MCE_DEBUG("AdUserCacheDBAccess::GetBaseInfoFromSourceDB ----> size="<<uid_seq.size());
  
  ostringstream oss;
  oss << "(";
  for (vector<int>::const_iterator it = uid_seq.begin(); it != uid_seq.end(); ++it) {
    if (it != uid_seq.begin()) {
      oss << ", ";
    }   
    oss << *it;
  }
  oss << ")";
  MCE_DEBUG("start load -->  time = " << MyUtil::Date(time(NULL)).minute() << " : " << MyUtil::Date(time(NULL)).second());
  UserBornDBAccess  born_obj = UserBornDBAccess(oss.str(), res);
  born_obj.handle();
  UserStageDBAccess stage_obj = UserStageDBAccess(oss.str(), res);
  stage_obj.handle();
  UniversityDBAccess univ_obj = UniversityDBAccess(oss.str(), res);
  univ_obj.handle();
  WorkplaceDBAccess workplace_obj = WorkplaceDBAccess(oss.str(), res);
  workplace_obj.handle();
  JuniorDBAccess junior_obj = JuniorDBAccess(oss.str(), res);
  junior_obj.handle();
  HighSchoolDBAccess highschool_obj = HighSchoolDBAccess(oss.str(), res);
  highschool_obj.handle();
  MCE_DEBUG("end load -->  time = " << MyUtil::Date(time(NULL)).minute() << " : " << MyUtil::Date(time(NULL)).second());
  return 0;
}


//更新debug db中的用户信息
int AdUserCacheDBAccess::UpdateDebugDB(const int &uid) {
  MCE_INFO("AdUserCacheDBAccess::UpdateDebugDB ----> uid="<<uid);

  com::xiaonei::xce::Statement sql;
  time_t now = time(0);
  sql << "insert into ad_user_debug (user_id,debug_time) values (" << uid << ", "<< now << ") " << "on duplicate key update debug_time = values(debug_time)";
  try {
    QueryRunner(DB_AD_USER_DEBUG, CDbWServer).store(sql);
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheDBAccess::UpdateDebugDB --> update ad_user_debug exception : " << e.what());
    return 1;
  } catch(...) {
    MCE_WARN("AdUserCacheDBAccess::UpdateDebugDB --> data loading error!");
    return 1;
  } 
  return 0;
}

//从debug db获取调试用户列表
int AdUserCacheDBAccess::GetListFromDebugDB(vector<Ice::Int> &uid_seq) {
  MCE_INFO("AdUserCacheDBAccess::GetListFromDebugDB ----> size = "<< uid_seq.size());

  com::xiaonei::xce::Statement sql;
  sql << "select * from ad_user_debug;";
  try {
    mysqlpp::StoreQueryResult res = QueryRunner(DB_AD_USER_DEBUG, CDbRServer).store(sql);
    if (res && res.num_rows() > 0) {
      for (size_t i = 0; i < res.num_rows(); ++i) {
        mysqlpp::Row row = res.at(i);
        int userid = (int) row["user_id"];
        uid_seq.push_back(userid);
      }
    }
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheDBAccess::GetListFromDebugDB --> get list from ad_user_debug exception : " << e.what());
    return 1;
  } catch(...) {
    MCE_WARN("AdUserCacheDBAccess::GetListFromDebugDB --> data loading error!");
    return 1;
  } 

  
  MCE_DEBUG("AdUserCacheDBAccess::GetListFromDebugDB ----> returned uid size="<<uid_seq.size());

  return 0;
}

//从debug db中删除指定用户调试信息
int AdUserCacheDBAccess::DeleteDebugDB(const int &uid) {
  MCE_INFO("AdUserCacheDBAccess::DeleteDebugDB ----> uid="<<uid);

  com::xiaonei::xce::Statement sql;
  sql << "delete from ad_user_debug where user_id = " << uid;
  try {
    QueryRunner(DB_AD_USER_DEBUG, CDbWServer).store(sql);
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheDBAccess::DeleteDebugDB --> delete from ad_user_debug exception : " << e.what());
    return 1;
  } catch(...) {
    MCE_WARN("AdUserCacheDBAccess::DeleteDebugDB --> data loading error!");
    return 1;
  } 
  return 0; 
}

//从线下推断db ad_user_info_offline获取推断出来的用户定向信息
int AdUserCacheDBAccess::GetFromOfflineAduserinfoDB(const vector<int> &user_ids, UserProfileMap &result) {
  MCE_DEBUG("AdUserCacheDBAccess::GetFromOfflineAduserinfoDB --> size = " << user_ids.size());
  
  com::xiaonei::xce::Statement sql;
  sql << "select userid, stage, gender, age, current_area, home_area, schoolid, grade, work_place from ad_user_info_offline where userid in";
  stringstream oss;
  oss << "(";
  for (vector<int>::const_iterator it = user_ids.begin(); it != user_ids.end(); ++it) {
    if (it != user_ids.begin()) {
      oss << ",";
    }
    oss << (*it);
  }
  oss << ")";
  sql << oss.str();
  try {
    mysqlpp::StoreQueryResult res = QueryRunner(DB_AD_USER_DEBUG, CDbRServer).store(sql);
    if (res && res.num_rows() > 0) {
      for (size_t i = 0; i < res.num_rows(); ++i) {
        mysqlpp::Row row = res.at(i);
        int userid = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["userid"]);
        int stage = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["stage"]);
        int gender = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["gender"]);
        int age = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["age"]);
        string current_area = (mysqlpp::Null<string, mysqlpp::NullIsBlank>)row["current_area"];
        string home_area = (mysqlpp::Null<string, mysqlpp::NullIsBlank>)row["home_area"];
        Ice::Long school = lexical_cast<Ice::Long> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["schoolid"]);
        int grade = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["grade"]);
        string work_place = (mysqlpp::Null<string, mysqlpp::NullIsBlank>)row["work_place"]; 
        UserProfile upf;
        upf.set_current_area(current_area);
        upf.set_home_area(home_area);
        upf.set_id(userid);
        upf.set_stage(stage);
        upf.set_gender(gender);
        upf.set_age(age);
        upf.set_school(school);  
        upf.set_grade(grade);
        upf.set_work_place(work_place);
  
        MCE_INFO("AdUserCacheDBAccess::GetFromOfflineAduserinfoDB --> RESULT : userid = " << userid << " gender = " << gender << " stage = " << stage << " age = " << age <<  " current_area = " << current_area << " home_area = " << home_area << " school = " << school << " grade = " << grade << " work_place = " << work_place);
      result[userid] = upf;
       }
    }
  } catch (std::exception& e) {
    MCE_WARN("AdUserCacheDBAccess::GetFromOfflineAduserinfoDB err:" << e.what());
    return 1;
  } catch (...) {
    MCE_WARN("AdUserCacheDBAccess::GetFromOfflineAduserinfoDB --> get data error!");
    return 1;
  } 
  return 0;
}

//在线下推断db ad_user_info_offline中把相应用户的标志位置位
int AdUserCacheDBAccess::SetFlagToOfflineAduserinfoDB(int userid, int flag) {
  MCE_INFO("AdUserCacheDBAccess::SetFlagToOfflineAduserinfoDB ----> userid = "<< userid << ", flag = " << flag);
  //获取用户非法（非法不是指信息错误，而是用于后续判断的一种标志）定向信息
  UserProfile upf = AdUserDefaultData::instance().InvalidUserProfile();
  int gender = upf.gender();
  int stage = upf.stage();
  string current_area = upf.current_area();
  string home_area = upf.home_area();
  Ice::Long school = upf.school();
  int grade = upf.grade();
  int age = upf.age();
  string work_place = upf.work_place(); 
  //对于线下推断db中还不存在的用户信息则插入，存在只修改
  com::xiaonei::xce::Statement sql;
  sql << "insert into ad_user_info_offline (userid, stage, gender, age, current_area, home_area, schoolid, grade, work_place, flag) values"
      << "(" << userid<< ", " << stage << ", " << gender << ", " << age << ", " << mysqlpp::quote << current_area << ", " << mysqlpp::quote << home_area << ", " << school << ", " << grade << ", " << "\'" << work_place << "\'" << ", " << flag << ")"
      << " on duplicate key update flag = values(flag)";  
  try {
    QueryRunner(DB_AD_USER_DEBUG, CDbWServer).store(sql);
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheDBAccess::SetFlagToOfflineAduserinfoDB --> update ad_user_info_offline exception : " << e.what());
    return 1;
  } catch(...) {  
    MCE_WARN("AdUserCacheDBAccess::SetFlagToOfflineAduserinfoDB --> data loading error!");
    return 1;
  } 
  return 0; 
}

//更新线下推断db ad_user_info_offline的用户定向信息
int AdUserCacheDBAccess::UpdateOfflineAduserinfoDB(UserProfile &upf) {
  int userid = upf.id();
  int gender = upf.gender();
  int stage = upf.stage();
  string current_area = upf.current_area();
  string home_area = upf.home_area();
  Ice::Long school = upf.school();
  int grade = upf.grade();
  int age = upf.age();
  string work_place = upf.work_place(); 
//  MCE_INFO("AdUserCacheDBAccess::UpdateOfflineAduserinfoDB -- > RESULT : userid = " << userid << " gender = " << gender << " stage = " << stage << " age = " << age << " home_area = " << home_area << " current_area = " << current_area << " school = " << school << " grade = " << grade << " work_place = " << work_place);
  
  com::xiaonei::xce::Statement sql;
  sql << "insert into ad_user_info_offline (userid, stage, gender, age, current_area, home_area, schoolid, grade, work_place) values"
      << "(" << userid<< ", " << stage << ", " << gender << ", " << age << ", " << mysqlpp::quote << current_area << ", " << mysqlpp::quote << home_area << ", " << school << ", " << grade << ", " << "\'" << work_place << "\'" << ")"
      << " on duplicate key update stage = values(stage), gender = values(gender), age = values(age), current_area = values(current_area), home_area = values(home_area), schoolid = values(schoolid), grade = values(grade), work_place = values(work_place)";  
  try{
    QueryRunner(DB_AD_USER_DEBUG, CDbWServer).store(sql);
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheDBAccess::UpdateOfflineAduserinfoDB --> insert into ad_user_info_offline exception : " << e.what());
    return 1;
  } catch(...) {
    MCE_WARN("AdUserCacheDBAccess::UpdateOfflineAduserinfoDB --> data loading error!");
    return 1;
  } 
  return 0;  
}

//更新线下推断db ad_user_info_offline中的用户性别信息
int AdUserCacheDBAccess::SetGenderToOfflineAduserinfoDB(int userid, int gender) {
  MCE_INFO("AdUserCacheDBAccess::SetGenderToOfflineAduserinfoDB -- > RESULT : userid = " << userid << " gender = " << gender);
  //获取用户非法（非法不是指信息错误，而是用于后续判断的一种标志）定向信息
  UserProfile upf = AdUserDefaultData::instance().InvalidUserProfile();
  int stage = upf.stage();
  string current_area = upf.current_area();
  string home_area = upf.home_area();
  Ice::Long school = upf.school();
  int grade = upf.grade();
  int age = upf.age();
  string work_place = upf.work_place(); 
  com::xiaonei::xce::Statement sql;
  //对于线下推断db中还不存在的用户信息则插入，存在只修改  
  sql << "insert into ad_user_info_offline (userid, stage, gender, age, current_area, home_area, schoolid, grade, work_place) values"
      << "(" << userid<< ", " << stage << ", " << gender << ", " << age << ", " << mysqlpp::quote << current_area << ", " << mysqlpp::quote << home_area << ", " << school << ", " << grade << ", " << "\'" << work_place << "\'" << ")"
      << " on duplicate key update gender = values(gender)";  
  try{
    QueryRunner(DB_AD_USER_DEBUG, CDbWServer).store(sql);
  } catch(std::exception& e) {
    MCE_WARN("AdUserCacheDBAccess::SetGenderToOfflineAduserinfoDB --> insert into ad_user_info_offline exception : " << e.what());
    return 1;
  } catch(...) {
    MCE_WARN("AdUserCacheDBAccess::SetGenderToOfflineAduserinfoDB --> data loading error!");
    return 1;
  } 
  return 0;  
}

//从好友关系db中获取指定用户的所有好友ID
int AdUserCacheDBAccess::GetFriendListFromRelationDB(int userId, vector<int>& friend_ids) {
  MCE_INFO("AdUserCacheDBAccess::GetFriendListFromRelationDB uid = "<<userId);
  com::xiaonei::xce::Statement sql;
  string tailStr = boost::lexical_cast <string>(userId%100);
  string table_name = "relation_"+tailStr;
  sql<<"select guest from "<<table_name<<" where host="<<userId;
  try {
    mysqlpp::StoreQueryResult res = QueryRunner(DB_USER_FRIEND, CDbRServer, table_name).store(sql);
    if (res && res.num_rows() > 0) {
      for (size_t i = 0; i < res.num_rows(); ++i) {
        mysqlpp::Row row = res.at(i);
        int friend_id = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["guest"]);
        friend_ids.push_back(friend_id);
      }
    }
  } catch (std::exception &e) {
    MCE_WARN("AdUserCacheDBAccess::GetFriendListFromRelationDB --> get relation exception : " << e.what());
    return 1;
  } catch (...) {
    MCE_WARN("AdUserCacheDBAccess::GetFriendListFromRelationDB --> data loading error!");
    return 1;
  } 
  return 0;
}

