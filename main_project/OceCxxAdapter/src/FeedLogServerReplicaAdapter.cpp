/*************************************************
 *
 *  @Copyright    Renren Inc.
 *  @Brief        FeedLogServer系统的adaptor
 *  @Filename     FeedLogServerReplicaAdapter.cpp 
 *  @Author       rui.qiu
 *  @Email        rui.qiu@renren-inc.com
 *  @Date         2012-11-16
 *  @History
 *
 *************************************************/


#include "FeedLogServerReplicaAdapter.h"

using namespace MyUtil;
using namespace xce::feed;

FeedLogServerPrx FeedLogServerReplicaAdapter::getManager(int index) {
  return getProxy(index);
}

FeedLogServerPrx FeedLogServerReplicaAdapter::getManagerOneway(int index) {
  return getProxyOneway(index);
}

void FeedLogServerReplicaAdapter::PutLog(string log_name, string log) {
  index_ = (index_ + 1) % getCluster();
  MCE_INFO("FeedLogServerReplicaAdapter::PutLog --> log_name:" << log_name << " log:" << log);
  getManagerOneway(index_)->PutLog(log_name, log);
}


void FeedLogServerReplicaAdapter::PutLogBatch(string log_name, MyUtil::StrSeq log_seq) {
  index_ = (index_ + 1) % getCluster();
  MCE_INFO("FeedLogServerReplicaAdapter::PutLogBatch --> log_name:" << log_name << " log_seq's size:" << log_seq.size());
  getManagerOneway(index_)->PutLogBatch(log_name, log_seq);
}

void FeedLogServerReplicaAdapter::PutLogsMap(LogsMap logs_map) {
  index_ = (index_ + 1) % getCluster();
  MCE_DEBUG("FeedLogServerReplicaAdapter::PutLogsMap --> map size:" << logs_map.size());
  getManagerOneway(index_)->PutLogsMap(logs_map);
}

