#ifndef __PAGE_UTIL_H__
#define __PAGE_UTIL_H__

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <Singleton.h>
#include <UserNetwork.h>
#include <OfferFriends.h>
#include "QueryRunner.h"

namespace xce {
namespace offerfriends {


class PageUtil {
public:
  static MyUtil::IntSeq getUserPages(int userId);
  static MyUtil::IntSeq getUniversityPages(const mop::hi::svc::model::UniversityInfoPtr universityInfoPtr);
  static MyUtil::IntSeq getCollegePages(const mop::hi::svc::model::CollegeInfoPtr collegeInfoPtr);
private:
};

class UserPagesHandler: public com::xiaonei::xce::ResultHandler {
public:
	UserPagesHandler(MyUtil::IntSeq& pageSeq):
		pageSeq_(pageSeq) { }
	
	virtual bool handle(mysqlpp::Row& row) const;

private:
  MyUtil::IntSeq& pageSeq_;
};

class UniversityPagesHandler: public com::xiaonei::xce::ResultHandler {
public:
	UniversityPagesHandler(MyUtil::IntSeq& pageSeq):
		pageSeq_(pageSeq) { }
	
	virtual bool handle(mysqlpp::Row& row) const;

private:
  MyUtil::IntSeq& pageSeq_;
};

class CollegePagesHandler: public com::xiaonei::xce::ResultHandler {
public:
	CollegePagesHandler(MyUtil::IntSeq& pageSeq):
		pageSeq_(pageSeq) { }
	
	virtual bool handle(mysqlpp::Row& row) const;

private:
  MyUtil::IntSeq& pageSeq_;
};


}
}

#endif
