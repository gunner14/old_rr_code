#ifndef GROUPCHAT_MESSGAE_H
#define GROUPCHAT_MESSGAE_H
#include "ServiceI.h"
#include "Singleton.h"
#include "GroupChatMessageLoader.h"
#include "FeedMemcProxy/client/gc_message_client.h"

namespace xce {
namespace xntalk {
namespace groupchat {
const char *DB_INSTANCE = "im_groupchat";
const char *DB_ROUTE_MSG = "message_body";
const char *DB_ROUTE_ATTACH = "session_attachments";
const char *DB_PREF_MSG = "message_body";
const char *DB_PREF_ATTACH = "session_attachments";



class MessageLoaderManagerI :public xce::xntalk::groupchat::MessageLoaderManager, public MyUtil::Singleton<MessageLoaderManagerI> {
public:
  virtual MessageBodySeq LoadMessages(::Ice::Long sessionId, const ::xce::xntalk::groupchat::MessageRequestPtr& request,const bool is_offline, 
      const ::Ice::Current& = ::Ice::Current());
  virtual bool SetMessages(const  xce::xntalk::groupchat::MessageBodySeq &msg_seq,  const ::Ice::Current& = ::Ice::Current());
  virtual void SetMessagesAsyn(const  xce::xntalk::groupchat::MessageBodySeq &msg_seq,  const ::Ice::Current& = ::Ice::Current());
private:
  bool SetMessages_(const  xce::xntalk::groupchat::MessageBodySeq &msg_seq);
  bool SetMessagesOneMod(long mod, const  xce::xntalk::groupchat::MessageBodySeq &msg_seq);
  bool LocateMessage(const Ice::Long sessionId, 
      const MessageRequestPtr &request, xce::xntalk::groupchat::MessageBodySeq &msg_seq, const bool is_offline);
  std::string EscapeString(std::string &raw_string);

  enum {
    // 这里假设了msg表和attache表份数一样，散表方式一样
    TABLE_COUNT = 100,
  };

};

};
};
};



#endif
