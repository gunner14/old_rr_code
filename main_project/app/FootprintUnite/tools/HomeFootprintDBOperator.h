/*
 * =====================================================================================
 *
 *       Filename:  HomeFootprintDBOperator.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月29日 15时48分11秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  xiaoguang.yang (), xiaoguang.yang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#ifndef __HOMEFOOTPRINT_DB_OPERATOR_H__
#define __HOMEFOOTPRINT_DB_OPERATOR_H__

#include "app/FootprintUnite/share/FootprintShare.h"
#include "QueryRunner.h"
#include <FootprintUnite.h>
#include <TaskManager.h>
#include "sstream"
#include "ActiveTrackAdapter.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "BatchUpdaterAdapter.h"
#include "FootprintViewAdapter.h"
#include "HomeFootprintReplicaAdapter.h"
#include "util/cpp/InvokeMonitor.h"
#include "FootprintUnite.h"

using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace xce::footprint;
using namespace xce::log;
using namespace xce::serverstate;
const int CLUSTER_NUM = 10;

bool equalByVisitor(const HomeStepPtr& stepPtr1, const HomeStepPtr& stepPtr2);

class VisitorsToDelete {
public:
  VisitorsToDelete(const vector<int>& visitorsToDelete) {
    visitorsToDelete_ = visitorsToDelete;
  }
  bool operator() (const HomeStepPtr& stepPtr) {
    return find(visitorsToDelete_.begin(), visitorsToDelete_.end(), stepPtr->visitor) != visitorsToDelete_.end(); 
  }
private:
  vector<int> visitorsToDelete_;
};

class HomeFootprintDBOperator : virtual public ::Ice::Object {
public:
  HomeFootprintDBOperator();
  vector<HomeStepPtr> getHomeStepSeq(int owner, int begin, int limit);
  void printHomeStepSeq(const vector<HomeStepPtr>& homeStepSeq);
  void addHomeStepSeq(int owner, const vector<HomeStepPtr>& homeStepSeq);
  void removeHomeStepSeq(int owner, const vector<int>& visitors);
private:
  void writeDB(int owner, const vector<HomeStepPtr>& homeStepSeq);
  void writeDBWithTableId(int owner, const vector<HomeStepPtr>& homeStepSeq, int tableId);
  vector<HomeStepPtr> queryDB(int owner, int tableId);
  string getTableName(int owner, int tableId);
  bool static cmpHomeStepVisitor(const HomeStepPtr&, const HomeStepPtr&);
  bool static cmpHomeStepTimestamp(const HomeStepPtr&, const HomeStepPtr&);
  void reloadObjectCache(int category, int userId);
  vector<ObjectCachePrx> objectCachePrxs_;
  ObjectCachePrx getObjectCachePrx(int userId);
};

typedef IceUtil::Handle<HomeFootprintDBOperator> HomeFootprintDBOperatorPtr;
 
 
#endif





