/*
 * =====================================================================================
 *
 *       Filename:  MenuTripodShare.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月10日 11时09分22秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#include "MenuTripodShare.h"

using namespace xce::menutripod;

int MenuTripodData::getMaxNum(CacheType type) {
  return type == RECENTLY ? MAX_RECENTLY_APP_NUM : MAX_FAVORITE_APP_NUM; 
}

MenuTripodData::MenuTripodData(int userId, CacheType cacheType) : userId_(userId), cacheType_(cacheType), maxNum_(getMaxNum(cacheType)) {
}

MenuTripodData::MenuTripodData(const MenuCacheRecordPtr& record) : userId_(record->userId), 
    cacheType_(record->type), appIds_(record->infoArray), maxNum_(getMaxNum(record->type)) {
}

MenuCacheRecordPtr MenuTripodData::parseTo() {
  IceUtil::Mutex::Lock lock(*this);
  MenuCacheRecordPtr ptr = new MenuCacheRecord;
  ptr->userId = this->userId_;
  ptr->type = this->cacheType_;
  ptr->infoArray = this->appIds_;
  return ptr;
}

MenuTripodData::MenuTripodData(int userId, CacheType cacheType, const vector<int>& appIds) : userId_(userId), cacheType_(cacheType),
    appIds_(appIds), maxNum_(getMaxNum(cacheType)) {
}

MenuTripodData::MenuTripodData(const string& byteArray) {
  MenuData menuData;
  istringstream in(byteArray);
  menuData.ParseFromIstream(&in);
  this->userId_ = menuData.userid();
  this->cacheType_ = (CacheType) menuData.cachetype();
  this->maxNum_ = getMaxNum(this->cacheType_); 
  for (int i = 0 ; i < menuData.appids_size(); ++i) {
    this->appIds_.push_back(menuData.appids(i));
  }
}

CacheType MenuTripodData::getCacheType() const {
  IceUtil::Mutex::Lock lock(*this);
  return this->cacheType_;
}

void MenuTripodData::setCacheType(CacheType cacheType) {
  IceUtil::Mutex::Lock lock(*this);
  this->cacheType_ = cacheType;
  this->maxNum_ = getMaxNum(this->cacheType_); 
}

int MenuTripodData::getUserId() const {
  IceUtil::Mutex::Lock lock(*this);
  return this->userId_;
}

void MenuTripodData::setUserId(int pUserId) {
  IceUtil::Mutex::Lock lock(*this);
  this->userId_ = pUserId;
}

vector<int> MenuTripodData::getAppIds() const {
  IceUtil::Mutex::Lock lock(*this);
  return this->appIds_;
}

void MenuTripodData::setAppIds(const vector<int>& appIds) {
  IceUtil::Mutex::Lock lock(*this);
  vector<int> tempAppIds(appIds.begin(), appIds.end());
  this->appIds_.swap(tempAppIds);
  if (this->appIds_.size() > (size_t)maxNum_) {
    this->appIds_.resize(maxNum_);
  }
}

void MenuTripodData::addAppIds(const vector<int>& appIds) {
  IceUtil::Mutex::Lock lock(*this);
  for (vector<int>::const_iterator it = appIds.begin(); it != appIds.end(); ++it) {
    int index = -1;
    for (size_t i = 0; i < this->appIds_.size(); ++i) {
      if (this->appIds_[i] == *it) {
        index = i;
        break;
      }
    }
    if (index == -1) {
      this->appIds_.insert(this->appIds_.begin(), *it);
    } else {
      for (int j = index; j > 0; --j) {
        this->appIds_[j] = this->appIds_[j - 1];
      }
      this->appIds_[0] = *it;
    }
  }
  if (this->appIds_.size() > (size_t)maxNum_) {
    this->appIds_.resize(maxNum_);
  }
}

void MenuTripodData::deleteAppIds(const vector<int>& appIds) {
  IceUtil::Mutex::Lock lock(*this);
  vector<int> tempAppIds;
  for (vector<int>::const_iterator it = this->appIds_.begin(); it != this->appIds_.end(); ++it) {
    if (find(appIds.begin(), appIds.end(), *it) == appIds.end()) {
      tempAppIds.push_back(*it);
    }
  }
  if (this->appIds_.size() > tempAppIds.size()) {
    this->appIds_.swap(tempAppIds);
  }
}

string MenuTripodData::serialize() const {
  MenuData menuData;
  {
    IceUtil::Mutex::Lock lock(*this);
    menuData.set_userid(this->userId_);
    menuData.set_cachetype((int)this->cacheType_);
    for (vector<int>::const_iterator it = this->appIds_.begin(); it != this->appIds_.end(); ++it) {
      menuData.add_appids(*it);
    }
  }
  ostringstream out;
  menuData.SerializeToOstream(&out);
  return out.str();
}

bool BatchMenuTripodDataHandler::handle(mysqlpp::Row& row) const {
  int userId = (int)row[COLUMN_USERID.c_str()];
  vector<int> appIds = MyUtil::IntegerSeqSerialize<int>::unserialize(row[COLUMN_APP_IDS.c_str()]);
  results_.push_back(new MenuTripodData(userId, cacheType_, appIds));
  return true;
}

string TripodHelper::getCacheTypeStr(CacheType cacheType) {
  if (cacheType == FAVORITE) {
    return STR_CACHE_TYPE_FAVORITE;
  } else if (cacheType == RECENTLY) {
    return STR_CACHE_TYPE_RECENTLY;
  } else {
    return STR_CACHE_TYPE_INSTALLED;
  }
} 

vector<string> TripodHelper::getKeySeq(int userId, const CacheTypeSeq& types) {
  vector<string> keys;
  for (CacheTypeSeq::const_iterator it = types.begin(); it != types.end(); ++it) {
    if (*it == INSTALLED) {
      continue;
    }
    keys.push_back(getCacheTypeStr(*it) + SPLITTOR + boost::lexical_cast<string>(userId));
  } 
  return keys;
} 


vector<string> TripodHelper::getKeySeq(const MenuCacheRecordSeq& records) {
  vector<string> keys;
  for (MenuCacheRecordSeq::const_iterator it = records.begin(); it != records.end(); ++it) {
    keys.push_back(getKey(*it));
  }
  return keys;
}

vector<string> TripodHelper::getKeySeq(const vector<MenuTripodDataPtr>& ptrs) {
  vector<string> keys;
  for (vector<MenuTripodDataPtr>::const_iterator it = ptrs.begin(); it != ptrs.end(); ++it) {
    keys.push_back(getKey(*it));
  }
  return keys;
}

string TripodHelper::getKey(const MenuTripodDataPtr& ptr) {
 return getCacheTypeStr(ptr->getCacheType()) + SPLITTOR + boost::lexical_cast<string>(ptr->getUserId());
}

string TripodHelper::getKey(const MenuCacheRecordPtr& ptr) {
 return getCacheTypeStr(ptr->type) + SPLITTOR + boost::lexical_cast<string>(ptr->userId);
}

CacheType TripodHelper::getCacheType(const string& key) {
  if (key.find(STR_CACHE_TYPE_RECENTLY) != string::npos) {
    return RECENTLY;
  } else if (key.find(STR_CACHE_TYPE_FAVORITE) != string::npos) {
    return FAVORITE;
  } else {
    return INSTALLED;
  }
}

int TripodHelper::getUserId(const string& key) {
  size_t pos = key.find_first_of(SPLITTOR);
  if (pos != string::npos) {
    return boost::lexical_cast<int>(key.substr(pos + 1));
  }
  return -1;
}

string TripodHelper::getDBSource(CacheType cacheType) {
  if (cacheType == RECENTLY) {
    return DB_DESC_RECMENUCACHE;
  } else if (cacheType == FAVORITE) {
    return DB_DESC_FAVMENUCACHE;
  }
  return "";
}

string TripodHelper::getTablePrefix(CacheType cacheType) {
  if (cacheType == RECENTLY) {
    return REC_TABLE_PREFIX;
  } else if (cacheType == FAVORITE) {
    return FAV_TABLE_PREFIX;
  }
  return "";
}

map<CacheType, vector<int> > TripodHelper::getCacheTypeToUserIds(const vector<string>& keys) {
  map<CacheType, vector<int> > cacheTypeToUserIds;
  for (vector<string>::const_iterator it = keys.begin(); it != keys.end(); ++it) {
    int userId = TripodHelper::getUserId(*it);
    if (userId == -1) {
      continue;
    }
    CacheType type = TripodHelper::getCacheType(*it);
    map<CacheType, vector<int> >::iterator itm = cacheTypeToUserIds.find(type);
    if (itm == cacheTypeToUserIds.end()) {
      vector<int> userIds;
      userIds.push_back(userId);
      cacheTypeToUserIds.insert(make_pair<CacheType, vector<int> >(type, userIds));
    } else {
      itm->second.push_back(userId);
    }
  }
  return cacheTypeToUserIds;
}

