#ifndef _XCE_TALK_MESSAGEBODY_CACHE_CLIENT_H_
#define _XCE_TALK_MESSAGEBODY_CACHE_CLIENT_H_
#include <Ice/Ice.h>
#include "Singleton.h"
#include "MemcachedClientPool.h"
#include "GroupChatMessageLoader.h"

namespace xce {
namespace xntalk {
namespace groupchat {

const int MESSAGES_MEM  = 40;

class GroupChatMessageCacheClient : public MyUtil::Singleton<GroupChatMessageCacheClient> {
public:
  GroupChatMessageCacheClient();
  ~GroupChatMessageCacheClient();

  xce::xntalk::groupchat::MessageBodySeqMap GetMessages(xce::xntalk::groupchat::MessageRequestMap &requests, int load_db=1);
  xce::xntalk::groupchat::MessageBodySeq GetMessage(const Ice::Long session_id, const MessageRequestPtr &request);
  xce::xntalk::groupchat::MessageBodySeq GetOfflineMessage(const Ice::Long session_id,  const MessageRequestPtr &request);
  xce::xntalk::groupchat::MessageBodySeqMap GetOfflineMessages(xce::xntalk::groupchat::MessageRequestMap &requests);

  //MessageBodySeqMap GetMessagesWithLoad(xce::xntalk::groupchat::MessageRequestMap &requests);

  bool SetMessages(const xce::xntalk::groupchat::MessageBodySeq &msg_seq);
private:
  bool SerializeMessageBody(const xce::xntalk::groupchat::MessageBodySeq &msgbody_seq, std::string &value);
  PoolManager pool_;
};
};
};
};



#endif
