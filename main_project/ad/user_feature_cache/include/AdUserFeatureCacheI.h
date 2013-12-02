/*
 * AdUserFeatureCacheI.h
 *
 * Created on: 2012-03-16
 *     Author: lei.yui
 */

#ifndef AD_USER_FEATURE_CACHE_H_
#define AD_USER_FEATURE_CACHE_H_

#include "LogWrapper.h"
#include "client/linux/handler/exception_handler.h"
#include "../../util/src/AdCommonUtil.h"
#include <IceUtil/Handle.h>
#include "TaskManager.h"
#include "Singleton.h"
#include "AdUserFeatureCache.h"
#include "../include/AdUserFeatureData.h"

namespace xce{
namespace ad{

typedef IceUtil::Handle<AdUserFeatureData> UserFeatureDataPtr;

class AdUserFeatureCacheI : public AdUserFeatureCache, public MyUtil::Singleton<AdUserFeatureCacheI> {
public:
    AdUserFeatureCacheI();

    void BeginLoad() { isLoading_ = true; initOK_ = false; }
    void EndLoad() { isLoading_ = false; initOK_ = true; }
    bool Load(const std::string& fileName);

    virtual std::string GetFeature(Ice::Long userID, const Ice::Current& = Ice::Current());

    virtual bool Rollback(const std::string& fileName, const Ice::Current& = Ice::Current());

    bool HasNewFile();
    std::string GetNewFile();
    virtual std::string GetNowFile(const Ice::Current& = Ice::Current());

private:
    bool initOK_;
    bool isLoading_;
    UserFeatureDataPtr userFeatureDataPtr_;
    
    std::string newFileName_;
    std::string newMd5Value_;
    
    friend class AdUserFeatureCacheITest;
};// end of class AdUserFeatureCacheI

class LoadTask : virtual public MyUtil::Task{
public:
    LoadTask(){}
    virtual void handle();
};//end of class LoadTask

}//end of namespace ad
}//end of namespace xce

#endif
