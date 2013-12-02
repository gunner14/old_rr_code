#ifndef __TALK_GROUP_VIEW_LOADER_ADAPTER_H__
#define __TALK_GROUP_VIEW_LOADER_ADAPTER_H__

#include "AdapterI.h"
#include "TalkGroupViewLoader.h"
#include "Singleton.h"

namespace com {
namespace xiaonei {
namespace talk {

using namespace MyUtil;
using namespace com::xiaonei::talk::common;
using namespace com::xiaonei::talk::groupview::loader;

class TalkGroupViewLoaderAdapter :
 public MyUtil::ReplicatedClusterAdapterI<GroupViewLoaderManagerPrx>, public MyUtil::Singleton<TalkGroupViewLoaderAdapter>  {
public:

 TalkGroupViewLoaderAdapter():
    MyUtil::ReplicatedClusterAdapterI<GroupViewLoaderManagerPrx> ("ControllerTalkGroupViewLoader",120,300,new MyUtil::XceFeedControllerChannel) {
	}


  bool Update(int actor, const TalkGroupViewPtr &groupinfo);
  bool Delete(int userid, long sessionid, int type);
  bool UpdateItem(int userid, long sid, const Str2StrMap &props);
  bool UpdateMsgid(int uid, long sessionid, int type, long msgid);

  void UpdateAsyn(int actor, const TalkGroupViewPtr &groupinfo);
  void DeleteAsyn(int userid, long sessionid, int type);
  void UpdateItemAsyn(int userid, long sid, const Str2StrMap &props);

  //  void LoadById(int owner, int type, TalkGroupViewSeq &groupviews);
  void LoadBySeq(const MyUtil::IntSeq &owners, int type, TalkGroupViewSeqMap &groupviewsdict);
  void GetCreatSessions(int creater, MyUtil::LongSeq &idseq);

protected:
	GroupViewLoaderManagerPrx getManagerOneway(int id);
	GroupViewLoaderManagerPrx getManager(int id);

};

}
;
}
;
}
;

#endif
