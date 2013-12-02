/*************************************************************************
    > File Name: OfferPage.cpp
    > Author: zunwen.you
    > Mail: zunwen.you@renren-inc.com 
    > Created Time: 2012年09月28日 星期五 14时02分51秒
 ************************************************************************/

#include "OfferPage.h"
#include "UserUniversityTripodAdapter.h"
#include "UserCollegeTripodAdapter.h"
#include "PageUtil.h"

using namespace std;
using namespace MyUtil;
using namespace xce::adapter;
using namespace xce::offerfriends;
using namespace mop::hi::svc::model;

IntSeq OfferPage::createPageOffer(int userId, int limit) {

  IntSeq result;
  
  //get university info
  UniversityInfoSeq univInfoSeq = UserUniversityTripodAdapter::instance().getUniversityInfoSeq(userId);
  BOOST_FOREACH(UniversityInfoPtr univInfoPtr, univInfoSeq) {
    IntSeq pageSeq = PageUtil::getUniversityPages(univInfoPtr);
    result.insert(result.end(), pageSeq.begin(), pageSeq.end());
  }
  int univOfferNum = result.size();
  MCE_INFO("[OfferPage::createPageOffer] university page size: " << univOfferNum);
  
  //get college info
  CollegeInfoSeq collInfoSeq = UserCollegeTripodAdapter::instance().getCollegeInfoSeq(userId);
  BOOST_FOREACH(CollegeInfoPtr collInfoPtr, collInfoSeq) {
    IntSeq pageSeq = PageUtil::getCollegePages(collInfoPtr);
    result.insert(result.end(), pageSeq.begin(), pageSeq.end());
  }
  MCE_INFO("[OfferPage::createPageOffer] college page size: " << result.size() - univOfferNum);
  
  //get user pages
  IntSeq userPageSeq = PageUtil::getUserPages(userId);
  MCE_INFO("[OfferPage::createPageOffer] userId: " << userId << " follow pages size: " << userPageSeq.size());
  set<int> filterSet;
  BOOST_FOREACH(int pageId, userPageSeq) {
    filterSet.insert(pageId);
  }
  for (vector<int>::iterator pItr = result.begin(); pItr != result.end(); ) {
    set<int>::iterator it = filterSet.find(*pItr);
    if (it != filterSet.end()) {
      pItr = result.erase(pItr);
    } else {
      pItr++;
    }
  }

  if (result.size() <= limit) {
    return result;
  } else {
    result.resize(limit);
    return result;
  }

}



