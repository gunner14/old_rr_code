#ifndef __VALIDCODE_ADAPTER_H__
#define __VALIDCODE_ADAPTER_H__
#include "gen-cpp/ValidCodeService.h" 
#include "Xoa2AdapterI.h"

namespace com {
namespace xiaonei {
namespace icode {
namespace xoa {

class ValidCodeAdapter : public MyUtil::Xoa2AdapterI<ValidCodeServiceClient>,
                          virtual public MyUtil::Singleton<ValidCodeAdapter> {


public:
 ValidCodeAdapter() : MyUtil::Xoa2AdapterI<ValidCodeServiceClient>("icode.sec.xoa.renren.com", 200)
  {
  }


	bool validCode(const std::string &inputCode, const std::string &type, const std::string &id);

};

};
};
};
};

#endif
