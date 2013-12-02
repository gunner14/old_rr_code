#ifndef __BUDDY_REDO_MONITOR_H__
#define __BUDDY_REDO_MONITOR_H__

#include "ReplicaCluster/CacheLoader/src/RedoMonitor.h"
#include "BuddyApplyCacheAdapter.h"
#include "BuddyByIdReplicaAdapter.h"
#include "DistBuddyApplyCacheReloaderAdapter.h"
#include "DistBuddyByIdCacheReloaderAdapter.h"

namespace xce{

  namespace redomonitor{

    //在被申请者的BuddyApplyCache中添加申请者的信息
    template<class T> class RebuildBuddyApplyCacheRedoMonitor :  virtual public xce::redomonitor::RedoMonitor,virtual public Ice::Object
    {
      public:
        RebuildBuddyApplyCacheRedoMonitor( T& ctx, int id, bool beUseQueue, std::string desc):
          xce::redomonitor::RedoMonitor(beUseQueue,desc),ctx_(ctx),id_(id){
          }
        virtual void handle(){
          xce::buddyapplycache::BuddyApplyCacheAdapterN::instance().rebuildBuddyApplyCache(id_,ctx_);
        }
      private:
        T ctx_;
        int id_;
    };

    //在被申请者的BuddyApplyCache中删除申请者的信息
    template<class T> class RemoveBuddyApplyCacheRedoMonitor :  virtual public xce::redomonitor::RedoMonitor,virtual public Ice::Object
    {
      public:
        RemoveBuddyApplyCacheRedoMonitor( int id, int applicant, bool beUseQueue, std::string desc):
          xce::redomonitor::RedoMonitor(beUseQueue,desc),id_(id),applicant_(applicant){
          }
        virtual void handle(){
          xce::buddyapplycache::BuddyApplyCacheAdapterN::instance().removeBuddyApplyCache(id_,applicant_);
        }
      private:
        int id_;
        int applicant_;
    };

    //在被申请者的DistBuddyApplyCache中添加申请信息
    template<class T> class AddBuddyApplyNotifyReloaderRedoMonitor :  virtual public xce::redomonitor::RedoMonitor,virtual public Ice::Object
    {
      public:
        AddBuddyApplyNotifyReloaderRedoMonitor( T& ctx, int id, bool beUseQueue, std::string desc):
          xce::redomonitor::RedoMonitor(beUseQueue,desc),ctx_(ctx),id_(id){
          }
        virtual void handle(){
          xce::distcache::buddyapplycache::DistBuddyApplyCacheReloaderAdapter::instance().add(id_,ctx_);
        }
      private:
        T ctx_;
        int id_;
    };

    //在被申请者的DistBuddyApplyCache中删除申请者的信息
    template<class T> class RemoveBuddyApplyNotifyReloaderRedoMonitor :  virtual public xce::redomonitor::RedoMonitor,virtual public Ice::Object
    {
      public:
        RemoveBuddyApplyNotifyReloaderRedoMonitor( int accepter, int applicant, bool beUseQueue, std::string desc):
          xce::redomonitor::RedoMonitor(beUseQueue,desc),accepter_(accepter),applicant_(applicant){
          }
        virtual void handle(){
          xce::distcache::buddyapplycache::DistBuddyApplyCacheReloaderAdapter::instance().remove(accepter_,applicant_);
        }
      private:
        int accepter_;
        int applicant_;
    };

    //在BuddyByIdCache中添加双向好友关系
    template<class T> class AddBuddyByIdLoaderRedoMonitor :  virtual public xce::redomonitor::RedoMonitor,virtual public Ice::Object
    {
      public:
        AddBuddyByIdLoaderRedoMonitor( int userid, int friendid, bool beUseQueue, std::string desc):
          xce::redomonitor::RedoMonitor(beUseQueue,desc),userid_(userid),friendid_(friendid){
          }
        virtual void handle(){
          xce::buddy::adapter::BuddyByIdLoaderAdapter::instance().add(userid_,friendid_);
        }
      private:
        int userid_;
        int friendid_;
    };

    //在DistBuddyByIdCache将guest插入host的好友列表中，保持好友列表递增有序
    template<class T> class BecomeFriendNotifyByIdReloaderRedoMonitor :  virtual public xce::redomonitor::RedoMonitor,virtual public Ice::Object
    {
      public:
        BecomeFriendNotifyByIdReloaderRedoMonitor( int host, int guest, bool beUseQueue, std::string desc):
          xce::redomonitor::RedoMonitor(beUseQueue,desc),host_(host),guest_(guest){
          }
        virtual void handle(){
          xce::distcache::buddybyidcache::DistBuddyByIdCacheReloaderAdapter::instance().add(host_,guest_);
        }
      private:
        int host_;
        int guest_;
    };
 
    //在DistBuddyByIdCache的host好友中删除guest
    template<class T> class RemoveFriendNotifyByIdReloaderRedoMonitor :  virtual public xce::redomonitor::RedoMonitor,virtual public Ice::Object
    {
      public:
        RemoveFriendNotifyByIdReloaderRedoMonitor( int host, int guest, bool beUseQueue, std::string desc):
          xce::redomonitor::RedoMonitor(beUseQueue,desc),host_(host),guest_(guest){
          }
        virtual void handle(){
          xce::distcache::buddybyidcache::DistBuddyByIdCacheReloaderAdapter::instance().remove(host_,guest_);
        }
      private:
        int host_;
        int guest_;
    }; 
  }
}

#endif
