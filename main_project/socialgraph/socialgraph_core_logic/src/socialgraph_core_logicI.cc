#include "socialgraph_core_logicI.h"

#include <boost/foreach.hpp>

#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/InvokeMonitor.h"
#include "util/cpp/TimeCost.h"
#include "Monitor/client/service/ServiceMonitor.h"

#include "socialgraph/socialgraphutil/config_parser2.h"

void MyUtil::initialize() {
  using namespace xce::serverstate;
  using namespace xce::socialgraph;
  using namespace MyUtil;
  using namespace com::xiaonei::xce;

  ServiceI& service = ServiceI::instance();
  
  service.getAdapter()->add(&SocialGraphCoreLogicI::instance(),
    service.createIdentity("SGCL", ""));
	
	TaskManager::instance().config(LEVEL_NOTIFY_HANDLER, ThreadPoolConfig(1, 100));
  ConnectionPoolManager::instance().initialize();

  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("SocialGraphCoreLogic.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("SocialGraphCoreLogic.Interval", 5);

  //register in controller
  ServerStateSubscriber::instance().initialize("ControllerSocialGraphCoreLogic", 
					&SocialGraphCoreLogicI::instance(), mod, interval, new MyUtil::SocialGraphChannel());
  ServerStateSubscriber::instance().setStatus(1);


  //Get socialgraph configuration
  ConfigParser::instance().Initialize("/data/xce/XSocialGraph/sg_conf/socialgraph_message.conf");
}

//-------------------------------------------------------------------

namespace xce {
namespace socialgraph {

void SocialGraphCoreLogicI::SendSGMessage(const SGMessage& message, const Ice::Current& current) {
	MCE_INFO("[SocialGraphCoreLogicI] SendSGMessage id:" << message.id 
					<< " field:" << message.field << " source:" << message.source
					<< " target:" << message.target << " action:" << message.action);
	MyUtil::TaskManager::instance().execute(new NotifyTask(message));
}

//-------------------------------------------------------------------

void NotifyTask::handle() {
	StrItemSeq str_seq = ConfigParser::instance().GetConfigAsStrItemSeq(BEHAVIOUR, message_.action);
	
	BOOST_FOREACH (const std::string& str, str_seq) {
		HandlerPtr ptr = HandlerFactory::Create(str);
		if (ptr.get() != NULL) {
			ptr->Update(message_);
		}
	}
}

}
}
