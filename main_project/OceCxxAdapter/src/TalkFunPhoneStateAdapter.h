#ifndef TALKFUN_PHONE_STATEADAPTER_H_
#define TALKFUN_PHONE_STATEADAPTER_H_

#include "AdapterI.h"
#include "TalkFunPhoneState.h"

#include "Singleton.h"

namespace com {
namespace xiaonei {
namespace talk {
namespace fun{
namespace state{
using namespace MyUtil;


class TalkFunPhoneStateAdapter : public MyUtil::ReplicatedClusterAdapterI<TalkFunPhoneStateManagerPrx>, public MyUtil::Singleton<TalkFunPhoneStateAdapter> {
public:
  TalkFunPhoneStateAdapter() : MyUtil::ReplicatedClusterAdapterI<TalkFunPhoneStateManagerPrx> ("ControllerTalkFunPhoneState",120,300,new XceFeedControllerChannel) {

	}




  void HeartBeatPhone(const MyUtil::IntSeq& uids); 
  void Offline(int uid);
  void Online(int uid);

  void getUsersIds(const MyUtil::IntSeq& uids, MyUtil::IntSeq &res);

 private:
  void getProxySeqForSet(int id, const char *fun, vector<TalkFunPhoneStateManagerPrx> &prxs);

};

}
;
}
;
}
;
};
};

#endif
