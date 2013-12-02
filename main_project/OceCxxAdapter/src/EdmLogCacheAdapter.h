#ifndef __AD_EDM_LOG_CACHE_ADAPTER_H__
#define __AD_EDM_LOG_CACHE_ADAPTER_H__

#include "AdapterI.h"
#include <string>
#include "EdmLogCache.h"

namespace xce{
namespace ad{

using namespace MyUtil;
class EdmLogCacheAdapter: public MyUtil::ReplicatedClusterAdapterI<EdmLogCachePrx>,
  public MyUtil::Singleton<EdmLogCacheAdapter>{
public:
  EdmLogCacheAdapter() : MyUtil::ReplicatedClusterAdapterI<EdmLogCachePrx>("ControllerEdmLogCache",120,300,new EdmChannel) ,server_index_(0) {
    srand(time(NULL));
  }
  
  void Add(const std::string& key, const std::string& value);
  void Expose(const std::string& key, Ice::Int index);
  void Click(const std::string& key);
protected:
  int GetClusterIndex(const std::string& key);

  virtual string name(){
    return "ADEDM";
  }
  virtual string endpoints(){
    return "@EdmLogCache";
  }
  virtual size_t cluster(){
    return 1;
  }
  EdmLogCachePrx getManagerOneway(int server_index);
  EdmLogCachePrx getManager(int server_index);
private:
  int server_index_;
  vector<EdmLogCachePrx> oneway_proxy_;
  vector<EdmLogCachePrx> twoway_proxy_;
};

}
}
#endif
