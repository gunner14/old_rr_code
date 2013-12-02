#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/InvokeMonitor.h"
#include "util/cpp/TimeCost.h"
//#include "UtilCxx/src/TaskManager.h"
#include "Monitor/client/service/ServiceMonitor.h"
#include "socialgraph/GuideHighQualityUGC/common/config_parser.h"
#include "socialgraph/GuideHighQualityUGC/builder/ugc_feed_builder.h"
#include "OceCxxAdapter/src/UserStateInfo.h"
#include "OceCxxAdapter/src/UserStateTripodAdapter.h"
#include "UserBase.h"
#include "guideuserUGCreshow.h"

using namespace xce::serverstate;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace xce::socialgraph;
using namespace xce::socialgraph::guideugc;
using namespace xce::adapter::userstate;

void  MyUtil::initialize() {

  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&GuideUserUGCReshow::instance(), service.createIdentity("GuideUGCReshow", ""));

  ConnectionPoolManager::instance().initialize();
  
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("GuideUGCReshow.Mod");
  int sInterval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("GuideUGCReshow.Interval", 5);

  //向controller注册     
  ServerStateSubscriber::instance().initialize("ControllerGuideUGCReshow", 
				 	&GuideUserUGCReshow::instance(), mod, sInterval, new MyUtil::SocialGraphChannel());

  ServerStateSubscriber::instance().setStatus(1);

}

//----------------------------------------------------    

namespace xce {
namespace socialgraph {

void  GuideUserUGCReshow::NotifyUserLogin(int userid, const Ice::Current& current) {
       MCE_INFO("GuideUserUGCReshow::NotifyUserLogin login userid:" << userid);
       if (IsGuideUser(userid)){
           int count = GuideUserUGCFeedMgr::instance().PushUGCFeeds(userid, 3); 
           MCE_INFO("GuideUserUGCReshow::NotifyUserLogin push to guide userid:" << userid << " " << count << " ugc feeds");
       }
}

bool  GuideUserUGCReshow::IsGuideUser(int userid){
       bool  ret  = false;
       try{
          UserStateInfoPtr stateptr = UserStateTripodAdapter::instance().Get(userid);
          long  state    = stateptr->state();
          long  bit_test = 1 << (xce::userbase::CStateGUIDEDONE);
          if ((state & bit_test) == 0){
              ret = true;
          }            
       }catch (Ice::Exception& e) {
           MCE_WARN("GuideUserUGCReshow::IsGuideUser ice exception " << e.what());
       } catch (std::exception& e) {
           MCE_WARN("GuideUserUGCReshow::IsGuideUser std exception " << e.what());
       } catch (...) {
           MCE_WARN("GuideUserUGCReshow::IsGuideUser unknow exception");
       }     
       return  ret;       
}

}
}
