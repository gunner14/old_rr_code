/*
 * ParametersController.cpp
 *
 *  Created on: 2012-2-24
 *      Author: sunzz
 */

#include "ParametersController.h"
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include "IceLogger.h"
#include <netdb.h>
#include <arpa/inet.h>
#include <connection.h>
#include <query.h>

namespace xce {
namespace ad {
using namespace mysqlpp;

bool EngineConfig::LoadConfig() {

  string file_name = "../etc/edm_sender.config";
  MCE_INFO("EngineConfig::LoadConfig-->begin,file:" << file_name);
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
        if (2 == n) {
         if (key.compare("db_source_main") == 0) {
           db_source_main_ = value;
           MCE_INFO("[LOAD_CONFIG] db_main = " << db_source_main_);
         } else if(key.compare("db_source_ads") == 0) {
           db_source_ads_ = value;
           MCE_INFO("[LOAD_CONFIG] db_ads = " << db_source_ads_);
         } else if(key.compare("para_db_host") == 0){
           para_db_host_ = value;
           MCE_INFO("[LOAD_CONFIG] para_db_host_ = " << para_db_host_);
         } else if(key.compare("edm_zone_id") == 0) {
           edm_zone_id_ = atol(value.c_str());
         } else if(key.compare("left_ad_min_price") == 0) {
           left_ad_min_price_ = atoi(value.c_str());
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

string EngineConfig::GetIp()
{
    int result = 0;
    char hostName[100];
    struct hostent* host = NULL;
    result = gethostname(hostName, 100);
    host = gethostbyname(hostName);
    return string(inet_ntoa(*((struct in_addr *)host->h_addr_list[0])));
}

void EngineParametersCache::LoadParameters() {
  MCE_INFO("EngineParametersCache::LoadParameters begin");
  map<string,string> system_para;
  try{
    const char* db_host = EngineConfig::instance().para_db_host().c_str();
    const char* db_name = "ad_strategy";
    Connection conn(db_name,db_host,"ad","rrad");
    MCE_INFO("EngineParametersCache::LoadParameters, db host:" << db_host<< ",db_name:" << db_name);

    stringstream sql;
    sql.str("");
    sql << " select config_key, config_value from edm_system_parameter";
    MCE_INFO("try to excute " << sql.str());
    string s= sql.str();
    Query q(&conn,true,s.c_str());
    mysqlpp::StoreQueryResult res = q.store();
    if (!res || res.num_rows() <= 0) {
      MCE_WARN("EngineParametersCache::LoadParameters Error " << sql.str() << ".  select  fail!");
    }
    MCE_INFO("EngineParametersCache::LoadParameters res size : " << res.num_rows());
    for (size_t i = 0; i < res.num_rows(); ++i) {
      mysqlpp::Row row = res.at(i);
      string key = (string) row["config_key"];
      string value = (string) row["config_value"];
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

string EngineParametersCache::GetParameterByName(string para_name) {
  IceUtil::RWRecMutex::RLock lock(mutex_);
  map<string, string>::iterator its = system_para_.find(para_name);
  if(its != system_para_.end()) {
    return its->second;
  }
  return "";
}

}
}



