#include "Date.h"

#include "AdUserDataWrapper.h"
#include "AdUserCacheDBAccess.h"

#include "openssl/md5.h"

using namespace xce::ad;

// start AdUserDefaultData Function Define
// 返回缺省userprofile
UserProfile AdUserDefaultData::DefaultUserProfile(int uid) {
  UserProfile upf;
  string default_current_area = "0086000000000000";
  string default_home_area = "0086000000000000";
  string default_work_place = "";
  int stage = 0;
  int gender = 0;
  int age = 0;

  srand(time(NULL));
  while (0 == stage || 3 == stage) {
    stage = rand() % 5;
  }

  gender = rand() % 2;
  age = 20 + rand() % 10;

  upf.set_id(uid);
  upf.set_stage(stage);
  upf.set_gender(gender);
  upf.set_age(age);
  upf.set_current_area(default_current_area);
  upf.set_school(0);
  upf.set_grade(0);
  upf.set_home_area(default_home_area);
  upf.set_work_place(default_work_place);

  return upf;
}

// 返回非法userprofile
UserProfile AdUserDefaultData::InvalidUserProfile() {
  UserProfile upf;
  string default_current_area = "";
  string default_home_area = "";
  string default_work_place = "";

  upf.set_id(-1);
  upf.set_stage(-1);
  upf.set_gender(-1);
  upf.set_age(-1);
  upf.set_current_area(default_current_area);
  upf.set_school(-1);
  upf.set_grade(-1);
  upf.set_home_area(default_home_area);
  upf.set_work_place(default_work_place);

  return upf;
}
// End AdUserDefaultData Function Define

// Start AdUserCacheUtil Function Define
// 计算字符串md5值
string AdUserCacheUtil::GetMD5(const string &str) {
  MD5_CTX ctx;
  char *data = (char *)str.c_str();
  unsigned char md[1 << 6] = {'\0'};
  char tmp[1 << 4] = {'\0'};
  string ans = "";

  MD5_Init(&ctx);
  MD5_Update(&ctx, data, strlen(data));
  MD5_Final(md, &ctx);
  for (int i = 0; i < 8; ++ i) {
    snprintf(tmp, (1 << 4), "%02x", md[i]);
    ans += tmp;
  }

  return ans;
}
// End AdUserCacheUtil Function Define

// Start AdUserPublicInfo Function Define
AdUserPublicInfo::AdUserPublicInfo() {

}

AdUserPublicInfo::~AdUserPublicInfo() {

}

// 加载publicinfo
int AdUserPublicInfo::Load() {
  int retval = 0;
  int ret = 0;
  std::map<string, string> cityname_areacode;
  std::map<Ice::Long, string> universityid_areacode;
  std::map<Ice::Long, string> highschoolid_areacode;
  std::map<Ice::Long, string> juniorschoolid_areacode;
  AdUserCacheDBAccess access;

  ret = access.GetPublicInfoFromSourceDB(
      cityname_areacode, 
      universityid_areacode,
      highschoolid_areacode,
      juniorschoolid_areacode);
  if (0 == ret) {
    MCE_DEBUG("AdUserPublicInfo::Load -> GetPublicInfoFromSourceDB success");
    IceUtil::RWRecMutex::WLock lock(mutex_);
    cityname_areacode_.swap(cityname_areacode);
    universityid_areacode_.swap(universityid_areacode);
    highschoolid_areacode_.swap(highschoolid_areacode);
    juniorschoolid_areacode_.swap(juniorschoolid_areacode);
  } else {
    MCE_WARN("AdUserPublicInfo::Load -> GetPublicInfoFromSourceDB failed");
    retval = -1;
  }

  return retval;
}


// 根据城市名查地区编码
string AdUserPublicInfo::GetAreacodeByCityName(const string& city_name) {
  string retstr = "";
  MCE_INFO("AdUserPublicInfo::GetAreacodeByCityName --> city_name = " << city_name);
  {
    IceUtil::RWRecMutex::RLock lock(mutex_);
    std::map<string, string>::iterator it = cityname_areacode_.find(city_name);
    if (it != cityname_areacode_.end()) {
      retstr = it->second;
    } else {
      MCE_WARN("AdUserPublicInfo::GetAreacodeByCityName --> miss city_name = "
          << city_name);
    }
  }
  MCE_DEBUG("CityName: " << city_name << "\tAreacode: " << retstr);
  
  return retstr;
}

// 根据大学ID查地区编码
string AdUserPublicInfo::GetAreacodeByUniversityID(Ice::Long univid) {
  string retstr = "";
  MCE_INFO("AdUserPublicInfo::GetAreacodeByUniversityID --> universityid = " << univid);
  {
    IceUtil::RWRecMutex::RLock lock(mutex_);
    std::map<Ice::Long, string>::iterator it = universityid_areacode_.find(univid);
    if (it != universityid_areacode_.end()) {
      retstr = it->second;
    } else {
      MCE_WARN("AdUserPublicInfo::GetAreacodeByUniversityID --> miss universityid = " 
          << univid);
    }  
  }
  MCE_DEBUG("Univid: " << univid << "\tAreaCode: " << retstr);

  return retstr;
}

// 根据大学ID查询地区编码
string AdUserPublicInfo::GetAreacodeByJuniorSchoolID(Ice::Long juniorschool_id) {
  string retstr = "";
  MCE_INFO("AdUserPublicInfo::GetAreacodeByJuniorSchoolID --> juniorschool_id = "
      << juniorschool_id);
  {
    IceUtil::RWRecMutex::RLock lock(mutex_);
    std::map<Ice::Long, string>::iterator it = juniorschoolid_areacode_.find(juniorschool_id);
    if (it != juniorschoolid_areacode_.end()) {
      retstr = it->second;
    } else {
      MCE_WARN("AdUserPublicInfo::GetAreacodeByJuniorSchoolID --> miss juniorschool_id = " 
          << juniorschool_id);
    }  
  }
  MCE_DEBUG("Juniorschool_id: " << juniorschool_id<< "\tAreaCode: " << retstr);

  return retstr;
}

// 根据中学ID查询地区
string AdUserPublicInfo::GetAreacodeByHighSchoolID(Ice::Long highschool_id) {
  string retstr = "";
  MCE_INFO("AdUserPublicInfo::GetAreacodeByHighSchoolID --> highschool_id = "
      << highschool_id);
  {
    IceUtil::RWRecMutex::RLock lock(mutex_);
    std::map<Ice::Long, string>::iterator it = highschoolid_areacode_.find(highschool_id);
    if (it != highschoolid_areacode_.end()) {
      retstr = it->second;
    } else {
      MCE_WARN("AdUserPublicInfo::GetAreacodeByHighSchoolID --> miss highschool_id = " 
          << highschool_id);
    }  
  }
  MCE_DEBUG("Highschool_id: " << highschool_id<< "\tAreaCode: " << retstr);

  return retstr;
}
// End AdUserPublicInfo Function Define



// Start AdUserBaseInfo Function Define
AdUserBaseInfo::AdUserBaseInfo() {
  id_ = 0;
  //identity_ = NONE;
  grade_ = 0;    
  gender_ = "";
  birthday_ = 0;
  birthmonth_ = 0;
  birthyear_ = 0;
  home_city_ = "";
  stage_ = 4; 
  work_city_id_ = "";
  univ_id_ = -1;
  type_of_course_ = "";
  univ_enrollyear_ = 0;
  high_school_id_ = -1;
  high_school_enrollyear_ = 0;  
  junior_school_id_ = -1;
  junior_school_enrollyear_ = 0; 
}

AdUserBaseInfo::~AdUserBaseInfo() {

}

void AdUserBaseInfo::SetUserId(int userid) {
  id_ = userid;
}

void AdUserBaseInfo::SetUserBornProperties(const string& gender,
                                           int birthyear,
                                           int birthmonth,
                                           int birthday,
                                           const string& home_city) {
  gender_ = gender;
  birthyear_ = birthyear;
  birthmonth_ = birthmonth;
  birthday_ = birthday;
  home_city_ = home_city;
}

void AdUserBaseInfo::SetUserStageProperties(int stage) {
  stage_ = stage;
}

void AdUserBaseInfo::SetUniversityProperties(Ice::Long univ_id,
                                             const string& type_of_course,
                                             int enrollyear) {
  univ_id_ = univ_id;
  type_of_course_ = type_of_course;
  univ_enrollyear_ = enrollyear;
}

void AdUserBaseInfo::SetWorkPlaceProperties(const string& work_city_id, const string& workplace_name) {
  work_city_id_ = work_city_id;
  workplace_name_.insert(workplace_name);
}

void AdUserBaseInfo::SetHighSchoolProperties(Ice::Long high_school_id,
                                             int high_school_enrollyear) {
  high_school_id_ = high_school_id;
  high_school_enrollyear_ = high_school_enrollyear;
}

void AdUserBaseInfo::SetJuniorProperties(Ice::Long junior_school_id,
                                         int junior_school_enrollyear) {
  junior_school_id_ = junior_school_id;
  junior_school_enrollyear_ = junior_school_enrollyear;
}

int AdUserBaseInfo::WorkplaceSize() {
  return workplace_name_.size();
}

//End AdUserBaseInfo Function Define


// Start AdUserProfileInference Function Define
AdUserProfileInference::AdUserProfileInference() {
  upf_ = AdUserDefaultData::instance().InvalidUserProfile();
  infe_upf_ = AdUserDefaultData::instance().InvalidUserProfile();
}

AdUserProfileInference::~AdUserProfileInference() {

}

// 根据baseinfo推断userprofile字段
UserProfile AdUserProfileInference::Inference(const AdUserBaseInfo& bi) { 
  MCE_INFO("AdUserProfileInference::Inference: uid = " << bi.id_);

  vector<int> vec_uid;
  UserProfileMap upm;
  AdUserCacheDBAccess access;

  bi_ = bi;
  upf_ = AdUserDefaultData::instance().InvalidUserProfile();
  infe_upf_ = AdUserDefaultData::instance().InvalidUserProfile();

  // 从推断DB查询
  vec_uid.push_back(bi_.id_);
  if (0 == access.GetFromOfflineAduserinfoDB(vec_uid, upm)) {
    if (!upm.empty()) {
      infe_upf_ = (upm.begin())->second;
    }
  } else {
    MCE_WARN("GetFromOfflineAduserinfoDB failed: uid = " << bi_.id_);
  }

  // 以下函数调用顺序很重要
  upf_.set_id(bi_.id_);
  upf_.set_stage(stage());
  upf_.set_school(school());
  upf_.set_grade(grade());
  upf_.set_gender(gender());
  upf_.set_age(age());
  upf_.set_work_place(work_place());
  upf_.set_home_area(home_area());
  upf_.set_current_area(current_area());

  MCE_DEBUG("Inference result ----> uid:" << upf_.id() << "\tstage:" << upf_.stage() << "\tschool:" << upf_.school()
      << "\tgrade: " << upf_.grade() << "\tgender:" << upf_.gender() << "\tage: " << upf_.age()
      << "\twork_place: " << upf_.work_place() << "\thome_area: " << upf_.home_area()
      << "\tcurrent_area" << upf_.current_area());

  return upf_;
}

// stage推断函数
int AdUserProfileInference::stage() {
  MCE_DEBUG("AdUserProfileInference::stage : ");
  int cur_year = MyUtil::Date(time(NULL)).year();
  int stage = 4;
  int grade = 0;
  int flag = 0;

  // 历史原因stage=30需要特殊处理
  if (0 == flag) {
    if (30 == bi_.stage_) {
      stage = 4;
      MCE_DEBUG("stage == 30 Worker AdUserProfileInference::stage : work_city_id : " <<
          bi_.work_city_id_ << " stage = " << stage);
      flag = 1;
    }
  }

  // 如果用户有work_city,则stage=4
  if (0 == flag) {
    if (bi_.work_city_id_.length() && "0" != bi_.work_city_id_) {
      stage = 4;
      MCE_DEBUG("Worker  AdUserProfileInference::stage : work_city_id : " <<
          bi_.work_city_id_ << " stage = " << stage);
      flag = 1;
    }
  }

  // 处理大学情况
  if (0 == flag) {
    if (bi_.univ_id_ > 0) {
      MCE_DEBUG("univ_id_ = " << bi_.univ_id_ << " univ_enrollyear_ = " << bi_.univ_enrollyear_);
      grade = cur_year - bi_.univ_enrollyear_ + 1;
      bool grade_ok = false;
      // 处理大学生
      if ("博士" == bi_.type_of_course_) {  // 处理博士
        if (grade >= 1 && grade <= 4) {
          grade += 60;
          grade_ok = true;
        } else {
          grade = 0;
        }
      } else if ("硕士" == bi_.type_of_course_) {  // 处理硕士
        if  (grade >= 1 && grade <= 4) {
          grade += 50;
          grade_ok = true;
        } else{
          grade = 0;
        }
      } else if ("大学生" == bi_.type_of_course_ || "本科生" == bi_.type_of_course_) {
        if (grade >= 1 && grade <= 5) {
          grade += 40;
          grade_ok = true;
        } else {
          grade = 0;
        }
      } else {  // type_of_course为空或其它值,出于结构清晰考虑,没有与上面分支合并
        if (grade >= 1 && grade <= 5) {
          grade += 40;
          grade_ok = true;
        } else {
          grade = 0;
        }
      }

      if (grade_ok) {
        stage = 2;
        MCE_DEBUG("Univ_stu : grade = " << grade << " stage = " << stage);
        flag = 1;
      }
    }
  }

  // 处理高中
  if (0 == flag) {
    if (bi_.high_school_id_ > 0) {
      MCE_DEBUG("high_school_id_ == " << bi_.high_school_id_);
      grade = cur_year - bi_.high_school_enrollyear_ + 1;
      MCE_DEBUG("HighSchool  high_school_enrollyear_ = " << bi_.high_school_enrollyear_ <<
          " grade = " << grade << " cur_year = " << cur_year);
      if (grade >= 1 && grade <= 4) { // 高中生
        stage = 1;
        grade = grade + 30;
        MCE_DEBUG("HighSchool_stu : grade = " << grade << " stage = " << stage);
        flag = 1;
      } else if (grade >= 5 && grade <= 9) { // 高中毕业成为大学生
        stage = 2;
        grade = grade + 40 - 4;
        flag = 1;
        MCE_DEBUG("After HighSchool Default to be Unive_stu : grade = " << grade
            << " stage = " << stage);
      }
    }
  }

  // 处理初中
  if (0 == flag) {
    if (bi_.junior_school_id_ > 0) {
      MCE_DEBUG("junior_schoool_id_ == " << bi_.junior_school_id_);
      grade = cur_year - bi_.junior_school_enrollyear_ + 1;
      MCE_DEBUG("Junior junior_school_enrollyear_ = " << bi_.junior_school_enrollyear_ <<
          " grade = " << grade<< " cur_year = " << cur_year);
      if (grade >= 1 && grade <= 4) {
        stage = 1;
        grade = grade + 20;
        MCE_DEBUG("Junior_stu : grade = " << grade << " stage = " << stage);
        flag = 1;
      } else if (grade >= 5 && grade <= 8) {
        stage = 1;
        grade = grade + 30 - 4;
        flag = 1;
        MCE_DEBUG("After JuniorSchool Default to be HighSchool_stu : grade = " << grade
            << " stage = " << stage);
      } else if (grade >= 9 && grade <= 13) {
        stage = 2;
        grade = grade + 40 - 8;
        flag = 1;
        MCE_DEBUG("Long Time After JuniorSchool Default to be Univ_stu : grade = " << grade
            << " stage = " << stage);
      }
    }
  }

  // 根据inference Db 填充stage
  if (0 == flag) {
    if (-1 != infe_upf_.stage()) {
      stage = infe_upf_.stage();
      grade = infe_upf_.grade();
      if (grade < 0) {
        grade = 0;
      }
      flag = 1;
    }
  }
  
  // 处理缺省情况
  if (0 == flag) {
    stage = 4;
    grade = 0;
    MCE_DEBUG("NONE__Worker grade = " << grade << " stage = " << stage);
  } 

  MCE_DEBUG("stage: " << stage);
  upf_.set_grade(grade);   

  return stage;
}

// 推断学校
Ice::Long AdUserProfileInference::school() {
  MCE_DEBUG("AdUserProfileInference::school : ");
  string school_id_str = "0000000000";
  string head = "00";
  Ice::Long school_id_long = 0;

  // 只有学生才需要推断school
  if(4 != upf_.stage()) {
    if(2 == upf_.stage() && bi_.univ_id_ > 0) {                 // 处理大学生
      school_id_str = boost::lexical_cast<string>(bi_.univ_id_);
      head = "40";
    } else if (1 == upf_.stage() && bi_.high_school_id_ > 0) {  // 处理高中生
      school_id_str = boost::lexical_cast<string>(bi_.high_school_id_);
      head = "30";
    } else if (1 == upf_.stage() && bi_.junior_school_id_ > 0) {// 处理初中生
      school_id_str = boost::lexical_cast<string>(bi_.junior_school_id_);
      head = "20";
    } else {
    
    }

    // 按10个字符补齐学校ID
    MCE_DEBUG("  school_id_str = " << school_id_str);
    int length = 10 - school_id_str.length();
    if(length >= 0) {
      school_id_str = head + string(length, '0') + school_id_str;
      try {
        school_id_long = boost::lexical_cast<Ice::Long>(school_id_str);
      } catch (exception& e) {
        MCE_WARN("boost::lexical_cast exception  school_id_str = " << school_id_str);
      }
    }
  }
  
  // 无法直接推断,采用线下推断库的值,否则返回0
  if (0 == school_id_long && -1 != infe_upf_.school()) {
    school_id_long = infe_upf_.school();
  }
  MCE_DEBUG("school_id_long: " << school_id_long);

  return school_id_long;
}

// 推断年级
int AdUserProfileInference::grade() {
  int grade = 0;
  MCE_DEBUG("AdUserProfileInference::grade : ");

  if (-1 != upf_.grade()) {                 // 如果推断stage时已经填充此值,则使用它
    grade = upf_.grade();
  } else if (-1 != infe_upf_.grade()) {     // 否则，如果线下库有值,采用线下库值
    grade = infe_upf_.grade();
    MCE_DEBUG("Get grade from inference db");
  } else {                                  // 如果线下库也没有值,采用默认值
    grade = 0;
    MCE_DEBUG("Default grade: " << grade);
  }
  MCE_DEBUG("grade: " << grade);

  return grade;
}

// 推断性别
int AdUserProfileInference::gender() {
  MCE_DEBUG("AdUserProfileInference::gender : ");
  int gender = -1;
  string str_gender = "";
  
  MCE_DEBUG("bi_gender_: " << bi_.gender_);
  
  // 如果用户填写了值,取此值,否则取线下推断库值,如果推断库也没有,则随机赋值
  if ("女生" == bi_.gender_) {
    gender = 0;
  } else if ("男生" == bi_.gender_) {
    gender = 1;
  } else if (-1 != infe_upf_.gender()) {
    MCE_DEBUG("Get gender from inference db");
    gender = infe_upf_.gender();
  } else {
    MCE_DEBUG("Random generate");
    srand(time(NULL));
    gender = rand() % 2;
  }

  // 打Debug日志
  if (0 == gender) {
    str_gender = "女生";
  } else {
    str_gender = "男生";
  }
  MCE_DEBUG("sex = " << str_gender << "  gender = " << gender );
  
  return gender;
}

// 推断年龄
int AdUserProfileInference::age() {
  MCE_DEBUG("AdUserProfileInference::age : "); 
  int age = 25;
  int cur_year = MyUtil::Date(time(NULL)).year();

  age = cur_year - bi_.birthyear_ + 1;
  // 如果age在合法值范围之内,采用此值,否则使用线下库推断值,如果推断库不合法,采用随机值
  if (age > 65 || age < 16) {
    int infe_age = infe_upf_.age();
    if (infe_age > 65 || infe_age < 16) {
      srand(time(NULL));
      age = 20 + (rand() % 10);
      MCE_DEBUG("Random generate age");
    } else {
      age = infe_age;
      MCE_DEBUG("Get age from inference db");
    }
  }
  MCE_DEBUG("birthyear = " << bi_.birthyear_ << " cur_year = " << cur_year << " age = " << age);
  
  return age;
}

// 推断公司
string AdUserProfileInference::work_place() {
  MCE_DEBUG("AdUserProfileInference::work_place: ");
  string workplace = "";
  set<string>::iterator it;
  
  // 把非空公司名md5值以逗号分隔组成字符串
  for (it = bi_.workplace_name_.begin(); it != bi_.workplace_name_.end(); ++it) {
    if ("" != *it) {
      string md5_char = AdUserCacheUtil::instance().GetMD5(*it);
      if (it == bi_.workplace_name_.begin()) {
        workplace = md5_char;
      } else {
        workplace = workplace + "," + md5_char;
      }
    }
  }

  MCE_DEBUG("workplace: " << workplace);
  return workplace;
}

// 推断家乡, 修改一下
string AdUserProfileInference::home_area() {
  MCE_DEBUG("AdUserProfileInference::home_area: ");
  string area = "0086";
  string home_city = bi_.home_city_;  // 从源DB得到用户家乡城市名
  
  // 如果源DB没有家乡城市名,则采用线下推断库值
  if ("" == home_city) {
    MCE_DEBUG("Generate from inference db");
    home_city = infe_upf_.home_area();
  } else {
    MCE_DEBUG("Generate from source db");
  }

  // 根据城市名查地区编码
  if ("" != home_city) {
    MCE_DEBUG("(" << home_city << ")");
    area += AdUserPublicInfo::instance().GetAreacodeByCityName(home_city);
  }

  // 如果源DB和推断DB都没有家乡城市信息,则返回默认地区编码
  if ("0086" == area) {
    area = "0086000000000000";
  }

  return area;
}

// 推断当前所在地编码
string AdUserProfileInference::current_area() {
  MCE_DEBUG("AdUserProfileInference::current_area : ");
  string area = "0086";

  // 已工作人员直接且填写了工作城市,则直接返回工作城市地区编码
  if (4 == upf_.stage() && "" != bi_.work_city_id_) {
    MCE_DEBUG("Get Current Area BY work_place_info's  work_city_id = " << bi_.work_city_id_);
    int length = 12 - bi_.work_city_id_.length();
    area +=  bi_.work_city_id_ + string(length, '0');
  }
  // 大学生且填写了大学ID,则返回大学地区编码
  else if (2 == upf_.stage() && bi_.univ_id_ && bi_.univ_id_ != -1) {
    MCE_DEBUG("GetAreacodeByUniversityID(" << bi_.univ_id_ << ")");
    area += AdUserPublicInfo::instance().GetAreacodeByUniversityID(bi_.univ_id_);
  }
  // 高中生且填写了学校ID,则返回高中学校地区编码
  else if (1 == upf_.stage() && bi_.high_school_id_ && bi_.high_school_id_ != -1) {
    MCE_DEBUG("GetAreacodeByHighSchoolID(" << bi_.high_school_id_ << ")");
    area += AdUserPublicInfo::instance().GetAreacodeByHighSchoolID(bi_.high_school_id_);
  }
  // 初中生且填写了学校ED,则返回初中学校地区编码
  else if (1 == upf_.stage() && bi_.junior_school_id_ && bi_.junior_school_id_ != -1) {
    MCE_DEBUG("GetAreacodeByJuniorSchoolID(" << bi_.junior_school_id_ << ")");
    area += AdUserPublicInfo::instance().GetAreacodeByJuniorSchoolID(bi_.junior_school_id_);
  }
  // 如果线下推断库有当前所在地编码,则返回此值
  else if ("" != infe_upf_.current_area()) {
    MCE_DEBUG("Get Current Area from inference Db(" << infe_upf_.current_area() << ")");
    area = infe_upf_.current_area();
  }
  // 如果填写了家乡城市,则采用家乡所在城市地区编码
  else if ("" != bi_.home_city_) {
    MCE_DEBUG("Get Current Area from home area: GetAreacodeByCityName(" << bi_.home_city_ << ")");
    area += AdUserPublicInfo::instance().GetAreacodeByCityName(bi_.home_city_);
  }

  // 检查当前所在地区编码的有效性,无效则采用默认值
  if ("0086" == area){
    area = "0086000000000000";
    MCE_DEBUG("Use Default Current Area: " << area);
  }

  return area;
}
// End AdUserProfileInference Function Define
