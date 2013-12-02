/*
 * =====================================================================================
 *
 *       Filename:  HomeFootprintDBOperator.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月22日 19时43分59秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  xiaoguang.yang (), xiaoguang.yang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */


#include "HomeFootprintDBOperator.h"
#include <algorithm>

using namespace std;

string HomeFootprintDBOperator::getTableName(int owner, int tableId) {
  ostringstream oss;
  if (1 == tableId) {
    oss<< TABLE_HOME_FOOTPRINT_HEAD <<"_"<< owner % TABLE_HOME_FOOTPRINT_HEAD_MOD;
  } else if (2 == tableId) {
    oss<< TABLE_HOME_FOOTPRINT_BODY <<"_"<< owner % TABLE_HOME_FOOTPRINT_HEAD_MOD;
  }
  return oss.str();

}

bool equalByVisitor(const HomeStepPtr& stepPtr1, const HomeStepPtr& stepPtr2) {
  return stepPtr1->visitor == stepPtr2->visitor;
}

HomeFootprintDBOperator::HomeFootprintDBOperator() {
  ObjectCachePrx prx;
  ChannelPtr channel = 0;
  try {
    channel = new FootprintChannel;
  } catch (...) {
  }
  Ice::CommunicatorPtr ic = channel->getCommunicator();
  string objectCacheServer = "SC@HomeFootprint";
  for (int i = 0; i < CLUSTER_NUM; ++i) {
    prx = ObjectCachePrx::checkedCast(ic->stringToProxy(objectCacheServer + boost::lexical_cast<string>(i)));
    objectCachePrxs_.push_back(prx);
  }
}

ObjectCachePrx HomeFootprintDBOperator::getObjectCachePrx(int userId) {
   return objectCachePrxs_[userId % CLUSTER_NUM];
} 

void HomeFootprintDBOperator::reloadObjectCache(int category, int userId) {
  ObjectCachePrx prx = getObjectCachePrx(userId);
  prx->reloadObject(category, userId);
}

bool HomeFootprintDBOperator::cmpHomeStepVisitor(const HomeStepPtr& stepPtr1, const HomeStepPtr& stepPtr2) {
  if (stepPtr1->visitor == stepPtr2->visitor) {
    return stepPtr1->timestamp > stepPtr2->timestamp;
  }
  return stepPtr1->visitor > stepPtr2->visitor;
}

bool HomeFootprintDBOperator::cmpHomeStepTimestamp(const HomeStepPtr& stepPtr1, const HomeStepPtr& stepPtr2) {
  return stepPtr1->timestamp > stepPtr2->timestamp;
}

void HomeFootprintDBOperator::printHomeStepSeq(const vector<HomeStepPtr>& homeStepSeq) {
  for (vector<HomeStepPtr>::const_iterator it = homeStepSeq.begin(); homeStepSeq.end() != it; ++it) {
    cout << "visitor, timestamp:" << (*it)->visitor << ", " << (*it)->timestamp << endl;
  }
}

void HomeFootprintDBOperator::removeHomeStepSeq(int owner, const vector<int>& visitors) {
  vector<HomeStepPtr> queryResult = getHomeStepSeq(owner, 0, CACHE_SIZE + CACHE_BUF_SIZE);
  vector<HomeStepPtr>::iterator it = remove_if(queryResult.begin(), queryResult.end(), VisitorsToDelete(visitors));
  queryResult.resize(it - queryResult.begin());
  writeDB(owner, queryResult);
}

void HomeFootprintDBOperator::addHomeStepSeq(int owner, const vector<HomeStepPtr>& toAddHomeStepSeq) {
  vector<HomeStepPtr> allHomeStepSeq = getHomeStepSeq(owner, 0 , CACHE_BUF_SIZE + CACHE_SIZE);
  allHomeStepSeq.insert(allHomeStepSeq.end(), toAddHomeStepSeq.begin(), toAddHomeStepSeq.end());
  sort(allHomeStepSeq.begin(), allHomeStepSeq.end(), cmpHomeStepVisitor); //按visitor降序,timestamp降序，去掉visitor重复的记录,保留timestamp较大者
  vector<HomeStepPtr>::iterator it = unique(allHomeStepSeq.begin(), allHomeStepSeq.end(), equalByVisitor);
  allHomeStepSeq.resize(it - allHomeStepSeq.begin());
  sort(allHomeStepSeq.begin(), allHomeStepSeq.end(), cmpHomeStepTimestamp);//第二次排序，按timestamp降序,插入DB
  writeDB(owner, allHomeStepSeq);
}

vector<HomeStepPtr> HomeFootprintDBOperator::queryDB(int owner, int tableId) {
  string table = getTableName(owner, tableId);
  Statement sql;
  sql << "SELECT * FROM " << table << " WHERE " << COLUMN_OWNER << "=" << owner;
  HomeFootprintSetPtr homeFootprintSetPtr = new HomeFootprintSet(CACHE_BUF_SIZE + CACHE_SIZE);
  try {
    QueryRunner(DESC_FOOTPRINT, CDbRServer, table).query(sql, FootprintSetHandler<HomeFootprintSetPtr>(homeFootprintSetPtr));
  } catch (mysqlpp::Exception& e ) {
    vector<HomeStepPtr> result;
    return result;
  } catch (...) {
    vector<HomeStepPtr> result;
    return result;
  }
  return homeFootprintSetPtr->getAll();
}

vector<HomeStepPtr> HomeFootprintDBOperator::getHomeStepSeq(int owner, int begin, int limit) {
  vector<HomeStepPtr> headHomeStepSeq = queryDB(owner, 1);
  vector<HomeStepPtr> bodyHomeStepSeq = queryDB(owner, 2);
  headHomeStepSeq.insert(headHomeStepSeq.end(), bodyHomeStepSeq.begin(), bodyHomeStepSeq.end());
  int end = (begin + limit < headHomeStepSeq.size() ? begin + limit : headHomeStepSeq.size());
  vector<HomeStepPtr> result(headHomeStepSeq.begin() + begin, headHomeStepSeq.begin() + end);
  return result;
}

void HomeFootprintDBOperator::writeDBWithTableId(int owner, const vector<HomeStepPtr>& homeStepSeq, int tableId) {
  string table = getTableName(owner, tableId);
  Statement sql;
  sql << "INSERT INTO " << table <<  " (owner, visitors) VALUES"
    << "(" << owner << ", "  << mysqlpp::quote << IntSeqSerialize::serialize(homeStepSeq) << ")"
    << " ON DUPLICATE KEY UPDATE " << "visitors = VALUES(visitors)";
  try {
    QueryRunner(DESC_FOOTPRINT, CDbWServer, table).execute(sql);
  } catch (mysqlpp::Exception& e ) {
  //
  } catch (...) {
  //
  }
}

void HomeFootprintDBOperator::writeDB(int owner, const vector<HomeStepPtr>& homeStepSeq) {
  if (0 == homeStepSeq.size())
    return ;
  int headEnd;
  headEnd = (CACHE_SIZE < homeStepSeq.size() ? CACHE_SIZE : homeStepSeq.size());
  vector<HomeStepPtr> headHomeStepSeq(homeStepSeq.begin(), homeStepSeq.begin() + headEnd);
  writeDBWithTableId(owner, headHomeStepSeq, 1);
  reloadObjectCache(FOOTPRINT_SET, owner);
  sleep(2);
  vector<HomeStepPtr> bodyHomeStepSeq;
  if (CACHE_SIZE < homeStepSeq.size()) {
    int bodyEnd = (homeStepSeq.size() < CACHE_SIZE + CACHE_BUF_SIZE ? homeStepSeq.size() : CACHE_SIZE + CACHE_BUF_SIZE);
    bodyHomeStepSeq.assign(homeStepSeq.begin() + CACHE_SIZE, homeStepSeq.begin() + bodyEnd);
   } else {
    bodyHomeStepSeq.resize(0);
  }
  writeDBWithTableId(owner, bodyHomeStepSeq, 2);
  sleep(2);
  reloadObjectCache(FOOTPRINT_BUF_SET, owner);
}



