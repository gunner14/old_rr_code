#ifndef __USERADSCACHE_LOADER_I_H__
#define __USERADSCACHE_LOADER_I_H__

#include <iostream>
#include <string>
#include <IceUtil/IceUtil.h>
#include <IceUtil/Thread.h>
#include <QueryRunner.h>
#include <ServiceI.h>
#include "UserAdsCacheDataI.h"
#include "UtilCxx/src/AtomicSTL.h"
#include "ReplicaCluster/CacheLoader/src/CacheLoaderFactory.h"

namespace xce {
namespace useradscache {

const std::string DB_USER_PASSPORT = "useradscache_user_passport";
const std::string DB_USER_BORN = "useradscache_user_born";
const std::string DB_USER_STAGE = "useradscache_user_stage";
const std::string DB_USER_URL = "useradscache_user_url";
const std::string DB_REGION_INFO = "useradscache_region_info";
const std::string DB_WORKPLACE_INFO = "useradscache_workplace_info";
const std::string DB_NETWORK_HISTORY = "useradscache_network_history";
const std::string DB_ELEMENTARY_SCHOOL_INFO = "useradscache_elementary_school_info";
const std::string DB_JUNIOR_HIGH_SCHOOL_INFO = "useradscache_junior_high_school_info";
const std::string DB_HIGH_SCHOOL_INFO = "useradscache_high_school_info";
const std::string DB_COLLEGE_INFO = "useradscache_college_info";
const std::string DB_UNIVERSITY_INFO = "useradscache_university_info";

class UserAdsCacheFactory : virtual public xce::cacheloader::MinMaxFactory {
public:
	UserAdsCacheFactory() {}
	virtual ~UserAdsCacheFactory() {}
	virtual MyUtil::ObjectResultPtr create(const MyUtil::IntSeq& ids);
	virtual MyUtil::ObjectResultPtr create(Ice::Int minId,Ice::Int maxId);
};

class UserAdsCacheLoaderI : virtual public UserAdsCacheLoader,
        virtual public MyUtil::Singleton<UserAdsCacheLoaderI>
{
public:
	UserAdsCacheLoaderI() {}
        virtual ~UserAdsCacheLoaderI() {} 	
        virtual void load(const ::MyUtil::IntSeq& userIds, const Ice::Current&);
};
//---------------------------------Thread-----------------------------

class BatchUserBornThread: public IceUtil::Thread {
public:
	BatchUserBornThread(const std::string& ids, const MyUtil::ObjectResultPtr& result);
	BatchUserBornThread(int minId, int maxId, const MyUtil::ObjectResultPtr& result);
	virtual ~BatchUserBornThread() {}
	virtual void run();
private:
	com::xiaonei::xce::Statement _sql;
	MyUtil::ObjectResultPtr _result;
};

class BatchUserStageThread: public IceUtil::Thread {
public:
	BatchUserStageThread(const std::string& ids, const MyUtil::ObjectResultPtr& result);
	BatchUserStageThread(int minId, int maxId, const MyUtil::ObjectResultPtr& result);
	virtual ~BatchUserStageThread() {}
	virtual void run();
private:
	com::xiaonei::xce::Statement _sql;
	MyUtil::ObjectResultPtr _result;
};

class BatchUserUrlThread: public IceUtil::Thread {
public:
	BatchUserUrlThread(const std::string& ids, const MyUtil::ObjectResultPtr& result);
	BatchUserUrlThread(int minId, int maxId, const MyUtil::ObjectResultPtr& result);
	virtual ~BatchUserUrlThread() {}
	virtual void run();
private:
	com::xiaonei::xce::Statement _sql;
	MyUtil::ObjectResultPtr _result;
};

class BatchRegionInfoThread: public IceUtil::Thread {
public:
	BatchRegionInfoThread(const std::string& ids, const MyUtil::ObjectResultPtr& result);
	BatchRegionInfoThread(int minId, int maxId, const MyUtil::ObjectResultPtr& result);
	virtual ~BatchRegionInfoThread() {}
	virtual void run();
private:
	com::xiaonei::xce::Statement _sql;
	MyUtil::ObjectResultPtr _result;
};

class BatchNetworkHistoryThread: public IceUtil::Thread {
public:
	BatchNetworkHistoryThread(const std::string& ids, const MyUtil::ObjectResultPtr& result);
	BatchNetworkHistoryThread(int minId, int maxId, const MyUtil::ObjectResultPtr& result);
	virtual ~BatchNetworkHistoryThread() {}
	virtual void run();
private:
	com::xiaonei::xce::Statement _sql;
	MyUtil::ObjectResultPtr _result;
};

class BatchWorkplaceInfoThread: public IceUtil::Thread {
public:
	BatchWorkplaceInfoThread(const string& ids, const MyUtil::ObjectResultPtr& result);
	BatchWorkplaceInfoThread(int minId, int maxId, const MyUtil::ObjectResultPtr& result);
	virtual ~BatchWorkplaceInfoThread() {}
	virtual void run();
private:
	com::xiaonei::xce::Statement _sql;
	MyUtil::ObjectResultPtr _result;
};

class BatchElementarySchoolInfoThread: public IceUtil::Thread {
public:
	BatchElementarySchoolInfoThread(const std::string& ids, const MyUtil::ObjectResultPtr& result);
	BatchElementarySchoolInfoThread(int minId, int maxId, const MyUtil::ObjectResultPtr& result);
	virtual ~BatchElementarySchoolInfoThread() {}
	virtual void run();
private:
	com::xiaonei::xce::Statement _sql;
	MyUtil::ObjectResultPtr _result;
};

class BatchJuniorHighSchoolInfoThread: public IceUtil::Thread {
public:
	BatchJuniorHighSchoolInfoThread(const std::string& ids, const MyUtil::ObjectResultPtr& result);
	BatchJuniorHighSchoolInfoThread(int minId, int maxId, const MyUtil::ObjectResultPtr& result);
	virtual ~BatchJuniorHighSchoolInfoThread() {}
	virtual void run();
private:
	com::xiaonei::xce::Statement _sql;
	MyUtil::ObjectResultPtr _result;
};

class BatchHighSchoolInfoThread: public IceUtil::Thread {
public:
	BatchHighSchoolInfoThread(const std::string& ids, const MyUtil::ObjectResultPtr& result);
	BatchHighSchoolInfoThread(int minId, int maxId, const MyUtil::ObjectResultPtr& result);
	virtual ~BatchHighSchoolInfoThread() {}
	virtual void run();
private:
	com::xiaonei::xce::Statement _sql;
	MyUtil::ObjectResultPtr _result;
};

class BatchCollegeInfoThread: public IceUtil::Thread {
public:
	BatchCollegeInfoThread(const std::string& ids, const MyUtil::ObjectResultPtr& result);
	BatchCollegeInfoThread(int minId, int maxId, const MyUtil::ObjectResultPtr& result);
	virtual ~BatchCollegeInfoThread() {}
	virtual void run();
private:
	com::xiaonei::xce::Statement _sql;
	MyUtil::ObjectResultPtr _result;
};

class BatchUniversityInfoThread: public IceUtil::Thread {
public:
	BatchUniversityInfoThread(const std::string& ids, const MyUtil::ObjectResultPtr& result);
	BatchUniversityInfoThread(int minId, int maxId, const MyUtil::ObjectResultPtr& result);
	virtual ~BatchUniversityInfoThread() {}
	virtual void run();
private:
	com::xiaonei::xce::Statement _sql;
	MyUtil::ObjectResultPtr _result;
};

//---------------------------------BatchResultHandler-----------------------------

class BatchUserPassportResultHandlerI: public com::xiaonei::xce::ResultHandler {
public:
        BatchUserPassportResultHandlerI(MyUtil::ObjectResultPtr& result);
        virtual bool handle(mysqlpp::Row& row) const;
private:
        MyUtil::ObjectResultPtr _result;
};

class BatchUserBornResultHandlerI: public com::xiaonei::xce::ResultHandler {
public:
        BatchUserBornResultHandlerI(MyUtil::ObjectResultPtr& result);
        virtual bool handle(mysqlpp::Row& row) const;
private:
        MyUtil::ObjectResultPtr _result;
};

class BatchUserStageResultHandlerI: public com::xiaonei::xce::ResultHandler {
public:
        BatchUserStageResultHandlerI(MyUtil::ObjectResultPtr& result);
        virtual bool handle(mysqlpp::Row& row) const;
private:
        MyUtil::ObjectResultPtr _result;
};

class BatchUserUrlResultHandlerI: public com::xiaonei::xce::ResultHandler {
public:
        BatchUserUrlResultHandlerI(MyUtil::ObjectResultPtr& result);
        virtual bool handle(mysqlpp::Row& row) const;
private:
        MyUtil::ObjectResultPtr _result;
};

class BatchRegionResultHandlerI: public com::xiaonei::xce::ResultHandler {
public:
        BatchRegionResultHandlerI(MyUtil::ObjectResultPtr& result);
        virtual bool handle(mysqlpp::Row& row) const;
private:
        MyUtil::ObjectResultPtr _result;
};

class BatchWorkplaceResultHandlerI: public com::xiaonei::xce::ResultHandler {
public:
        BatchWorkplaceResultHandlerI(MyUtil::ObjectResultPtr& result);
        virtual bool handle(mysqlpp::Row& row) const;
private:
        MyUtil::ObjectResultPtr _result;
};

class BatchNetworkResultHandlerI: public com::xiaonei::xce::ResultHandler {
public:
        BatchNetworkResultHandlerI(MyUtil::ObjectResultPtr& result);
        virtual bool handle(mysqlpp::Row& row) const;
private:
        MyUtil::ObjectResultPtr _result;
};

class BatchElementaryResultHandlerI: public com::xiaonei::xce::ResultHandler {
public:
        BatchElementaryResultHandlerI(MyUtil::ObjectResultPtr& result);
        virtual bool handle(mysqlpp::Row& row) const;
private:
        MyUtil::ObjectResultPtr _result;
};

class BatchJuniorResultHandlerI: public com::xiaonei::xce::ResultHandler {
public:
        BatchJuniorResultHandlerI(MyUtil::ObjectResultPtr& result);
        virtual bool handle(mysqlpp::Row& row) const;
private:
        MyUtil::ObjectResultPtr _result;
};

class BatchSeniorResultHandlerI: public com::xiaonei::xce::ResultHandler {
public:
        BatchSeniorResultHandlerI(MyUtil::ObjectResultPtr& result);
        virtual bool handle(mysqlpp::Row& row) const;
private:
        MyUtil::ObjectResultPtr _result;
};

class BatchCollegeResultHandlerI: public com::xiaonei::xce::ResultHandler {
public:
        BatchCollegeResultHandlerI(MyUtil::ObjectResultPtr& result);
        virtual bool handle(mysqlpp::Row& row) const;
private:
        MyUtil::ObjectResultPtr _result;
};

class BatchUniversityResultHandlerI: public com::xiaonei::xce::ResultHandler {
public:
        BatchUniversityResultHandlerI(MyUtil::ObjectResultPtr& result);
        virtual bool handle(mysqlpp::Row& row) const;
private:
        MyUtil::ObjectResultPtr _result;
};
//---------------------------------BatchResultHandler end-----------------------------

}
}

#endif
