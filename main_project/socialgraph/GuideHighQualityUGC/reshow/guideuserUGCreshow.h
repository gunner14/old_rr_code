#ifndef _GUIDE_USER_UGC_RESHOW_H_
#define _GUIDE_USER_UGC_RESHOW_H_

//#include "UtilCxx/src/TaskManager.h"
#include "UtilCxx/src/Singleton.h"
#include "GuideUserHighQualityUGC.h"

namespace xce {
namespace socialgraph {

class GuideUserUGCReshow: virtual public GuideUserHighQualityUGC,
        virtual public MyUtil::Singleton<GuideUserUGCReshow> {
 public:
	GuideUserUGCReshow() { 
	   // MyUtil::TaskManager::instance().config(TASK_LEVEL_FEED_PUSH, MyUtil::ThreadPoolConfig(0, 100));
        }
	
	virtual void NotifyUserLogin(int, const Ice::Current&);

 private:
        bool  IsGuideUser(int userid);

	friend class MyUtil::Singleton<GuideUserUGCReshow>;
};

}}

#endif
