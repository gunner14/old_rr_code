/*
 * AdConfig.cpp
 *
 *  Created on: 2010-11-22
 *      Author: ark
 */
#include "AdConfig.h"
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include "IceLogger.h"
#include <netdb.h>
#include <arpa/inet.h>

namespace xce {
namespace ad {

bool AdDemoConfig::LoadConfig() {
  string file_name = "../etc/ad_creative_quality_d.config";
  try {
    ifstream config(file_name.c_str());
    string line;

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
        if (!key.empty()) {
          map_[key] = value;
          MCE_INFO("EngineConfig::LoadConfig Get config key: " << key
              << ", value: " << value);
        }
        if (2 == n) {
          if (key.compare("all_pv_th") == 0) {
            all_pv_th_ = boost::lexical_cast<int>(value);
            MCE_INFO("[LOAD_CONFIG] all_pv_th = " << all_pv_th_);
          } else if (key.compare("all_click_th") == 0) {
            all_click_th_ = boost::lexical_cast<int>(value);
            MCE_INFO("[LOAD_CONFIG] all_click_th = " << all_click_th_);
          } else if (key.compare("no_gender_pv_th") == 0) {
            no_gender_pv_th_ = boost::lexical_cast<int>(value);
            MCE_INFO("[LOAD_CONFIG] no_gender_pv_th = " << no_gender_pv_th_);
          } else if (key.compare("no_gender_click_th") == 0) {
            no_gender_click_th_ = boost::lexical_cast<int>(value);
            MCE_INFO("[LOAD_CONFIG] no_gender_click_th = " << no_gender_click_th_);
          } else if (key.compare("no_stage_pv_th") == 0) {
            no_stage_pv_th_ = boost::lexical_cast<int>(value);
            MCE_INFO("[LOAD_CONFIG] no_stage_pv_th = " << no_stage_pv_th_);
          } else if (key.compare("no_stage_click_th") == 0) {
            no_stage_click_th_ = boost::lexical_cast<int>(value);
            MCE_INFO("[LOAD_CONFIG] no_stage_click_th = " << no_stage_click_th_);
          } else if (key.compare("spid_pv_th") == 0) {
            spid_pv_th_ = boost::lexical_cast<int>(value);
            MCE_INFO("[LOAD_CONFIG] spid_pv_th = " << spid_pv_th_);
          } else if (key.compare("spid_click_th") == 0) {
            spid_click_th_ = boost::lexical_cast<int>(value);
            MCE_INFO("[LOAD_CONFIG] spid_click_th = " << spid_click_th_);
          } else if (key.compare("two_feature_click_th") == 0) {
            two_feature_click_th_ = boost::lexical_cast<int>(value);
            MCE_INFO("[LOAD_CONFIG] two_feature_click_th = " << two_feature_click_th_);
          } else if (key.compare("two_feature_pv_th") == 0) {
            two_feature_pv_th_ = boost::lexical_cast<int>(value);
            MCE_INFO("[LOAD_CONFIG] two_feature_pv_th = " << two_feature_pv_th_);
          }  else if (key.compare("db_name") == 0) {
            db_name_ = boost::lexical_cast<string>(value);
            MCE_INFO("[LOAD_CONFIG] db_name = " << db_name_);
          } else if (key.compare("db_host") == 0) {
            db_host_ = boost::lexical_cast<string>(value);
            MCE_INFO("[LOAD_CONFIG] db_host = " << db_host_);
          }else if (key.compare("db_ad_name") == 0){
						db_ad_name_ = boost::lexical_cast<string>(value);
						MCE_INFO("[LOAD_CONFIG] db_ad_name = " << db_ad_name_);
					}
				}
			}
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
}
}
