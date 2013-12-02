/*
 * =====================================================================================
 *
 *       Filename:  EdmMonitorConfig.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年04月27日
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lei.yu1 
 *        Company:  renren
 *
 * =====================================================================================
 */

#ifndef __XCE_EDM_MONITOR_CONFIG_H_
#define __XCE_EDM_MONITOR_CONFIG_H_

#include <map>
#include <string>
#include <query.h>
#include "Singleton.h"
#include "LogWrapper.h"
#include "TaskManager.h"
#include <connection.h>
#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>
#include "gtest/gtest_prod.h"
#include "../common/ns_common.h"
#include "../../../util/src/AdCommonUtil.h"
#include "../common/edm_monitor_constant.h"

NS_BEGIN(xce, edm)

class EdmMonitorConfigUnitTest;

class EdmMonitorConfig : public MyUtil::Singleton<EdmMonitorConfig> {
public:
    EdmMonitorConfig() : isLoading_(true) {}

    void Load();

    double GetValue(const std::string&);

protected:
    void Reset(const std::string& name, mysqlpp::StoreQueryResult& res);
	IceUtil::RWRecMutex mutex_;

private:
    std::map<std::string, double> config_;
    bool isLoading_;
    friend class EdmMonitorConfigUnitTest;
};

NS_END

#endif
