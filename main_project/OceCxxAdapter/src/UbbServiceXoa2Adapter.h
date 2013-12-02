#ifndef UbbServiceXoa2Adapter_h_
#define UbbServiceXoa2Adapter_h_

#include "gen-cpp/UbbService.h"
#include "Xoa2AdapterI.h"

namespace com {
namespace renren {
namespace xoa2 {
namespace ugc {
namespace status {

class UbbServiceAdapter:public MyUtil::Xoa2AdapterI<UbbServiceClient>,
                        virtual public MyUtil::Singleton<UbbServiceAdapter> {
public:
  UbbServiceAdapter() : MyUtil::Xoa2AdapterI<UbbServiceClient>("status_ubb.ugc.xoa.renren.com", 200) 
                          {
                          }
  void getDoingUbbsBytypeNE(Xoa2UbbListResult& result,int32_t type);
};
};
};
};
};
};




























#endif
