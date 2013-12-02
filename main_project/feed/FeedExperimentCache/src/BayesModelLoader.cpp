#include <cstdio>
#include <cstring>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>
#include "Util.h"
#include "BayesModelLoader.h"
#include "FeedExperimentCache.h"

using namespace std;
using namespace MyUtil;
using namespace xce::feed;
using namespace mysqlpp;
using namespace boost;

const int gFileNotExist = -1;
const int gMd5Length = 33;
const int gEqual = 0;
const int gExecOk = 0;
const std::string gMd5Cmd = "md5sum ";
const std::string gMd5TmpFile = "/tmp/md5";
const std::string gReadMode = "r";

const char* gDbHost = "10.4.16.98";
const char* gDbName = "feed_sys";
const char* gDbUser = "feed";
const char* gDbPassword = "rrfeed";

typedef boost::tokenizer<boost::char_separator<char> > model_tokenizer;

bool BayesModelLoader::HasNewFile(std::string& new_file_name, std::string& new_md5_value) {
  stringstream sql;
  sql << " SELECT file_name, md5_value, update_interval, targets FROM feature_data_config"
      << " WHERE feature_name = 'bayes' AND valid = 1 AND version = " << version_;
  StoreQueryResult res;
  try {
    Connection conn(gDbName, gDbHost, gDbUser, gDbPassword);
    Query query(&conn, true, sql.str().c_str());
    res = query.store();
  } catch (Ice::Exception& e) { 
    MCE_WARN("BayesModelLoaderI version " << version_ << " ::HasNewFile --> ice error! " << e.what());
    return false;
  } catch (std::exception& e) {
    MCE_WARN("BayesModelLoaderI version " << version_ << " ::HasNewFile --> std error! " << e.what());
    return false;
  }

  if (!res) {
    MCE_WARN("BayesModelLoaderI version " << version_ << " ::HasNewFile --> Query Result Error!");
    return false;
  } else if (res.empty()) {
    MCE_INFO("BayesModelLoaderI version " << version_ << " ::HasNewFile --> nothing in db!");
    return false;
  } else if (res.size() > 1) {
    MCE_WARN("BayesModelLoaderI version " << version_ << " ::HasNewFile --> dirty data in db!");
    return false;
  }
  std::string temp_file_name, temp_md5_value, targets_string;
  int interval;
  MyUtil::IntSeq targets;
  try {
    temp_file_name = boost::lexical_cast<string>(res.at(0)["file_name"]);
    temp_md5_value = boost::lexical_cast<string>(res.at(0)["md5_value"]);
    interval = boost::lexical_cast<int>(res.at(0)["update_interval"]);
    targets_string = boost::lexical_cast<string>(res.at(0)["targets"]);
    boost::char_separator<char> sep(",");
    model_tokenizer tokens(targets_string, sep);
    for (model_tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it) {
      int target = boost::lexical_cast<int>(*it);
      targets.push_back(target);
    }
  } catch (...) {
    MCE_WARN("BayesModelLoader version " << version_ << " ::LoadModel --> parse line data error.");
    return false;
  }

  //Loading Data of the new_file_name now, not finished.
  if (loading_){
    MCE_INFO("BayesModelLoaderI version " << version_ << " ::HasNewFiles --> The File is loading.");
    return false;
  }

  if (temp_md5_value != md5_value_) {  
    new_file_name = temp_file_name;
    new_md5_value = temp_md5_value;
    interval_ = interval;
    target_news_ = targets;
    return true;
  }
  return false;
}

bool BayesModelLoader::IsValidFile(const std::string& file_name, const std::string& md5_value) {
  if (file_name.empty()) {
    MCE_WARN("BayesModelLoader version " << version_ << " ::IsValidFile --> FileName Is Empty");
    return false;
  }

  if (gFileNotExist == access(file_name.c_str(), R_OK)) {
    MCE_WARN("BayesModelLoader version " << version_ << " ::IsValidFile --> Not Exist File: " << file_name);
    return false;
  }

  char md5_char[gMd5Length];
  ComputeMd5(file_name, md5_char);

  if ((gMd5Length - 1 != md5_value.size()) || (gEqual != md5_value.compare(md5_char))) {
    std::string value(md5_char);
    MCE_WARN("BayesModelLoader version " << version_ << " ::IsValidFile --> Md5 From SQL: " << md5_value_ << " Size: " << md5_value.size() << " Md5 From File: " << value);
    return false;
  }
  return true;
}

void BayesModelLoader::ComputeMd5(const string& file_name, char* md5_char) {
  stringstream os;
  os << gMd5TmpFile << version_;
  string md5_tmp_file = os.str();
  string command = gMd5Cmd + file_name + " > " + md5_tmp_file;
    
  if (gExecOk != std::system(command.c_str())) {
    MCE_WARN("BayesModelLoader version " << version_ << " ::ComputeMD5 --> Execute CMD " << command << " Error!" );
    return;
  }

  FILE *fp = fopen(md5_tmp_file.c_str(), gReadMode.c_str());
  if (NULL == fp) {
    MCE_WARN("BayesModelLoader version " << version_ << " ::GomputeMD5 --> open file: " << file_name << " failed!");
    md5_char[0] = '\0';
  } else if (NULL == fgets(md5_char, gMd5Length, fp)) {
    MCE_WARN("BayesModelLoader version " << version_ << " ::ComputeMD5 --> Read MD5 Stdout Failed!");
    md5_char[0] = '\0';
    fclose(fp);
  }
  return;
}

bool BayesModelLoader::LoadModel() {
  MCE_INFO("BayesModelLoader version " << version_ << " ::LoadModel --> Begin Load.");
  fstream in;
  bool res = false;
  BayesModelList model_list;
  string new_file_name, new_md5_value;
  if (HasNewFile(new_file_name, new_md5_value)) {
    if (IsValidFile(new_file_name, new_md5_value)) {
      try {
        loading_ = true;
        boost::char_separator<char> sep(" ");
        in.open(new_file_name.c_str(), fstream::in);
        string line;
        while (getline(in, line).good()) {
          model_tokenizer tokens(line, sep);
          BayesModel model;
          int cnt = 0;
          try {
            for (model_tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it,++cnt) {
              switch (cnt) {
                case 0 : model.key = *it; break;
                case 1 : model.value = boost::lexical_cast<double>(*it); break;
                default: ;
              }
            }
            if (cnt == 2) {
              model_list.push_back(model);
            } else {
              MCE_INFO("BayesModelLoader version " << version_ << " ::LoadModel --> bad data:" << line);
            }
          } catch (...) {
            MCE_WARN("BayesModelLoader version " << version_ << " ::LoadModel --> parse line data error.");
          }
        }
        in.close();
      } catch (...) {
        in.close();
        MCE_WARN("BayesModelLoader version " << version_ << " ::LoadModel --> Load model failed");
      }
    }
  }
  if (model_list.size() > 0 ) {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    model_list_.swap(model_list);
    MCE_INFO("BayesModelLoader version " << version_ << " ::LoadModel --> new model size=" << model_list_.size());
    file_name_ = new_file_name;
    md5_value_ = new_md5_value;
    res = true;
  } else {
    MCE_WARN("BayesModelLoader version " << version_ << " ::LoadModel --> no new model loaded");
  }
  loading_ = false;
  return res;
}

void BayesModelLoader::GetBayesModels(int begin, int size, BayesModelList& res) {
  if (begin < 0 || size < 0) {
    MCE_WARN("BayesModelLoader version " << version_ << " ::GetBayesModels --> wrong arguments. begin:" << begin << ", size:" << size);
    return;
  }
  if (!loading_) {
    IceUtil::RWRecMutex::RLock lock(mutex_);
    for (int i = begin; i < (int)model_list_.size() && i < begin + size; i++) {
      res.push_back(model_list_[i]);
    }
    MCE_INFO("BayesModelLoader version " << version_ << " ::GetBayesModels --> begin=" << begin << ", size=" << size << ". result size=" << res.size());
  }
  return;
}

void BayesModelLoader::BayesModelLoaderTask::handle() {
  while(true) {
    int version = model_loader_->version();
    stringstream log;
    log << "BayesModelLoaderTask::handle. model version=" << version;
    if (model_loader_->LoadModel()) {
      int model_size = model_loader_->GetModelSize();
      MyUtil::IntSeq targets = model_loader_->target_news();
      FeedExperimentCache::instance().UpdateMod2Version(version, targets);
      log << " model_size=" << model_size << " target news size=" << targets.size();
      MCE_INFO(log.str());
    } else {
      log << " load model failed.";
      MCE_WARN(log.str());
    }
    sleep(model_loader_->interval());
  }
}
