#include "SocialGraphGuideUGCAdapter.h"
using namespace xce;
using namespace xce::socialgraph;
using namespace clusterstate;

void  SocialGraphGuideUGCAdapter::NotifyUserLogin(int hostId){
    
      MCE_INFO("[SocialGraphGuideUGCAdapter::NotifyUserLogin]  hostId: " << hostId);
      try {
            getProxy(hostId)->NotifyUserLogin(hostId);
      } catch(Ice::Exception &e){
            MCE_WARN( "[SocialGraphGuideUGCAdapter::NotifyUserLogin  Alert! ]" << e);
      }
}
