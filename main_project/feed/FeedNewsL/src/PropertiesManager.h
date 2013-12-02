/*
 * =====================================================================================
 *
 *       Filename:  PropertiesManager.h
 *
 *    Description:  负责通知其他类配置更新
 *
 *        Version:  1.0
 *        Created:  2012年09月20日 17时48分03秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Duo Zhang (), duo.zhang@renren-inc.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _PROPERTIES_MANAGER_H_
#define _PROPERTIES_MANAGER_H_
#include "ServiceI.h"

namespace xce {
namespace feed {

class UpdaterBase {
public:
  virtual void Update() = 0;
  virtual ~UpdaterBase() {
  }
};

class Properties_Descriptor : public MyUtil::Descriptor, public MyUtil::Singleton<Properties_Descriptor> {
protected:
  virtual void configure(const Ice::PropertiesPtr& props);
};

class PropertiesManager : public MyUtil::Singleton<PropertiesManager> {
public:
  void Init() {
    MyUtil::ServiceI::instance().registerXceObserver(&Properties_Descriptor::instance());
  }

  void AppendUpdater(UpdaterBase * up) {
    MCE_INFO("PropertiesManager::AppendUpdater >> " << typeid(*up).name());
    updaters_.push_back(up);
  }

  void Update() {
    for(std::vector<UpdaterBase* >::iterator it = updaters_.begin(); it != updaters_.end(); ++it) {
      (*it)->Update();
    }
  }

private:
  std::vector<UpdaterBase* > updaters_;
};

inline void Properties_Descriptor::configure(const Ice::PropertiesPtr& props) {
  PropertiesManager::instance().Update();
}

}
}
#endif //_PROPERTIES_MANAGER_H_
