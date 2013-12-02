#ifndef _MY_TASKS_H_
#define _MY_TASKS_H_

#include "TaskManager.h"
#include "HotFeedRecommendCacheAdapter.h"
#include "NegtiveListCacheAdapter.h"
#include "socialgraph/socialgraphutil/clogging.h" // for clog

namespace tasks {

static const int TASK_LEVEL_SET_CACHE = 5;

//********************************************************************************************
class AppendSingleRelationAndSetCacheTask : public MyUtil::Task {
public:
    //将negtiveFriendList添加入Cache
	AppendSingleRelationAndSetCacheTask(int ownerId, const std::vector<int>& negtiveFriendList, 
		::Ice::Int level = TASK_LEVEL_SET_CACHE):
		MyUtil::Task(level), _ownerId(ownerId), _negtiveFriendList(negtiveFriendList) {
	}

	virtual void handle();

private:
	int _ownerId;
	std::vector<int> _negtiveFriendList;
};

}

#endif
