#include <IceBox/IceBox.h>
#include "ServiceI.h"

#include "UserLogicI.h"
#include <boost/lexical_cast.hpp>
#include "ConnectionQuery.h"
#include "TalkCacheLoaderAdapter.h"
#include "CharityAdapter.h"
#include "IceLogger.h"
#include "TalkRightAdapter.h"
#include "PresenceMsgHandlerAdapter.h"
#include "UserBase.h"
#include "BuddyByIdReplicaAdapter.h"
#include "BuddyByNameReplicaAdapter.h"
#include "BuddyGroupAdapter.h"

#include "DistUserCacheReloaderAdapter.h"
#include "DistUserApiCacheReloaderAdapter.h"
#include "SearchLogicAdapter.h"
#include "DistSearchLogicAdapter.h"
#include "StatusCacheAdapter.h"
#include "DistBuddyByIdCacheReloaderAdapter.h"

#include "PhotoStateFilterAdapter.h"

#include "ScoreCacheAdapter.h"

#include "UserStateFilterAdapter.h"
#include "InviteRewardAdapter.h"
#include "HighSchoolFilterAdapter.h"
#include "CardInfoCacheAdapter.h"
#include "UserCountAdapter.h"
#include "FriendFinderNewComerWorkerAdapter.h"
#include "DistBuddyByIdCacheReloaderAdapter.h"
#include "HomeFootprintAdapter.h"
#include "VisitFootprintAdapter.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include <util/cpp/InvokeMonitor.h>
#include <Monitor/client/service/ServiceMonitor.h>

#include <UserSyncAdapter.h>

#include "UserConfigTripodAdapter.h"
#include "user_network_tripod_adapter.h"

#include "UserBornTripodAdapter.h"
#include "UserDoingTripodAdapter.h"
#include "UserLoginCountTripodAdapter.h"
#include "UserNameTripodAdapter.h"
#include "UserPassportTripodAdapter.h"
#include "UserRightTripodAdapter.h"
#include "UserStageTripodAdapter.h"
#include "UserStateTripodAdapter.h"
#include "UserUrlTripodAdapter.h"

#include "User3GNotifyAdapter.h"
#include "AccountFilterAdapter.h"


using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace mop::hi::svc::model;
using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::adapter;
using namespace xce::usercache;
using namespace xce::userapicache;
using namespace xce::searchcache;
using namespace talk::adapter;
using namespace ::xce::serverstate;
using namespace xce::buddy::adapter;
using namespace mop::hi::svc::adapter;
using namespace xce::photostatefilter::adapter;
using namespace xce::scorecache::adapter;
using namespace xce::userstatefilter::adapter;
using namespace com::renren::search2::friendoffriend;
using namespace xce::cardcache;
using namespace wservice::invitereward::adapter;
using namespace xce::highschoolfilter::adapter;
using namespace xce::socialgraph::adapter;
using namespace com::renren::datasync; //UserSyncAdapter

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&UserLogicManagerI::instance(), service.createIdentity(
			USER_LOGIC_MANAGER, ""));
	TaskManager::instance().config(ThreadPoolConfig(10, 100));

	//    --------------------------------HA--------------------------------------------------
        int mod = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("UserLogic.Mod", 0);
 	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("UserLogic.Interval",5);
        ServerStateSubscriber::instance().initialize("ControllerUserLogic",&UserLogicManagerI::instance(),mod,interval);
	UserLogicManagerI::instance().setValid(true);
  ServiceMonitorManager::instance().start();
}

//-------------------------------------------------------------------------------------------------

bool mop::hi::svc::model::UserLogicManagerI::isValid(const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current);
	IceUtil::RWRecMutex::WLock lock(validmutex_);
	MCE_INFO("UserLogicManagerI::isValid -> "<<valid_);
	return valid_;
}

void mop::hi::svc::model::UserLogicManagerI::setValid(bool state, const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(state), InvokeClient::INVOKE_LEVEL_INFO);
	IceUtil::RWRecMutex::WLock lock(validmutex_);
	valid_ = state;
	if (valid_) {
                ServerStateSubscriber::instance().setStatus(1);
        } else {
                ServerStateSubscriber::instance().setStatus(0);
        }
}

void mop::hi::svc::model::UserLogicManagerI::updateUser(::Ice::Int userId,
		const ::MyUtil::Str2StrMap& props, const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	TaskManager::instance().execute(new PresenceMsgNotifyTask(userId,props,current));
	SearchLogicAdapter::instance().load(userId);
	DistSearchLogicAdapter::instance().load(userId);
	xce::distcache::userapicache::DistUserApiCacheReloaderAdapter::instance().reload(userId);
}

void mop::hi::svc::model::UserLogicManagerI::updateUserDesc(::Ice::Int userId,
		const ::MyUtil::Str2StrMap& props, const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	SearchLogicAdapter::instance().load(userId);
	DistSearchLogicAdapter::instance().load(userId);
}

void mop::hi::svc::model::UserLogicManagerI::insertUserState(::Ice::Int userId,
		const ::MyUtil::Str2StrMap& props, const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
  UserStateSyncAdapter::instance().add(userId);
  TaskManager::instance().execute(new UserStateReloadTask(userId));
}

void mop::hi::svc::model::UserLogicManagerI::updateUserState(::Ice::Int userId,
		const ::MyUtil::Str2StrMap& props, const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
  //把long state字符串截断为int字符串
  MyUtil::Str2StrMap::iterator it = const_cast<MyUtil::Str2StrMap&>(props).find("STATE");
  if(it != props.end()) {
    int stateInt = (int)boost::lexical_cast<long>(it->second);
    it->second = boost::lexical_cast<string>(stateInt);
  }
	TaskManager::instance().execute(new IMNotifyTask(userId,props,current));
	TaskManager::instance().execute(new ScoreCacheVipNotifyTask(userId, props));
	TaskManager::instance().execute(new UserStateFilterUpdateNotifyTask(userId,  props));
	TaskManager::instance().execute(new InviteRewardUpdateNotifyTask(userId,  props));
	xce::distcache::userapicache::DistUserApiCacheReloaderAdapter::instance().update(userId, xce::userapicache::TABLESTATE, props);
	TaskManager::instance().execute(new UserCacheUpdateNotifyTask(userId, xce::usercache::TABLESTATE, props));
	SearchLogicAdapter::instance().update(userId, "user_state", props);
	DistSearchLogicAdapter::instance().update(userId, "user_state", props);
	TaskManager::instance().execute(new CharityNotifyTask(userId, props));
  UserStateSyncAdapter::instance().add(userId);
  TaskManager::instance().execute(new UserStateReloadTask(userId));
}

void mop::hi::svc::model::UserLogicManagerI::updateUserNetwork(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	SearchLogicAdapter::instance().update(userId,"network_history",MyUtil::Str2StrMap());
	DistSearchLogicAdapter::instance().update(userId,"network_history",MyUtil::Str2StrMap());
    NetworkHistorySyncAdapter::instance().add(userId);
  TaskManager::instance().execute(new UserNetworkTripodLoadTask(userId));
}

void mop::hi::svc::model::UserLogicManagerI::insertUserNames(
                ::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
                const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
  UserNamesSyncAdapter::instance().add(userId);
  TaskManager::instance().execute(new UserNameReloadTask(userId));
  TaskManager::instance().execute(new User3GNotifyTask(userId, xce::adapter::user3gnotify::TABLE_USER_NAME, props));
}

void mop::hi::svc::model::UserLogicManagerI::updateUserNames(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	TaskManager::instance().execute(new BuddyByNameLoaderNotifyTask(userId,props,current));
	TaskManager::instance().execute(new IMNotifyTask(userId,props,current));
	xce::distcache::userapicache::DistUserApiCacheReloaderAdapter::instance().update(userId, xce::userapicache::TABLENAMES, props);
	SearchLogicAdapter::instance().update(userId, "user_names", props);
	DistSearchLogicAdapter::instance().update(userId, "user_names", props);
	TaskManager::instance().execute(new UserCacheUpdateNotifyTask(userId, xce::usercache::TABLENAMES, props));
  UserNamesSyncAdapter::instance().add(userId);
  TaskManager::instance().execute(new UserNameReloadTask(userId));
  TaskManager::instance().execute(new User3GNotifyTask(userId, xce::adapter::user3gnotify::TABLE_USER_NAME, props));
}

void mop::hi::svc::model::UserLogicManagerI::insertUserPassport(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	TaskManager::instance().execute(new FootprintNotifyTask(userId));
	TaskManager::instance().execute(new DistBuddyByIdCacheReloaderNotifyTask(userId,props));
	TaskManager::instance().execute(new BuddyByIdLoaderNotifyTask(userId,props,current));
	TaskManager::instance().execute(new UserCountNotifyTask(userId));
	BuddyGroupAdapter::instance().load(userId);
	TaskManager::instance().execute(new StatusCacheNotifyTask(userId,props,current));
	UserCacheReloadCallGuarantee::instance().Add(userId, 0);
	TaskManager::instance().execute(new UserCacheInsertNotifyTask(userId));
	xce::distcache::userapicache::DistUserApiCacheReloaderAdapter::instance().reload(userId);
	SearchLogicAdapter::instance().update(userId, "",MyUtil::Str2StrMap());
	DistSearchLogicAdapter::instance().update(userId, "",MyUtil::Str2StrMap());
	TaskManager::instance().execute(new CompleteUserInfoTask(userId));
  UserPassportSyncAdapter::instance().add(userId);
  TaskManager::instance().execute(new UserPassportReloadTask(userId));
  TaskManager::instance().execute(new User3GNotifyTask(userId, xce::adapter::user3gnotify::TABLE_USER_PASSPORT, props));
  TaskManager::instance().execute(new AccountFilterNotifyTask(userId, props));
}

void mop::hi::svc::model::UserLogicManagerI::updateUserPassport(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	TaskManager::instance().execute(new FootprintNotifyTask(userId));
	TaskManager::instance().execute(new IMNotifyTask(userId,props,current));
	TaskManager::instance().execute(new PresenceMsgNotifyTask(userId,props,current));
	TaskManager::instance().execute(new BuddyByIdLoaderNotifyTask(userId,props,current));
	TaskManager::instance().execute(new StatusCacheNotifyTask(userId,props,current));
	TaskManager::instance().execute(new DistBuddyByIdCacheReloaderNotifyTask(userId,props));
	TaskManager::instance().execute(new UserCacheUpdateNotifyTask(userId, xce::usercache::TABLEPASSPORT, props));
	xce::distcache::userapicache::DistUserApiCacheReloaderAdapter::instance().update(userId, xce::userapicache::TABLEPASSPORT, props);
	SearchLogicAdapter::instance().update(userId, "user_passport",props);
	DistSearchLogicAdapter::instance().update(userId, "user_passport",props);
  UserPassportSyncAdapter::instance().add(userId);
  TaskManager::instance().execute(new UserPassportReloadTask(userId));
  TaskManager::instance().execute(new User3GNotifyTask(userId, xce::adapter::user3gnotify::TABLE_USER_PASSPORT, props));
  TaskManager::instance().execute(new AccountFilterNotifyTask(userId, props));
}

void mop::hi::svc::model::UserLogicManagerI::insertUserConfig(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
  UserConfigSyncAdapter::instance().add(userId);
  TaskManager::instance().execute(new ScoreCacheVipNotifyTask(userId, props));
  // 更新 UserConfig TripodCache 中的数据
  TaskManager::instance().execute(new UserConfigReloadTask(userId));

}

void mop::hi::svc::model::UserLogicManagerI::updateUserConfig(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
  TaskManager::instance().execute(new ScoreCacheVipNotifyTask(userId, props));
  xce::distcache::userapicache::DistUserApiCacheReloaderAdapter::instance().update(userId, xce::userapicache::TABLECONFIG, props);
  SearchLogicAdapter::instance().update(userId, "user_config", props);
  DistSearchLogicAdapter::instance().update(userId, "user_config", props);
  CardInfoCacheAdapter::instance().setProperty(userId, props);
  UserConfigSyncAdapter::instance().add(userId);
  // 更新 UserConfig TripodCache 中的数据
  TaskManager::instance().execute(new UserConfigReloadTask(userId));
}

void mop::hi::svc::model::UserLogicManagerI::insertUserUrl(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	TaskManager::instance().execute(new PhotoStateFilterUpdateNotifyTask(userId,  props));
  UserUrlSyncAdapter::instance().add(userId);
  TaskManager::instance().execute(new UserUrlReloadTask(userId));
  TaskManager::instance().execute(new User3GNotifyTask(userId, xce::adapter::user3gnotify::TABLE_USER_URL, props));
}

void mop::hi::svc::model::UserLogicManagerI::updateUserUrl(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	TaskManager::instance().execute(new IMNotifyTask(userId,props,current));
	TaskManager::instance().execute(new UserCacheUpdateNotifyTask(userId, xce::usercache::TABLEURL, props));
	TaskManager::instance().execute(new PhotoStateFilterUpdateNotifyTask(userId,  props));
	xce::distcache::userapicache::DistUserApiCacheReloaderAdapter::instance().update(userId, xce::userapicache::TABLEURL, props);
	SearchLogicAdapter::instance().update(userId, "user_url", props);
	DistSearchLogicAdapter::instance().update(userId, "user_url", props);
  UserUrlSyncAdapter::instance().add(userId);
  TaskManager::instance().execute(new UserUrlReloadTask(userId));
  TaskManager::instance().execute(new User3GNotifyTask(userId, xce::adapter::user3gnotify::TABLE_USER_URL, props));
}

void mop::hi::svc::model::UserLogicManagerI::insertUserTime(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
}

void mop::hi::svc::model::UserLogicManagerI::updateUserTime(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	SearchLogicAdapter::instance().update(userId, "user_time", props);
	DistSearchLogicAdapter::instance().update(userId, "user_time", props);
  UserTimeSyncAdapter::instance().add(userId);
}

void mop::hi::svc::model::UserLogicManagerI::insertUserDoing(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
  UserDoingSyncAdapter::instance().add(userId);
  TaskManager::instance().execute(new UserDoingReloadTask(userId));
}

void mop::hi::svc::model::UserLogicManagerI::updateUserDoing(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	TaskManager::instance().execute(new IMNotifyTask(userId,props,current));
  UserDoingSyncAdapter::instance().add(userId);
  TaskManager::instance().execute(new UserDoingReloadTask(userId));
}

void mop::hi::svc::model::UserLogicManagerI::insertUserStage(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
  UserStageSyncAdapter::instance().add(userId);
  TaskManager::instance().execute(new UserStageReloadTask(userId));
}

void mop::hi::svc::model::UserLogicManagerI::updateUserStage(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	TaskManager::instance().execute(new UserCacheUpdateNotifyTask(userId, xce::usercache::TABLESTAGE, props));
	TaskManager::instance().execute(new HighSchoolUpdateNotifyTask(userId,  props));
	xce::distcache::userapicache::DistUserApiCacheReloaderAdapter::instance().update(userId, xce::userapicache::TABLESTAGE, props);
	SearchLogicAdapter::instance().update(userId, "user_stage", props);
	DistSearchLogicAdapter::instance().update(userId, "user_stage", props);
  UserStageSyncAdapter::instance().add(userId);
  TaskManager::instance().execute(new UserStageReloadTask(userId));
}

void mop::hi::svc::model::UserLogicManagerI::insertUserBorn(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
  UserBornSyncAdapter::instance().add(userId);
  TaskManager::instance().execute(new UserBornReloadTask(userId));
  TaskManager::instance().execute(new User3GNotifyTask(userId, xce::adapter::user3gnotify::TABLE_USER_BORN, props));
}

void mop::hi::svc::model::UserLogicManagerI::updateUserBorn(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	TaskManager::instance().execute(new UserCacheUpdateNotifyTask(userId, xce::usercache::TABLEBORN, props));
	xce::distcache::userapicache::DistUserApiCacheReloaderAdapter::instance().update(userId, xce::userapicache::TABLEBORN, props);
	SearchLogicAdapter::instance().update(userId, "user_born", props);
	DistSearchLogicAdapter::instance().update(userId, "user_born", props);
  UserBornSyncAdapter::instance().add(userId);
  TaskManager::instance().execute(new UserBornReloadTask(userId));
  TaskManager::instance().execute(new User3GNotifyTask(userId, xce::adapter::user3gnotify::TABLE_USER_BORN, props));
}

void mop::hi::svc::model::UserLogicManagerI::insertUserContact(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
  TaskManager::instance().execute(new User3GNotifyTask(userId, xce::adapter::user3gnotify::TABLE_USER_CONTACT, props));
}

void mop::hi::svc::model::UserLogicManagerI::updateUserContact(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	CardInfoCacheAdapter::instance().setProperty(userId, props);
  UserContactSyncAdapter::instance().add(userId);
  TaskManager::instance().execute(new User3GNotifyTask(userId, xce::adapter::user3gnotify::TABLE_USER_CONTACT, props));
}

void mop::hi::svc::model::UserLogicManagerI::insertUserFav(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
}
void mop::hi::svc::model::UserLogicManagerI::updateUserFav(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	SearchLogicAdapter::instance().update(userId, "user_fav", props);
	DistSearchLogicAdapter::instance().update(userId, "user_fav", props);
}

void mop::hi::svc::model::UserLogicManagerI::insertUserRight(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
  UserRightSyncAdapter::instance().add(userId);
  TaskManager::instance().execute(new UserRightReloadTask(userId));
}

void mop::hi::svc::model::UserLogicManagerI::updateUserRight(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	xce::distcache::userapicache::DistUserApiCacheReloaderAdapter::instance().update(userId, xce::userapicache::TABLERIGHT, props);
  UserRightSyncAdapter::instance().add(userId);
  TaskManager::instance().execute(new UserRightReloadTask(userId));
}

void mop::hi::svc::model::UserLogicManagerI::insertUserLoginCount(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
  TaskManager::instance().execute(new UserLoginCountReloadTask(userId));
}

void mop::hi::svc::model::UserLogicManagerI::updateUserLoginCount(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
  TaskManager::instance().execute(new UserLoginCountReloadTask(userId));
}

void mop::hi::svc::model::UserLogicManagerI::updateUserRegion(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	SearchLogicAdapter::instance().update(userId,"region_info",MyUtil::Str2StrMap());
	DistSearchLogicAdapter::instance().update(userId,"region_info",MyUtil::Str2StrMap());
    RegionInfoSyncAdapter::instance().add(userId);
  TaskManager::instance().execute(new UserRegionTripodLoadTask(userId));
}

void mop::hi::svc::model::UserLogicManagerI::updateUserWorkplace(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	SearchLogicAdapter::instance().update(userId, "workplace_info",MyUtil::Str2StrMap());
	DistSearchLogicAdapter::instance().update(userId, "workplace_info",MyUtil::Str2StrMap());
    WorkplaceInfoSyncAdapter::instance().add(userId);
	//the following line was added by shangmin to notify loader that user info changed
	TaskManager::instance().execute(new FriendFinderNewComerUpdateNotifyTask(userId));
  TaskManager::instance().execute(new UserWorkplaceTripodLoadTask(userId));
}

void mop::hi::svc::model::UserLogicManagerI::updateUserElementarySchool(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	SearchLogicAdapter::instance().update(userId, "elementary_school_info",MyUtil::Str2StrMap());
	DistSearchLogicAdapter::instance().update(userId, "elementary_school_info",MyUtil::Str2StrMap());
    ElementarySchoolInfoSyncAdapter::instance().add(userId);
	//the following line was added by shangmin to notify loader that user info changed
	TaskManager::instance().execute(new FriendFinderNewComerUpdateNotifyTask(userId));
  TaskManager::instance().execute(new UserElementaryTripodLoadTask(userId));

}

void mop::hi::svc::model::UserLogicManagerI::updateUserJuniorHighSchool(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	SearchLogicAdapter::instance().update(userId, "junior_high_school_info",MyUtil::Str2StrMap());
	DistSearchLogicAdapter::instance().update(userId, "junior_high_school_info",MyUtil::Str2StrMap());
    JuniorHighSchoolInfoSyncAdapter::instance().add(userId);
	//the following line was added by shangmin to notify loader that user info changed
	TaskManager::instance().execute(new FriendFinderNewComerUpdateNotifyTask(userId));
  TaskManager::instance().execute(new UserJuniorhighschoolTripodLoadTask(userId));

}

void mop::hi::svc::model::UserLogicManagerI::updateUserHighSchool(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	SearchLogicAdapter::instance().update(userId, "high_school_info",MyUtil::Str2StrMap());
	DistSearchLogicAdapter::instance().update(userId, "high_school_info",MyUtil::Str2StrMap());
    HighSchoolInfoSyncAdapter::instance().add(userId);
	//the following line was added by shangmin to notify loader that user info changed
	TaskManager::instance().execute(new FriendFinderNewComerUpdateNotifyTask(userId));
  TaskManager::instance().execute(new UserHighschoolTripodLoadTask(userId));
}

void mop::hi::svc::model::UserLogicManagerI::updateUserCollege(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	SearchLogicAdapter::instance().update(userId, "college_info",MyUtil::Str2StrMap());
	DistSearchLogicAdapter::instance().update(userId, "college_info",MyUtil::Str2StrMap());
    CollegeInfoSyncAdapter::instance().add(userId);
	//the following line was added by shangmin to notify loader that user info changed
	TaskManager::instance().execute(new FriendFinderNewComerUpdateNotifyTask(userId));
  TaskManager::instance().execute(new UserCollegeTripodLoadTask(userId));

}

void mop::hi::svc::model::UserLogicManagerI::updateUserUniversity(
		::Ice::Int userId, const ::MyUtil::Str2StrMap& props,
		const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(userId), InvokeClient::INVOKE_LEVEL_INFO);
	SearchLogicAdapter::instance().update(userId, "university_info",MyUtil::Str2StrMap());
	DistSearchLogicAdapter::instance().update(userId, "university_info",MyUtil::Str2StrMap());
    UniversityInfoSyncAdapter::instance().add(userId);
	//the following line was added by shangmin to notify loader that user info changed
	TaskManager::instance().execute(new FriendFinderNewComerUpdateNotifyTask(userId));
  TaskManager::instance().execute(new UserUniversityTripodLoadTask(userId));

}

//------------------------
void PhotoStateFilterUpdateNotifyTask::handle() {
        Str2StrMap::const_iterator it = _props.find("TINYURL");
        if (it != _props.end()) {
                try {
                        PhotoStateFilterAdapter::instance().updatePhotoState(_userId,it->second);
                } catch(Ice::Exception& e) {
                        MCE_WARN("PhotoStateFilterUpdateNotifyTask::handle --> do PhotoStateFilterAdapter err:"<<e);
                }
        }
}

void UserCountNotifyTask::handle() {
  xce::usercount::adapter::UserCountAdapter::instance().getAll(_userId);
}

void UserStateFilterUpdateNotifyTask::handle() {
        Str2StrMap::const_iterator it = _props.find("STATE");
        if (it != _props.end()) {
                try {
                        UserStateFilterAdapter::instance().updateUserState(_userId,boost::lexical_cast<int>(it->second));
                } catch(Ice::Exception& e) {
                        MCE_WARN("UserStateFilterUpdateNotifyTask::handle --> do UserStateFilterAdapter err:"<<e);
                }
        }
}

void InviteRewardUpdateNotifyTask::handle() {
        Str2StrMap::const_iterator it = _props.find("STATE");
        if (it != _props.end()) {
                int state = boost::lexical_cast<int>(it->second);
		if (state&1) {
			try {
				InviteRewardAdapter::instance().addStar(_userId);
			} catch(Ice::Exception& e) {
				MCE_WARN("InviteRewardUpdateNotifyTask::handle --> do InviteRewardAdapter userId="<<_userId<<" err:"<<e);
			}
		}
        }
}

void HighSchoolUpdateNotifyTask::handle() {
        Str2StrMap::const_iterator it = _props.find("STAGE");
        if (it != _props.end()) {
		try {
			HighSchoolFilterAdapter::instance().updateStageHighSchool(_userId,boost::lexical_cast<int>(it->second) );
		} catch(Ice::Exception& e) {
			MCE_WARN("HighSchoolUpdateNotifyTask::handle --> do HighSchoolFilterAdapter userId="<<_userId<<" err:"<<e);
		}
        }
}

void IMNotifyTask::handle() {
	try {
		TalkRightAdapter::instance().reloadUserStatus(_userId);
	} catch(Ice::Exception& e) {
		MCE_WARN("IMNotifyTask::handle --> do TalkRightAdapter err:"<<e);
	}
	if (_props.count("DOING") || _props.count("NAME")
			|| _props.count("HEADURL")) {

		try {
			TalkCacheLoaderAdapter::instance().updateUser(_userId, _props);
		} catch(Ice::Exception& e) {
			MCE_WARN("IMNotifyTask::handle --> do TalkCacheAdapter err:"<<e);
		}
		try {
			PresenceMsgHandlerAdapter::instance().webPresence(_userId, _current.ctx);
		} catch(Ice::Exception& e) {
			MCE_WARN("IMNotifyTask::handle --> do PresenceMsgHandlerAdapter err:"<<e);
		}
	}
}

void FriendFinderNewComerUpdateNotifyTask::handle() {
	try {
		FriendFinderNewComerWorkerAdapter::instance().updateInfo(userid_);
	} catch(Ice::Exception& e) {
		MCE_WARN("FriendFinderUpdateNotityTask::handle --> do update notify err:" << e);
	}
}

void PresenceMsgNotifyTask::handle() {
	MyUtil::Str2StrMap::iterator i=_props.find("STATUS");
	if (i!=_props.end()) {
		int status = boost::lexical_cast<int>(i->second);
		PresenceMsgHandlerAdapter::instance().userStatusChanged(_userId,status);
	}
}

void BuddyByIdLoaderNotifyTask::handle() {
	Str2StrMap::const_iterator it = _props.find("STATUS");
	if (it != _props.end()) {
		try {
			int status = boost::lexical_cast<int>(it->second);
			BuddyByIdLoaderAdapter::instance().statusUpdate(_userId,status);
		} catch(Ice::Exception& e) {
			MCE_WARN("BuddyByIdLoaderNotifyTask::handle --> do BuddyByIdLoaderAdapter err:"<<e);
		}
	}
}

void StatusCacheNotifyTask::handle() {
	Str2StrMap::const_iterator it = _props.find("STATUS");
	if (it != _props.end()) {
		try {
			xce::statuscache::StatusCacheAdapter::instance().updateStatus(_userId,boost::lexical_cast<int>(it->second));
		} catch(Ice::Exception& e) {
			MCE_WARN("StatusCacheNotifyTask::handle -> "<<e);
		}
	}

}

void BuddyByNameLoaderNotifyTask::handle() {
	Str2StrMap::const_iterator it = _props.find("NAME");
	if (it != _props.end()) {
		try {
			BuddyByNameLoaderAdapter::instance().rename(_userId,it->second);
		} catch(Ice::Exception& e) {
			MCE_WARN("BuddyByNameLoaderNotifyTask::handle --> do BuddyByNameLoaderAdapter err:"<<e);
		}
	}

}

void FriendOfFriendIndexerNotifyTask::handle() {
         Str2StrMap::const_iterator it = _props.find("NAME");
	if (it != _props.end()) {
		try {
                  FriendOfFriendIndexerAdapter::instance().RebuildIndex(_userId);
		} catch(Ice::Exception& e) {
                   MCE_WARN("FriendOfFriendIndexerNotifyTask::handle --> do FriendOfFriendIndexerNotifyAdapter err:"<<e);
		}
         }
}

void CharityNotifyTask::handle() {
	bool flag = false;
	Str2StrMap::const_iterator it= data_.find("STATE");
	if (it != data_.end()) {
		int state = boost::lexical_cast<int>(it->second);
		if (state & (1 << xce::userbase::CStateCHARITY)) {
			flag = true;
		} 
		xce::charity::CharityAdapter::instance().set(userid_, flag);
	}
}

void ScoreCacheVipNotifyTask::handle() {
	Str2StrMap::const_iterator itState = _props.find("STATUS_CONFIG");
        if (itState != _props.end()) {
                try {
                        ScoreLoaderNAdapter::instance().updateVipState(_userId,boost::lexical_cast<int>(itState->second));
                } catch(Ice::Exception& e) {
                        MCE_WARN("ScoreCacheVipNotifyTask::handle --> do ScoreLoaderNAdapter::updateVipState("<<_userId<<" err:"<<e);
                }
        }
	Str2StrMap::const_iterator itLevel = _props.find("LEVEL");
        if (itLevel != _props.end()) {
                try {
                        ScoreLoaderNAdapter::instance().updateVipLevel(_userId,boost::lexical_cast<int>(itLevel->second));
                } catch(Ice::Exception& e) {
                        MCE_WARN("ScoreCacheVipNotifyTask::handle --> do ScoreLoaderNAdapter::updateVipLevel("<<_userId<<" err:"<<e);
                }
        }
}

void UserCacheInsertNotifyTask::handle() {
	std::vector<std::pair<int, Ice::ObjectPtr> > params;
	params.push_back(std::pair<int, Ice::ObjectPtr>(userid_,0));
	UserCacheInsertCallGuarantee::instance().DoInTime(params);
}

void DistBuddyByIdCacheReloaderNotifyTask::handle() {
  Str2StrMap::iterator i = m_.find("STATUS");
  if (i != m_.end()) {
    int status = boost::lexical_cast<int>(i->second);
    xce::distcache::buddybyidcache::DistBuddyByIdCacheReloaderAdapter::instance().statusUpdate(userid_,status);
  }
}

void CompleteUserInfoTask::handle() {
	xce::adapter::completeuserinfo::CompleteUserInfoAdapter::instance().completeUserInfo(userId_);
}

void FootprintNotifyTask::handle() {
	try {
		MCE_DEBUG("FootprintNotifyTask::handle, userid= " << userId_);
		xce::footprint::HomeFootprintAdapter::instance().getSize(userId_);
		xce::footprint::VisitFootprintAdapter::instance().getSize(userId_);
	}catch (...) {
		MCE_WARN("UserLogic::FootprintNotifyTask::handle, userId: " << userId_);
	}
}

void UserCacheUpdateNotifyTask::handle() {
	UserCacheUpdateOldDataPtr data = new UserCacheUpdateOldData;
	data->userid = userid_;
	data->table = table_;
	data->data = data_;
	std::vector<std::pair<int, Ice::ObjectPtr> > params;
	params.push_back(std::pair<int, Ice::ObjectPtr>(userid_, data));
	UserCacheUpdateCallGuarantee::instance().DoInTime(params);
}

bool UserCacheInsertCallGuarantee::Do(const std::vector<std::pair<int, Ice::ObjectPtr> >& params) {
	if (params.size() != 1) {
		MCE_WARN("UserCacheInsertCallGuarantee::Do -> params.size is not 1 but "<<params.size());
		return true;
	}
	try {
		xce::distcache::usercache::DistUserCacheReloaderAdapter::instance().getProxy(0)->insert(params.begin()->first);
		return true;
	} catch (const Ice::Exception& e) {
		MCE_WARN("UserCacheInsertCallGuarantee::Do -> exception distcache :" << e.what());
	} catch (...) {
		MCE_WARN("UserCacheInsertCallGuarantee::Do -> unknown exception distcache");
	}
	return false;
}

bool UserCacheInsertCallGuarantee::Redo(const std::vector<int>& ids) {
        if (ids.size() <= 0) {
                return true;
        }
	std::set<int> is;
	MyUtil::IntSeq idss;
        for (std::vector<int>::const_iterator i = ids.begin(); i != ids.end(); ++i) {
                is.insert(*i);
        }
	for (std::set<int>::const_iterator i = is.begin(); i != is.end(); ++i) {
		idss.push_back(*i);
	}
        try {
		xce::distcache::usercache::DistUserCacheReloaderAdapter::instance().getProxy(0)->reload(ids);
                return true;
        } catch (const Ice::Exception& e) {
                MCE_WARN("UserCacheInsertCallGuarantee::Redo -> exception distcache: " << e.what());
        } catch (...) {
                MCE_WARN("UserCacheInsertCallGuarantee::Redo -> unknown exception distcache");
        }
        return false;
}

bool UserCacheUpdateCallGuarantee::Do(const std::vector<std::pair<int, Ice::ObjectPtr> >& params) {
	if (params.size() != 1) {
		MCE_WARN("UserCacheUpdateCallGuarantee::Do -> params.size is not 1 but "<<params.size());
		return true;
	}
	try {
		xce::distcache::usercache::DistUserCacheReloaderAdapter::instance().getProxy(0)->update(UserCacheUpdateOldDataPtr::dynamicCast(params.begin()->second));
		return true;
	} catch (const Ice::Exception& e) {
		MCE_WARN("UserCacheUpdateCallGuarantee::Do -> exception distcache: " << e.what());
	} catch (...) {
		MCE_WARN("UserCacheUpdateCallGuarantee::Do -> unknown exception distcache");
	}
	return false;
}

bool UserCacheUpdateCallGuarantee::Redo(const std::vector<int>& ids) {
        if (ids.size() <= 0) {
                return true;
        }
	std::set<int> is;
	MyUtil::IntSeq idss;
        for (std::vector<int>::const_iterator i = ids.begin(); i != ids.end(); ++i) {
                is.insert(*i);
        }
	for (std::set<int>::const_iterator i = is.begin(); i != is.end(); ++i) {
		idss.push_back(*i);
	}
        try {
		xce::distcache::usercache::DistUserCacheReloaderAdapter::instance().getProxy(0)->reload(ids);
                return true;
        } catch (const Ice::Exception& e) {
                MCE_WARN("UserCacheUpdateCallGuarantee::Redo -> exception distcache: " << e.what());
        } catch (...) {
                MCE_WARN("UserCacheUpdateCallGuarantee::Redo -> unknown exception distcache");
        }
        return false;
}

bool UserCacheReloadCallGuarantee::Do(const std::vector<std::pair<int, Ice::ObjectPtr> >& params) {
	if (params.size() <= 0) {
		return true;
	}
	std::set<int> is;
	MyUtil::IntSeq ids;
	for (std::vector<std::pair<int, Ice::ObjectPtr> >::const_iterator i = params.begin(); i != params.end(); ++i) {
		is.insert(i->first);
	}
	for (std::set<int>::const_iterator i = is.begin(); i != is.end(); ++i) {
		ids.push_back(*i);
	}
	try {
		xce::distcache::usercache::DistUserCacheReloaderAdapter::instance().getProxy(0)->reload(ids);
		return true;
	} catch (const Ice::Exception& e) {
		MCE_WARN("UserCacheReloadCallGuarantee::Do -> exception distcache: " << e.what());
	} catch (...) {
		MCE_WARN("UserCacheReloadCallGuarantee::Do -> unknown exception distcache");
	}
	return false;
}

bool UserCacheReloadCallGuarantee::Redo(const std::vector<int>& ids) {
        if (ids.size() <= 0) {
                return true;
        }
	std::set<int> is;
	MyUtil::IntSeq idss;
        for (std::vector<int>::const_iterator i = ids.begin(); i != ids.end(); ++i) {
                is.insert(*i);
        }
	for (std::set<int>::const_iterator i = is.begin(); i != is.end(); ++i) {
		idss.push_back(*i);
	}
        try {
		xce::distcache::usercache::DistUserCacheReloaderAdapter::instance().getProxy(0)->reload(ids);
                return true;
        } catch (const Ice::Exception& e) {
                MCE_WARN("UserCacheReloadCallGuarantee::Redo -> exception distcache: " << e.what());
        } catch (...) {
                MCE_WARN("UserCacheReloadCallGuarantee::Redo -> unknown exception distcache");
        }
        return false;
}

void UserConfigReloadTask::handle() {
	try {
		MCE_DEBUG("UserConfigReloadTask::handle, userid= " << userId_);
		bool success = xce::adapter::UserConfigTripodAdapter::instance().reload(userId_);
		MCE_DEBUG("UserConfigReloadTask::handle, " << userId_ << " load result:" << success);
	}catch (...) {
		MCE_WARN("UserLogic::UserConfigReloadTask::handle exception. userId: " << userId_);
	}
}

void UserNetworkTripodLoadTask::handle() {
  try {
    MCE_DEBUG("UserNetworkTripodLoadTask::handle, userid= " << userId_);
    bool success = xce::adapter::UserNetworkTripodAdapter::instance().LoadNetwork(userId_,true);
    MCE_DEBUG("UserNetworkTripodLoadTask::handle, " << userId_ << " load result:" << success);
  }catch (...) {
    MCE_WARN("UserLogic::UserNetworkTripodLoadTask::handle exception. userId: " << userId_);
  }
}

void UserCollegeTripodLoadTask::handle() {
  try {
    MCE_DEBUG("UserCollegeTripodLoadTask::handle, userid= " << userId_);
    bool success = xce::adapter::UserNetworkTripodAdapter::instance().LoadCollege(userId_,true);
    MCE_DEBUG("UserCollegeTripodLoadTask::handle, " << userId_ << " load result:" << success);
  }catch (...) {
    MCE_WARN("UserLogic::UserCollegeTripodLoadTask::handle exception. userId: " << userId_);
  }
}

void UserUniversityTripodLoadTask::handle() {
  try {
    MCE_DEBUG("UserUniversityTripodLoadTask::handle, userid= " << userId_);
    bool success = xce::adapter::UserNetworkTripodAdapter::instance().LoadUniversity(userId_,true);
    MCE_DEBUG("UserUniversityTripodLoadTask::handle, " << userId_ << " load result:" << success);
  }catch (...) {
    MCE_WARN("UserLogic::UserUniversityTripodLoadTask::handle exception. userId: " << userId_);
  }
}

void UserWorkplaceTripodLoadTask::handle() {
  try {
    MCE_DEBUG("UserWorkplaceTripodLoadTask::handle, userid= " << userId_);
    bool success = xce::adapter::UserNetworkTripodAdapter::instance().LoadWorkplace(userId_,true);
    MCE_DEBUG("UserWorkplaceTripodLoadTask::handle, " << userId_ << " load result:" << success);
  }catch (...) {
    MCE_WARN("UserLogic::UserWorkplaceTripodLoadTask::handle exception. userId: " << userId_);
  }
}

void UserRegionTripodLoadTask::handle() {
  try {
    MCE_DEBUG("UserRegionTripodLoadTask::handle, userid= " << userId_);
    bool success = xce::adapter::UserNetworkTripodAdapter::instance().LoadRegion(userId_,true);
    MCE_DEBUG("UserRegionTripodLoadTask::handle, " << userId_ << " load result:" << success);
  }catch (...) {
    MCE_WARN("UserLogic::UserRegionTripodLoadTask::handle exception. userId: " << userId_);
  }
}

void UserHighschoolTripodLoadTask::handle() {
  try {
    MCE_DEBUG("UserHighschoolTripodLoadTask::handle, userid= " << userId_);
    bool success = xce::adapter::UserNetworkTripodAdapter::instance().LoadHighSchool(userId_,true);
    MCE_DEBUG("UserHighschoolTripodLoadTask::handle, " << userId_ << " load result:" << success);
  }catch (...) {
    MCE_WARN("UserLogic::UserHighschoolTripodLoadTask::handle exception. userId: " << userId_);
  }
}

void UserJuniorhighschoolTripodLoadTask::handle() {
  try {
    MCE_DEBUG("UserJuniorhighschoolTripodLoadTask::handle, userid= " << userId_);
    bool success = xce::adapter::UserNetworkTripodAdapter::instance().LoadJuniorHighSchool(userId_,true);
    MCE_DEBUG("UserJuniorhighschoolTripodLoadTask::handle, " << userId_ << " load result:" << success);
  }catch (...) {
    MCE_WARN("UserLogic::UserJuniorhighschoolTripodLoadTask::handle exception. userId: " << userId_);
  }
}

void UserElementaryTripodLoadTask::handle() {
  try {
    MCE_DEBUG("UserElementaryTripodLoadTask::handle, userid= " << userId_);
    bool success = xce::adapter::UserNetworkTripodAdapter::instance().LoadElementarySchool(userId_,true);
    MCE_DEBUG("UserElementaryTripodLoadTask::handle, " << userId_ << " load result:" << success);
  }catch (...) {
    MCE_WARN("UserLogic::UserElementaryTripodLoadTask::handle exception. userId: " << userId_);
  }
}

void UserBornReloadTask::handle() {
	try {
		MCE_DEBUG("UserBornReloadTask::handle, userid= " << userId_);
		bool success = xce::adapter::userborn::UserBornTripodAdapter::instance().Load(userId_, true);
		MCE_DEBUG("UserBornReloadTask::handle, " << userId_ << " load result:" << success);
	}catch (...) {
		MCE_WARN("UserLogic::UserBornReloadTask::handle exception. userId: " << userId_);
	}
}

void UserDoingReloadTask::handle() {
	try {
		MCE_DEBUG("UserDoingReloadTask::handle, userid= " << userId_);
		bool success = xce::adapter::userdoing::UserDoingTripodAdapter::instance().Load(userId_, true);
		MCE_DEBUG("UserDoingReloadTask::handle, " << userId_ << " load result:" << success);
	}catch (...) {
		MCE_WARN("UserLogic::UserDoingReloadTask::handle exception. userId: " << userId_);
	}
}

void UserLoginCountReloadTask::handle() {
	try {
		MCE_DEBUG("UserLoginCountReloadTask::handle, userid= " << userId_);
		bool success = xce::adapter::userlogincount::UserLoginCountTripodAdapter::instance().Load(userId_, true);
		MCE_DEBUG("UserLoginCountReloadTask::handle, " << userId_ << " load result:" << success);
	}catch (...) {
		MCE_WARN("UserLogic::UserLoginCountReloadTask::handle exception. userId: " << userId_);
	}
}

void UserNameReloadTask::handle() {
	try {
		MCE_DEBUG("UserNameReloadTask::handle, userid= " << userId_);
		bool success = xce::adapter::username::UserNameTripodAdapter::instance().Load(userId_, true);
		MCE_DEBUG("UserNameReloadTask::handle, " << userId_ << " load result:" << success);
	}catch (...) {
		MCE_WARN("UserLogic::UserNameReloadTask::handle exception. userId: " << userId_);
	}
}

void UserPassportReloadTask::handle() {
	try {
		MCE_DEBUG("UserPassportReloadTask::handle, userid= " << userId_);
		bool success = xce::adapter::userpassport::UserPassportTripodAdapter::instance().Load(userId_, true);
		MCE_DEBUG("UserPassportReloadTask::handle, " << userId_ << " load result:" << success);
	}catch (...) {
		MCE_WARN("UserLogic::UserPassportReloadTask::handle exception. userId: " << userId_);
	}
}

void UserRightReloadTask::handle() {
	try {
		MCE_DEBUG("UserRightReloadTask::handle, userid= " << userId_);
		bool success = xce::adapter::userright::UserRightTripodAdapter::instance().Load(userId_, true);
		MCE_DEBUG("UserRightReloadTask::handle, " << userId_ << " load result:" << success);
	}catch (...) {
		MCE_WARN("UserLogic::UserRightReloadTask::handle exception. userId: " << userId_);
	}
}

void UserStageReloadTask::handle() {
	try {
		MCE_DEBUG("UserStageReloadTask::handle, userid= " << userId_);
		bool success = xce::adapter::userstage::UserStageTripodAdapter::instance().Load(userId_, true);
		MCE_DEBUG("UserStageReloadTask::handle, " << userId_ << " load result:" << success);
	}catch (...) {
		MCE_WARN("UserLogic::UserStageReloadTask::handle exception. userId: " << userId_);
	}
}

void UserStateReloadTask::handle() {
	try {
		MCE_DEBUG("UserStateReloadTask::handle, userid= " << userId_);
		bool success = xce::adapter::userstate::UserStateTripodAdapter::instance().Load(userId_, true);
		MCE_DEBUG("UserStateReloadTask::handle, " << userId_ << " load result:" << success);
	}catch (...) {
		MCE_WARN("UserLogic::UserStateReloadTask::handle exception. userId: " << userId_);
	}
}

void UserUrlReloadTask::handle() {
	try {
		MCE_DEBUG("UserUrlReloadTask::handle, userid= " << userId_);
		bool success = xce::adapter::userurl::UserUrlTripodAdapter::instance().Load(userId_, true);
		MCE_DEBUG("UserUrlReloadTask::handle, " << userId_ << " load result:" << success);
	}catch (...) {
		MCE_WARN("UserLogic::UserUrlReloadTask::handle exception. userId: " << userId_);
	}
}

void User3GNotifyTask::handle() {
	try {
    MCE_DEBUG("UserLogic::User3GNotifyTask::handle user_id: "
        << user_id_ << ", table: " << table_);
		xce::adapter::user3gnotify::User3GNotifyAdapter::instance().Update(user_id_, table_, props_);
	}catch (std::exception e) {
		MCE_WARN("UserLogic::User3GNotifyTask::handle exception. user_id: "
        << user_id_ << ", table: " << table_ << ", " << e.what());
	}
}

void AccountFilterNotifyTask::handle() {
  try {
    Str2StrMap::const_iterator it = props_.find(xce::userbase::CUPACCOUNT);
    if(it != props_.end()) {
      MCE_DEBUG("UserLogic::AccountFilterNotifyTask::handle account: " << it->second << " ,id: " << id_);
      xce::adapter::account::AccountFilterAdapter::instance().AddAccount(it->second, id_);
    }
  }catch (std::exception e) {
    MCE_WARN("UserLogic::AccountFilterNotifyTask::handle exception. id: "
        << id_ << ", " << e.what());
  }
}
