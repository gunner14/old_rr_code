/*************************************************************************
    > File Name: main.cc
    > Author: zunwen.you
    > Mail: zunwen.you@renren-inc.com 
    > Created Time: 2012年09月25日 星期二 11时24分08秒
 ************************************************************************/

#include <iostream>
#include <boost/lexical_cast.hpp>
#include "../share/ARCacheUtil.h"
#include "../share/AssociationRuleUtil.h"
#include "item.pb.h"     
#include "Singleton.h"
#include "tripod2/cc/core/tripod_client.h"
#include "OceCxxAdapter/src/SocialGraphCommonTripodAdapter.h"
#include "OceCxxAdapter/src/SocialGraphRcdTripodAdapter.h"
#include "OceCxxAdapter/src/AssociationRuleAdapter.h"
#include "socialgraph/socialgraphutil/mc_adapter.h"
#include <sstream>
#include "../share/ARCreator.h"
#include "DbCxxPool/src/ConnectionPoolManager.h"
#include "UtilCxx/src/Date.h"

typedef vector<char> BinaryData;
using namespace std;
using namespace xce::socialgraph;
using namespace MyUtil;
  using namespace xce::serverstate;
  using namespace com::xiaonei::xce;

int main(int argc, char* argv[]) {


  ConnectionPoolManager::instance().initialize();
	TaskManager::instance().config(TASK_LEVEL_GETRULE, ThreadPoolConfig(0, 10));
	TaskManager::instance().config(TASK_LEVEL_PARSERULE, ThreadPoolConfig(0, 10));

  int userId = boost::lexical_cast<int>(argv[1]); 
  int applyId = boost::lexical_cast<int>(argv[2]); 
  RuleSeq testRuleSeq;
  ARCacheUtil::getRule(230465612, testRuleSeq);
  friendDataSeq adapterResult2 = AssociationRuleAdapter::instance().getRecommendByAssociationRule(applyId, userId, 30);
  IntSeq blockList = AssociationRuleUtil::getBlockSeq(userId);
  friendDataSeq fofResult3 = AssociationRuleUtil::getFoFRecommend(userId, 10);


	struct timeval tvStart, tvEnd;
	double linStart, linEnd;
	gettimeofday( &tvStart, NULL );
	linStart = (double)tvStart.tv_sec*1000000 + (double)tvStart.tv_usec;
  
  friendDataSeq adapterResult = AssociationRuleAdapter::instance().getRecommendByAssociationRule(userId, applyId, 30);
  //int testApplys[10] = {223431437, 71367404, 230408220, 122900446, 110718571, 1556200763, 200484432, 231915228, 44674, 248528808 };
  //BOOST_FOREACH(int testApply, testApplys) {
  //  adapterResult = AssociationRuleAdapter::instance().getRecommendByAssociationRule(userId, testApply, 30);
  //}
  cout << " adapter rcd size :" << adapterResult.size() << endl;
  BOOST_FOREACH(FriendData rcd, adapterResult) {
    cout <<"[userID: " << rcd.userID << " weight:" << rcd.weight << "]  " << endl;
  }
  /* get time */
	gettimeofday( &tvEnd, NULL );
	linEnd = (double)tvEnd.tv_sec*1000000 + (double)tvEnd.tv_usec;
  cout << "get adapter rule time(ms):"<< (linEnd - linStart) / (11 * 1000) << endl;
  linStart = linEnd;
  /* get time */

  RcdInfoSeq adapterResultRcd = AssociationRuleAdapter::instance().getRcdByAssociationRule(userId, applyId, 30);
  cout << " Rcd adapter rcd size :" << adapterResultRcd.size() << endl;
  BOOST_FOREACH(RcdInfo rcd, adapterResultRcd) {
    cout <<"[userID: " << rcd.userID << " weight:" << rcd.weight  << " type: " << rcd.type << "]  " << endl;
  }
  /* get time */
	gettimeofday( &tvEnd, NULL );
	linEnd = (double)tvEnd.tv_sec*1000000 + (double)tvEnd.tv_usec;
  cout << "get Rcd adapter rule time(ms):"<< (linEnd - linStart) / (11 * 1000) << endl;
  linStart = linEnd;
  /* get time */
  
  friendDataSeq result;
  result = ARCreator::instance().create(userId, applyId, 30); 
  cout << " rcd size :" << result.size() << endl;
  BOOST_FOREACH(FriendData rcd, result) {
    cout <<"[userID: " << rcd.userID << " weight:" << rcd.weight << "]  " << endl;
  }

  /* get time */
	gettimeofday( &tvEnd, NULL );
	linEnd = (double)tvEnd.tv_sec*1000000 + (double)tvEnd.tv_usec;
  cout << "create time(ms):"<< (linEnd - linStart) / (11 * 1000) << endl;
  linStart = linEnd;
  /* get time */

  //-----------------------------------------
  RcdInfoSeq resultRcd = ARCreator::instance().createRcd(userId, applyId, 30); 
  cout << " rcd size :" << resultRcd.size() << endl;
  BOOST_FOREACH(RcdInfo rcd, resultRcd) {
    cout <<"[userID: " << rcd.userID << " weight:" << rcd.weight << " type: " << rcd.type << "]  " << endl;
  }

  /* get time */
	gettimeofday( &tvEnd, NULL );
	linEnd = (double)tvEnd.tv_sec*1000000 + (double)tvEnd.tv_usec;
  cout << "create rcd time(ms):"<< (linEnd - linStart) / (11 * 1000) << endl;
  linStart = linEnd;
  /* get time */
/*
  map<int, Rule> indexRuleMap;
  MyUtil::IntSeq idxList;
  for(int i = 0; i != 10; i++) {
    idxList.push_back(i);
  }

  ARCacheUtil::multiGetRule(idxList, indexRuleMap);
  for(map<int, Rule>::iterator it = indexRuleMap.begin(); it != indexRuleMap.end(); it++) {
    cout << it->first << "\t";
    Rule rule = it->second;
    cout << rule.index << " " << rule.confidence << " " << rule.recommendId;
    BOOST_FOREACH(int i, rule.aprioriIds) {
      cout << " " << i;
    }
    cout << endl;
  }
*/
/*  
  Rule rule;
  bool succ = ARCacheUtil::getRule(2, rule);
  cout << rule.index << "\t" << rule.confidence << "\t" << rule.recommendId << endl;
  cout << "succ " << succ << endl;

  MyUtil::IntSeq idList;
  idList.push_back(279365435);
  idList.push_back(339507839);
  idList.push_back(451160753);

  MyUtil::Int2IntSeqMap result;

  ARCacheUtil::multiGetIndexList(idList, result);
  
  cout << "idlist size: " << idList.size() << endl;
  cout << "result size: " << result.size() << endl;
  for(MyUtil::Int2IntSeqMap::iterator it = result.begin(); it != result.end(); it++) {
    cout << it->first << "\t";
    for(MyUtil::IntSeq::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++) {
      cout << *it2 << " ";
    }
    cout << endl;
  }
*/
/*
  for(int i = 1; i != 10; i++) {
    string key = boost::lexical_cast<string>(i) + "_AR";
    Rule rule;
    bool succ = ARCacheUtil::getRule(key, &rule);
    if(!succ) {
      cout << "fail" << endl;
    }
    cout << rule.index << " " << rule.recommendId << " " << rule.confidence << "\t" ;
    BOOST_FOREACH(int id, rule.aprioriIds) {
      cout << "rule "<< id << " ";
    }
    cout << endl;
  }*/
}

