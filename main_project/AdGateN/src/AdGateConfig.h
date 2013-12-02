#ifndef _AD_GATE_CONFIG_H_
#define _AD_GATE_CONFIG_H_
#include <netdb.h>
#include <arpa/inet.h>
#include "Singleton.h"
#include "TaskManager.h"
#include "IceLogger.h"
#include <map>
namespace xce {
namespace ad {

class GateConfig : public MyUtil::Singleton<GateConfig> {
public:
  GateConfig();
  /*string DB() {
    return DB_;
  }
  void set_DB(string DB) {
    DB_ = DB;
  }*/
  string server_name() {
    return server_name_;
  }
  void set_server_name(string server_name) {
    ostringstream oss;
    oss << server_name << "@" << GetIp();
    server_name_ = oss.str();
  }
  int ping_seq_size() {
    return ping_seq_size_;
  }
  string engine_host() {
    return engine_host_;
  }
  int engine_port() {
    return engine_port_;
  }
  bool env_test() {
    return env_test_;
  }
  
  string flush_db() {
    return flush_db_;
  }
  string GetFlushDBSource() {
    return flush_dbsource_;
  }
  bool LoadConfig();
private:
  string GetIp();
  //string DB_;
  string server_name_;
  int ping_seq_size_;
  string engine_host_;
  int engine_port_;
  bool env_test_;
  string flush_db_;
  string flush_dbsource_;

  class GateConfigLoadTimer : public MyUtil::Timer{
  public:
    GateConfigLoadTimer(int time = 0):Timer(time){}
    virtual void handle() {
      MyUtil::TaskManager::instance().schedule(new GateConfigLoadTimer(60*1000));//1分钟
      GateConfig::instance().LoadConfig();
    }
  };
};

/*
*  负责从数据库中读取各类参数
*  Cache + Timer的方式去读数据库全局参数表global_parameters
*
*/
class GateParametersCache : public MyUtil::Singleton<GateParametersCache>{
public:
  GateParametersCache() : config_dbsource_("ad_st"){ 
    MyUtil::TaskManager::instance().schedule(new GateParametersCacheTimer());
  };
  
  void LoadParameters();
  
  string GetConfigDBSource() {
    return config_dbsource_;
  }

  string GetParameterByName(string para_name);

private:
  string config_dbsource_;
  map<string, string> global_para_; //为以后统一预留
  IceUtil::RWRecMutex mutex_;
  
  class GateParametersCacheTimer : public MyUtil::Timer {
  public:
    GateParametersCacheTimer(int time = 0) :Timer(time){}
    virtual void handle() {
      GateParametersCache::instance().LoadParameters();
      MyUtil::TaskManager::instance().schedule(new  GateParametersCacheTimer(2 * 60 * 1000)); //2分钟
    }
  };
};


}
}
#endif //_AD_GATE_CONFIG_H_
