/******************************************************************************
Function:  Providing all types of block lists for friends recommendation
Developer: xiongjunwu
Email:     junwu.xiong@renren-inc.com
Date:      2012-03-20
*******************************************************************************/

#ifndef __RECOMMEND_FRIEND_BLOCK_H_
#define __RECOMMEND_FRIEND_BLOCK_H_

#include <iostream>
#include <sstream>
#include <string>

#include "Ice/Ice.h"

#include "Util.h"
#include "BuddyByIdReplicaAdapter.h"
#include "QueryRunner.h"

#include "socialgraph/socialgraphutil/mysqlpputil.h"

namespace xce {
namespace socialgraph {
//result handler for fetching block id from various type of data sources
class IdResultHandler : public com::xiaonei::xce::ResultHandler {
 public:
  IdResultHandler(MyUtil::IntSeq& idSeq, const std::string& fieldName);
  virtual ~IdResultHandler();
 protected:
  virtual bool handle(mysqlpp::Row& row) const;
 private:
  MyUtil::IntSeq& idSeq_;
  const std::string fieldName_;
};

inline bool IdResultHandler::handle(mysqlpp::Row& row) const {
  const int id = static_cast<int>(row[fieldName_.c_str()]);
  idSeq_.push_back(id);
  return true;
}

//class of getting all types of block lists of friends recommendation
class RecommendFriendBlock {
 public: 
  //relevant table and database definitions 

  //get block list of the host's friends alreaby were the user's friends
  static MyUtil::IntSeq getBlockFriendSeq(Ice::Int hostId);
  //get block list of those who already applied to be the host's friends but are not friends yet
  static MyUtil::IntSeq getBlockApplySeq(Ice::Int hostId);
  //get block list in the user's black list and the user may want to be friends with them 
  static MyUtil::IntSeq getBlockBlackSeq(Ice::Int hostId);
  ////get block list of those recommended to the user but were deleted 
  //static MyUtil::IntSeq getBlockDelRcdSeq(Ice::Int hostId);
  ////get all block lists of mentioned above and the user himself
  static MyUtil::IntSeq getBlockSeq(Ice::Int hostId);
  //static void getBlockSeq(Ice::Int hostId, MyUtil::IntSeq&);
};



};
};

#endif
