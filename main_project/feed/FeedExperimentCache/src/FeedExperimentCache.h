#ifndef FEED_EXPERIMENT_CACHE_H_
#define FEED_EXPERIMENT_CACHE_H_

#include <Ice/Ice.h>
#include <ServiceI.h>
#include "Singleton.h"
#include "FeedExperiment.h"
#include "BayesModelLoader.h"

namespace xce{
namespace feed{

class FeedExperimentCache : public FeedExperimentManager, public MyUtil::Singleton<FeedExperimentCache> {
  
public:
  void Initialize();

  virtual BayesModelList GetBayesModels(int mod, int begin, int size, const ::Ice::Current& = ::Ice::Current()); 
  void UpdateMod2Version(int version, MyUtil::IntSeq mods);

private:
  std::map<int, BayesModelLoaderPtr> bayes_model_loaders_;
  std::map<int, int> mod2version_;
  IceUtil::RWRecMutex mutex_;

};//end of class FeedExperimentCache

} //end of namespace feed
} //end of namespace xce

#endif
