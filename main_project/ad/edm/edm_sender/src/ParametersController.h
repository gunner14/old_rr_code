/*
 * ParametersController.h
 *
 *  Created on: 2012-2-24
 *      Author: sunzz
 */

#ifndef PARAMETERSCONTROLLER_H_
#define PARAMETERSCONTROLLER_H_

#include "Util.h"
#include "Singleton.h"
#include "TaskManager.h"
#include <map>
#include <string>
#include "IceLogger.h"
#include "LogWrapper.h"
namespace xce {
namespace ad {

//设置无法从数据库获得的参数
class EngineConfig : public MyUtil::Singleton<EngineConfig> {
public:
  EngineConfig(): db_source_main_("adedm_db_main"), db_source_ads_("adedm_db_ads"), db_source_wr_("db_adwr"),
    server_name_("AdEdmSender0@127.0.0.1"), para_db_host_("10.3.23.69"), 
    edm_zone_id_(2), left_ad_min_price_(100) { // TODO 上线时写读69
  }
  string db_source_main() {
    return db_source_main_;
  }
  string db_source_ads() {
    return db_source_ads_;
  }
  string db_source_wr() {
    return db_source_wr_;
  }
  void set_db_source_main(string db_source) {
    db_source_main_ = db_source;
  }
  void set_db_source_ads(string db_source) {
    db_source_ads_ = db_source;
  }
  void set_db_source_wr(string db_source) {
    db_source_wr_ = db_source;
  }

  void set_para_db_host(string para_db_host) {
    para_db_host_ = para_db_host;
  }
  string para_db_host(){
    return para_db_host_;
  }

  string server_name() {
    return server_name_;
  }
  void set_server_name(int server_index) {
    server_index_ = server_index;
    ostringstream oss;
    oss << "AdEdmSender" << server_index << "@" << GetIp();
    server_name_ = oss.str();
  }
  
  int server_index() {
    return server_index_;
  }

  bool LoadConfig();
  void set_edm_zone_id(Ice::Long zone_id) {
    edm_zone_id_ = zone_id;
  }
  Ice::Long edm_zone_id() {
    return edm_zone_id_;
  }

  void set_left_ad_min_price(int price) {
    left_ad_min_price_ = price;
  }
  int left_ad_min_price() {
    return left_ad_min_price_;
  }

private:
  string db_source_main_;//数据源
  string db_source_ads_;//数据源
  string db_source_wr_;//数据源
  string server_name_;//服务器名称
  string para_db_host_;//参数数据库host
  int server_index_;  //服务Index
  string GetIp();
  //Edm推左策略增加，以下两项配置用于推左log输出
  Ice::Long edm_zone_id_;  //Edm映射到广告系统的逻辑广告位ID
  int left_ad_min_price_;      //Edm推左广告的最小出价
public:
  class ConfigLoadTimer : public MyUtil::Timer{
  public:
    ConfigLoadTimer(int time = 0):Timer(time){}
    virtual void handle() {
      EngineConfig::instance().LoadConfig();
      MyUtil::TaskManager::instance().schedule(new ConfigLoadTimer(60 * 1000));//1分钟更改一次

    }
  };
};

//参数控制中心
class EngineParametersCache : public MyUtil::Singleton<EngineParametersCache>{
  private:
    map<string, string> system_para_;
    IceUtil::RWRecMutex mutex_;
  public:
    void LoadParameters();
    string GetParameterByName(string para_name);
};

class EngineParametersTimer : public MyUtil::Timer {
  public:
    EngineParametersTimer(int time =0 ) :Timer(time){
    }
    virtual void handle() {
      MCE_INFO(" EngineParametersTimer handle :begin");
      EngineParametersCache::instance().LoadParameters();
      MyUtil::TaskManager::instance().schedule(new  EngineParametersTimer(3 * 60 * 1000)); //每3分钟读一次

    }

};

} // end namespace ad
} // end namespace xce


#endif /* PARAMETERSCONTROLLER_H_ */
