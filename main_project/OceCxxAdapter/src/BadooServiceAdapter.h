#ifndef __BADOOSERVICE_ADAPTER_H__
#define __BADOOSERVICE_ADAPTER_H__
#include "gen-cpp/BadooService.h" 
#include "Xoa2AdapterI.h"


namespace com {
namespace renren {
namespace xoa2 {
namespace sns {
namespace badoo {

class BadooAdapter : public MyUtil::Xoa2AdapterI<BadooServiceClient>,
                          virtual public MyUtil::Singleton<BadooAdapter> {

public:
 BadooAdapter() : MyUtil::Xoa2AdapterI<BadooServiceClient>("badoo.ugc.xoa.renren.com", 200)
  {
  }

    int32_t getBadooIdByRId(const int32_t rId);
    int32_t getRIdByBId(const int32_t bId);
    int32_t sendGossip(const int32_t senderBId, const int32_t receiverBId, const std::string& message);
    void getLikeUser(BadooLikeInfoList& rv, const int32_t rId, const int32_t limit);
    void getBeLikeUser(BadooLikeInfoList& rv, const int32_t rId, const int32_t limit);
    void getMutileLikeUser(BadooLikeInfoList& rv, const int32_t rId, const int32_t limit);

    void getUserBaseInfoByRId(BadooUserInfoList& rv, const IdWrappers& rIds);
    void getUserBaseInfoByBId(BadooUserInfoList& rv, const IdWrappers& bIds);


};


}
}
}
}
}

#endif
