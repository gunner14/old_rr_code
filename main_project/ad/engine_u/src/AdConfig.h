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
#include "IceLogger.h"
#include <boost/lexical_cast.hpp>
namespace xce {
namespace ad {

const static int CPMMEMBER = 1;  //糯米保量handle
const static int NEWAD= 2;       //新广告处理handle
const static int ALLPK = 3;      //全竞价处理handle
const static int BIDDINGN = 4;   //N次Top1处理handle
const static int RANDOM = 5;     //随机处理handle
const static int REFRESH = 6;    //刷新处理handle,即下一个更精彩
//默认广告选择逻辑对应的配置
//BOTH :表示既可以选择默认大图，也可以选择默认小图
//BIG: 表示只能选择默认大图
//SMALL： 表示只能选择默认小图
enum HandleNum{BOTH=1,BIG,SMALL};

#define ADZONE_NUMBER 11
const string ADZONE_CONFIG[] = {"100000000060;1-2-4;2",
                                "100000000001;2-4;3",
                                "100000000063;2-4;3",
                                "100000000075;2-3;3",
                                "100000000065;2-3-6;3",
                                "100000000070;2-3;3",
                                "100000000071;2-3;3",
                                "100000000072;2-3;3",
                                "100000000073;2-3;3",
                                "100000000093;2-3-6;3",
                                "100000000000;2-4;3"};

class EngineConfig : public MyUtil::Singleton<EngineConfig> {
public:
	EngineConfig(): url_encode_(0), //Bool
		sys_min_price_(100), rand_seed_(1500),  price_power_(1),ctr_power_(1),  //int
		sort_type_(1), user_id_(-1), ad_blind_threshold_(50), ping_seq_size_(300),
		lr_model_id_(0), lr_demo_state_(1024), big_pic_ratio_75_(50), photo_wb1_count_(2),
		big_pic_id_(31),
		X_(2), Y_(1), cost_discount_(0.85), //float
		lr_confidence_th_(0.0), lr_confidence_discount_(1.0), alpha_threshold_(2.5), //double
		test_impr_ratio_(0.05), state1_test_impr_ratio_(0.0), state2_test_impr_ratio_(0.0),
		state3_test_impr_ratio_(0.0), state1_reserved_test_impr_ratio_(0.2), 
		state2_reserved_test_impr_ratio_(0.2), state3_reserved_test_impr_ratio_(0.2),
		DB_Ads_("jebe_ads_load"), DB_Main_("jebe_main_load"), Config_DB_("ad_st"), 
    server_name_("AdEngineU0@127.0.0.1"), server_object_("self"), 
    memcached_("10.7.18.60:11211 500,10.7.18.60:11212 500") {

		priority_pos_.push_back(1);
		priority_pos_.push_back(2);
		priority_pos_.push_back(3);
		priority_pos_.push_back(4);

		inner_priority_pos_.push_back(3);
		inner_priority_pos_.push_back(1);
		inner_priority_pos_.push_back(2);
		inner_priority_pos_.push_back(4);
	}
  bool url_encode() {
    return url_encode_;
  }
  float X() {
    return X_;
  }
  float Y() {
    return Y_;
  }
  string DB_Ads() {
    return DB_Ads_;
  }
  void set_DB_Ads(string DB) {
    DB_Ads_ = DB;
  }
	string DB_Main() {
    return DB_Main_;
  }
  void set_DB_Main(string DB) {
    DB_Main_ = DB;
  }
  string ConfigDB() {
		return Config_DB_;
	}
	void set_ConfigDB(string Config_DB) {
		Config_DB_= Config_DB;
	}
  int sys_min_price() {
    return sys_min_price_;
  }
  vector<int> priority_pos() {
    return priority_pos_;
  }
  vector<int> inner_priority_pos() {
    return inner_priority_pos_;
  }
  void set_X(float X) {
    X_ = X;
  }
  void set_Y(float Y) {
    Y_ = Y;
  }
  int server_index() {
    return server_index_;
  }
  string server_name() {
    return server_name_;
  }
  void set_server_name(int server_index) {
    server_index_ = server_index;
    ostringstream oss;
    oss << "AdEngineU" << server_index << "@" << GetIp();
    server_name_ = oss.str();
  }
  void set_server_name(string server_name) {
    server_name_ = server_name;
  }
  bool LoadConfig();
  void set_server_object(string server_object) {
    server_object_ = server_object;
  }
  bool IsForBrand() {
    return server_object_ == "brand";
  }
  float GetCostDiscount() {
    return cost_discount_;
  }
  double GetAlphaThreshold(){
    return alpha_threshold_;
  }
  int GetRandSeed() {
    return rand_seed_;
  }
  int sort_type() {
    return sort_type_;
  }
  int price_power() {
    return price_power_;
  }
  int ctr_power() {
    return ctr_power_;
  }
  int GetUserId() {
    return user_id_;
  }
  int AdBlindThreshold() {
    return ad_blind_threshold_;
  }
  int ping_seq_size() {
    return ping_seq_size_;
  }
  double GetTestImprRatio() {
    return test_impr_ratio_;
  }
  double GetStateTestImprRatio(Ice::Long zone, int pos) {
    Ice::Long key = zone * 10 + pos;
    if(key == 1000000000723 || key == 1000000000702 || key == 1000000000703) {
      return state1_test_impr_ratio_;
    } else if(key == 1000000000701 || key == 1000000000722 || key == 1000000000633) {
      return state2_test_impr_ratio_;
    } else if(key == 1000000000721 || key == 1000000000632 || key == 1000000000631) {
      return state3_test_impr_ratio_;
    }
    return 0.05;
  }
  double GetReservedStateTestImprRatio(Ice::Long zone, int pos) {
    Ice::Long key = zone * 10 + pos;
    if(key == 1000000000723 || key == 1000000000702 || key == 1000000000703) {
      return state1_reserved_test_impr_ratio_;
    } else if(key == 1000000000701 || key == 1000000000722 || key == 1000000000633) {
      return state2_reserved_test_impr_ratio_;
    } else if(key == 1000000000721 || key == 1000000000632 || key == 1000000000631) {
      return state3_reserved_test_impr_ratio_;
    }
    return 0.05;
  }
  int photo_wb1_count() {
    return photo_wb1_count_;
  }
  int big_pic_id() {
    return big_pic_id_;
  }
  void set_memcached(const string& servers) {
    memcached_ = servers;
  }
  string memcached() {
    return memcached_;
  }
  int big_pic_ratio_75() {
    return big_pic_ratio_75_;
  }
  int lr_model_id(){
    return lr_model_id_;
  }
  int lr_demo_state(){
    return lr_demo_state_;
  }
  double lr_confidence_th(){
    return lr_confidence_th_;
  }
  double lr_confidence_discount(){
    return lr_confidence_discount_;
  }

  string GetConfigByName(const string& config_name);

  template<typename EngineConfigT>
  EngineConfigT  GetConfigByName(const string& config_name, EngineConfigT default_value){
    EngineConfigT res = default_value;
    string value = GetConfigByName(config_name);
    if (!value.empty()){
      try {
        res = boost::lexical_cast<EngineConfigT>(value);
      } catch (...){
        res = default_value;
        MCE_INFO("EngineConfig::GetConfigByName error: key=" << config_name<<",value="<<value); 
      }
    }
    return res;
  }

private:
	bool url_encode_;

	int sys_min_price_;
	int rand_seed_;
	int price_power_;
	int ctr_power_;
	int sort_type_;
	int server_index_;
	int user_id_;
	int ad_blind_threshold_;
	int ping_seq_size_;
	int lr_model_id_;
	int lr_demo_state_;
	int big_pic_ratio_75_;
	int photo_wb1_count_;
	int big_pic_id_;

	float X_;
	float Y_;
	float cost_discount_;

	double lr_confidence_th_;
	double lr_confidence_discount_;
	double alpha_threshold_;
	double test_impr_ratio_;
	double state1_test_impr_ratio_;
	double state2_test_impr_ratio_;
	double state3_test_impr_ratio_;
	double state1_reserved_test_impr_ratio_;
	double state2_reserved_test_impr_ratio_;
	double state3_reserved_test_impr_ratio_;

	string DB_Ads_;
	string DB_Main_;
	string Config_DB_;
	string server_name_;
	string server_object_; // 服务对象, 分为品牌/ 自助
	string memcached_;

	map<string, string> config_pool_;
	vector<int> priority_pos_;
	vector<int> inner_priority_pos_;

	IceUtil::RWRecMutex mutex_;

	string GetIp();
public:
  class ConfigLoadTimer : public MyUtil::Timer{
  public:
    ConfigLoadTimer(int time = 0):Timer(time){}
    virtual void handle() {
      MyUtil::TaskManager::instance().schedule(new ConfigLoadTimer(60 * 1000));//1分钟更改一次
      EngineConfig::instance().LoadConfig();
    }
  };
};

class EngineParametersCache : public MyUtil::Singleton<EngineParametersCache>{
  private:
    map<string, string> system_para_;
    IceUtil::RWRecMutex mutex_;
  public:
    void LoadParameters();
    string GetParameterByName(const string& para_name);
    template<typename EngineParameterT>
    EngineParameterT  GetParameterByName(const string& para_name, EngineParameterT default_value){
      EngineParameterT res = default_value;
      string value = GetParameterByName(para_name);
      if (!value.empty()){
        try {
          res = boost::lexical_cast<EngineParameterT>(value);
        } catch (...){
          res = default_value;
          MCE_INFO("EngineParametersCache::GetParameterByName error: key=" << para_name<<",value="<<value << " res = " << res); 
        }
      }
      return res;
    }
};

class EngineParametersTimer : public MyUtil::Timer {
  public:
    EngineParametersTimer(int time =0 ) :Timer(time){
    }
    virtual void handle() {
      MCE_INFO(" EngineParametersTimer handle :begin");
      MyUtil::TaskManager::instance().schedule(new  EngineParametersTimer(3 * 60 * 1000)); //每3分钟读一次
      EngineParametersCache::instance().LoadParameters();
    }

};

class StrategyConfig : public MyUtil::Singleton<StrategyConfig> {
	public:
		bool LoadConfig();
		bool ParseConfig(const string& strSource, vector<int>& vecResult);
		map<Ice::Long,vector<int> > GetSelectList(){return adzone2select_;}
		vector<int> GetDefaultVecByAdzone(const Ice::Long& adzone);
		bool IsIncludeReFresh(const Ice::Long & adzone);
	private:
		map<Ice::Long ,vector<int> > adzone2default_;
		map<Ice::Long ,vector<int> > adzone2select_;
};

} // end namespace ad
} // end namespace xce
#endif /* AD_CONFIG_H_ */
