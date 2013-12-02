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
static const char* const kEmptyContent = "EMPTY_CONTENT";
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
  if (notify_cont) {
    notify_cont->id = pb_content.notify_id();
    notify_cont->source = pb_content.source();
    notify_cont->owner = pb_content.owner();
    notify_cont->from = pb_content.from();
    notify_cont->type = pb_content.type();
    notify_cont->time = pb_content.time();
    notify_cont->isreply = pb_content.isreply();
    notify_cont->props = pb_content.props();
  } else {
    MCE_WARN("NotifyMemcClient::PbContent2NotifyContent notify_cont is null");
  }
}

NotifyContentPtr NotifyMemcClient::GetNotifyContent(int64_t nid) {
  vector<int64_t> notify_ids, deleted_ids;
  notify_ids.push_back(nid);
  NotifyContentDict ntfy_cont_dict;
  GetNotifyContents(notify_ids, ntfy_cont_dict, deleted_ids);
  if (ntfy_cont_dict.empty()) {
    MCE_WARN("NotifyMemcClient::GetNotifyContent empty nid[" << nid << "]");
    return 0;
  }
  return ntfy_cont_dict[nid];
}

bool NotifyMemcClient::GetNotifyContents(const vector<int64_t>& notify_ids,
    NotifyContentDict& notify_cont_dict, vector<int64_t>& deleted_ids) {
  if (notify_ids.empty()) {
    MCE_WARN("NotifyMemcClient::GetNotifyContents notify_ids empty");
    return false;
  }
  size_t ntfy_ids_size = notify_ids.size(); 
  vector<string> content_keys;
  char buffer[kKeyBufLen] = {0};
  for (size_t i = 0; i < ntfy_ids_size; ++i) {
    memset(buffer, 0, sizeof(buffer));
    GetNotifyContMemcKey(notify_ids[i], buffer, sizeof(buffer));
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
    MCE_WARN("NotifyMemcClient::GetNotifyContents:memcache get failed"
        << " content_keys size[" << content_keys.size() << "]");
    return false;
  }
  double memc_get_cost = timeDifference(Timestamp::now(), start_time);
  memc_pool_.ReleaseClient(memc_cli.first, memc_cli.second);
  start_time = Timestamp::now();
  map<string, string>::iterator it = result.begin();
  for (; it != result.end(); ++it) {
    int64_t notify_id = GetNotifyContId(it->first.c_str(), it->first.size());
    if (it->second == kEmptyContent) {
      deleted_ids.push_back(notify_id);
      MCE_DEBUG("it->second:" << it->second << "kEmptyContent:" << kEmptyContent);
      continue;
    }
    PbNotifyContent pb_ntfy_cont;
    int flag = flags[it->first];
    if (flag & 0x01) {
      string decompressed;
      CompressWrap::instance().Decompress(it->second.c_str(), &decompressed);
      pb_ntfy_cont.ParseFromArray(decompressed.c_str(), decompressed.size());
      MCE_DEBUG("NotifyMemcClient::GetNotifyContents --> decompressed nid:" 
          << notify_id); 
    } else {
      pb_ntfy_cont.ParseFromArray(it->second.c_str(), it->second.size());
      MCE_DEBUG("NotifyMemcClient::GetNotifyContents --> nid:" << notify_id);
    }
    NotifyContentPtr notify_cont = new NotifyContent();
    PbContent2NotifyContent(pb_ntfy_cont, notify_cont);
    notify_cont_dict.insert(make_pair(notify_id, notify_cont));
  }
  MCE_INFO("NotifyMemcClient::GetNotifyContents --> nid_size:" 
      << notify_ids.size() << " dict_size:" << notify_cont_dict.size()
      << " memc_get_cost:" << memc_get_cost << "");
  return true;
}

bool NotifyMemcClient::SerializeContent2Str(const NotifyContentPtr& ntfy_cont,
    string* pb_cont_str){
  PbNotifyContent pb_ntfy_cont;
  if (ntfy_cont) {
    pb_ntfy_cont.set_notify_id(ntfy_cont->id);
    pb_ntfy_cont.set_source(ntfy_cont->source);
    pb_ntfy_cont.set_owner(ntfy_cont->owner);
    pb_ntfy_cont.set_from(ntfy_cont->from);
    pb_ntfy_cont.set_type(ntfy_cont->type);
    pb_ntfy_cont.set_time(ntfy_cont->time);
    pb_ntfy_cont.set_isreply(ntfy_cont->isreply);
    pb_ntfy_cont.set_props(ntfy_cont->props);
  }
  return pb_ntfy_cont.SerializeToString(pb_cont_str);
}

bool NotifyMemcClient::SetNotifyContent(const NotifyContentPtr& ntfy_content,
   bool compress_flag) {
  char key_str_buff[kKeyBufLen] = {0};
  Timestamp start_time = Timestamp::now();
  GetNotifyContMemcKey(ntfy_content->id, key_str_buff, sizeof(key_str_buff));
  string pb_cont_str;
  if (!ntfy_content) {
    MCE_WARN("NotifyMemcClient::SetNotifyContent --> set empty notify content");
    return false;
  }
  if (!SerializeContent2Str(ntfy_content, &pb_cont_str)) {
    MCE_WARN("NotifyMemcClient::SetNotifyContent --> SerializeContent2Str failed");
    return false;
  }
  pair<int, MemcachedClient*> memc_cli = memc_pool_.GetClient(
      ntfy_content->id);
  bool ret = false;
  if (compress_flag) {
    string compressed_str;
    CompressWrap::instance().Compress(pb_cont_str, &compressed_str);
    ret = memc_cli.second->Set(key_str_buff, compressed_str, 0x01);
  } else {
    ret = memc_cli.second->Set(key_str_buff, pb_cont_str, 0);
  }
  memc_pool_.ReleaseClient(memc_cli.first, memc_cli.second);
  if (!ret) {
    MCE_WARN("NotifyMemcClient::SetNotifyContent memcached set failed");
  } 
  double memc_set_cost = timeDifference(Timestamp::now(), start_time);
  MCE_INFO("NotifyMemcClient::SetNotifyContent --> nid:" << ntfy_content->id 
      << " owner:" << ntfy_content->owner
      << " from:" << ntfy_content->from
      << " flag:" << compress_flag 
      << " cost:" << memc_set_cost);
  return ret;
}

bool NotifyMemcClient::SetEmptyContent(int64_t ntfy_cont_id) {
  char key_str_buff[kKeyBufLen] = {0};
  Timestamp start_time = Timestamp::now();
  GetNotifyContMemcKey(ntfy_cont_id, key_str_buff, sizeof(key_str_buff));
  pair<int, MemcachedClient*> cli = memc_pool_.GetClient(ntfy_cont_id);
  if (!cli.second) {
    MCE_WARN("NotifyMemcClient::SetEmptyContent --> cant't get client nid:"
        << ntfy_cont_id);
    return false; 
  } 
  bool res = cli.second->Set(key_str_buff, kEmptyContent, 0);
  memc_pool_.ReleaseClient(cli.first, cli.second);
  double memc_set_cost = timeDifference(Timestamp::now(), start_time);
  MCE_INFO("NotifyMemcClient::SetEmptyContent --> nid:" << ntfy_cont_id 
      << " cost:" << memc_set_cost);
  return res;
}

}
}

