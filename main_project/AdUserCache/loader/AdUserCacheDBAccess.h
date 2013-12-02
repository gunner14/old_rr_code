/*************************************************
 *
 *  @Copyright    Renren inc.
 *  @Brief        AdUseCacheLoader模块的数据访问接口层
 *  @Filename     AdUserCacheDBAccess.h
 *  @Author       chaojie.liu
 *  @Email        chaojie.liu@renren-inc.com
 *  @Date         2012-09-08
 *  @History
 *
 *************************************************/

#ifndef __ADUSERCACHE_DB_Access_H__
#define __ADUSERCACHE_DB_Access_H__

#include <iostream>
#include <string> 
#include <set>
#include <map>
#include <IceUtil/IceUtil.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/Thread.h>
#include <Ice/Ice.h>
#include <QueryRunner.h>
#include "ServiceI.h"
#include "AdUserCache.h" 
#include "Singleton.h"
#include "AdUserDataWrapper.h"
#include "FeedMemcProxy/client/user_profile_client.h"
#include <query.h>
#include <utility>
#include <Ice/Config.h>
#include <connection.h>

namespace xce {
namespace ad {

const std::string DB_USER_BORN = "useradscache_user_born";//gender && age
const std::string DB_USER_STAGE = "useradscache_user_born";////stage
const std::string DB_REGION_INFO = "useradscache_region_info";//region
const std::string DB_UNIVERSITY_INFO = "useradscache_university_info";//school && grade
const std::string DB_UNIVERSITY = "useradscache_university_info";
const std::string DB_AD_CHINA_AREA = "useradscache_university_info";
const std::string DB_AD_USER_INFO = "ad_db";
const std::string DB_USER_FRIEND = "reload_friend";//friend relation 
const std::string DB_WORKPLACE_INFO = "useradscache_workplace_info";
const std::string DB_AD_USER_DEBUG = "ads_db";//debug db and offline inference db
const std::string DB_HIGH_SCHOOL_INFO = "useradscache_high_school_info";//高中
const std::string DB_COLLEGE_INFO = "useradscache_college_info";//技校
const std::string DB_JUNIOR_HIGH_SCHOOL_INFO = "useradscache_junior_high_school_info";//初中

//读取性别、生日、家乡信息
class UserBornDBAccess {
  public:
    UserBornDBAccess(const string& pattern, xce::ad::AdUserBaseInfoMap& result) : result_(result){
      sql_ << "SELECT id, gender, birth_year, birth_month, birth_day, home_city FROM user_born WHERE id IN " << pattern;
    }
    void handle();
  private:
    com::xiaonei::xce::Statement sql_;
    AdUserBaseInfoMap &result_;
};

class UserStageDBAccess {
  public:
    UserStageDBAccess(const string& pattern, AdUserBaseInfoMap& result) : result_(result){
      sql_ << "SELECT id, stage FROM user_stage WHERE id IN " << pattern;
    }
    void handle();
  private:
    com::xiaonei::xce::Statement sql_;
    AdUserBaseInfoMap &result_;
};

//读取大学、学历、入学信息
class UniversityDBAccess {
  public:
    UniversityDBAccess(const string& pattern, AdUserBaseInfoMap& result) : result_(result){
      sql_ << "SELECT userid,university_id, type_of_course, max(enroll_year) as enroll_year FROM university_info WHERE userid IN "
           << pattern << " group by userid";
    }
    void handle();
  private:
    com::xiaonei::xce::Statement sql_;
    AdUserBaseInfoMap &result_;
};

//读取公司、工作城市信息
class WorkplaceDBAccess {
  public:
    WorkplaceDBAccess(const string& pattern, AdUserBaseInfoMap& result) : result_(result){
      sql_ << "select userid, city_id, workplace_name from workplace_info where userid in "<<pattern << "order by userid, join_year desc";
    }
    void handle();
  private:
    com::xiaonei::xce::Statement sql_;
    AdUserBaseInfoMap &result_;
};

//读取高中、入学信息
class HighSchoolDBAccess {
  public:
    HighSchoolDBAccess(const string& pattern, AdUserBaseInfoMap& result) : result_(result){
      sql_ << "select userid, high_school_id, max(enroll_year) as enroll_year from high_school_info where userid in " << pattern << " group by userid";
    }
    void handle();
  private:
    com::xiaonei::xce::Statement sql_;
    AdUserBaseInfoMap &result_;
};

//读取初中、入学信息
class JuniorDBAccess {
  public:
    JuniorDBAccess(const string& pattern, AdUserBaseInfoMap& result) : result_(result){
      sql_ << "select  userid, junior_high_school_id, max( junior_high_school_year ) as junior_high_school_year from junior_high_school_info where userid in " << pattern << " group by userid";
    } 
    void handle();
  private:
    com::xiaonei::xce::Statement sql_;
    AdUserBaseInfoMap &result_;
};

class AdUserCacheDBAccess {
  public:
    AdUserCacheDBAccess() {}
    ~AdUserCacheDBAccess() {};

    //与一级缓存memcache交互
    int UpdateMemcache(const UserProfile &upf);

    //与二级缓存DB ad_user_info交互
    int GetFromAduserinfoDB(const vector<int> &user_ids, vector<int> &miss_ids, UserProfileMap &res);
    int UpdateAduserinfoDB(const UserProfile &upf);

    //与源DB交互
    int GetPublicInfoFromSourceDB(map<string, string> &cityname_areacode, map<Ice::Long, string> &universityid_areacode, map<Ice::Long, string> &highschoolid_areacode, map<Ice::Long, string> &juniorschoolid_areacode);
    int GetBaseInfoFromSourceDB(const vector<int> &uid_seq, AdUserBaseInfoMap &res);

    //与DB ad_user_debug交互
    int UpdateDebugDB(const int &uid);
    int GetListFromDebugDB(vector<Ice::Int> &uid_seq);
    int DeleteDebugDB(const int &uid);

    //与DB ad_user_info_offline交互
    int GetFromOfflineAduserinfoDB(const vector<int> &user_ids, UserProfileMap &res);
    int SetGenderToOfflineAduserinfoDB(int userid, int gender);
    int SetFlagToOfflineAduserinfoDB(int uids, int flag);
    int UpdateOfflineAduserinfoDB(UserProfile &upf);

    //与好友关系DB交互
    int GetFriendListFromRelationDB(int uid, vector<int>& friend_ids);


};




}
}

#endif
