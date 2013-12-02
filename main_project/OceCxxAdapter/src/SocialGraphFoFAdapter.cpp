#include "SocialGraphFoFAdapter.h"
using namespace xce;
using namespace xce::socialgraph;
using namespace clusterstate;

FoFRankSeq SocialGraphFoFAdapter::getFoFRankSeq(Ice::Int hostId, Ice::Int limit) {
  MCE_INFO("[SocialGraphFoFAdapter::getFoFRankSeq] hostId: " << hostId << " limit: " << limit);
  try {
    return getProxy(hostId)->getFoFRankSeq(hostId, limit);
  } catch(Ice::Exception &e){
    MCE_WARN( "[SocialGraphFoFAdapter::getFoFRankSeq Alert! ]"<<e );
  }
  return FoFRankSeq();
}


FoFRankSeq SocialGraphFoFAdapter::getFoFRankSeq(Ice::Int hostId, Ice::Int begin, Ice::Int limit) {
  MCE_INFO("[SocialGraphFoFAdapter::getFoFRankSeq] hostId: " << hostId << " begin: " << begin << " limit: " << limit);
  try {
    return getProxy(hostId)->getFoFRankSeqN(hostId, begin, limit);
  } catch(Ice::Exception &e){
     MCE_WARN( "[SocialGraphFoFAdapter::getFoFRankSeqN Alert! ]"<<e );
  }
  return FoFRankSeq();
}


//Int2FoFRankSeqMap SocialGraphFoFAdapter::getFoFRankSeqs(const MyUtil::IntSeq& hostIds, Ice::Int limit) {
//  MCE_INFO("[SocialGraphFoFAdapter::getFoFRankSeqs] limit: " << limit);
//  if (hostIds.empty()) {
//    return Int2FoFRankSeqMap();
//  }
//  
//  int clusterNum = static_cast<int>(getCluster()); 
//  MCE_INFO("[SocialGraphFoFAdapter::getFoFRankSeqs] cluster number " << clusterNum);
//  if (clusterNum < 0) {
//    MCE_WARN("[SocialGraphFoFAdapter::getFoFRankSeqs] cluster number less than 0");
//    return Int2FoFRankSeqMap();
//  } 
//
//  if ((clusterNum == 0) || (clusterNum ==1)) {
//    try {
//      return getProxy(hostIds.at(0))->getFoFRankSeqs(hostIds, limit);
//    } catch(...) {
//      return Int2FoFRankSeqMap();
//    }
//  } 
// 
//  MyUtil::IntSeqSeq subHostIdSeqSeq;
//  subHostIdSeqSeq.resize(clusterNum);
//  for (MyUtil::IntSeq::const_iterator it = hostIds.begin(); it != hostIds.end(); ++it) {
//    subHostIdSeqSeq.at((*it) % clusterNum).push_back((*it));
//  }
//  Int2FoFRankSeqMap hostId2FoFRankSeqMap;
//  Int2FoFRankSeqMap subMap;
//  for (int clusterIndex = 0; clusterIndex < clusterNum; ++clusterIndex) {
//    try {
//      subMap = getProxy(clusterIndex)->getFoFRankSeqs(subHostIdSeqSeq.at(clusterIndex), limit);
//    } catch (...) {
//      MCE_WARN("[SocialGraphFoFAdapter::getFoFRankSeqs] from Proxy at cluster " << clusterNum << " handle unknown exception");
//    }
//    hostId2FoFRankSeqMap.insert(subMap.begin(), subMap.end());
//  }
//
//  return hostId2FoFRankSeqMap;
//}
//
//
//Int2FoFRankSeqMap SocialGraphFoFAdapter::getFoFRankSeqs(const MyUtil::IntSeq& hostIds, Ice::Int begin, Ice::Int limit) {
//  MCE_INFO("[SocialGraphFoFAdapter::getFoFRankSeqs] begin: " << begin << " limit: " << limit);
//  if (hostIds.empty()) {
//    return Int2FoFRankSeqMap();
//  }
//  
//  int clusterNum = static_cast<int>(getCluster()); 
//  MCE_INFO("[SocialGraphFoFAdapter::getFoFRankSeqs] cluster number " << clusterNum);
//  if (clusterNum < 0) {
//    MCE_WARN("[SocialGraphFoFAdapter::getFoFRankSeqs] cluster number less than 0");
//    return Int2FoFRankSeqMap();
//  } 
//
//  if ((clusterNum == 0) || (clusterNum ==1)) {
//    try {
//      return getProxy(hostIds.at(0))->getFoFRankSeqsN(hostIds, begin, limit);
//    } catch(...) {
//      return Int2FoFRankSeqMap();
//    }
//  } 
// 
//  MyUtil::IntSeqSeq subHostIdSeqSeq;
//  subHostIdSeqSeq.resize(clusterNum);
//  for (MyUtil::IntSeq::const_iterator it = hostIds.begin(); it != hostIds.end(); ++it) {
//    subHostIdSeqSeq.at((*it) % clusterNum).push_back((*it));
//  }
//  Int2FoFRankSeqMap hostId2FoFRankSeqMap;
//  Int2FoFRankSeqMap subMap;
//  for (int clusterIndex = 0; clusterIndex < clusterNum; ++clusterIndex) {
//    try {
//      subMap = getProxy(clusterIndex)->getFoFRankSeqsN(subHostIdSeqSeq.at(clusterIndex), begin, limit);
//    } catch (...) {
//      MCE_WARN("[SocialGraphFoFAdapter::getFoFRankSeqs] from Proxy at cluster " << clusterNum << " handle unknown exception");
//    }
//    hostId2FoFRankSeqMap.insert(subMap.begin(), subMap.end());
//  }
//
//  return hostId2FoFRankSeqMap;
//}
//
