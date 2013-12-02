/*
 *  EdmTranslator.h
 *  @Author: jieyu.zeng
 *  @Date: 2012-02-24
 *  @Description: bid unit数据化工具
 *
 */
#ifndef EDMTRANSLATOR_H
#define EDMTRANSLATOR_H

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <iostream>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include "FeedMemcProxy/client/user_profile_client.h"
#include "Singleton.h"
#include "ServiceI.h"

#include "EdmStruct.h"
#include "EdmInvertedIndex.h"
namespace xce {
namespace ad {

using namespace std;

struct TargetUnit {
  Ice::Long gid;
  Ice::Long bid_id;
  Ice::Long member_id;
  int gender;
  int stage;
  string grade;
  string age;
  string school;
  string area;
  string company;
  string interest;
};

class Translator : public MyUtil::Singleton<Translator> {
public:
  static void ProcessTargetUnit(const TargetUnit& unit, const AdGroupPtr& group, DimIndexPtr& dim_index);
  
  static bool ProcessGender(const int gender_code, vector<int>& genders);
  static bool ProcessStage(const int stage_code, vector<int>& stages);
  static bool ProcessGrade(const string& grade_code, vector<int>& grades);
  static bool ProcessAge(const string& age_code, vector<int>& ages);
  static bool ProcessSchool(const string& school_code, vector<uint64_t>& areas, vector<uint64_t>& schools);
  static bool ProcessArea(const string& area_code, vector<uint64_t>& areas);
  //TODO  2012.09.10
  static bool ProcessCompany(const std::string& company_content, std::vector<uint64_t>& companys);
  static bool ProcessInterest(const std::string& interest_content, std::vector<uint64_t>& interests);

private:
  static bool ProcessLeadingArea(const string& area_code, vector<uint64_t>& areas);
  static bool ProcessRealSchool(const string& school_code, vector<uint64_t>& schools);
};

class CommonUtil {
  public:
	static vector<uint64_t> Split(const string& str, boost::char_separator<char> separator); // Split the input with separator, and turn per to unit64_t.
};

} 
} 
#endif // EDMTRANSLATOR_H
