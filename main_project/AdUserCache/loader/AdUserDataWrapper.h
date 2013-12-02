#ifndef __ADUSERDATAWRAPPER_H__
#define __ADUSERDATAWRAPPER_H__

#include <iostream>
#include <string>

#include "FeedMemcProxy/client/user_profile_client.h"

using namespace std;
using namespace IceUtil;


namespace xce{
namespace ad{

// 定义缺省值的类
class AdUserDefaultData : virtual public MyUtil::Singleton<AdUserDefaultData> {
public:
  /*
   * @brief:            生成一个用户id为uid的缺省userprofile,其中stage,age,gender采用随机值
   * @param[in]: uid    用户id
   * @return:           userprofile
   */
  UserProfile DefaultUserProfile(int uid);
  
  /* @brief:            生成一个非法的userprofile,用于合法值判断和字段是否需要推理的判断
   *                    字符串类型字段非法值为空串,整数类型字段非法值为-1
   * @return:           非法userprofile
   */
  UserProfile InvalidUserProfile();
};

// AdUserCache Util类
class AdUserCacheUtil : virtual public MyUtil::Singleton<AdUserCacheUtil> {
public:
  /*
   * @brief:            计算字符串MD5
   * @param[in]: str    输入字符串
   * @return:           MD5串
   */
  string GetMD5(const string& str);
};

// 用户公有信息类
class AdUserPublicInfo : virtual public MyUtil::Singleton<AdUserPublicInfo> {
public:
  // 基础函数
  AdUserPublicInfo();
  ~AdUserPublicInfo();

  // Load接口
  int Load();

  // Get接口
  string GetAreacodeByCityName(const string& city_name);
  string GetAreacodeByUniversityID(Ice::Long univid);
  string GetAreacodeByJuniorSchoolID(Ice::Long juniorschool_id);
  string GetAreacodeByHighSchoolID(Ice::Long highschool_id);

private:
  // std::map<Ice::Long, string> regionid_areacode_;
  // 这四个表是预加载进内存的公共信息
  std::map<string, string> cityname_areacode_;
  std::map<Ice::Long, string> universityid_areacode_;
  std::map<Ice::Long, string> highschoolid_areacode_;
  std::map<Ice::Long, string> juniorschoolid_areacode_;

  // 读写锁
  IceUtil::RWRecMutex mutex_;
};

// 用户私有信息类
class AdUserBaseInfo {
public:
  //基础函数
  AdUserBaseInfo();
  ~AdUserBaseInfo();

  void SetUserId(int userid);
  void SetUserBornProperties(const string& gender,
                             int birthyear,
                             int birthmonth,
                             int birthday,
                             const string& home_city);
  void SetUserStageProperties(int stage);
  void SetUniversityProperties(Ice::Long univ_id, const string& type_of_course, int enrollyear);
  void SetWorkPlaceProperties(const string& work_city_id, const string& workplace_name);
  void SetHighSchoolProperties(Ice::Long high_school_id, int high_school_enrollyear);
  void SetJuniorProperties(Ice::Long junior_school_id, int junior_school_enrollyear);

  // 返回公司名个数
  int WorkplaceSize();

public:
  // 用户ID
  int id_;
  //  身份
  //UserIdentity identity_;
  int stage_;
  int grade_;
  // 性别
  string gender_;
  // 生日
  int birthday_;
  int birthmonth_;
  int birthyear_;
  // 家乡
  string home_city_;
  // 工作相关
  string work_city_id_;
  set<string> workplace_name_;
  // 大学
  Ice::Long univ_id_;
  string type_of_course_;
  int univ_enrollyear_;
  // 高中
  Ice::Long high_school_id_;
  int high_school_enrollyear_;
  // 初中
  Ice::Long junior_school_id_;
  int junior_school_enrollyear_;
};

typedef std::map<int, AdUserBaseInfo> AdUserBaseInfoMap;
typedef std::map<int, UserProfile > UserProfileMap;

// 逻辑推断类
class AdUserProfileInference {
public:
  // 基础函数
  AdUserProfileInference();
  virtual ~AdUserProfileInference();
  //核心推断逻辑，对外的唯一接口
  //根据用户基本信息推断UserProfile 
  virtual UserProfile Inference(const AdUserBaseInfo& bi);
private:
  //独立的属性推断函数
  virtual int stage();          // 推断stage
  virtual Ice::Long school();   // 推断学校
  virtual int grade();          // 推断年级
  virtual int gender();         // 推断性别
  virtual int age();            // 推断年龄
  virtual string work_place();  // 推断公司
  virtual string home_area();   // 推断家乡
  virtual string current_area();// 推断目前所在地
private:
  //输入信息
  AdUserBaseInfo bi_;
  //输出信息，通过Inference接口直接输出
  UserProfile upf_;
  //保存从inference db查询到的信息
  UserProfile infe_upf_;
};

} // end namespace ad
} // end namespace xce

#endif
