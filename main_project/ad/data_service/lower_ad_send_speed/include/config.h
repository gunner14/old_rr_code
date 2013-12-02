/*
 * =====================================================================================
 *
 *       Filename:  LowerConfig.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年08月24日
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lei.yu1 
 *        Company:  renren
 *
 * =====================================================================================
 */

#ifndef __XCE_AD_LOWER_CONFIG_H__
#define __XCE_AD_LOWER_CONFIG_H_

#include <map>
#include <string>
#include "ServiceI.h"
#include "Singleton.h"
#include "LogWrapper.h"
#include <QueryRunner.h>
#include <DbDescriptor.h>

namespace xce{
namespace ad{

class LowerConfig : public MyUtil::Singleton<LowerConfig> {
public:
    LowerConfig() : is_loading_(true) { 
    }

    double GetValue(const std::string&);

    void SetDB(const std::string& db) { db_instance_ = db; }

    void UpdateData();
private:
    std::map<std::string, double> config_;
    bool is_loading_;
    std::string db_instance_;
};

}//end of namespace ad
}//end of namespace xce

#endif
