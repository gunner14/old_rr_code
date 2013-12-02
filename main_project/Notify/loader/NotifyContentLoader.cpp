#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "UtilCxx/src/LogWrapper.h"
#include "XmppTools.h"
#include "Notify/util/time_stat.h"
#include "Notify/loader/NotifyContentLoader.h"
#include "Notify/loader/NotifyContentDBLoader.h"
#include "feed/FeedMemcProxy/client/notify_memc_client.h"

using namespace std;
using namespace xce::notify;
using namespace MyUtil;
using namespace com::xiaonei::talk::util;

//first find in memcache, if not found then find in db and store into cache
NotifyContentDict NotifyContentLoader::GetNotifyContentDict(
    const vector<int64_t>& nids_seq) {
	TimeStat ts;
  NotifyContentDict memc_cont_dict;
  //first find in cache
  vector<int64_t> deleted_nids;
  bool ret = NotifyMemcClient::instance().GetNotifyContents(nids_seq, 
      memc_cont_dict, deleted_nids);
  if (!ret) { //Get Content failed, return empty dict
    return memc_cont_dict;
  }
  vector<int64_t> miss_nids;
  size_t nids_seq_size = nids_seq.size();
  for(size_t i = 0; i < nids_seq_size; ++i) {
    int64_t nid = nids_seq[i];
    if(memc_cont_dict.count(nid) == 0) {
      miss_nids.push_back(nid);
    }
  }
  miss_nids.insert(miss_nids.begin(), deleted_nids.begin(), deleted_nids.end());
  sort(miss_nids.begin(), miss_nids.end());
  miss_nids.erase(unique(miss_nids.begin(), miss_nids.end()), miss_nids.end());
  if (miss_nids.empty()) {
    MCE_INFO("NotifyContentLoader::GetNotifyContentDict --> all hit nids_size:"
        << nids_seq_size <<" contents_size:" << memc_cont_dict.size());
    return memc_cont_dict;
  }
  //load the missed contents from db
  NotifyContentDict db_cont_dict;
  NotifyContentDBLoader::instance().LoadNotifyContentsParal(miss_nids, &db_cont_dict);
  for(NotifyContentDict::iterator it = db_cont_dict.begin(); 
      it != db_cont_dict.end(); ++it) {
    if (!it->second) {
      MCE_WARN("NotifyContentLoader::GetNotifyContentDict --> get empty content nid:" 
          << it->first);
    } else {
      AddNotifyContent(it->second, false);
      memc_cont_dict.insert(*it);
    }
  }
  MCE_INFO("NotifyContentLoader::GetNotifyContentDict --> load from db" 
      << " miss:" << miss_nids.size() 
      << " load_from_db:" << db_cont_dict.size()
      << " memc_cont_size:" << memc_cont_dict.size()
      << " miss_nids:" << ContentPrintHelper::Print(miss_nids, 20));
	FunStatManager::instance().Stat("NotifyContentLoader::GetNotifyContentDict", ts.getTime(), false);
  return memc_cont_dict;
}

bool NotifyContentLoader::RemoveNotifyContents(const vector<int64_t>& nids) {
	TimeStat ts;
	bool flag = false;
  MCE_INFO("NotifyContentLoader::RemoveNotifyContents nids.size" << nids.size());
  for (size_t i = 0; i < nids.size(); i++) {
    NotifyMemcClient::instance().SetEmptyContent(nids[i]);
  }
  flag =  NotifyContentDBLoader::instance().DeleteNotifyContents(nids);
	FunStatManager::instance().Stat("NotifyContentLoader::RemoveNotifyContents", ts.getTime(), false);
	return flag;
}

bool NotifyContentLoader::AddNotifyContent(const NotifyContentPtr& content,
    bool compressFlag) {
	TimeStat ts;
	bool flag = false;
  //MCE_INFO("NotifyContentLoader::AddNotifyContent --> add content to memcache nid:" 
  //    << content->id << " flag:" << compressFlag);
  flag = NotifyMemcClient::instance().SetNotifyContent(content, compressFlag);
	FunStatManager::instance().Stat("NotifyContentLoader::AddNotifyContent", ts.getTime(), false);
	return flag;
}

