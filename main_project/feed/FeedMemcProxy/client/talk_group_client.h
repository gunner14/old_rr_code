#ifndef _XCE_FEED_CLIENT_TALK_GROUP_CLIENT_H_
#define _XCE_FEED_CLIENT_TALK_GROUP_CLIENT_H_

#include <stdint.h>
#include <vector>
#include "TalkGroupLoader.h"
#include "Group.pb.h"  //guoqing.liu  如果可能的话，咱们改改 proto 的这个命名....太大了，起码叫 TalkGroupInfo 也行啊
#include "feed/FeedMemcProxy/client/MemcachedClientPool.h"

namespace xce {
namespace talk {

using namespace MyUtil;
using namespace com::xiaonei::talk::group::loader;
using namespace xce::xntalk::groupchat;

class TalkGroupInfoClient : public Singleton<TalkGroupInfoClient> {
 public:
  TalkGroupInfoClient(); 
  ~TalkGroupInfoClient();

  static void GetGroupMemcKey(int64_t nid, char* key_buff, size_t max_len);
  static void PbTalkGroupInfo(const GroupChatGroupInfo& info_content, GroupInfoPtr& info_cont);
  bool GetGroupInfo(int64_t sid, GroupInfoPtr& info);  
  //GroupInfoPtr GetGroupInfo(int64_t sid);
	int64_t GetSessionId(const char* key_str_buff, size_t key_str_len); 
  bool GetGroupInfos(const std::vector<int64_t>& session_ids, GroupInfoDict& info_dict); // guoqing.liu  不把loader封装在client里面的方式
  // 走cache不全，走db的方式
  bool GetGroupInfoByIdWithLoad(int64_t sid, GroupInfoPtr &info);
  bool GetGroupInfosBySeqWithLoad(const std::vector<int64_t> &session_ids, GroupInfoDict &info_dict);

  bool SetGroupInfo(const GroupInfoPtr& info_content); 
  bool DeleteGroupInfo(const long sid);

  bool SerializeContent2Str(const GroupInfoPtr& info_cont, string* pb_info_str);

 private:
  static const int kKeyBufLen;
  PoolManager pool_;
	typedef map<int,GroupInfoPtr> GroupInfoInterMap;
};

}
}

#endif //_XCE_FEED_CLIENT_TALK_GROUP_CLIENT_H_
