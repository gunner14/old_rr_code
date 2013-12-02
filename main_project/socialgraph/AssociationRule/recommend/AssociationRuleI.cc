#include "AssociationRuleI.h"

#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/InvokeMonitor.h"
#include "util/cpp/TimeCost.h"
#include "Monitor/client/service/ServiceMonitor.h"

#include "socialgraph/AssociationRule/share/ARCreator.h"

  
void MyUtil::initialize() { 
  using namespace xce::serverstate;
  using namespace MyUtil;
  using namespace com::xiaonei::xce;
  using namespace xce::socialgraph;

  ServiceI& service = ServiceI::instance();   
  service.getAdapter()->add(&AssociationRuleI::instance(), service.createIdentity("SGAR", ""));

  ConnectionPoolManager::instance().initialize();
	TaskManager::instance().config(TASK_LEVEL_GETRULE, ThreadPoolConfig(0, 100));
	TaskManager::instance().config(TASK_LEVEL_PARSERULE, ThreadPoolConfig(0, 100));

  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("AssociationRule.Mod");
  int sInterval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("AssociationRule.Interval", 5);

  //向controller注册
	ServerStateSubscriber::instance().initialize("ControllerAssociationRule", &AssociationRuleI::instance(), 
																								mod, sInterval, new MyUtil::SocialGraphChannel());
  ServerStateSubscriber::instance().setStatus(1);
}

namespace xce {
namespace socialgraph {

  friendDataSeq AssociationRuleI::getRecommendByAssociationRule(int userId, int applyId, int limit, const Ice::Current& current) {
  	MCE_INFO("[AssociationRuleI] getRecommendByAssociationRule userid:" << userId << " applyId:" << applyId << " limit:" << limit);

    friendDataSeq rcdSeq =  ARCreator::instance().create(userId, applyId, limit); 
  	MCE_INFO("[AssociationRuleI] getRecommendByAssociationRule userid:" << userId << " applyId:" << applyId << " rcd size:" << rcdSeq.size());
    return rcdSeq;
  }

  RcdInfoSeq AssociationRuleI::getRcdByAssociationRule(int userId, int applyId, int limit, const Ice::Current& current) {
  	MCE_INFO("[AssociationRuleI] getRcdByAssociationRule userid:" << userId << " applyId:" << applyId << " limit:" << limit);

    RcdInfoSeq rcdSeq =  ARCreator::instance().createRcd(userId, applyId, limit); 
  	MCE_INFO("[AssociationRuleI] getRcdByAssociationRule userid:" << userId << " applyId:" << applyId << " rcd size:" << rcdSeq.size());
    return rcdSeq;
  }

}
}
