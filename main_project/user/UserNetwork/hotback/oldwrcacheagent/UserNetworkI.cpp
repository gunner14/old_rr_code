#include "UserNetworkI.h"
#include "LogWrapper.h"
#include "Util.h"
#include <boost/lexical_cast.hpp>
#include "BatchUpdaterAdapter.h"
#include "UserLogicAdapter.h"
#include "Date.h"
#include "ServiceI.h"
#include "QueryRunner.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

#include "util/cpp/TimeCost.h"
#include "util/cpp/InvokeMonitor.h"
#include "Monitor/client/service/ServiceMonitor.h"
#include "OceCxxAdapter/src/user_network_tripod_adapter.h"

using namespace mop::hi::svc::model;
using namespace mop::hi::svc::adapter;
using namespace MyUtil;
using namespace ::xce::serverstate;
using namespace xce::tempmutex;

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();

	NetworkManagerPtr manager = new NetworkManagerI;
	service.getAdapter()->add(manager, service.createIdentity(NETWORK_MANAGER, ""));

	TaskManager::instance().config(TASK_LEVEL_SYNCCREATE, MyUtil::ThreadPoolConfig(1, 5));

	int mod = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("UserNetworkN.Mod", 0);
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("UserNetworkN.Interval", 5);
	ServerStateSubscriber::instance().initialize("ControllerUserNetwork", manager, mod, interval);
	{
		int status = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("UserNetworkN.Status", 1);
		ServerStateSubscriber::instance().setStatus(status);
		std::ostringstream os;
		os << "MyUtil::initialize set status : " << status;
		MCE_WARN( os.str() );
	}
	ServiceMonitorManager::instance().start();
}

//--------------------------------------------------------------
NetworkManagerI::NetworkManagerI() :
	_isValid(false) {
	_tempMutexManager = new TempMutexManager();
	_factory = new UserNetworkDataFactory;
}

NetworkManagerI::~NetworkManagerI() {
}

//--------------------------------------------------------------
NetworkPtr NetworkManagerI::getNetwork(Ice::Int userId, Ice::Int networkId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << "  networkId=" << networkId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getNetwork(userId, networkId);
}

NetworkPtr NetworkManagerI::getRegionalNetwork(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getRegionalNetwork(userId);
}

NetworkPtr NetworkManagerI::getNetworkByNetId(Ice::Int userId, Ice::Int networkId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "userid=" << userId << "  networkId=" << networkId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getNetworkByNetId(userId, networkId);
}

NetworkSeq NetworkManagerI::getNetworkSeq(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getNetworkSeq(userId);
}

NetworkSeq NetworkManagerI::getNetworkSeqByStage(Ice::Int userId, Ice::Int stage, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getNetworkSeqByStage(userId, stage);
}

NetworkSeq NetworkManagerI::getNetworkSeqByStatus(Ice::Int userId, Ice::Int status, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , status=" << status;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getNetworkSeqByStatus(userId, status);
}

NetworkSeq NetworkManagerI::getNetworkSeqByStatuses(Ice::Int userId, const MyUtil::IntSeq& status, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getNetworkSeqByStatuses(userId, status);
}

NetworkSeq NetworkManagerI::getNetworkSeqByStageAndStatus(Ice::Int userId, Ice::Int stage, const MyUtil::IntSeq& status, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , stage=" << stage;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getNetworkSeqByStageAndStatus(userId, stage, status);
}

Ice::Int NetworkManagerI::getNetworkCountByStage(Ice::Int userId, Ice::Int stage, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , stage=" << stage;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getNetworkCountByStage(userId, stage);
}

Ice::Int NetworkManagerI::getValidNetworkCountByStage(Ice::Int userId, Ice::Int stage, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , stage=" << stage;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getValidNetworkCountByStage(userId, stage);
}

bool NetworkManagerI::isExistInNetwork(Ice::Int userId, Ice::Int networkId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , networkid=" << networkId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().isExistInNetwork(userId, networkId);
}

void NetworkManagerI::reloadNetworkSeq(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	reloadAll(userId);
}

Ice::Int NetworkManagerI::addNetwork(Ice::Int userId, const MyUtil::Str2StrMap& network, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	int index = 0;
	try {
		{
			Lock lock(_tempMutexManager, userId);
			MCE_INFO("[NetworkManagerI::addNetwork] lock : id=" << userId
				<< ", Client=" << (current.con ? current.con->toString().substr(current.con->toString().find("remote",25)).substr(17):"localhost:-1" ));
			index = QueryRunner("common", CDbWServer).execute(Statement::buildInsert(TABLE_NETWORK, network)).insert_id();
			MyUtil::Str2StrMap& finalNetwok = const_cast<MyUtil::Str2StrMap&> (network);
			finalNetwok.insert(make_pair("id", boost::lexical_cast<string>(index)));
			UserNetworkReaderAdapter::instance().addNetwork(userId, finalNetwok);
			MCE_INFO("[NetworkManagerI::addNetwork] unlock : id=" << userId
				<< ", Client=" << (current.con ? current.con->toString().substr(current.con->toString().find("remote",25)).substr(17):"localhost:-1" ));
		}
		xce::adapter::UserNetworkTripodAdapter::instance().RemoveNetwork(userId);
		UserLogicAdapter::instance().updateUserNetwork(userId, network);
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::addNetwork] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::addNetwork] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::addNetwork] UNKNOWN exception");
	}
	return index;
}

void NetworkManagerI::setNetwork(Ice::Int userId, Ice::Int networkId, const MyUtil::Str2StrMap& network, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , networkId=" << networkId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[NetColNetworkId] = boost::lexical_cast<string>(networkId);
	filter[NetColUSERID] = boost::lexical_cast<string>(userId);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			QueryRunner("common", CDbWServer).execute(Statement::buildUpdate(TABLE_NETWORK, filter, network));
			UserNetworkReaderAdapter::instance().setNetwork(userId, networkId, network);
		}
		xce::adapter::UserNetworkTripodAdapter::instance().RemoveNetwork(userId);
		UserLogicAdapter::instance().updateUserNetwork(userId, network);
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::setNetwork] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::setNetwork] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::setNetwork] UNKNOWN exception");
	}
}

void NetworkManagerI::removeNetwork(Ice::Int userId, Ice::Int networkId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , networkId=" << networkId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[NetColUSERID] = boost::lexical_cast<string>(userId);
	filter[NetColNetworkId] = boost::lexical_cast<string>(networkId);
	ConnectionQuery conn("common", CDbWServer);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			MCE_INFO("[NetworkManagerI::removeNetwork] lock : id=" << userId << ", networkId=" << networkId
				<< ", Client=" << (current.con ? current.con->toString().substr(current.con->toString().find("remote",25)).substr(17):"localhost:-1" ));
			conn.doDelete(TABLE_NETWORK, filter);
			UserNetworkReaderAdapter::instance().removeNetwork(userId, networkId);
			MCE_INFO("[NetworkManagerI::removeNetwork] unlock : id=" << userId << ", networkId=" << networkId
				<< ", Client=" << (current.con ? current.con->toString().substr(current.con->toString().find("remote",25)).substr(17):"localhost:-1" ));
		}
		xce::adapter::UserNetworkTripodAdapter::instance().RemoveNetwork(userId);
		UserLogicAdapter::instance().updateUserNetwork(userId, filter);
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::removeNetwork] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::removeNetwork] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::removeNetwork] UNKNOWN exception");
	}
}

void NetworkManagerI::removeNetworkByStage(Ice::Int userId, Ice::Int stage, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , stage=" << stage;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[NetColUSERID] = boost::lexical_cast<string>(userId);
	filter[NetColSTAGE] = boost::lexical_cast<string>(stage);
	ConnectionQuery conn("common", CDbWServer);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			conn.doDelete(TABLE_NETWORK, filter);
			UserNetworkReaderAdapter::instance().removeNetworkByStage(userId, stage);
		}
		xce::adapter::UserNetworkTripodAdapter::instance().RemoveNetwork(userId);
		UserLogicAdapter::instance().updateUserNetwork(userId, filter);
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::removeNetwork] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::removeNetworkByStage] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::removeNetworkByStage] UNKNOWN exception");
	}
}

//--------------------------------------------------------------
HighSchoolInfoPtr NetworkManagerI::getHighSchoolInfo(Ice::Int userId, Ice::Int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << ", schoolId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getHighSchoolInfo(userId, id);
}

HighSchoolInfoPtr NetworkManagerI::getHighSchoolInfoBySchoolId(Ice::Int userId, Ice::Int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << ", schoolId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getHighSchoolInfoBySchoolId(userId, id);
}

HighSchoolInfoSeq NetworkManagerI::getHighSchoolInfoSeq(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getHighSchoolInfoSeq(userId);
}

Ice::Int NetworkManagerI::getHighSchoolInfoCount(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getHighSchoolInfoCount(userId);
}

void NetworkManagerI::reloadHighSchoolInfoSeq(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	reloadAll(userId);
}

Ice::Int NetworkManagerI::addHighSchoolInfo(Ice::Int userId, const MyUtil::Str2StrMap& highSchoolInfo, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	int index = 0;
	try {
		{
			Lock lock(_tempMutexManager, userId);
			index = QueryRunner("common", CDbWServer).execute(Statement::buildInsert(TABLE_HIGH_SCHOOL_INFO, highSchoolInfo)).insert_id();
			MyUtil::Str2StrMap& finalHighSchoolInfo = const_cast<MyUtil::Str2StrMap&> (highSchoolInfo);
			finalHighSchoolInfo.insert(make_pair("id", boost::lexical_cast<string>(index)));
			UserNetworkReaderAdapter::instance().addHighSchoolInfo(userId, finalHighSchoolInfo);
		}
		xce::adapter::UserNetworkTripodAdapter::instance().RemoveHighSchool(userId);
		UserLogicAdapter::instance().updateUserHighSchool(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::addHighSchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::addHighSchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::addHighSchoolInfo] UNKNOWN exception");
	}
	return index;
}

void NetworkManagerI::setHighSchoolInfo(Ice::Int userId, Ice::Int id, const MyUtil::Str2StrMap& HighSchoolInfo, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , schoolId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[HighColID] = boost::lexical_cast<string>(id);
	filter[HighColUSERID] = boost::lexical_cast<string>(userId);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			QueryRunner("common", CDbWServer).execute(Statement::buildUpdate(TABLE_HIGH_SCHOOL_INFO, filter, HighSchoolInfo));
			UserNetworkReaderAdapter::instance().setHighSchoolInfo(userId, id, HighSchoolInfo);
		}
		xce::adapter::UserNetworkTripodAdapter::instance().RemoveHighSchool(userId);
		UserLogicAdapter::instance().updateUserHighSchool(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::setHighSchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::setHighSchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::setHighSchoolInfo] UNKNOWN exception");
	}
}

void NetworkManagerI::removeAllHighSchoolInfo(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[HighColUSERID] = boost::lexical_cast<string>(userId);
	ConnectionQuery conn("common", CDbWServer);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			conn.doDelete(TABLE_HIGH_SCHOOL_INFO, filter);
			UserNetworkReaderAdapter::instance().removeAllHighSchoolInfo(userId);
		}
		xce::adapter::UserNetworkTripodAdapter::instance().RemoveHighSchool(userId);
		UserLogicAdapter::instance().updateUserHighSchool(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::removeAllHighSchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::removeAllHighSchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::removeAllHighSchoolInfo] UNKNOWN exception");
	}
}

void NetworkManagerI::removeHighSchoolInfo(Ice::Int userId, Ice::Int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << ", id" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[HighColUSERID] = boost::lexical_cast<string>(userId);
	filter[HighColID] = boost::lexical_cast<string>(id);
	ConnectionQuery conn("common", CDbWServer);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			conn.doDelete(TABLE_HIGH_SCHOOL_INFO, filter);
			UserNetworkReaderAdapter::instance().removeHighSchoolInfo(userId, id);
		}
		xce::adapter::UserNetworkTripodAdapter::instance().RemoveHighSchool(userId);
		UserLogicAdapter::instance().updateUserHighSchool(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::removeHighSchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::removeHighSchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::removeHighSchoolInfo] UNKNOWN exception");
	}
}

//--------------------------------------------------------------
UniversityInfoPtr NetworkManagerI::getUniversityInfo(Ice::Int userId, Ice::Int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , universityId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getUniversityInfo(userId, id);
}

UniversityInfoPtr NetworkManagerI::getUniversityInfoByUnivId(Ice::Int userId, Ice::Int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , universityId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getUniversityInfoByUnivId(userId, id);
}

UniversityInfoSeq NetworkManagerI::getUniversityInfoSeq(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getUniversityInfoSeq(userId);
}

Ice::Int NetworkManagerI::getUniversityInfoCount(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getUniversityInfoCount(userId);
}

void NetworkManagerI::reloadUniversityInfoSeq(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	reloadAll(userId);
}

Ice::Int NetworkManagerI::addUniversityInfo(Ice::Int userId, const MyUtil::Str2StrMap& universityInfo, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	int index = 0;
	try {
		{
			Lock lock(_tempMutexManager, userId);
			index = QueryRunner("common", CDbWServer).execute(Statement::buildInsert("university_info", universityInfo)).insert_id();
			MyUtil::Str2StrMap& finalUniversityInfo = const_cast<MyUtil::Str2StrMap&> (universityInfo);
			finalUniversityInfo.insert(make_pair("id", boost::lexical_cast<string>(index)));
			UserNetworkReaderAdapter::instance().addUniversityInfo(userId, finalUniversityInfo);
		}
		xce::adapter::UserNetworkTripodAdapter::instance().RemoveUniversity(userId);
		UserLogicAdapter::instance().updateUserUniversity(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::addUniversityInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::addUniversityInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::addUniversityInfo] UNKNOWN exception");
	}
	return index;
}

void NetworkManagerI::setUniversityInfo(Ice::Int userId, Ice::Int id, const MyUtil::Str2StrMap& universityInfo, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , universityId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[UniColID] = boost::lexical_cast<string>(id);
	filter[UniColUSERID] = boost::lexical_cast<string>(userId);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			QueryRunner("common", CDbWServer).execute(Statement::buildUpdate(TABLE_UNIVERSITY_INFO, filter, universityInfo));
			UserNetworkReaderAdapter::instance().setUniversityInfo(userId, id, universityInfo);
		}
		xce::adapter::UserNetworkTripodAdapter::instance().RemoveUniversity(userId);
		UserLogicAdapter::instance().updateUserUniversity(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::setUniversityInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::setUniversityInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::setUniversityInfo] UNKNOWN exception");
	}
}

void NetworkManagerI::removeAllUniversityInfo(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[UniColUSERID] = boost::lexical_cast<string>(userId);
	ConnectionQuery conn("common", CDbWServer);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			conn.doDelete(TABLE_UNIVERSITY_INFO, filter);
			UserNetworkReaderAdapter::instance().removeAllUniversityInfo(userId);
		}
		xce::adapter::UserNetworkTripodAdapter::instance().RemoveUniversity(userId);
		UserLogicAdapter::instance().updateUserUniversity(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::removeAllUniversityInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::removeAllUniversityInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::removeAllUniversityInfo] UNKNOWN exception");
	}
}

void NetworkManagerI::removeUniversityInfo(Ice::Int userId, Ice::Int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , universityId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[UniColUSERID] = boost::lexical_cast<string>(userId);
	filter[UniColID] = boost::lexical_cast<string>(id);
	ConnectionQuery conn("common", CDbWServer);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			conn.doDelete(TABLE_UNIVERSITY_INFO, filter);
			UserNetworkReaderAdapter::instance().removeUniversityInfo(userId, id);
		}
		xce::adapter::UserNetworkTripodAdapter::instance().RemoveUniversity(userId);
		UserLogicAdapter::instance().updateUserUniversity(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::removeUniversityInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::removeUniversityInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::removeUniversityInfo] UNKNOWN exception");
	}
}

void NetworkManagerI::removeUniversityInfoByUnivId(Ice::Int userId, Ice::Int universityId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " universityId=" << universityId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[UniColUSERID] = boost::lexical_cast<string>(userId);
	filter[UniColUNIVERSITYID] = boost::lexical_cast<string>(universityId);
	ConnectionQuery conn("common", CDbWServer);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			conn.doDelete(TABLE_UNIVERSITY_INFO, filter);
			UserNetworkReaderAdapter::instance().removeUniversityInfoByUnivId(userId, universityId);
		}
		xce::adapter::UserNetworkTripodAdapter::instance().RemoveUniversity(userId);
		UserLogicAdapter::instance().updateUserUniversity(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::removeUniversityInfoByUnivId] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::removeUniversityInfoByUnivId] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::removeUniversityInfoByUnivId] UNKNOWN exception");
	}
}

//--------------------------------------------------------------
WorkplaceInfoPtr NetworkManagerI::getWorkplaceInfo(Ice::Int userId, Ice::Int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , workplaceId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getWorkplaceInfo(userId, id);
}

WorkplaceInfoPtr NetworkManagerI::getWorkplaceInfoByWorkId(Ice::Int userId, Ice::Int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , workplaceId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getWorkplaceInfoByWorkId(userId, id);
}

WorkplaceInfoSeq NetworkManagerI::getWorkplaceInfoSeq(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getWorkplaceInfoSeq(userId);
}

void NetworkManagerI::reloadWorkplaceInfoSeq(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	reloadAll(userId);
}

Ice::Int NetworkManagerI::addWorkplaceInfo(Ice::Int userId, const MyUtil::Str2StrMap& workplaceInfo, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	int index = addWorkspaceInfo(userId, workplaceInfo);

	return index;
}

void NetworkManagerI::setWorkplaceInfo(Ice::Int userId, Ice::Int id, const MyUtil::Str2StrMap& workplaceInfo, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , workplaceId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	setWorkspaceInfo(userId, id, workplaceInfo);
}

void NetworkManagerI::removeAllWorkplaceInfo(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[WorkColUSERID] = boost::lexical_cast<string>(userId);
	ConnectionQuery conn("common", CDbWServer);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			conn.doDelete(TABLE_WORKPLACE_INFO, filter);
			UserNetworkReaderAdapter::instance().removeAllWorkplaceInfo(userId);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveWorkplace(userId);
		UserLogicAdapter::instance().updateUserWorkplace(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::removeAllWorkplaceInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::removeAllWorkplaceInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::removeAllWorkplaceInfo] UNKNOWN exception");
	}
}

void NetworkManagerI::removeWorkplaceInfo(Ice::Int userId, Ice::Int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , workplaceId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[WorkColUSERID] = boost::lexical_cast<string>(userId);
	filter[WorkColID] = boost::lexical_cast<string>(id);
	ConnectionQuery conn("common", CDbWServer);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			conn.doDelete(TABLE_WORKPLACE_INFO, filter);
			UserNetworkReaderAdapter::instance().removeWorkplaceInfo(userId, id);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveWorkplace(userId);
		UserLogicAdapter::instance().updateUserWorkplace(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::removeWorkplaceInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::removeWorkplaceInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::removeWorkplaceInfo] UNKNOWN exception");
	}
}

void NetworkManagerI::removeWorkplaceInfoByWorkId(Ice::Int userId, Ice::Int workplaceId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , workplaceId=" << workplaceId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);
	removeWorkspaceInfo(userId, workplaceId);
}

Ice::Int NetworkManagerI::getWorkplaceInfoCount(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getWorkplaceInfoCount(userId);
}

//--------------------------------------------------------------
WorkspaceInfoPtr NetworkManagerI::getWorkspaceInfo(Ice::Int userId, Ice::Int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , workplaceId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getWorkspaceInfo(userId, id);
}

WorkspaceInfoPtr NetworkManagerI::getWorkspaceInfoByWorkId(Ice::Int userId, Ice::Int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , workplaceId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getWorkspaceInfoByWorkId(userId, id);
}

WorkspaceInfoSeq NetworkManagerI::getWorkspaceInfoSeq(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getWorkspaceInfoSeq(userId);
}

Ice::Int NetworkManagerI::addWorkspaceInfo(Ice::Int userId, const MyUtil::Str2StrMap& workspaceInfo, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	int index = 0;
	try {
		{
			Lock lock(_tempMutexManager, userId);
			index = QueryRunner("common", CDbWServer).execute(Statement::buildInsert(TABLE_WORKPLACE_INFO, workspaceInfo)).insert_id();
			MyUtil::Str2StrMap& finalWorkspaceInfo = const_cast<MyUtil::Str2StrMap&> (workspaceInfo);
			finalWorkspaceInfo.insert(make_pair("id", boost::lexical_cast<string>(index)));
			UserNetworkReaderAdapter::instance().addWorkspaceInfo(userId, finalWorkspaceInfo);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveWorkplace(userId);
		UserLogicAdapter::instance().updateUserWorkplace(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::addWorkspaceInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::addWorkspaceInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::addWorkspaceInfo] UNKNOWN exception");
	}
	return index;
}

void NetworkManagerI::setWorkspaceInfo(Ice::Int userId, Ice::Int id, const MyUtil::Str2StrMap& workspaceInfo, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , workplaceId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[WorkColID] = boost::lexical_cast<string>(id);
	filter[WorkColUSERID] = boost::lexical_cast<string>(userId);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			QueryRunner("common", CDbWServer).execute(Statement::buildUpdate(TABLE_WORKPLACE_INFO, filter, workspaceInfo));
			UserNetworkReaderAdapter::instance().setWorkspaceInfo(userId, id, workspaceInfo);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveWorkplace(userId);
		UserLogicAdapter::instance().updateUserWorkplace(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::setWorkspaceInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::setWorkspaceInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::setWorkspaceInfo] UNKNOWN exception");
	}
}

void NetworkManagerI::removeWorkspaceInfo(Ice::Int userId, Ice::Int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , workplaceId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[WorkColUSERID] = boost::lexical_cast<string>(userId);
	filter[WorkColWORKPLACEID] = boost::lexical_cast<string>(id);
	ConnectionQuery conn("common", CDbWServer);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			conn.doDelete(TABLE_WORKPLACE_INFO, filter);
			UserNetworkReaderAdapter::instance().removeWorkspaceInfo(userId, id);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveWorkplace(userId);
		UserLogicAdapter::instance().updateUserWorkplace(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::removeWorkspaceInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::removeWorkspaceInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::removeWorkspaceInfo] UNKNOWN exception");
	}
}

//--------------------------------------------------------------
CollegeInfoPtr NetworkManagerI::getCollegeInfo(Ice::Int userId, Ice::Int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , collegeId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getCollegeInfo(userId, id);
}

CollegeInfoPtr NetworkManagerI::getCollegeInfoByCollegeId(Ice::Int userId, Ice::Int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , collegeId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getCollegeInfoByCollegeId(userId, id);
}

CollegeInfoSeq NetworkManagerI::getCollegeInfoSeq(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getCollegeInfoSeq(userId);
}

Ice::Int NetworkManagerI::getCollegeInfoCount(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getCollegeInfoCount(userId);
}

void NetworkManagerI::reloadCollegeInfoSeq(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	reloadAll(userId);
}

Ice::Int NetworkManagerI::addCollegeInfo(Ice::Int userId, const MyUtil::Str2StrMap& info, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	int index = 0;
	try {
		{
			Lock lock(_tempMutexManager, userId);
			index = QueryRunner("common", CDbWServer).execute(Statement::buildInsert(TABLE_COLLEGE_INFO, info)).insert_id();
			MyUtil::Str2StrMap& finalInfo = const_cast<MyUtil::Str2StrMap&> (info);
			finalInfo.insert(make_pair("id", boost::lexical_cast<string>(index)));
			UserNetworkReaderAdapter::instance().addCollegeInfo(userId, finalInfo);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveCollege(userId);
		UserLogicAdapter::instance().updateUserCollege(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::addCollegeInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::addCollegeInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::addCollegeInfo] UNKNOWN exception");
	}
	return index;
}

void NetworkManagerI::setCollegeInfo(Ice::Int userId, Ice::Int id, const MyUtil::Str2StrMap& info, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , collegeId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[UniColID] = boost::lexical_cast<string>(id);
	filter[UniColUSERID] = boost::lexical_cast<string>(userId);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			QueryRunner("common", CDbWServer).execute(Statement::buildUpdate(TABLE_COLLEGE_INFO, filter, info));
			UserNetworkReaderAdapter::instance().setCollegeInfo(userId, id, info);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveCollege(userId);
		UserLogicAdapter::instance().updateUserCollege(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::setCollegeInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::setCollegeInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::setCollegeInfo] UNKNOWN exception");
	}
}

void NetworkManagerI::removeAllCollegeInfo(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[UniColUSERID] = boost::lexical_cast<string>(userId);
	ConnectionQuery conn("common", CDbWServer);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			conn.doDelete(TABLE_COLLEGE_INFO, filter);
			UserNetworkReaderAdapter::instance().removeAllCollegeInfo(userId);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveCollege(userId);
		UserLogicAdapter::instance().updateUserCollege(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::removeAllCollegeInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::removeAllCollegeInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::removeAllCollegeInfo] UNKNOWN exception");
	}
}

void NetworkManagerI::removeCollegeInfo(Ice::Int userId, Ice::Int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , collegeId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[UniColUSERID] = boost::lexical_cast<string>(userId);
	filter[UniColID] = boost::lexical_cast<string>(id);
	ConnectionQuery conn("common", CDbWServer);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			conn.doDelete(TABLE_COLLEGE_INFO, filter);
			UserNetworkReaderAdapter::instance().removeCollegeInfo(userId, id);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveCollege(userId);
		UserLogicAdapter::instance().updateUserCollege(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::removeCollegeInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::removeCollegeInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::removeCollegeInfo] UNKNOWN exception");
	}
}

//--------------------------------------------------------------
ElementarySchoolInfoPtr NetworkManagerI::getElementarySchoolInfoById(Ice::Int userId, int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "userId=" << userId << ", schoolId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getElementarySchoolInfoById(userId, id);
}

ElementarySchoolInfoPtr NetworkManagerI::getElementarySchoolInfoBySchoolId(Ice::Int userId, int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "userId=" << userId << " schoolId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getElementarySchoolInfoBySchoolId(userId, id);
}

ElementarySchoolInfoSeq NetworkManagerI::getElementarySchoolInfoSeq(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "userId=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getElementarySchoolInfoSeq(userId);
}

ElementarySchoolInfoPtr NetworkManagerI::getElementarySchoolInfo(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getElementarySchoolInfo(userId);
}

void NetworkManagerI::reloadElementarySchoolInfo(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	reloadAll(userId);
}

void NetworkManagerI::removeAllElementarySchoolInfo(int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "userId=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);
	Str2StrMap filter;
	filter[ElemColUSERID] = boost::lexical_cast<string>(userId);
	ConnectionQuery conn("common", CDbWServer);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			conn.doDelete(TABLE_ELEMENTARY_SCHOOL_INFO, filter);
			UserNetworkReaderAdapter::instance().removeAllElementarySchoolInfo(userId);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveElementarySchool(userId);
		UserLogicAdapter::instance().updateUserElementarySchool(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::removeAllElementarySchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::removeAllElementarySchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::removeAllElementarySchoolInfo] UNKNOWN exception");
	}
}

void NetworkManagerI::removeElementarySchoolInfoById(int userId, int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "userId=" << userId << ", schoolId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);
	Str2StrMap filter;
	filter[ElemColUSERID] = boost::lexical_cast<string>(userId);
	filter[ElemColID] = boost::lexical_cast<string>(id);
	ConnectionQuery conn("common", CDbWServer);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			conn.doDelete(TABLE_ELEMENTARY_SCHOOL_INFO, filter);
			UserNetworkReaderAdapter::instance().removeElementarySchoolInfoById(userId, id);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveElementarySchool(userId);
		UserLogicAdapter::instance().updateUserElementarySchool(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::removeElementarySchoolInfoById] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::removeElementarySchoolInfoById] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::removeElementarySchoolInfoById] UNKNOWN exception");
	}
}

void NetworkManagerI::setElementarySchoolInfo(int userId, int id, const MyUtil::Str2StrMap& info, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "userId=" << userId << ", schoolId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);
	Str2StrMap filter;
	filter[ElemColID] = boost::lexical_cast<string>(id);
	filter[ElemColUSERID] = boost::lexical_cast<string>(userId);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			QueryRunner("common", CDbWServer).execute(Statement::buildUpdate(TABLE_ELEMENTARY_SCHOOL_INFO, filter, info));
			UserNetworkReaderAdapter::instance().setElementarySchoolInfo(userId, id, info);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveElementarySchool(userId);
		UserLogicAdapter::instance().updateUserElementarySchool(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::setElementarySchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::setElementarySchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::setElementarySchoolInfo] UNKNOWN exception");
	}
}

Ice::Int NetworkManagerI::addElementarySchoolInfo(Ice::Int userId, const MyUtil::Str2StrMap& info, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	int index = 0;
	try {
		{
			Lock lock(_tempMutexManager, userId);
			index = QueryRunner("common", CDbWServer).execute(Statement::buildInsert(TABLE_ELEMENTARY_SCHOOL_INFO, info)).insert_id();
			MyUtil::Str2StrMap& finalInfo = const_cast<MyUtil::Str2StrMap&> (info);
			finalInfo.insert(make_pair("id", boost::lexical_cast<string>(index)));
			UserNetworkReaderAdapter::instance().addElementarySchoolInfo(userId, finalInfo);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveElementarySchool(userId);
		UserLogicAdapter::instance().updateUserElementarySchool(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::addElementarySchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::addElementarySchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::addElementarySchoolInfo] UNKNOWN exception");
	}
	return index;
}

void NetworkManagerI::removeElementarySchoolInfo(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[ElemColUSERID] = boost::lexical_cast<string>(userId);
	ConnectionQuery conn("common", CDbWServer);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			conn.doDelete(TABLE_ELEMENTARY_SCHOOL_INFO, filter);
			UserNetworkReaderAdapter::instance().removeElementarySchoolInfo(userId);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveElementarySchool(userId);
		UserLogicAdapter::instance().updateUserElementarySchool(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::removeElementarySchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::removeElementarySchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::removeElementarySchoolInfo] UNKNOWN exception");
	}
}

//--------------------------------------------------------------
JuniorHighSchoolInfoPtr NetworkManagerI::getJuniorHighSchoolInfoById(int userId, int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "userId=" << userId << ", schoolId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getJuniorHighSchoolInfoById(userId, id);
}

JuniorHighSchoolInfoPtr NetworkManagerI::getJuniorHighSchoolInfoBySchoolId(int userId, int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "userId=" << userId << " schoolid=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getJuniorHighSchoolInfoBySchoolId(userId, id);
}

JuniorHighSchoolInfoSeq NetworkManagerI::getJuniorHighSchoolInfoSeq(int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "userId=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getJuniorHighSchoolInfoSeq(userId);
}

JuniorHighSchoolInfoPtr NetworkManagerI::getJuniorHighSchoolInfo(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getJuniorHighSchoolInfo(userId);
}

void NetworkManagerI::removeAllJuniorHighSchoolInfo(int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "userId=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);
	Str2StrMap filter;
	filter[JunHighColUSERID] = boost::lexical_cast<string>(userId);
	ConnectionQuery conn("common", CDbWServer);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			conn.doDelete(TABLE_JUNIOR_HIGH_SCHOOL_INFO, filter);
			UserNetworkReaderAdapter::instance().removeAllJuniorHighSchoolInfo(userId);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveJuniorHighSchool(userId);
		UserLogicAdapter::instance().updateUserJuniorHighSchool(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::removeAllJuniorHighSchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::removeAllJuniorHighSchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::removeAllJuniorHighSchoolInfo] UNKNOWN exception");
	}
}

void NetworkManagerI::removeJuniorHighSchoolInfoById(int userId, int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "userId=" << userId << ", schoolId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);
	Str2StrMap filter;
	filter[JunHighColUSERID] = boost::lexical_cast<string>(userId);
	filter[JunHighColID] = boost::lexical_cast<string>(id);
	ConnectionQuery conn("common", CDbWServer);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			conn.doDelete(TABLE_JUNIOR_HIGH_SCHOOL_INFO, filter);
			UserNetworkReaderAdapter::instance().removeJuniorHighSchoolInfoById(userId, id);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveJuniorHighSchool(userId);
		UserLogicAdapter::instance().updateUserJuniorHighSchool(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::removeJuniorHighSchoolInfoById] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::removeJuniorHighSchoolInfoById] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::removeJuniorHighSchoolInfoById] UNKNOWN exception");
	}
}

void NetworkManagerI::setJuniorHighSchoolInfo(int userId, int id, const MyUtil::Str2StrMap& data, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "userId=" << userId << ", schoolId=" << id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);
	Str2StrMap filter;
	filter[JunHighColID] = boost::lexical_cast<string>(id);
	filter[JunHighColUSERID] = boost::lexical_cast<string>(userId);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			QueryRunner("common", CDbWServer).execute(Statement::buildUpdate(TABLE_JUNIOR_HIGH_SCHOOL_INFO, filter, data));
			UserNetworkReaderAdapter::instance().setJuniorHighSchoolInfo(userId, id, data);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveJuniorHighSchool(userId);
		UserLogicAdapter::instance().updateUserJuniorHighSchool(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::setJuniorHighSchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::setJuniorHighSchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::setJuniorHighSchoolInfo] UNKNOWN exception");
	}
}

void NetworkManagerI::reloadJuniorHighSchoolInfo(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	reloadAll(userId);
}

Ice::Int NetworkManagerI::addJuniorHighSchoolInfo(Ice::Int userId, const MyUtil::Str2StrMap& info, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	int index = 0;
	try {
		{
			Lock lock(_tempMutexManager, userId);
			index = QueryRunner("common", CDbWServer).execute(Statement::buildInsert(TABLE_JUNIOR_HIGH_SCHOOL_INFO, info)).insert_id();
			MyUtil::Str2StrMap& finalJuniorHighSchool = const_cast<MyUtil::Str2StrMap&> (info);
			finalJuniorHighSchool.insert(make_pair("id", boost::lexical_cast<string>(index)));
			UserNetworkReaderAdapter::instance().addJuniorHighSchoolInfo(userId, finalJuniorHighSchool);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveJuniorHighSchool(userId);
		UserLogicAdapter::instance().updateUserJuniorHighSchool(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::addJuniorHighSchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::addJuniorHighSchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::addJuniorHighSchoolInfo] UNKNOWN exception");
	}
	return index;
}

void NetworkManagerI::removeJuniorHighSchoolInfo(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[ElemColUSERID] = boost::lexical_cast<string>(userId);
	ConnectionQuery conn("common", CDbWServer);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			conn.doDelete(TABLE_JUNIOR_HIGH_SCHOOL_INFO, filter);
			UserNetworkReaderAdapter::instance().removeJuniorHighSchoolInfo(userId);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveJuniorHighSchool(userId);
		UserLogicAdapter::instance().updateUserJuniorHighSchool(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::removeJuniorHighSchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::removeJuniorHighSchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::removeJuniorHighSchoolInfo] UNKNOWN exception");
	}
}

//--------------------------------------------------------------
RegionInfoPtr NetworkManagerI::getRegionInfo(Ice::Int userId, Ice::Int regionId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , regionId=" << regionId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getRegionInfo(userId, regionId);
}

RegionInfoSeq NetworkManagerI::getRegionInfoSeq(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getRegionInfoSeq(userId);
}

void NetworkManagerI::reloadRegionInfo(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	reloadAll(userId);
}

Ice::Int NetworkManagerI::addRegionInfo(Ice::Int userId, const MyUtil::Str2StrMap& regionInfo, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	int index = 0;
	try {
		{
			Lock lock(_tempMutexManager, userId);
			index = QueryRunner("common", CDbWServer).execute(Statement::buildReplace(TABLE_REGION_INFO, regionInfo)).insert_id();
			MyUtil::Str2StrMap& finalRegion = const_cast<MyUtil::Str2StrMap&> (regionInfo);
			finalRegion.insert(make_pair("id", boost::lexical_cast<string>(index)));
			UserNetworkReaderAdapter::instance().addRegionInfo(userId, finalRegion);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveRegion(userId);
		UserLogicAdapter::instance().updateUserRegion(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::addRegionInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::addRegionInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::addRegionInfo] UNKNOWN exception");
	}
	return index;
}

void NetworkManagerI::removeRegionInfo(Ice::Int userId, Ice::Int regionId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , regionId=" << regionId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[RegionColUSERID] = boost::lexical_cast<string>(userId);
	filter[RegionColREGIONID] = boost::lexical_cast<string>(regionId);
	ConnectionQuery conn("common", CDbWServer);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			conn.doDelete(TABLE_REGION_INFO, filter);
			UserNetworkReaderAdapter::instance().removeRegionInfo(userId, regionId);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveRegion(userId);
		UserLogicAdapter::instance().updateUserRegion(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::removeRegionInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::removeRegionInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::removeRegionInfo] UNKNOWN exception");
	}
}

void NetworkManagerI::removeAllRegionInfo(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[RegionColUSERID] = boost::lexical_cast<string>(userId);
	ConnectionQuery conn("common", CDbWServer);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			conn.doDelete(TABLE_REGION_INFO, filter);
			UserNetworkReaderAdapter::instance().removeAllRegionInfo(userId);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveRegion(userId);
		UserLogicAdapter::instance().updateUserRegion(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::removeAllRegionInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::removeAllRegionInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::removeAllRegionInfo] UNKNOWN exception");
	}
}

void NetworkManagerI::setRegionInfo(Ice::Int userId, Ice::Int regionId, const MyUtil::Str2StrMap& regionInfo, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId << " , regionId=" << regionId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	Str2StrMap filter;
	filter[RegionColREGIONID] = boost::lexical_cast<string>(regionId);
	filter[RegionColUSERID] = boost::lexical_cast<string>(userId);

	try {
		{
			Lock lock(_tempMutexManager, userId);
			QueryRunner("common", CDbWServer).execute(Statement::buildUpdate(TABLE_REGION_INFO, filter, regionInfo));
			UserNetworkReaderAdapter::instance().setRegionInfo(userId, regionId, regionInfo);
		}
	        xce::adapter::UserNetworkTripodAdapter::instance().RemoveRegion(userId);
		UserLogicAdapter::instance().updateUserRegion(userId, Str2StrMap());
	} catch (Ice::Exception& e) {
		MCE_WARN("[NetworkManagerI::setRegionInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[NetworkManagerI::setRegionInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[NetworkManagerI::setRegionInfo] UNKNOWN exception");
	}
}

//--------------------------------------------------------------
NetworkBigObjectPtr NetworkManagerI::getBigObject(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getBigObject(userId);
}

NetworkBigObjectNPtr NetworkManagerI::getBigObjectN(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getBigObjectN(userId);
}

NetworkDataPtr NetworkManagerI::getNetworkFullData(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return UserNetworkReaderAdapter::instance().getNetworkFullData(userId);
}

//--------------------------------------------------------------
bool NetworkManagerI::isValid() {
	IceUtil::Mutex::Lock lock(_mutex);
	return _isValid;
}

void NetworkManagerI::setValid(bool valid, const ::Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(valid), InvokeClient::INVOKE_LEVEL_INFO);
	{
		IceUtil::Mutex::Lock lock(_mutex);
		_isValid = valid;
	}
	ServerStateSubscriber::instance().setStatus((int) valid);
}

//仅供NetworkPreloader使用，由于服务要一份一份下，Preloader不能先下掉，所以暂时保留
//在Preloader删掉后可以删除此接口
void NetworkManagerI::setData(const ::MyUtil::ObjectResultPtr& userData, const ::Ice::Current& current) {
}

void NetworkManagerI::reloadAll(Ice::Int userId, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg << "id=" << userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	{
		Lock lock(_tempMutexManager, userId);
		NetworkCacheDataPtr data = NetworkCacheDataPtr::dynamicCast(_factory->create(userId));
		if (data) {
			UserNetworkReaderAdapter::instance().setData(userId, data->getData());
		}
	}
}
