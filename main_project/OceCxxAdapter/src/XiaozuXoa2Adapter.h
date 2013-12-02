#ifndef __XIAOZUXOA2_ADAPTER_H__
#define __XIAOZUXOA2_ADAPTER_H__
#include "gen-cpp/XiaozuXoa2Service.h" 
#include "Xoa2AdapterI.h"


namespace com {
namespace renren {
namespace xoa2 {
namespace sns {
namespace xiaozu {

class XiaozuXoa2Adapter : public MyUtil::Xoa2AdapterI<XiaozuXoa2ServiceClient>,
                          virtual public MyUtil::Singleton<XiaozuXoa2Adapter> {

public:
 XiaozuXoa2Adapter() : MyUtil::Xoa2AdapterI<XiaozuXoa2ServiceClient>("forum.ugc.xoa.renren.com", 200)
  {
  }
  bool postComment(const int64_t userId, const int64_t forumId, 
                   const int64_t threadId, const std::string& body, const int32_t toId);

  void getXiaozuThread(XiaozuThread& rt, const int32_t forumId, const int32_t threadId);
  void getCommentList(XiaozuCommentList& rt, const int64_t forumId, const int64_t threadId, const int32_t page);



};


}
}
}
}
}

#endif
