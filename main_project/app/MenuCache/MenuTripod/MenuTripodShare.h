/*
 * =====================================================================================
 *
 *       Filename:  MenuTripodShare.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011年12月14日 10时43分28秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#ifndef __MENU_TRIPOD_SHARE_H__
#define __MENU_TRIPOD_SHARE_H__

#include "MenuCache.h"
#include <QueryRunner.h>
#include "util/cpp/IntegerSeqSerialize.h"
#include "MenuTripodData.pb.h"

namespace xce {
namespace menutripod {

using namespace xce::menu;

static const int TABLE_CLUSTER = 100;

static const string REC_TABLE_PREFIX = "recent_menu_";
static const string FAV_TABLE_PREFIX = "favorite_menu_";

static const string DB_DESC_RECPATTERN = "menucache_recent_menu_";
static const string DB_DESC_FAVPATTERN = "menucache_favorite_menu_";

static const string DB_DESC_FAVMENUCACHE = "menucache_favorite_menu"; //数据源
static const string DB_DESC_RECMENUCACHE = "menucache_recent_menu"; //数据源

static const string DB_DESC_USERPASSPORT = "user_passport";
static const string COLUMN_USERID = "user_id";
static const string COLUMN_APP_IDS = "app_ids";
static const string TABLE_USER_PASSPORT = "user_passport";

static const int MAX_FAVORITE_APP_NUM = 10;
static const int MAX_RECENTLY_APP_NUM = 50;

static const string STR_CACHE_TYPE_FAVORITE = "FAV";
static const string STR_CACHE_TYPE_RECENTLY = "REC";
static const string STR_CACHE_TYPE_INSTALLED = "INS";

static const string SPLITTOR = "_";
/********************************************************************************************/

class MenuTripodData;
typedef IceUtil::Handle<MenuTripodData> MenuTripodDataPtr;

class MenuTripodData : public Ice::Object, virtual public IceUtil::Mutex {
public:
  MenuTripodData(int userId, CacheType cacheType);
  MenuTripodData(const MenuCacheRecordPtr& record);
  MenuTripodData(int userId, CacheType cacheType, const vector<int>& appIds);
  MenuTripodData(const string& byteArray);
  MenuCacheRecordPtr parseTo();
  CacheType getCacheType() const;
  void setCacheType(CacheType cacheType);
  int getUserId() const;
  void setUserId(int pUserId);
  vector<int> getAppIds() const;
  void setAppIds(const vector<int>& appIds);
  void addAppIds(const vector<int>& appIds);
  void deleteAppIds(const vector<int>& appIds);
  string serialize() const;
  static int getMaxNum(CacheType type);
private:
  int userId_;
  CacheType cacheType_;
  vector<int> appIds_;
  int maxNum_;
};

/*********************************************************************************************/
class BatchMenuTripodDataHandler : public com::xiaonei::xce::ResultHandler {
public:
   BatchMenuTripodDataHandler(vector<MenuTripodDataPtr>& result, CacheType cacheType) 
      : results_(result), cacheType_(cacheType) {
   }
protected:
   virtual bool handle(mysqlpp::Row& row) const;
private:
   vector<MenuTripodDataPtr>& results_;
   CacheType cacheType_;
};

/*********************************************************************************************/
class TripodHelper {
public:
  static string getCacheTypeStr(CacheType cacheType);
  static vector<string> getKeySeq(int userId, const CacheTypeSeq& types);
  static vector<string> getKeySeq(const MenuCacheRecordSeq& records);
  static vector<string> getKeySeq(const vector<MenuTripodDataPtr>& ptrs);
  static string getKey(const MenuCacheRecordPtr& ptr);
  static string getKey(const MenuTripodDataPtr& ptr);
  static CacheType getCacheType(const string& key);
  static int getUserId(const string& key);
  static string getDBSource(CacheType cacheType);
  static string getTablePrefix(CacheType cacheType);
  static map<CacheType, vector<int> > getCacheTypeToUserIds(const vector<string>& keys);
};

/*********************************************************************************************/
}
}

#endif
