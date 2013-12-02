#include "UserNetworkAdapter.h"
#include <iostream>

using namespace mop::hi::svc::adapter;
using namespace std;

//以下get方法兼容NetworkManager提供的接口
//--------------------------------------------------------------

NetworkBigObjectNPtr UserNetworkAdapter::getBigObjectN(int userId) {
  return UserNetworkReaderAdapter::instance().getBigObjectN(userId);
}

HighSchoolInfoSeq UserNetworkAdapter::getHighSchoolInfoSeq(int userId) {
  return UserNetworkReaderAdapter::instance().getHighSchoolInfoSeq(userId);
}

NetworkManagerPrx UserNetworkAdapter::getNetworkManager(int id) {
  return NULL;
}

NetworkManagerPrx UserNetworkAdapter::getNetworkManagerOneway(int id) {
  return NULL;
}

//以下get方法为checker提供
//--------------------------------------------------------------
NetworkDataPtr UserNetworkAdapter::getNetworkFullData(int userId){
  return UserNetworkReaderAdapter::instance().getNetworkFullData(userId);
}

//以下get方法为人人桌面提供
//--------------------------------------------------------------
NetworkSeq UserNetworkAdapter::getNetworkSeq(Ice::Int userId) {
  return UserNetworkReaderAdapter::instance().getNetworkSeq(userId);
}
