/*
 * =====================================================================================
 *
 *       Filename:  degrade.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月24日 09时10分03秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef __AD_DEGRADE_H__
#define __AD_DEGRADE_H__

#include <string>
#include <vector>
#include <map>
#include <IceUtil/RWRecMutex.h>
#include <IceUtil/Thread.h>
#include "Singleton.h"
#include <connection.h>
#include <DbDescriptor.h>
#include <QueryRunner.h>

class DegradeManager: public IceUtil::Thread, public MyUtil::Singleton<DegradeManager> {

  public:

    DegradeManager():interval_(3), db_source_("ad_st"), service_name_("user"),gray_(0){}

    void setInterval(unsigned interval);
    void SetDBSource(const std::string &source);

    int Init();
    void Register(const std::string &module_name);
    bool IsGray(int tail);
    bool IsEnable(const std::string &module_name);
    void Enable(const std::string& module_name);
    void Disable(const std::string& module_name);

    void RegisterGrayName(const std::string &garyname){
			service_name_ = garyname; 
		}
    virtual void run();

  private:

    void Load();
    void LoadGray();
    void LoadModuleStatus();
    void SetModuleStatus(const std::string &module_name, int status);
    int excuteQuery(const com::xiaonei::xce::Statement &sql, 
        const std::string &table,
        mysqlpp::StoreQueryResult & res);

    struct DegradeModule{
      std::string module_name_; 
      bool is_enable_;
    };

    unsigned interval_;
    std::string db_source_;
		std::string service_name_;

    std::map<std::string, DegradeModule> modules_;
    int gray_;
};

#endif
