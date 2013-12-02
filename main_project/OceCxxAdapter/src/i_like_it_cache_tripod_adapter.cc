#include "OceCxxAdapter/src/i_like_it_cache_tripod_adapter.h"
#include "OceProto/proto/Like.pb.h"

namespace xce {
namespace ilikeit {

const std::string ILikeItCacheTripodAdapter::kNameSpace = "xce_like";
const std::string ILikeItCacheTripodAdapter::kBizId = "i_like_it";


ILikeItCacheTripodAdapter::ILikeItCacheTripodAdapter() {
  tripod_client_.reset(new xce::tripod::TripodClient(kNameSpace, kBizId));
}

int ILikeItCacheTripodAdapter::getLikeCount(int type, long id) {
  GlobalIdPtr gid = new GlobalId;
  gid->type = type;
  gid->id = id;
  int count = 0;
  try {
    count = getWithKey(gid)->users.size();
  } catch (Ice::Exception& e) {
    MCE_WARN("[ILikeItCacheTripodAdapter::getLikeCount] " << type << ":" << id 
        << " Ice::Exception : " << e.what());
  } catch (std::exception& e) {
    MCE_WARN("[ILikeItCacheTripodAdapter::getLikeCount] " << type << ":" << id 
        << " std::excetpion : " << e.what());
  } catch (...) {
    MCE_WARN("[ILikeItCacheTripodAdapter::getLikeCount] " << type << ":" << id
        << " exception");
  }
  return count;
}

MyUtil::IntSeq ILikeItCacheTripodAdapter::getLikeCache(int type, long id) {
  GlobalIdPtr gid = new GlobalId;
  gid->type = type;
  gid->id = id;
  try {
    return getWithKey(gid)->users;
  } catch (Ice::Exception& e) {
    MCE_WARN("[ILikeItCacheTripodAdapter::getLikeCount] " << type << ":" << id 
        << " Ice::Exception : " << e.what());
  } catch (std::exception& e) {
    MCE_WARN("[ILikeItCacheTripodAdapter::getLikeCount] " << type << ":" << id 
        << " std::excetpion : " << e.what());
  } catch (...) {
    MCE_WARN("[ILikeItCacheTripodAdapter::getLikeCount] " << type << ":" << id
        << " exception");
  }
  return MyUtil::IntSeq();
}

/*
bool ILikeItCacheTripodAdapter::isValid(long id){
  return true;
}

template<class Type, class TypePtr>
TypePtr ILikeItCacheTripodAdapter::getLikeInfo(const GlobalIdPtr& gid,
  int uid, int limit, bool showStrangers) {
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
  getCountAndSort(usersSort, result->selfCount, result->friendCount,
      users, uid, friendList, result->withFriendList, result->showStrangers, limit);
  // 加入用户详情
  if (limit > 0) {
    UserDetail2Seq userDetails = getUserDetail(usersSort);
    assign(result->detailList, userDetails);
  }
  return result;
}

LikeInfoPtr ILikeItCacheTripodAdapter::getLikeDetail(const string& gidStr,
    int uid, int limit, bool showStrangers) {
  GlobalIdPtr gid = GlobalIdTrans::Str2GlobalId(gidStr);
  return getLikeDetail(gid, uid, limit, showStrangers);
}

LikeInfo2Ptr ILikeItCacheTripodAdapter::getLikeDetail2(const string& gidStr,
    int uid, int limit, bool showStrangers) {
  GlobalIdPtr gid = GlobalIdTrans::Str2GlobalId(gidStr);
  return getLikeDetail2(gid, uid, limit, showStrangers);
}

LikeInfoPtr ILikeItCacheTripodAdapter::getLikeDetail(const GlobalIdPtr& gid,
    int uid, int limit, bool showStrangers) {
  LikeInfoPtr result = ILikeItUtil::emptyLikeInfo(limit, showStrangers);
  try {
    result = getLikeInfo<LikeInfo, LikeInfoPtr>(gid, uid, limit, showStrangers);
  } catch (Ice::Exception& e) {
    MCE_WARN("[ILikeItCacheTripodAdapter::getLikeDetail] " << type << ":" << id 
        << " Ice::Exception : " << e.what());
  } catch (std::exception& e) {
    MCE_WARN("[ILikeItCacheTripodAdapter::getLikeDetail] " << type << ":" << id 
        << " std::excetpion : " << e.what());
  } catch (...) {
    MCE_WARN("[ILikeItCacheTripodAdapter::getLikeDetail] " << type << ":" << id
        << " exception");
  }
  return result;
}

LikeInfo2Ptr ILikeItCacheTripodAdapter::getLikeDetail2(const GlobalIdPtr& gid,
    int uid, int limit, bool showStrangers) {
  LikeInfo2Ptr result = ILikeItUtil::emptyLikeInfo2(limit, showStrangers);
  try {
    result = getLikeInfo<LikeInfo2, LikeInfo2Ptr>(gid, uid, limit, showStrangers);
  } catch (Ice::Exception& e) {
    MCE_WARN("[ILikeItCacheTripodAdapter::getLikeDetail2] " << type << ":" << id 
        << " Ice::Exception : " << e.what());
  } catch (std::exception& e) {
    MCE_WARN("[ILikeItCacheTripodAdapter::getLikeDetail2] " << type << ":" << id 
        << " std::excetpion : " << e.what());
  } catch (...) {
    MCE_WARN("[ILikeItCacheTripodAdapter::getLikeDetail2] " << type << ":" << id
        << " exception");
  }
  return result;
}

template<class Type, class TypePtr>
vector<TypePtr> ILikeItCacheTripodAdapter::getLikeInfoBatch(const GlobalIdSeq& gids,
    int uid, int limit, int detailNum) {
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
    getCountAndSort(usersSort, result->selfCount, result->friendCount, users, uid,
        friendList, result->withFriendList, result->showStrangers, result->detailLimit);

    // 加入用户详情
    if ( result->detailLimit > 0 ) {
      UserDetail2Seq userDetails = getUserDetail(usersSort);
      assign(result->detailList, userDetails);
    }
    results.push_back(result);
  }
  return results;
}


LikeInfoSeq ILikeItCacheTripodAdapter::getLikeDetailBatch(const std::vector<std::string>& gidStrs,
    int uid, int limit, int detailNum) {
  GlobalIdSeq gids;
  for ( size_t i = 0; i < gidStrs.size(); ++i ) {
    GlobalIdPtr gid = GlobalIdTrans::Str2GlobalId(gidStrs[i]);
    gids.push_back(gid);
  }
  return getLikeInfoBatch<LikeInfo, LikeInfoPtr>(gids, uid, limit, detailNum);
}

LikeInfo2Seq ILikeItCacheTripodAdapter::getLikeDetailBatch2(const std::vector<std::string>& gidStrs,
    int uid, int limit, int detailNum) {
  GlobalIdSeq gids;
  for ( size_t i = 0; i < gidStrs.size(); ++i ) {
    GlobalIdPtr gid = GlobalIdTrans::Str2GlobalId(gidStrs[i]);
    gids.push_back(gid);
  }
  return getLikeInfoBatch<LikeInfo2, LikeInfo2Ptr>(gids, uid, limit, detailNum);
}

LikeInfoSeq ILikeItCacheTripodAdapter::getLikeDetailBatchG(const std::vector<GlobalIdPtr>& gids,
    int uid, int limit, int detailNum) {
  return getLikeInfoBatch<LikeInfo, LikeInfoPtr>(gids, uid, limit, detailNum);
}

LikeInfo2Seq ILikeItCacheTripodAdapter::getLikeDetailBatchG2(const std::vector<GlobalIdPtr>& gids,
    int uid, int limit, int detailNum) {
  return getLikeInfoBatch<LikeInfo2, LikeInfo2Ptr>(gids, uid, limit, detailNum);
}
*/


LikeDataPtr ILikeItCacheTripodAdapter::getWithKey(const GlobalIdPtr& gid) {
  long key = GlobalIdTrans::GlobalId2Long(gid);
  std::string str_value;
  bool result = tripod_client_->Get(boost::lexical_cast<std::string>(key),&str_value,0);
  if (!result) {
    LikeDataPtr ptr = new LikeData;
    return ptr;
  }
  xce::tripodlike::PbLikeData data;
  std::istringstream in(str_value);
  data.ParseFromIstream(&in);
  LikeDataPtr ptr = new LikeData;
  ptr->users = vector<int>( data.userid().begin(), data.userid().end() );
  return ptr;
}


}}

