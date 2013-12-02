#include <vector>
#include "feed/FeedMemcProxy/client/notify_memc_client.h"
#include "feed/FeedMemcProxy/client/compress_wrap.h"
#include "Notify/util/Timestamp.h"

namespace xce {
namespace notify {

using namespace std;
using namespace xce::feed;
using namespace xce::notify;

static const char* const kNotifyContKeyPrefix = "NOTIFY_CONT#";
const int NotifyMemcClient::kKeyBufLen = 64;

NotifyMemcClient::NotifyMemcClient() { 
  memc_pool_.Init(50);
}

NotifyMemcClient::~NotifyMemcClient() {
}


void NotifyMemcClient::GetNotifyContMemcKey(int64_t nid, char* key_buff, 
    size_t max_len) {
  snprintf(key_buff, max_len, "%s%ld", kNotifyContKeyPrefix, nid);
}

int64_t NotifyMemcClient::GetNotifyContId(const char* key_str_buff, 
    size_t key_str_len) {
  size_t key_prefix_len = strlen(kNotifyContKeyPrefix);
  if (key_str_len < key_prefix_len) {
    MCE_WARN("NotifyMemcClient::GetNotifyContId key length error:" 
      << key_prefix_len);
    return -1;
  }
  if (strncmp(key_str_buff, kNotifyContKeyPrefix, key_prefix_len) != 0) {
    MCE_WARN("NotifyMemcClient::GetNotifyContId key error" << key_str_buff);
    return -2;
  }
  int64_t notify_id;
  try {
    string notify_id_str(key_str_buff + key_prefix_len);
    notify_id = boost::lexical_cast<int64_t>(notify_id_str);
  } catch (boost::bad_lexical_cast& exp) {
    MCE_WARN("NotifyMemcClient::GetNotifyContId lexical_cast error" 
        << " key_str_buff:" << key_str_buff);
    return -3;
  }
  return notify_id;
}

void NotifyMemcClient::PbContent2NotifyContent(const PbNotifyContent& pb_content,
    NotifyContentPtr notify_cont) {
  assert(!notify_cont);
  notify_cont->id = pb_content.notify_id();
  notify_cont->source = pb_content.source();
  notify_cont->owner = pb_content.owner();
  notify_cont->from = pb_content.from();
  notify_cont->type = pb_content.type();
  notify_cont->time = pb_content.time();
  notify_cont->isreply = pb_content.isreply();
  notify_cont->props = pb_content.props();
}

NotifyContentPtr NotifyMemcClient::GetNotifyContent(int64_t nid) {
  vector<int64_t> notify_ids;
  notify_ids.push_back(nid);
  NotifyContentDict ntfy_cont_dict;
  GetNotifyContents(notify_ids, ntfy_cont_dict);
  if (ntfy_cont_dict.empty()) {
    MCE_WARN("NotifyMemcClient::GetNotifyContent empty nid[" << nid << "]");
    return 0;
  }
  return ntfy_cont_dict[nid];
}

bool NotifyMemcClient::GetNotifyContents(const vector<int64_t>& notify_ids,
    NotifyContentDict& notify_cont_dict) {
  if (notify_ids.empty()) {
    MCE_WARN("NotifyMemcClient::GetNotifyContents notify_ids empty");
    return false;
  }
  int ntfy_ids_size = notify_ids.size(); 
  vector<string> content_keys;
  char buffer[kKeyBufLen] = {0};
  for (int i = 0; i < ntfy_ids_size; ++i) {
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s%ld", kNotifyContKeyPrefix, 
        notify_ids.at(i));
    content_keys.push_back(string(buffer));
  }
  pair<int, MemcachedClient*> memc_cli = memc_pool_.GetClient(notify_ids.at(0));
  if (!memc_cli.second) {
    MCE_WARN("NotifyMemcClient::GetNotifyContents GetClient failed");
    return false;
  }
  map<string, string> result;
  map<string, int> flags;
  Timestamp start_time = Timestamp::now();
  if(!memc_cli.second->Get(content_keys, result, flags)) {
    MCE_WARN("NotifyMemcClient::GetNotifyContents:memc client get failed"
        << " content_keys.size[" << content_keys.size() << "]");
    return false;
  }
  double memc_get_cost = timeDifference(Timestamp::now(), start_time);
  start_time = Timestamp::now();
  map<string, string>::iterator it = result.begin();
  for (; it != result.end(); ++it) {
    int64_t notify_id = GetNotifyContId(it->first.c_str(), it->first.size());
    PbNotifyContent pb_ntfy_cont;
    int flag = flags[it->first];
    if (flag & 0x01) {
      string decompressed;
      CompressWrap::instance().Decompress(it->second.c_str(), &decompressed);
      pb_ntfy_cont.ParseFromArray(decompressed.c_str(), decompressed.size());
    } else {
      pb_ntfy_cont.ParseFromArray(it->second.c_str(), it->second.size());
    }
    NotifyContentPtr notify_cont = new NotifyContent();
    PbContent2NotifyContent(pb_ntfy_cont, notify_cont);
    notify_cont_dict.insert(make_pair(notify_id, notify_cont));
  }
  double parse_cost = timeDifference(Timestamp::now(), start_time);
  memc_pool_.ReleaseClient(memc_cli.first, memc_cli.second);
  MCE_INFO("NotifyMemcClient::GetNotifyContents:nid_size[" 
      << notify_ids.size() << "] dict_size[" << notify_cont_dict.size()
      << "] memc_get_cost[" << memc_get_cost << "] parse_cost[" 
      << parse_cost << "]");
  return true;
}

bool NotifyMemcClient::SerializeContent2Str(const NotifyContentPtr& ntfy_cont,
    string* pb_cont_str){
  assert(!ntfy_cont);
  PbNotifyContent pb_ntfy_cont;
  pb_ntfy_cont.set_notify_id(ntfy_cont->id);
  pb_ntfy_cont.set_source(ntfy_cont->source);
  pb_ntfy_cont.set_owner(ntfy_cont->owner);
  pb_ntfy_cont.set_from(ntfy_cont->from);
  pb_ntfy_cont.set_type(ntfy_cont->type);
  pb_ntfy_cont.set_time(ntfy_cont->time);
  pb_ntfy_cont.set_isreply(ntfy_cont->isreply);
  pb_ntfy_cont.set_props(ntfy_cont->props);

  return pb_ntfy_cont.SerializeToString(pb_cont_str);
}

bool NotifyMemcClient::SetNotifyContent(const NotifyContentPtr& ntfy_content) {
  char key_str_buff[kKeyBufLen] = {0};
  GetNotifyContMemcKey(ntfy_content->id, key_str_buff, sizeof(key_str_buff));
  string pb_cont_str;
  if (!SerializeContent2Str(ntfy_content, &pb_cont_str)) {
    MCE_WARN("NotifyMemcClient SerializeContent2Str failed");
    return false;
  }
  pair<int, MemcachedClient*> memc_cli = memc_pool_.GetClient(
      ntfy_content->id);
  bool ret = memc_cli.second->Set(key_str_buff, pb_cont_str, 0);
  memc_pool_.ReleaseClient(memc_cli.first, memc_cli.second);
  if (!ret) {
    MCE_WARN("NotifyMemcClient::SetNotifyContent memcached set failed");
  } 
  return ret;
}

}
}

