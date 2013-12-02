#ifndef __IM_TALK_PROTO__
#define __IM_TALK_PROTO__

#include <Util.ice>
#include <TalkCommon.ice>

module com{
module xiaonei{
module talk{
module proto{

class TalkProtoManager{
	void Send(com::xiaonei::talk::common::Jid actor, string xml);
	void Receive(MyUtil::Str2StrMap paras);
	void BatchReceive(MyUtil::Str2StrMapSeq parasSeq);
  void BatchReceiveExpress(MyUtil::Str2StrMapSeq paraSeq);
  void ReceiveExpress(MyUtil::Str2StrMap paras);
};

};
};
};
};

#endif
