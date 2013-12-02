#ifndef TALKGROUPMEMBERLOADER_H_
#define TALKGROUPMEMBERLOADER_H_ 

#include "AdapterI.h"
#include "TalkGroupMemberLoader.h"
#include "Singleton.h"

namespace com {
namespace xiaonei {
namespace talk {
namespace group {
namespace memberloader {
using namespace MyUtil;
using namespace com::xiaonei::talk::common;
class TalkGroupMemberLoaderAdapter : public MyUtil::ReplicatedClusterAdapterI<TalkGroupMemberLoaderManagerPrx>, public MyUtil::Singleton<TalkGroupMemberLoaderAdapter> {

  public:

 TalkGroupMemberLoaderAdapter() : MyUtil::ReplicatedClusterAdapterI<TalkGroupMemberLoaderManagerPrx> ("ControllerTalkGroupMemberLoader",120,300,new XceFeedControllerChannel) {
  }
  // 同步接口
  void LoadGroupMember(const MyUtil::LongSeq &idSeq, GroupMemberDict &ans);
  bool AddNewGroup(const GroupMemberDict& memberdict);
  bool DeleteGroup(const MyUtil::LongSeq &idSeq);
  bool AddNewMember(const long sid, const GroupMemberSeq &members);
  bool RemoveMember(const long sid, const MyUtil::IntSeq &idSeq);
  bool SetMember(const GroupMemberDict& memberdict);
  // 异步接口
  void AddNewGroupAsyn(const GroupMemberDict& memberdict);
  void DeleteGroupAsyn(const MyUtil::LongSeq &idSeq);
  void AddNewMemberAsyn(const long sid, const GroupMemberSeq &members);
  void RemoveMemberAsyn(const long sid, const MyUtil::IntSeq &idSeq);
  void SetMemberAsyn(const GroupMemberDict& memberdict);


 protected:

  TalkGroupMemberLoaderManagerPrx getManagerOneway(long id);
  TalkGroupMemberLoaderManagerPrx getManager(long id);

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
