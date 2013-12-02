/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
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

void MyUtil::initialize() {
}

HomeStepSeq generateHomeStepSeq(int owner,const vector<int>& visitorIds,const vector<int>& timeStamps) {
  HomeStepSeq result;
  vector<int>::const_iterator it1, it2;
  for (it1 = visitorIds.begin(), it2 = timeStamps.begin(); it1 != visitorIds.end() && it2 != timeStamps.end(); ++it1, ++it2) {
    HomeStepPtr tmpHomeStepPtr = new HomeStep();
    tmpHomeStepPtr->visitor = *it1;
    tmpHomeStepPtr->timestamp = *it2;
    tmpHomeStepPtr->gid = owner;
    result.push_back(tmpHomeStepPtr);
  }
  return result;
}

void handleGet(const HomeFootprintDBOperatorPtr& dbOperatorPtr) {
  int owner, begin, limit;
  cout << "input:owner, begin, limit\n";
  cin >> owner >> begin >> limit;
  vector<HomeStepPtr> result = dbOperatorPtr->getHomeStepSeq(owner, begin, limit);
  dbOperatorPtr->printHomeStepSeq(result);
}

void handleSet(const HomeFootprintDBOperatorPtr& dbOperatorPtr) {
  int owner, visitor, timestamp;
  vector<int> visitors, timestamps;
  cout << "input owner:\n";
  cin >> owner;
  while (true) {
    cout << "input visitor(-1:end input), timestamp:\n";
    cin >> visitor;
    if (-1 == visitor) break;
    cin >> timestamp;
    visitors.push_back(visitor);
    timestamps.push_back(timestamp);
  }
  if (0 == visitors.size()) return;
  vector<HomeStepPtr> toAddHomeStepSeq = generateHomeStepSeq(owner, visitors, timestamps);
  dbOperatorPtr->addHomeStepSeq(owner, toAddHomeStepSeq);
}

void handleDelete(const HomeFootprintDBOperatorPtr& dbOperatorPtr) {
  int owner, visitor;
  vector<int> visitors;
  cout << "input owner(-1:end input):\n";
  cin >> owner;
  if (-1 == owner) {
    return;
  }
  while (true) {
    cout << "input visitors(-1:end input):\n";
    cin >> visitor;
    if (-1 == visitor) {
      break;
    }
    visitors.push_back(visitor);
  }
  dbOperatorPtr->removeHomeStepSeq(owner, visitors);
}


int main()
{
  string op;
  ConnectionPoolManager::instance().initialize();
  HomeFootprintDBOperatorPtr dbOperatorPtr = new HomeFootprintDBOperator();
  while (true) {
    cout << "input: set, get, delete, quit\n";
    cin >> op;
    if ("quit" == op) {
      break;
    } else if ("set" == op) {
      handleSet(dbOperatorPtr);
    } else if ("get" == op) {
      handleGet(dbOperatorPtr);
    } else if ("delete" == op) {
      handleDelete(dbOperatorPtr);
    }
  }
  return 0;
}
