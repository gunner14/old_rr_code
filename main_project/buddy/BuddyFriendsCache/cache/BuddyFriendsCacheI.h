#ifndef __BUDDY_FRIENDS_CACHE_I_H__
#define __BUDDY_FRIENDS_CACHE_I_H__

#include "BuddyFriendsCache.h"
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <QueryRunner.h>
#include <string>
#include "UtilCxx/src/Singleton.h"
#include "IceExt/src/ObjectCacheI.h"
#include "IceExt/src/ServiceI.h"

namespace xce {
  namespace buddy {

    const int BUDDY_FRIENDS_CACHE_DATA = 0;

    const int TASK_LEVEL_BUDDY_FRIENDS_CACHE_FILL = 1;
    const int TASK_LEVEL_BUDDY_FRIENDS_CACHE_LOAD = 2;
    const int TASK_LEVEL_BUDDY_FRIENDS_CACHE_RELOAD = 3;
    const int TASK_LEVEL_BUDDY_FRIENDS_CACHE_REMOVE = 4;
    const int TASK_LEVEL_BUDDY_FRIENDS_CACHE_ADD = 5;

    /*****************************************************************************/

    class BuddyFriendsDataI: virtual public Ice::Object {
      public:
        BuddyFriendsDataI();
        BuddyFriendsDataI(BuddyFriendsDataPtr);
        BuddyFriendsDataI(::MyUtil::IntSeq&);
        Ice::Int getCount();
        ::MyUtil::IntSeq getList(::Ice::Int,::Ice::Int);
        void remove(::Ice::Int);
        void add(::Ice::Int);
      private:
        ::MyUtil::IntSeq _friends;
        IceUtil::Mutex _mutex;
    };
    typedef IceUtil::Handle<BuddyFriendsDataI> BuddyFriendsDataIPtr;

    class BuddyFriendsCacheManagerI: virtual public BuddyFriendsCacheManager, virtual public MyUtil::Singleton<BuddyFriendsCacheManagerI> {
      public:
        void bootstrap(const string& endpoint, int mod);
        virtual ::Ice::Int getFriendCount(::Ice::Int, const Ice::Current&);
        virtual ::MyUtil::Int2IntMap getFriendCounts(const MyUtil::IntSeq&, const Ice::Current&);
        virtual ::MyUtil::IntSeq getFriendList(::Ice::Int, ::Ice::Int, ::Ice::Int, const Ice::Current&);
        virtual ::MyUtil::Int2IntSeqMap getFriendLists(const MyUtil::IntSeq&, const Ice::Current&);
        virtual void load( ::Ice::Int, const ::Ice::ObjectPtr&, const Ice::Current&);
        virtual void remove( ::Ice::Int, ::Ice::Int, const Ice::Current&);
        virtual void add( ::Ice::Int, ::Ice::Int, const Ice::Current&);

        virtual bool isFriendsFriend(::Ice::Int, ::Ice::Int, const Ice::Current&);

        virtual MyUtil::Int2IntMap isFriendsFriendBatch(int hostId, const MyUtil::IntSeq& potentialFriends,  const Ice::Current& current = Ice::Current());

        virtual ::MyUtil::IntSeq getCommonFriends( ::Ice::Int hostId, ::Ice::Int guestId, const Ice::Current& current = Ice::Current());
        
        virtual ::MyUtil::Str2IntSeqMap getCommonFriendsBatch(const ::MyUtil::Int2IntMap& idpairs, const Ice::Current& current = Ice::Current());
        
        virtual Ice::Int getCommonFriendsCount( ::Ice::Int hostId, ::Ice::Int guestId, const Ice::Current& current = Ice::Current());
        
        virtual bool isValid(const Ice::Current&);
        virtual void setValid(bool valid,const Ice::Current& = Ice::Current());
        virtual void setData(const ::MyUtil::ObjectResultPtr& BuddyData, const Ice::Current&);

      private:
        /***
         * getCommonItems of two sorted seqence, the common items will be deleted from the second seqence, here is friends2
         ***/
        MyUtil::IntSeq getCommonItems(const MyUtil::IntSeq& friends1, MyUtil::IntSeq& friends2);
        string _endpoint;
        int _mod;
        bool _isValid;

        IceUtil::RWRecMutex _validMutex;
        friend class MyUtil::Singleton<BuddyFriendsCacheManagerI>;
        BuddyFriendsCacheManagerI(){
        }
        BuddyFriendsDataIPtr getBuddyFriendsDataI(Ice::Int userId);
    };

    /*****************************************************************************/

    class BuddyFriendsCacheFactory : virtual public MyUtil::ServantFactory {
      public:
        virtual Ice::ObjectPtr create(Ice::Int id);
    };

    /*****************************************************************************/
    /*
       class OrderFriendsAscHandler : virtual public com::xiaonei::xce::ResultHandler{
       public:
       OrderFriendsAscHandler(std::map<int, int>& data,
       const std::string& keyField, const std::string& valueField ) :
       _data(data), _keyField(keyField), _valueField(valueField) {
       }
       protected:
       virtual bool handle(mysqlpp::Row& res) const;
       private:
       std::map<int, int>& _data;
       std::string _keyField;
       std::string _valueField;
       };

       class StatusFilterHandler : virtual public com::xiaonei::xce::ResultHandler{
       public:
       StatusFilterHandler(std::map<int, int>& orderFriend, ostringstream& filter_str, bool& filted, const std::string& idField, const std::string& statusField="") :
       _orderFriend(orderFriend), _filter_str(filter_str), _filted(filted), _idField(idField), _statusField(statusField) {
       }
       protected:
       virtual bool handle(mysqlpp::Row& res) const;
       private:
       std::map<int, int>& _orderFriend;
       ostringstream& _filter_str;
       bool& _filted;
       std::string _idField;
       std::string _statusField;
       };
     */
    /*****************************************************************************/

    class FillTask: virtual public MyUtil::Task {
      public:
        FillTask(const MyUtil::ObjectResultPtr& buddyData,::Ice::Int level=TASK_LEVEL_BUDDY_FRIENDS_CACHE_FILL):
          MyUtil::Task(level), _buddyData(buddyData) {
          }
        virtual void handle();
      private:
        MyUtil::ObjectResultPtr _buddyData;
    };

    class LoadTask: virtual public MyUtil::Task {
      public:
        LoadTask(Ice::Int id, const Ice::ObjectPtr& obj, Ice::Int level=TASK_LEVEL_BUDDY_FRIENDS_CACHE_LOAD):
          MyUtil::Task(level), _id(id), _obj(obj)  {
          }
        virtual void handle();
      private:
        Ice::Int _id;
        Ice::ObjectPtr _obj;

    };

    class ReloadTask: virtual public MyUtil::Task {
      public:
        ReloadTask(Ice::Int id, Ice::Int level=TASK_LEVEL_BUDDY_FRIENDS_CACHE_RELOAD):
          MyUtil::Task(level), _id(id) {
          }
        virtual void handle();
      private:
        Ice::Int _id;

    };

    class RemoveTask : virtual public MyUtil::Task {
      public:
        RemoveTask( ::Ice::Int hostId, ::Ice::Int guestId, ::Ice::Int level=TASK_LEVEL_BUDDY_FRIENDS_CACHE_REMOVE ) :
          Task(level),_hostId(hostId),_guestId(guestId){
          }
      protected:
        virtual void handle();
      private:
        ::Ice::Int _hostId;
        ::Ice::Int _guestId;
    };

    class AddTask : virtual public MyUtil::Task {
      public:
        AddTask( ::Ice::Int hostId, ::Ice::Int guestId, ::Ice::Int level=TASK_LEVEL_BUDDY_FRIENDS_CACHE_ADD ) :
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
