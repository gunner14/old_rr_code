#include "socialgraph/socialgraphfof/src/recommendfriendblock.h"

namespace xce {
namespace socialgraph {
/*******************************************************************************
  implementation of IdResultHandler class 
*******************************************************************************/
IdResultHandler::IdResultHandler(MyUtil::IntSeq& idSeq, const std::string& fieldName) :
                                 idSeq_(idSeq), fieldName_(fieldName) {
}


IdResultHandler::~IdResultHandler() {
}


//bool IdResultHandler::handle(mysqlpp::Row& row) const {
//  const int id = static_cast<int>(row[fieldName_.c_str()]);
//  idSeq_.push_back(id);
//  return true;
//}

/*******************************************************************************
  implementation of RecommendFriendBlock class 
*******************************************************************************/
MyUtil::IntSeq RecommendFriendBlock::getBlockFriendSeq(Ice::Int hostId) {
   //MCE_INFO("[RecommendFriendBlock::getBlockFriendSeq] hostId: " << hostId << " from BuddyByIdCacheAdapter" );
   MyUtil::IntSeq idSeq;
   idSeq = xce::buddy::adapter::BuddyByIdCacheAdapter::instance().getFriendListAsc(hostId, -1);
   return idSeq;
}


MyUtil::IntSeq RecommendFriendBlock::getBlockApplySeq(Ice::Int hostId) {
  //MCE_INFO("[RecommendFriendBlock::getBlockApplySeq] hostId: " << hostId << " from buddyapply_friendfinder_applicant" );
  if (hostId < 0) {
    return MyUtil::IntSeq();
  }
  using namespace ::com::xiaonei::xce;
  Statement sql;
  std::string table = MysqlppUtil::SubTableName("buddyapply_applicant", hostId);
  sql << "SELECT accepter FROM " << table << " WHERE applicant = " << hostId << " LIMIT 100";
  MyUtil::IntSeq idSeq;
  IdResultHandler idHandler(idSeq, "accepter");
  QueryRunner("buddyapply_friendfinder_applicant", CDbRServer, table).query(sql, idHandler);
  return idSeq;  
}


MyUtil::IntSeq RecommendFriendBlock::getBlockBlackSeq(Ice::Int hostId) { 
  //MCE_INFO("[RecommendFriendBlock::getBlockBlackSeq] hostId: " << hostId << " from friendfinder_block" );
  if (hostId < 0) {
    return MyUtil::IntSeq();
  }
  using namespace ::com::xiaonei::xce;
  Statement sql;
  std::string table = "relation_block";
  sql << "SELECT guest FROM " << table << " WHERE host = " << hostId;
  MyUtil::IntSeq idSeq;
  IdResultHandler idHandler(idSeq, "guest");
  QueryRunner("friendfinder_block", CDbRServer).query(sql, idHandler);
  return idSeq;
}


//MyUtil::IntSeq RecommendFriendBlock::getBlockDelRcdSeq(Ice::Int hostId) {
//  if (hostId <= 0) {
//    return MyUtil::IntSeq();
//  }
//  using namespace ::com::xiaonei::xce;
//  Statement sql;
//  std::string table = MysqlppUtil::SubTableName("commmon_friends_block", hostId);
//  sql << "SELECT block_id FROM " << table << " WHERE owner_id = " << hostId;
//  MyUtil::IntSeq idSeq;
//  IdResultHandler idHandler(idSeq, "block_id");  
//  QueryRunner("friendfinder_common_friends_new", CDbRServer, table).query(sql, idHandler);
//  return idSeq; 
//}


MyUtil::IntSeq RecommendFriendBlock::getBlockSeq(Ice::Int hostId) {
  if (hostId < 0) {
    return MyUtil::IntSeq();
  }
  MyUtil::IntSeq idSeq = getBlockApplySeq(hostId);
  MyUtil::IntSeq blackSeq = getBlockBlackSeq(hostId);
  idSeq.insert(idSeq.end(), blackSeq.begin(), blackSeq.end());
  idSeq.push_back(hostId); 
  return idSeq;
}


};
};
