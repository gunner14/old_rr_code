#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/InvokeMonitor.h"
#include "util/cpp/TimeCost.h"
#include "UtilCxx/src/TaskManager.h"
#include "Monitor/client/service/ServiceMonitor.h"
#include "socialgraph/newsocialfeed/socialfeedworker/common/config_parser.h"
#include "socialgraph/newsocialfeed/socialfeedworker/builder/feed_push_task.h"

#include "socialfeedworker.h"

using namespace xce::serverstate;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace xce::socialgraph;

void  MyUtil::initialize() {

  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&SocialFeedWorker::instance(), service.createIdentity("SocialFeedWorker", ""));

  ConnectionPoolManager::instance().initialize();
  
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("SocialFeedWorker.Mod");
  int sInterval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("SocialFeedWorker.Interval", 5);

  //向controller注册     
  ServerStateSubscriber::instance().initialize("ControllerSocialFeedWorker", 
					&SocialFeedWorker::instance(), mod, sInterval, new MyUtil::SocialGraphChannel());

  ServerStateSubscriber::instance().setStatus(1);

}

//----------------------------------------------------    

namespace xce {
namespace socialgraph {

void SocialFeedWorker::NotifySocialFeedUserLogin(int userid, const Ice::Current& current) {
	cout << "SocialFeedWorker::NotifySocialFeedUserLogin userid:" << userid << endl;
        int task_size = TaskManager::instance().size(TASK_LEVEL_FEED_PUSH);   
        if (task_size > 10000){
            MCE_INFO("SocialFeedWorker::NotifySocialFeedUserLogin userid:" << userid << " task count:" << task_size); 
        }
     	TaskManager::instance().execute(new FeedPushTask(userid, 2));
}

bool  SocialFeedWorker::LoadWhiteUserList(){
        return  true;
}

}
}

int main(){
    
    SocialFeedWorker   wk;
    Ice::Current  current;
    
    wk.NotifySocialFeedUserLogin(33990, current);
     
    sleep(5);
}
