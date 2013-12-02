/*************************************************************************
    > File Name: PageUtil.cpp
    > Author: zunwen.you
    > Mail: zunwen.you@renren-inc.com 
    > Created Time: 2012年09月28日 星期五 14时23分29秒
 ************************************************************************/
#include "PageUtil.h"

using namespace std;
using namespace MyUtil;
using namespace xce::offerfriends;
using namespace mop::hi::svc::model;
using namespace com::xiaonei::xce;


IntSeq PageUtil::getUserPages(int userId) {
	Statement sql;
  int tableSurfix = userId % 100;
	sql << "SELECT user_id, page_id FROM fans_pages_" << tableSurfix <<" WHERE user_id = ";
  sql << userId;
  
	IntSeq pageSeq;
	try {
		QueryRunner("fans_pages_a", CDbRServer).query(sql, UserPagesHandler(pageSeq));
	} catch (Ice::Exception& e) {
		MCE_WARN("[PageUtil] getUserPages Ice Exception " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[PageUtil] getUserPages std exception " << e.what());
	} catch (...) {
		MCE_WARN("[PageUtil] getUserPages unknown exception");
	}
  MCE_INFO("[PageUtil::getUserPages] userId: " << userId << " follow page size: " << pageSeq.size());
	return pageSeq;
}

IntSeq PageUtil::getUniversityPages(const UniversityInfoPtr universityInfoPtr) {
	Statement sql;
	sql << "SELECT tid, value FROM page_recommend_config WHERE tid = ";
	sql << universityInfoPtr->universityId;

  MyUtil::IntSeq pageSeq;
	try {
		QueryRunner("page", CDbRServer).query(sql, UniversityPagesHandler(pageSeq));
	} catch (Ice::Exception& e) {
		MCE_WARN("[PageUtil] getUniversityPages Ice Exception " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[PageUtil] getUniversityPages std exception " << e.what());
	} catch (...) {
		MCE_WARN("[PageUtil] getUniversityPages unknown exception");
	}
  MCE_INFO("[PageUtil::getUniversityPages] university name: " << universityInfoPtr->universityName 
      <<"(" <<universityInfoPtr->universityId << ")" << " page size: " << pageSeq.size());
	return pageSeq;

}

IntSeq PageUtil::getCollegePages(const CollegeInfoPtr collegeInfoPtr) {
	Statement sql;
	sql << "SELECT tid, value FROM page_recommend_config WHERE tid = ";
	sql << collegeInfoPtr->collegeId;

  MyUtil::IntSeq pageSeq;
	try {
		QueryRunner("page", CDbRServer).query(sql, CollegePagesHandler(pageSeq));
	} catch (Ice::Exception& e) {
		MCE_WARN("[PageUtil] getCollegePages Ice Exception " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[PageUtil] getCollegePages std exception " << e.what());
	} catch (...) {
		MCE_WARN("[PageUtil] getCollegePages unknown exception");
	}

  MCE_INFO("[PageUtil::getCollegePages] college name: " << collegeInfoPtr->collegeName 
      << "(" << collegeInfoPtr->collegeId << ")" << " page size: " << pageSeq.size());
	return pageSeq;
}


bool UserPagesHandler::handle(mysqlpp::Row& row) const {
	int pageId = (int)row["page_id"];
  pageSeq_.push_back(pageId);
	return true;
}

bool UniversityPagesHandler::handle(mysqlpp::Row& row) const {
	int pageId = (int)row["value"];
  pageSeq_.push_back(pageId);
	return true;
}

bool CollegePagesHandler::handle(mysqlpp::Row& row) const {
	int pageId = (int)row["value"];
  pageSeq_.push_back(pageId);
	return true;
}
