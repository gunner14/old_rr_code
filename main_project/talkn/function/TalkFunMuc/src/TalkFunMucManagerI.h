#ifndef __TALK_FUN_MUC_MANAGERI__
#define __TALK_FUN_MUC_MANAGERI__


#include "ServiceI.h"
#include "TalkFunction.h"
namespace com{
namespace xiaonei{
namespace talk{
namespace function{
namespace muc{

using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::talk::common;
class TalkFunMucManagerI: public TalkFunMucManager, public Singleton<TalkFunMucManagerI>{
public:
	virtual void notify(const Str2StrMap& paras, const Ice::Current& = Ice::Current());
};	

};
};
};
};
};

#endif

