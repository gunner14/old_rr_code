#ifndef __BUDDY_BYADDTIME_CACHE_I_H__
#define __BUDDY_BYADDTIME_CACHE_I_H__

#include "BuddyByAddTimeCache.h"
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <string>
#include "UtilCxx/src/Singleton.h"
#include "IceExt/src/ObjectCacheI.h"
#include "IceExt/src/ServiceI.h"

namespace xce {
  namespace buddy {

    const int BUDDY_BYADDTIME_CACHE_DATA = 0;

    const int TASK_LEVEL_BUDDY_BYADDTIME_CACHE_FILL = 1;
    const int TASK_LEVEL_BUDDY_BYADDTIME_CACHE_LOAD = 2;
    const int TASK_LEVEL_BUDDY_BYADDTIME_CACHE_RELOAD = 3;
    const int TASK_LEVEL_BUDDY_BYADDTIME_CACHE_REMOVE = 4;

    /*****************************************************************************/
    class BuddyByAddTimeDataI: virtual public Ice::Object {
      public:
        BuddyByAddTimeDataI();
        BuddyByAddTimeDataI(const MyUtil::IntSeq&,const MyUtil::IntSeq&);
        BuddyByAddTimeDataI(BuddyByAddTimeDataNPtr);
        Ice::Int getCount();
        ::MyUtil::IntSeq getList(::Ice::Int,::Ice::Int);
        BuddyByAddTimeNodeSeq getListWithinTime(::Ice::Int,::Ice::Int);
        BuddyByAddTimeNodeSeq getListAndTime(::Ice::Int);
        BuddyByAddTimeNodeSeq getListAndTimeWithBegin(::Ice::Int, ::Ice::Int);
        void remove(::Ice::Int);
      private:
        ::MyUtil::IntSeq _friends;
        ::MyUtil::IntSeq _times;
        IceUtil::Mutex _mutex;
    };
    typedef IceUtil::Handle<BuddyByAddTimeDataI> BuddyByAddTimeDataIPtr;

    class BuddyByAddTimeCacheManagerI: virtual public BuddyByAddTimeCacheManager, virtual public MyUtil::Singleton<BuddyByAddTimeCacheManagerI> {
      public:
        void bootstrap(const string& endpoint, int mod);
        virtual ::Ice::Int getFriendCount(::Ice::Int, const Ice::Current&);
        virtual ::MyUtil::Int2IntMap getFriendCounts(const MyUtil::IntSeq&, const Ice::Current&);
        virtual ::MyUtil::IntSeq getFriendList(::Ice::Int, ::Ice::Int, const Ice::Current&);
        virtual ::MyUtil::IntSeq getFriendListN(::Ice::Int, ::Ice::Int, ::Ice::Int, const Ice::Current&);
        virtual ::MyUtil::Int2IntSeqMap getFriendLists(const MyUtil::IntSeq&, const Ice::Current&);
        virtual BuddyByAddTimeNodeSeq getFriendListWithinTime(::Ice::Int, ::Ice::Long, ::Ice::Long, const Ice::Current&);
        virtual BuddyByAddTimeNodeSeq getFriendListAndTime(::Ice::Int, ::Ice::Int, const Ice::Current&);
        //add by jipeng.lin
        virtual BuddyByAddTimeNodeSeq getFriendListAndTimeWithBegin(::Ice::Int, ::Ice::Int, ::Ice::Int, const Ice::Current&);
        virtual void load( ::Ice::Int, const ::Ice::ObjectPtr&, const Ice::Current&);
        virtual void remove( ::Ice::Int, ::Ice::Int, const Ice::Current&);

        virtual bool isValid(const Ice::Current& = Ice::Current());
        virtual void setValid(bool valid,const Ice::Current& = Ice::Current());
        virtual void setData(const ::MyUtil::ObjectResultPtr& BuddyData, const Ice::Current&);

      private:
        string _endpoint;
        int _mod;
        bool _isValid;

        IceUtil::RWRecMutex _validMutex;
        friend class MyUtil::Singleton<BuddyByAddTimeCacheManagerI>;
        BuddyByAddTimeCacheManagerI(){
        }
        BuddyByAddTimeDataIPtr getBuddyByAddTimeData(Ice::Int userId);
    };

    /*****************************************************************************/

    class BuddyByAddTimeCacheFactory: virtual public MyUtil::ServantFactory {
      public:
        virtual Ice::ObjectPtr create(Ice::Int id);
    };

    /*****************************************************************************/

    class FillTask: virtual public MyUtil::Task {
      public:
        FillTask(const MyUtil::ObjectResultPtr& buddyData,::Ice::Int level=TASK_LEVEL_BUDDY_BYADDTIME_CACHE_FILL):
          MyUtil::Task(level), _buddyData(buddyData) {
          }
        virtual void handle();
      private:
        MyUtil::ObjectResultPtr _buddyData;
    };

    class LoadTask: virtual public MyUtil::Task {
      public:
        LoadTask(Ice::Int id, const Ice::ObjectPtr& obj, const std::string& times, Ice::Int level=TASK_LEVEL_BUDDY_BYADDTIME_CACHE_LOAD):
          MyUtil::Task(level), _id(id), _obj(obj), _times(times) {
          }
        virtual void handle();
      private:
        Ice::Int _id;
        Ice::ObjectPtr _obj;
        std::string _times;

    };

    class ReloadTask: virtual public MyUtil::Task {
      public:
        ReloadTask(Ice::Int id, Ice::Int level=TASK_LEVEL_BUDDY_BYADDTIME_CACHE_RELOAD):
          MyUtil::Task(level), _id(id) {
          }
        virtual void handle();
      private:
        Ice::Int _id;
    };

    class RemoveTask : virtual public MyUtil::Task {
      public:
        RemoveTask( ::Ice::Int hostId, ::Ice::Int guestId, ::Ice::Int level=TASK_LEVEL_BUDDY_BYADDTIME_CACHE_REMOVE ) :
          Task(level),_hostId(hostId),_guestId(guestId){
          }
      protected:
        virtual void handle();
      private:
        ::Ice::Int _hostId;
        ::Ice::Int _guestId;
    };

    //***************************************************************************

    class ServiceSchemaI: public MyUtil::Descriptor {
      public:
        virtual void configure(const Ice::PropertiesPtr& props);
    };

  }
}

#endif
