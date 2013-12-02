#ifndef _CLIENT_TALK_GROUP_MEMBER_LIST_H_
#define _CLIENT_TALK_GROUP_MEMBER_LIST_H_

#include <stdint.h>
#include <vector>
#include "TalkGroupMemberLoader.h"
#include "TalkGroupMember.pb.h"
#include "feed/FeedMemcProxy/client/MemcachedClientPool.h"

namespace xce {
namespace talk {

using namespace MyUtil;
using namespace com::xiaonei::talk::group::memberloader;

class TalkGroupMemberClient : public Singleton<TalkGroupMemberClient> {
 public:
  TalkGroupMemberClient(); 
  ~TalkGroupMemberClient();

  static void GetGroupMemcKey(int64_t nid, char* key_buff, size_t max_len);//OK
	int64_t GetSessionId(const char* key_str_buff, size_t key_str_len); 
  bool SetGroupMember(const GroupMemberDict& group_member); 
  static void PbTalkGroupMember(const PbGroupMember& pb_group_member, GroupMemberSeq& group_member);
  bool GetGroupMember(int64_t sid, GroupMemberDict& member_list);  
  bool GetGroupMembers(const std::vector<int64_t>& session_ids, GroupMemberDict& info_dict); // guoqing.liu  不把loader封装在client里面的方式
  // 走cache不全，走db的方式
  bool GetGroupMembersBySeqWithLoad(const std::vector<int64_t>& session_ids, GroupMemberDict& info_dict);
  bool GetGroupMembersByIdWithLoad(int64_t sid, GroupMemberSeq& member_list);  

  bool DeleteOneGroup(int64_t sid);
  void DeleteGroup(const MyUtil::LongSeq &idSeq);

  static bool SerializeContent2Str(long sid, const GroupMemberSeq& members, string* pb_info_str);


 private:
  static const int kKeyBufLen;
  PoolManager pool_;
	typedef map<int,GroupMemberPtr> GroupMemberInterMap;
};

}
}

#endif
