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
#include "EdmLogCacheAdapter.h"
#include <openssl/md5.h>
#include <boost/algorithm/string.hpp>

using namespace xce::ad;

namespace xce {
namespace ad {

const string LINK = "_";

bool LogHandler::handle(MatchHandlerParamter& para) {
  TimeStat ts; 
  TimeStat subts;
  set<SelectedAdGroupPtr> selected_adgroups = para.selected_groups();
  ostringstream send_log;
  UserProfile profile = para.userProfile();
  string ip = LogHelper::instance().MakeIp(profile.ip());
  MyUtil::Date date(time(NULL));
  for(set<SelectedAdGroupPtr>::iterator sit = selected_adgroups.begin();
      sit != selected_adgroups.end(); ++sit){
    AdGroupPtr group_ptr = (*sit)->group();
    ostringstream key;
    key  << profile.id() << LINK << group_ptr->group_id();
    double cost = (*sit)->cost();
    int max_price = group_ptr->max_price();
    if( AdGroup::EDM_LEFT == group_ptr->edm_type() ) {
      cost = 0;
      max_price = 0;
    } 
    
    Ice::Long edm_zone_id = 0;
    if ( AdGroup::EDM_LEFT == group_ptr->edm_type() ) {  //推左广告
      edm_zone_id = EngineConfig::instance().edm_zone_id();
    }
    int left_ad_min_price = EngineConfig::instance().left_ad_min_price();
    
    send_log  << EngineConfig::instance().server_name() << "," << profile.id() << "," << profile.stage() << "," << profile.age() << ","
         << profile.gender() << "," << profile.grade() << "," <<  ((profile.home_area().empty()) ? "-" : profile.home_area()) << ","
         << ((profile.current_area().empty()) ? "-" : profile.current_area()) << "," << ip << "," << group_ptr->member_id() << ","
         << group_ptr->member_category() << "," << group_ptr->member_industry() << ","
         << group_ptr->creative_id() << "," << group_ptr->group_id() << ","
         << group_ptr->campaign_id() << "," << max_price << ","  << cost << "," << group_ptr->trans_type() << ","
         << group_ptr->bid_unit_id() << "," << group_ptr->widget_id() << "," << profile.ip_area() << "," << edm_zone_id << ",0," << date.toDateTime() << ","; 

   /********************************************************************
   * 曝光日志通过新鲜事数据模板传给新鲜事的字段，格式如下：
   * 服务器名称@ip,user_id,stage,age,gender,grade,home_area_code,current_area_code,user_ip,
   * member_id,member_category,member_industry,creative_id,adgroup_id,campaign_id,price,cost,
   * transtype(消费类型),bidunit_id,adzone_id(暂时使用0),cheat_flag(暂时使用0),
   *  曝光时间,index(曝光时处于新鲜事中的第几条) ------->曝光时间和index由新鲜事在曝光时增加
   *
   * 点击日志需要记录的通过点击跳转url传递，格式如下：
   * 服务器名称@ip,----------->服务器名称在点击时打印
   * user_id,stage,age,gender,grade,home_area_code,current_area_code,user_ip,
   * member_id,member_category,member_industry,creative_id,adgroup_id,campaign_id,price,cost,
   * transtype(消费类型),bidunit_id,adzone_id(暂时使用0),
   * cheat_flag(暂时使用0),点击时间,------->cheat_flag和点击时间在点击时打印
   *********************************************************************/

    ostringstream value;
    
    value << profile.id() << "," << profile.stage() << "," << profile.age() << ","
      << profile.gender() << "," << profile.grade() << "," <<  ((profile.home_area().empty()) ? "-" : profile.home_area()) << ","
      << ((profile.current_area().empty()) ? "-" : profile.current_area()) << "," << ip << "," << group_ptr->member_id() << ","
      << group_ptr->member_category() << "," << group_ptr->member_industry() << "," 
      << group_ptr->creative_id() << "," << group_ptr->group_id() << ","
      << group_ptr->campaign_id() << "," << max_price << ",0," << group_ptr->trans_type() << ","  
      << group_ptr->bid_unit_id() << "," << group_ptr->widget_id() << "," << profile.ip_area() << "," << edm_zone_id <<",0"; 
    
    if ( AdGroup::EDM_LEFT == group_ptr->edm_type() ) {  //推左广告  
      value << "|";
      //加入推左广告click log
      value << profile.id() << "," << profile.stage() << "," << profile.age() << "," 
        << profile.gender() << "," << profile.grade() << "," <<  ((profile.home_area().empty()) ? "-" : profile.home_area()) << "," 
        << ((profile.current_area().empty()) ? "-" : profile.current_area()) << "," << ip << "," << group_ptr->member_id() << "," 
        << group_ptr->member_category() << "," << group_ptr->member_industry() << "," 
        << group_ptr->creative_id() << "," << group_ptr->group_id() << "," 
        << group_ptr->campaign_id() << "," << group_ptr->max_price() << "," << (*sit)->cost() << "," << group_ptr->trans_type() << ","  
        << group_ptr->bid_unit_id() << "," << group_ptr->widget_id() << "," << profile.ip_area() << "," << edm_zone_id << ",0," << "CLICKTIME" << ",";

      value << "|";
      //加入推左广告cpc charge log
      value << group_ptr->creative_id() << "," << group_ptr->group_id() << "," << group_ptr->campaign_id() << ","
        << group_ptr->member_id() << "," << edm_zone_id << "," << "0" << "," << "0" << ","
        << edm_zone_id << "," << (*sit)->cost() << "," << "CLICKTIME" << "," << group_ptr->max_price() << ","
        << group_ptr->max_price() << "," << "1" << "," << "lm" << ","
        << left_ad_min_price << "," << group_ptr->bid_unit_id() << ","
        << "0.0.0.0" << "," << profile.id() << "," << "0";
    }
    try {
      if (value.str() == "") {
	      return true;
      }
      EdmLogCacheAdapter::instance().Add(key.str(), value.str());
    } catch (Ice::Exception& e) {                                                
      MCE_WARN("LogHandler::handle --> call EdmLogCacheAdapter error -> "<< e);
    } catch (std::exception& ex) {                                               
      MCE_WARN("LogHandler::handle --> call EdmLogCacheAdapter exception error -> "<< ex.what());
    } catch (...) {
      MCE_WARN("LogHandler::handle --> call EdmLogCacheAdapter failed");  
    } 

  }

  if(send_log.str() == "") {
    return true;
  }

  try {
    AdEdmLogAdapter::instance().Send(send_log.str());
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

}
}
