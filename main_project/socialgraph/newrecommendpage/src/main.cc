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
#include "OceCxxAdapter/src/UserUniversityTripodAdapter.h"

using namespace xce::socialgraph;
using namespace boost::program_options;
using namespace boost::filesystem;
using namespace MyUtil;
using namespace xce;
using namespace xce::socialgraph;
using namespace xce::adapter;

//typedef std::vector<int> IntSeq;
static const int TASK_LEVEL_CREATE_RECOMMEND = 5;

//load uid list from file
void   LoadUserIdlist(const std::string& filename, IntSeq& idlist);

//get  page fans from db
void   GetPageFans(int pageid, IntSeq& idlist);

//get  user uid list from db
//void   GetUserIdList(const std::string& useridtablename, IntSeq& useridlist);

//get  page id list from db
void   GetPageIdMap(const std::string&  pageidtablename, std::map<int, int>& pageidmap);

//get  record num of talble 
void   GetTableRecordNumber(const std::string& instancename, const std::string& tablename, int& recordnum);

//set  data into db
void   SetRecommendDb(int pageid, int collegeid, const IntSeq& useridlist, const IntSeq& fansidlist);

//-----------------------------------------------------------------------
/*Get Fans Data From Db*/
class PageFansDataHandler: public com::xiaonei::xce::ResultHandler {
 public:
	PageFansDataHandler(IntSeq& idlist): idlist_(idlist) {}

	virtual bool handle(mysqlpp::Row& row) const {
		idlist_.push_back((int)row["user_id"]);
		return true;
	}

 private:
	IntSeq& idlist_;
};

/*Get <Page ID, Collogde ID> From Db*/
class PageIdDataHandler: public com::xiaonei::xce::ResultHandler {
 public:
	PageIdDataHandler(std::map<int,int>& idmap): idmap_(idmap) {}

	virtual bool handle(mysqlpp::Row& row) const {//page id and univercity id
		idmap_.insert(pair<int,int>((int)row["value"],(int)row["tid"]));
		return true;
	}

 private:
	std::map<int,int>& idmap_;
};

/*Get User Id From Db*/
class UserIdDataHandler: public com::xiaonei::xce::ResultHandler {
 public:
	UserIdDataHandler(IntSeq& idlist): idlist_(idlist) {}

	virtual bool handle(mysqlpp::Row& row) const {
		idlist_.push_back((int)row["user_id"]);
		return true;
	}

 private:
	IntSeq& idlist_;
};

/*check records size of Db by 'show index form tablename' */
class   ShowIndexHandler: public com::xiaonei::xce::ResultHandler {
 public:
	ShowIndexHandler(int& recordnum): recordnum_(recordnum) {}

	virtual bool handle(mysqlpp::Row& row) const {
                int  temp = (int)row["Cardinality"];
		if(temp > recordnum_){ 
                   recordnum_ = temp;
                }
		return true;
	}

 private:
	int& recordnum_;
};


//-----------------------------------------------------------------------
/*Get data from recommend db for test*/
class PageRecommendDataHandler: public com::xiaonei::xce::ResultHandler {
 public:
	PageRecommendDataHandler() {}

	virtual bool handle(mysqlpp::Row& row) const {
		CLOG(INFO) << row["user_id"] << ":" << row["page_id"] << ":" << row["common_friend_number"];
		IntSeq shares = MyUtil::IntSeqSerialize::unserialize(row["common_friend_ids"]);
		PRINT_ELEMENTS(shares);
		return true;
	}
};

//-----------------------------------------------------------------------
/*Compute common friends and set recommend db*/
class CreatePageRecommendTask: public MyUtil::Task {
 public:
	CreatePageRecommendTask(int userid, int pageid, const IntSeq& fans_list, ::Ice::Int level = TASK_LEVEL_CREATE_RECOMMEND):
		MyUtil::Task(TASK_LEVEL_CREATE_RECOMMEND), userid_(userid), pageid_(pageid), fans_list_(fans_list) {}

	virtual void handle() {
		std::ostringstream oss;
		oss << "CreatePageRecommendTask userid:" << userid_ << " pageid:" << pageid_ << " fans:" << fans_list_.size();
		RunTime timecost = RunTime::create(oss.str());

		FriendFetcher fetcher;
		IntSeq  friends_list;
		fetcher.Read(userid_, friends_list);
		
		if (!friends_list.empty()) {
			IntSeq  shares;
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
                             oss.str("");
                             oss << "CreatePageRecommendTask userid:" << userid_ << " pageid:" << pageid_ << " comomnfriends:" << shares.size();
                             RunTime timecost = RunTime::create(oss.str());

			     InsertDb(userid_, pageid_, shares);  //Insert recommend db;
			     //GetDb(userid_, appid);		//Get Data
			}
		}
	}

 private:
	void InsertDb(int userid, int pageid, IntSeq& shares) {
		using namespace com::xiaonei::xce;
		std::string tablename = "recommend_by_page_college";
		int shares_limit = 7;			//limit size of share is 7

		Statement sql;
		sql
			<< "INSERT INTO " << tablename << " (user_id, page_id, fans_counter, common_friend_number, common_friend_ids) VALUES "
			<< "(" << userid << ", " << pageid << ", " << (int)shares.size() << ", " << (int)shares.size() << ", ";

		if ((int)shares.size() > shares_limit) {		
			shares.resize(shares_limit);	
			sql << mysqlpp::quote << MyUtil::IntSeqSerialize::serialize(shares) << ")";
		} else {
			sql << mysqlpp::quote << MyUtil::IntSeqSerialize::serialize(shares) << ")";
		}

		sql << " ON DUPLICATE KEY UPDATE fans_counter=VALUES(fans_counter), common_friend_number=VALUES(common_friend_number), common_friend_ids=VALUES(common_friend_ids);";

		try {
			QueryRunner("socialgraph_recommend_page_new", CDbWServer).execute(sql);
		} catch (Ice::Exception& e) {
			CLOG(WARN) << "[CreatePageRecommendTask] InsertDb Ice Exception: " << e.what();
		} catch (std::exception& e) {
			CLOG(WARN) << "[CreatePageRecommendTask] InsertDb std Exception: " << e.what();
		} catch (...) {
			CLOG(WARN) << "[CreatePageRecommendTask] InsertDb Unknown Exception";
		}
	}

	void GetDb(int userid, int pageid) {
		using namespace com::xiaonei::xce;
		std::string tablename = "recommend_by_page_college";

		Statement sql;
		sql
			<< "SELECT * FROM " << tablename << " WHERE user_id = " << userid << " AND page_id = " << pageid;
		
		try {
			QueryRunner("socialgraph_recommend_page_new", CDbWServer).query(sql, PageRecommendDataHandler());
		} catch (Ice::Exception& e) {
			CLOG(WARN) << "[CreatePageRecommendTask] GetDb Ice Exception: " << e.what();
		} catch (std::exception& e) {
			CLOG(WARN) << "[CreatePageRecommendTask] GetDb std Exception: " << e.what();
		} catch (...) {
			CLOG(WARN) << "[CreatePageRecommendTask] GetDb Unknown Exception";
		}
	}

	int userid_;
	int pageid_;
	const IntSeq& fans_list_;
};

//-----------------------------------------------------------------------

int main(int argc, char* argv[]) {
	// 1. get loading id
	// 2. get page fans
	// 3. compute common friends
	// 4. set db
        InitLogger("PageRecommendCollege");

        if(argc !=2 || access(argv[1], R_OK) != 0){
           CLOG(ERROR) << "[main] wrong input parameters(files path)!" ;
           return -1;
        }
        
        std::string  pageidtablename    = "page_recommend_config";
        std::string  recommendtablename = "recommend_by_page_college";
        
        int  pageidnum = 0;
        int  useridnum = 0;
        int  recommendnum = 0;

        CLOG(INFO) << "page id table name is " << pageidtablename;
        com::xiaonei::xce::ConnectionPoolManager::instance().initialize();
       
        GetTableRecordNumber("page", pageidtablename, pageidnum);
        GetTableRecordNumber("socialgraph_recommend_page_new", recommendtablename, recommendnum);
        
        /*pre-condition:pageid num < 300*/
        if(/*(recommendnum>0) || */(pageidnum<=0 || pageidnum>300)){
          CLOG(WARN) << "[main] pageid num is " << pageidnum << ",page recommend num in DB is " << recommendnum << ",and the main has returned.";
          return  0;
        }
        
        IntSeq            useridlist;
        std::map<int,int> pageidmap;
        IntSeq            fanslist;
         
        LoadUserIdlist(argv[1], useridlist);//load user id 
        GetPageIdMap(pageidtablename, pageidmap);//get page id
        
        CLOG(INFO) << "[main] pageid num is " << pageidnum << ",userid num is " << useridlist.size();
        
        for(std::map<int,int>::const_iterator iter = pageidmap.begin(); 
                    iter != pageidmap.end(); ++iter){ //for each page id 
        
           int  pageid    = iter->first;
           int  collegeid = iter->second;
           
           GetPageFans(pageid, fanslist);
           CLOG(INFO) << "[main] fans list size of page id " << pageid << " is " << fanslist.size();  
           if(fanslist.size() > 0){
              SetRecommendDb(pageid, collegeid, useridlist, fanslist);  
              fanslist.clear();//clear the connection set of fans.
           }
        } 
        CLOG(INFO) << "[main] run over!"; 

        return 0;         
}

void   GetTableRecordNumber(const std::string& instancename, const std::string& tablename, int& recordnum){

       	using namespace com::xiaonei::xce;

        if(!tablename.empty()){
          
		Statement sql;
		sql
			<< "SHOW index FROM " << tablename;

		try {
			QueryRunner(instancename, CDbRServer).query(sql, ShowIndexHandler(recordnum));
		} catch (Ice::Exception& e) {
			CLOG(WARN) << "[GetTableRecordNumber] Connection Exception: " << e.what();
		} catch (mysqlpp::Exception& e) {
			CLOG(WARN) << "[GetTableRecordNumber] mysqlpp Exception: " << e.what();
		} catch (...) {
			CLOG(WARN) << "[GetTableRecordNumber] Unknown Exception";
		}
        }         
}

/*get page id map from DB*/
void   GetPageIdMap(const std::string& pageidtablename,std::map<int, int>& pageidmap){
	using namespace com::xiaonei::xce;

        if(!pageidtablename.empty()){
          
		Statement sql;
		sql
			<< "SELECT value, tid FROM " << pageidtablename << " WHERE type = 1";

		try {
			QueryRunner("page", CDbRServer).query(sql, PageIdDataHandler(pageidmap));
		} catch (Ice::Exception& e) {
			CLOG(WARN) << "[GetPageIdList] Connection Exception: " << e.what();
		} catch (mysqlpp::Exception& e) {
			CLOG(WARN) << "[GetPageIdList] mysqlpp Exception: " << e.what();
		} catch (...) {
			CLOG(WARN) << "[GetPageIdList] Unknown Exception";
		}
        }
}

/*new changes: get user id list from DB*/
void   GetUserIdList(const std::string& useridtablename,IntSeq& useridlist){
	using namespace com::xiaonei::xce;
	
        if(!useridtablename.empty()){

		Statement sql;
		sql
			<< "SELECT user_id FROM " << useridtablename;

		try {
			QueryRunner("socialgraph_recommend_app", CDbRServer).query(sql, UserIdDataHandler(useridlist));
		} catch (Ice::Exception& e) {
			CLOG(WARN) << "[GetUserIdList] Connection Exception: " << e.what();
		} catch (mysqlpp::Exception& e) {
			CLOG(WARN) << "[GetUserIdList] mysqlpp Exception: " << e.what();
		} catch (...) {
			CLOG(WARN) << "[GetUserIdList] Unknown Exception";
		}
        }
}

void LoadUserIdlist(const std::string& filename, IntSeq& idlist) {
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

void GetPageFans(int pageid, IntSeq& idlist) {
	using namespace com::xiaonei::xce;

        for (int i = 0; i< 100; i++){
  	     
             std::string table_name = "fans_pages_" + boost::lexical_cast<std::string>(i);

	     Statement sql;
	     sql
	             << "SELECT user_id FROM " << table_name
		     << " WHERE page_id = " << pageid << " AND state = 0";

	     try {
	             QueryRunner("fans_pages", CDbRServer, table_name).query(sql, PageFansDataHandler(idlist));
                 } catch (Ice::Exception& e) {
		     CLOG(WARN) << "[GetPageFans] Connection Exception: " << e.what();
	         } catch (mysqlpp::Exception& e) {
	             CLOG(WARN) << "[GetPageFans] mysqlpp Exception: " << e.what();
	         } catch (...) {
		     CLOG(WARN) << "[GetPageFans] Unknown Exception";
	     }
        }
}

void SetRecommendDb(int pageid, int collegeid, const IntSeq& useridlist, const IntSeq& fanslist) {
	using namespace  MyUtil;
	TaskManager::instance().config(TASK_LEVEL_CREATE_RECOMMEND, ThreadPoolConfig(0, 100));

	int  task_size = 0;
	long wait  = 0;
	long count = 0;
	time_t  now;   
        time(&now);
        struct  tm* tm_now = localtime(&now);
       
        for (IntSeq::const_iterator iter = useridlist.begin();
			iter != useridlist.end(); ++iter) {

                mop::hi::svc::model::UniversityInfoSeq pUn = UserUniversityTripodAdapter::instance().getUniversityInfoSeq(*iter);
                int  max_enrollyear = 0;
                int  max_collegeid  = 0;
                for (mop::hi::svc::model::UniversityInfoSeq::const_iterator it = pUn.begin(); it != pUn.end(); it++){
                     if ((*it)->enrollYear > max_enrollyear){
                         max_enrollyear = (*it)->enrollYear;
                         max_collegeid  = (*it)->universityId;
                     }
                }
                if((collegeid != max_collegeid) ||
                   (tm_now != NULL && ((1900 + tm_now->tm_year) - max_enrollyear) >= 3)){
                    continue;
                }    
               
		if (++count % 1000 == 0) {
		    CLOG(INFO) << "count:" << count << " total:" << useridlist.size();
		}

		TaskManager::instance().execute(new CreatePageRecommendTask(*iter, pageid, fanslist));
		while ((task_size = TaskManager::instance().size(TASK_LEVEL_CREATE_RECOMMEND)) > 10000) {
			wait += 2;
			sleep(2);
		}
	}

	sleep(100);

	CLOG(INFO) << "PageRecommend Set Db completed.";
	while (TaskManager::instance().size(TASK_LEVEL_CREATE_RECOMMEND) > 0) {
	       sleep(2);
	}
}
