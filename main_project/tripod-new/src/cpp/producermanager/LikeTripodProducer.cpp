/*
 * =====================================================================================
 *
 *       Filename:  LikeTripodProducer.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年01月10日 10时14分05秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#include "LikeTripodProducer.h"
#include "app/Utils/src/DBUtils.h"
#include "util/cpp/IntegerSeqSerialize.h"

using namespace xce::liketripod;
using namespace com::renren::tripod;
using namespace com::xiaonei::xce;
using namespace xce::ilikeit;
using namespace MyUtil;

vector<long> LikeTripodProducer::parseFrom(const KeySeq& keys) {
  vector<long> result;
  for (vector<string>::const_iterator it = keys.begin(); it != keys.end(); ++it) {
    result.push_back(boost::lexical_cast<long>(*it));
  }
  return result;
}

DataMap LikeTripodProducer::create(const KeySeq& keys, bool isMaster) {
  MCE_INFO("[LikeTripodProducer::create] keys.size = " << keys.size());
  DataMap resultMap;
  vector<long> longKeys = parseFrom(keys);
  map<long, LikeDataIPtr> likeDataMap;
  map<long, vector<long> > seperatedLongKeys = xce::utils::DBUtils::seperateIdsByMod<long>(longKeys, TABLE_NUM);
  for (map<long, vector<long> >::const_iterator it = seperatedLongKeys.begin(); it != seperatedLongKeys.end(); ++it) {
    Statement querySql;
    querySql << "select " << FIELD_GlobalId << ", " << FIELD_UID << " from " << TABLE_LIKESHIP << boost::lexical_cast<string>(it->first)
        << " where " << FIELD_GlobalId << " in " << xce::utils::DBUtils::getIdsStr<long>(it->second) << " order by " << FIELD_GlobalId
        << ", " << FIELD_UID;
    BatchLikeshipHandler handler(likeDataMap);
    QueryRunner(DB_DESC_ILIKEIT, CDbRServer).query(querySql, handler);
  }
  for (map<long, LikeDataIPtr>::const_iterator it = likeDataMap.begin(); it != likeDataMap.end(); ++it) {
    string byteString = IntegerSeqSerialize<int>::serialize(it->second->getUsers());
    resultMap[boost::lexical_cast<string>(it->first)] = Data(byteString.begin(), byteString.end());
  }
  return resultMap;
}

KeySeq LikeTripodProducer::createHotKeys(const string& beginKey, int limit) {
  KeySeq resultSeq;
  return resultSeq;
}

KeySeq LikeTripodProducer::createKeys(const string& beginKey, int limit) {
  MCE_INFO("[LikeTripodProducer::createKeys] beginKey = " << beginKey << ", limit = " << limit);
  KeySeq resultSeq;
  for (size_t i = 0; i < TABLE_NUM; ++i) {
    Statement querySql;
    querySql << "select distinct(" << FIELD_GlobalId << ") from " << TABLE_LIKESHIP << boost::lexical_cast<string>(i)
        << " where " << FIELD_GlobalId << " >= " << beginKey << " limit " << limit;
    vector<long> globalIds;
    xce::utils::BatchIdResultHandler<long> handler(FIELD_GlobalId, globalIds);
    QueryRunner(DB_DESC_ILIKEIT, CDbRServer).query(querySql, handler);
    for (vector<long>::const_iterator it = globalIds.begin(); it != globalIds.end(); ++it) {
      resultSeq.push_back(boost::lexical_cast<string>(*it));
    }
  }
  return resultSeq;
}
