#ifndef TALKGROUPLOADERADAPTER_H_
#define TALKGROUPLOADERADAPTER_H_

#include "AdapterI.h"
#include "TalkGroupLoader.h"

#include "Singleton.h"

namespace com {
namespace xiaonei {
namespace talk {
namespace group {
namespace loader {


using namespace MyUtil;
using namespace com::xiaonei::talk::common;

class TalkGroupLoaderAdapter : public MyUtil::ReplicatedClusterAdapterI<TalkGroupLoaderManagerPrx>, public MyUtil::Singleton<TalkGroupLoaderAdapter> {

 public:

 TalkGroupLoaderAdapter() : MyUtil::ReplicatedClusterAdapterI<TalkGroupLoaderManagerPrx> ("ControllerTalkGroupLoader",120,300,new MyUtil::XceFeedControllerChannel()) {
  }

  void LoadGroupInfo(const MyUtil::LongSeq &idSeq, GroupInfoDict &infodict);
  //void GetCreatSessions(int creater, MyUtil::LongSeq &idseq);

  bool SetGroupInfo(const GroupInfoPtr &groupInfo);
  bool AddGroupInfo(const GroupInfoPtr &groupInfo);
  bool DeleteGroupInfo(const MyUtil::LongSeq& idSeq);
  bool UpdateGroupInfo(long sid, const MyUtil::Str2StrMap &props);


  void SetGroupInfoAsyn(const GroupInfoPtr &groupInfo);
  void AddGroupInfoAsyn(const GroupInfoPtr &groupInfo);
  void DeleteGroupInfoAsyn(const MyUtil::LongSeq& idSeq);
  void UpdateGroupInfoAsyn(long sid, const MyUtil::Str2StrMap &props);

 protected:
  TalkGroupLoaderManagerPrx getManagerOneway(long id);
  TalkGroupLoaderManagerPrx getManager(long id);
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
