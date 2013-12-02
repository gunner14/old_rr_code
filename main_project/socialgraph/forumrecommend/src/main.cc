#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include "socialgraph/socialgraphutil/clogging.h"
#include "QueryRunner.h"
#include "UtilCxx/src/TaskManager.h"
#include "util/cpp/IntSeqSerialize.h"
#include "FriendFetcher.h"
#include "socialgraph/socialgraphutil/algostuff.h"
#include "socialgraph/socialgraphutil/timecost.h"

using namespace xce::socialgraph;
using namespace boost::program_options;
using namespace boost::filesystem;
using namespace MyUtil;
using namespace xce;

//typedef std::vector<int> IntSeq;
static const int TASK_LEVEL_CREATE_RECOMMEND = 5;

//load idlist from file
void LoadIdlist(const std::string& filename, IntSeq& idlist);

//get appfans from db
void GetAppFans(int appid, IntSeq& idlist);

//set data into db
void SetRecommendDb(int appid, const IntSeq& idlist, const IntSeq& fanslist);

//-----------------------------------------------------------------------
/*Get Fans Data From Db*/
class AppFansDataHandler: public com::xiaonei::xce::ResultHandler {
 public:
	AppFansDataHandler(IntSeq& idlist): idlist_(idlist) {}

	virtual bool handle(mysqlpp::Row& row) const {
		idlist_.push_back((int)row["member_id"]);
		return true;
	}

 private:
	IntSeq& idlist_;
};

//-----------------------------------------------------------------------
/*Get data from recommend db for test*/
class AppRecommendDataHandler: public com::xiaonei::xce::ResultHandler {
 public:
	AppRecommendDataHandler() {}

	virtual bool handle(mysqlpp::Row& row) const {
		CLOG(INFO) << row["userid"] << ":" << row["appid"] << ":" << row["sharescount"];
		IntSeq shares = MyUtil::IntSeqSerialize::unserialize(row["shares"]);
		PRINT_ELEMENTS(shares);
		return true;
	}
};

//-----------------------------------------------------------------------
/*Compute common friends and set recommend db*/
class CreateAppRecommendTask: public MyUtil::Task {
 public:
	CreateAppRecommendTask(int userid, int appid, const IntSeq& fans_list, ::Ice::Int level = TASK_LEVEL_CREATE_RECOMMEND):
		MyUtil::Task(TASK_LEVEL_CREATE_RECOMMEND), userid_(userid), appid_(appid), fans_list_(fans_list) {}

	virtual void handle() {
		std::ostringstream oss;
		oss << "CreateAppRecommendTask userid:" << userid_ << " appid:" << appid_ << " fans:" << fans_list_.size();
		RunTime timecost = RunTime::create(oss.str());

		FriendFetcher fetcher;
		IntSeq friends_list;
		fetcher.Read(userid_, friends_list);
		//PRINT_ELEMENTS(friends_list);		
		//PRINT_ELEMENTS(fans_list_);
		
		if (!friends_list.empty()) {
			IntSeq shares;
			IntSeq::iterator friends_iter = friends_list.begin();
			IntSeq::const_iterator fans_iter = fans_list_.begin();	

			while (friends_iter != friends_list.end() &&
						fans_iter != fans_list_.end()) {
				if (*friends_iter == *fans_iter) {
					shares.push_back(*friends_iter);
					++friends_iter;
					++fans_iter;
				} else if (*friends_iter < *fans_iter) {
					++friends_iter;
				} else {
					++fans_iter;
				}
			}

			if (!shares.empty()) {
				InsertDb(userid_, appid_, shares);		//Insert recommend db;
				//GetDb(userid_, appid);		//Get Data
			}
		}
	}

 private:
	void InsertDb(int userid, int appid, IntSeq& shares) {
		using namespace com::xiaonei::xce;
		std::string tablename = "recommend_by_forum";
		int shares_limit = 7;			//limit size of share is 7

		Statement sql;
		sql
			<< "INSERT INTO " << tablename << " (userid, appid, sharescount, create_time, shares) VALUES "
			<< "(" << userid << ", " << appid << ", " << (int)shares.size() << ", " 
			<< mysqlpp::quote << mysqlpp::DateTime(time(NULL)) << ", ";

		if ((int)shares.size() > shares_limit) {		
			shares.resize(shares_limit);	
			sql << mysqlpp::quote << MyUtil::IntSeqSerialize::serialize(shares) << ")";
		} else {
			sql << mysqlpp::quote << MyUtil::IntSeqSerialize::serialize(shares) << ")";
		}

		sql << " ON DUPLICATE KEY UPDATE sharescount=VALUES(sharescount), create_time=VALUES(create_time), shares=VALUES(shares);";

		try {
			QueryRunner("socialgraph_recommend_app", CDbWServer).execute(sql);
		} catch (Ice::Exception& e) {
			CLOG(WARN) << "[CreateAppRecommendTask] InsertDb Ice Exception: " << e.what();
		} catch (std::exception& e) {
			CLOG(WARN) << "[CreateAppRecommendTask] InsertDb std Exception: " << e.what();
		} catch (...) {
			CLOG(WARN) << "[CreateAppRecommendTask] InsertDb Unknown Exception";
		}
	}

	void GetDb(int userid, int appid) {
		using namespace com::xiaonei::xce;
		std::string tablename = "recommend_by_app";

		Statement sql;
		sql
			<< "SELECT * FROM " << tablename << " WHERE userid = " << userid << " AND appid = " << appid;
		
		try {
			QueryRunner("socialgraph_recommend_app", CDbWServer).query(sql, AppRecommendDataHandler());
		} catch (Ice::Exception& e) {
			CLOG(WARN) << "[CreateAppRecommendTask] GetDb Ice Exception: " << e.what();
		} catch (std::exception& e) {
			CLOG(WARN) << "[CreateAppRecommendTask] GetDb std Exception: " << e.what();
		} catch (...) {
			CLOG(WARN) << "[CreateAppRecommendTask] GetDb Unknown Exception";
		}
	}

	int userid_;
	int appid_;
	const IntSeq& fans_list_;
};

//-----------------------------------------------------------------------

int main(int argc, char* argv[]) {
	// 1. get loading id
	// 2. get app fans
	// 3. compute common friends
	// 4. set db
	CLOG(INFO) << "apprecommend";
	options_description opts("AppRecommend options");
	opts.add_options()
				("help", "./AppRecommend --filename file --appid app_id")
				("filename", value<std::string>())
				("appid", value<int>());
				;

	variables_map vm;
	store(parse_command_line(argc, argv, opts), vm);

	if (vm.count("help")) {
		CLOG(INFO) << opts;
		return 0;
	}
	if (vm.count("filename") && vm.count("appid")) {
		CLOG(INFO) << "filename:" << vm["filename"].as<std::string>() << " appid:" << vm["appid"].as<int>();
		com::xiaonei::xce::ConnectionPoolManager::instance().initialize();

		IntSeq idlist;
		IntSeq fanslist;

		LoadIdlist(vm["filename"].as<std::string>(), idlist);
		CLOG(INFO) << "idlist size:" << idlist.size();

		GetAppFans(vm["appid"].as<int>(), fanslist);
		CLOG(INFO) << "fanslist size: " << fanslist.size();
		sort(fanslist.begin(), fanslist.end());
		SetRecommendDb(vm["appid"].as<int>(), idlist, fanslist);
	}
	if (vm.size() == 0) {
		CLOG(INFO) << "no options";
	}
}

void LoadIdlist(const std::string& filename, IntSeq& idlist) {
	path p(filename);
	if (exists(p)) {
		std::ifstream infile(filename.c_str(), std::ifstream::in);
		std::string line = "";
		while (getline(infile, line)) {
			int id = 0;
			std::istringstream stream(line);
			stream >> id;
			if (0 != id) {
				idlist.push_back(id);
			}
		}
	} else {
		CLOG(INFO)  << filename << " do not exists!!";
		assert(false);
	}
}

void GetAppFans(int appid, IntSeq& idlist) {
	using namespace com::xiaonei::xce;
	std::string table_name = "forum_member_" + boost::lexical_cast<std::string>(appid % 100);

	Statement sql;
	sql
		<< "SELECT member_id FROM " << table_name
		<< " WHERE forum_id=" << appid;

	try {
		QueryRunner("forum", CDbRServer, table_name).query(sql, AppFansDataHandler(idlist));
	} catch (Ice::Exception& e) {
		CLOG(WARN) << "[GetAppFans] Connection Exception: " << e.what();
	} catch (mysqlpp::Exception& e) {
		CLOG(WARN) << "[GetAppFans] mysqlpp Exception: " << e.what();
	} catch (...) {
		CLOG(WARN) << "[GetAppFans] Unknown Exception";
	}
}

void SetRecommendDb(int appid, const IntSeq& idlist, const IntSeq& fanslist) {
	using namespace MyUtil;
	TaskManager::instance().config(TASK_LEVEL_CREATE_RECOMMEND, ThreadPoolConfig(0, 100));

	int task_size = 0;
	long wait = 0;
	long count = 0;
	for (IntSeq::const_iterator iter = idlist.begin();
			iter != idlist.end(); ++iter) {
		if (++count % 1000 == 0) {
			CLOG(INFO) << "count:" << count << " total:" << idlist.size();
		}

		TaskManager::instance().execute(new CreateAppRecommendTask(*iter, appid, fanslist));
		while ((task_size = TaskManager::instance().size(TASK_LEVEL_CREATE_RECOMMEND)) > 10000) {
			wait += 2;
			sleep(2);
		}
	}

	sleep(100);

	CLOG(INFO) << "AppRecommend Set Db completed.";
	while (TaskManager::instance().size(TASK_LEVEL_CREATE_RECOMMEND) > 0) {
		sleep(2);
	}
}
