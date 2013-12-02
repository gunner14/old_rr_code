#include "AdGateConfig.h"
#include "Singleton.h"
#include "TaskManager.h"
#include <fstream>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include "IceLogger.h"
#include "QueryRunner.h"
using namespace std;
using namespace com::xiaonei::xce;
using namespace MyUtil;
namespace xce {
namespace ad {
using namespace mysqlpp;

GateConfig::GateConfig() : ping_seq_size_(30),
  engine_host_("localhost"), engine_port_(9090),
  env_test_(false), flush_dbsource_("jebe_ads_load") {
  MyUtil::TaskManager::instance().schedule(new GateConfigLoadTimer());
};

string GateConfig::GetIp()
{
    int result = 0;
    char hostName[100];
    struct hostent* host = NULL;
    result = gethostname(hostName, 100);
    host = gethostbyname(hostName);
    return string(inet_ntoa(*((struct in_addr *)host->h_addr_list[0])));
}

bool GateConfig::LoadConfig() {
  string file_name = "../etc/ad_gate_n.config";
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
          if (key.compare("PING_SEQ_SIZE") == 0) {
            ping_seq_size_ = boost::lexical_cast<int>(value);
            MCE_INFO("[LOAD_CONFIG] ping_seq_size_ = " << ping_seq_size_);
          } else if (key.compare("ENGINE_HOST") == 0) {
            MCE_INFO("[LOAD_CONFIG] ENGINE_HOST = " << value);
            engine_host_ = value;
          } else if (key.compare("ENGINE_PORT") == 0) {
            MCE_INFO("[LOAD_CONFIG] ENGINE_PORT = " << value);
            engine_port_ = boost::lexical_cast<int>(value);
          } else if (key.compare("env_test") == 0) {
            env_test_ = boost::lexical_cast<bool>(value);
            MCE_INFO("[LOAD_CONFIG] _ = " << env_test_);
          } else if (key.compare("flush_db") == 0) {
            flush_db_ = value;
            MCE_INFO("[LOAD_CONFIG] flush_db_ = " << flush_db_);
          } else if (key.compare("flush_dbsource") == 0) {
            flush_dbsource_ = value;
            MCE_INFO("[LOAD_CONFIG] flush_dbsource = " << flush_dbsource_);
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
    Statement sql; 
    sql << " select * from global_parameters";
    MCE_INFO("try to excute select * from global_parameters");
    mysqlpp::StoreQueryResult res = QueryRunner(config_dbsource_, CDbRServer, "global_parameters").store(sql);
    if (!res || res.num_rows() <= 0) { 
      MCE_WARN("GateParametersCache::LoadParameters Error, select global_parameters fail!");
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


}
}
