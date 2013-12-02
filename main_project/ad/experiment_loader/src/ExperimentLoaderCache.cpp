/*
 *  ExperimentLoaderCache.cpp
 *  @Author: jieyu.zeng
 *  @Date: 2012-01-05
 *  @Description: 实验数据cache
 *
 */

#include "ExperimentLoaderCache.h"
#include "LrCtrLoader.h"
#include <query.h>
#include <string>
#include <algorithm>
#include "AdConfig.h"
#include "Util.h"
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <connection.h>
#include <query.h>


using namespace boost;

bool MyCallback(const char *dump_path, const char *minidump_id, void *context, bool succeeded) {
  printf("got crash %s %s\n", dump_path, minidump_id);
  return true;
}

void ::MyUtil::initialize() {
 // static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&xce::ad::ExperimentLoaderCache::instance(), service.createIdentity("M",""));

  MyUtil::TaskManager::instance().schedule(new xce::ad::AdExperimentConfig::ConfigLoadTimer(0));
 
  xce::ad::LrCtrLoader::instance().LoadPool();
  TaskManager::instance().schedule(new xce::ad::LrCtrLoaderTimer());
}

namespace xce{
namespace ad{
using namespace mysqlpp;

CtrOnlineModelMap ExperimentLoaderCache::GetCtrOnlineModelPoolById(int id, const ::Ice::Current& current){
  MCE_INFO("ExperimentLoaderCache::GetCtrOnlineModelPoolById--> model id = " << id <<" call from " << current.con->toString());
  CtrOnlineModelMap result = LrCtrLoader::instance().GetCtrOnlineModelMap(id);
  MCE_INFO("ExperimentLoaderCache::GetCtrOnlineModelPoolById, pool size:"<<result.size());
  return result;
}

AdClassListMap ExperimentLoaderCache::GetAdClassListPool(const ::Ice::Current& current){
  MCE_INFO("ExperimentLoaderCache::GetAdClassListPool--> call from " << current.con->toString());
  AdClassListMap result = LrCtrLoader::instance().GetAdClassListMap();;
  MCE_INFO("ExperimentLoaderCache::GetAdClassListPool, pool size:"<<result.size());
  return result;
}

AdExtraDataMap ExperimentLoaderCache::GetAdExtraDataPool(const ::Ice::Current& current){
  MCE_INFO("ExperimentLoaderCache::GetAdExtraDataPool--> call from " << current.con->toString());
  AdExtraDataMap result = LrCtrLoader::instance().GetAdExtraDataMap();;
  MCE_INFO("ExperimentLoaderCache::GetAdExtraDataPool, pool size:" << result.size());
  return result;
}

AdCtrFeatureList ExperimentLoaderCache::GetAdCtrFeaturePool(const ::Ice::Current& current){
  MCE_INFO("ExperimentLoaderCache::GetAdCtrFeaturePool--> call from " << current.con->toString());
  AdCtrFeatureList result = LrCtrLoader::instance().GetAdCtrFeatureList();
  MCE_INFO("ExperimentLoaderCache::GetAdCtrFeaturePool, pool size:"<<result.size());
  return result;
}

AdCtrRealTimeMap ExperimentLoaderCache::GetAdCtrRealTimePool(const ::Ice::Current& current){
  MCE_INFO("ExperimentLoaderCache::GetCtrRealTimPool--> call from " << current.con->toString());
  AdCtrRealTimeMap result = LrCtrLoader::instance().GetAdCtrRealTimeMap();;
  MCE_INFO("ExperimentLoaderCache::GetCtrRealTimPool,pool size:"<<result.size());
  return result;
}

AdRefreshCtrRatioMap ExperimentLoaderCache::GetAdRefreshCtrRatioPoolById(int id, const ::Ice::Current& current){
  MCE_INFO("ExperimentLoaderCache::GetAdRefreshCtrRatioPoolById--> model id = "<< id<< " call from " << current.con->toString());
  AdRefreshCtrRatioMap result = LrCtrLoader::instance().GetAdRefreshCtrRatioPool(id);
  MCE_INFO("ExperimentLoaderCache::GetAdRefreshCtrRatioPoolById, pool size:"<<result.size());
  return result;
}

string ExperimentLoaderCache::GetModelMd5sumById(int id, const ::Ice::Current& current){
  MCE_INFO("ExperimentLoaderCache::GetModelMd5sumById--> model id = "<< id<< " call from " << current.con->toString());
  string result = LrCtrLoader::instance().GetModelMd5sum(id);
  MCE_INFO("ExperimentLoaderCache::GetModelMd5sumById, md5sum:"<< result);
  return result;
}

AdMemberIndustryMap ExperimentLoaderCache::GetAdMemberIndustryPool(const ::Ice::Current& current){
  MCE_INFO("ExperimentLoaderCache::GetAdMemberIndustryPool--> call from " << current.con->toString());
  AdMemberIndustryMap result = LrCtrLoader::instance().GetAdMemberIndustryPool();;
  MCE_INFO("ExperimentLoaderCache::GetAdMemberIndustryPool, pool size:"<<result.size());
  return result;
}
	
}
}

