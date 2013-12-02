#ifndef __TALK_RECENT_LOADER_I_H__
#define __TALK_RECENT_LOADER_I_H__

#include "ServiceI.h"
#include "TalkRecentLoader.h"
#include "FeedMemcProxy/client/talk_recent_client.h"
namespace com {
namespace xiaonei {
namespace talk {
namespace recent {
namespace loader{
using namespace MyUtil;
	
const string DB_INSTANCE = "chat";

class RecentLoaderManagerI : public RecentLoaderManager, public Singleton<RecentLoaderManagerI> {
	bool LocateUser(int owner, TalkRecentUserSeq &seq);

public:
  enum {
    LEN_RECENT = 20, // 最近联系人存储的个数
  };

	virtual	void Update(int actor, const TalkRecentUserPtr &groupinfo, const Ice::Current& = Ice::Current());
	virtual	void Delete(int actor, int id, int type, const Ice::Current& = Ice::Current());
	virtual	TalkRecentUserSeq LoadById(int owner, const Ice::Current& = Ice::Current());
	virtual	TalkRecentUserSeqMap LoadBySeq(const MyUtil::IntSeq &owners, const Ice::Current& = Ice::Current());
  TalkRecentUserPtr GenInfo(int id, int type, const string &subject, const string &info);
  TalkRecentUserPtr LocateItem(int owner, int id, int type);
};

class UpdateTask: virtual public MyUtil::Task {
public:
 UpdateTask(int actor, const TalkRecentUserPtr &groupinfo) : _actor(actor), _groupinfo(groupinfo) {
  }
  virtual void handle();
private:
  int _actor;
  TalkRecentUserPtr _groupinfo;

};


class DeleteTask: virtual public MyUtil::Task {
public:
 DeleteTask(int actor, int id, int type) : _actor(actor), _id(id), _type(type) {
  }
  virtual void handle();
private:
  
  int _actor;
  int _id;
  int _type;


};



}
;
}
;
}
;
}
;
}
;

#endif

