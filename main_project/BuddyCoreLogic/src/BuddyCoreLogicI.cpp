#include "BuddyCoreLogicI.h"
#include <boost/lexical_cast.hpp>
#include "ServiceI.h"
#include "FriendDistAdapter.h"
#include "FriendFinderReplicaAdapter.h"
#include "TalkRightAdapter.h"
#include "FeedDispatcherAdapter.h"
#include "BuddyGroupAdapter.h"
#include "OfferFriendsAdapter.h"
#include "FriendFinderNewComerWorkerAdapter.h"
#include "FriendFinderByMSNReplicaAdapter.h"
#include "RecommendFriendAdapter.h"
#include "UserCountAdapter.h"

#include "OnlineCenterAdapter.h"
#include "PresenceMsgHandlerAdapter.h"
#include "IqMsgHandlerAdapter.h"

#include "BuddyByIdReplicaAdapter.h"
#include "BuddyByNameReplicaAdapter.h"
#include "BuddyByOnlineTimeReplicaAdapter.h"
#include "BuddyByAddTimeReplicaAdapter.h"
#include "FriendCountCacheReplicaAdapter.h"
#include "BuddyRedoMonitor.h"
#include "AntispamRelationAdapter.h"
#include "BuddyByApplicantAdapter.h"
#include "BuddyRelationReplicaAdapter.h"
#include "CardRelationCacheAdapter.h"
#include "AbstractCheckerAdapterFactory.h"
#include "FriendFinderWorkerAdapter.h"
#include "SocialGraphLogicAdapter.h"
#include "BuddyFriendsAdapter.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "BuddyXoaLogicAdapter.h"
#include "SocialGraphRcdMessageServiceAdapter.h"
#include "XceLoggerAdapter.h"
#include "RelationNotifyAdapter.h"
#include "SocialGraphCoreLogicAdapter.h"

void MyUtil::initialize() {
  using namespace ::MyUtil;
  using namespace ::com::xiaonei::service::buddy;

  ServiceI& service = ServiceI::instance();
  BuddyCoreLogicI* manager = new BuddyCoreLogicI;
  service.getAdapter()->add(manager, service.createIdentity(BUDDY_CORE_LOGIC, ""));

  //设置各类别的任务线程池
  TaskManager::instance().config(TASK_LEVEL_NORMAL, ThreadPoolConfig(1, 1));

  //    --------------------------------HA--------------------------------------------------
  int mod = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("BuddyCoreLogic.Mod", 0);
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("BuddyCoreLogic.Interval", 5);
  using namespace ::xce::serverstate;
  ServerStateSubscriber::instance().initialize("ControllerBuddyCoreLogic", manager, mod, interval);

  //IM
  TaskManager::instance().config(TASK_LEVEL_PRESENCESUBSCRIBE, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_PRESENCESUBSCRIBED, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_PRESENCEUNSUBSCRIBED, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_REMOVEBUDDYNOTIFY, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_ADDBUDDYONLINESTAT, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_REMOVEBUDDYONLINESTAT, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_RELOADBLOCKUSER, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_NOTIFYBUDDYAPPLYCHANGE, ThreadPoolConfig(1, 1));

  //XFeed
  TaskManager::instance().config(TASK_LEVEL_DELIVERSUPPLY, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_DELINSQL, ThreadPoolConfig(1, 1));

  //Apply & Apply_Count
  TaskManager::instance().config(TASK_LEVEL_REBUILDBUDDYAPPLYCACHEN, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_REMOVEBUDDYAPPLYCACHEN, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_INCUSERCOUNTGUESTREQUEST, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_DECUSERCOUNTGUESTREQUEST, ThreadPoolConfig(1, 1));

  //people you may know
  TaskManager::instance().config(TASK_LEVEL_APPLYOFFER, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_ACCEPTOFFER, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_REMOVEMSNFINDER, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_REMOVEFRIENDFINDER, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_REMOVECOMMOMFRIEND, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_REBUILDFRIENDFINDER, ThreadPoolConfig(1, 1));

  //Recommend
  TaskManager::instance().config(TASK_LEVEL_REMOVERECOMMENDFRIEND, ThreadPoolConfig(1, 1));

  //FriendDist
  TaskManager::instance().config(TASK_LEVEL_REBUILDFRIENDDIST, ThreadPoolConfig(1, 1));

  //BuddyGroup
  TaskManager::instance().config(TASK_LEVEL_REMOVEBUDDYGROUP, ThreadPoolConfig(1, 1));

  //New Buddy
  TaskManager::instance().config(TASK_LEVEL_ADDBUDDYBYIDLOADER, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_REMOVEBUDDYBYIDCACHE, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_REMOVEBUDDYBYNAMECACHE, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_REMOVEBUDDYBYONLINETIMECACHE, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_REMOVEBUDDYBYADDTIMECACHE, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_DECFRIENDCOUNTCACHE, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_RELOADBUDDYBYIDLOADER, ThreadPoolConfig(1, 1));

  TaskManager::instance().config(TASK_LEVEL_NOTIFYANTISPAMADDAPPLY, ThreadPoolConfig(1, 1));

  //BuddyByApplicantCache
  TaskManager::instance().config(TASK_LEVEL_ADDBUDDYBYAPPLICANTCACHE, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_REMOVEBUDDYBYAPPLICANTCACHE, ThreadPoolConfig(1, 1));

  //BuddyRelationCache
  TaskManager::instance().config(TASK_LEVEL_SETBUDDYRELATIONCACHE_APPLY, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_SETBUDDYRELATIONCACHE_FROM_TO, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_SETBUDDYRELATIONCACHE_BLOCK, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_SETBUDDYRELATIONCACHE_ACCEPTAPPLY, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_SETBUDDYRELATIONCACHE_DENYAPPLY, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_SETBUDDYRELATIONCACHE_REMOVEBLOCK, ThreadPoolConfig(1, 1));

  TaskManager::instance().config(TASK_LEVEL_REMOVECARDRELATIONCACHE_FRIEND, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_CHANGEBUDDYNOTIFYFEED, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_CHECKADDBUDDY, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_CHECKREMOVEBUDDY, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_CHECKADDAPPLY, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_CHECKREMOVEAPPLY, ThreadPoolConfig(1, 1));

  TaskManager::instance().config(TASK_LEVEL_SOCALGRAPHLOGICPEPORT, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_BUDDYAPPLYNOTIFYFEED, ThreadPoolConfig(1, 1));

  TaskManager::instance().config(TASK_LEVEL_ADDBUDDYFRIENDCACHE, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_REMOVEBUDDYFRIENDCACHE, ThreadPoolConfig(1, 1));

  TaskManager::instance().config(TASK_LEVEL_ADDBUDDYAPPLYNOTIFYRELOADER, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_REMOVEBUDDYAPPLYNOTIFYRELOADER, ThreadPoolConfig(1, 1));

  TaskManager::instance().config(TASK_LEVEL_BECOMEFRIENDNOTIFYBYIDRELOADER, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_REMOVEFRIENDNOTIFYBYIDRELOADER, ThreadPoolConfig(1, 1));

  TaskManager::instance().config(TASK_LEVEL_XOAADDAPPLYTASK, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_XOAACCEPTAPPLYTASK, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_XOADENYAPPLYTASK, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_XOAADDFRIENDTASK, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_XOAREMOVEFRIENDTASK, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_XOAADDBLOCKTASK, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_XOAREMOVEBLOCKTASK, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_XOARELOADTASK, ThreadPoolConfig(1, 1));

  TaskManager::instance().config(TASK_LEVEL_REMOVERCDCACHETASK, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_BECOMEFRIENDSTASK, ThreadPoolConfig(1, 1));
  TaskManager::instance().config(TASK_LEVEL_BLOCKTASK, ThreadPoolConfig(1, 1));

  // 通知3G FriendRelation
  TaskManager::instance().config(TASK_LEVEL_FRIEND_RELATION_NOTIFY_TASK, ThreadPoolConfig(1, 1));

  // 通知SocialGraphCoreLogic
  TaskManager::instance().config(TASK_LEVEL_SOCIAL_GRAPH_CORE_LOGIC_REPORT_TASK, ThreadPoolConfig(1, 1));
}

namespace com {
namespace xiaonei {
namespace service {
namespace buddy {

// 添加好友申请
void BuddyCoreLogicI::addApply(const Applyship& apply, const Ice::Current& current) {
  using namespace ::MyUtil;
  MCE_INFO("addApply -> " << apply.applicant << " apply " << apply.accepter);

  //通知人人桌面更新相关数据
  TaskManager::instance().execute(new PresenceSubscribeWithPostscriptTask(
      const_cast<Ice::Context&> (current.ctx)["id"], apply.applicant, apply.accepter,
      const_cast<Ice::Context&> (current.ctx)["status"], const_cast<Ice::Context&> (current.ctx)["WHY"]));

  //设置关系，即applicant对accepter的关系为Apply
  TaskManager::instance().execute(new SetBuddyRelationCacheTask(apply, ::xce::buddy::Apply));

  //为被申请者增加申请信息
  TaskManager::instance().execute(new RebuildBuddyApplyCacheNTask(apply.accepter, current.ctx));

  //在被申请者的DistBuddyApplyCache中添加申请信息
  TaskManager::instance().execute(new AddBuddyApplyNotifyReloaderTask(apply.accepter, current.ctx));

  //在申请者的guide推荐好友中删除被申请者
  TaskManager::instance().execute(new ApplyOfferTask(apply.applicant, apply.accepter));

  //在申请者的推荐新加入好友中删除被申请者
  TaskManager::instance().execute(new ApplyFriendFinderNewComerTask(apply.applicant, apply.accepter));

  //在申请者的MSN推荐好友中删除被申请者
  TaskManager::instance().execute(new RemoveMSNFinderTask(apply.applicant, apply.accepter));

  //在申请者的二度推荐好友中删除被申请者
  TaskManager::instance().execute(new RemoveRecommendFriendTask(apply.applicant, apply.accepter));

  //发送好友申请时通知人人桌面更新相关数据
  TaskManager::instance().execute(new NotifyBuddyApplyChangeTask(apply.accepter));

  //通知安全服务
  TaskManager::instance().execute(new NotifyAntispamAddApplyTask(apply.applicant, apply.accepter, current.ctx));

  //在申请者的申请列表中添加被申请者的信息
  TaskManager::instance().execute(new AddBuddyByApplicantCacheTask(apply, current.ctx));

  //将被申请者的一部分新鲜事发送给申请者（日志，照片，状态，分享)
  TaskManager::instance().execute(new BuddyApplyNotifyFeedTask(apply.applicant, apply.accepter));

  //向BuddyXoaLogic发送添加好友申请通知
  TaskManager::instance().execute(new XoaAddApplyTask(apply.applicant, apply.accepter, current.ctx));

  //删除申请者的Business Cache中被申请者的数据
  TaskManager::instance().execute(new RemoveRcdCacheTask(apply.applicant, apply.accepter, current.ctx));

  MCE_DEBUG("addApply -> " << apply.applicant << " apply " << apply.accepter << " done");

  // 通知3G FriendRelation
  TaskManager::instance().execute(new FriendRelationNotifyTask(::wap::notify::Request, apply.applicant, apply.accepter,
      current.ctx));

  // 通知SocialGraphCoreLogic
  TaskManager::instance().execute(new SocialGraphCoreLogicReportTask(ADDAPPLY, apply.applicant, apply.accepter));
}

//接受好友申请
void BuddyCoreLogicI::acceptApply(const Applyship& apply, const Ice::Current& current) {
  using namespace ::MyUtil;
  using namespace ::xce::check;
  MCE_INFO("acceptApply -> " << apply.accepter << " accept " << apply.applicant);

  //设置关系，即applicant对accepter的关系为Friend
  TaskManager::instance().execute(new SetBuddyRelationCacheTask(apply, ::xce::buddy::Friend,
      TASK_LEVEL_SETBUDDYRELATIONCACHE_ACCEPTAPPLY));

  //在被申请者的BuddyApplyCache中删除申请者的信息
  TaskManager::instance().execute(new RemoveBuddyApplyCacheNTask(apply.accepter, apply.applicant));

  //在被申请者的DistBuddyApplyCache中删除申请者的信息
  TaskManager::instance().execute(new RemoveBuddyApplyNotifyReloaderTask(apply.accepter, apply.applicant));

  //remember to modify addFriend to keep up with acceptApply

  //通知人人桌面更新相关数据
  TaskManager::instance().execute(new PresenceSubscribedTask(const_cast<Ice::Context&> (current.ctx)["id"],
      apply.applicant, apply.accepter));

  //在申请者的二度推荐好友中删除被申请者
  TaskManager::instance().execute(new RemoveRecommendFriendTask(apply.applicant, apply.accepter));

  //通知Checker进行检查
  {
    string value = "";
    string doStr = "add";
    stringstream sthStr;
    sthStr << apply.applicant;
    OperaDataPtr key = new OperaData(apply.accepter, doStr, sthStr.str());
    TaskManager::instance().execute(new CheckAddBuddyTask(key, value));
  }

  //在BuddyByIdCache中添加添加双向好友关系
  TaskManager::instance().execute(new AddBuddyByIdLoaderTask(apply.accepter, apply.applicant));

  // 在BuddyFriendCache中添加双向好友关系
  TaskManager::instance().execute(new AddBuddyFriendCacheTask(apply.accepter, apply.applicant));

  // 相互更新在线状态
  TaskManager::instance().execute(new AddBuddyOnlineStatTask(apply.accepter, apply.applicant));

  //在被申请者的guide推荐好友中删除申请者
  TaskManager::instance().execute(new AcceptOfferTask(apply.accepter, apply.applicant));

  //在被申请者的推荐新加入好友中删除申请者
  TaskManager::instance().execute(new AcceptFriendFinderNewComerTask(apply.accepter, apply.applicant));

  //在被申请者的MSN推荐好友中删除申请者
  TaskManager::instance().execute(new RemoveMSNFinderTask(apply.accepter, apply.applicant));

  //在申请者的MSN推荐好友中删除被申请者
  TaskManager::instance().execute(new RemoveMSNFinderTask(apply.applicant, apply.accepter));

  // 相互添加对方的新鲜事（日志，照片，状态，分享）
  TaskManager::instance().execute(new DeliverSupplyTask(apply.accepter, apply.applicant, current.ctx));

  //发送好友申请时通知人人桌面更新相关数据
  TaskManager::instance().execute(new NotifyBuddyApplyChangeTask(apply.accepter));

  //在申请者的申请列表中删除被申请者的信息
  TaskManager::instance().execute(new RemoveBuddyByApplicantCacheTask(apply.applicant, apply.accepter));

  //相互成为朋友,删除各自Business Cache中对方的数据
  TaskManager::instance().execute(new BecomeFriendsTask(apply.applicant, apply.accepter, current.ctx));

  //从申请者的推荐好友圈子中删除被申请者
  TaskManager::instance().execute(new SocalGraphLogicReportTask(apply.applicant, apply.accepter,
      com::xiaonei::service::buddy::Apply));

  //将对方加入各自的好友列表中，保持好友列表按ID递增有序
  TaskManager::instance().execute(new BecomeFriendNotifyByIdReloaderTask(apply.applicant, apply.accepter));
  TaskManager::instance().execute(new BecomeFriendNotifyByIdReloaderTask(apply.accepter, apply.applicant));

  // 更新双方的新鲜事接受好友列表
  TaskManager::instance().execute(new ChangeBuddyNotifyFeedTask(apply.applicant, apply.accepter));

  //向BuddyXoaLogic发送接受好友申请通知
  TaskManager::instance().execute(new XoaAcceptApplyTask(apply.applicant, apply.accepter, current.ctx));

  MCE_DEBUG("acceptApply -> " << apply.accepter << " accept " << apply.applicant << " done");

  // 通知3G FriendRelation
  TaskManager::instance().execute(new FriendRelationNotifyTask(Become, apply.accepter, apply.applicant, current.ctx));

}

//添加好友
void BuddyCoreLogicI::addFriend(const Ice::Int from, const Ice::Int to, const Ice::Current& current) {
  using namespace ::MyUtil;
  MCE_INFO("addFriend ->  from: " << from << " to: " << to);

  //设置单向好友关系，通过desc表示具体关系
  TaskManager::instance().execute(new SetBuddyRelationCacheTask(from, to, ::xce::buddy::Friend));

  //通知人人桌面更新相关数据
  TaskManager::instance().execute(new PresenceSubscribedTask(const_cast<Ice::Context&> (current.ctx)["id"], from, to));

  //在添加者的推荐好友中删除被添加者
  TaskManager::instance().execute(new RemoveRecommendFriendTask(from, to));

  //在BuddyByIdCache中添加添加双向好友关系
  TaskManager::instance().execute(new AddBuddyByIdLoaderTask(to, from));

  //在BuddyFriendCache中添加双向好友关系
  TaskManager::instance().execute(new AddBuddyFriendCacheTask(to, from));

  // 相互更新在线状态
  TaskManager::instance().execute(new AddBuddyOnlineStatTask(to, from));

  //在各自的guide推荐好友中删除对方
  TaskManager::instance().execute(new ApplyOfferTask(to, from));
  TaskManager::instance().execute(new ApplyOfferTask(from, to));

  //在各自的推荐新加入好友中删除对方
  TaskManager::instance().execute(new ApplyFriendFinderNewComerTask(to, from));
  TaskManager::instance().execute(new ApplyFriendFinderNewComerTask(from, to));

  //在各自的MSN推荐好友中删除对方
  TaskManager::instance().execute(new RemoveMSNFinderTask(to, from));
  TaskManager::instance().execute(new RemoveMSNFinderTask(from, to));

  // 相互添加对方的新鲜事（日志，照片，状态，分享）
  TaskManager::instance().execute(new DeliverSupplyTask(to, from, current.ctx));

  //在添加者的申请列表中删除被添加者的信息
  TaskManager::instance().execute(new RemoveBuddyByApplicantCacheTask(from, to));

  //将对方加入各自的好友列表中，保持好友列表按ID递增有序
  TaskManager::instance().execute(new BecomeFriendNotifyByIdReloaderTask(from, to));
  TaskManager::instance().execute(new BecomeFriendNotifyByIdReloaderTask(to, from));

  // 更新双方的新鲜事接受好友列表
  TaskManager::instance().execute(new ChangeBuddyNotifyFeedTask(from, to));

  //向BuddyXoaLogic发送添加好友通知
  TaskManager::instance().execute(new XoaAddFriendTask(from, to, current.ctx));

  //删除各自Business Cache中对方的数据
  TaskManager::instance().execute(new BecomeFriendsTask(from, to, current.ctx));

  MCE_DEBUG("addFriend -> from: " << from << " to: " << to << " done");
}

//拒绝加好友申请
void BuddyCoreLogicI::denyApply(const Applyship& apply, const Ice::Current& current) {
  using namespace ::MyUtil;
  MCE_INFO("denyApply -> " << apply.accepter << " deny " << apply.applicant);

  //设置申请者对于被申请者的关系为NoPath
  TaskManager::instance().execute(new SetBuddyRelationCacheTask(apply, ::xce::buddy::NoPath,
      TASK_LEVEL_SETBUDDYRELATIONCACHE_DENYAPPLY));

  //在被申请者的BuddyApplyCache中删除申请者的信息
  TaskManager::instance().execute(new RemoveBuddyApplyCacheNTask(apply.accepter, apply.applicant));

  //在被申请者的DistBuddyApplyCache中删除申请者的信息
  TaskManager::instance().execute(new RemoveBuddyApplyNotifyReloaderTask(apply.accepter, apply.applicant));

  //通知人人桌面更新相关数据
  TaskManager::instance().execute(new PresenceUnsubscribedTask(const_cast<Ice::Context&> (current.ctx)["id"],
      apply.applicant, apply.accepter));

  //在被申请者的MSN推荐好友中删除申请者
  TaskManager::instance().execute(new RemoveMSNFinderTask(apply.accepter, apply.applicant));

  //发送好友申请时通知人人桌面更新相关数据
  TaskManager::instance().execute(new NotifyBuddyApplyChangeTask(apply.accepter));

  //在申请者的申请列表中删除被申请者的信息
  TaskManager::instance().execute(new RemoveBuddyByApplicantCacheTask(apply.applicant, apply.accepter));

  //向BuddyXoaLogic发送拒绝好友申请通知
  TaskManager::instance().execute(new XoaDenyApplyTask(apply.applicant, apply.accepter, current.ctx));

  MCE_DEBUG("denyApply -> " << apply.accepter << " deny " << apply.applicant << " done");

  // 通知3G FriendRelation
  TaskManager::instance().execute(new FriendRelationNotifyTask(Refuse, apply.accepter, apply.applicant, current.ctx));

}

//删除好友
void BuddyCoreLogicI::removeFriend(const Friendship& buddy, const Ice::Current& current) {
  using namespace ::MyUtil;
  using namespace ::xce::check;
  MCE_INFO("removeFriend -> " << buddy.from << " remove " << buddy.to);

  //设置被删除好友对于删除人的关系为NoPath
  TaskManager::instance().execute(new SetBuddyRelationCacheTask(buddy.from, buddy.to, ::xce::buddy::NoPath));

  //从数据库中的relation_feed_high和relation_feed_low表中删除相关数据
  TaskManager::instance().execute(new DelInSqlTask("feedwb", buddy.from, buddy.to));

  //删除好友时更新好友在线状态数据
  TaskManager::instance().execute(new RemoveBuddyOnlineStatTask(buddy.from, buddy.to));

  //在被删除者的好友分组中删除删除者
  TaskManager::instance().execute(new RemoveBuddyGroupTask(buddy.from, buddy.to));

  //删除好友时通知人人桌面更新相关数据
  TaskManager::instance().execute(new RemoveBuddyNotifyTask(buddy.from, buddy.to));

  //通知checker进行检查
  {
    string value = "";
    string doStr = "remove";
    ostringstream fromSth, toSth;
    fromSth << buddy.to;
    toSth << buddy.from;
    OperaDataPtr fromkey = new OperaData(buddy.from, doStr, fromSth.str());
    OperaDataPtr tokey = new OperaData(buddy.to, doStr, toSth.str());
    TaskManager::instance().execute(new CheckRemoveBuddyTask(fromkey, value));
    TaskManager::instance().execute(new CheckRemoveBuddyTask(tokey, value));
  }

  //在BuddyByIdCache中删除人的好友中删除被删除人
  TaskManager::instance().execute(new RemoveBuddyByIdCacheTask(buddy.to, buddy.from));

  //在BuddyFriendCache中删除人的好友中删除被删除人
  TaskManager::instance().execute(new RemoveBuddyFriendCacheTask(buddy.to, buddy.from));

  //在各种BuddyCache中删除人的好友中删除被删除人
  TaskManager::instance().execute(new RemoveBuddyByNameCacheTask(buddy.to, buddy.from));
  TaskManager::instance().execute(new RemoveBuddyByOnlineTimeCacheTask(buddy.to, buddy.from));
  TaskManager::instance().execute(new RemoveBuddyByAddTimeCacheTask(buddy.to, buddy.from));

  //在FriendCountCache中将删除人的好友数减1
  TaskManager::instance().execute(new DecFriendCountCacheTask(buddy.to));

  //在BuddyByIdCache中被删除人的好友中删除删除人
  TaskManager::instance().execute(new RemoveBuddyByIdCacheTask(buddy.from, buddy.to));

  //在各种BuddyCache中删除人的好友中删除被删除人
  TaskManager::instance().execute(new RemoveBuddyFriendCacheTask(buddy.from, buddy.to));
  TaskManager::instance().execute(new RemoveBuddyByNameCacheTask(buddy.from, buddy.to));
  TaskManager::instance().execute(new RemoveBuddyByOnlineTimeCacheTask(buddy.from, buddy.to));
  TaskManager::instance().execute(new RemoveBuddyByAddTimeCacheTask(buddy.from, buddy.to));

  //在FriendCountCache中将被删除人的好友数减1
  TaskManager::instance().execute(new DecFriendCountCacheTask(buddy.from));

  //移除两个用户的名片交换信息
  TaskManager::instance().execute(new RemoveCardRelationCacheTask(buddy.from, buddy.to));

  // 更新双方的新鲜事接受好友列表
  TaskManager::instance().execute(new ChangeBuddyNotifyFeedTask(buddy.from, buddy.to));

  //在DistBuddyByIdCache的各自的好友中删除对方
  TaskManager::instance().execute(new RemoveFriendNotifyByIdReloaderTask(buddy.from, buddy.to));
  TaskManager::instance().execute(new RemoveFriendNotifyByIdReloaderTask(buddy.to, buddy.from));

  //向BuddyXoaLogic发送删除好友通知
  TaskManager::instance().execute(new XoaRemoveFriendTask(buddy.from, buddy.to, current.ctx));

  MCE_DEBUG("removeFriend -> " << buddy.from << " remove " << buddy.to << " done");

  // 通知3G FriendRelation
  TaskManager::instance().execute(new FriendRelationNotifyTask(Remove, buddy.from, buddy.to, current.ctx));

}

void BuddyCoreLogicI::addBlock(const Blockship& block, const Ice::Current& current) {
  using namespace ::MyUtil;
  MCE_INFO("addBlock -> " << block.from << " block " << block.to);

  MyUtil::Str2StrMap::const_iterator it;
  it = current.ctx.find("RELATION");
  if (it != current.ctx.end()) {
    //设置单向黑名单关系，通过desc表示from对to的关系
    TaskManager::instance().execute(new SetBuddyRelationCacheTask(block, ::xce::buddy::BuddyDesc(boost::lexical_cast<
        int>(it->second))));
  } else {
    MCE_WARN("addBlock notify RelationCache error: " << block.from << " -> " << block.to);
  }

  //在各自的BuddyApplyCache中删除对方的信息
  TaskManager::instance().execute(new RemoveBuddyApplyCacheNTask(block.from, block.to));
  TaskManager::instance().execute(new RemoveBuddyApplyCacheNTask(block.to, block.from));

  //在各自的DistBuddyApplyCache中删除对方的信息
  TaskManager::instance().execute(new RemoveBuddyApplyNotifyReloaderTask(block.from, block.to));
  TaskManager::instance().execute(new RemoveBuddyApplyNotifyReloaderTask(block.to, block.from));

  //删除好友时更新好友在线状态数据
  TaskManager::instance().execute(new RemoveBuddyOnlineStatTask(block.from, block.to));

  //在设置黑名单时通知人人桌面更新用户数据
  TaskManager::instance().execute(new ReloadBlockUserTask(block.from));

  //在BuddyByIdCache中数据库重新加载双方的好友列表
  TaskManager::instance().execute(new ReloadBuddyByIdLoaderTask(block.from));
  TaskManager::instance().execute(new ReloadBuddyByIdLoaderTask(block.to));

  //在各自的被申请人列表中删除对方
  TaskManager::instance().execute(new RemoveBuddyByApplicantCacheTask(block.from, block.to));
  TaskManager::instance().execute(new RemoveBuddyByApplicantCacheTask(block.to, block.from));

  // 更新双方的新鲜事接受好友列表
  TaskManager::instance().execute(new ChangeBuddyNotifyFeedTask(block.from, block.to));
  TaskManager::instance().execute(new SocalGraphLogicReportTask(block.from, block.to,
      com::xiaonei::service::buddy::Block));

  //在DistBuddyByIdCache中各自的好友中删除对方
  TaskManager::instance().execute(new RemoveFriendNotifyByIdReloaderTask(block.from, block.to));
  TaskManager::instance().execute(new RemoveFriendNotifyByIdReloaderTask(block.to, block.from));

  //向BuddyXoaLogic发送添加黑名单通知
  TaskManager::instance().execute(new XoaAddBlockTask(block.from, block.to, current.ctx));

  //from对to添加黑名单，并删除from的Business Cache中的to的数据
  TaskManager::instance().execute(new BlockTask(block.from, block.to, current.ctx));

  MCE_DEBUG("addBlock -> " << block.from << " block " << block.to << " done");

  // 通知3G FriendRelation
  TaskManager::instance().execute(new FriendRelationNotifyTask(Black, block.from, block.to, current.ctx));
}

void BuddyCoreLogicI::removeBlock(const Blockship& block, const Ice::Current& current) {
  using namespace ::MyUtil;
  MCE_INFO("removeBlock -> " << block.from << " remove block " << block.to);

  TaskManager::instance().execute(new ReloadBlockUserTask(block.from));

  Str2StrMap::const_iterator it;
  it = current.ctx.find("RELATION");
  if (it != current.ctx.end()) {
    //解除黑名单关系
    TaskManager::instance().execute(new SetBuddyRelationCacheTask(block, ::xce::buddy::BuddyDesc(boost::lexical_cast<
        int>(it->second)), TASK_LEVEL_SETBUDDYRELATIONCACHE_REMOVEBLOCK));
  } else {
    MCE_WARN("removeBlock notify RelationCache error: " << block.from << " -> " << block.to);
  }

  //向BuddyXoaLogic发送删除黑名单通知
  TaskManager::instance().execute(new XoaRemoveBlockTask(block.from, block.to, current.ctx));

  MCE_INFO("removeBlock -> " << block.from << " remove block " << block.to << " done");

  // 通知3G FriendRelation
  TaskManager::instance().execute(new FriendRelationNotifyTask(UnBlack, block.from, block.to, current.ctx));

}

void BuddyCoreLogicI::reload(const Ice::Int userId, const Ice::Current& current) {
  using namespace ::MyUtil;
  MCE_INFO("reload -> " << userId);

  // 在BuddyByIdCache中数据库重新加载用户的好友列表
  TaskManager::instance().execute(new ReloadBuddyByIdLoaderTask(userId));

  //向BuddyXoaLogic发送Reload通知
  TaskManager::instance().execute(new XoaReloadTask(userId, current.ctx));

  MCE_INFO("reload -> " << userId << " done");
}

// **************************************************************************

void PresenceSubscribeWithPostscriptTask::handle() {
  using namespace ::talk::adapter;
  try {
    //TODO
    PresenceMsgHandlerAdapter::instance().presenceSubscribeWithPostscript(id_, from_, to_, status_, postscript_);
  } catch (...) {
    // do nothing...
  }
}

void PresenceSubscribeTask::handle() {
  try {
    using namespace ::talk::adapter;
    PresenceMsgHandlerAdapter::instance().presenceSubscribe(id_, from_, to_, status_);
  } catch (...) {
  }
}

void PresenceSubscribedTask::handle() {
  try {
    using namespace ::talk::adapter;
    PresenceMsgHandlerAdapter::instance().presenceSubscribed(id_, from_, to_);
  } catch (...) {
  }
}

void PresenceUnsubscribedTask::handle() {
  try {
    using namespace ::talk::adapter;
    PresenceMsgHandlerAdapter::instance().presenceUnsubscribed(id_, from_, to_);
  } catch (...) {
  }
}

void RebuildBuddyApplyCacheNTask::handle() {
  try {
    using namespace ::xce::redomonitor;
    std::stringstream ss;
    std::string desc;
    ss << "BuddyApplyCache " << id_;
    desc = ss.str();

    RebuildBuddyApplyCacheRedoMonitor<Ice::Context> rbac(ctx_, id_, false, desc);
    rbac.monitorHandle();
  } catch (...) {
  }
}

void AddBuddyApplyNotifyReloaderTask::handle() {
  try {
    using namespace ::xce::redomonitor;
    std::stringstream ss;
    std::string desc;
    ss << "BuddyApplyNotifyReloader " << id_;
    desc = ss.str();

    AddBuddyApplyNotifyReloaderRedoMonitor<Ice::Context> abanr(ctx_, id_, false, desc);
    abanr.monitorHandle();
  } catch (...) {
  }
}

void BecomeFriendNotifyByIdReloaderTask::handle() {
  try {
    using namespace ::xce::redomonitor;
    std::stringstream ss;
    std::string desc;
    ss << "BecomeFriendNotifyByIdReloader " << host_ << " " << guest_;
    desc = ss.str();
    BecomeFriendNotifyByIdReloaderRedoMonitor<Ice::Context> bfnb(host_, guest_, false, desc);
    bfnb.monitorHandle();
  } catch (...) {
  }
}

void RemoveFriendNotifyByIdReloaderTask::handle() {
  try {
    using namespace ::xce::redomonitor;
    std::stringstream ss;
    std::string desc;
    ss << "RemoveFriendNotifyByIdReloader " << host_ << " " << guest_;
    desc = ss.str();
    RemoveFriendNotifyByIdReloaderRedoMonitor<Ice::Context> rfnb(host_, guest_, false, desc);
    rfnb.monitorHandle();
  } catch (...) {
  }
}

void RemoveBuddyApplyCacheNTask::handle() {
  try {
    using namespace ::xce::redomonitor;
    std::stringstream ss;
    std::string desc;
    ss << "BuddyApplyCache " << id_;
    desc = ss.str();

    RemoveBuddyApplyCacheRedoMonitor<Ice::Context> instance(id_, applicant_, false, desc);
    instance.monitorHandle();
  } catch (...) {
  }
}

void RemoveBuddyApplyNotifyReloaderTask::handle() {
  try {
    using namespace ::xce::redomonitor;
    std::stringstream ss;
    std::string desc;
    ss << "BuddyApplyNotifyReloader " << id_;
    desc = ss.str();

    RemoveBuddyApplyNotifyReloaderRedoMonitor<Ice::Context> instance(id_, applicant_, false, desc);
    instance.monitorHandle();
  } catch (...) {
  }
}

void AddBuddyByApplicantCacheTask::handle() {
  try {
    using namespace ::xce::buddy::adapter;
    BuddyByApplicantLoaderAdapter::instance().addAccepter(id_, accepter_);
  } catch (...) {
  }
}

void RemoveBuddyByApplicantCacheTask::handle() {
  try {
    using namespace ::xce::buddy::adapter;
    BuddyByApplicantLoaderAdapter::instance().removeAccepter(id_, accepterId_);
  } catch (...) {
  }
}

void RemoveCommonFriendTask::handle() {
  try {
    using namespace ::xce::socialgraph;
    FriendFinderWorkerAdapter::instance().RemoveCommonFriend(userId_, friendId_);
  } catch (...) {
  }
}

void RemoveFriendFinderTask::handle() {
  //FriendFinderGuideReplicaNAdapter::instance().removeCommonFriend(userId_, friendId_);
}

void RebuildFriendFinderTask::handle() {
  //FriendFinderGuideReplicaNAdapter::instance().rebuildCommonFriend(userId_);
}

void ApplyOfferTask::handle() {
  try {
    using namespace ::xce::offerfriends::adapter;
    OfferFriendsLoaderAdapter::instance().apply(applicant_, acceptor_);
  } catch (...) {
  }
}

void AcceptOfferTask::handle() {
  try {
    using namespace ::xce::offerfriends::adapter;
    OfferFriendsLoaderAdapter::instance().accept(acceptor_, applicant_);
  } catch (...) {
  }
}

void ApplyFriendFinderNewComerTask::handle() {
  try {
    using namespace ::xce::socialgraph::adapter;
    FriendFinderNewComerWorkerAdapter::instance().apply(applicant_, acceptor_);
  } catch (Ice::Exception e) {
    MCE_WARN("[FriendFinderNewComerWorkerAdapter::apply()] applicant=" << applicant_ << " acceptor=" << acceptor_
        << " found Ice::Exception : " << e.what());
  } catch (...) {
    MCE_WARN("[FriendFinderNewComerWorkerAdapter::apply()] applicant=" << applicant_ << " acceptor=" << acceptor_
        << " found unknown exception");
  }
}

void AcceptFriendFinderNewComerTask::handle() {
  try {
    using namespace ::xce::socialgraph::adapter;
    FriendFinderNewComerWorkerAdapter::instance().accept(acceptor_, applicant_);
  } catch (Ice::Exception e) {
    MCE_WARN("[FriendFinderNewComerWorkerAdapter::accept()] applicant=" << applicant_ << " acceptor=" << acceptor_
        << " found Ice::Exception : " << e.what());
  } catch (...) {
    MCE_WARN("[FriendFinderNewComerWorkerAdapter::accept()] applicant=" << applicant_ << " acceptor=" << acceptor_
        << " found unknown exception");
  }
}

void RemoveMSNFinderTask::handle() {
  try {
    using namespace ::xce::friendfinder::adapter;
    FriendFinderByMSNAdapter::instance().remove(userId_, friendId_);
  } catch (...) {
  }
}

void AddBuddyOnlineStatTask::handle() {
  try {
    using namespace ::talk::online::adapter;
    OnlineCenterAdapter::instance().addBuddy(host_, guest_);
  } catch (...) {
  }
}

void RemoveBuddyOnlineStatTask::handle() {
  try {
    using namespace ::talk::online::adapter;
    OnlineCenterAdapter::instance().removeBuddy(host_, guest_);
  } catch (...) {
  }
}

void DeliverSupplyTask::handle() {
  try {
    using namespace ::xce::feed;
    FeedDispatcherAdapter::instance().addBuddyNotify(supplier_, accepter_);
  } catch (...) {
  }
}

void RemoveBuddyGroupTask::handle() {
  try {
    using namespace ::mop::hi::svc::adapter;
    BuddyGroupAdapter::instance().removeBuddy(from_, to_);
  } catch (...) {
  }
}

void RemoveBuddyNotifyTask::handle() {
  try {
    using namespace ::talk::adapter;
    IqMsgHandlerAdapter::instance().removeBuddyNotify(from_, to_);
  } catch (...) {
  }
}

void DelInSqlTask::handle() {
  try {
    using namespace ::com::xiaonei::xce;

    Statement sqlFeedHigh;
    sqlFeedHigh << "DELETE FROM relation_feed_high" << " WHERE SOURCE=" << from_ << " AND TARGET=" << to_
        << " OR SOURCE=" << to_ << " AND TARGET=" << from_;
    QueryRunner(source_, CDbWServer).schedule(sqlFeedHigh);
    Statement sqlFeedLow;
    sqlFeedLow << "DELETE FROM relation_feed_low" << " WHERE SOURCE=" << from_ << " AND TARGET=" << to_
        << " OR SOURCE=" << to_ << " AND TARGET=" << from_;
    QueryRunner(source_, CDbWServer).schedule(sqlFeedLow);
  } catch (...) {
  }
}

void ReloadBlockUserTask::handle() {
  try {
    using namespace com::xiaonei::talk::adapter;
    TalkRightAdapter::instance().reloadBlockUser(id_);
  } catch (...) {
  }
}

void RemoveRecommendFriendTask::handle() {
  try {
    using namespace ::xce::recommendfriend::adapter;
    RecommendFriendAdapter::instance().removeRecommendFriend(ownerId_, otherId_);
  } catch (...) {
  }
}

void RebuildFriendDistTask::handle() {
  //FriendDistAdapter::instance().rebuildFriendDist(id_);
}

void AddBuddyByIdLoaderTask::handle() {
  try {
    using namespace ::xce::redomonitor;
    std::stringstream ss;
    std::string desc;
    ss << "BuddyByIdLoader " << userid_ << " " << friendid_;
    desc = ss.str();

    AddBuddyByIdLoaderRedoMonitor<Ice::Context> instance(userid_, friendid_, false, desc);
    instance.monitorHandle();
  } catch (...) {
  }
}

void RemoveBuddyByIdCacheTask::handle() {
  try {
    using namespace ::xce::buddy::adapter;
    BuddyByIdCacheAdapter::instance().remove(userid_, friendid_);
  } catch (...) {
  }
}

void AddBuddyFriendCacheTask::handle() {
  try {
    using namespace ::xce::buddy::adapter;
    BuddyFriendsLoaderAdapter::instance().add(userid_, friendid_);
  } catch (...) {
  }
}
void RemoveBuddyFriendCacheTask::handle() {
  try {
    using namespace ::xce::buddy::adapter;
    BuddyFriendsCacheAdapter::instance().remove(userid_, friendid_);
  } catch (...) {
  }
}
void RemoveBuddyByNameCacheTask::handle() {
  try {
    using namespace ::xce::buddy::adapter;
    BuddyByNameCacheAdapter::instance().remove(userid_, friendid_);
  } catch (...) {
  }
}

void RemoveBuddyByOnlineTimeCacheTask::handle() {
  try {
    using namespace ::xce::buddy::adapter;
    BuddyByOnlineTimeCacheAdapter::instance().remove(userid_, friendid_);
  } catch (...) {
  }
}

void RemoveBuddyByAddTimeCacheTask::handle() {
  try {
    using namespace ::xce::buddy::adapter;
    BuddyByAddTimeCacheAdapter::instance().remove(userid_, friendid_);
  } catch (...) {
  }
}

void DecFriendCountCacheTask::handle() {
  try {
    using namespace ::xce::buddy::adapter;
    FriendCountCacheAdapter::instance().dec(userid_);
  } catch (...) {
  }
}

void ReloadBuddyByIdLoaderTask::handle() {
  try {
    using namespace ::xce::buddy::adapter;
    BuddyByIdLoaderAdapter::instance().reload(id_);
  } catch (...) {
  }
}

void NotifyBuddyApplyChangeTask::handle() {
  try {
    using namespace ::talk::adapter;
    IqMsgHandlerAdapter::instance().NotifyBuddyApplyChange(id_);
  } catch (...) {
  }
}

void NotifyAntispamAddApplyTask::handle() {
  try {
    using namespace MyUtil;
    Str2StrMap map;
    ostringstream applicant;
    applicant << applicant_;
    ostringstream accepter;
    accepter << accepter_;
    map["func"] = "addfriends._notify";
    map["addfriends._notify.applicant"] = applicant.str();
    map["addfriends._notify.accepter"] = accepter.str();
    map["addfriends.Operation"] = "AddApply";
    map["addfriends._notify.time"] = ctx_["TIME"];
    map["addfriends._notify.why"] = ctx_["WHY"];
    map["addfriends._notify.statfrom"] = ctx_["STAT_FROM"];
    map["addfriends._notify.groupname"] = ctx_["GROUP_NAME"];
    map["addfriends._notify.ref"] = ctx_["REF"];
    map["addfriends._notify.invokeip"] = ctx_["INVOKE_IP"];
    using namespace ::xce::antispamrelation::adapter;
    AntispamRelationAdapter::instance().notify(map);
  } catch (...) {
  }
}

void SetBuddyRelationCacheTask::handle() {
  try {
    using namespace ::xce::buddy::adapter;
    BuddyRelationLoaderAdapter::instance().setRelation(relation_, desc_);
  } catch (...) {
  }
}

void RemoveCardRelationCacheTask::handle() {
  try {
    using namespace ::xce::cardcache;
    CardRelationCacheAdapter::instance().remove(host_, guest_);
  } catch (...) {
  }
}

void ChangeBuddyNotifyFeedTask::handle() {
  try {
    using namespace ::xce::feed;
    FeedDispatcherAdapter::instance().changeBuddyNotify(host_, guest_);
  } catch (...) {
  }
}

void BuddyApplyNotifyFeedTask::handle() {
  try {
    using namespace ::xce::feed;
    FeedDispatcherAdapter::instance().buddyApplyNotify(host_, guest_);
  } catch (...) {
  }
}

void CheckAddBuddyTask::handle() {
  try {
    using namespace ::xce::check;
    AbstractCheckerAdapterFactory::instance().getBuddyChecker("BuddyCoreLogicCheckAddBuddy", "@CheckerBuddy",
        "CheckerAddBuddy", "BuddyCoreLogic", "BuddyCoreLogic")->asycAdd(key_, value_);
  } catch (...) {
  }
}

void CheckRemoveBuddyTask::handle() {
  try {
    using namespace ::xce::check;
    AbstractCheckerAdapterFactory::instance().getBuddyChecker("BuddyCoreLogicCheckRemoveBuddy", "@CheckerBuddy",
        "CheckerRemoveBuddy", "BuddyCoreLogic", "BuddyCoreLogic")->asycAdd(key_, value_);
  } catch (...) {
  }
}

void CheckAddApplyTask::handle() {
  try {
    using namespace ::MyUtil;
    using namespace ::xce::check;
    MCE_INFO(" CheckAddApplyTask::handle -> " << apply_.applicant << " apply " << apply_.accepter);
    string doStr = "applied";
    stringstream sthStr;
    sthStr << apply_.applicant;

    OperaDataPtr key = new OperaData(apply_.accepter, doStr, sthStr.str());
    ostringstream value;
    value << sthStr.str();
    string business = ServiceI::instance().getBusiness();
    string name = ServiceI::instance().getName();
    string replica = name;
    using namespace ::xce::check;
    AbstractCheckerAdapterFactory::instance().getOceChecker(replica + "CheckerAddApply", "@CheckerApply",
        "CheckerAddApply", business, replica)->asycAdd(key, value.str());
  } catch (...) {
  }
}

void CheckRemoveApplyTask::handle() {
  try {
    using namespace ::MyUtil;
    using namespace ::xce::check;
    MCE_INFO(" CheckRemoveApplyTask::handle -> " << apply_.applicant << " apply " << apply_.accepter);
    string doStr = "removeApplied";
    stringstream sthStr;
    sthStr << apply_.applicant;

    OperaDataPtr key = new OperaData(apply_.accepter, doStr, sthStr.str());
    ostringstream value;
    value << sthStr.str();
    string business = ServiceI::instance().getBusiness();
    string name = ServiceI::instance().getName();
    string replica = name;

    using namespace ::xce::check;
    AbstractCheckerAdapterFactory::instance().getOceChecker(replica + "CheckerRemoveApply", "@CheckerApply",
        "CheckerRemoveApply", business, name)->asycAdd(key, value.str());
  } catch (...) {
  }
}

void SocalGraphLogicReportTask::handle() {
  try {
    MCE_INFO(" SocalGraphLogicReportTask::handle -> " << host_ << " do " << guest_ << desc_);
    ::xce::socialgraph::Message message;
    ::xce::socialgraph::Info info;
    info.hostId = host_;
    info.type = "FFUCR";
    if (com::xiaonei::service::buddy::Apply == desc_) {
      info.operation = ::xce::socialgraph::APPLY;
    } else if (com::xiaonei::service::buddy::Block == desc_) {
      info.operation = ::xce::socialgraph::DELETE;
    }
    info.friendIds.push_back(guest_);
    message.content.insert(std::make_pair(info.type, info));
    using namespace ::xce::socialgraph;
    SocialGraphLogicAdapter::instance().Report(info.hostId, message);
  } catch (...) {
  }
}

//------------------------BuddyRcdLogic-------------------------

void RemoveRcdCacheTask::handle() {
  try {
    using namespace ::xce::socialgraph;
    RcdMessageServiceAdapter::instance().RemoveRcdCache(host_, guest_);
  } catch (...) {
  }
}

void BecomeFriendsTask::handle() {
  try {
    using namespace ::xce::socialgraph;
    RcdMessageServiceAdapter::instance().BecomeFriends(host_, guest_);
  } catch (...) {
  }
}

void BlockTask::handle() {
  try {
    using namespace ::xce::socialgraph;
    RcdMessageServiceAdapter::instance().Block(host_, guest_);
  } catch (...) {
  }
}

//------------------------BuddyXoaLogic-------------------------
void XoaAddApplyTask::handle() {
  try {
    using namespace ::xce::buddy;
    BuddyXoaLogicAdapter::instance().addApply(applicant_, apply_, ctx_);
  } catch (...) {
  }
}

void XoaAcceptApplyTask::handle() {
  try {
    using namespace ::xce::buddy;
    BuddyXoaLogicAdapter::instance().acceptApply(applicant_, apply_, ctx_);
  } catch (...) {
  }
}

void XoaDenyApplyTask::handle() {
  try {
    using namespace ::xce::buddy;
    BuddyXoaLogicAdapter::instance().denyApply(applicant_, apply_, ctx_);
  } catch (...) {
  }
}

void XoaAddFriendTask::handle() {
  try {
    using namespace ::xce::buddy;
    BuddyXoaLogicAdapter::instance().addFriend(from_, to_, ctx_);
  } catch (...) {
  }
}

void XoaRemoveFriendTask::handle() {
  try {
    using namespace ::xce::buddy;
    BuddyXoaLogicAdapter::instance().removeFriend(from_, to_, ctx_);
  } catch (...) {
  }
}

void XoaAddBlockTask::handle() {
  using namespace ::xce::buddy;
  try {
    BuddyXoaLogicAdapter::instance().addBlock(block_, blocked_, ctx_);
  } catch (...) {
  }
}

void XoaRemoveBlockTask::handle() {
  using namespace ::xce::buddy;
  try {
    BuddyXoaLogicAdapter::instance().removeBlock(block_, blocked_, ctx_);
  } catch (...) {
  }
}

void XoaReloadTask::handle() {
  using namespace ::xce::buddy;
  try {
    BuddyXoaLogicAdapter::instance().reload(userId_, ctx_);
  } catch (...) {
  }
}

void FriendRelationNotifyTask::handle() {
  using namespace ::wap::notify;
  try {
    RelationNotifyAdapter::instance().notify(type_, from_, target_);
    std::string typeStr = "";
    switch (type_) {
    case Become:
      typeStr = "Become";
      break;
    case Refuse:
      typeStr = "Refuse";
      break;
    case ::wap::notify::Request:
      typeStr = "Request";
      break;
    case Remove:
      typeStr = "Remove";
      break;
    case Black:
      typeStr = "Black";
      break;
    case UnBlack:
      typeStr = "UnBlack";
      break;
    }
    MCE_DEBUG("Notify 3G: " << typeStr << "|" << from_ << "|" << target_ << " done");
  } catch (Ice::Exception& e) {
    MCE_INFO("FriendRelationNotifyTask::handle(" << type_ << "," << from_ << "," << target_ << ") " << e.what());
  } catch (...) {
    MCE_INFO("FriendRelationNotifyTask::handle(" << type_ << "," << from_ << "," << target_ << ") ...exception");
  }
}

void SocialGraphCoreLogicReportTask::handle() {
  try {
    switch (type_) {
    case ADDAPPLY:
      ::xce::socialgraph::SocialGraphCoreLogicAdapter::instance().ReportBuddyApply(from_, to_);
      break;
    case ACCEPTAPPLY:
      break;
    case DENYAPPLY:
      break;
    case REMOVEFRIEND:
      break;
    case ADDBLOCK:
      break;
    case REMOVEBLOCK:
      break;
    default:
      break;
    }
    MCE_DEBUG("Notify SocialGraphCoreLogic: " << getOperationStr(type_) << "|" << from_ << "|" << to_ << " done");
  } catch (Ice::Exception& e) {
    MCE_INFO(__FUNCTION_NAME__ << " type:" << type_ << ", from:" << from_ << ", to:" << to_ << ") " << e.what());
  } catch (std::exception e) {
    MCE_INFO(__FUNCTION_NAME__ << " type:" << type_ << ", from:" << from_ << ", to:" << to_ << ") " << e.what());
  }
}


}
}
}
} //  com::xiaonei::service::buddy
