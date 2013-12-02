/*
 *  Translator.h
 *
 *   Created on: 2012-04-21
 *       Author: yongjun.yin
 *  Description: 定义转义类 Translator，负责解析DB中数据协议
 */

#ifndef __AD_MOB_TRANSLATOR_H__
#define __AD_MOB_TRANSLATOR_H__

#include "Singleton.h"
#include <vector>
#include "FeedMemcProxy/client/user_profile_client.h"
#include "Struct.h"

using namespace MyUtil;
using namespace xce::ad;

namespace xce{
namespace ad{

class Translator : public MyUtil::Singleton<Translator> {
public:
  static bool ProcessGender(const int gender_code, vector<short>& genders);
  static bool ProcessStage(const int stage_code, vector<short>& stages);
  static bool ProcessGrade(const string& grade_code, vector<short>& grades);
  static bool ProcessAge(const string& age_code, vector<short>& ages);
  static bool ProcessNetwork(const string& network_code, vector<short>& networks);
  static bool ProcessResolution(const string& resolution_code, vector<short>& resolutions);
  
  static bool ProcessSchool(const string& school_code, vector<uint64_t>& areas, vector<uint64_t>& schools);
  static bool ProcessArea(const string& area_code, vector<uint64_t>& areas);
  static bool ProcessPlatform(const string& platform_code, vector<uint64_t>& platforms);
  static bool ProcessBrand3G(const string& brand_code, vector<uint64_t>& brands);
  static bool ProcessLBS(const string& lbs_code, const double radius, vector<Rectangle>& lbs_list);
private:
  static bool ProcessLeadingArea(const string& area_code, vector<uint64_t>& areas);
  static bool ProcessRealSchool(const string& school_code, vector<uint64_t>& schools);
};

}
}
#endif
