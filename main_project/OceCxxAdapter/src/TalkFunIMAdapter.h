#ifndef __TALK_FUN_IM_ADAPTER__
#define __TALK_FUN_IM_ADAPTER__

#include "Singleton.h"
#include "AdapterI.h"
#include "TalkFunction.h"
#include "TalkLogic.h"

namespace talk {
namespace adapter {

using namespace MyUtil;
using namespace com::xiaonei::talk::function;
using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::common;

class TalkFunIMAdapter : public ReplicatedClusterAdapterI<TalkFunIMManagerPrx>, 
  public Singleton<TalkFunIMAdapter> {
public:
  TalkFunIMAdapter() : ReplicatedClusterAdapterI<TalkFunIMManagerPrx>("ControllerTalkFunIM", 
    120, 300, new XceFeedControllerChannel) {}

  void presenceSubscribe(const string& id, int from, int to, const string& status);
  void presenceSubscribeWithPostscript(const string& id, int from, int to, 
    const string& status, const string& postscript);
  void presenceSubscribed(const string& id, int from, int to);
  void presenceUnsubscribed(const string& id, int from, int to);
  void webPresence(int userId);
  void presence(const JidPtr& jid, const string& show, const string& ver, const string& subver, const string& ext);
  void presenceWithSecver(const JidPtr& jid, const string& show, const string& ver, const string& subver, 
    const string& ext, const string& secver);
  void offline(const JidPtr& jid);
  void userStatusChanged(int userId, int userStatus); 

  void OnlineCenterPresence(const JidPtr& jid, com::xiaonei::talk::common::OnlineStat stat, 
    const string& ver, const string& ext);
private:
  TalkFunIMManagerPrx getManager(int id);
  TalkFunIMManagerPrx getManagerOneway(int id);
};

};
};

#endif
