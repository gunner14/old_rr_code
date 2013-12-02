/******************************************************************************
Function:  SocialGraphFoF service database interface implementation
Developer: xiongjunwu
Email:     junwu.xiong@renren-inc.com
Date:      2012-03-21
*******************************************************************************/
#ifndef __FOF_DB_UTIL_H_
#define __FOF_DB_UTIL_H_

#include <iostream>
#include <vector>
#include <boost/unordered_map.hpp>
#include <sstream>
#include <string>

#include "Util.h"
#include "base/logging.h"
#include "base/ptime.h"
#include "base/threadpool.h"
#include "base/logrotate.h"

#include "DbCxxPool/src/QueryRunner.h"
#include "UtilCxx/src/TaskManager.h"
#include "util/cpp/IntSeqSerialize.h"
#include "SocialGraphFoF.h"
#include "socialgraph/socialgraphutil/cctypedef.h"
#include "socialgraph/socialgraphutil/mysqlpputil.h"
#include "socialgraph/socialgraphutil/clogging.h"

namespace xce {
namespace socialgraph {

struct FoFRankRecord {
  int host_id;
  int position;
  int fof_id;
  float rank;
  int mutual_friend_number;
  IntVec mutual_friend_ids;
  int create_time;
};
typedef std::vector<FoFRankRecord> RecordVec;
typedef std::vector<FoFRankRecord>::const_iterator RecordVecIter;
typedef boost::unordered_map<int, RecordVec> Int2RecordVecMap;
typedef boost::unordered_map<int, RecordVec>::const_iterator Int2RecordVecMapIter;

class FoFRankRecordResultHandler : public com::xiaonei::xce::ResultHandler {
 public:
  FoFRankRecordResultHandler(RecordVec& recordVec);
  virtual ~FoFRankRecordResultHandler();
 protected:
  virtual bool handle(mysqlpp::Row& row) const;
 private:
  RecordVec& result_;
};

inline bool FoFRankRecordResultHandler::handle(mysqlpp::Row& row) const {
  //CLOG(INFO) << "FoFRankRecordResultHandler::handle"; 
  FoFRankRecord record;
  record.host_id = static_cast<int>(row["host_id"]);
  record.position = static_cast<int>(row["position"]);
  record.fof_id = static_cast<int>(row["fof_id"]);
  record.rank = static_cast<float>(row["rank"]);
  record.mutual_friend_number = static_cast<int>(row["mutual_friend_number"]);
  record.mutual_friend_ids = MyUtil::IntSeqSerialize::unserialize(row["mutual_friend_ids"]);
  record.create_time = static_cast<int>(row["UNIX_TIMESTAMP(create_time)"]);
  result_.push_back(record); 
  return true; 
}


class FoFRankDbUtil {
 public:
  static const int kEmptyId = -1;  //empty fof id denote as -1 when the total number of recommended fofs less kMaxRcd 
  static const int kMaxRecord = 100;  //max record threshold of recommend fofs stored for each user 
  static const std::string TableName;
  static const std::string DbInstanceName;

  static void setFoFRankSeq(const int host_id, const FoFRankSeq&);
  static void setFoFRankSeqs(const Int2FoFRankSeqMap&);
  static RecordVec getFoFRankSeq(const int host_id);
  static Int2RecordVecMap getFoFRankSeqs(const IntVec&);
  // virtual void AddFoFRank(Ice::Int host_id, const FoFRank&, const Ice::Current&);
  // virtual void DelFoFRank(Ice::Int host_id, const FoFRank&, const Ice::Current&);

 private:
  FoFRankDbUtil() {}
};


};
};

#endif
