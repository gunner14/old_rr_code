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


CtrOnlineModelMap ExperimentLoaderCache::GetCtrOnlineModelPool(const ::Ice::Current& current){
  MCE_INFO("ExperimentLoaderCache::GetCtrOnlineModelPool--> call from " << current.con->toString());
  CtrOnlineModelMap result = LrCtrLoader::instance().GetCtrOnlineModelMap();
  MCE_INFO("ExperimentLoaderCache::GetCtrOnlineModelPool, pool size:"<<result.size());
  return result;
}

AdClassListMap ExperimentLoaderCache::GetAdClassListPool(const ::Ice::Current& current){
  MCE_INFO("ExperimentLoaderCache::GetAdClassListPool--> call from " << current.con->toString());
  AdClassListMap result = LrCtrLoader::instance().GetAdClassListMap();;
  MCE_INFO("ExperimentLoaderCache::GetAdClassListPool, pool size:"<<result.size());
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

	
}
}

