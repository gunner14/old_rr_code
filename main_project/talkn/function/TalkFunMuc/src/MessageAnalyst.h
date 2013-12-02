#ifndef MESSAGEANALYST_H_
#define MESSAGEANALYST_H_
#include "Singleton.h"
#include "Util.h"

namespace com{
namespace xiaonei{
namespace talk{
namespace function{
namespace muc{
class MessageAnalyst: public MyUtil::Singleton<MessageAnalyst> {
public:
  void handle(const MyUtil::Str2StrMap &paras);
};

};
};
};
};
};
#endif
