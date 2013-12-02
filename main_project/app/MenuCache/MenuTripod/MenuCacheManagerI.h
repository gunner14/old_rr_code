/*
 * =====================================================================================
 *
 *       Filename:  MenuTripod.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011年12月14日 16时44分00秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */

#ifndef __MENU_TRIPOD_H__
#define __MENU_TRIPOD_H__

#include <Ice/Ice.h>
#include "MenuCache.h"
#include "MenuTripodShare.h"
#include "Singleton.h"
#include "tripod-new/src/cpp/include/TripodClient.h"
#include <IceUtil/IceUtil.h>
#include "ServiceI.h"

namespace xce {
namespace menutripod {

using namespace com::renren::tripod;
using namespace xce::menu;
using namespace MyUtil;

enum OpType {Add, Delete};

const string TRIPODCACHE_ZOOKEEPER_ADDR = "UserZooKeeper1:2181,UserZooKeeper2:2181,UserZooKeeper3:2181,UserZooKeeper4:2181,UserZooKeeper5:2181/Tripod";
const string TRIPODCACHE_NAMESPACE = "ne0";
const string TRIPODCACHE_RECENT_MENU = "RecentMenu";
const string TRIPODCACHE_FAVORITE_MENU = "FavoriteMenu";

const int RECMENU_CACHE_CATEGORY = 0;
const int FAVMENU_CACHE_CATEGORY = 2;

class ReloadFromTripodThread : public IceUtil::Thread {
public:
  ReloadFromTripodThread(const string& menuType, int objCacheCategory);
  void add(const KeySeq& keys);
  virtual void run();
private:
  IceUtil::Monitor<IceUtil::Mutex> mutex_;
  KeySeq keysToReload_;
  TripodClient* tripodClient_;
  int objCacheCategory_;
};

typedef IceUtil::Handle<ReloadFromTripodThread> ReloadFromTripodThreadPtr;

class MenuCacheManagerI : virtual public MenuCacheManager, public MyUtil::Singleton<MenuCacheManagerI> {
public:
  void init();
  //类型数组typeArray，0返回ALL数组，1返回REC数组，2返回FAV数组
  virtual CacheType2AppIdSeqMap get(::Ice::Int, const CacheTypeSeq&, const Ice::Current& current = Ice::Current());

  virtual CacheType2AppIdSeqMap getAll(::Ice::Int, const Ice::Current&);

  virtual void set(::Ice::Int, CacheType, const AppIdSeq&, const Ice::Current&);

  virtual void setBatch(const MenuCacheRecordSeq& records, const Ice::Current&);
  virtual void addBatch(const MenuCacheRecordSeq& records, const Ice::Current& current = Ice::Current());
  virtual void deleteBatch(const MenuCacheRecordSeq& records, const Ice::Current& current = Ice::Current());

  virtual void load(const UserIdSeq&, const Ice::Current& current = Ice::Current());

  virtual bool isValid(const Ice::Current& = Ice::Current());

  virtual void setValid(bool newState, const Ice::Current& = Ice::Current());
  virtual void setData(const MyUtil::ObjectResultPtr& objects, CacheType type,
      const Ice::Current& = Ice::Current());
  static vector<string> convertIntSeq2StringSeq(const vector<int>& userIds);
  static vector<long> convertStringSeq2LongSeq(const vector<string>& userIds);
private:
  bool isValid_;
  void addKeysToLoad(const vector<string>& userIds, CacheType cacheType);
  vector<MenuTripodDataPtr> getWithUserId(int userId, const CacheTypeSeq& types);
  MenuTripodDataPtr getWithUserId(int userId, CacheType cacheType);
  vector<MenuTripodDataPtr> parseFromMenuCacheRecordSeq(const MenuCacheRecordSeq& records);
  void updateCache(const vector<MenuTripodDataPtr>& menuDatas);
  TripodClient* getTripodClientWithCacheType(CacheType cacheType);
  int getObjCacheCategoryWithCacheType(CacheType cacheType);
  IceUtil::RWRecMutex validMutex_;
  TripodClient* recentMenuTripodClient_;
  TripodClient* favoriteMenuTripodClient_;
  CacheTypeSeq allCacheTypes_;
  ReloadFromTripodThreadPtr loadRecentMenuThread_;
  ReloadFromTripodThreadPtr loadFavoriteMenuThread_;
};

class RecentMenuCacheDataFactory: virtual public MyUtil::ServantFactory {
public:
  virtual Ice::ObjectPtr create(Ice::Int id) {
    return 0;
  }
  virtual MyUtil::ObjectResultPtr create(MyUtil::IntSeq& ids) {
    return 0;
  }
};

class FavoriteMenuCacheDataFactory: virtual public MyUtil::ServantFactory {
public:
  virtual Ice::ObjectPtr create(Ice::Int id) {
    return 0;
  }
  virtual MyUtil::ObjectResultPtr create(MyUtil::IntSeq& ids) {
    return 0;
  }
};


}
}
#endif
