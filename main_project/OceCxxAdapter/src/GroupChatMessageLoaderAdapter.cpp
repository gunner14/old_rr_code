#include "GroupChatMessageLoaderAdapter.h"
using namespace MyUtil;
using namespace xce::xntalk::groupchat;
MessageLoaderManagerPrx GroupChatMessageLoaderAdapter::getManager(int id) {
	return getProxy(id);
}

MessageLoaderManagerPrx GroupChatMessageLoaderAdapter::getManagerOneway(int id) {
  return getProxyOneway(id);
}

void GroupChatMessageLoaderAdapter::LoadMessages(Ice::Long sessionId, const MessageRequestPtr &request, MessageBodySeq &ans, const bool is_offline) {
  ans = getManager(sessionId)->LoadMessages(sessionId, request, is_offline);
}

void GroupChatMessageLoaderAdapter::SetMessagesAsyn(const MessageBodySeq &msg_seq) {
  getManagerOneway((msg_seq.at(0))->sessionId)->SetMessagesAsyn(msg_seq);
}

bool GroupChatMessageLoaderAdapter::SetMessages(const MessageBodySeq &msg_seq) {
  return getManager((msg_seq.at(0))->sessionId)->SetMessages(msg_seq);
}
