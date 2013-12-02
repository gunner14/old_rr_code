/*
 * FeedQueueI.h
 *
 *  Created on: Feb 17, 2011
 *      Author: yejingwei
 */

#ifndef FeedLRUI_H_
#define FeedLRUI_H_

#include "SearchFriendCache.h"
#include "FeedDemo.h"
#include "Singleton.h"
#include <IceUtil/IceUtil.h>

#include "ObjectCacheI.h"
using namespace std;
using namespace MyUtil;

namespace com {
namespace renren {
namespace search {
namespace friendcache{

class UserInfo: public Ice::Object {
public:
	int uid_;
	vector<int> secondfriends_;
	bool loaded_;
	time_t timestamp_;
	IceUtil::Mutex mutex_;
	int GetUid() const { return uid_; }
	UserInfo(int uid) : uid_(uid),loaded_(false),timestamp_(time(0)){}
	bool IsExpired(){
		const static time_t live = 60 * 24 ;//one day
		time_t now = time(0)	;
		return now - timestamp_ > live;
	}
	vector<int> Load();
	//const vector<int> & GetSecondFriends() const{ 
	//	IceUtil::Mutex::Lock lock(mutex_);
	//	return secondfriends_;
	//}
	string ToString(){
		IceUtil::Mutex::Lock lock(mutex_);
		ostringstream oss;
		oss << " uid:"	<< uid_ << " time:" << timestamp_ << " secondfriends:" << secondfriends_.size();
		return oss.str();
	}

};
typedef IceUtil::Handle<UserInfo> UserInfoPtr;

class SearchFriendCacheI: public SearchFriendCache, public MyUtil::Singleton<SearchFriendCacheI> {
public:
	virtual map<int,int> GetSecond(int uid,const Ice::Current& = Ice::Current());
	virtual vector<int> GetSecondFriends(int uid,const Ice::Current& = Ice::Current());
	UserInfoPtr Load(int uid);
};

}
}
}
}
#endif /* FEEDQUEUEI_H_ */
