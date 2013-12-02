#include "LRUCache.h"

using namespace std;
using namespace xce::ad;

bool LRUCache::GetData(int  uid, set<uint64_t>& hobbies){
	/*IceUtil::Mutex::TryLock lock(mutex_);
	if(!lock.acquired()){
		return false;
	}*/
	
	IceUtil::Mutex::Lock lock(mutex_);
	map<int, pair<time_t, set<uint64_t> > >::iterator it = localCache_.find(uid);
  if (it == localCache_.end()){
    return false;
  }else{
    hobbies.insert(it->second.second.begin(), it->second.second.end());
		list<int>::iterator iter = find(uids_.begin(), uids_.end(), it->first);
		if(iter == uids_.end()){
			MCE_WARN("LRUCache::GetData:cache not consistent"); 
		}else{
			int  strKey = *iter;
			uids_.erase(iter);
			uids_.push_front(strKey);
			//注意有效期的判断
			if (IsExpired(it->second.first)){
				updateItem_.Put(uid);
			}
		}
    return true;
  }
}

//向本地cache写数据
bool  LRUCache::PutData(int uid, const set<uint64_t>&  hobbies){ 
	IceUtil::Mutex::Lock lock(mutex_);
	map<int, pair<time_t, set<uint64_t> > >::iterator it = localCache_.find(uid);
  if (it != localCache_.end()){
    localCache_[uid].first = time(0);
		localCache_[uid].second.clear();
		localCache_[uid].second.insert(hobbies.begin(), hobbies.end());
    list<int>::iterator iter = find(uids_.begin(), uids_.end(), it->first);
		if(iter == uids_.end()){
			MCE_WARN("LRUCache::PutData:cache not consistent"); 
		}else{
			int  uid = *iter;
			uids_.erase(iter);
			uids_.push_front(uid);
		}
  }else{
    if (localCache_.size() > (size_t)max_cache_size_){
      int  deleteuid = uids_.back();
      uids_.pop_back();
      localCache_.erase(deleteuid);
    }
		localCache_[uid] = make_pair(time(0), set<uint64_t>(hobbies.begin(), hobbies.end()));
    uids_.push_front(uid);
  }
	return true;
}

void LRUCache::UpdateData(int uid){
	updateItem_.Put(uid);
}


void LRUCache::LRUExpiredUpdate::run(){
  while(true){
    int item = item_update_.Get();
    //从memcache取数据
		set<uint64_t>  hobbies;
		bool ret  = hobbyMemcClient_.GetData(item, hobbies);
		if(ret == false){
			MCE_WARN("LRUCache::LRUExpiredUpdate Get Data From Memcached Error");
			continue;
		}
		lruCache_.PutData(item, hobbies);
  }
}

void LRUCache::UpdateItemQueue::Put(const  int& uid) {
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
	if(q_.size() >= 100){
    return;
  }

  std::list<int>::iterator it = find(q_.begin(), q_.end(), uid);
  if (it != q_.end()){
    return;
  }

  q_.push_back(uid);
  if (q_.size() == 1){
    notify();
  }
}

int LRUCache::UpdateItemQueue::Get() {
	IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
	while (q_.size() == 0){
    wait();
  }
  int item = q_.front();
  q_.pop_front();
  return item;
}

