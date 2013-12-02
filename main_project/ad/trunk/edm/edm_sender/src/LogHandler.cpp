/*
 * LogHandler.cpp
 *
 *  Created on: 2012-2-24
 *      Author: ping.zhang
 */

#include "LogHandler.h"
#include <curl/curl.h>
#include "Date.h"
#include "ParametersController.h"
#include "AdEdmLogAdapter.h"
#include <openssl/md5.h>
#include <boost/algorithm/string.hpp>

using namespace xce::ad;

namespace xce {
namespace ad {


bool LogHandler::handle(MatchHandlerParamter& para) {
  TimeStat ts; 
  TimeStat subts;
  set<SelectedAdGroupPtr> selected_adgroups = para.selected_groups();
  ostringstream log;
  UserProfile profile = para.userProfile();
  string ip = LogHelper::instance().MakeIp(profile.ip());
  MyUtil::Date date(time(NULL));
  for(set<SelectedAdGroupPtr>::iterator sit = selected_adgroups.begin();
      sit != selected_adgroups.end(); ++sit){
    AdGroupPtr group_ptr = (*sit)->group();
    log << EngineConfig::instance().server_name() << "," << profile.id() << "," << profile.stage() << "," << profile.age() << ","
        << profile.gender() << "," << profile.grade() << "," <<  ((profile.home_area().empty()) ? "-" : profile.home_area()) << ","
        << ((profile.current_area().empty()) ? "-" : profile.current_area()) << "," << ip << "," << group_ptr->member_id() << ","
        << group_ptr->member_category() << "," << group_ptr->member_industry() << "," 
        << group_ptr->creative_id() << "," << group_ptr->group_id() << ","
        << group_ptr->campaign_id() << "," << group_ptr->max_price() << ","  << (*sit)->cost() << "," << group_ptr->trans_type() << ","  
        << group_ptr->bid_unit_id() << "," << "0" << "," << "0"<< "," << date.toDateTime() << ","; 
  }
  if(log.str() == "") {
    return true;
  }
  try {
    AdEdmLogAdapter::instance().Send(log.str());
  } catch (Ice::Exception& e) {
    MCE_WARN("call AdEdmLogAdapter error -> "<<e);
  } catch (std::exception& ex) {
    MCE_WARN("call AdEdmLogAdapter exception error -> "<<ex.what());
  } catch (...) {
    MCE_WARN("call AdEdmLogAdapter failed");
  }
  return true;
}
/**************************************************** 
 *将URL编码并加双引号
 ****************************************************/
void LogHelper::ProcessUrl(string& str){
  CURL *handle = curl_easy_init();
  char *encodedURL = curl_easy_escape(handle, str.c_str(), 0);
  if (encodedURL){
    str = encodedURL;
    curl_free(encodedURL);
  } else {
    MCE_DEBUG("URL encoding failed");
  }
  curl_easy_cleanup(handle);
}
/***************************************************
 *将int型的ip转为*.*.*.*的类型
 ***************************************************/
string LogHelper::MakeIp(int ip){ 
  int a, b, c, d;
  unsigned ip_u = (unsigned) ip;
  a = ip_u / 16777216;
  b = (ip_u - a * 16777216) / 65536;
  c = (ip_u - a * 16777216 - b * 65536) / 256; 
  d = (ip_u - a * 16777216 - b * 65536 - c * 256);
  ostringstream ios2;
  ios2 << a << "." << b << "." << c << "." << d;
  return ios2.str();
}

/**********************************************************************
 * 获取串的MD5值
 **********************************************************************/
string LogHelper::GetMD5(const string &str){
  MD5_CTX ctx;
  char *data = (char *)str.c_str();
  unsigned char md[16];
  char tmp[3] = {'\0'};
  string ans = "";
  MD5_Init(&ctx);
  MD5_Update(&ctx,data,strlen(data));
  MD5_Final(md,&ctx);
  for (int i=0;i<16;++i) {
    sprintf(tmp,"%02x",md[i]);
    ans += tmp;
  }
  return ans;
}

/***************************************************
 *处理点击请求，打印click log
 ***************************************************/
bool LogHelper::Click(int uid, const string & text) {
  ostringstream log;
  MyUtil::Date date(time(NULL));
  //点击log的格式：text(发送时传入的log信息),cheat_flag(暂时为0),发送时间,
  log << EngineConfig::instance().server_name() << "," << text << "," << "0"<< "," << date.toDateTime() << ",";
  AdEdmLogAdapter::instance().Click(log.str());
  return true;
}

}
}
