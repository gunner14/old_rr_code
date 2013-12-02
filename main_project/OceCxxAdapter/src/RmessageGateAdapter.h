#ifndef _RMESSAGEGATE_ADAPTER_H_
#define _RMESSAGEGATW_ADAPTER_H_

#include "AdapterI.h"
#include "Notify.h"
#include "RmessageGate.h"
#include "Singleton.h"
#include "AdapterI.h"
#include "TaskManager.h"


namespace xce {
namespace notify {


using namespace MyUtil;

class RmessageGateAdapter : public AdapterI,
 public AdapterISingleton<MyUtil::NotifyChannel, RmessageGateAdapter> {
 protected:
  virtual string name() {
    return "RmessageGate";
  }
  virtual string endpoints() {
    return "@RmessageGate";
  }
  virtual size_t cluster() {
    return 1;
  }
 public:
  RmessageGateAdapter() {
    _managers.resize(cluster() > 0 ? cluster() : 1);
    _managersOneway.resize(cluster() > 0 ? cluster() : 1);
  }
  bool reloadSchema() {
    // all of gate reload schema
    try {
      for(size_t i = 0; i < cluster(); ++i) {
        getManager(i)->reloadSchema();
      }
    } catch(Ice::Exception &e) {
      MCE_WARN("RmessageGateAdapter::reloadSchema Exception:" << e);
      return false;
    }
    return true;
  }
 private:
  RmessageGatePrx getManager(int id);
  RmessageGatePrx getManagerOneway(int id) {
    return locate<RmessageGatePrx> (_managersOneway, "M", id, ONE_WAY);
  }
  vector<RmessageGatePrx> _managersOneway;
  vector<RmessageGatePrx> _managers;
};

}
}

#endif // _RMESSAGEGATE_ADAPTER_H_

