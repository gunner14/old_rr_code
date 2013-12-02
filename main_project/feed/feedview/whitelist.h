/*************************************|
|***** Author:tinghaier     **********|
|***** Created on:Dec 8,2011**********|
|*************************************/
#ifndef WHITELIST_H_
#define WHITELIST_H_
#include <map>
#include <IceUtil/IceUtil.h>
#include "TaskManager.h"
#include "Singleton.h"

namespace xce {
namespace feed {

class Whitelist : public MyUtil::Singleton<Whitelist> {
public:
  Whitelist() {}
  int IsWhiteUser(int uid);
  void LoadWhitelist();
  ~Whitelist() {}
private:
  IceUtil::RWRecMutex _mutex;
  map<int, int> white_map;
};
class WhiteInitTask : public MyUtil::Task {
public:
  WhiteInitTask() {}
  virtual void handle();
};

}
}
#endif
