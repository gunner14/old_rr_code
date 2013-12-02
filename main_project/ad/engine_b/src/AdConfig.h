/*
 * AdConfig.h
 *
 *  Created on: 2010-11-2
 *      Author: zhaohui.tang
 */

#ifndef AD_CONFIG_H_
#define AD_CONFIG_H_

#include "Util.h"
#include "Singleton.h"
#include "TaskManager.h"
#include "Evictor.h"
#include <map>

namespace xce {
namespace ad {

class EngineConfig : public MyUtil::Singleton<EngineConfig> {
public:                                       
  EngineConfig(): db_source_ads_("jebe_ads_load"), db_source_main_("jebe_main_load"), NielsonDB_("jebe_ads_load"), sys_min_price_(100),
                  SA_(53), SAA_(43),
                  server_name_("AdEngineB0@127.0.0.1"), server_object_("brand"), url_encode_(true),
                  cpm_esc_pro_(30), cpm_esc_pro75_(30),fresh_creative_ratio_(30),server_index_(-1){
		Ice::Long  zonedata[][3]={  {100000000060,100000000065,100000000093},  
														{100000000069,100000000070,100000000071}, 
														{100000000074,100000000075},   
														{100000000076,100000000077,100000000078} 
													};
		for (size_t i = 0; i < sizeof(zonedata)/sizeof(zonedata[0]); ++i){
			ai_zone_set_.push_back(set<Ice::Long>()); 
			for(size_t j = 0; j < sizeof(zonedata[i])/sizeof(zonedata[i][0]) && zonedata[i][j] != 0;j++){
				ai_zone_set_[i].insert(zonedata[i][j]); 
			} 
		}
		LoadConfig();
    MyUtil::TaskManager::instance().schedule(new xce::ad::EngineConfig::ConfigLoadTimer(60*1000));
  }
  bool url_encode() {
    return url_encode_;
  }
  int SA() {
    return SA_;
  }
  int SAA() {
    return SAA_;
  }
  int sys_min_price() {
    return sys_min_price_;
  }
  string server_name() {
    return server_name_;
  }
  void set_server_name(int server_index) {
    ostringstream oss;
    oss << "AdEngineB" << server_index << "@" << GetIp();
    server_name_ = oss.str();
  }
  void set_server_name(string server_name) {
    server_name_ = server_name;
  }
  bool LoadConfig();
  string GetValue(string key) {
    map<string, string>::iterator it = map_.find(key);
    if (it != map_.end()) {
      return it->second;
    } else {
      return string();
    }
  }
  void set_server_object(string server_object) {
    server_object_ = server_object;
  }
  bool IsForBrand() {
    return server_object_ == "brand";
  }
  int cpm_esc_pro() {
    return cpm_esc_pro_;
  }
  int cpm_esc_pro75() {
    return cpm_esc_pro75_;
  }
	void  GetAIZoneSet(vector<set<Ice::Long> >& ai_zone_set){  
		IceUtil::RWRecMutex::RLock lock(mutex_);
		ai_zone_set = ai_zone_set_;
	}
  int fresh_creative_ratio(){
    return fresh_creative_ratio_;
  }
	void setServerIndex(int server_index){
		server_index_ = server_index;
	}
	int getServerIndex(){
		return server_index_;
	}

  void SetDBSource_Ads(const std::string &ad_db_source){
    db_source_ads_ = ad_db_source;
  }
  string GetDBSource_Ads(){
    return db_source_ads_;
  }
  void SetDBSource_Main(const std::string &ad_db_source)
  {
    db_source_main_ = ad_db_source;
  }
  string GetDBSource_Main(){
    return db_source_main_;
  }

  void SetDBSource_Nielson(const std::string &ad_db_source){
    NielsonDB_ = ad_db_source;
  }
  string GetDBSource_Nielson(){
    return NielsonDB_;
  }

private:
  string db_source_ads_;
  string db_source_main_;
  string NielsonDB_;
  int sys_min_price_;
  int SA_;
  int SAA_;
  string server_name_;
  string server_object_; // 服务对象, 分为品牌/ 自助
  map<string, string> map_;
  bool url_encode_;
  int cpm_esc_pro_;
	vector<set<Ice::Long> >  ai_zone_set_;
  int cpm_esc_pro75_;

	IceUtil::RWRecMutex mutex_;
	int fresh_creative_ratio_;
  int server_index_;

  string GetIp();
	void  ParseAIZoneSet(const string& zoneSetStr);
  class ConfigLoadTimer : public MyUtil::Timer{
  public:
    ConfigLoadTimer(int time = 0):Timer(time){}
    virtual void handle() {
      MyUtil::TaskManager::instance().schedule(new ConfigLoadTimer(60 * 1000));//1分钟更改一次
      EngineConfig::instance().LoadConfig();
    }
  };
};

} // end namespace ad
} // end namespace xce
#endif /* AD_CONFIG_H_ */
