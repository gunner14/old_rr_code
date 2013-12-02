#include "Singleton.h"
#include "TaskManager.h"
#include  <map>

namespace xce {
namespace ad {

class GateConfig : public MyUtil::Singleton<GateConfig> {
public:
  GateConfig();
  string DB() {
    return DB_;
  }
  void set_DB(string DB) {
    DB_ = DB;
  }
  bool LoadConfig();
private:
  string DB_;

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
  GateParametersCache() : bw_switch_(false), bw_switch_key_("gates_bw_switch"), 
                          config_dbhost_("10.3.23.69"), config_dbname_("ad_strategy"){ // TODO 默认应该是我们？还是BW？
    MyUtil::TaskManager::instance().schedule(new GateParametersCacheTimer());
  }; 
  void LoadParameters();
  void SetBwSwitch();
  bool BwSwitch(){
    return bw_switch_;  
  } 
  string BwSwitchKey(){
    return bw_switch_key_;  
  }
  string ConfigDbHost(){
    return config_dbhost_;
  }
  string ConfigDbName(){
    return config_dbname_;
  }
  string GetParameterByName(string para_name);

private:
  bool bw_switch_;
  string bw_switch_key_;
  string config_dbhost_;
  string config_dbname_;
  map<string, string> global_para_; //为以后统一预留
  IceUtil::RWRecMutex mutex_;
  
  class GateParametersCacheTimer : public MyUtil::Timer {
  public:
    GateParametersCacheTimer(int time = 0) :Timer(time){}
    virtual void handle() {
      GateParametersCache::instance().LoadParameters();
      GateParametersCache::instance().SetBwSwitch();
      MyUtil::TaskManager::instance().schedule(new  GateParametersCacheTimer(2 * 60 * 1000)); //2分钟
    }
  };
};

}
}
