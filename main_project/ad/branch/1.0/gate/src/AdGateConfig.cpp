#include "AdGateConfig.h"
#include "Singleton.h"
#include "TaskManager.h"
#include <fstream>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include "IceLogger.h"
#include "AdGateData.h"
#include <connection.h>
#include <query.h>

namespace xce {
namespace ad {
using namespace mysqlpp;

GateConfig::GateConfig():DB_("ade_db") {
  MyUtil::TaskManager::instance().schedule(new GateConfigLoadTimer());
};

bool GateConfig::LoadConfig() {
  string file_name = "../etc/ad_gate_s.config";
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
          if (key.compare("100000000001") == 0) {
            MCE_INFO("[LOAD_CONFIG] adzone_id = 100000000001, next_load_time = " << value);
            RefreshTime::instance().Load(value);
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
    MCE_WARN("GateConfig::LoadConfig error : " << e);
  } catch (...) {
    MCE_WARN("GateConfig::LoadConfig error : unkown error \"...\"");
  }
  return true;
};

void GateParametersCache::LoadParameters() {
  MCE_INFO("GateParametersCache::LoadParameters begin");
  map<string,string> global_para;
  try{ 
    const char* db_host = GateParametersCache::instance().ConfigDbHost().c_str();
    const char* db_name = GateParametersCache::instance().ConfigDbName().c_str();
    Connection conn(db_name, db_host,"ad","rrad");
    MCE_INFO("GateParametersCache::LoadParameters, db host:" << db_host<< ",db_name:" << db_name);
    stringstream sql; 
    sql.str("");
    sql << " select * from global_parameters";
    MCE_INFO("try to excute " << sql.str());
    string s= sql.str();
    Query q(&conn,true,s.c_str());
    mysqlpp::StoreQueryResult res = q.store();
    if (!res || res.num_rows() <= 0) { 
      MCE_WARN("GateParametersCache::LoadParameters Error " << sql.str() << ".  select  fail!");
    }    
    MCE_DEBUG("GateParametersCache::LoadParameters res size : " << res.num_rows());
    for (size_t i = 0; i < res.num_rows(); ++i) {
      mysqlpp::Row row = res.at(i);
      string key = (string) row["key"];
      string value = (string) row["value"];
      MCE_DEBUG("GateParametersCache::LoadParameters key=" << key << ", value=" << value);
      global_para[key] = value;
    }    
    IceUtil::RWRecMutex::WLock lock(mutex_);
    global_para_.swap(global_para);
    MCE_DEBUG("GateParametersCache::LoadParameters size is " << global_para.size());
  } catch (std::exception& e) { 
    MCE_WARN("GateParametersCache::LoadParameters error: " << e.what());
  } catch (...) {
    MCE_WARN("GateParametersCache::LoadParameters parameters error: unknown error!");
  }

  MCE_INFO("GateParametersCache::LoadParameters stop,global_para size:" << global_para.size());
}

string GateParametersCache::GetParameterByName(string para_name) {
  IceUtil::RWRecMutex::RLock lock(mutex_);
  map<string, string>::iterator its = global_para_.find(para_name);
  if(its != global_para_.end()) {
    return its->second;
  }
  return "";
}

void GateParametersCache::SetBwSwitch(){
  string bw_switch_str = GateParametersCache::instance().GetParameterByName(GateParametersCache::instance().BwSwitchKey());
  try {
    int tmp = boost::lexical_cast<int>(bw_switch_str); 
    IceUtil::RWRecMutex::WLock lock(mutex_);
    bw_switch_ = (tmp != 0);
  } catch (std::exception& e) { 
    MCE_WARN("GateParametersCache::SetBwSwitch error: " << e.what());
  } catch (...) {
    MCE_WARN("GateParametersCache::SetBwSwitch error: unknown error!");
  }
}

}
}
