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
#include <boost/algorithm/string/regex.hpp>
#include <algorithm>

namespace xce {
namespace ad {

bool EngineConfig::LoadConfig() {
  string file_name = "../etc/ad_engine_b.config";
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
          if (key.compare("sys_min_price") == 0) {
            sys_min_price_ = boost::lexical_cast<int>(value);
            MCE_INFO("[LOAD_CONFIG] sys_min_price = " << sys_min_price_);
          } else if (key.compare("SA") == 0) {
            SA_ = boost::lexical_cast<int>(value);
            MCE_INFO("[LOAD_CONFIG] SA = " << SA_);
          } else if (key.compare("SAA") == 0) {
            SAA_ = boost::lexical_cast<int>(value);
            MCE_INFO("[LOAD_CONFIG] SAA = " << SAA_);
          } else if (key.compare("DBSource_Ads") == 0) {
            db_source_ads_ = value;
            MCE_INFO("[LOAD_CONFIG] DBSource_Ads = " << db_source_ads_);
          } else if (key.compare("DBSource_Main") == 0) {
            db_source_main_ = value;
            MCE_INFO("[LOAD_CONFIG] DBSource_Main = " << db_source_main_);
          } else if (key.compare("NielsonDBSource") == 0) {
            NielsonDB_ = value;
            MCE_INFO("[LOAD_CONFIG] NielsonDB = " << NielsonDB_);
          } else if(key.compare("url_encode") == 0) {
            url_encode_ = boost::lexical_cast<bool>(value);
            MCE_INFO("[LOAD_CONFIG] url_encode = " << url_encode_);
          } else if(key.compare("cpm_esc_pro") == 0) {
            cpm_esc_pro_ = boost::lexical_cast<int>(value);
            MCE_INFO("[LOAD_CONFIG] cpm_esc_pro = " << cpm_esc_pro_);
          } else if(key.compare("cpm_esc_pro75") == 0) {
            cpm_esc_pro75_ = boost::lexical_cast<int>(value);
            MCE_INFO("[LOAD_CONFIG] cpm_esc_pro75 = " << cpm_esc_pro75_);
          } else if(key.compare("zone_ai_sort_set") == 0){
						MCE_INFO("[LOAD_CONFIG] zone_ai_sort_set =" << value);
						ParseAIZoneSet(value);
					}else if (key.compare("fresh_creative_ratio")==0){
            fresh_creative_ratio_ = boost::lexical_cast<int>(value);
            MCE_INFO("[LOAD_CONFIG] fresh_creative_ratio = " << fresh_creative_ratio_);
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

string EngineConfig::GetIp(){
	int result = 0;
	char hostName[100];
	struct hostent* host = NULL;
	result = gethostname(hostName, 100);
	host = gethostbyname(hostName);
	return string(inet_ntoa(*((struct in_addr *)host->h_addr_list[0])));
}

void  EngineConfig::ParseAIZoneSet(const string& zoneSetStr){
	vector<set<Ice::Long> > ai_zone_set;
	vector<string> para_strs;
	boost::algorithm::split_regex(para_strs, zoneSetStr, boost::regex("\\|"));
	
	int idx = 0;
	for(vector<string>::iterator it = para_strs.begin(); it != para_strs.end(); ++it,++idx){
		vector<string>  zones;
		boost::algorithm::split_regex(zones, *it, boost::regex(","));
		ai_zone_set.push_back(set<Ice::Long>());
		for(vector<string>::iterator it2 = zones.begin(); it2 != zones.end(); ++it2){
			char*  chTemp = NULL;
			ai_zone_set[idx].insert(strtoul(it2->c_str(), &chTemp, 10));
		}
	}
	IceUtil::RWRecMutex::WLock lock(mutex_);
	ai_zone_set_.swap(ai_zone_set);
}
}
}
