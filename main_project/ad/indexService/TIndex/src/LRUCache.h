#ifndef LRU_CACHE_H_
#define LRU_CACHE_H_

#include <map>
#include <set>
#include <list>
#include <IceUtil/Monitor.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/RWRecMutex.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/Handle.h>
#include "HobbyMemcClient.h"
#include "TaskManager.h"
#define MAX_CACHE_SIZE 500000
namespace xce{
  namespace ad{
    class  LRUCache{
      class  UpdateItemQueue:public IceUtil::Monitor<IceUtil::Mutex>{ 
        public:
          void  Put(const  int& uid);
          int Get();
        private:
          std::list<int> q_;
      };
  
      class  LRUExpiredUpdate:public IceUtil::Thread{
        public:
          LRUExpiredUpdate(LRUCache& lruCache, UpdateItemQueue&  update_uids):lruCache_(lruCache),item_update_(update_uids){}
          virtual void run();
        private:
	  			LRUCache&  lruCache_;
          UpdateItemQueue &item_update_;
	  			HobbyMemcClient	  hobbyMemcClient_;
        };

			 class OutPutCacheInfo : virtual public MyUtil::Task{
				 public: 
					 OutPutCacheInfo(LRUCache& lruCache):lruCache_(lruCache){} 
					 virtual void handle(){	
						 while(true){
							 lruCache_.OutPutInfo();
							 usleep(3 * 1000 * 1000);
						 }
					 }
				 private:
					 LRUCache& lruCache_;
			 };
    
      private:
        std::map<int, std::pair<time_t, std::set<uint64_t> > >   localCache_;
        std::list<int>    uids_;
        int  max_cache_size_;
        LRUExpiredUpdate  lruExpiredUpdate_;
        UpdateItemQueue   updateItem_;
				
				IceUtil::Mutex mutex_;
      private:
        bool IsExpired(time_t   old_time){
          time_t  now = time(0);
          if (now - old_time >7 * 24 * 3600){
            return true;
          }else{
            return false;
          }
        }
      public:
        LRUCache():lruExpiredUpdate_(*this, updateItem_){
					max_cache_size_ = MAX_CACHE_SIZE;
					lruExpiredUpdate_.start().detach();
					MyUtil::TaskManager::instance().execute(new  OutPutCacheInfo(*this));
        }

				void  OutPutInfo(){
					IceUtil::Mutex::Lock lock(mutex_);	 
					MCE_INFO("LRUCache Info: localCache_ size=" << localCache_.size());
					MCE_INFO("LRUCache Info: uids_ size=" << uids_.size());
				}
        bool GetData(int  uid, std::set<uint64_t>& hobbies);
        bool PutData(int uid, const std::set<uint64_t>&  hobbies);
				void UpdateData(int uid);
    };
  };
}

#endif

