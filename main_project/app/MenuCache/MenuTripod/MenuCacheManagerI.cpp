/*
 * =====================================================================================
 *
 *       Filename:  MenuCacheManagerI.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011年12月14日 16时53分10秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#include "MenuCacheManagerI.h"
#include "MenuTripodShare.h"
#include "ServiceI.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "DbCxxPool/src/QueryRunner.h"
#include "app/Utils/src/DBUtils.h"

using namespace xce::menutripod;
using namespace xce::menu;
using namespace xce::serverstate;
using namespace com::xiaonei::xce;

void MyUtil::initialize() {
  MCE_DEBUG("MenuCacheManagerI::initialize");
  ServiceI& service = ServiceI::instance();
  service.registerObjectCache(RECMENU_CACHE_CATEGORY, "RCMC",
      new RecentMenuCacheDataFactory, true);
  service.registerObjectCache(FAVMENU_CACHE_CATEGORY, "FVMC",
      new FavoriteMenuCacheDataFactory, true);

  // 向icegrid加入服务
  service.getAdapter()->add(&MenuCacheManagerI::instance(), service.createIdentity("M", ""));

  // 注册HA
  int mod = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("MenuCacheTripod.Mod", 1);
  string controllerEndpoint = service.getCommunicator()->getProperties()->getPropertyWithDefault("MenuCacheTripod.Endpoint", "ControllerMenuCacheN");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("MenuCacheTripod.Interval", 5);
  ServerStateSubscriber::instance().initialize(controllerEndpoint, &MenuCacheManagerI::instance(), mod, interval);
  MenuCacheManagerI::instance().init();
}

void MenuCacheManagerI::init() {
  recentMenuTripodClient_ = new TripodClient(TRIPODCACHE_ZOOKEEPER_ADDR, TRIPODCACHE_NAMESPACE, TRIPODCACHE_RECENT_MENU);
  favoriteMenuTripodClient_ = new TripodClient(TRIPODCACHE_ZOOKEEPER_ADDR, TRIPODCACHE_NAMESPACE, TRIPODCACHE_FAVORITE_MENU);
  allCacheTypes_.push_back(RECENTLY);
  allCacheTypes_.push_back(FAVORITE);
  loadRecentMenuThread_ = new ReloadFromTripodThread(TRIPODCACHE_RECENT_MENU, RECMENU_CACHE_CATEGORY);
  loadFavoriteMenuThread_ = new ReloadFromTripodThread(TRIPODCACHE_FAVORITE_MENU, FAVMENU_CACHE_CATEGORY);
  loadRecentMenuThread_->start(128*1024).detach();
  loadFavoriteMenuThread_->start(128*1024).detach();
}

void MenuCacheManagerI::addKeysToLoad(const vector<string>& userIds, CacheType cacheType) {
  if (cacheType == RECENTLY) {
    loadRecentMenuThread_->add(userIds);
  } else if (cacheType == FAVORITE) {
    loadFavoriteMenuThread_->add(userIds);
  }
}

void MenuCacheManagerI::updateCache(const vector<MenuTripodDataPtr>& menuDatas) {
  map<CacheType, vector<string> > reloadMap;
  for (vector<MenuTripodDataPtr>::const_iterator it = menuDatas.begin(); it != menuDatas.end(); ++it) {
    CacheType type = (*it)->getCacheType();
    string cacheTypeStr = TripodHelper::getCacheTypeStr(type);
    //MCE_INFO("[MenuCacheManagerI::updateCache] userId = " << (*it)->getUserId() << ", cacheType = " << cacheTypeStr << " add to ObjectCache");
    ServiceI::instance().getObjectCache()->addObject(getObjCacheCategoryWithCacheType(type), (*it)->getUserId(), *it);
    map<CacheType, vector<string> >::iterator itm = reloadMap.find(type);
    if (itm == reloadMap.end()) {
      vector<string> userIds;
      userIds.push_back(boost::lexical_cast<string>((*it)->getUserId()));
      reloadMap.insert(make_pair<CacheType, vector<string> >(type, userIds));
    } else {
      itm->second.push_back(boost::lexical_cast<string>((*it)->getUserId()));
    }
  }
  for (map<CacheType, vector<string> >::const_iterator it = reloadMap.begin(); it != reloadMap.end(); ++it) {
    addKeysToLoad(it->second, it->first);
  }
}

TripodClient* MenuCacheManagerI::getTripodClientWithCacheType(CacheType cacheType) {
  TripodClient* tempClient = 0;
  if (cacheType == RECENTLY) {
    tempClient = recentMenuTripodClient_; 
  } else if (cacheType == FAVORITE) {
    tempClient = favoriteMenuTripodClient_;
  }
  return tempClient;
}

int MenuCacheManagerI::getObjCacheCategoryWithCacheType(CacheType cacheType) {
  if (cacheType == RECENTLY) {
    return RECMENU_CACHE_CATEGORY; 
  } else if (cacheType == FAVORITE) {
    return FAVMENU_CACHE_CATEGORY;
  }
  return -1;
}

MenuTripodDataPtr MenuCacheManagerI::getWithUserId(int userId, CacheType cacheType) {
  CacheTypeSeq types;
  types.push_back(cacheType);
  vector<MenuTripodDataPtr> ptrs = getWithUserId(userId, types);
  return *(ptrs.begin());
}

vector<MenuTripodDataPtr> MenuCacheManagerI::getWithUserId(int userId, const CacheTypeSeq& types) {
  vector<MenuTripodDataPtr> result;
  for (CacheTypeSeq::const_iterator it = types.begin(); it != types.end(); ++it) {
    int category = getObjCacheCategoryWithCacheType(*it);
    if (category == -1) {
      continue;
    }
    string cacheTypeStr = TripodHelper::getCacheTypeStr(*it);
    MenuTripodDataPtr ptr = ServiceI::instance().findObject<MenuTripodDataPtr>(category, userId);
    if (ptr != 0) {
      //MCE_INFO("[MenuCacheManagerI::getWithUserId] userId = " << userId << ", cacheType = " <<  cacheTypeStr
      //    << " found in ObjectCache");
      result.push_back(ptr);
    } else {
      TripodClient* tripodClient = getTripodClientWithCacheType(*it);
      KeySeq missedKeys, keys;
      keys.push_back(boost::lexical_cast<string>(userId));
      DataMap dataMap = tripodClient->get(keys, missedKeys);
      if (!missedKeys.empty()) {
        addKeysToLoad(missedKeys, *it);
      }
      if (dataMap.empty()) {
        //MCE_INFO("[MenuCacheManagerI::getWithUserId] userId = " << userId << ", cacheType = " << cacheTypeStr
        //    << ", not found in TripodCache");
        result.push_back(new MenuTripodData(userId, *it));
      } else {
        //MCE_INFO("[MenuCacheManagerI::getWithUserId] userId = " << userId << ", cacheType = " << cacheTypeStr
        //    << ", found in TripodCache");
        for (DataMap::const_iterator itx = dataMap.begin(); itx != dataMap.end(); ++itx) {
          string byteArray(itx->second.begin(), itx->second.end());
          MenuTripodDataPtr tempPtr = new MenuTripodData(byteArray);
          result.push_back(tempPtr);
        }
      }
    }
  }
  return result;
}

CacheType2AppIdSeqMap MenuCacheManagerI::get(int userId, const CacheTypeSeq& types, const Ice::Current& current) {
  MCE_INFO("[MenuCacheManagerI::get] userId = " << userId);
  CacheType2AppIdSeqMap resultMap;
  vector<MenuTripodDataPtr> menuDatas = getWithUserId(userId, types);
  //MCE_INFO("[MenuCacheManagerI::get] userId = " << userId << ", result.size = " << menuDatas.size());
  for (vector<MenuTripodDataPtr>::const_iterator it = menuDatas.begin(); it != menuDatas.end(); ++it) {
    resultMap.insert(make_pair<CacheType, AppIdSeq>((*it)->getCacheType(), (*it)->getAppIds()));
  }
  return resultMap;
}

CacheType2AppIdSeqMap MenuCacheManagerI::getAll(int userId, const Ice::Current& current) {
  MCE_INFO("[MenuCacheManagerI::getAll] userId = " << userId);
  return get(userId, allCacheTypes_);
}

void MenuCacheManagerI::set(int userId, CacheType type, const AppIdSeq& appIds, const Ice::Current& current) {
  MCE_INFO("[MenuCacheManagerI::set] userId = " << userId << ", cacheType = " << type << ", appIds.size = " << appIds.size());
  MenuTripodDataPtr ptr = getWithUserId(userId, type);
  ptr->setAppIds(appIds);
  vector<MenuTripodDataPtr> ptrs;
  ptrs.push_back(ptr);
  updateCache(ptrs);
}

vector<MenuTripodDataPtr> MenuCacheManagerI::parseFromMenuCacheRecordSeq(const MenuCacheRecordSeq& records) {
  vector<MenuTripodDataPtr> result;
  for (MenuCacheRecordSeq::const_iterator it = records.begin(); it != records.end(); ++it) {
    result.push_back(new MenuTripodData(*it));
  }
  return result;
}

void MenuCacheManagerI::setBatch(const MenuCacheRecordSeq& records, const Ice::Current& current) {
  MCE_INFO("[MenuCacheManagerI::setBatch] records.size = " << records.size());
  updateCache(parseFromMenuCacheRecordSeq(records));
}

void MenuCacheManagerI::addBatch(const MenuCacheRecordSeq& records, const Ice::Current& current) {
  MCE_INFO("[MenuCacheManagerI::addBatch] records.size = " << records.size());
  vector<MenuTripodDataPtr> ptrsToUpdate;
  for (MenuCacheRecordSeq::const_iterator it = records.begin(); it != records.end(); ++it) {
    MenuTripodDataPtr ptrToAdd = new MenuTripodData(*it);
    MenuTripodDataPtr ptr = getWithUserId(ptrToAdd->getUserId(), ptrToAdd->getCacheType());
    ptr->addAppIds(ptrToAdd->getAppIds());
    ptrsToUpdate.push_back(ptr);
  }
  if (!ptrsToUpdate.empty()) {
    updateCache(ptrsToUpdate);
  }
}

void MenuCacheManagerI::deleteBatch(const MenuCacheRecordSeq& records, const Ice::Current& current) {
  MCE_INFO("[MenuCacheManagerI::deleteBatch] records.size = " << records.size());
  vector<MenuTripodDataPtr> ptrsToUpdate;
  for (MenuCacheRecordSeq::const_iterator it = records.begin(); it != records.end(); ++it) {
    MenuTripodDataPtr ptrToDelete = new MenuTripodData(*it);
    MenuTripodDataPtr ptr = getWithUserId(ptrToDelete->getUserId(), ptrToDelete->getCacheType());
    ptr->deleteAppIds(ptrToDelete->getAppIds());
    ptrsToUpdate.push_back(ptr);
  }
  if (!ptrsToUpdate.empty()) {
    updateCache(ptrsToUpdate);
  }
}

vector<string> MenuCacheManagerI::convertIntSeq2StringSeq(const vector<int>& userIds) {
  vector<string> resultSeq;
  for (vector<int>::const_iterator it = userIds.begin(); it != userIds.end(); ++it) {
    resultSeq.push_back(boost::lexical_cast<string>(*it));
  }
  return resultSeq;
}

vector<long> MenuCacheManagerI::convertStringSeq2LongSeq(const vector<string>& userIds) {
  vector<long> resultSeq;
  for (vector<string>::const_iterator it = userIds.begin(); it != userIds.end(); ++it) {
    resultSeq.push_back(boost::lexical_cast<long>(*it));
  }
  return resultSeq;
}

void MenuCacheManagerI::load(const UserIdSeq& userIds, const Ice::Current& current) {
  MCE_INFO("[MenuCacheManagerI::load] userIds.size = " << userIds.size());
  addKeysToLoad(convertIntSeq2StringSeq(userIds), RECENTLY);
  addKeysToLoad(convertIntSeq2StringSeq(userIds), FAVORITE);
}

bool MenuCacheManagerI::isValid(const Ice::Current& current) {
  IceUtil::RWRecMutex::RLock lock(validMutex_);
  return isValid_;
}

void MenuCacheManagerI::setValid(bool newState, const Ice::Current& current) {
  MCE_INFO("MenuCacheManagerI::setValid valid = " << newState);
  IceUtil::RWRecMutex::WLock lock(validMutex_);
  isValid_ = newState;
  if (isValid_) {
    ServerStateSubscriber::instance().setStatus(1);
  } else {
    ServerStateSubscriber::instance().setStatus(0);
  }
}

void MenuCacheManagerI::setData(const MyUtil::ObjectResultPtr& objects, CacheType type,
      const Ice::Current& current) {
}

ReloadFromTripodThread::ReloadFromTripodThread(const string& menuType, int objCacheCategory) : objCacheCategory_(objCacheCategory) {
  tripodClient_ = new TripodClient(TRIPODCACHE_ZOOKEEPER_ADDR, TRIPODCACHE_NAMESPACE, menuType);
}

void ReloadFromTripodThread::add(const KeySeq& keys) {
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(mutex_);
  this->keysToReload_.insert(this->keysToReload_.begin(), keys.begin(), keys.end());
  mutex_.notify();
}

void ReloadFromTripodThread::run() {
  while (true) {
    KeySeq keysToReload;
    {
      IceUtil::Monitor<IceUtil::Mutex>::Lock lock(mutex_);
      if (this->keysToReload_.empty()) {
        mutex_.wait();
      }
      keysToReload.insert(keysToReload.end(), this->keysToReload_.begin(), this->keysToReload_.end());
      this->keysToReload_.clear();
    }
    if (!keysToReload.empty()) {
      sleep(10);
      //MCE_INFO("[ReloadFromTripodThread::run] load keys.size = " << keysToReload.size());
      this->tripodClient_->load(keysToReload, 2000, true);
      //刪除掉ObjectCache相应的内容，因为TripodCache已经是最新的了
      vector<long> userIds = MenuCacheManagerI::convertStringSeq2LongSeq(keysToReload);
      ServiceI::instance().getObjectCache()->removeObjects(objCacheCategory_, userIds);
    }
  }
}
