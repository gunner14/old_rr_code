#ifndef _XCE_FEED_CLIENT_NOTIFY_MEMC_CLIENT_H_
#define _XCE_FEED_CLIENT_NOTIFY_MEMC_CLIENT_H_

#include <stdint.h>
#include <vector>
#include "Notify.h"
#include "Notify.pb.h"
#include "feed/FeedMemcProxy/client/MemcachedClientPool.h"

namespace xce {
namespace notify {

using namespace MyUtil;

class NotifyMemcClient : public Singleton<NotifyMemcClient> {
 public:
  NotifyMemcClient(); 
  ~NotifyMemcClient();

  static void GetNotifyContMemcKey(int64_t nid, char* key_buff, size_t max_len);
  static int64_t GetNotifyContId(const char* key_str_buff, size_t key_str_len);
  static void PbContent2NotifyContent(const PbNotifyContent& pb_content,
      NotifyContentPtr notify_cont);
  NotifyContentPtr GetNotifyContent(int64_t nid);
  bool GetNotifyContents(const std::vector<int64_t>& notify_ids,
      NotifyContentDict& notify_content_dict, 
      std::vector<int64_t>& deleted_nids);
  bool SetNotifyContent(const NotifyContentPtr& ntfy_content, 
      bool compress_flag);
  bool SetEmptyContent(int64_t ntfy_cont_id);
  bool SerializeContent2Str(const NotifyContentPtr& ntfy_cont, string* pb_cont);

 private:
  static const int kKeyBufLen;
  PoolManager memc_pool_;
};

}
}

#endif //_XCE_FEED_CLIENT_NOTIFY_MEMC_CLIENT_H_
