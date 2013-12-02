#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>
#include "ServiceI.h"
#include "IceExt/src/Channel.h"
#include "FeedExperimentCache.h"
#include "client/linux/handler/exception_handler.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

using namespace std;
using namespace boost;
using namespace MyUtil;
using namespace xce::feed;
using namespace mysqlpp;

bool MyCallback(const char *dump_path, const char *minidump_id, void *context, bool succeeded) {
  printf("got crash %s %s\n", dump_path, minidump_id);
  return true;
}

void MyUtil::initialize() {
  //google breakpad for dump
  srand(time(NULL));
  static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
  // service start
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&FeedExperimentCache::instance(), service.createIdentity("FEC", ""));

  FeedExperimentCache::instance().Initialize();

  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("FeedExperimentCache.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FeedExperimentCache.Interval",5);
  xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerFeedExperimentCache",&FeedExperimentCache::instance(),mod,interval,new XceFeedControllerChannel());
  MCE_INFO("MyUtil::initialize start ");
}

void FeedExperimentCache::Initialize() {
  stringstream sql;
  sql << "SELECT version FROM feature_data_config WHERE feature_name = 'bayes' AND valid = 1";
  StoreQueryResult res;
  try {
    Connection conn("feed_sys", "10.4.16.98", "feed", "rrfeed");
    Query query(&conn, true, sql.str().c_str());
    res = query.store();
  } catch (Ice::Exception& e) { 
    MCE_WARN("FeedExperimentCache::Initialize --> ice error! " << e.what());
    return;
  } catch (std::exception& e) {
    MCE_WARN("FeedExperimentCache::Initialize --> std error! " << e.what());
    return;
  }

  if (!res) {
    MCE_WARN("FeedExperimentCache::Initialize --> Query Result Error!");
    return;
  }  
  for (size_t i = 0; i < res.num_rows(); i++) {
    mysqlpp::Row row = res.at(i);
    try {
      int version = boost::lexical_cast<int>(row["version"]);
      BayesModelLoaderPtr loader = new BayesModelLoader(version);
      bayes_model_loaders_[version] = loader;
    } catch (...) {
      MCE_WARN("FeedExperimentCacheI::Initialize --> Parser db config error. cast ubc error");
    }
  }
  MCE_INFO("FeedExperimentCacheI::Initialize --> Initialize finished. There are " << bayes_model_loaders_.size() << " loaders now");
}

BayesModelList FeedExperimentCache::GetBayesModels(int mod, int begin, int size, const ::Ice::Current& current) {
  stringstream log;
  log << "FeedExperimentCache::GetBayesModels --> call from FeedNews" << mod << "  begin=" << begin << " size=" << size;
  BayesModelList result;
  IceUtil::RWRecMutex::RLock lock(mutex_);
  std::map<int, int>::const_iterator it = mod2version_.find(mod);
  if (it != mod2version_.end()) { //查找对应的BayesModelLoader版本号
    std::map<int, BayesModelLoaderPtr>::const_iterator iter = bayes_model_loaders_.find(it->second);
    if (iter != bayes_model_loaders_.end() && iter->second) { //对应的BayesModelLoader
      (iter->second)->GetBayesModels(begin, size, result);
      log << " result's size=" << result.size();
      MCE_INFO(log.str());
    } else {
      log << " No BayesModelLoader found for version:" << it->second;
      MCE_WARN(log.str());
    }
  } else {
    log << " No BayesModelLoader version found for mod:" << mod;
    MCE_WARN(log.str());
  }
  return result;
}

void FeedExperimentCache::UpdateMod2Version(int version, MyUtil::IntSeq mods) {
  IceUtil::RWRecMutex::WLock lock(mutex_);
  for(int i = 0; i < mods.size(); ++i) {
    int mod = mods[i];
    mod2version_[mod] = version;
  }
}
