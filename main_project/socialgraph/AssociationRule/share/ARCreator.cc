/*************************************************************************
    > File Name: CacheManager.cc
    > Author: zunwen.you
    > Mail: zunwen.you@renren-inc.com 
    > Created Time: 2012年09月25日 星期二 12时55分40秒
 ************************************************************************/
#include "ARCreator.h"

#include <ext/hash_map>
#include <boost/lexical_cast.hpp>
#include "Singleton.h"
#include "tripod2/cc/core/tripod_client.h"
#include "OceCxxAdapter/src/SocialGraphCommonTripodAdapter.h"
#include "socialgraph/socialgraphutil/mc_adapter.h"

#include "AssociationRuleUtil.h"
#include "ARCacheUtil.h"

using namespace std;
using namespace MyUtil;
using namespace xce::socialgraph;


bool rulePriority(const Rule* r1, const Rule* r2) {
  if (r1->hit > r2->hit) {
    return true;
  } else if (r1->ratio < r2->ratio) {
    return false;
  }
  if (r1->ratio > r2->ratio) {
    return true;
  } else if (r1->ratio < r2->ratio) {
    return false;
  }
  if (r1->confidence > r2->confidence) {
    return true;
  } else {
    return false;
  }
}

bool rcdPriority(const FriendData& r1, const FriendData& r2) {
  if (r1.weight >= r2.weight) {
    return true;
  } else {
    return false;
  }
}

bool rcdInfoPriority(const RcdInfo& r1, const RcdInfo& r2) {
  if (r1.weight >= r2.weight) {
    return true;
  } else {
    return false;
  }
}

const string ARCreator::_ARtype = "AR";

friendDataSeq ARCreator::create(int userId, int applyId, int limit) {

  // 获取新二度好友数据
  friendDataSeq fofRcd = AssociationRuleUtil::getFoFRecommend(userId, limit);

  // 获取applyId的rule list
  RuleSeq applyRules;
  bool succ = ARCacheUtil::getRule(applyId, applyRules);
  if (false == succ) {
    MCE_INFO("[ARCreator::create] userID: " << userId << " cache has no rules of applyId: " << applyId); 
    return fofRcd;
  }
  MCE_INFO("[ARCreator::create] userID: " << userId << " get applyId: " << applyId << " rules size: " << applyRules.size());

  // 获取好友列表 
  _friendList = AssociationRuleUtil::getFriendSeq(userId);
  MCE_INFO("[ARCreator::create] userID: " << userId << " friend list size: " << _friendList.size());

  // 获取_applyList 
  _applyList = AssociationRuleUtil::getApplySeqByCache(userId);
  MCE_INFO("[ARCreator::create] userID: " << userId << " apply list size: " << _applyList.size());
  
  // 统计当前userId下，规则的ratio
  vector<Rule*> rulePtrSeq;
  collectRuleRatio(applyId, applyRules, rulePtrSeq);
  MCE_INFO("[ARCreator::create] userID: " << userId << " remains " << rulePtrSeq.size() << " rules, ruleLimit is set " << _ruleLimit);
  
  //排序
  friendDataSeq rcdSeq; 
  rankOnlyApplyId(rulePtrSeq, applyId, rcdSeq);
  
  
  // 过滤好友列表、申请列表、黑名单等 
  filterList(userId, applyId, rcdSeq);
  MCE_INFO("[ARCreator::create] userID: " << userId << " after fiter block list, rcdSeq size: " << rcdSeq.size());

  rcdSeq.insert(rcdSeq.end(), fofRcd.begin(), fofRcd.end());
  if (rcdSeq.size() > limit) {
    rcdSeq.resize(limit);
  }
  return rcdSeq;
}

RcdInfoSeq ARCreator::createRcd(int userId, int applyId, int limit) {

  // 获取applyId的rule list
  RuleSeq applyRules;
  bool succ = ARCacheUtil::getRule(applyId, applyRules);
  MCE_INFO("[ARCreator::createRcd] userID: " << userId << " get applyId: " << applyId << " rules size: " << applyRules.size());

  // 获取好友列表 
  _friendList = AssociationRuleUtil::getFriendSeq(userId);
  MCE_INFO("[ARCreator::createRcd] userID: " << userId << " friend list size: " << _friendList.size());

  // 获取_applyList 
  _applyList = AssociationRuleUtil::getApplySeqByCache(userId);
  MCE_INFO("[ARCreator::createRcd] userID: " << userId << " apply list size: " << _applyList.size());
  
  // 统计当前userId下，规则的ratio
  vector<Rule*> rulePtrSeq;
  collectRuleRatio(applyId, applyRules, rulePtrSeq);
  MCE_INFO("[ARCreator::createRcd] userID: " << userId << " remains " << rulePtrSeq.size() << " rules, ruleLimit is set " << _ruleLimit);
  
  //排序
  RcdInfoSeq rcdSeq; 
  rankOnlyApplyId(rulePtrSeq, applyId, rcdSeq);
  

  // 获取新二度好友数据
  RcdInfoSeq fofRcd = AssociationRuleUtil::getFoFRcd(userId, limit);
  addOtherRcd(rcdSeq, fofRcd);  

  // 过滤好友列表、申请列表、黑名单等 
  filterList(userId, applyId, rcdSeq);
  MCE_INFO("[ARCreator::createRcd] userID: " << userId << " after fiter block list, rcdSeq size: " << rcdSeq.size());

  if (rcdSeq.size() > limit) {
    rcdSeq.resize(limit);
  }
  return rcdSeq;
}

IntSeq ARCreator::getSelectedFriends(int userId, int applyId) {
  IntSeq selectFriends;
  if (_friendList.size() < _friendLimit) {
    selectFriends = _friendList;
    selectFriends.insert(selectFriends.end(), _applyList.begin(), _applyList.end());
    if (selectFriends.size() > _friendLimit) {
      selectFriends.resize(_friendLimit);
    }
  } else {
    selectFriends = AssociationRuleUtil::getFriendRank(userId);
    MCE_INFO("[ARCreator::create] userID: " << userId << " FriendRank size: " << selectFriends.size());
  }
  selectFriends.push_back(applyId);
  return selectFriends;
}

void ARCreator::collectRuleRatio(int applyId, RuleSeq& applyRules, vector<Rule*>& rulePtrSeq) {
  set<int> friendApplySet(_friendList.begin(), _friendList.end());
  friendApplySet.insert(_applyList.begin(), _applyList.end());
  friendApplySet.insert(applyId);
  BOOST_FOREACH(Rule& rule, applyRules) {
    BOOST_FOREACH(int uId, rule.aprioriIds) {
      if (friendApplySet.count(uId) > 0) {
        rule.hit += 1;
      }
    }
    rule.ratio = ((float)rule.hit) / rule.aprioriIds.size();
    rulePtrSeq.push_back(&rule);
  }
  if (rulePtrSeq.size() > _ruleLimit) {
    sort(rulePtrSeq.begin(), rulePtrSeq.end(), rulePriority);
    rulePtrSeq.resize(_ruleLimit);
  }
}

void ARCreator::collectRuleRatio(int userId, vector<Rule*>& rulePtrSeq) {
  IndexCountMap indexCountMap;  
  IndexCountMap::iterator itrI2C;
  UserIndexSeqMap::iterator itrU2I = _userIndexSeqMap.begin(); 
  for ( ; itrU2I != _userIndexSeqMap.end(); itrU2I++) {
    BOOST_FOREACH(long& index, itrU2I->second) {
      itrI2C = indexCountMap.find(index);
      if (itrI2C == indexCountMap.end()) {
        indexCountMap.insert(pair<long, int>(index, 1));
      } else {
        itrI2C->second += 1;
      }
    }
  }

  MCE_INFO("[ARCreator::collectRuleRatio] userID: " << userId << " covers " << indexCountMap.size() << " rules");
  
  IndexRuleMap::iterator itrI2R = _indexRuleMap.begin();
  for ( ; itrI2R != _indexRuleMap.end(); itrI2R++) {
    itrI2C = indexCountMap.find(itrI2R->first);
    if (itrI2C == indexCountMap.end()) {
      continue;
    }
    float ratio = ((float) itrI2C->second) / itrI2R->second.aprioriIds.size();
    itrI2R->second.hit = itrI2C->second;
    itrI2R->second.ratio = ratio;
    rulePtrSeq.push_back(&(itrI2R->second));
  }

  if (rulePtrSeq.size() > _ruleLimit) {
    sort(rulePtrSeq.begin(), rulePtrSeq.end(), rulePriority);
    rulePtrSeq.resize(_ruleLimit);
  }
}


void ARCreator::rankOnlyApplyId(const vector<Rule*>& rulePtrSeq, int applyId, friendDataSeq& rcdSeq) {
  map<int, FriendData> userRcdMap;
  BOOST_FOREACH (const Rule* rulePtr, rulePtrSeq) {
    float weight = rulePtr->ratio * rulePtr->confidence * rulePtr->support;
    BOOST_FOREACH(int rcdId, rulePtr->consequenceIds) {
      FriendData rcd;
      rcd.userID = rcdId;
      rcd.weight = weight;
      map<int, FriendData>::iterator it = userRcdMap.find(rcdId);
      if (it != userRcdMap.end()) {
        it->second.weight += weight;
      } else {
        userRcdMap.insert(pair<int, FriendData>(rcdId, rcd));
      }
    }
  }

  map<int, FriendData>::iterator itrU2Rcd = userRcdMap.begin();
  for ( ; itrU2Rcd != userRcdMap.end(); itrU2Rcd++) {
    rcdSeq.push_back(itrU2Rcd->second);
  }
  sort(rcdSeq.begin(), rcdSeq.end(), rcdPriority);
}

void ARCreator::rankOnlyApplyId(const vector<Rule*>& rulePtrSeq, int applyId, RcdInfoSeq& rcdSeq) {
  map<int, RcdInfo> userRcdMap;
  BOOST_FOREACH (const Rule* rulePtr, rulePtrSeq) {
    float weight = rulePtr->ratio * rulePtr->confidence * rulePtr->support;
    BOOST_FOREACH(int rcdId, rulePtr->consequenceIds) {
      RcdInfo rcd;
      rcd.userID = rcdId;
      rcd.weight = weight;
      rcd.type = _ARtype;
      map<int, RcdInfo>::iterator it = userRcdMap.find(rcdId);
      if (it != userRcdMap.end()) {
        it->second.weight += weight;
      } else {
        userRcdMap.insert(pair<int, RcdInfo>(rcdId, rcd));
      }
    }
  }

  map<int, RcdInfo>::iterator itrU2Rcd = userRcdMap.begin();
  for ( ; itrU2Rcd != userRcdMap.end(); itrU2Rcd++) {
    rcdSeq.push_back(itrU2Rcd->second);
  }
  sort(rcdSeq.begin(), rcdSeq.end(), rcdInfoPriority);
}

void ARCreator::addOtherRcd(RcdInfoSeq& oriRcdSeq, const RcdInfoSeq& newRcdSeq) {
  map<int, RcdInfo> userRcdMap;
  BOOST_FOREACH (const RcdInfo& rcd, oriRcdSeq) {
    userRcdMap.insert(pair<int, RcdInfo>(rcd.userID, rcd));
  }
  BOOST_FOREACH (const RcdInfo& rcd, newRcdSeq) {
    map<int, RcdInfo>::iterator it = userRcdMap.find(rcd.userID);
    if (it == userRcdMap.end()) {
      oriRcdSeq.push_back(rcd);
    }
  }
}


void ARCreator::rankUsers(const vector<Rule*>& rulePtrSeq, int applyId, friendDataSeq& rcdSeq) {
  BOOST_FOREACH (const Rule* rulePtr, rulePtrSeq) {
    set<int> aprioriSet(rulePtr->aprioriIds.begin(), rulePtr->aprioriIds.end());
    float applyFactor = 1.0;
    if (aprioriSet.find(applyId) != aprioriSet.end()) {
      applyFactor += 2 / rulePtr->aprioriIds.size();
    } 
    float weight = applyFactor * rulePtr->ratio * rulePtr->confidence * rulePtr->support;
    BOOST_FOREACH(int rcdId, rulePtr->consequenceIds) {
      FriendData rcd;
      rcd.userID = rcdId;
      rcd.weight = weight;
      rcdSeq.push_back(rcd);
    }
  }
  sort(rcdSeq.begin(), rcdSeq.end(), rcdPriority);
}


void ARCreator::filterList(int userId, int applyId, friendDataSeq& rcdSeq) {

  // friend list
  set<int> filterSet(_friendList.begin(), _friendList.end());

  // apply id
  filterSet.insert(applyId);

  // apply list 
  filterSet.insert(_applyList.begin(), _applyList.end());

  // block list 
  IntSeq blockList = AssociationRuleUtil::getBlockSeqByCache(userId);
  filterSet.insert(blockList.begin(), blockList.end());

  MCE_INFO("[ARCreator::filterList] userID: " << userId << " filter size:" << filterSet.size());
  friendDataSeq::iterator it = rcdSeq.begin();
  while (it != rcdSeq.end()) {
    set<int>::iterator it2 = filterSet.find(it->userID);
    if (it2 != filterSet.end()) {
      it = rcdSeq.erase(it);
    } else {
      it++;
    }
  }
}

void ARCreator::filterList(int userId, int applyId, RcdInfoSeq& rcdSeq) {

  // friend list
  set<int> filterSet(_friendList.begin(), _friendList.end());

  // apply id
  filterSet.insert(applyId);

  // apply list 
  filterSet.insert(_applyList.begin(), _applyList.end());

  // block list 
  IntSeq blockList = AssociationRuleUtil::getBlockSeqByCache(userId);
  filterSet.insert(blockList.begin(), blockList.end());

  MCE_INFO("[ARCreator::filterList] userID: " << userId << " filter size:" << filterSet.size());
  RcdInfoSeq::iterator it = rcdSeq.begin();
  while (it != rcdSeq.end()) {
    set<int>::iterator it2 = filterSet.find(it->userID);
    if (it2 != filterSet.end()) {
      it = rcdSeq.erase(it);
    } else {
      it++;
    }
  }
}
