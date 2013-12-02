#include "commondbhelper.h"

namespace xce {
namespace socialgraph {

void CommonDbHelper::GetCommonsFromDb(int hostid, CommonFriendSeq& comms) {
	using namespace com::xiaonei::xce;

  Statement oss;
  oss << "SELECT friend_id, weight, shares FROM " << TableName::common_friends_table(hostid)
      << " WHERE owner_id=" << hostid << " ORDER BY position DESC";

  CommonFriendSeq commFriends;
  CommonFriendSetResultHandler handler(comms);
  QueryRunner("friendfinder_common_friends_new", CDbRServer,
          TableName::common_friends_pattern(hostid)).query(oss, handler);	
}

void CommonDbHelper::SetCommonsToDb(int hostid, const CommonFriendSeq& comms) {
	using namespace com::xiaonei::xce;

  mysqlpp::DateTime now(time(NULL));
  int comm_size = (int) comms.size();

  Statement sqlInsert;
  ostringstream sqllog;
  sqlInsert << "INSERT INTO " << TableName::common_friends_table(hostid)
      << " (owner_id, friend_id, create_time, weight, shares, position) VALUES ";
  sqllog << "SAVE " << hostid << " FOR";
  for (int pos = CF_SAVE_THRESHOLD_DB - 1, cs = 0; pos >= 0; --pos, ++cs) {
    sqlInsert << "(" << hostid << ", ";
    if (cs < comm_size) {
      std::vector<int> tmp_shares = comms.at(cs).shares;
      tmp_shares.insert(tmp_shares.begin(), comms.at(cs).sharesCount);     //首位存储共同好友的实际个数
      tmp_shares.resize(tmp_shares.size() > SAVE_DATA_LENGTH ? SAVE_DATA_LENGTH : tmp_shares.size()); //只存储定长字节

      sqllog << " " << comms.at(cs).userId;
      sqlInsert << comms.at(cs).userId << ", " << mysqlpp::quote << now << ", "
          << comms.at(cs).weight << ", " << mysqlpp::quote << MyUtil::IntSeqSerialize::serialize(tmp_shares);
    } else {
      sqlInsert << EMPTY_ID << ", " << mysqlpp::quote << now << ", " << 0 << ", " << 0;
    }
    sqlInsert << ", " << pos << ")";

    if (pos != 0) {
      sqlInsert << ", ";
    }
  }
  sqlInsert << " ON DUPLICATE KEY UPDATE " << "friend_id=VALUES(friend_id), create_time=VALUES(create_time), weight=VALUES(weight), shares=VALUES(shares);";
	try {
  	QueryRunner("friendfinder_common_friends_new", CDbWServer,
  	    TableName::common_friends_pattern(hostid)).execute(sqlInsert, sqllog.str());
	} catch (Ice::Exception& e) {
  	MCE_WARN("CommonDbHelper::SetCommonsToDb " << hostid << " ice exception " << e.what());
  } catch (std::exception& e) {
    MCE_WARN("CommonDbHelper::SetCommonsToDb " << hostid << " std exception " << e.what());
  } catch (...) {
    MCE_WARN("CommonDbHelper::SetCommonsToDb " << hostid << " exception");
  }
}

void CommonDbHelper::RemoveCommonsFromDb(int hostid, int friendid) {
	using namespace com::xiaonei::xce;

	try {
	  Statement sqlDelete;
    sqlDelete << "UPDATE " << TableName::common_friends_table(hostid)
        << " set friend_id=" << EMPTY_ID << " WHERE owner_id=" << hostid
        << " AND friend_id=" << friendid;
    QueryRunner("friendfinder_common_friends_new", CDbWServer,
        TableName::common_friends_pattern(hostid)).execute(sqlDelete);

    Statement sqlWithBlock;
    sqlWithBlock << "INSERT INTO " << TableName::common_friends_block_table(hostid)
        << " (owner_id, block_id) VALUES (" << hostid << ", " << friendid << ")";
    sqlWithBlock << " ON DUPLICATE KEY UPDATE " << "block_id=" << friendid << ";";
    QueryRunner("friendfinder_common_friends_new", CDbWServer,
        TableName::common_friends_block_pattern(hostid)).execute(sqlWithBlock);
	} catch (Ice::Exception& e) {
  	MCE_WARN("CommonDbHelper::RemoveCommonsFromDb " << hostid << "-" << friendid << " ice exception " << e.what());
  } catch (std::exception& e) {
    MCE_WARN("CommonDbHelper::RemoveCommonsFromDb " << hostid << "-" << friendid << " std exception " << e.what());
  } catch (...) {
    MCE_WARN("CommonDbHelper::RemoveCommonsFromDb " << hostid << "-" << friendid << " exception");
  }
}

void CommonDbHelper::GetRecentlyloginUserId(std::vector<int>& userid_list) {
	using namespace com::xiaonei::xce;

	time_t t = time(0);
	t = t - 24 * 3600;     //回退1天0点
	struct tm *Tm = localtime(&t);
	time_t ct = t - Tm->tm_hour * 3600 - Tm->tm_min * 60 - Tm->tm_sec;

	std::string tableName = "user_time";
	int interval = 10;
	int last = 0;

	for (int i = 10; i < 24 * 3600; i = i + interval) {       //以10秒钟为间隔统计昨天登录的用户
		char startTime[30];
		struct tm *AscTm_1 = localtime(&ct);
		sprintf(startTime, "%04d-%02d-%02d %02d:%02d:%02d",
				AscTm_1->tm_year+1900,
				AscTm_1->tm_mon+1,
				AscTm_1->tm_mday,
				AscTm_1->tm_hour,
				AscTm_1->tm_min,
				AscTm_1->tm_sec);

		char endTime[30];
		ct += interval;
		struct tm *AscTm_2 = localtime(&ct);
		sprintf(endTime, "%04d-%02d-%02d %02d:%02d:%02d",
				AscTm_2->tm_year+1900,
				AscTm_2->tm_mon+1,
				AscTm_2->tm_mday,
				AscTm_2->tm_hour,
				AscTm_2->tm_min,
				AscTm_2->tm_sec);

		Statement sql;       
		sql << "SELECT * FROM " << tableName << " WHERE lastlogintime BETWEEN "
				<< mysqlpp::quote << string(startTime) << " AND " << mysqlpp::quote 
				<< string(endTime);                  
		
		try {
			QueryRunner("user_time", CDbWServer).query(sql, UsertimeBatchReadHandler(last, userid_list));
		} catch (Ice::Exception& e) {
			MCE_WARN("[CommonDbHelper] GetRecentlyloginUserId Connection Exception: " << e.what() << "\n");
		} catch (std::exception& e) {
			MCE_WARN("[CommonDbHelper] GetRecentlyloginUserId std Exception: " << e.what() << "\n");
		} catch (...) {
			MCE_WARN("[CommonDbHelper] Unknow Exception");
		}
	}
}

long CommonDbHelper::GetCreateDataTime(int hostid) {
	using namespace com::xiaonei::xce;
	long time = 0;

  Statement oss;
  oss << "SELECT UNIX_TIMESTAMP(create_time) FROM " << TableName::common_friends_table(hostid)
      << " WHERE owner_id=" << hostid;

	try {
  	mysqlpp::StoreQueryResult result = QueryRunner("friendfinder_common_friends_new", CDbWServer,
  		    TableName::common_friends_pattern(hostid)).store(oss);
		for (size_t i = 0; i < result.num_rows(); ++i) {
			time = result[i][0];
			break;
		}
	} catch (Ice::Exception& e) {
		MCE_WARN("[CommonDbHelper] GetCreateDataTime Connection Exception: " << e.what() << "\n");
	} catch (std::exception& e) {
		MCE_WARN("[CommonDbHelper] GetCreateDataTime std Exception: " << e.what() << "\n");
	} catch (...) {
		MCE_WARN("[CommonDbHelper] Unknow Exception");
	}
	return time;
}

std::vector<int> CommonDbHelper::GetUserFollowSites(int hostid, int limit) {
	using namespace com::xiaonei::xce;

	Statement oss;
	oss << "SELECT site_id FROM " << TableName::minisite_follower_site_table(hostid)
			<< " WHERE follower_id = " << hostid << " LIMIT " << limit;

	std::vector<int> site_seq;
  FollowSitesReadHandler handler(hostid, site_seq);
	try {
  	QueryRunner("minisite_site_follower_a", CDbRServer).query(oss, handler);	
	} catch (Ice::Exception& e) {
		MCE_WARN("[CommonDbHelper] GetUserFollowSites Connection Exception: " << e.what() << "\n");
	} catch (std::exception& e) {
		MCE_WARN("[CommonDbHelper] GetUserFollowSites std Exception: " << e.what() << "\n");
	} catch (...) {
		MCE_WARN("[CommonDbHelper] GetUserFollowSites Unknow Exception");
	}

	return site_seq;
}

std::map<int, std::string> CommonDbHelper::GetSiteUrls(const std::vector<int>& siteids) {
	using namespace com::xiaonei::xce;

	std::map<int, std::string> result;
	if (siteids.empty()) {
		return result;
	}

	Statement oss;
	oss << "SELECT id, url FROM minisite WHERE id in (";
	for (std::vector<int>::const_iterator iter = siteids.begin();
			iter != siteids.end(); ++iter) {
		oss << *iter;
		if (iter + 1 != siteids.end()) {
			oss << ", ";
		}
	}
	oss << ")";

	FollowSitesUrlHandler handler(result);
	try {
  	QueryRunner("minisite", CDbRServer).query(oss, handler);	
	} catch (Ice::Exception& e) {
		MCE_WARN("[CommonDbHelper] GetSiteUrls Connection Exception: " << e.what() << "\n");
	} catch (std::exception& e) {
		MCE_WARN("[CommonDbHelper] GetSiteUrls std Exception: " << e.what() << "\n");
	} catch (...) {
		MCE_WARN("[CommonDbHelper] GetSiteUrls Unknow Exception");
	}

	return result;
}

std::vector<int> CommonDbHelper::GetUserFollowPages(int hostid, int limit) {
	using namespace com::xiaonei::xce;

	Statement oss;
	oss << "SELECT page_id FROM " << TableName::page_fans_table(hostid)
			<< " WHERE user_id = " << hostid << " LIMIT " << limit;
	std::vector<int> page_seq;

  FollowPagesReadHandler handler(hostid, page_seq);
	try {
  	QueryRunner("fans_pages_a", CDbRServer).query(oss, handler);	
	} catch (Ice::Exception& e) {
		MCE_WARN("[CommonDbHelper] GetUserFollowPages Connection Exception: " << e.what() << "\n");
	} catch (std::exception& e) {
		MCE_WARN("[CommonDbHelper] GetUserFollowPages std Exception: " << e.what() << "\n");
	} catch (...) {
		MCE_WARN("[CommonDbHelper] GetUserFollowPages Unknow Exception");
	}

	return page_seq;
}

//---------------------------------------------------------------------------------------------------------------

bool CommonFriendSetResultHandler::handle(mysqlpp::Row& row) const {
	if (EMPTY_ID == (int)row["friend_id"]) {
		return true;
	}
	CommonFriend cf;
	cf.userId = (int) row["friend_id"];
	cf.weight = (int) row["weight"];

	vector<int> tmp = MyUtil::IntSeqSerialize::unserialize( row["shares"] );
	if( tmp.front() >= (int)SAVE_DATA_LENGTH - 1 ) {
		cf.shares = vector<int>(++tmp.begin(), tmp.end());             //实际共同好友数超出存储长度，则取除第一位外全部字节串，要舍弃末尾补0位
	} else {
		cf.shares = vector<int>(++tmp.begin(), tmp.begin() + tmp.front() + 1);     //实际共同好友书未超出存储长度，则取实际共同好友个数的字节串长度，要舍弃末尾补0位
	}
	cf.sharesCount = tmp.front();

	results_.push_back(cf);
	return true;
}

//---------------------------------------------------------------------------------------------------------------

bool UsertimeBatchReadHandler::handle(mysqlpp::Row& row) const {
	last_ = last_ > (int)row["id"] ? last_ : (int)row["id"];
	idseq_.push_back((int)row["id"]);
	return true;
}

//---------------------------------------------------------------------------------------------------------------

bool FollowSitesReadHandler::handle(mysqlpp::Row& row) const {
	int site_id = (int)row["site_id"];
	site_seq_.push_back(site_id);
	return true;
}

//---------------------------------------------------------------------------------------------------------------

bool FollowPagesReadHandler::handle(mysqlpp::Row& row) const {
	int page_id = (int)row["page_id"];
	page_seq_.push_back(page_id);
	return true;
}

//---------------------------------------------------------------------------------------------------------------

bool FollowSitesUrlHandler::handle(mysqlpp::Row& row) const {
	int site_id = (int)row["id"];
	std::string url = (std::string)row["url"];

	site_url_map_[site_id] = url;
	return true;
}

}
}
