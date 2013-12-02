#include "DbHelper.h"
#include "QueryRunner.h"
#include "IceLogger.h"
using namespace xce::feed;
using namespace com::xiaonei::xce;
using namespace MyUtil;


void DbHelper::MarkFeed(int uid, int friendid, bool mark) {
	Statement sql;
	sql << "INSERT INTO " << GetTableName(uid)
			<< " (" << COL_UID << "," << COL_FRIEND << "," << COL_MARK << "," << COL_TIME << ")"
			<< " VALUES (" << uid << "," << friendid << "," << mark << ","<<" NOW() "<<")"
      << " ON DUPLICATE KEY UPDATE " << COL_MARK << " = " << mark <<","
      << COL_TIME <<" = " << "NOW()";
  
  try {
    QueryRunner(DB_INSTANCE, CDbWServer, GetTableName(uid)).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedMarkI::DbHelper:: MarkFeed   ****     sql wrong!!" << e);
  } catch (std::exception& e) {
    MCE_WARN("FeedMarkI::DbHelper:: MarkFeed   ****    std Wrong!!" << e.what());
  }
  
  MCE_INFO("FeedMarkI::DbHelper:: MarkFeed   ---->  uid friend mark:"<< uid << friendid << mark <<"Insert Success!!");
}

UserMarkStatusPtr DbHelper::GetFeedMark(int uid) {
	Statement sql;
	mysqlpp::StoreQueryResult res;

  sql << "SELECT * FROM " << GetTableName(uid) << " WHERE " << COL_UID
			<< " = " << uid;
  
  try {
    res = QueryRunner(DB_INSTANCE, CDbRServer, GetTableName(uid)).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedMarkI::DbHelper:: GetFeedMark  ****    sql wrong!!" << e);
  } catch (std::exception& e) {
    MCE_WARN("FeedMarkI::DbHelper:: GetFeedMark  ****     std Wrong!!" << e.what());
  }

	if (!res || res.num_rows() == 0) {
    MCE_INFO("FeedMarkI::DbHelper::  GetFeedMark  ---->  uid:"<< uid << " load null from DB!!");
		return NULL;
	}

  bool mark;
  int friendid;
	UserMarkStatusPtr uinfo = new UserMarkStatus(uid);
	for (size_t i = 0; i < res.num_rows(); ++i) {
		mysqlpp::Row& row = res.at(i);
    friendid = (int)row[COL_FRIEND];
		mark = (bool)row[COL_MARK];
    uinfo->Add(friendid, mark);
	}
  MCE_INFO("FeedMark::Db::GetFeedMark ----> get in DB Sucess!!" << uinfo->ToString());

  return uinfo;
}


