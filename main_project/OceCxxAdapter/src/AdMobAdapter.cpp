#include <boost/lexical_cast.hpp>
#include "AdMobAdapter.h"

using namespace xce::ad;

AdMobPrx AdMobAdapter::getManager(int id) {
  return getProxy(id);
}

AdMobPrx AdMobAdapter::getManagerOneway(int id) {
  return getProxyOneway(id);
}

AdMobResPara AdMobAdapter::GetAds(const xce::ad::AdMobReqPara& para) {
  getManagerOneway(GetServerIndex(para.uid))->GetAds(para);
}

AdMobResPara AdMobAdapter::GetAdsForTest(const xce::ad::AdMobReqPara& para) {
  getManagerOneway(GetServerIndex(para.uid))->GetAdsForTest(para);
}

void AdMobAdapter::PvFeedBack(const xce::ad::AdMobFeedback& para) {
  getManagerOneway(GetServerIndex(para.uid))->PvFeedBack(para);
}

int AdMobAdapter::GetServerIndex(int uid) {
  int index_uid = uid;
  if (uid < 10) {
    index_uid = rand() % 100000;
  }
  return (index_uid / 10) % getCluster();
}
