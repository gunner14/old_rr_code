#ifndef SOCIAL_FEED_WORK_H_
#define SOCIAL_FEED_WORK_H_

#include "UtilCxx/src/TaskManager.h"
#include "SocialFeedRecommendationManager.h"
#include "UtilCxx/src/Singleton.h"
#include "socialgraph/newsocialfeed/socialfeedworker/builder/feed_push_task.h"

namespace xce {
namespace socialgraph {

//-----------------------------------------------------
const  int     CONST_UPDATE_UID_LIST_INTERVAL = 1*24*60*60; //very day
const  string  CONST_WHITE_UID_LIST_FILE      = "/data/xce/XSocialGraph/sg_conf/userid_file";

class SocialFeedWorker: virtual public SocialFeedRecommendationManager,
		virtual public MyUtil::Singleton<SocialFeedWorker>{
 public:
	SocialFeedWorker() { 
              IceUtil::RWRecMutex::WLock  lock(_monitor);
              if (LoadWhiteUserList()){
                  _lastUpdateUidListTimeSec = time(NULL); 
              }
	      MyUtil::TaskManager::instance().config(TASK_LEVEL_FEED_PUSH, MyUtil::ThreadPoolConfig(0, 100));
        }
	
	virtual void NotifySocialFeedUserLogin(int, const Ice::Current&);

 private:
        bool   LoadWhiteUserList();

        IceUtil::RWRecMutex  _monitor; 
        time_t               _lastUpdateUidListTimeSec; 
        std::map<int, bool>  _userMap; 

	friend class MyUtil::Singleton<SocialFeedWorker>;
};

}}

#endif
