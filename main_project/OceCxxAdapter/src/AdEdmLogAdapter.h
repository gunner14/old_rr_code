#ifndef __EDMLOG_ADAPTER_H__
#define __EDMLOG_ADAPTER_H__

#include "AdapterI.h"
#include "EdmLog.h"

namespace xce{
namespace ad{
  using namespace MyUtil;
  class AdEdmLogAdapter : public AdapterI, public AdapterISingleton<MyUtil::EdmChannel, AdEdmLogAdapter>{
  //class AdLogAdapter : public MyUtil::ReplicatedClusterAdapterI<AdLogManagerPrx>,
  //public MyUtil::Singleton<AdLogAdapter>{
  public:
    AdEdmLogAdapter() : server_index_(0) {
    }
    /*AdLogAdapter() : MyUtil::ReplicatedClusterAdapterI<AdLogManagerPrx>("ControllerAdLog",120,300,new AdChannel) ,server_index_(0) {
    }*/
    void Send(const string& log);
    //void PvBySeq(const MyUtil::StrSeq & logs);
    void Click(const string& log);
    void Expose(const string& log);
   /* int getCluster(){
      return _cluster;
    }*/
  protected:
    virtual string name(){
      return "AdEdmLog";
    }
    virtual string endpoints(){
      return "@AdEdmLog";
    }
    virtual size_t getCluster(){
      return 1;
    }
    virtual size_t cluster(){
      return 1;
    }
    EdmLogManagerPrx getManagerOneway(int server_index);
    EdmLogManagerPrx getManager(int server_index);
    EdmLogManagerPrx getManagerDatagram(int server_index);
  private:
    int server_index_;
    vector<EdmLogManagerPrx> oneway_proxy_;
    vector<EdmLogManagerPrx> twoway_proxy_;
  };
};
};
#endif
