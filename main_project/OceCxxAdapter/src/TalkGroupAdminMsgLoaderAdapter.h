#ifndef _TALKGROUPADMIN_MSG_LOADER_H_
#define _TALKGROUPADMIN_MSG_LOADER_H_

#include "AdapterI.h"
#include "TalkGroupAdminMsgLoader.h"
#include "Singleton.h"

namespace com {
namespace xiaonei {
namespace talk {
namespace group {
namespace admmsgloader {
using namespace MyUtil;
using namespace com::xiaonei::talk::common;
class TalkGroupAdminMsgLoaderAdapter : public MyUtil::ReplicatedClusterAdapterI<TalkGroupAdminMsgLoaderPrx>, public MyUtil::Singleton<TalkGroupAdminMsgLoaderAdapter> {

  public:

 TalkGroupAdminMsgLoaderAdapter() : MyUtil::ReplicatedClusterAdapterI<TalkGroupAdminMsgLoaderPrx> ("ControllerTalkGroupAdminMsgLoader",120,300,new XceFeedControllerChannel) {
  }
  MyUtil::StrSeq getMsg(int uid);
  MyUtil::StrSeq getdelMsg(int uid);
  bool saveMsg(int uid, const string &msg);
  void saveMsgAsyn(int uid, const string &msg);
  void delMsgAsyn(int uid);
  bool delMsg(int uid);

 protected:

  TalkGroupAdminMsgLoaderPrx getManagerOneway(int id);
  TalkGroupAdminMsgLoaderPrx getManager(int id);

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
