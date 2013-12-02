#ifndef __INDEXSERVICE_ADAPTER_H__
#define __INDEXSERVICE_ADAPTER_H__

#include "AdapterI.h"
#include "IndexService.h"
#include "Singleton.h"

namespace xce{
namespace ad{
  using namespace MyUtil;
  class IndexServiceReplicaAdapter : public MyUtil::ReplicatedClusterAdapterI<IndexServicePrx>,
  public MyUtil::Singleton<IndexServiceReplicaAdapter>{
  public:
    IndexServiceReplicaAdapter() : MyUtil::ReplicatedClusterAdapterI<IndexServicePrx>("ControllerIndexService",120,150,new EdmChannel) ,server_index_(0) {
    }
	int trigger(const TriggerInputInfo &input, GroupDict &groups, bool env_test = false);
  void Close(int uid, long creativeid, bool env_test = false);
	int GetServerIndex(int uid, bool env_test);
  protected:
    virtual string name(){
      return "IdxSrv";
    }
    virtual string endpoints(){
      return "@IndexService";
    }
    IndexServicePrx getManagerOneway(int server_index);
    IndexServicePrx getManager(int server_index);
  private:
    int server_index_;
    vector<IndexServicePrx> oneway_proxy_;
    vector<IndexServicePrx> twoway_proxy_;
  };
};
};

#endif

