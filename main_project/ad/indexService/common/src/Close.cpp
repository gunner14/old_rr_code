/*
 * =====================================================================================
 *
 *       Filename:  Close.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月30日 11时39分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "Close.h"
#include "db.h"

using namespace xce::ad;

void AdCloseManager::CloseAd(int uid, long creative_id){
  
  MCE_DEBUG("CloseAd, UID: " << uid << ", creative_id: " << creative_id);

  CloseItem item(uid, creative_id);
  InsertToPool(item); 
  DispatchToDBInserter(item); 
}

void AdCloseManager::FilterClosedAds(int uid, std::set<AdGroupPtr> &groups){

  //MCE_DEBUG("FilterClosedAds, UID:" << uid);

  std::set<CloseItem> closed_items;

  {
    IceUtil::RWRecMutex::RLock lock(lock_);

    __gnu_cxx::hash_map<int, std::set<CloseItem> >::iterator cit = closed_ad_pool_.find(uid);
    if(cit == closed_ad_pool_.end()){
      return;
    }else{
      closed_items = cit->second;
    }

  }

  if(closed_items.empty()){
    //MCE_DEBUG("closed_items IS EMPTY");
    return ;
  }

  for(std::set<AdGroupPtr>::iterator it = groups.begin(); it != groups.end();){
    AdGroupPtr g = *it;
    CloseItem item(uid, g->creative_id_, 0);
    if(closed_items.find(item) != closed_items.end()){
      groups.erase(it++);
      //MCE_DEBUG("FilterClosedAds group " << g->creative_id_ << " is closed");
    }
    else{
      //MCE_DEBUG("FilterClosedAds " << g->creative_id_ << " is not closed");
      ++it;
    }
  }
}

void AdCloseManager::InsertToPool(const CloseItem &item){

  IceUtil::RWRecMutex::WLock lock(lock_);
  closed_ad_pool_[item.uid_].insert(item);
}

void AdCloseManager::DispatchToDBInserter(const CloseItem &item){
  ci_queue_.Put(item);
}

int AdCloseManager::init(){

  int ret = closed_ad_reloader_.Load();
  if(ret < 0){
    return -1;
  }

  db_inserter_.start().detach();
  closed_ad_reloader_.start().detach();

  return 0;

}

void AdCloseManager::AdCloseInserter::run(){
  while(true){
    CloseItem item = item_queue_.Get();
    MCE_DEBUG("InsertClosedAd UID: " << item.uid_ << ", creative_id: " << item.creative_id_);
    DBSelector::instance().InsertClosedAd(item);
  }
}

int AdCloseManager::AdCloseLoader::Load(){
  __gnu_cxx::hash_map<int, std::set<CloseItem> > pool;
  MCE_INFO("AdCloseManager::AdCloseLoader::Load begin to load closed ads before " << ad_valid_minutes_ << " minutes");
  int ret = DBSelector::instance().LoadClosedAd(pool, ad_valid_minutes_, max_close_id_);
  if(ret < 0){
    MCE_WARN("Load ClosedAd FAILED");
    return -1;
  }

  {
    IceUtil::RWRecMutex::WLock lock(AdCloseManager::instance().lock_);
    pool.swap(AdCloseManager::instance().closed_ad_pool_);
  }

  MCE_INFO("AdCloseManager::AdCloseLoader::Load load " << AdCloseManager::instance().closed_ad_pool_.size() << " closed users" 
            << ",max_close_id: " << max_close_id_);
  return 0;
}

int AdCloseManager::AdCloseLoader::Reload(){
  std::map<int, std::list<CloseItem> > closed_items;
  int ret = DBSelector::instance().ReloadClosedAd(closed_items, max_close_id_);
  if(ret < 0){
    MCE_WARN("ReLoad ClosedAd FAILED");
    return -1;
  }
  if(closed_items.empty()){
    MCE_INFO("ReLoad ClosedAd get 0 closed ad");
    return 0;
  }

  MCE_INFO("ReLoad ClosedAd get " << closed_items.size() << " closed ads" << ", max_close_id: " << max_close_id_);

  __gnu_cxx::hash_map<int, std::set<CloseItem> > &closed_ad_pool = AdCloseManager::instance().closed_ad_pool_;

  for(std::map<int, std::list<CloseItem> >::iterator it = closed_items.begin(); it != closed_items.end(); ++it){
    int uid = it->first;
    std::list<CloseItem> &user_closed_items = it->second;

    IceUtil::RWRecMutex::WLock lock(AdCloseManager::instance().lock_);
    closed_ad_pool[uid].insert(user_closed_items.begin(), user_closed_items.end());
  }


  return 0;
}

void AdCloseManager::AdCloseLoader::run(){
  while(true){
    sleep(180);
    Reload();
    time_t lt = time(NULL);
    struct tm t;
    localtime_r(&lt, &t);

    int hour = t.tm_hour;
    if(hour >= 3 && hour < 6 && last_clear_time_.tm_mday != t.tm_mday){
      ClearInvalidAds();
      last_clear_time_ = t;
    }
  }
}

void AdCloseManager::AdCloseLoader::ClearInvalidAds(){

  MCE_INFO("BEGIN running ClearInvalidAds");
  int total_ads_num = 0;
  int removed_ads_num = 0;
  time_t now = time(NULL);
  __gnu_cxx::hash_map<int, std::set<CloseItem> > &closed_ad_pool = AdCloseManager::instance().closed_ad_pool_;
  for(__gnu_cxx::hash_map<int, std::set<CloseItem> >::iterator it = closed_ad_pool.begin(); it != closed_ad_pool.end(); ++it){
    std::set<CloseItem> &itemSet = it->second;
    total_ads_num += itemSet.size();
    IceUtil::RWRecMutex::WLock lock(AdCloseManager::instance().lock_);
    for(std::set<CloseItem>::iterator sit = itemSet.begin(); sit != itemSet.end();){
      if(sit->close_time_ + ad_valid_minutes_ * 60 < now){
        //MCE_DEBUG("ClearInvalidAds uid:" << sit->uid_ << ",creative_id:" << sit->creative_id_);
        itemSet.erase(sit++);
        ++removed_ads_num;
      }
      else{
        ++sit;
      }
    }
  }

  int remain_ads_num = total_ads_num - removed_ads_num;
  MCE_INFO("END running ClearInvalidAds," << removed_ads_num << " were removed, " << "remain " << remain_ads_num);
}

/*
 * Func:AdCloseManager::Print
 * Desc:用户关闭广告信息
 */
string AdCloseManager::Print() {
  /*IceUtil::RWRecMutex::RLock lock(lock_);
  ostringstream os;
  __gnu_cxx::hash_map<int, std::set<CloseItem> >::iterator it = closed_ad_pool_.begin();
  MCE_INFO("bin.qin"<<closed_ad_pool_.size());
  for (int i=1; it != closed_ad_pool_.end(); ++it, ++i) {
    os << "[" << i << "]" << it->first << ":";
    set<CloseItem>::iterator it_c = it->second.begin();
    MCE_INFO("bin.qin"<<it->second.size());
    for (; it_c != it->second.end(); ++it_c) {
      os << (*it_c).creative_id_ << ",";
    }
    os << ";\n";
  }
  return os.str();*/
  return "";
}

