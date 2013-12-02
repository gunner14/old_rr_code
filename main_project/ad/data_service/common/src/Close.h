/*
 * =====================================================================================
 *
 *       Filename:  Close.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月30日 10时43分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef __INDEX_SERVICE_CLOSE_H__
#define __INDEX_SERVICE_CLOSE_H__

#include <map>
#include <set>
#include <list>
#include <ext/hash_map>

#include <IceUtil/Monitor.h>
#include <IceUtil/Mutex.h>
#include "Singleton.h"
#include "index.h"

namespace xce{
  namespace ad{

    struct CloseItem{

      bool operator <(const CloseItem &item) const {
        return creative_id_ < item.creative_id_;
      }

      CloseItem(int uid, long creative_id):uid_(uid), creative_id_(creative_id){
        close_time_ = time(NULL);
      }

      CloseItem(){}
      CloseItem(int uid, long creative_id, time_t t):uid_(uid), creative_id_(creative_id), close_time_(t){}

      int uid_;
      long creative_id_;
      time_t close_time_;

    };

    class CloseItemQueue: public IceUtil::Monitor<IceUtil::Mutex> {

      public:

        void Put(const CloseItem& item) {
          IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
          if(q_.size() >= 100){
            MCE_WARN("CloseItemQueue::Put close queue's size is :" << q_.size());
            return;
          }
          q_.push_back(item);
          notify();
        }

        CloseItem Get() {
          IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
          while (q_.size() == 0)
            wait();
          if(q_.size() > 50){
            MCE_WARN("CloseItemQueue::Get close queue's size is :" << q_.size());
          }
          CloseItem item = q_.front();
          q_.pop_front();
          return item;
        }

      private:

        std::list<CloseItem> q_;
    };

    class AdCloseManager : public MyUtil::Singleton<AdCloseManager>{

      public:

        AdCloseManager():db_inserter_(ci_queue_){}
        int init();
        void CloseAd(int uid, long creative_id);
        void FilterClosedAds(int uid, std::set<AdGroupPtr> &groups);
        void setAdValidTime(int minutes){
          return closed_ad_reloader_.setAdValidTime(minutes);
        }

      private:

        class AdCloseLoader : public IceUtil::Thread{
          public:
            AdCloseLoader():max_close_id_(0){
              time_t lt = time(NULL);
              localtime_r(&lt, &last_clear_time_);
            }

            int Load();
            int Reload();
            void ClearInvalidAds();
            virtual void run();
            void setAdValidTime(int minutes){
              ad_valid_minutes_ = minutes;
            }
          private:
            long max_close_id_; 
            int ad_valid_minutes_;
            struct tm last_clear_time_;
        };

        class AdCloseInserter: public IceUtil::Thread{
          public:
            AdCloseInserter(CloseItemQueue &queue):item_queue_(queue){}
            virtual void run();
          private:
            CloseItemQueue &item_queue_;
        };

        friend class AdCloseInserter;
        friend class AdCloseLoader;

        void InsertToPool(const CloseItem &item);
        void DispatchToDBInserter(const CloseItem &item);
        
        std::string db_source_;

        IceUtil::RWRecMutex lock_;

        CloseItemQueue ci_queue_;

        AdCloseInserter db_inserter_;
        AdCloseLoader closed_ad_reloader_;

        __gnu_cxx::hash_map<int, std::set<CloseItem> > closed_ad_pool_; 
    };
  }
}

#endif
