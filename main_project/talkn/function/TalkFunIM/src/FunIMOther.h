#ifndef __FUN_IM_OTHER__
#define __FUN_IM_OTHER__

#include "FunIMCommon.h"
#include "Singleton.h"

namespace com {
namespace xiaonei {
namespace talk {
namespace function {
namespace im {
namespace other {

using namespace com::xiaonei::talk::function::im;
using namespace MyUtil;

class FunIMOther : public Singleton<FunIMOther> {
public:
  void ProcessPingRequest(const Str2StrMap& paras);
};

};
};
};
};
};
};

#endif
