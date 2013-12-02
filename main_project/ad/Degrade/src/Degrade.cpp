/*
 * =====================================================================================
 *
 *       Filename:  degrade.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月24日 09时06分34秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <query.h>
#include <connection.h>
#include "Degrade.h"
#include "LogWrapper.h"

void DegradeManager::SetDBSource(const std::string &source){
  db_source_ = source;
}


void DegradeManager::setInterval(unsigned interval){
  interval_ = interval;
}


int DegradeManager::Init(){

  Load();
  start().detach();
  return 0;
}

void DegradeManager::Register(const std::string &module_name){

  DegradeModule module;
  module.module_name_ = module_name;
  module.is_enable_ = true;

  if(modules_.find(module_name) == modules_.end()){
    modules_[module_name] = module; 
  }
}

void DegradeManager::SetModuleStatus(const std::string &module_name, int status){
  std::map<std::string, DegradeModule>::iterator it = modules_.find(module_name);  
  if(it == modules_.end()){
    MCE_WARN("DegradeManager::SetModuleStatus cannot find module: " << module_name);
    return;
  }else{
    it->second.is_enable_ = (status == 1 ? true : false);
  }
}

bool DegradeManager::IsEnable(const std::string &module_name){

  std::map<std::string, DegradeModule>::iterator it = modules_.find(module_name);
  if(it == modules_.end()){
    MCE_WARN("DegradeManager::IsEnable cannot find module: " << module_name);
    return true; 
  }else{
    return it->second.is_enable_;
  }
}

bool DegradeManager::IsGray(int tail) {
  if(tail < gray_) {
    return true;
  } else {
    return false;
  }
}

void DegradeManager::Enable(const std::string &module_name){
  return SetModuleStatus(module_name, 1); 
}

void DegradeManager::Disable(const std::string &module_name){
  return SetModuleStatus(module_name, 0);
}

void DegradeManager::LoadModuleStatus(){

  if(modules_.empty()){
    MCE_WARN("DegradeManager::Load Failed. no module registered");
    return;
  }

  ostringstream s;
  std::map<std::string, DegradeModule>::iterator it = modules_.begin();
  s << "'" << it->second.module_name_ << "'";
  while(++it != modules_.end()){
    s << ", " << "'" << it->second.module_name_ << "'";
  }

  com::xiaonei::xce::Statement sql;
  mysqlpp::StoreQueryResult res;
  sql << "select * from module_status where module in (" << s.str() << ")";

  int ret = excuteQuery(sql, "adcampaign", res);
  if(ret < 0){
    MCE_WARN("DegradeManager::LoadByDBSource excuteQuery failed");
    return;
  }

  if (res.num_rows() > 0){
    for (size_t i = 0; i < res.num_rows(); ++i){
      mysqlpp::Row row = res.at(i);
      std::string module = row["module"].c_str();
      int status = ((int) row["status"]);
      SetModuleStatus(module, status);
      MCE_DEBUG("DegradeManager::LoadModuleStatus module:"<<module<<" ---> status : "<<status);

    }
  }

  return;

}
  
void DegradeManager::LoadGray(){

  com::xiaonei::xce::Statement sql;
  mysqlpp::StoreQueryResult res;
  sql << "select * from gray_args where name='" << service_name_ << "'";

  int ret = excuteQuery(sql, "gray_args", res);
  if(ret < 0){
    MCE_WARN("DegradeManager::LoadGray excuteQuery failed");
    return;
  }

  if (res.num_rows() >= 1){
    mysqlpp::Row row = res.at(0);
    std::string name = row["name"].c_str();
    int arg = ((int) row["arg"]);
    gray_ = arg;
    MCE_DEBUG("DegradeManager::LoadGray " << service_name_ << " ---> gray:"<< arg);
  }
}

void DegradeManager::Load(){
  LoadModuleStatus();
  LoadGray();
}

void DegradeManager::run(){
  while(true){
    sleep(interval_);
    Load();
  }
}

int DegradeManager::excuteQuery(const com::xiaonei::xce::Statement &sql, const std::string &table, mysqlpp::StoreQueryResult & res){
  try{
    res = com::xiaonei::xce::QueryRunner(db_source_.c_str(), com::xiaonei::xce::CDbRServer, table.c_str()).store(sql);
  }
  catch(...){
    MCE_WARN("QueryRunner catch exception!");
    return -1;
  }

  return 0;

}
