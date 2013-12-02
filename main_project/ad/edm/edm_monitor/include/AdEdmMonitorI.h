/*
 * =====================================================================================
 *
 *       Filename:  AdEdmMonitorI.h
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

#ifndef __XCE_EDM_MONITORI_H_
#define __XCE_EDM_MONITORI_H_

#include <map>
#include <vector>
#include <string>
#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>
#include "AdEdmMonitor.h"
#include "TaskManager.h"
#include "Singleton.h"
#include "EdmFilter.h"
#include "EdmQuotaLoader.h"
#include "../common/edm_monitor_constant.h"

NS_BEGIN(xce, edm)

class AdEdmMonitorI : public AdEdmMonitor, public MyUtil::Singleton<AdEdmMonitorI>{
public:
    AdEdmMonitorI();

    void UpdateData();
    void Clear();
    virtual EdmSeq Filter(const EdmSeq& edmSeq, const Ice::Current&);

private:
    std::map<EdmType, EdmFilterPtr> filter_;
    std::map<EdmType, EdmQuotaLoaderPtr> loader_;

    time_t last_clear_time_;
};

class LoadTask : virtual public MyUtil::Task{
public:
    LoadTask(){}
    virtual void handle();
};//end of class LoadTask

NS_END

#endif
