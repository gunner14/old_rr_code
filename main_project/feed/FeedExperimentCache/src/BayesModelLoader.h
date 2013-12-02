/*
 * BayesModelLoader.h
 *
 * Author: rui.qiu
 */

#ifndef BAYES_MODEL_LOADER_H
#define BAYES_MODEL_LOADER_H

#include <string>
#include <sstream>
#include <query.h>
#include <utility>
#include <connection.h>
#include <Ice/Ice.h>
#include <IceUtil/Mutex.h>
#include "FeedExperiment.h"
#include "Singleton.h"
#include "IceLogger.h"
#include "ServiceI.h"
#include "TaskManager.h"
#include "Util.h"

namespace xce{
namespace feed{

using namespace MyUtil;

class BayesModelLoader :  public IceUtil::Shared {

public:
  BayesModelLoader(int version) : version_(version) {
    Initialize();
  }

  void Initialize() {
    loading_ = false;
    TaskManager::instance().execute(new BayesModelLoaderTask(this, version_));
  }

  int interval() {
    return interval_;
  }

  int version() {
    return version_;
  }

  MyUtil::IntSeq target_news() {
    return target_news_;
  }

  bool LoadModel();
  void GetBayesModels(int start, int size, BayesModelList& res);

  int GetModelSize() {
    return model_list_.size();
  }

private:
  bool HasNewFile(std::string& new_file_name, std::string& new_md5_value);
  bool IsValidFile(const std::string& file_name, const std::string& md5_value);
  void ComputeMd5(const std::string& file_name, char* md5_char);

private:
  std::string file_name_;
  std::string md5_value_;
  int interval_;
  bool loading_;
  int version_;
  MyUtil::IntSeq target_news_;
  BayesModelList model_list_;
  IceUtil::RWRecMutex mutex_;

  class BayesModelLoaderTask : public MyUtil::Task {
  public:
    BayesModelLoaderTask(BayesModelLoader* model_loader, int task_level) : MyUtil::Task(task_level) {
      model_loader_ = model_loader;
    }

    virtual void handle();

  private:
    BayesModelLoader* model_loader_;
  };//end of class BayesModelLoaderTask
};//end of class BayesModelLoader

typedef IceUtil::Handle<BayesModelLoader> BayesModelLoaderPtr;

}//end of namespace feed
}//end of namespace xce

#endif

