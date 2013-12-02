#include "ServiceI.h"
#include "IceExt/src/Channel.h"
#include "../include/BrandEdmQuotaLoader.h"
#include "../include/SelfEdmQuotaLoader.h"
#include "../include/LeftAdEdmQuotaLoader.h"
#include "../include/ProductEdmQuotaLoader.h"
#include "../include/BrandEdmFilter.h"
#include "../include/SelfEdmFilter.h"
#include "../include/LeftAdEdmFilter.h"
#include "../include/ProductEdmFilter.h"
#include "../include/AdEdmMonitorI.h"
#include "../../../util/src/AdCommonUtil.h"
#include "client/linux/handler/exception_handler.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

USE_NS(std)
USE_NS(MyUtil)
USE_NS(xce::edm)

const unsigned int HALF_MINUTE = 30 * 1000 * 1000;
const int THREE_CLOCK = 3;

bool MyCallback(const char *dump_path, const char *minidump_id, void *context, bool succeeded){
    printf("got crash %s %s\n", dump_path, minidump_id);
    return true;
}

void MyUtil::initialize(){
    //google breakpad for dump
    srand(time(NULL));
    static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
    // service start
    ServiceI& service = ServiceI::instance();
    service.getAdapter()->add(&AdEdmMonitorI::instance(), service.createIdentity("ADEDM", ""));
    TaskManager::instance().execute(new LoadTask());

    int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("AdEdmMonitor.Mod");
    int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("AdEdmMonitor.Interval",5);
    xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerAdEdmMonitor",&AdEdmMonitorI::instance(),mod,interval,new EdmChannel());
    MCE_INFO("MyUtil::initialize start ");
}

AdEdmMonitorI::AdEdmMonitorI() {
    loader_[EdmBrand] = EdmQuotaLoaderPtr(new BrandEdmQuotaLoader());
    loader_[EdmSelf] = EdmQuotaLoaderPtr(new SelfEdmQuotaLoader());
    loader_[EdmLeftAd] = EdmQuotaLoaderPtr(new LeftAdEdmQuotaLoader());
    loader_[EdmProduct] = EdmQuotaLoaderPtr(new ProductEdmQuotaLoader());

    filter_[EdmBrand] = EdmFilterPtr(new BrandEdmFilter());
    filter_[EdmSelf] = EdmFilterPtr(new SelfEdmFilter());
    filter_[EdmLeftAd] = EdmFilterPtr(new LeftAdEdmFilter());
    filter_[EdmProduct] = EdmFilterPtr(new ProductEdmFilter());

    time_t now = time(NULL);
    struct tm* time = localtime(&now);
    time->tm_hour = 3;
    last_clear_time_ = mktime(time);
    MCE_INFO("AdEdmMonitor::AdEdmMonitorI --> last_clear_time init: " << last_clear_time_);
}

EdmSeq AdEdmMonitorI::Filter(const EdmSeq& edmSeq, const Ice::Current&){
  EdmSeq resultSeq;
	ostringstream adIDs;
 	MCE_DEBUG("Filter size:"<<edmSeq.size());
  for ( EdmSeq::const_iterator vIter = edmSeq.begin();
  vIter != edmSeq.end(); ++vIter ){
    loader_[vIter->type]->GetQuota(*vIter);
		adIDs << vIter->adGroupID << " ";
     if (filter_[vIter->type]->IsOffline(*vIter)){
	     MCE_INFO("AdEdmMonitor::Filter --> skip: CampaignID: " << vIter->campaignID << " AdGroupID: " << vIter->adGroupID << " Type:" << vIter->type);
       continue;
     }
	   resultSeq.push_back(*vIter);
  }
  if ((edmSeq.size() > resultSeq.size()) && (resultSeq.empty())) {
    // 15.15 
    MCE_INFO("AdEdmMonitor::Filter-->before_size: "<< edmSeq.size() << " after_size: "<< resultSeq.size() << " type: " << edmSeq[0].type << " AdGroupIDs: " << adIDs.str());
  }
  return resultSeq;
}

void AdEdmMonitorI::UpdateData(){
  EdmMonitorConfig::instance().Load();
  /*if(filter_.find(EdmBrand) != filter_.end()){
    MCE_INFO("AdEdmMonitorI::LoadConfig()");
    filter_[EdmBrand]->LoadConfig();
  }*/
  for (map<EdmType, EdmQuotaLoaderPtr>::const_iterator mIter = loader_.begin();
    mIter != loader_.end(); ++mIter) {
    MCE_INFO("AdEdmMonitorI::UpdateData() --> type: " << mIter->first);
    mIter->second->UpdateData();
  }
  for (map<EdmType, EdmFilterPtr>::const_iterator mIter = filter_.begin();
    mIter != filter_.end(); ++mIter) {
    MCE_INFO("AdEdmMonitorI::CalcSendRatio() --> type: " << mIter->first);
    mIter->second->CalcSendRatio(loader_[mIter->first]->GetAdGroupRecords());
  }
}

void AdEdmMonitorI::Clear(){
    time_t now = time(NULL);
    if (now < (last_clear_time_ + ONE_DAY)){
	return;
    } 
    struct tm* time = localtime(&now);
    time->tm_hour = THREE_CLOCK;
    time->tm_min = ZERO;
    last_clear_time_ = mktime(time);
    MCE_INFO("AdEdmMonitorI::Clear() --> last_clear_time: " << last_clear_time_);

    for (map<EdmType, EdmQuotaLoaderPtr>::const_iterator mIter = loader_.begin(); 
	mIter != loader_.end(); ++mIter) {
	mIter->second->Clear();
    }
}

void LoadTask::handle(){
    try {
        while(true) {
      	    MCE_INFO("LoadTask::handle --> AdEdmMonitorI::load start");
      	    AdEdmMonitorI::instance().UpdateData();
	    AdEdmMonitorI::instance().Clear();
      	    MCE_INFO("LoadTask::handle --> AdEdmMonitorI::load done");
      	    usleep(HALF_MINUTE);
    	}
    } catch (std::exception& e) {
    	MCE_WARN("LoadTask--> handle ERR  line:" << __LINE__ << " err:" << e.what());
    } catch (...) {
    	MCE_WARN("LoadTask--> handle ERR  line:" << __LINE__);
    }
}
