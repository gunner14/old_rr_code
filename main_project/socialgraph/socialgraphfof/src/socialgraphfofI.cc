#include "socialgraph/socialgraphfof/src/socialgraphfofI.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/InvokeMonitor.h"
#include "Monitor/client/service/ServiceMonitor.h"
#include "socialgraph/socialgraphutil/socialgraphworkerI.h"


/**************************************************************************************************/
//redis initialization 
void MyUtil::initialize() {
  using namespace xce::serverstate;
  using namespace MyUtil;
  using namespace com::xiaonei::xce;
  using namespace xce::socialgraph;

  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&SocialGraphFoFI::instance(), service.createIdentity("FoF", ""));

  //TaskManager::instance().config(LEVEL_ADD, ThreadPoolConfig(1,1));
  ConnectionPoolManager::instance().initialize();

  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("SocialGraphFoF.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("SocialGraphFoF.Interval", 5);

  //register controller
  ServerStateSubscriber::instance().initialize("ControllerSocialGraphFoF", &SocialGraphFoFI::instance(), mod, interval, new MyUtil::SocialGraphChannel);
  ServerStateSubscriber::instance().setStatus(1);

  //register zookeeper
  //SocialGraphFoFI::instance().Register("FoF", "ControllerSocialGraphFoF");

  //start up monitor
  ServiceMonitorManager::instance().start();
}


namespace xce {
namespace socialgraph {
using namespace MyUtil;
using namespace std;

typedef Int2FoFRankSeqMap::const_iterator Int2FoFRankSeqMapIter;

FoFRankSeq SocialGraphFoFI::getFoFRankSeq(Ice::Int hostId, Ice::Int limit, const Ice::Current&) {
  MCE_INFO("[SocialGraphFoFI::getFoFRankSeq] hostId: " << hostId << " limit: " << limit);
  //return FoFRankCompute::getFoFRankSeq(hostId, 0, limit);
  try {
    return FoFRankCompute::getFoFRankSeq(hostId, 0, limit);
  } catch (Ice::Exception& e) {
    MCE_WARN("[SocialGraphFoF::getFoFRankSeq] hostId: " << hostId << " limit: " << limit << " Ice Exception");
  } catch (std::exception& e) {
    MCE_WARN("[SocialGraphFoF::getFoFRankSeq] hostId: " << hostId << " limit: " << limit << " std Exception" << e.what());
  } catch (...) {
    MCE_WARN("[SocialGraphFoF::getFoFRankSeq] hostId: " << hostId << " limit: " << limit << " unknown Exception");
  }
  return FoFRankSeq();
}

FoFRankSeq SocialGraphFoFI::getFoFRankSeqN(Ice::Int hostId, Ice::Int begin, Ice::Int limit, const Ice::Current&) {
  MCE_INFO("[SocialGraphFoFI::getFoFRankSeqN] hostId: " << hostId << " begin: " << begin << " limit: " << limit);
  try {
    return FoFRankCompute::getFoFRankSeq(hostId, begin, limit);
  } catch (Ice::Exception& e) {
    MCE_WARN("[SocialGraphFoF::getFoFRankSeq] hostId: " << hostId << " limit: " << limit << " Ice Exception");
  } catch (std::exception& e) {
    MCE_WARN("[SocialGraphFoF::getFoFRankSeq] hostId: " << hostId << " limit: " << limit << " std Exception" << e.what());
  } catch (...) {
    MCE_WARN("[SocialGraphFoF::getFoFRankSeq] hostId: " << hostId << " limit: " << limit << " unknown Exception");
  }
  return FoFRankSeq();
}

//Int2FoFRankSeqMap SocialGraphFoFI::getFoFRankSeqs(const MyUtil::IntSeq& hostIds, Ice::Int limit, const Ice::Current&) {
//  MCE_INFO("[SocialGraphFoFI::getFoFRankSeqs] limit: " << limit);
//  Int2FoFRankSeqMap hostId2FoFRankSeqMap;
//  for (IntVecIter it = hostIds.begin(); it != hostIds.end(); ++it){
//    hostId2FoFRankSeqMap.insert(make_pair(*it, FoFRankCompute::getFoFRankSeq(*it, 0, limit)));
//  }
//  return hostId2FoFRankSeqMap;
//}

//Int2FoFRankSeqMap SocialGraphFoFI::getFoFRankSeqsN(const MyUtil::IntSeq& hostIds, Ice::Int begin, Ice::Int limit, const Ice::Current&) {
//  MCE_INFO("[SocialGraphFoFI::getFoFRankSeqs] begin: " << begin << " limit: " << limit);
//  Int2FoFRankSeqMap hostId2FoFRankSeqMap;
//  for (IntVecIter it = hostIds.begin(); it != hostIds.end(); ++it){
//    hostId2FoFRankSeqMap.insert(make_pair(*it, FoFRankCompute::getFoFRankSeq(*it, begin, limit)));
//  }
//  return hostId2FoFRankSeqMap;
//}

//void SocialGraphFoFI::setFoFRank(Ice::Int hostId, const FoFRank&, const Ice::Current&) {
//}
//
//void SocialGraphFoFI::setFoFRanks(Ice::Int hostId, const FoFRankSeq&, const Ice::Current&) {
//}
//
//
//void SocialGraphFoFI::AddFoFRank(Ice::Int hostId, const FoFRank&, const Ice::Current&) {
//}
//
//void SocialGraphFoFI::DelFoFRank(Ice::Int hostId, const FoFRank&, const Ice::Current&) {
//}

};
};
