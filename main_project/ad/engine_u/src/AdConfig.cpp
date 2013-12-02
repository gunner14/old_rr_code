/*
 * AdConfig.cpp
 *
 *  Created on: 2010-11-22
 *      Author: ark
 */
#include <boost/tokenizer.hpp>
#include <fstream>
#include <netdb.h>
#include <arpa/inet.h>

#include "AdConfig.h"
#include "IceLogger.h"
#include "QueryRunner.h"
#include "DbDescriptor.h"

namespace xce {
namespace ad {
using namespace mysqlpp;
using namespace com::xiaonei::xce;

bool EngineConfig::LoadConfig() {
//  string file_name = "../etc/ad_engine.config";
  string server_index; 
  if(server_index_ >= 0) {
    server_index = boost::lexical_cast<string>(server_index_);
  }
  string file_name = "../etc/ad_engine" + server_index + ".config";
  try {
    ifstream config(file_name.c_str());
    string line;
    map<string, string> pool; 

    if (config) {
      while (config >> line) {
        typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
        boost::char_separator<char> sep("=");
        tokenizer tokens(line, sep);
        string key;
        string value;
        int n = 0;
        for(tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it, ++n) {
          if (0==n) {
            key = *it;
          } else if(1==n) {
            value = *it;
          }
        }
        if (!key.empty() && 2 == n) {
          pool[key] = value;
          MCE_INFO("EngineConfig::LoadConfig Get config key: " << key << ", value: " << value);
        }
      }
      if (!pool.empty()){
        IceUtil::RWRecMutex::WLock lock(mutex_);
        config_pool_.swap(pool);
      } else {
         MCE_WARN("EngineConfig::LoadConfig new pool is empty!");
      }
      string key = "sys_min_price";
      sys_min_price_ = GetConfigByName(key, sys_min_price_); 
      MCE_INFO("[LOAD_CONFIG] sys_min_price = " << sys_min_price_);
      
      key = "priority_pos";
      string value = GetConfigByName(key);
      if (!value.empty()){
        priority_pos_.clear();
        for(string::iterator sit = value.begin(); sit != value.end(); ++sit) {
          int pos = boost::lexical_cast<int>(*sit);
          priority_pos_.push_back(pos);
          MCE_INFO("[LOAD_CONFIG] pos = " << pos);
        }
      } 
      key = "X";
      X_ = GetConfigByName(key, X_);
      MCE_INFO("[LOAD_CONFIG] X = " << X_);
      key = "Y";
      Y_ = GetConfigByName(key, Y_);
      MCE_INFO("[LOAD_CONFIG] Y = " << Y_);
      key = "inner_priority_pos";
      value = GetConfigByName(key);
      if (!value.empty()) {
        inner_priority_pos_.clear();
        for(string::iterator sit = value.begin(); sit != value.end(); ++sit) {
          int pos = boost::lexical_cast<int>(*sit);
          inner_priority_pos_.push_back(pos);
          MCE_INFO("[LOAD_CONFIG] pos = " << pos);
        }
      }
      key = "cost_discount";
      cost_discount_ = GetConfigByName(key, cost_discount_);
      MCE_INFO("[LOAD_CONFIG] cost_discount = " << cost_discount_);
      
      key = "rand_seed";
      rand_seed_ = GetConfigByName(key, rand_seed_);
      MCE_INFO("[LOAD_CONFIG] rand_seed = " << rand_seed_);
          
      key = "sort_type";
      sort_type_ = GetConfigByName(key, sort_type_);
      MCE_INFO("[LOAD_CONFIG] sort_type = " << sort_type_);

      key = "price_power";
      price_power_ = GetConfigByName(key, price_power_);
      MCE_INFO("[LOAD_CONFIG] price_power = " << price_power_);
 
      key = "ctr_power";
      ctr_power_ = GetConfigByName(key, ctr_power_);
      MCE_INFO("[LOAD_CONFIG] ctr_power = " << ctr_power_);
      
      key = "user_id";
      user_id_ = GetConfigByName(key, user_id_);
      MCE_INFO("[LOAD_CONFIG] user_id = " << user_id_);
         
      key = "ad_blind_threshold";
      ad_blind_threshold_ = GetConfigByName(key, ad_blind_threshold_);
      MCE_INFO("[LOAD_CONFIG] ad_blind_threshold = " << ad_blind_threshold_);
      
      key = "url_encode";
      url_encode_ = GetConfigByName(key, url_encode_);
      MCE_INFO("[LOAD_CONFIG] url_encode = " << url_encode_);

      key = "alpha_threshold";
      alpha_threshold_ = GetConfigByName(key, alpha_threshold_);
      MCE_INFO("[LOAD_CONFIG] alpha_threshold = " << alpha_threshold_);

      key = "test_impr_ratio";
      test_impr_ratio_ = GetConfigByName(key, test_impr_ratio_);
      MCE_INFO("[LOAD_CONFIG] test_impr_ratio_ = " << test_impr_ratio_);
      
      key = "state1_test_impr_ratio";
      state1_test_impr_ratio_ = GetConfigByName(key, state1_test_impr_ratio_);
      MCE_INFO("[LOAD_CONFIG] state1_test_impr_ratio_ = " << state1_test_impr_ratio_);

      key = "state2_test_impr_ratio";
      state2_test_impr_ratio_ = GetConfigByName(key, state2_test_impr_ratio_);
      MCE_INFO("[LOAD_CONFIG] state2_test_impr_ratio_ = " << state2_test_impr_ratio_);
 
      key = "state3_test_impr_ratio";
      state3_test_impr_ratio_ = GetConfigByName(key, state3_test_impr_ratio_);
      MCE_INFO("[LOAD_CONFIG] state3_test_impr_ratio_ = " << state3_test_impr_ratio_);

      key = "state1_reserved_test_impr_ratio";
      state1_reserved_test_impr_ratio_ = GetConfigByName(key, state1_reserved_test_impr_ratio_);;
      MCE_INFO("[LOAD_CONFIG] state1_reserved_test_impr_ratio_ = " << state1_reserved_test_impr_ratio_);

      key = "state2_reserved_test_impr_ratio";
      state2_reserved_test_impr_ratio_ = GetConfigByName(key, state2_reserved_test_impr_ratio_);
      MCE_INFO("[LOAD_CONFIG] state2_reserved_test_impr_ratio_ = " << state2_reserved_test_impr_ratio_);

      key = "state3_reserved_test_impr_ratio";
      state3_reserved_test_impr_ratio_ = GetConfigByName(key, state3_reserved_test_impr_ratio_);
      MCE_INFO("[LOAD_CONFIG] state3_reserved_test_impr_ratio_ = " << state3_reserved_test_impr_ratio_);

      key = "PHOTO_WB1_COUNT";
      photo_wb1_count_ = GetConfigByName(key, photo_wb1_count_);
      MCE_INFO("[LOAD_CONFIG] photo_wb1_count_ = " << photo_wb1_count_);

      key = "big_pic_id";
      big_pic_id_ = GetConfigByName(key, big_pic_id_);
      MCE_INFO("[LOAD_CONFIG] big_pic_id_ = " << big_pic_id_);

      key = "PING_SEQ_SIZE";
      ping_seq_size_ = GetConfigByName(key, ping_seq_size_);
      MCE_INFO("[LOAD_CONFIG] ping_seq_size_ = " << ping_seq_size_);

      key = "big_pic_ratio_75";
      big_pic_ratio_75_ = GetConfigByName(key, big_pic_ratio_75_);
      MCE_INFO("[LOAD_CONFIG] big_pic_ratio_75 = " << big_pic_ratio_75_);

      key = "lr_model_id";
      lr_model_id_ = GetConfigByName(key, lr_model_id_);
      MCE_INFO("[LOAD_CONFIG] lr_model_id_ = " << lr_model_id_);

      key = "lr_demo_state";
      lr_demo_state_ = GetConfigByName(key, lr_demo_state_);
      MCE_INFO("[LOAD_CONFIG] lr_demo_state_ = " << lr_demo_state_);

      key = "lr_confidence_th";
      lr_confidence_th_ = GetConfigByName(key, lr_confidence_th_);
      MCE_INFO("[LOAD_CONFIG] lr_confidence_th_ = " << lr_confidence_th_);
     
      key = "lr_confidence_discount";
      lr_confidence_discount_ = GetConfigByName(key, lr_confidence_discount_);
      MCE_INFO("[LOAD_CONFIG] lr_confidence_discount_ = " << lr_confidence_discount_);

    } else {
      MCE_INFO("no file " << file_name);
    }
    if (config) {
      config.close();
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("EngineConfig::LoadConfig error : " << e);
  } catch (...) {
    MCE_WARN("EngineConfig::LoadConfig error : unkown error \"...\"");
  }
  return true;
}

string EngineConfig::GetIp()
{
    int result = 0;
    char hostName[100];
    struct hostent* host = NULL;
    result = gethostname(hostName, 100);
    host = gethostbyname(hostName);
    return string(inet_ntoa(*((struct in_addr *)host->h_addr_list[0])));
}

string EngineConfig::GetConfigByName(const string& config_name) {
  IceUtil::RWRecMutex::RLock lock(mutex_);
  map<string, string>::iterator its = config_pool_.find(config_name);
  if(its != config_pool_.end()) {
    return its->second;
  } else {
    return string();
  }
}

void EngineParametersCache::LoadParameters() {
	MCE_INFO("EngineParametersCache::LoadParameters begin");
	map<string,string> system_para;
	try{ 
		mysqlpp::StoreQueryResult res;
		Statement sql;
		sql << "select * from system_parameter";
		res = QueryRunner(EngineConfig::instance().ConfigDB(), CDbRServer, "system_parameter").store(sql);

		if (!res || res.num_rows() <= 0) { 
			MCE_WARN("EngineParametersCache::LoadParameters Error select  fail!");
		}    
		MCE_INFO("EngineParametersCache::LoadParameters res size : " << res.num_rows());
		for (size_t i = 0; i < res.num_rows(); ++i) {
			mysqlpp::Row row = res.at(i);
			string key = (string) row["key"];
			string value = (string) row["value"];
			MCE_INFO("EngineParametersCache::LoadParameters key=" << key << ", value=" << value);
			system_para[key] = value;
		}    
		MCE_INFO("EngineParametersCache::LoadParameters size is " << system_para.size());
	} catch (std::exception& e) { 
		MCE_WARN("EngineParametersCache::LoadParameters error: " << e.what());
	} catch (...) {
		MCE_WARN("EngineParametersCache::LoadParameters parameters error!");
	}

	IceUtil::RWRecMutex::WLock lock(mutex_);
	system_para_.swap(system_para);
	MCE_INFO("EngineParametersCache::LoadParameters stop,system_para size:" << system_para.size());
}

string EngineParametersCache::GetParameterByName(const string& para_name) {
  IceUtil::RWRecMutex::RLock lock(mutex_);
  map<string, string>::iterator its = system_para_.find(para_name);
  if(its != system_para_.end()) {
    return its->second;
  }
  return "";
}

bool StrategyConfig::LoadConfig() {
	try {
		string line;
		for (int i = 0; i  < ADZONE_NUMBER; i++) {
			line = ADZONE_CONFIG[i];
			typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
			boost::char_separator<char> sep(";");
			tokenizer tokens(line, sep);
			Ice::Long adzone;
			string selects,defaults;
			int n = 0;
			for(tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it, ++n) {
				if (0==n) {
					adzone = boost::lexical_cast<Ice::Long>(*it);
				} else if(1==n) {
					selects = *it;
				} else if(2==n) {
					defaults = *it;
				}   
			}   
			vector<int> vecSelectNum;
			vector<int> vecDefaultNum;
      if(!ParseConfig(selects,vecSelectNum)){
			  return false;
			}
			if(!ParseConfig(defaults,vecDefaultNum)){
			  return false;
			}
		  adzone2select_[adzone] = vecSelectNum;
			adzone2default_[adzone] = vecDefaultNum;
		}   

		if(adzone2select_.size() != ADZONE_NUMBER || adzone2default_.size() != ADZONE_NUMBER){
		  MCE_WARN("StrategyConfig::LoadConfig Error");
			return false;
		}
	} catch (Ice::Exception& e) {
		MCE_WARN("StrategyConfig::LoadConfig error : " << e);
		return false;
	} catch (...) {
		MCE_WARN("StrategyConfig::LoadConfig error : unkown error \"...\"");
		return false;
	}
	return true;
}

bool StrategyConfig::ParseConfig(const string& strSource, vector<int>& vecResult) {
	try {
			typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
			boost::char_separator<char> sep("-");
			tokenizer tokens_default(strSource, sep);
			for(tokenizer::iterator its = tokens_default.begin(); its != tokens_default.end(); ++its) {
				vecResult.push_back(boost::lexical_cast<int>(*its));
			}
	} catch (Ice::Exception& e) {
		MCE_WARN("EngineConfig::Parse Config error : " << e);
		return false;
	} catch (...) {
		MCE_WARN("EngineConfig::Parse Config error : unkown error \"...\"");
		return false;
	}
  return true;
}

vector<int> StrategyConfig::GetDefaultVecByAdzone(const Ice::Long& adzone){
	map<Ice::Long,vector<int> >::iterator it = adzone2default_.find(adzone);
	if(it != adzone2default_.end()){
		return  it->second;
	}else{
		return adzone2default_[100000000000];
	} 
}

bool StrategyConfig::IsIncludeReFresh(const Ice::Long & adzone){
	map<Ice::Long ,vector<int> >::iterator it = adzone2select_.find(adzone);
	if(it != adzone2select_.end()){
		vector<int>::iterator itvec = std::find(it->second.begin(),it->second.end(),REFRESH);
		if(itvec != it->second.end()){
			return true;
		}
	}
	return false;
}

}
}
