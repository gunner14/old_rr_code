#include "UserBaseI.h"
#include "LogWrapper.h"
#include "UserLogicAdapter.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "Monitor/client/service/ServiceMonitor.h"

using namespace com::xiaonei::xce;
using namespace mop::hi::svc::adapter;
using namespace xce::userbase;
using namespace MyUtil;
using namespace xce::serverstate;

void MyUtil::initialize() {
  ServiceI& service=ServiceI::instance();
  service.getAdapter()->add(&UserLoginCountManagerI::instance(), service.createIdentity(USER_LOGINCOUNT_MANAGER, ""));

  //-----------------------------------HA---------------------------------------------
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("UserBaseReader.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("UserBaseReader.Interval",5);

  map<string,Ice::ObjectPtr> mapNameAndService;
  mapNameAndService.insert(make_pair<string,Ice::ObjectPtr>(USER_LOGINCOUNT_MANAGER,&UserLoginCountManagerI::instance()));
  ServerStateSubscriber::instance().initialize("ControllerUserBaseReader",mapNameAndService,mod,interval);

  {
    int status = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("UserBaseReader.Status",1);
    ServerStateSubscriber::instance().setStatus(status,USER_LOGINCOUNT_MANAGER);
    ServerStateSubscriber::instance().setStatus(status);
    std::ostringstream os;
    os<<"MyUtil::initialize set status : "<<status;
    MCE_WARN( os.str() );
  }
  ServiceMonitorManager::instance().start();
}

int UserLoginCountManagerI::getUserLoginCount(Ice::Int userId,
    const Ice::Current& current) {
  ostringstream inMsg;
  inMsg << "userid = " << userId;
  InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

  return 0;
}

void UserLoginCountManagerI::incUserLoginCount(Ice::Int userId,
    const Ice::Current& current) {
  ostringstream inMsg;
  inMsg << "userid = " << userId;
  InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

  MyUtil::Str2StrMap props;
  //UserLogicAdapter::instance().updateUserLoginCount(userId, props, current.ctx);
}

void UserLoginCountManagerI::setData(const ObjectResultPtr& userData,
    const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current);
  for(MyUtil::ObjectMap::iterator it = userData->data.begin();it != userData->data.end();++it) {
    UserLoginCountDataPtr data = UserLoginCountDataPtr::dynamicCast(it->second);
    int userId = data->uID; 
    MCE_DEBUG("UserLoginCountManagerI::setData userid = " << userId);
    MyUtil::Str2StrMap props;
    //UserLogicAdapter::instance().updateUserLoginCount(userId, props, current.ctx);
  }
}

void UserLoginCountManagerI::setValid(bool valid,
    const Ice::Current& current) {
  ostringstream inMsg;
  inMsg << "valid = " << valid;
  InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

  ServerStateSubscriber::instance().setStatus((int)valid, USER_LOGINCOUNT_MANAGER);
}

bool UserLoginCountManagerI::isValid(const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, "", InvokeClient::INVOKE_LEVEL_INFO);

  return ServerStateSubscriber::instance().getStatus(USER_LOGINCOUNT_MANAGER);
}
