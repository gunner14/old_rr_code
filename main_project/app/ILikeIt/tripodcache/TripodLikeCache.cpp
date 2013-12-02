/*
 * =====================================================================================
 *
 *       Filename:  TripodLikeCache.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年01月11日 11时07分20秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#include "TripodLikeCache.h"

#include "UtilCxx/src/Allocator.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/TimeCost.h"
#include "tripodlike.pb.h"
#include "util/cpp/IntegerSeqSerialize.h"

using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace com::xiaonei::platform::storage;
using namespace xce::usercache; 
using namespace xce::serverstate;
using namespace xce::clusterstate;
using namespace xce::ilikeit;
using namespace xce::tripodlike;

// ================ MyUtil::initialize =====================
void MyUtil::initialize() {

  ServiceI& service = ServiceI::instance();
  service.registerObjectCache(ILIKEIT_CACHE, "IL", new ILikeItCacheFactoryI);
  service.getAdapter()->add(&ILikeItCacheI::instance(), service.createIdentity("M", ""));

  //int cluster = service.getCommunicator()->getProperties()->getPropertyAsInt("ILikeItCache.Cluster");
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("ILikeItCache.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("ILikeItCache.Interval", 5);
  //int syncCacheNum = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("ILikeItCache.SyncCache", 1);
  string ctlEndpoint = service.getCommunicator()
    ->getProperties()->getPropertyWithDefault("ILikeItCache.ControllerEndpoint", "ControllerILikeCacheM");

  // 加入向Controller注册
  ServerStateSubscriber::instance().initialize(ctlEndpoint,
      &ILikeItCacheI::instance(), mod, interval);
  ILikeItCacheI::instance().init(TRIPOD_BUSINESS_NAME);
  HeadUrlUtil::instance();
  //ServerStateSubscriber::instance().setStatus(0);
}

void ILikeItCacheI::init(const string& businessName) {
  tripodClient_ = new TripodClient(TRIPOD_ZK_ADDR, TRIPOD_NAMESPACE, businessName);
  reloadThreadPtr_ = new ReloadFromTripodThread(businessName);
  reloadThreadPtr_->start(128*1024).detach();
}

int ILikeItCacheI::getLikeCount(const GlobalIdPtr& gid, const Ice::Current& current) {
  return getWithKey(gid)->users.size();
}

MyUtil::IntSeq ILikeItCacheI::getLikeCache(const GlobalIdPtr& gid, const Ice::Current& current) {
  return getWithKey(gid)->users;
}

template<class Type, class TypePtr>
TypePtr ILikeItCacheI::getLikeInfo(const GlobalIdPtr& gid, ::Ice::Int uid, ::Ice::Int limit, bool showStrangers) {
  TypePtr result = new Type;
  result->detailLimit = limit;
  result->showStrangers = showStrangers;
  result->withFriendList = (limit > 0) ? true : false;

  // 获取对象本身所有顶过的用户id
  MyUtil::IntSeq users = getWithKey(gid)->users;
  vector<int> friendList;
  if ( result->withFriendList ) {
    result->withFriendList = getFriendList(friendList, uid);
  }
  // 取得总数
  result->totalCount = users.size();
  result->selfCount = 0;
  if(users.empty()){
    return result;
  }
  if(limit==0){
    result->selfCount =( binary_search(users.begin(), users.end(), uid)? 1:0);
    return result;
  }
  vector<int> usersSort;
  getCountAndSort(usersSort, result->selfCount, result->friendCount, users, uid, friendList, result->withFriendList, result->showStrangers, limit);
  // 加入用户详情
  if (limit > 0) {
    UserDetail2Seq userDetails = getUserDetail(usersSort);
    assign(result->detailList, userDetails);
  }
  return result;
}

LikeInfoPtr ILikeItCacheI::getLikeDetail(const GlobalIdPtr& gid, ::Ice::Int uid, ::Ice::Int limit, bool showStrangers, const Ice::Current& current) {
  LikeInfoPtr result = getLikeInfo<LikeInfo, LikeInfoPtr>(gid, uid, limit, showStrangers);
  return result;
}

LikeInfo2Ptr ILikeItCacheI::getLikeDetail2(const GlobalIdPtr& gid, ::Ice::Int uid, ::Ice::Int limit, bool showStrangers, const Ice::Current& current) {
  LikeInfo2Ptr result = getLikeInfo<LikeInfo2, LikeInfo2Ptr>(gid, uid, limit, showStrangers);
  return result;
}

template<class Type, class TypePtr>
vector<TypePtr> ILikeItCacheI::getLikeInfoBatch(const GlobalIdSeq& gids, ::Ice::Int uid, ::Ice::Int limit, ::Ice::Int detailNum) {
  TimeCost tc = TimeCost::create("GetLikeDetailBatch");
  vector<TypePtr> results;
  // 获取对象本身所有顶过的用户id
  LikeDataMap likeMap = getWithKeys(gids);
  // 获取的指定用户的好友列表(只有获取详情时才取)
  vector<int> friendList;
  bool withFriends = false;
  if ( (limit > 0) && (detailNum > 0) ) {
    withFriends = getFriendList(friendList, uid);
  }

  // 依次处理各对象
  for ( size_t i = 0; i < gids.size(); ++i ) {
    TypePtr result = new Type();
    result->detailLimit = (i < (size_t)detailNum)?limit:0;
    result->showStrangers = (i < (size_t)detailNum)?true:false;
    result->withFriendList = (i < (size_t)detailNum)?withFriends:false;

    LikeDataMap::iterator itL = likeMap.find(GlobalIdTrans::GlobalId2Long(gids[i]));
    if ( itL == likeMap.end() ) {
      result->selfCount = 0;
      result->friendCount = 0;
      result->totalCount = 0;
      results.push_back(result);
      continue;
    }

    // 获取对象本身所有顶过的用户id
    MyUtil::IntSeq users = (*itL).second->users;
    // 取得总数
    result->totalCount = users.size();

    // 排序、组合
    vector<int> usersSort;
    getCountAndSort(usersSort, result->selfCount, result->friendCount, users, uid, friendList, result->withFriendList, result->showStrangers, result->detailLimit);

    // 加入用户详情
    if ( result->detailLimit > 0 ) {
      UserDetail2Seq userDetails = getUserDetail(usersSort);
      assign(result->detailList, userDetails);
    }
    results.push_back(result);
  }
  return results;
}

LikeInfoSeq ILikeItCacheI::getLikeDetailBatch(const GlobalIdSeq& gids, ::Ice::Int uid, ::Ice::Int limit, ::Ice::Int detailNum, const Ice::Current& current) {
  return getLikeInfoBatch<LikeInfo, LikeInfoPtr>(gids, uid, limit, detailNum);
}

LikeInfo2Seq ILikeItCacheI::getLikeDetailBatch2(const GlobalIdSeq& gids, ::Ice::Int uid, ::Ice::Int limit, ::Ice::Int detailNum, const Ice::Current& current) {
  return getLikeInfoBatch<LikeInfo2, LikeInfo2Ptr>(gids, uid, limit, detailNum);
}

LikeInfoSimplePtr ILikeItCacheI::getLikeInfoSimple(const GlobalIdPtr& gid, ::Ice::Int limit, const Ice::Current& current) {
  LikeInfoSimplePtr result = new LikeInfoSimple;
  LikeDataPtr likeData = NULL;
  likeData = getWithKey(gid);
  if ( likeData != 0 ) {
    result->totalCount = likeData->users.size();
    if ( limit >= result->totalCount ) {
      result->users = likeData->users;
    } else {
      result->users.assign(likeData->users.begin(), likeData->users.begin()+limit);
    }
  } else {
    result->totalCount = 0;
  }
  return result;
}

LikeInfoSimpleSeq ILikeItCacheI::getLikeInfoSimpleBatch(const GlobalIdSeq& gids, int limit, const Ice::Current& current) {
  LikeInfoSimpleSeq result;
  LikeDataMap likeMap = getWithKeys(gids);
  for (GlobalIdSeq::const_iterator it = gids.begin(); it != gids.end(); ++it) {
    long gidLong = GlobalIdTrans::GlobalId2Long(*it);
    LikeDataMap::iterator itL = likeMap.find(gidLong);
    LikeInfoSimplePtr simple = new LikeInfoSimple;
    if ( itL != likeMap.end() ) {
      simple->totalCount = (*itL).second->users.size();
      if ( limit >= simple->totalCount ) {
        simple->users = (*itL).second->users;
      } else {
        simple->users.assign((*itL).second->users.begin(), (*itL).second->users.begin()+limit);
      }
    } else {
      simple->totalCount = 0;
    }
    result.push_back(simple);
  }
  return result;
}

int ILikeItCacheI::isLiked(const GlobalIdPtr& gid, int uid, const Ice::Current& current) {
  LikeDataPtr likeData = getWithKey(gid);
  return binary_search(likeData->users.begin(), likeData->users.end(), uid) ? 1 : 0;
}

MyUtil::IntSeq ILikeItCacheI::isLikedBatch(const GlobalIdSeq& gids, int uid, const Ice::Current& current) {
  MyUtil::IntSeq result;
  LikeDataMap likeMap = getWithKeys(gids);
  for (GlobalIdSeq::const_iterator it = gids.begin(); it != gids.end(); ++it) {
    long gidLong = GlobalIdTrans::GlobalId2Long(*it);
    int liked = 0;
    LikeDataMap::iterator itL = likeMap.find(gidLong);
    if ( itL != likeMap.end() ) {
      liked = binary_search((*itL).second->users.begin(), (*itL).second->users.end(), uid)?1:0;
    }
    result.push_back(liked);
  }
  return result;
}

void ILikeItCacheI::load(const GlobalIdSeq& gids, const Ice::Current& current) {
  reloadThreadPtr_->add(TripodLikeUtils::getKeySeq(gids));
}

void ILikeItCacheI::setData(const ::MyUtil::ObjectResultPtr& objects, const Ice::Current& current) {
}

void ILikeItCacheI::addLikeship(const LikeshipPtr& ships, const Ice::Current& current) {
  MCE_INFO("[ILikeItCacheI::addLikeship] userId = " << ships->uid << ", gid.type = " << ships->gid->type << ", gid.id = " << ships->gid->id);
  //LikeDataPtr ptr = ServiceI::instance().findObject<LikeDataPtr>(ILIKEIT_CACHE, GlobalIdTrans::GlobalId2Long(ship->gid));
  LikeDataPtr ptr = getWithKey(ships->gid);
  vector<int>::iterator it = lower_bound(ptr->users.begin(), ptr->users.end(), ships->uid);
  if (it == ptr->users.end() || *it != ships->uid){
    //MCE_INFO("[ILikeItCacheI::addLikeship] add user = " << ships->uid << " and add ptr to ObjectCache");
    ptr->users.insert(it, ships->uid);
    ServiceI::instance().getObjectCache()->addObject(ILIKEIT_CACHE, GlobalIdTrans::GlobalId2Long(ships->gid), ptr);
    reloadThreadPtr_->add(boost::lexical_cast<string>(GlobalIdTrans::GlobalId2Long(ships->gid)));
  }
}
	
void ILikeItCacheI::removeLikeship(const LikeshipPtr& ships, const Ice::Current& current) {
  MCE_INFO("[ILikeItCacheI::removeLikeship] userId = " << ships->uid << ", gid.type = " << ships->gid->type << ", gid.id = " << ships->gid->id);
  LikeDataPtr ptr = getWithKey(ships->gid);
  MyUtil::IntSeq::iterator itPos = find(ptr->users.begin(), ptr->users.end(), ships->uid);
  if (itPos != ptr->users.end()) {
    ServiceI::instance().getObjectCache()->addObject(ILIKEIT_CACHE, GlobalIdTrans::GlobalId2Long(ships->gid), ptr);
    ptr->users.erase(itPos);
    reloadThreadPtr_->add(boost::lexical_cast<string>(GlobalIdTrans::GlobalId2Long(ships->gid)));
  }
}

void ILikeItCacheI::setValid(bool newState, const Ice::Current& current) {
  /*MCE_INFO("ILikeItCacheI::setValid -> "<< newState);
  IceUtil::RWRecMutex::RLock lock(_validMutex);
  _valid = newState;
  ServerStateSubscriber::instance().setStatus((int)newState);*/
}

bool ILikeItCacheI::isValid(const Ice::Current& current) {
  //IceUtil::RWRecMutex::WLock lock(_validMutex);
  //return _valid;
  return true;
}

bool ILikeItCacheI::getFriendList(vector<int>& friends, int uid) {
  try {
    friends = xce::buddy::adapter::BuddyByIdCacheAdapter::instance().getFriendListAsc(uid, FRIENDS_LIMIT);
  } catch (Ice::Exception& e) {
    return false;
    MCE_WARN("[ILikeItCacheI::getFriends] getFriendList of " << uid << " Ice::Excetpion : " << e.what());
  } catch (std::exception& e) {
    return false;
    MCE_WARN("[ILikeItCacheI::getFriends] getFriendList of " << uid << " std::excetpion : " << e.what());
  } catch (...) {
    return false;
    MCE_WARN("[ILikeItCacheI::getFriends] getFriendList of " << uid << " Excetpion");
  }
  return true;
}

void ILikeItCacheI::getCountAndSort(vector<int>& usersSort, int& selfCount, int& friendCount, const vector<int>& users, int selfId, const vector<int>& friendList, bool withFriends, bool showStrangers, int limit) {

  selfCount =( binary_search(users.begin(), users.end(), selfId)? 1:0);
  MyUtil::IntSeq usersFriends, usersOther;
  int otherSize = 0;
  for ( MyUtil::IntSeq::const_iterator it = users.begin(); it != users.end(); ++it) {
    if((*it)==selfId) {
      continue;
    }
    if ( binary_search(friendList.begin(), friendList.end(), (*it)) ) {
      usersFriends.push_back(*it);
      if(int(usersFriends.size())>=limit)
        break;
    } else if ( otherSize < limit ) { // 陌生人列表最多保存limit个,多了没有必要
      usersOther.push_back(*it);
      ++otherSize;
      if(otherSize>=limit && !withFriends)
        break;
    }
  }
  friendCount = usersFriends.size();

  // 进行排序
  usersSort.clear();
  if ( limit > 0 ) {
    // 生成列表
    if ( selfCount > 0 ) {
      usersSort.push_back(selfId);
    }
    size_t FEnd = (usersFriends.size()<(limit-usersSort.size()))?
      (usersFriends.size()):(limit-usersSort.size());
    for ( size_t i = 0; i < FEnd; i++ ) {
      usersSort.push_back(usersFriends[i]);
    }
    if ( ( showStrangers ) && ( usersSort.size() < size_t(limit)) ) {
      size_t OEnd = (usersOther.size()<(limit - usersSort.size()))?
        (usersOther.size()):(limit - usersSort.size());
      for ( size_t i = 0; i < OEnd; i++ ) {
        usersSort.push_back(usersOther[i]);
      }
    }
  }
}

map<long, LikeDataPtr> ILikeItCacheI::getWithKeys(const vector<string>& keySeq) {
  map<long, LikeDataPtr> resultMap;
  KeySeq missedKeys;
  DataMap dataMap = tripodClient_->get(keySeq, missedKeys);
  reloadThreadPtr_->add(missedKeys);
  for (DataMap::const_iterator it = dataMap.begin(); it != dataMap.end(); ++it) {
    string byteArray(it->second.begin(), it->second.end());
    TripodLikeDataPtr ptr = new TripodLikeData(byteArray);
    resultMap.insert(make_pair<long, LikeDataPtr>(boost::lexical_cast<long>(it->first), ptr->parseTo()));
  }
  return resultMap;
}

map<long, LikeDataPtr> ILikeItCacheI::getWithKeys(const GlobalIdSeq& gids) {
  vector<long> longIds = TripodLikeUtils::getLongSeq(gids);
  map<long, LikeDataPtr> resultMap;
  MyUtil::ObjectResultPtr ptr = ServiceI::instance().getObjectCache()->findObjects(ILIKEIT_CACHE, longIds);
  KeySeq keySeq;
  for (vector<long>::const_iterator it = longIds.begin(); it != longIds.end(); ++it) {
    map<long, Ice::ObjectPtr>::iterator itm = ptr->data.find(*it);
    if (itm == ptr->data.end()) {
      keySeq.push_back(boost::lexical_cast<string>(*it));
    } else {
      resultMap.insert(make_pair<long, LikeDataPtr>(*it, LikeDataPtr::dynamicCast(itm->second)));
    }
  }
  map<long, LikeDataPtr> likeDataFromTripodCache = getWithKeys(keySeq);
  resultMap.insert(likeDataFromTripodCache.begin(), likeDataFromTripodCache.end());
  return resultMap;
}

UserDetail2Seq ILikeItCacheI::getUserDetail(const vector<int>& uids) {
  map<int, UserCachePtr> usersCache;
  map<int, UserDetail2Ptr> userMap;
  try {
    usersCache = UserCacheAdapter::instance().GetUserCache(uids);
  } catch (Ice::Exception& e) {
    MCE_WARN("[ILikeItCacheI::getLikeDetail] getUserCacheList Ice::Excetpion : " << e.what());
  } catch (std::exception& e) {
    MCE_WARN("[ILikeItCacheI::getLikeDetail] getUserCacheList std::excetpion : " << e.what());
  } catch (...) {
    MCE_WARN("[ILikeItCacheI::getLikeDetail] getUserCacheList Excetpion");
  }
  MCE_DEBUG("[ILikeItCacheI::getLikeDetail] get " << usersCache.size() << " userdetails for " << uids.size() << " users");

  for ( map<int, UserCachePtr>::iterator it = usersCache.begin(); it != usersCache.end(); ++it) {
    UserDetail2Ptr detail = new UserDetail2;
    detail->uid = it->first;
    detail->name = it->second->name();
    string imgOri = it->second->tinyurl();
    detail->img = HeadUrlUtil::instance().getHeadFullUrl(imgOri);
    MCE_DEBUG("UserId : "<<it->first<<" IsKEEPUSE: "<<it->second->IsKEEPUSE());
    detail->isKeepUse= it->second->IsKEEPUSE();
    // detail->img = imgOri;

    if ( it->first >=  0 ) {
      userMap.insert( make_pair<int, UserDetail2Ptr>(it->first, detail) );
    }
  }
  UserDetail2Seq details;
  for ( vector<int>::const_iterator itUser = uids.begin();
      itUser != uids.end(); ++itUser) {
    if ( userMap.find(*itUser) != userMap.end() ) {
      details.push_back(userMap[*itUser]);
    } else {
      UserDetail2Ptr detail = new UserDetail2;
      detail->uid = (*itUser);
      detail->name = "";
      detail->img = "";
      detail->isKeepUse= false;
      details.push_back(detail);
    }
  }
  return details;
}

void ILikeItCacheI::assign(UserDetail2Seq& userDetails, const UserDetail2Seq& userDetail2Seq) {
  userDetails = userDetail2Seq;
}

void ILikeItCacheI::assign(UserDetailSeq& userDetails, const UserDetail2Seq& userDetail2Seq) {
  for (UserDetail2Seq::const_iterator it = userDetail2Seq.begin(); it != userDetail2Seq.end(); ++it) {
    UserDetailPtr ptr = new UserDetail;
    ptr->uid = (*it)->uid;
    ptr->name = (*it)->name;
    ptr->img = (*it)->img;
    userDetails.push_back(ptr);
  }
}

LikeDataPtr ILikeItCacheI::getWithKey(const GlobalIdPtr& gid) {
  long key = GlobalIdTrans::GlobalId2Long(gid);
  //MCE_INFO("[ILikeItCacheI::getWithKey] gid = " << key);
  LikeDataPtr ptrInObjectCache = ServiceI::instance().findObject<LikeDataPtr>(ILIKEIT_CACHE, key);
  if (ptrInObjectCache != 0) {
    //MCE_INFO("[ILikeItCacheI::getWithKey] gid.type = " << gid->type << ", gid.id = " << gid->id << ", found in ObjectCache");
    return ptrInObjectCache;
  }
  //MCE_INFO("[ILikeItCacheI::getWithKey] gid = " << key << " no object exist in ObjectCache");
  LikeDataPtr ptr = new LikeData;
  KeySeq missedKeys;
  KeySeq keySeq = TripodLikeUtils::getKeySeq(key);
  DataMap dataMap = tripodClient_->get(keySeq, missedKeys);
  reloadThreadPtr_->add(missedKeys);
  if (!dataMap.empty()) {
    DataMap::const_iterator it = dataMap.begin();
    string byteArray(it->second.begin(), it->second.end());
    return (new TripodLikeData(byteArray))->parseTo(); 
  } 
  return ptr;
}

ReloadFromTripodThread::ReloadFromTripodThread(const string& businessName) {
  tripodClient_ = new TripodClient(TRIPOD_ZK_ADDR, TRIPOD_NAMESPACE, businessName);
}

void ReloadFromTripodThread::add(const KeySeq& keys) {
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(mutex_);
  this->keysToReload_.insert(this->keysToReload_.begin(), keys.begin(), keys.end());
  mutex_.notify();
}

void ReloadFromTripodThread::add(const string& key) {
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(mutex_);
  this->keysToReload_.insert(this->keysToReload_.begin(), key);
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
      this->tripodClient_->load(keysToReload, 2000, true);
      //MCE_INFO("[ReloadFromTripodThread::run] removeObject keys.size = " << keysToReload.size());
      ServiceI::instance().getObjectCache()->removeObjects(ILIKEIT_CACHE, TripodLikeUtils::getLongSeq(keysToReload));
    }
  }
}


TripodLikeData::TripodLikeData(const LikeDataPtr& ptr) {
  this->userIds_ = ptr->users;
}

TripodLikeData::TripodLikeData(const string& byteArray) {
  this->userIds_ = IntegerSeqSerialize<int>::unserialize(byteArray);
}

LikeDataPtr TripodLikeData::parseTo() const {
  LikeDataPtr ptr = new LikeData;
  IceUtil::Mutex::Lock lock(*this);
  ptr->users = this->userIds_;
  return ptr;
}

string TripodLikeData::serialize() const {
  IceUtil::Mutex::Lock lock(*this);
  return IntegerSeqSerialize<int>::serialize(this->userIds_);
}

vector<int> TripodLikeData::getUserIds() const {
  IceUtil::Mutex::Lock lock(*this);
  return this->userIds_;
}

vector<string> TripodLikeUtils::getKeySeq(const vector<long>& gids) {
  vector<string> keySeq;
  for (vector<long>::const_iterator it = gids.begin(); it != gids.end(); ++it) {
    keySeq.push_back(boost::lexical_cast<string>(*it));
  }
  return keySeq;
}

vector<string> TripodLikeUtils::getKeySeq(const GlobalIdSeq& gids) {
  vector<string> keySeq;
  for (GlobalIdSeq::const_iterator it = gids.begin(); it != gids.end(); ++it) {
    keySeq.push_back(boost::lexical_cast<string>(GlobalIdTrans::GlobalId2Long(*it)));
  }
  return keySeq;
}

vector<string> TripodLikeUtils::getKeySeq(long gid) {
  vector<string> keySeq;
  keySeq.push_back(boost::lexical_cast<string>(gid));
  return keySeq;
}

vector<long> TripodLikeUtils::getLongSeq(const vector<string>& keys) {
  vector<long> longGids;
  for (vector<string>::const_iterator it = keys.begin(); it != keys.end(); ++it) {
    longGids.push_back(boost::lexical_cast<long>(*it));
  }
  return longGids;
}

vector<long> TripodLikeUtils::getLongSeq(const GlobalIdSeq& gids) {
  vector<long> longGids;
  for (GlobalIdSeq::const_iterator it = gids.begin(); it != gids.end(); ++it) {
    longGids.push_back(GlobalIdTrans::GlobalId2Long(*it));
  }
  return longGids;
}
