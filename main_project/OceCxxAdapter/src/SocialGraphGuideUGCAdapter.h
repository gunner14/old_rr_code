#ifndef _GUIDE_USER_HIGH_QUALITY_UGC_ADAPTER_H_
#define _GUIDE_USER_HIGH_QUALITY_UGC_ADAPTER_H_

#include "GuideUserHighQualityUGC.h"
#include "Channel.h"
#include "AdapterI.h"
#include "Singleton.h"

namespace  xce {
namespace  socialgraph {
class  SocialGraphGuideUGCAdapter: public MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::GuideUserHighQualityUGCPrx>,
                                   public MyUtil::Singleton<SocialGraphGuideUGCAdapter> {
 public: 
   SocialGraphGuideUGCAdapter():
        MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::GuideUserHighQualityUGCPrx>("ControllerGuideUGCReshow",
								            120, 5000, new MyUtil::SocialGraphChannel()) { }
   void  NotifyUserLogin(int userid);
};

};
};

#endif
