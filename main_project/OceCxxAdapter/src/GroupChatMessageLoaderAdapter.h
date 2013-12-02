#ifndef GROUPCHATMESSAGEAPTER_H__
#define GROUPCHATMESSAGEAPTER_H__

#include "Singleton.h"
#include "AdapterI.h"
#include "GroupChatMessageLoader.h"
namespace xce {
namespace xntalk {
namespace groupchat{

class GroupChatMessageLoaderAdapter:
  public MyUtil::ReplicatedClusterAdapterI<MessageLoaderManagerPrx>, public MyUtil::Singleton<GroupChatMessageLoaderAdapter>  {
public:
  GroupChatMessageLoaderAdapter() :
    MyUtil::ReplicatedClusterAdapterI<MessageLoaderManagerPrx> ("ControllerGroupChatMessageLoader",120,300,new MyUtil::XceFeedControllerChannel) {
  }
  void LoadMessages(Ice::Long sessionId, const MessageRequestPtr &request, MessageBodySeq &ans, const bool is_offline); 
  bool SetMessages(const MessageBodySeq &msg_seq);
  void SetMessagesAsyn(const MessageBodySeq &msg_seq);

	MessageLoaderManagerPrx getManager(int id);
	MessageLoaderManagerPrx getManagerOneway(int id);
private:

};

};
};
};


#endif
