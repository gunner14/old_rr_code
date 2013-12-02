#ifndef _NOTIFY_GATE_ADAPTER_H_
#define _NOTIFY_GATE_ADAPTER_H_

#include "AdapterI.h"
#include "Notify.h"
#include "Singleton.h"
#include "AdapterI.h"
#include "TaskManager.h"

namespace xce {
namespace notify {

using namespace MyUtil;

class NotifyGateAdapter : public AdapterI,
    public AdapterISingleton<MyUtil::NotifyChannel, NotifyGateAdapter> {
 protected:
  virtual string name() {
    return "NotifyGate";
  }
  virtual string endpoints() {
    return "@NotifyGate";
  }
  virtual size_t cluster() {
    return 10;
  }
  
 public:
  NotifyGateAdapter() {
    _managers.resize(cluster() > 0 ? cluster() : 1);
    _managersOneway.resize(cluster() > 0 ? cluster() : 1);
  }
  bool reloadSchema(){
    // all of gate reload schema
    for(size_t i = 0; i < cluster(); ++i) {
      getManager(i)->reloadSchema();
    }
    return true;
  }
  StrSeq getNotifySeq2(int uid, int view, long lastNotifyId, int limit);

  void instantNotify(const NotifyContentPtr & content, const IntSeq & idSeq) {
    if(idSeq.empty())
      return;
    getManagerOneway(idSeq[0])->instantNotify(content, idSeq);
  }
  void instantNotify(const NotifyContentPtr& content, const set<Ice::Int>& idSet) {
    if(idSet.empty()) {
      return;
    }
    IntSeq idSeq(idSet.begin(),idSet.end());
    getManagerOneway(idSeq[0])->instantNotify(content, idSeq);
  }
  void deliverWebpager(const NotifyContentPtr& content, const IntSeq& idSeq) {
    if(idSeq.empty()) {
      return;
    }
    getManagerOneway(idSeq[0])->deliverWebpager(content, idSeq);
  }
  void deliverWebpager(const NotifyContentPtr& content, 
      const set<Ice::Int>& idSet) {
    if(idSet.empty()) {
      return;
    }
    IntSeq idSeq(idSet.begin(),idSet.end());
    getManagerOneway(idSeq[0])->deliverWebpager(content, idSeq);
  }
  void RemoveLogicBySource(int uid, int type, Ice::Long source) {
    getManagerOneway(uid)->RemoveLogicSource(uid, type, source);
  }
	int getUndealCountByBigType(int uid, int bigType){
		getManager(uid)->getUndealCountByBigType(uid,bigType);
	}
  //add v6 interface for IM
  StrSeq getRMsgByBigtype(int uid, int view, 
    int bigtype, int begin, int limit); 
  StrSeq getRMsgByType(int uid, int view, 
    int type, int begin, int limit); 
 private:
  NotifyGatePrx getManager(int id);
  NotifyGatePrx getManagerOneway(int id) {
    return locate<NotifyGatePrx> (_managersOneway, "M", id, ONE_WAY);
  }
  vector<NotifyGatePrx> _managersOneway;
  vector<NotifyGatePrx> _managers;
};

}
}

#endif // _NOTIFY_GATE_ADAPTER_H_ 

