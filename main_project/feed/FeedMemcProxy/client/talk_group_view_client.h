#ifndef _TALK_GROUP_VIEW_CLIENT_H_
#define _TALK_GROUP_VIEW_CLIENT_H_

#include "Singleton.h"
#include "MemcachedClientPool.h"
#include "TalkCommon.h"
#include "XmppTools.h"
#include "TalkGroupViewLoader.h"
#include "TalkGroupView.pb.h"
namespace xce{
namespace talk {
namespace recent{
using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::talk::common;
using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk::groupview::loader;
using namespace com::xiaonei::talk::groupview;

class TimeStat{
  timeval _begin, _end;
  public:
  TimeStat(){
    reset();
  }
  void reset(){
    gettimeofday(&_begin, NULL);
  }
  float getTime(){
    gettimeofday(&_end, NULL);
    float timeuse=1000000*(_end.tv_sec-_begin.tv_sec)+_end.tv_usec
      -_begin.tv_usec;
    timeuse/=1000;
    return timeuse;
  }
};


class TalkGroupViewClient: public Singleton<TalkGroupViewClient> {
public:
	enum{
		SOLID_GROUP = 0,
		DISCUSS_GROUP = 1,
		CONVERSATION_GROUP = 2,
	};

	~TalkGroupViewClient();
	TalkGroupViewClient();

  bool GetAllGroups(int uid, TalkGroupViewSeq &groupviews);
	//混合所有的组给出最近联系的列表;
  bool GetRecentGroup(int uid, TalkGroupViewSeq &groupviews);
	bool GetRecentGroups(const MyUtil::IntSeq& ids, TalkGroupViewSeqMap &groupviews);

  bool GetRecentGroupByIdWithLoad(int uid, TalkGroupViewSeq &groupviews);
	bool GetRecentGroupsBySeqWithLoad(int actor, const MyUtil::IntSeq& ids, TalkGroupViewSeqMap &groupviews, bool is_get_all_groups);

	
	//按类型取出最近联系列表
  bool GetRecentGroupByType(int uid, int type, TalkGroupViewSeq &groupviews);
  bool GetRecentGroupsByType(const MyUtil::IntSeq&ids, int type, TalkGroupViewSeqMap &groupviews);


  bool GetRecentGroupTypeByIdWithLoad(int userid, int type, TalkGroupViewSeq &groupviews);
  bool GetRecentGroupsTypeBySeqWithLoad(int actor, int type, const MyUtil::IntSeq& ids, TalkGroupViewSeqMap &groupviews);

	void SetRecentGroup(int userid, const TalkGroupViewSeq& seq, int type);

protected:
	bool SerializeTalkGroupViewSeq(McGroups& mc_groups, const TalkGroupViewSeq& aseq,int type, string* serialized);
private:
	PoolManager pool_;
};

}
}
}
#endif // _TALK_RECENT_GROUP_CLIENT_H_ 
