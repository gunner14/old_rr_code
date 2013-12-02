#ifndef FRIEND_FINDER_FFW_ADAPTER_H_
#define FRIEND_FINDER_FFW_ADAPTER_H_

#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce {
namespace socialgraph {

struct CommonFriend {
  CommonFriend() {
    userId = 0;
    weight = 0;
    sharesCount = 0;
  }

  int userId;
  int weight;
  int sharesCount;
  std::vector<int> shares;
};

typedef std::vector<CommonFriend> CommonFriendSeq;

//--------------------------------------------------------------------

class FriendFinderFFWAdapter : public MyUtil::Singleton<FriendFinderFFWAdapter> {
 public:
	FriendFinderFFWAdapter() {}
	
  /* @brief 从FFW cache中获取commons friends数据
   * @param userid 用户id 
   * @param limit 返回的个数
   */
	CommonFriendSeq GetCommonfriends(int userid, int limit);

  /* @brief 删除FFW cache中commons friends数据
   * @param userid 用户id 
   * @param friend 删除推荐用户的id
   */
	void RemoveCommonfriends(int userid, int friendid);
};

}
}

#endif
