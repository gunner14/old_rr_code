#include "UserNetworkOldAdapter.h"
#include <iostream>

using namespace mop::hi::svc::adapter;
using namespace std;

//以下get方法兼容NetworkManager提供的接口
//--------------------------------------------------------------
NetworkManagerPrx UserNetworkOldAdapter::getNetworkManager(int id) {
	return getProxy(id);
}

NetworkManagerPrx UserNetworkOldAdapter::getNetworkManagerOneway(int id) {
	return getProxyOneway(id);
}

NetworkBigObjectNPtr UserNetworkOldAdapter::getBigObjectN(int userId) {
	try {
		return getNetworkManager(userId)->getBigObjectN(userId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::getBigObjectN] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::getBigObjectN] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::getBigObjectN] UNKNOWN exception");
	}
	return NULL;
}

HighSchoolInfoSeq UserNetworkOldAdapter::getHighSchoolInfoSeq(int userId) {
	try {
		return getNetworkManager(userId)->getHighSchoolInfoSeq(userId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::getHighSchoolInfoSeq] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::getHighSchoolInfoSeq] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::getHighSchoolInfoSeq] UNKNOWN exception");
	}
	return HighSchoolInfoSeq();
}

//以下get方法为checker提供
//--------------------------------------------------------------
NetworkDataPtr UserNetworkOldAdapter::getNetworkFullData(int userId){
	NetworkManagerPrx prx = getNetworkManager(userId);
	try {
		if (prx) {
			return prx->getNetworkFullData(userId);
		}
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::getNetworkFullData] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::getNetworkFullData] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::getNetworkFullData] UNKNOWN exception");
	}
	return NULL;
}

//以下reload/add/remove/set方法供UserNetWorkWriter(agent)调用
//--------------------------------------------------------------
void UserNetworkOldAdapter::reloadNetworkSeq(Ice::Int userId) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->reloadNetworkSeq(userId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::reloadNetworkSeq] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::reloadNetworkSeq] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::reloadNetworkSeq] UNKNOWN exception");
	}
}

Ice::Int UserNetworkOldAdapter::addNetwork(Ice::Int userId, const MyUtil::Str2StrMap& network) {
	if (userId <= 0) {
		return 0;
	}
	try {
		return getNetworkManager(userId)->addNetwork(userId, network);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::addNetwork] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::addNetwork] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::addNetwork] UNKNOWN exception");
	}
	return 0;
}

void UserNetworkOldAdapter::setNetwork(Ice::Int userId, Ice::Int networkId, const MyUtil::Str2StrMap& network) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->setNetwork(userId, networkId, network);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::setNetwork] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::setNetwork] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::setNetwork] UNKNOWN exception");
	}
}

void UserNetworkOldAdapter::removeNetwork(Ice::Int userId, Ice::Int networkId) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->removeNetwork(userId, networkId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeNetwork] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeNetwork] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::removeNetwork] UNKNOWN exception");
	}
}

void UserNetworkOldAdapter::removeNetworkByStage(Ice::Int userId, Ice::Int stage) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->removeNetworkByStage(userId, stage);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeNetworkByStage] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeNetworkByStage] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::removeNetworkByStage] UNKNOWN exception");
	}
}

//--------------------------------------------------------------
void UserNetworkOldAdapter::reloadHighSchoolInfoSeq(Ice::Int userId) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->reloadHighSchoolInfoSeq(userId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::reloadHighSchoolInfoSeq] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::reloadHighSchoolInfoSeq] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::reloadHighSchoolInfoSeq] UNKNOWN exception");
	}
}

Ice::Int UserNetworkOldAdapter::addHighSchoolInfo(Ice::Int userId, const MyUtil::Str2StrMap& highSchoolInfo) {
	if (userId <= 0) {
		return 0;
	}
	try {
		return getNetworkManager(userId)->addHighSchoolInfo(userId, highSchoolInfo);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::addHighSchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::addHighSchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::addHighSchoolInfo] UNKNOWN exception");
	}
	return 0;
}

void UserNetworkOldAdapter::setHighSchoolInfo(Ice::Int userId, Ice::Int id, const MyUtil::Str2StrMap& HighSchoolInfo) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->setHighSchoolInfo(userId, id, HighSchoolInfo);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::setHighSchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::setHighSchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::setHighSchoolInfo] UNKNOWN exception");
	}
}

void UserNetworkOldAdapter::removeAllHighSchoolInfo(Ice::Int userId) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->removeAllHighSchoolInfo(userId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeAllHighSchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeAllHighSchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::removeAllHighSchoolInfo] UNKNOWN exception");
	}
}

void UserNetworkOldAdapter::removeHighSchoolInfo(Ice::Int userId, Ice::Int id) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->removeHighSchoolInfo(userId, id);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeHighSchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeHighSchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::removeHighSchoolInfo] UNKNOWN exception");
	}
}

//--------------------------------------------------------------
void UserNetworkOldAdapter::reloadUniversityInfoSeq(Ice::Int userId) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->reloadUniversityInfoSeq(userId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::reloadUniversityInfoSeq] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::reloadUniversityInfoSeq] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::reloadUniversityInfoSeq] UNKNOWN exception");
	}
}

Ice::Int UserNetworkOldAdapter::addUniversityInfo(Ice::Int userId, const MyUtil::Str2StrMap& universityInfo) {
	if (userId <= 0) {
		return 0;
	}
	try {
		return getNetworkManager(userId)->addUniversityInfo(userId, universityInfo);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::addUniversityInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::addUniversityInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::addUniversityInfo] UNKNOWN exception");
	}
	return 0;
}

void UserNetworkOldAdapter::setUniversityInfo(Ice::Int userId, Ice::Int id, const MyUtil::Str2StrMap& universityInfo) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->setUniversityInfo(userId, id, universityInfo);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::setUniversityInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::setUniversityInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::setUniversityInfo] UNKNOWN exception");
	}
}

void UserNetworkOldAdapter::removeAllUniversityInfo(Ice::Int userId) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->removeAllUniversityInfo(userId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeAllUniversityInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeAllUniversityInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::removeAllUniversityInfo] UNKNOWN exception");
	}
}

void UserNetworkOldAdapter::removeUniversityInfo(Ice::Int userId, Ice::Int id) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->removeUniversityInfo(userId, id);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeUniversityInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeUniversityInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::removeUniversityInfo] UNKNOWN exception");
	}
}

void UserNetworkOldAdapter::removeUniversityInfoByUnivId(Ice::Int userId, Ice::Int universityId) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->removeUniversityInfoByUnivId(userId, universityId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeUniversityInfoByUnivId] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeUniversityInfoByUnivId] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::removeUniversityInfoByUnivId] UNKNOWN exception");
	}
}

//--------------------------------------------------------------
void UserNetworkOldAdapter::reloadWorkplaceInfoSeq(Ice::Int userId) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->reloadWorkplaceInfoSeq(userId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::reloadWorkplaceInfoSeq] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::reloadWorkplaceInfoSeq] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::reloadWorkplaceInfoSeq] UNKNOWN exception");
	}
}

Ice::Int UserNetworkOldAdapter::addWorkplaceInfo(Ice::Int userId, const MyUtil::Str2StrMap& workplaceInfo) {
	if (userId <= 0) {
		return 0;
	}
	try {
		return getNetworkManager(userId)->addWorkplaceInfo(userId, workplaceInfo);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::addWorkplaceInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::addWorkplaceInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::addWorkplaceInfo] UNKNOWN exception");
	}
	return 0;
}

void UserNetworkOldAdapter::setWorkplaceInfo(Ice::Int userId, Ice::Int id, const MyUtil::Str2StrMap& workplaceInfo) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->setWorkplaceInfo(userId, id, workplaceInfo);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::setWorkplaceInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::setWorkplaceInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::setWorkplaceInfo] UNKNOWN exception");
	}
}

void UserNetworkOldAdapter::removeAllWorkplaceInfo(Ice::Int userId) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->removeAllWorkplaceInfo(userId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeAllWorkplaceInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeAllWorkplaceInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::removeAllWorkplaceInfo] UNKNOWN exception");
	}
}

void UserNetworkOldAdapter::removeWorkplaceInfo(Ice::Int userId, Ice::Int id) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->removeWorkplaceInfo(userId, id);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeWorkplaceInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeWorkplaceInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::removeWorkplaceInfo] UNKNOWN exception");
	}
}

void UserNetworkOldAdapter::removeWorkplaceInfoByWorkId(Ice::Int userId, Ice::Int workplaceId) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->removeWorkplaceInfoByWorkId(userId, workplaceId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeWorkplaceInfoByWorkId] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeWorkplaceInfoByWorkId] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::removeWorkplaceInfoByWorkId] UNKNOWN exception");
	}
}

//--------------------------------------------------------------
Ice::Int UserNetworkOldAdapter::addWorkspaceInfo(Ice::Int userId, const MyUtil::Str2StrMap& workspaceInfo) {
	if (userId <= 0) {
		return 0;
	}
	try {
		return getNetworkManager(userId)->addWorkspaceInfo(userId, workspaceInfo);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::addWorkspaceInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::addWorkspaceInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::addWorkspaceInfo] UNKNOWN exception");
	}
	return 0;
}

void UserNetworkOldAdapter::setWorkspaceInfo(Ice::Int userId, Ice::Int id, const MyUtil::Str2StrMap& workspaceInfo) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->setWorkspaceInfo(userId, id, workspaceInfo);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::setWorkspaceInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::setWorkspaceInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::setWorkspaceInfo] UNKNOWN exception");
	}
}

void UserNetworkOldAdapter::removeWorkspaceInfo(Ice::Int userId, Ice::Int id) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->removeWorkspaceInfo(userId, id);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeWorkspaceInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeWorkspaceInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::removeWorkspaceInfo] UNKNOWN exception");
	}
}

//--------------------------------------------------------------
void UserNetworkOldAdapter::reloadCollegeInfoSeq(Ice::Int userId) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->reloadCollegeInfoSeq(userId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::reloadCollegeInfoSeq] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::reloadCollegeInfoSeq] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::reloadCollegeInfoSeq] UNKNOWN exception");
	}
}

Ice::Int UserNetworkOldAdapter::addCollegeInfo(Ice::Int userId, const MyUtil::Str2StrMap& info) {
	if (userId <= 0) {
		return 0;
	}
	try {
		return getNetworkManager(userId)->addCollegeInfo(userId, info);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::addCollegeInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::addCollegeInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::addCollegeInfo] UNKNOWN exception");
	}
	return 0;
}

void UserNetworkOldAdapter::setCollegeInfo(Ice::Int userId, Ice::Int id, const MyUtil::Str2StrMap& info) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->setCollegeInfo(userId, id, info);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::setCollegeInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::setCollegeInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::setCollegeInfo] UNKNOWN exception");
	}
}

void UserNetworkOldAdapter::removeAllCollegeInfo(Ice::Int userId) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->removeAllCollegeInfo(userId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeAllCollegeInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeAllCollegeInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::removeAllCollegeInfo] UNKNOWN exception");
	}
}

void UserNetworkOldAdapter::removeCollegeInfo(Ice::Int userId, Ice::Int id) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->removeCollegeInfo(userId, id);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeCollegeInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeCollegeInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::removeCollegeInfo] UNKNOWN exception");
	}
}

//--------------------------------------------------------------
void UserNetworkOldAdapter::reloadElementarySchoolInfo(Ice::Int userId) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->reloadElementarySchoolInfo(userId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::reloadElementarySchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::reloadElementarySchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::reloadElementarySchoolInfo] UNKNOWN exception");
	}
}

void UserNetworkOldAdapter::removeAllElementarySchoolInfo(int userId) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->removeAllElementarySchoolInfo(userId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeAllElementarySchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeAllElementarySchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::removeAllElementarySchoolInfo] UNKNOWN exception");
	}
}

void UserNetworkOldAdapter::removeElementarySchoolInfoById(int userId, int id) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->removeElementarySchoolInfoById(userId, id);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeElementarySchoolInfoById] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeElementarySchoolInfoById] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::removeElementarySchoolInfoById] UNKNOWN exception");
	}
}

void UserNetworkOldAdapter::setElementarySchoolInfo(int userId, int id, const MyUtil::Str2StrMap& info) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->setElementarySchoolInfo(userId, id, info);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::setElementarySchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::setElementarySchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::setElementarySchoolInfo] UNKNOWN exception");
	}
}

Ice::Int UserNetworkOldAdapter::addElementarySchoolInfo(Ice::Int userId, const MyUtil::Str2StrMap& info) {
	if (userId <= 0) {
		return 0;
	}
	try {
		return getNetworkManager(userId)->addElementarySchoolInfo(userId, info);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::addElementarySchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::addElementarySchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::addElementarySchoolInfo] UNKNOWN exception");
	}
	return 0;
}

void UserNetworkOldAdapter::removeElementarySchoolInfo(Ice::Int userId) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->removeElementarySchoolInfo(userId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeElementarySchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeElementarySchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::removeElementarySchoolInfo] UNKNOWN exception");
	}
}

//--------------------------------------------------------------
void UserNetworkOldAdapter::reloadJuniorHighSchoolInfo(Ice::Int userId) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->reloadJuniorHighSchoolInfo(userId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::reloadJuniorHighSchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::reloadJuniorHighSchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::reloadJuniorHighSchoolInfo] UNKNOWN exception");
	}
}

void UserNetworkOldAdapter::removeAllJuniorHighSchoolInfo(int userId) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->removeAllJuniorHighSchoolInfo(userId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeAllJuniorHighSchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeAllJuniorHighSchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::removeAllJuniorHighSchoolInfo] UNKNOWN exception");
	}
}

void UserNetworkOldAdapter::removeJuniorHighSchoolInfoById(int userId, int id) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->removeJuniorHighSchoolInfoById(userId, id);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeJuniorHighSchoolInfoById] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeJuniorHighSchoolInfoById] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::removeJuniorHighSchoolInfoById] UNKNOWN exception");
	}
}

void UserNetworkOldAdapter::setJuniorHighSchoolInfo(int userId, int id, const MyUtil::Str2StrMap& data) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->setJuniorHighSchoolInfo(userId, id, data);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::setJuniorHighSchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::setJuniorHighSchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::setJuniorHighSchoolInfo] UNKNOWN exception");
	}
}

Ice::Int UserNetworkOldAdapter::addJuniorHighSchoolInfo(Ice::Int userId, const MyUtil::Str2StrMap& info) {
	if (userId <= 0) {
		return 0;
	}
	try {
		return getNetworkManager(userId)->addJuniorHighSchoolInfo(userId, info);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::addJuniorHighSchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::addJuniorHighSchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::addJuniorHighSchoolInfo] UNKNOWN exception");
	}
	return 0;
}

void UserNetworkOldAdapter::removeJuniorHighSchoolInfo(Ice::Int userId) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->removeJuniorHighSchoolInfo(userId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeJuniorHighSchoolInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeJuniorHighSchoolInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::removeJuniorHighSchoolInfo] UNKNOWN exception");
	}
}

//--------------------------------------------------------------
void UserNetworkOldAdapter::reloadRegionInfo(Ice::Int userId) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->reloadRegionInfo(userId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::reloadRegionInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::reloadRegionInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::reloadRegionInfo] UNKNOWN exception");
	}
}

Ice::Int UserNetworkOldAdapter::addRegionInfo(Ice::Int userId, const MyUtil::Str2StrMap& regionInfo) {
	if (userId <= 0) {
		return 0;
	}
	try {
		return getNetworkManager(userId)->addRegionInfo(userId, regionInfo);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::addRegionInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::addRegionInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::addRegionInfo] UNKNOWN exception");
	}
	return 0;
}

void UserNetworkOldAdapter::removeRegionInfo(Ice::Int userId, Ice::Int regionId) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->removeRegionInfo(userId, regionId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeRegionInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeRegionInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::removeRegionInfo] UNKNOWN exception");
	}
}

void UserNetworkOldAdapter::removeAllRegionInfo(Ice::Int userId) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->removeAllRegionInfo(userId);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeAllRegionInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::removeAllRegionInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::removeAllRegionInfo] UNKNOWN exception");
	}
}

void UserNetworkOldAdapter::setRegionInfo(Ice::Int userId, Ice::Int regionId, const MyUtil::Str2StrMap& regionInfo) {
	if (userId <= 0) {
		return;
	}
	try {
		getNetworkManager(userId)->setRegionInfo(userId, regionId, regionInfo);
	} catch (Ice::Exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::setRegionInfo] Ice::Exception : " << e.what());
	} catch (std::exception& e) {
		MCE_WARN("[UserNetworkOldAdapter::setRegionInfo] std::exception : " << e.what());
	} catch (...) {
		MCE_WARN("[UserNetworkOldAdapter::setRegionInfo] UNKNOWN exception");
	}
}

