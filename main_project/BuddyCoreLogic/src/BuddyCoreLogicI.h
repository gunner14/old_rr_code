#ifndef __BUDDY_CORE_LOGICI_H__
#define __BUDDY_CORE_LOGICI_H__

#include <BuddyCore.h>
#include <BuddyRelationCache.h>
#include <BuddyByApplicantCache.h>
#include <IceUtil/IceUtil.h>
#include <Date.h>
#include "QueryRunner.h"
#include "ServiceI.h"
#include "Checker/share/OperaData.h"
#include <FriendRelationNotifier.h>

#define __FUNCTION_NAME__     "[" << __FUNCTION__ << ":] "

using namespace ::wap::notify;

namespace com {

  namespace xiaonei {

    namespace service {

      namespace buddy {

        const static string BUDDY_CORE_LOGIC = "BCL";

        //IM
        const int TASK_LEVEL_PRESENCESUBSCRIBE = 1;
        const int TASK_LEVEL_PRESENCESUBSCRIBED = 2;
        const int TASK_LEVEL_PRESENCEUNSUBSCRIBED = 3;
        const int TASK_LEVEL_REMOVEBUDDYNOTIFY = 4;
        const int TASK_LEVEL_ADDBUDDYONLINESTAT = 5;
        const int TASK_LEVEL_REMOVEBUDDYONLINESTAT = 6;
        const int TASK_LEVEL_RELOADBLOCKUSER = 7;

        const int TASK_LEVEL_NOTIFYBUDDYAPPLYCHANGE = 71;

        //XFeed
        const int TASK_LEVEL_DELIVERSUPPLY = 8;
        const int TASK_LEVEL_DELINSQL = 9;

        //Apply & Apply_Count
        //const int TASK_LEVEL_REBUILDBUDDYAPPLYCACHE = 10;
        //const int TASK_LEVEL_REMOVEBUDDYAPPLYCACHE = 11;
        const int TASK_LEVEL_INCUSERCOUNTGUESTREQUEST = 12;
        const int TASK_LEVEL_DECUSERCOUNTGUESTREQUEST = 13;

        //people you may know

        const int TASK_LEVEL_APPLYNEWCOMER = 15;
        const int TASK_LEVEL_ACCEPTNEWCOMER = 16;
        const int TASK_LEVEL_APPLYOFFER = 17;
        const int TASK_LEVEL_ACCEPTOFFER = 18;
        const int TASK_LEVEL_REMOVEMSNFINDER = 19;
        const int TASK_LEVEL_REMOVEFRIENDFINDER = 20;
        const int TASK_LEVEL_REBUILDFRIENDFINDER = 21;

        //Recommend
        const int TASK_LEVEL_REMOVERECOMMENDFRIEND = 22;

        //FriendDist
        const int TASK_LEVEL_REBUILDFRIENDDIST = 23;

        //BuddyGroup
        const int TASK_LEVEL_REMOVEBUDDYGROUP = 24;

        //New Buddy
        const int TASK_LEVEL_ADDBUDDYBYIDLOADER = 28;
        const int TASK_LEVEL_REMOVEBUDDYBYIDCACHE = 29;
        const int TASK_LEVEL_REMOVEBUDDYBYNAMECACHE = 30;
        const int TASK_LEVEL_REMOVEBUDDYBYONLINETIMECACHE = 31;
        const int TASK_LEVEL_REMOVEBUDDYBYADDTIMECACHE = 32;
        const int TASK_LEVEL_DECFRIENDCOUNTCACHE = 33;
        const int TASK_LEVEL_RELOADBUDDYBYIDLOADER = 34;
        const int TASK_LEVEL_NOTIFYANTISPAMADDAPPLY = 35;

        const int TASK_LEVEL_NOTIFYFRIENDOFFRIENDSEARCH = 36;

        const int TASK_LEVEL_ADDBUDDYBYAPPLICANTCACHE = 37;
        const int TASK_LEVEL_REMOVEBUDDYBYAPPLICANTCACHE = 38;

        const int TASK_LEVEL_SETBUDDYRELATIONCACHE_APPLY = 39;
        const int TASK_LEVEL_SETBUDDYRELATIONCACHE_FROM_TO = 40;
        const int TASK_LEVEL_SETBUDDYRELATIONCACHE_BLOCK = 41;


        const int TASK_LEVEL_SETBUDDYRELATIONCACHE_ACCEPTAPPLY = 42;
        const int TASK_LEVEL_SETBUDDYRELATIONCACHE_DENYAPPLY = 43;
        const int TASK_LEVEL_SETBUDDYRELATIONCACHE_REMOVEBLOCK = 44;

        const int TASK_LEVEL_REMOVECARDRELATIONCACHE_FRIEND = 45;
        const int TASK_LEVEL_CHANGEBUDDYNOTIFYFEED = 46;

        const int TASK_LEVEL_CHECKADDBUDDY = 47;

        const int TASK_LEVEL_REMOVECOMMOMFRIEND = 48;

        const int TASK_LEVEL_CHECKREMOVEBUDDY = 49;
        const int TASK_LEVEL_CHECKADDAPPLY = 50;
        const int TASK_LEVEL_CHECKREMOVEAPPLY = 51;
        const int TASK_LEVEL_SOCALGRAPHLOGICPEPORT = 52;
        const int TASK_LEVEL_BUDDYAPPLYNOTIFYFEED = 53;

        const int TASK_LEVEL_ADDBUDDYFRIENDCACHE = 54;
        const int TASK_LEVEL_REMOVEBUDDYFRIENDCACHE = 55;

        const int TASK_LEVEL_ADDBUDDYAPPLYNOTIFYRELOADER = 56;
        const int TASK_LEVEL_REMOVEBUDDYAPPLYNOTIFYRELOADER = 57;
        
        const int TASK_LEVEL_BECOMEFRIENDNOTIFYBYIDRELOADER = 58;
        const int TASK_LEVEL_REMOVEFRIENDNOTIFYBYIDRELOADER = 59;

        const int TASK_LEVEL_XOAADDAPPLYTASK = 60;
        const int TASK_LEVEL_XOAACCEPTAPPLYTASK = 61;
        const int TASK_LEVEL_XOADENYAPPLYTASK = 62;
        const int TASK_LEVEL_XOAADDFRIENDTASK = 63;
        const int TASK_LEVEL_XOAREMOVEFRIENDTASK = 64;
        const int TASK_LEVEL_XOAADDBLOCKTASK = 65;
        const int TASK_LEVEL_XOAREMOVEBLOCKTASK = 66;
        const int TASK_LEVEL_XOARELOADTASK = 67;

        const int TASK_LEVEL_REBUILDBUDDYAPPLYCACHEN = 69;//tmp
        const int TASK_LEVEL_REMOVEBUDDYAPPLYCACHEN = 70;//tmp

        const int TASK_LEVEL_REMOVERCDCACHETASK = 68;
        const int TASK_LEVEL_BECOMEFRIENDSTASK = 71;
        const int TASK_LEVEL_BLOCKTASK = 72;
//        const int TASK_LEVEL_BUDDY_RELATION_TRIPOD_TASK = 73;

        const int TASK_LEVEL_FRIEND_RELATION_NOTIFY_TASK = 74;

        //  通知SocialGraphCoreLogicAdapter的任务
        const int TASK_LEVEL_SOCIAL_GRAPH_CORE_LOGIC_REPORT_TASK = 75;

        // 变更关系的操作代码
        enum OperationType {
            ADDAPPLY = 1,
            ACCEPTAPPLY,
            DENYAPPLY,
            REMOVEFRIEND,
            ADDBLOCK,
            REMOVEBLOCK
        };

        std::string getOperationStr(OperationType type){
          switch(type){
          case ADDAPPLY:
            return "AddApply";
          case ACCEPTAPPLY:
            return "AcceptApply";
          case DENYAPPLY:
            return "DenyApply";
          case REMOVEFRIEND:
            return "RemoveApply";
          case ADDBLOCK:
            return "AddBlock";
          case REMOVEBLOCK:
            return "RemoveBlock";
          default:
            return "Unknown";
          }
        }

  
        //69,70已使用
          
        /* @brief	接口总类
         */
        class BuddyCoreLogicI : virtual public BuddyCoreLogic {
          public:

            virtual void addApply(const Applyship& apply, const Ice::Current&);

            virtual void acceptApply(const Applyship& apply, const Ice::Current&);

            virtual void denyApply(const Applyship& apply, const Ice::Current&);

            virtual void removeFriend(const Friendship& buddy, const Ice::Current&);

            virtual void addBlock(const Blockship& block, const Ice::Current&);

            virtual void removeBlock(const Blockship& block, const Ice::Current&);

            virtual void addFriend(const Ice::Int from, const Ice::Int to,
                const Ice::Current&);

            virtual void reload(const Ice::Int userId, const Ice::Current&);
        };

        // ------------------------------------------------
        class DelInSqlTask : public MyUtil::Task {
           //从数据库中的relation_feed_high和relation_feed_low表中删除相关数据
          public:
            DelInSqlTask(const string& source, int from, int to, ::Ice::Int level=TASK_LEVEL_DELINSQL ):
              MyUtil::Task(level),
              source_(source),
              from_(from),
              to_(to)  {
              }
            virtual void handle();
          private:
            string source_;
            int from_;
            int to_;
        };

        // -------------------------------------------------
        class PresenceSubscribeTask : public MyUtil::Task {
            //通知人人桌面更新相关数据
          public:
            PresenceSubscribeTask(const string& id, int from, int to, const string& status, ::Ice::Int level=TASK_LEVEL_PRESENCESUBSCRIBE ):
              MyUtil::Task(level),
              id_(id),
              from_(from),
              to_(to),
              status_(status) {
              }
            virtual void handle();
          private:
            string id_;
            int from_;
            int to_;
            string status_;
        };

        class PresenceSubscribeWithPostscriptTask : public MyUtil::Task {
            //通知人人桌面更新相关用户数据
         public:
          PresenceSubscribeWithPostscriptTask(const std::string& id, int from, int to, 
                                              const std::string& status, const std::string& postscript,
                                              Ice::Int level = TASK_LEVEL_PRESENCESUBSCRIBE) : 
            MyUtil::Task(level),
            id_(id),
            from_(from),
            to_(to),
            status_(status),
            postscript_(postscript) {

          }
          virtual void handle();
         private:
          std::string id_;
          int from_;
          int to_;
          std::string status_;
          std::string postscript_;
        };

        class PresenceSubscribedTask : public MyUtil::Task {
            //通知人人桌面更新相关数据
          public:
            PresenceSubscribedTask(const string& id, int from, int to, ::Ice::Int level=TASK_LEVEL_PRESENCESUBSCRIBED ):
              MyUtil::Task(level),
              id_(id),
              from_(from),
              to_(to) {
              }
            virtual void handle();
          private:
            string id_;
            int from_;
            int to_;
        };

        class PresenceUnsubscribedTask : public MyUtil::Task {
            //通知人人桌面更新相关数据
          public:
            PresenceUnsubscribedTask(const string& id, int from, int to, ::Ice::Int level=TASK_LEVEL_PRESENCEUNSUBSCRIBED ):
              MyUtil::Task(level),
              id_(id),
              from_(from),
              to_(to) {
              }
            virtual void handle();
          private:
            string id_;
            int from_;
            int to_;
        };

        class RebuildBuddyApplyCacheNTask : public MyUtil::Task {
            //在被申请者的BuddyApplyCache中添加申请者的信息
          public:
            RebuildBuddyApplyCacheNTask(int id, const Ice::Context& ctx, ::Ice::Int level=TASK_LEVEL_REBUILDBUDDYAPPLYCACHEN ) :
              MyUtil::Task(level),
              id_(id),ctx_(ctx) {
              }
            virtual void handle();
          private:
            int id_;    //accepter id
            Ice::Context ctx_;
        };

        class AddBuddyApplyNotifyReloaderTask : public MyUtil::Task {
            //在被申请者的DistBuddyApplyCache中添加申请信息
          public:
            AddBuddyApplyNotifyReloaderTask(int id, const Ice::Context& ctx, ::Ice::Int level=TASK_LEVEL_ADDBUDDYAPPLYNOTIFYRELOADER ) :
              MyUtil::Task(level),
              id_(id),ctx_(ctx) {
              }
            virtual void handle();
          private:
            int id_;
            Ice::Context ctx_;
        };

        class RemoveBuddyApplyCacheNTask : public MyUtil::Task {
            //在被申请者的BuddyApplyCache中删除申请者的信息
          public:
            RemoveBuddyApplyCacheNTask(int id, int applicant, ::Ice::Int level=TASK_LEVEL_REMOVEBUDDYAPPLYCACHEN ) :
              MyUtil::Task(level),
              id_(id),
              applicant_(applicant) {
              }
            virtual void handle();
          private:
            int id_;    //accepter id
            int applicant_;
        };

        class RemoveBuddyApplyNotifyReloaderTask : public MyUtil::Task {
            //在被申请者的DistBuddyApplyCache中删除申请者的信息
          public:
            RemoveBuddyApplyNotifyReloaderTask(int id, int applicant, ::Ice::Int level=TASK_LEVEL_REMOVEBUDDYAPPLYNOTIFYRELOADER ) :
              MyUtil::Task(level),
              id_(id),
              applicant_(applicant) {
              }
            virtual void handle();
          private:
            int id_;
            int applicant_;
        };

        class BecomeFriendNotifyByIdReloaderTask : public MyUtil::Task {
            //在DistBuddyByIdCache将guest插入host的好友列表中，保持好友列表递增有序
          public:
            BecomeFriendNotifyByIdReloaderTask(int host, int guest, ::Ice::Int level=TASK_LEVEL_BECOMEFRIENDNOTIFYBYIDRELOADER ) :
              MyUtil::Task(level),
              host_(host),guest_(guest) {
              }
            virtual void handle();
          private:
            int host_;
            int guest_;
        };

        class RemoveFriendNotifyByIdReloaderTask : public MyUtil::Task {
            //在DistBuddyByIdCache的host好友中删除guest
          public:
            RemoveFriendNotifyByIdReloaderTask(int host, int guest, ::Ice::Int level=TASK_LEVEL_REMOVEFRIENDNOTIFYBYIDRELOADER ) :
              MyUtil::Task(level),
              host_(host),guest_(guest) {
              } 
            virtual void handle(); 
          private:
            int host_;
            int guest_;
        };
 
        class AddBuddyByApplicantCacheTask : public MyUtil::Task {
            //在申请者的申请列表中添加被申请者的信息
          public:
            AddBuddyByApplicantCacheTask(const ::com::xiaonei::service::buddy::Applyship& apply, const Ice::Context& ctx, ::Ice::Int level=TASK_LEVEL_ADDBUDDYBYAPPLICANTCACHE) :
              MyUtil::Task(level) {
                id_ = apply.applicant;
                accepter_.accepter = apply.accepter;
                MyUtil::Date date(const_cast<Ice::Context&>(ctx)["TIME"]);
                accepter_.time = date.toSeconds();

              }
            virtual void handle();
          private:
            int id_;
            ::xce::buddy::AccepterInfo accepter_;
        };

        class RemoveBuddyByApplicantCacheTask : public MyUtil::Task {
            //在申请者的申请列表中删除被申请者的信息
          public:
            RemoveBuddyByApplicantCacheTask(int id, int accepterId, ::Ice::Int level=TASK_LEVEL_REMOVEBUDDYBYAPPLICANTCACHE ) :
              MyUtil::Task(level),
              id_(id),
              accepterId_(accepterId) {
              }
            virtual void handle();
          private:
            int id_;
            int accepterId_;
        };

        class RemoveBuddyNotifyTask : public MyUtil::Task {
            //删除好友时通知人人桌面更新相关数据
          public:
            RemoveBuddyNotifyTask(int from, int to, ::Ice::Int level=TASK_LEVEL_REMOVEBUDDYNOTIFY ) :
              MyUtil::Task(level),
              from_(from),
              to_(to) {
              }
            virtual void handle();
          private:
            int from_;
            int to_;
        };

        // --------------------------------------------
        class IncUserCountGuestRequestTask : public MyUtil::Task {
            //!! Not used
          public:
            IncUserCountGuestRequestTask(int id, ::Ice::Int level=TASK_LEVEL_INCUSERCOUNTGUESTREQUEST ):
              MyUtil::Task(level),
              id_(id) {
              }
            virtual void handle();
          private:
            int id_;
        };

        class DecUserCountGuestRequestTask : public MyUtil::Task {
            //!! Not used
          public:
            DecUserCountGuestRequestTask(int id, ::Ice::Int level=TASK_LEVEL_DECUSERCOUNTGUESTREQUEST ):
              MyUtil::Task(level),
              id_(id) {
              }
            virtual void handle();
          private:
            int id_;
        };

        // ---------------------------------------

        class ApplyOfferTask : public MyUtil::Task {
            //在申请者的guide推荐好友中删除被申请者
          public:
            ApplyOfferTask(int applicant, int acceptor, ::Ice::Int level=TASK_LEVEL_APPLYOFFER ):
              MyUtil::Task(level),
              applicant_(applicant),
              acceptor_(acceptor) {
              }
            virtual void handle();
          private:
            int applicant_;
            int acceptor_;
        };

        class AcceptOfferTask : public MyUtil::Task {
            //在被申请者的guide推荐好友中删除申请者
          public:
            AcceptOfferTask(int acceptor, int applicant, ::Ice::Int level=TASK_LEVEL_ACCEPTOFFER ):
              MyUtil::Task(level),
              acceptor_(acceptor),
              applicant_(applicant) {
              }
            virtual void handle();
          private:
            int acceptor_;
            int applicant_;
        };

        class ApplyFriendFinderNewComerTask : public MyUtil::Task {
            //在申请者的推荐新加入好友中删除被申请者
          public:
            ApplyFriendFinderNewComerTask(int applicant, int acceptor, ::Ice::Int level=TASK_LEVEL_APPLYOFFER ):
              MyUtil::Task(level),
              applicant_(applicant),
              acceptor_(acceptor) {
              }
          private:
            virtual void handle();
            int applicant_;
            int acceptor_;
        };

        class AcceptFriendFinderNewComerTask : public MyUtil::Task {
            //在被申请者的推荐新加入好友中删除申请者
          public:
            AcceptFriendFinderNewComerTask(int acceptor, int applicant, ::Ice::Int level=TASK_LEVEL_ACCEPTOFFER ):
              MyUtil::Task(level),
              acceptor_(acceptor),
              applicant_(applicant) {
              }
            virtual void handle();
          private:
            int acceptor_;
            int applicant_;
        };

        class RemoveMSNFinderTask : public MyUtil::Task {
            //在userId的MSN推荐好友中删除friendId
          public:
            RemoveMSNFinderTask(int userId, int friendId, ::Ice::Int level=TASK_LEVEL_REMOVEMSNFINDER ):
              MyUtil::Task(level),
              userId_(userId),
              friendId_(friendId) {
              }
            virtual void handle();
          private:
            int userId_;
            int friendId_;
        };

        class RemoveFriendFinderTask : public MyUtil::Task {
            //!! Not used
          public:
            RemoveFriendFinderTask(int userId, int friendId, ::Ice::Int level=TASK_LEVEL_REMOVEFRIENDFINDER ):
              MyUtil::Task(level),
              userId_(userId),
              friendId_(friendId) {
              }
            virtual void handle();
          private:
            int userId_;
            int friendId_;
        };

        class RemoveCommonFriendTask : public MyUtil::Task {
            //TODO 没找到对应的servent类
          public:
            RemoveCommonFriendTask(int userId, int friendId, ::Ice::Int level=TASK_LEVEL_REMOVECOMMOMFRIEND ):
              MyUtil::Task(level),
              userId_(userId),
              friendId_(friendId) {
              }
            virtual void handle();
          private:
            int userId_;
            int friendId_;
        };

        class RebuildFriendFinderTask : public MyUtil::Task {
            //!! Not used
          public:
            RebuildFriendFinderTask(int userId, ::Ice::Int level=TASK_LEVEL_REBUILDFRIENDFINDER ):
              MyUtil::Task(level),
              userId_(userId) {
              }
            virtual void handle();
          private:
            int userId_;
        };

        // ---------------------------------------
        class AddBuddyOnlineStatTask : public MyUtil::Task {
            // 相互更新在线状态
          public:
            AddBuddyOnlineStatTask(int host, int guest, ::Ice::Int level=TASK_LEVEL_ADDBUDDYONLINESTAT ) :
              MyUtil::Task(level),
              host_(host),
              guest_(guest) {
              }
            virtual void handle();
          private:
            int host_;
            int guest_;

        };

        class RemoveBuddyOnlineStatTask : public MyUtil::Task {
            //删除好友时更新好友在线状态数据
          public:
            RemoveBuddyOnlineStatTask(int host, int guest, ::Ice::Int level=TASK_LEVEL_REMOVEBUDDYONLINESTAT ) :
              MyUtil::Task(level),
              host_(host),
              guest_(guest) {
              }
            virtual void handle();
          private:
            int host_;
            int guest_;

        };

        // ---------------------------------------
        class DeliverSupplyTask : public MyUtil::Task {
            // 相互添加对方的新鲜事（日志，照片，状态，分享）
          public:
            DeliverSupplyTask(int supplier, int accepter, const Ice::Context& ctx, ::Ice::Int level=TASK_LEVEL_DELIVERSUPPLY ):
              MyUtil::Task(level),
              supplier_(supplier),
              accepter_(accepter),
              ctx_(ctx) {
              }
            virtual void handle();
          private:
            int supplier_;
            int accepter_;
            Ice::Context ctx_;
        };

        // ---------------------------------------
        class RemoveBuddyGroupTask : public MyUtil::Task {
            //在被删除者的好友分组中删除删除者
          public:
            RemoveBuddyGroupTask(int from, int to, ::Ice::Int level=TASK_LEVEL_REMOVEBUDDYGROUP ) :
              MyUtil::Task(level),
              from_(from),
              to_(to) {
              }
            virtual void handle();
          private:
            int from_;
            int to_;
        };

        // ---------------------------------------
        class ReloadBlockUserTask : public MyUtil::Task {
            //在设置黑名单时通知人人桌面更新用户数据
          public:
            ReloadBlockUserTask(int id, ::Ice::Int level=TASK_LEVEL_RELOADBLOCKUSER ):
              MyUtil::Task(level),
              id_(id) {
              }
            virtual void handle();
          private:
            int id_;
        };

        // ---------------------------------------
        class RemoveRecommendFriendTask : public MyUtil::Task {
            //在ownerId的二度推荐好友中删除otherId
          public:
            RemoveRecommendFriendTask(int ownerId, int otherId, ::Ice::Int level=TASK_LEVEL_REMOVERECOMMENDFRIEND ):
              MyUtil::Task(level),
              ownerId_(ownerId),otherId_(otherId) {
              }
            virtual void handle();
          private:
            int ownerId_;
            int otherId_;
        };

        // ---------------------------------------
        class RebuildFriendDistTask : public MyUtil::Task {
            //!! Not used
          public:
            RebuildFriendDistTask(int id, ::Ice::Int level=TASK_LEVEL_REBUILDFRIENDDIST ):
              MyUtil::Task(level),
              id_(id) {
              }
            virtual void handle();
          private:
            int id_;
        };

        // ---------------------------------------

        class AddBuddyByIdLoaderTask : public MyUtil::Task {
            //在BuddyByIdCache中添加双向好友关系
          public:
            AddBuddyByIdLoaderTask (int userid,int friendid, ::Ice::Int level=TASK_LEVEL_ADDBUDDYBYIDLOADER ):
              MyUtil::Task(level),
              userid_(userid),
              friendid_(friendid){
              }
            virtual void handle();
          private:
            int userid_;
            int friendid_;
        };
        class RemoveBuddyByIdCacheTask : public MyUtil::Task {
            //在BuddyByIdCache中删除双向好友关系
          public:
            RemoveBuddyByIdCacheTask (int userid,int friendid, ::Ice::Int level=TASK_LEVEL_REMOVEBUDDYBYIDCACHE ):
              MyUtil::Task(level),
              userid_(userid),
              friendid_(friendid){
              }
            virtual void handle();
          private:
            int userid_;
            int friendid_;
        };

        class AddBuddyFriendCacheTask : public MyUtil::Task {
            //在BuddyFriendCache中添加双向好友关系
          public:
            AddBuddyFriendCacheTask (int userid,int friendid, ::Ice::Int level= TASK_LEVEL_ADDBUDDYFRIENDCACHE):
              MyUtil::Task(level),
              userid_(userid),
              friendid_(friendid){
              }
            virtual void handle();
          private:
            int userid_;
            int friendid_;
        };


        class RemoveBuddyFriendCacheTask : public MyUtil::Task {
            //在BuddyFriendCache中删除双向好友关系
          public:
            RemoveBuddyFriendCacheTask (int userid,int friendid, ::Ice::Int level=TASK_LEVEL_REMOVEBUDDYFRIENDCACHE ):
              MyUtil::Task(level),
              userid_(userid),
              friendid_(friendid){
              }
            virtual void handle();
          private:
            int userid_;
            int friendid_;
        };

        class RemoveBuddyByNameCacheTask : public MyUtil::Task {
            //在BuddyByNameCache中删除双向好友关系
          public:
            RemoveBuddyByNameCacheTask (int userid,int friendid, ::Ice::Int level=TASK_LEVEL_REMOVEBUDDYBYNAMECACHE ):
              MyUtil::Task(level),
              userid_(userid),
              friendid_(friendid){
              }
            virtual void handle();
          private:
            int userid_;
            int friendid_;
        };
        class RemoveBuddyByOnlineTimeCacheTask : public MyUtil::Task {
            //在BuddyByOnlineTimeCache中删除双向好友关系
          public:
            RemoveBuddyByOnlineTimeCacheTask (int userid,int friendid, ::Ice::Int level=TASK_LEVEL_REMOVEBUDDYBYONLINETIMECACHE ):
              MyUtil::Task(level),
              userid_(userid),
              friendid_(friendid){
              }
            virtual void handle();
          private:
            int userid_;
            int friendid_;
        };
        class RemoveBuddyByAddTimeCacheTask : public MyUtil::Task {
            //在BuddyByAddTimeCache中删除双向好友关系
          public:
            RemoveBuddyByAddTimeCacheTask (int userid,int friendid, ::Ice::Int level=TASK_LEVEL_REMOVEBUDDYBYADDTIMECACHE ):
              MyUtil::Task(level),
              userid_(userid),
              friendid_(friendid){
              }
            virtual void handle();
          private:
            int userid_;
            int friendid_;
        };
        class DecFriendCountCacheTask : public MyUtil::Task {
            //在FriendCountCache中将删除人的好友数减1
          public:
            DecFriendCountCacheTask (int userid, ::Ice::Int level=TASK_LEVEL_DECFRIENDCOUNTCACHE ):
              MyUtil::Task(level),
              userid_(userid){
              }
            virtual void handle();
          private:
            int userid_;
        };
        class ReloadBuddyByIdLoaderTask : public MyUtil::Task {
            // 在BuddyByIdCache中数据库重新加载id用户的好友列表
          public:
            ReloadBuddyByIdLoaderTask (int id, ::Ice::Int level=TASK_LEVEL_RELOADBUDDYBYIDLOADER ):
              MyUtil::Task(level),
              id_(id) {
              }
            virtual void handle();
          private:
            int id_;
        };

        class NotifyBuddyApplyChangeTask : public MyUtil::Task {
            //发送好友申请时通知人人桌面更新相关数据
          public:
            NotifyBuddyApplyChangeTask(int id, ::Ice::Int level=TASK_LEVEL_NOTIFYBUDDYAPPLYCHANGE ):
              MyUtil::Task(level),
              id_(id) {
              }
            virtual void handle();
          private:
            int id_;    //accepter id
        };

        class NotifyAntispamAddApplyTask : public MyUtil::Task {
            //通知安全服务
          public:
            NotifyAntispamAddApplyTask(int applicant, int accepter, const Ice::Context& ctx,::Ice::Int level=TASK_LEVEL_NOTIFYANTISPAMADDAPPLY ):
              MyUtil::Task(level),
              applicant_(applicant),
              accepter_(accepter),
              ctx_(ctx) {
              }
            virtual void handle();
          private:
            int applicant_;
            int accepter_;
            Ice::Context ctx_;
        };


        class SetBuddyRelationCacheTask : public MyUtil::Task {
            //设置单向关系，通过desc具体关系
          public:
            //设置单向申请关系，通过desc表示from对to的关系
            SetBuddyRelationCacheTask(const Applyship& apply, ::xce::buddy::BuddyDesc desc, ::Ice::Int level=TASK_LEVEL_SETBUDDYRELATIONCACHE_APPLY) : MyUtil::Task(level) {
              relation_.from = apply.applicant;
              relation_.to = apply.accepter;
              desc_ = desc;
            }

            //设置单向好友关系，通过desc表示from对to的关系
            SetBuddyRelationCacheTask(const Ice::Int from, const Ice::Int to,  ::xce::buddy::BuddyDesc desc, ::Ice::Int level=TASK_LEVEL_SETBUDDYRELATIONCACHE_FROM_TO) : MyUtil::Task(level) {
              relation_.from = from;
              relation_.to = to;
              desc_ = desc;
            }

            //设置单向黑名单关系，通过desc表示from对to的关系
            SetBuddyRelationCacheTask(const Blockship& block, ::xce::buddy::BuddyDesc desc, ::Ice::Int level=TASK_LEVEL_SETBUDDYRELATIONCACHE_BLOCK) : MyUtil::Task(level) {
              relation_.from = block.from;
              relation_.to = block.to;
              desc_ = desc;
            }

            virtual void handle();
          private:
            ::xce::buddy::Relationship relation_;
            ::xce::buddy::BuddyDesc desc_;
        };

        class RemoveCardRelationCacheTask : public MyUtil::Task {
           //移除两个用户的名片交换信息
          public:
            RemoveCardRelationCacheTask(int host, int guest, ::Ice::Int level=TASK_LEVEL_REMOVECARDRELATIONCACHE_FRIEND ):
              MyUtil::Task(level),
              host_(host),
              guest_(guest) {
              }
            virtual void handle();
          private:
            int host_;
            int guest_;
        };

        class ChangeBuddyNotifyFeedTask : public MyUtil::Task {
            // 更新双方的新鲜事接受好友列表
          public:
            ChangeBuddyNotifyFeedTask(int host, int guest, ::Ice::Int level=TASK_LEVEL_CHANGEBUDDYNOTIFYFEED ):
              MyUtil::Task(level),
              host_(host),
              guest_(guest) {
              }
            virtual void handle();
          private:
            int host_;
            int guest_;
        };

        class BuddyApplyNotifyFeedTask : public MyUtil::Task {
            //申请时将被申请者的一部分新鲜事发送给申请者（日志，照片，状态，分享)
          public:
            BuddyApplyNotifyFeedTask(int host, int guest, ::Ice::Int level=TASK_LEVEL_BUDDYAPPLYNOTIFYFEED ):
              MyUtil::Task(level),
              host_(host),
              guest_(guest) {
              }
            virtual void handle();
          private:
            int host_;
            int guest_;
        };

        class CheckAddBuddyTask : public MyUtil::Task {
            //添加好友后通知Checer进行检查
          public:
            CheckAddBuddyTask(const ::xce::check::OperaDataPtr& key, const std::string& value, ::Ice::Int level=TASK_LEVEL_CHECKADDBUDDY ):
              MyUtil::Task(level),
              key_(key),
              value_(value) {
              }
            virtual void handle();
          private:
            const ::xce::check::OperaDataPtr key_;
            const std::string value_;
        };

        class CheckRemoveBuddyTask : public MyUtil::Task {
            //删除好友后通知Checer进行检查
          public:
            CheckRemoveBuddyTask(const ::xce::check::OperaDataPtr& key, const std::string& value, ::Ice::Int level=TASK_LEVEL_CHECKREMOVEBUDDY ):
              MyUtil::Task(level),
              key_(key),
              value_(value) {
              }
            virtual void handle();
          private:
            const ::xce::check::OperaDataPtr key_;
            const std::string value_;
        };


        class CheckAddApplyTask : public MyUtil::Task {
            //添加申请后通知Checer进行检查
          public:
            CheckAddApplyTask(const Applyship& apply,  ::Ice::Int level=TASK_LEVEL_CHECKADDAPPLY ):
              MyUtil::Task(level),
              apply_(apply) {
              }
            virtual void handle();
          private:
            Applyship apply_;
        };


        class CheckRemoveApplyTask : public MyUtil::Task {
            //!! Not used
          public:
            CheckRemoveApplyTask(const Applyship& apply,  ::Ice::Int level=TASK_LEVEL_CHECKREMOVEAPPLY ):
              MyUtil::Task(level),
              apply_(apply) {
              }
            virtual void handle();
          private:
            Applyship apply_;
        };
        
        class SocalGraphLogicReportTask : public MyUtil::Task {
            //从host的推荐好友圈子中删除guest
          public:
            SocalGraphLogicReportTask(int host, int guest, com::xiaonei::service::buddy::BuddyDesc desc, ::Ice::Int level=TASK_LEVEL_SOCALGRAPHLOGICPEPORT ):
              MyUtil::Task(level),
              host_(host),guest_(guest),desc_(desc) {
              }
            virtual void handle();
          private:
            int host_;
            int guest_;
            com::xiaonei::service::buddy::BuddyDesc desc_;
        };


        // ---------------------------------------

        // BuddyXoaLogic
        class XoaAddApplyTask : public MyUtil::Task {
            // 向BuddyXoaLogic发送添加好友申请通知
          public:
            XoaAddApplyTask(int applicant, int apply, const Ice::Context& ctx, ::Ice::Int level = TASK_LEVEL_XOAADDAPPLYTASK ):
              MyUtil::Task(level),
              applicant_(applicant),
              apply_(apply),
              ctx_(ctx){
              }
            virtual void handle();
          private:
            int applicant_;
            int apply_;
            Ice::Context ctx_;
        };

        class XoaAcceptApplyTask : public MyUtil::Task {
            // 向BuddyXoaLogic发送接受好友申请通知
          public:
        	XoaAcceptApplyTask(int applicant, int apply, const Ice::Context& ctx, ::Ice::Int level = TASK_LEVEL_XOAACCEPTAPPLYTASK ):
              MyUtil::Task(level),
              applicant_(applicant),
              apply_(apply),
              ctx_(ctx) {
              }
            virtual void handle();
          private:
            int applicant_;
            int apply_;
            Ice::Context ctx_;
        };

        class XoaDenyApplyTask : public MyUtil::Task {
            // 向BuddyXoaLogic发送拒绝好友申请通知
          public:
        	XoaDenyApplyTask(int applicant, int apply, const Ice::Context& ctx, ::Ice::Int level = TASK_LEVEL_XOADENYAPPLYTASK ):
              MyUtil::Task(level),
              applicant_(applicant),
              apply_(apply),
              ctx_(ctx) {
              }
            virtual void handle();
          private:
            int applicant_;
            int apply_;
            Ice::Context ctx_;
        };

        class XoaAddFriendTask : public MyUtil::Task {
            // 向BuddyXoaLogic发送添加好友通知
          public:
        	XoaAddFriendTask(int from, int to, const Ice::Context& ctx, ::Ice::Int level = TASK_LEVEL_XOAADDFRIENDTASK ):
              MyUtil::Task(level),
              from_(from),
              to_(to),
              ctx_(ctx) {
              }
            virtual void handle();
          private:
            int from_;
            int to_;
            Ice::Context ctx_;
        };

        class XoaRemoveFriendTask : public MyUtil::Task {
            // 向BuddyXoaLogic发送删除好友通知
          public:
        	XoaRemoveFriendTask(int from, int to, const Ice::Context& ctx, ::Ice::Int level = TASK_LEVEL_XOAREMOVEFRIENDTASK ):
              MyUtil::Task(level),
              from_(from),
              to_(to),
              ctx_(ctx) {
              }
            virtual void handle();
          private:
            int from_;
            int to_;
            Ice::Context ctx_;
        };

        class XoaAddBlockTask : public MyUtil::Task {
            // 向BuddyXoaLogic发送添加黑名单通知
          public:
        	XoaAddBlockTask(int block, int blocked, const Ice::Context& ctx, ::Ice::Int level = TASK_LEVEL_XOAADDBLOCKTASK ):
              MyUtil::Task(level),
              block_(block),
              blocked_(blocked),
              ctx_(ctx) {
              }
            virtual void handle();
          private:
            int block_;
            int blocked_;
            Ice::Context ctx_;
        };

        class XoaRemoveBlockTask : public MyUtil::Task {
            // 向BuddyXoaLogic发送删除黑名单通知
          public:
        	XoaRemoveBlockTask(int block, int blocked, const Ice::Context& ctx, ::Ice::Int level = TASK_LEVEL_XOAREMOVEBLOCKTASK ):
              MyUtil::Task(level),
              block_(block),
              blocked_(blocked),
              ctx_(ctx) {
              }
            virtual void handle();
          private:
            int block_;
            int blocked_;
            Ice::Context ctx_;
        };
  
        class XoaReloadTask : public MyUtil::Task {
            // 向BuddyXoaLogic发送Reload通知
          public:
          XoaReloadTask(int userId, const Ice::Context& ctx, ::Ice::Int level = TASK_LEVEL_XOARELOADTASK ):
              MyUtil::Task(level),
              userId_(userId),
              ctx_(ctx) {
              }
            virtual void handle();
          private:
            int userId_;
            Ice::Context ctx_;
        };

        class RemoveRcdCacheTask : public MyUtil::Task {
            //删除host的Business Cache中guest的数据
          public:
          RemoveRcdCacheTask(int host, int guest, const Ice::Context& ctx, ::Ice::Int level = TASK_LEVEL_REMOVERCDCACHETASK ):
              MyUtil::Task(level),
              host_(host),
              guest_(guest),
              ctx_(ctx) {
              }
            virtual void handle();
          private:
            int host_;
            int guest_;
            Ice::Context ctx_;
        };

        class BecomeFriendsTask : public MyUtil::Task {
            //删除各自Business Cache中对方的数据
          public:
          BecomeFriendsTask(int host, int guest, const Ice::Context& ctx, ::Ice::Int level = TASK_LEVEL_BECOMEFRIENDSTASK ):
              MyUtil::Task(level),
              host_(host),
              guest_(guest),
              ctx_(ctx) {
              }
            virtual void handle();
          private:
            int host_;
            int guest_;
            Ice::Context ctx_;
        };

        class BlockTask : public MyUtil::Task {
            //host对guest添加黑名单，并删除host的Business Cache中的guest数据
          public:
          BlockTask(int host, int guest, const Ice::Context& ctx, ::Ice::Int level = TASK_LEVEL_BLOCKTASK ):
              MyUtil::Task(level),
              host_(host),
              guest_(guest),
              ctx_(ctx) {
              }
            virtual void handle();
          private:
            int host_;
            int guest_;
            Ice::Context ctx_;
        };
    

//关系变更时通知3G 的FriendRelation
class FriendRelationNotifyTask : public MyUtil::Task {

 public:
  FriendRelationNotifyTask(FriendRelationType type, int from, int target, const Ice::Context& ctx, ::Ice::Int level = TASK_LEVEL_FRIEND_RELATION_NOTIFY_TASK) :
    MyUtil::Task(level),
    type_(type),
    from_(from),
    target_(target),
    ctx_(ctx) {}
  virtual void handle();
 private:
  FriendRelationType type_;
  int from_;
  int target_;
  Ice::Context ctx_;
};

//关系变更时通知SocialGraphCoreLogic
class SocialGraphCoreLogicReportTask : public MyUtil::Task {

 public:
  SocialGraphCoreLogicReportTask(OperationType type, int from, int to, ::Ice::Int level = TASK_LEVEL_SOCIAL_GRAPH_CORE_LOGIC_REPORT_TASK) :
    MyUtil::Task(level),
    type_(type),
    from_(from),
    to_(to) {}
  virtual void handle();
 private:
  OperationType type_;
  int from_;
  int to_;
};

        // ---------------------------------------
      }
    }
  }
}

#endif
