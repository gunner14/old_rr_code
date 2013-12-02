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
#include "ServiceI.h"


MyUtil::Str2StrMap xce::feed::FeedDegradeBackdoorI::control(
    const MyUtil::Str2StrMap& data, const Ice::Current& current) {
  MyUtil::Str2StrMap::const_iterator i = data.begin();
  for (; i != data.end(); ++i) {
    if (i->second == "1") {
      xce::feed::DegradeManager::instance().Enable(i->first);
    } else if (i->second == "0") {
      xce::feed::DegradeManager::instance().Disable(i->first);
    }
  }
  return xce::feed::DegradeManager::instance().getStatus();
}
void xce::feed::DegradeManager::SetDBSource(const std::string &source){
  db_source_ = source;
}


void xce::feed::DegradeManager::setInterval(unsigned interval){
  interval_ = interval;
}


int xce::feed::DegradeManager::Init(){

  Load();
  start().detach();
  if (!door_) {
    door_ = new FeedDegradeBackdoorI(MyUtil::ServiceI::instance().getAdapter());
  }
  return 0;
}

void xce::feed::DegradeManager::Register(const std::string &module_name){

  DegradeModule module;
  module.module_name_ = module_name;
  module.is_enable_ = true;

  if(modules_.find(module_name) == modules_.end()){
    modules_[module_name] = module; 
  }

  com::xiaonei::xce::Statement sql;
  sql << "INSERT IGNORE INTO module_status(module, status, module_desc) "
      << "VALUES('" << module_name << "',1,'degrade')";

  try {
    com::xiaonei::xce::QueryRunner(db_source_, com::xiaonei::xce::CDbWServer, "module_status").store(sql);
    MCE_INFO("DegradeManager::Register excuteQuery sucessed. module_name: " << module_name);
  } catch (mysqlpp::Exception& e) {
    MCE_WARN("DegradeManager::Register excuteQuery failed. module_name: " << module_name << ". " << e.what());
  } catch (...) {
    MCE_WARN("DegradeManager::Register excuteQuery failed. module_name: " << module_name << ". UNKOWN EXCEPTION!");
  }
}

void xce::feed::DegradeManager::SetModuleStatus(const std::string &module_name, int status){
  std::map<std::string, DegradeModule>::iterator it = modules_.find(module_name);  
  if(it == modules_.end()){
    MCE_WARN("DegradeManager::SetModuleStatus cannot find module: " << module_name);
    return;
  }else{
    it->second.is_enable_ = (status == 1 ? true : false);
    MCE_INFO("DegradeManager::SetModuleStatus --> module: " << module_name << " status: " << (status == 1));
  }
}

bool xce::feed::DegradeManager::IsEnable(const std::string &module_name){

  std::map<std::string, DegradeModule>::iterator it = modules_.find(module_name);
  if(it == modules_.end()){
    MCE_WARN("DegradeManager::IsEnable cannot find module: " << module_name);
    return true; 
  }else{
    return it->second.is_enable_;
  }
}


void xce::feed::DegradeManager::Enable(const std::string &module_name){
  return SetModuleStatus(module_name, 1); 
}

void xce::feed::DegradeManager::Disable(const std::string &module_name){
  return SetModuleStatus(module_name, 0);
}

void xce::feed::DegradeManager::LoadModuleStatus(){

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

  int ret = excuteQuery(sql, "module_status", res);
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
      MCE_INFO("module="<<module<<",status="<<status);

    }
  }

  return;

}
  
void xce::feed::DegradeManager::Load(){
  LoadModuleStatus();
}

void xce::feed::DegradeManager::run(){
  while(true){
    sleep(interval_);
    Load();
  }
}

std::map<std::string, std::string> xce::feed::DegradeManager::getStatus() {  
  std::map<std::string, std::string> m;
  std::map<std::string, DegradeModule>::iterator it = modules_.begin();
  for ( ; it != modules_.end(); ++it) {
    m.insert(make_pair(it->first, it->second.is_enable_ ? "1":"0"));
  }
  return m;
}

int xce::feed::DegradeManager::excuteQuery(const com::xiaonei::xce::Statement &sql, const std::string &table, mysqlpp::StoreQueryResult & res){
  try{
    res = com::xiaonei::xce::QueryRunner(db_source_.c_str(), com::xiaonei::xce::CDbRServer, table.c_str()).store(sql);
  }
  catch(...){
    MCE_WARN("QueryRunner catch exception!");
    return -1;
  }

  return 0;

}
