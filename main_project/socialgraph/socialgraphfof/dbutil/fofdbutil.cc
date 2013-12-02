#include "socialgraph/socialgraphfof/dbutil/fofdbutil.h"

namespace xce {
namespace socialgraph {
/******************************************************************************/  
FoFRankRecordResultHandler::FoFRankRecordResultHandler(RecordVec& recordVec) : result_(recordVec) {
} 

FoFRankRecordResultHandler::~FoFRankRecordResultHandler() {
}

/******************************************************************************/
/*
Ice slice FoFRank structure definition:
struct FoFRank {
  int fofId;
  float rank; //friends recommendation weight for each user
  int mutualFriendNumber;
  MyUtil::IntSeq mutualFriendIdSeq;
};
sequence<FoFRank> FoFRankSeq;
dictionary<int, FoFRankSeq> Int2FoFRankSeqMap;
Database record structure:
struct FoFRankRecord {
  int host_id;
  int position;
  int fof_id;
  float rank;
  int mutual_friend_number;
  IntVec mutual_friend_ids; 
  int create_time;
};
*/
typedef std::vector<FoFRank>::const_iterator FoFRankSeqIter;
typedef Int2FoFRankSeqMap::const_iterator Int2FoFRankSeqMapIter;
const int FoFRankDbUtil::kEmptyId;
const int FoFRankDbUtil::kMaxRecord;
const std::string FoFRankDbUtil::kDbInstanceName = "socialgraph_recommend_fof";
const std::string FoFRankDbUtil::kTableName = "recommend_fof";
void FoFRankDbUtil::setFoFRankSeq(const int host_id, const FoFRankSeq& fofRankSeq) {
  CLOG(INFO) << "[FOFRankDbUtil::setFoFRankSeq] host_id: " << host_id << " size: " << fofRankSeq.size(); 
  using namespace com::xiaonei::xce;
  const std::string kSubTableName = MysqlppUtil::SubTableName(kTableName,host_id);
  Statement sql_insert;
  sql_insert << "INSERT INTO " << kSubTableName 
             << " (host_id, position, fof_id, rank, mutual_friend_number, mutual_friend_ids, create_time) VALUES ";
  mysqlpp::DateTime create_time(time(NULL));
  FoFRankSeqIter it = fofRankSeq.begin();
  for (int pos = 1; pos <= kMaxRecord; ++pos) {
     sql_insert << "(" << host_id << ", " << pos << ", ";
    if (it != fofRankSeq.end()) { 
      sql_insert << it->fofId << ", "
                 << it->rank << ", "
          	 << it->mutualFriendNumber << ", " << mysqlpp::quote
          	 << MyUtil::IntSeqSerialize::serialize(it->mutualFriendIdSeq);
      ++it;
    } else {
      sql_insert << kEmptyId << ", " 
          	 << 0.0 << ", "
          	 << 0 << ", " << mysqlpp::quote
     	 	 << 0;            
    }
    sql_insert << ", " << mysqlpp::quote << create_time << ")";
    if (pos != kMaxRecord) {
      sql_insert << ", ";
    }
  }
  sql_insert << " ON DUPLICATE KEY UPDATE "
      << "host_id = VALUES(host_id), "
      << "position = VALUES(position), "
      << "fof_id = VALUES(fof_id), "
      << "rank = VALUES(rank), "
      << "mutual_friend_number = VALUES(mutual_friend_number), "
      << "mutual_friend_ids = VALUES(mutual_friend_ids), "
      << "create_time = VALUES(create_time)";
  try {
    ostringstream sql_log;
    sql_log << "Insert for host_id: " << host_id;
    QueryRunner(kDbInstanceName, CDbWServer, kSubTableName).execute(sql_insert, sql_log.str());   
  } catch (...) {
    CLOG(WARN) << "[FOFRankDbUtil::setFoFRankSeq] host_id: " << host_id << " insert exception"; 
  }
}

void FoFRankDbUtil::setFoFRankSeqs(const Int2FoFRankSeqMap& idSeqMap) {
  CLOG(INFO) << "[FOFRankDbUtil::setFoFRankSeqs] " << " size: " << idSeqMap.size(); 
  for (Int2FoFRankSeqMapIter it = idSeqMap.begin(); it != idSeqMap.end(); ++it) {
    setFoFRankSeq(it->first, it->second); 
  }
}

RecordVec FoFRankDbUtil::getFoFRankSeq(const int host_id) {
  CLOG(INFO) << "[FOFRankDbUtil::getFoFRankSeq] " << " host_id: " << host_id; 
  using namespace com::xiaonei::xce;
  const std::string kSubTableName = MysqlppUtil::SubTableName(kTableName,host_id);
  Statement sql_query;;
  sql_query << "SELECT host_id, position, fof_id, rank, mutual_friend_number, mutual_friend_ids, UNIX_TIMESTAMP(create_time)"
            << " FROM " << kSubTableName << " WHERE host_id = " << host_id;
  RecordVec recordVec;
  FoFRankRecordResultHandler recordHandler(recordVec); 
  QueryRunner(kDbInstanceName, CDbRServer, kSubTableName).query(sql_query, recordHandler);
  return recordVec;
}

Int2RecordVecMap FoFRankDbUtil::getFoFRankSeqs(const IntVec& host_id_vec) {
  CLOG(INFO) << "[FOFRankDbUtil::getFoFRankSeqs] " << " host_id_vec size: " << host_id_vec.size(); 
  Int2RecordVecMap idVecMap;
  for (IntVecIter it = host_id_vec.begin(); it != host_id_vec.end(); ++it ) {
    idVecMap.insert(make_pair(*it, getFoFRankSeq(*it)));  
  }
  return idVecMap;
}


};
};
