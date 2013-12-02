#include "XiaozuXoa2Adapter.h"

using namespace com::renren::xoa2::sns::xiaozu;

bool XiaozuXoa2Adapter::postComment(const int64_t userId, const int64_t forumId, 
                 const int64_t threadId, const std::string& body, const int32_t toId)
{
  XOA2_RPC_RETURN_CALL(XiaozuXoa2ServiceClient, postComment, 0, (userId, forumId, threadId, body, toId));

}

void XiaozuXoa2Adapter::getXiaozuThread(XiaozuThread& rt, const int32_t forumId, const int32_t threadId)
{
  XOA2_RPC_CALL(XiaozuXoa2ServiceClient, getXiaozuThread, 0, (rt, forumId, threadId));

}


void XiaozuXoa2Adapter::getCommentList(XiaozuCommentList& rt, const int64_t forumId, const int64_t threadId, const int32_t page)
{
  XOA2_RPC_CALL(XiaozuXoa2ServiceClient, getCommentList, 0, (rt, forumId, threadId, page));
}


