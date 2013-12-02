#include "Monitor.h"
#include "index.h"
#include "indexConfig.h"
#include "Close.h"
#include "ad/indexService/TIndex/src/targetingIndex.h"

using namespace xce::ad;

/*
 * Func::IndexMonitorI::GetAdPoolInfo
 * Desc::获得内存中的广告商、广告计划、广告组
 */
string IndexMonitorI::GetAdPoolInfo(int type, const Ice::Current&) {
  //0,广告商;1,广告计划;2,广告组
  string result = AdPool::instance().Print(type);
  return result;
}

/*
 * Func::IndexMonitorI::GetAdZonePrice
 * Desc::获得广告组在广告位上的价格，groupId=0，则返回内存中的所有广告组价格信息
 */
string IndexMonitorI::GetAdZonePrice(Ice::Long groupId, const Ice::Current&) {
  if (0==groupId){
    return AdPool::instance().PrintAllZonePrice();
  } else {
    return AdPool::instance().PrintZonePrice(groupId); 
  }
}

/*
 * Func::IndexMonitorI::GetZoneAdNum
 * Desc::广告位上的广告数
 */
string IndexMonitorI::GetZoneAdNum(const Ice::Current&) {
  return IndexConfig::instance().PrintZoneAdNum();
}

/*
 * Func::IndexMonitorI::GetTriggerInfo
 * Desc::trigger info
 */
string IndexMonitorI::GetTriggerInfo(const Ice::Current&) {
  return IndexConfig::instance().PrintTriggerInfo();
}

/*
 * Func::IndexMonitorI::GetGameAdNum
 * Desc::广告位上的游戏广告数
 */
string IndexMonitorI::GetGameAdNum(const Ice::Current&) {
  return IndexConfig::instance().PrintGameAdNum();
}

/*
 * Func::IndexMonitorI::GetGameMemNum
 * Desc::广告位上的游戏广告商数
 */
string IndexMonitorI::GetGameMemNum(const Ice::Current&) {
  return IndexConfig::instance().PrintGameMemNum();
}

/*
 * Func::IndexMonitorI::GetZoneIndex
 * Desc::广告位上的索引信息
 */
string IndexMonitorI::GetZoneIndex(Ice::Long zoneId, const Ice::Current&) {
  if (0 == zoneId) {
    return TargetingIndex::instance().PrintAll();
  } else {
    return TargetingIndex::instance().Print(zoneId); 
  }
}

/*
 * Func::IndexMonitorI::GetCloseAd
 * Desc::用户关闭的广告
 */
string IndexMonitorI::GetClosedAd(const Ice::Current&) {
  return AdCloseManager::instance().Print(); 
}


