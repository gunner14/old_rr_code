/*
 * =====================================================================================
 *
 *       Filename:  ExperimentManager.h
 *
 *    Description:  模型数据管理
 *
 *        Version:  1.0
 *        Created:  2012年11月09日 14时06分38秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Duo Zhang (), duo.zhang@renren-inc.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _FEEDNEWS_EXPERIMENTMANAGER_H_
#define _FEEDNEWS_EXPERIMENTMANAGER_H_
#include "Singleton.h"
#include "FeedExperimentCacheReplicaAdapter.h"
#include "FeedExperiment.h"
#include <string>
#include <map>

#define NOT_FOUND_VALUE -1

namespace xce {
namespace feed {

class ExperimentManager : public MyUtil::Singleton<ExperimentManager> {
  typedef std::map<std::string, double> ModelData;
public:
  ExperimentManager() : mod_(-1) {
  }

  void Init(int mod) {
    mod_ = mod;
  }

  void LoadExperiment(int totalsize) {
    static const int stepsize = 10000;
    ModelData temp;
    try {
      for(int i = 0; i < 100; ++i) {
        BayesModelList modellist = FeedExperimentCacheReplicaAdapter::instance().GetBayesModels(mod_, i * stepsize, stepsize);
        if (modellist.empty()) {
          break;
        }
        for(BayesModelList::const_iterator it = modellist.begin(); it != modellist.end(); ++it) {
          temp[it->key] = it->value;
        }
      }
    } catch (const Ice::Exception& e) {
      MCE_WARN("ExperimentManager::LoadExperiment Ice::Exception: " << e.what());
    } catch (...) {
      MCE_WARN("ExperimentManager::LoadExperiment Unknown Exception.");
    }
    int expsize = temp.size();
    if (expsize == 0 || (totalsize != -1 && expsize != totalsize)) {
      MCE_WARN("ExperimentManager::LoadExperiment Exception! load size: " << expsize << " total size: " << totalsize);
      return ;
    } else {
      MCE_INFO("ExperimentManager::LoadExperiment load size: " << expsize);
    }
    IceUtil::RWRecMutex::WLock lock(mutex_);
    model_.swap(temp);
  }

  double GetValue(std::string key) {
    if (!mutex_.tryReadLock()) {
      return NOT_FOUND_VALUE;
    }
    double res = NOT_FOUND_VALUE;
    ModelData::const_iterator it = model_.find(key);
    if (it != model_.end()) {
      res = it->second;
    }
    mutex_.unlock();
    return res;
  }

private:
  int mod_;
  IceUtil::RWRecMutex mutex_;
  ModelData model_;
};

}
}

#endif //_FEEDNEWS_EXPERIMENTMANAGER_H_
