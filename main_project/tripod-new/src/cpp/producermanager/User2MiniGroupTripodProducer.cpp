/*
 * =====================================================================================
 *
 *       Filename:  User2MiniGroupTripodProducer.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月13日 18时20分23秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  xiaoguang.yang (), xiaoguang.yang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#include "User2MiniGroupTripodProducer.h"
#include "app/MiniGroup/MiniGroupCommon/src/MiniGroupCommon.h"
#include "app/Utils/src/DBUtils.h"
#include "UserIdsProducer.h"

using namespace com::renren::tripod;
using namespace com::xiaonei::xce;
using namespace MyUtil;
using namespace xce::minigrouptripod;
using namespace xce::mngp;

DataMap User2MiniGroupTripodProducer::create(const KeySeq& keys, bool isMaster){
  MCE_INFO("[User2MiniGroupTripodProducer::create] keys.size = " << keys.size());
  DataMap resultMap;
  vector<int> intKeys = parseFrom(keys);
  map<int, vector<int> > seperatedKeys = xce::utils::DBUtils::seperateIdsByMod<int>(intKeys, TABLE_MOD);
  for (map<int, vector<int> >::const_iterator it = seperatedKeys.begin(); it != seperatedKeys.end(); ++it){
    Statement querySql;
    querySql << "select " << TABLE_MEMBER_MINIGROUP_MEMBER_ID << ", " << TABLE_MEMBER_MINIGROUP_MNGP_ID << " from "
      << TABLE_MEMBER_MINIGROUP << "_" << boost::lexical_cast<string>(it->first) << " where " << TABLE_MEMBER_MINIGROUP_MEMBER_ID
      << " in " << xce::utils::DBUtils::getIdsStr<int>(it->second) << " and " << TABLE_MEMBER_MINIGROUP_RESTRICT
      << TABLE_MEMBER_MINIGROUP_SORT;
    vector<User2MiniGroupInfoPtr> user2MiniGroupInfoPtrs;
    User2MiniGroupInfoResultHandlerI handler(user2MiniGroupInfoPtrs);
    QueryRunner(DB_SOURCE, CDbRServer).query(querySql, handler);
    for (vector<User2MiniGroupInfoPtr>::const_iterator it2 = user2MiniGroupInfoPtrs.begin(); it2 != user2MiniGroupInfoPtrs.end(); ++it2){
      string byteArray = (*it2)->serialize();
      resultMap.insert(make_pair<string, Data>(boost::lexical_cast<string>((*it2)->getUserId()), Data(byteArray.begin(), byteArray.end())));
    }
  }
  return resultMap;
}

KeySeq User2MiniGroupTripodProducer::createHotKeys(const string& beginKey, int limit){
  KeySeq resultSeq;
  return resultSeq;
}

KeySeq User2MiniGroupTripodProducer::createKeys(const string& beginKey, int limit){
  return UserIdsProducer::createKeys("[User2MiniGroupTripodProducer::createKeys]", beginKey, limit);
}

vector<int> User2MiniGroupTripodProducer::parseFrom(const KeySeq& keys){
  vector<int> result;
  for (vector<string>::const_iterator it = keys.begin(); it != keys.end(); ++it){
    result.push_back(boost::lexical_cast<int>(*it));
  }
  return result;
}
