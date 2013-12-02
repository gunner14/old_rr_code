#ifndef __ADLOG_ADAPTER_H__
#define __ADLOG_ADAPTER_H__

#include "AdapterI.h"
#include "AdLog.h"

namespace xce{
namespace ad{
  using namespace MyUtil;
  class AdLogAdapter : public AdapterI, public AdapterISingleton<MyUtil::AdChannel, AdLogAdapter>{
  public:
    AdLogAdapter() : server_index_(0) {
    }
    void Pv(const string& log);
    void PvBySeq(const MyUtil::StrSeq & logs);
    void ClickForBp(const string& log);
    void ClickForCharge(const string& log);
    void PvForCharge(const ::MyUtil::StrSeq& logs);
    int getCluster(){
      return 4;
    }
  protected:
    virtual string name(){
      return "AdLog";
    }
    virtual string endpoints(){
      return "@AdLog";
    }
    virtual size_t cluster(){
      return 4;
    }
    AdLogManagerPrx getManagerOneway(int server_index);
    AdLogManagerPrx getManager(int server_index);
  private:
    int server_index_;
    vector<AdLogManagerPrx> oneway_proxy_;
    vector<AdLogManagerPrx> twoway_proxy_;
  };
};
};
#endif
