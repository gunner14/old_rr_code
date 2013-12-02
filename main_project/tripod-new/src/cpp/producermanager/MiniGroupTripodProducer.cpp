/*
 * =====================================================================================
 *
 *       Filename:  MiniGroupTripodProducer.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月10日 18时50分39秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  xiaoguang.yang (), xiaoguang.yang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */

#include "MiniGroupTripodProducer.h"
#include "app/MiniGroup/MiniGroupCommon/src/MiniGroupCommon.h"
#include "app/Utils/src/DBUtils.h"

using namespace com::renren::tripod;
using namespace com::xiaonei::xce;
using namespace MyUtil;
using namespace xce::minigrouptripod;
using namespace xce::mngp;

DataMap MiniGroupTripodProducer::create(const KeySeq& keys, bool isMaster){
  MCE_INFO("[MiniGroupTripodProducer::create] keys.size = " << keys.size());
  DataMap resultMap;
  vector<long> longKeys = parseFrom(keys);
  map<long, vector<long> > seperatedKeys = xce::utils::DBUtils::seperateIdsByMod<long>(longKeys, TABLE_MOD);
  for (map<long, vector<long> >::const_iterator it = seperatedKeys.begin(); it != seperatedKeys.end(); ++it){
    Statement querySql;
    querySql << "select " << TABLE_MINIGROUP_ID << ", " << TABLE_MINIGROUP_NAME << ", " << TABLE_MINIGROUP_ICON << " from " 
      << TABLE_MINIGROUP << "_" << boost::lexical_cast<string>(it->first) << " where " << TABLE_MINIGROUP_ID << " in " 
      << xce::utils::DBUtils::getIdsStr<long>(it->second) << " order by " << TABLE_MINIGROUP_ID;
      vector<MiniGroupInfoPtr> minigroups;
      MiniGroupInfoResultHandlerI handler(minigroups);
      QueryRunner(DB_SOURCE, CDbRServer).query(querySql, handler);
      for (vector<MiniGroupInfoPtr>::const_iterator it2 = minigroups.begin(); it2 != minigroups.end(); ++it2) {
        string byteArray = (*it2)->serialize();
        resultMap.insert(make_pair<string, Data>(boost::lexical_cast<string>((*it2)->getId()), Data(byteArray.begin(), byteArray.end())));
    }
  }
  return resultMap;
}

KeySeq MiniGroupTripodProducer::createHotKeys(const string& beginKey, int limit){
  KeySeq resultSeq;
  return resultSeq;
}

KeySeq MiniGroupTripodProducer::createKeys(const string& beginKey, int limit){
  MCE_INFO("[MiniGroupTripodProducer::createKeys], beginKey = " << beginKey << ", limit = " << limit );
  KeySeq resultSeq;
  for (size_t i = 0; i < TABLE_MOD; ++i){
    Statement querySql;
    querySql << "select distinct(" << TABLE_MINIGROUP_ID << ") from " << TABLE_MINIGROUP << "_" << boost::lexical_cast<string>(i)
      << " where " << TABLE_MINIGROUP_ID << " >= " << beginKey << " limit " << limit;
   vector<long> miniGroupIds;
   xce::utils::BatchIdResultHandler<long> handler(TABLE_MINIGROUP_ID, miniGroupIds);
   QueryRunner(DB_SOURCE, CDbRServer).query(querySql, handler);
   for (vector<long>::const_iterator it = miniGroupIds.begin(); it != miniGroupIds.end(); ++it){
     resultSeq.push_back(boost::lexical_cast<string>(*it));
    }
  }
  return resultSeq;
}

vector<long> MiniGroupTripodProducer::parseFrom(const KeySeq& keys){
  vector<long> result;
  for (vector<string>::const_iterator it = keys.begin(); it != keys.end(); it++){
    result.push_back(boost::lexical_cast<long>(*it));
  }
  return result;
}
