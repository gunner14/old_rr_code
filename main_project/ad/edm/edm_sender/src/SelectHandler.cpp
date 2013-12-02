/*
 * SelectHandler.cpp
 *
 *  Created on: 2012-2-26
 *      Author: ping.zhang
 */

#include "SelectHandler.h"
#include "BrandKeeper.h"
#include "AdEdmMonitor.h"
#include "EdmInvertedIndex.h"
#include "EdmStructCache.h"
#include "AdEdmMonitorAdapter.h"
#include "AdEdmUserTargetCacheAdapter.h"
#include "EdmDiscountCache.h"
#include "ParametersController.h"
#include "ad/Degrade/src/Degrade.h"

using namespace xce::ad;
using namespace xce::edm;

const string LEFT_AD_BY_EDM_THRESHOLD = "left_ad_by_edm_threshold";
const string LEFT_AD_BY_AD_THRESHOLD = "left_ad_by_ad_threshold";

namespace xce {
namespace ad {

bool IsSameEcpmRandom(double max_ecpm, double ecpm){//相同竞价随机选
  return (fabs(max_ecpm - ecpm) < 1e-10 && (rand() & 1));
}

bool SelectByTopN(const set<AdGroupPtr>& bid_gids, set<AdGroupPtr>&result){//topN 方法
  MCE_INFO("Selector::select-->SelectByTopN selector. candidate size:"<<bid_gids.size());
  double max_ecpm = -1;
  AdGroupPtr m_ptr = NULL;
  vector<pair<double, AdGroupPtr> > rand_result;
  for(set<AdGroupPtr>::iterator itb = bid_gids.begin(); itb != bid_gids.end(); ++itb) {
    if((*itb)->IsCpm()) {
      double discount = EdmDiscountCache::instance().GetDiscountRatio((*itb)->member_id());
	  if(discount < 0 ){
		MCE_INFO("Selector::select-->SelectByTopN, Filter it,member_id:"<<(*itb)->member_id()<<" groupid:"<<(*itb)->group_id()<<" discount :"<<discount);
		continue;
	  }
      double ecpm = (*itb)->max_price() * discount;
      MCE_DEBUG("Selector::select-->SelectByTopN,groupid:"<<(*itb)->group_id()<<"  memberid:"<<(*itb)->member_id()<<" ,discount:"<<discount);
      if((ecpm > max_ecpm) || IsSameEcpmRandom(max_ecpm, ecpm)) {
		MCE_DEBUG("Selector::select-->SelectByTopN, groupid:"<<(*itb)->group_id()<<" member_id"<<(*itb)->member_id()<<" max_price:"<<max_ecpm<<" ecpm:"<<ecpm<<" selected,");
		max_ecpm = ecpm;
		m_ptr = *itb;//TO DO
      }else{
	    MCE_DEBUG("Selector::select-->SelectByTopN,max_price:"<<max_ecpm<<" ecpm:"<<ecpm<<" not selected,");
	  }
    }else{
	  MCE_DEBUG("Selector::select-->SelectByTopN, Not Cpm Group, groupid:"<<(*itb)->group_id());  
	}
  }
  if(m_ptr) {
    result.insert(m_ptr);
	MCE_INFO("Selector::select-->SelectByTopN,Successful,member_id:"<<m_ptr->member_id()<<" groupid:"<<m_ptr->group_id());
  }
  if(result.empty()) {
    MCE_DEBUG("Selector::select-->SelectByTopN,Failed");
    return false;
  } else {
    MCE_DEBUG("Selector::select-->SelectByTopN,Successful");
    return true;
  }
}

bool LeftSMEAdSelector::SelectByEcpm(const set<AdGroupPtr>& good_groups, set<AdGroupPtr>& selected_groups, map<Ice::Long, double>& ecpm_map){
  double max_ecpm = -1;
  AdGroupPtr m_ptr = NULL;
  set<AdGroupPtr>::const_iterator s_iter = good_groups.begin();
  for (; s_iter != good_groups.end(); ++s_iter){
    double discount = EdmDiscountCache::instance().GetDiscountRatio((*s_iter)->member_id());
    if (discount < 0){
      MCE_INFO("Selector::select-->SelectByTopN, Filter it,member_id:"<<(*s_iter)->member_id()<<" groupid:"<<(*s_iter)->group_id()<<" discount :"<<discount);
      continue;
    }
    Ice::Long group_id = (*s_iter)->group_id();
    if (ecpm_map.find( group_id ) == ecpm_map.end()){
      MCE_ERROR("LeftSMEAdSelector::SelectByEcpm --> There is Error in left ad: " << group_id);
      continue;
    }
    double ecpm = ecpm_map[ group_id ] * discount;
    if ((ecpm > max_ecpm) || IsSameEcpmRandom(max_ecpm, ecpm)){
      MCE_DEBUG("Selector::select-->SelectByTopN, groupid:"<< group_id <<" member_id"<<(*s_iter)->member_id()<<" max_price:"<<max_ecpm<<" ecpm:"<<ecpm<<" selected,");
      max_ecpm = ecpm;
      m_ptr = *s_iter;//TO DO
    } else {
      MCE_DEBUG("Selector::select-->SelectByTopN,max_price:"<<max_ecpm<<" ecpm:"<<ecpm<<" not selected,");
    }
  }
  if (m_ptr){
    selected_groups.insert(m_ptr);
    return true;
  }
  return false;
}

bool SelectByRand(const set<AdGroupPtr>& bid_gids, set<AdGroupPtr>&result) {//从定向广告中按权重随机选出广告
  MCE_INFO("SelectByRand::select,candidate size:"<<bid_gids.size());
  vector<pair<double, AdGroupPtr> > sort_gids;
  double total = 0.0;
  for(set<AdGroupPtr>::iterator itb = bid_gids.begin(); itb != bid_gids.end(); ++itb) {
	double discount = EdmDiscountCache::instance().GetDiscountRatio((*itb)->member_id());
	if(discount < 0){
	  MCE_INFO("Selector::select-->SelectByRand, Filter it, member_id:"<<(*itb)->member_id()<<" groupid:"<<(*itb)->group_id()<<" discount:"<<discount);
	  continue;
	}
    double w = (double)((*itb)->edm_send_level()) * discount;
    sort_gids.push_back(make_pair(w, (*itb)));
    total += w;
    MCE_DEBUG("SelectByRand result groupid:"<<(*itb)->group_id()<<" member_id"<<(*itb)->member_id()<<" Send_level:"<<(*itb)->edm_send_level()<<"--->"<<w<<" discount:"<<discount);
  }
  MCE_DEBUG("SelectByRand--->total =" << total );
  if (total > 0) {
    double randnum = ((double)(rand() % 100)/100) * total;
    double sum = 0;
    MCE_DEBUG("SelectByRand--->total =" << total << ",randnum = " << randnum);
    for (vector<pair<double, AdGroupPtr> >::iterator its = sort_gids.begin();
        its != sort_gids.end(); ++its) {
      sum += its->first;
      if (randnum <= sum) {
        result.insert(its->second);
	    MCE_INFO("Selector::select-->SelectByRand,Success ,member_id:"<<its->second->member_id()<<" groupid:"<<its->second->group_id()<<" Send_level:"<<its->second->edm_send_level()<<"-->"<<its->first);
        break;
      }
    }
  }
  if(result.empty()) {
	MCE_DEBUG("Selector::select-->SelectByRand,Failed");
	return false;
  } else {                                             
    MCE_DEBUG("Selector::select-->SelectByRand,Successful");
    return true;                                       
  }
}


/*********************************************************
 *选择框架
 ********************************************************/

bool SelectHandler::handle(MatchHandlerParamter& para) {
  TimeStat ts; 
  TimeStat subts;
  int user_status = para.userStatus();
  vector<SelectorPtr> selector_list;
  //15.1 if user_status == 1, 2, 3; user is valid, unrepeated!
  MCE_INFO("SelectHandler::handle begin,user_status:" << user_status <<",user_id:" << para.user_id() << ",user_ip:" <<(unsigned int) para.userProfile().ip());
  switch(user_status) {
    case 1:
      selector_list.push_back(new BrandAdSelector());
      selector_list.push_back(new SMEAdSelector());
      if (DegradeManager::instance().IsEnable("left_ad_edm_sender")){
        selector_list.push_back(new LeftSMEAdSelector());
      }
      break;
    case 2:
      selector_list.push_back(new ProductAdSelector());
      break;
    case 3:
      selector_list.push_back(new BrandAdSelector());
      selector_list.push_back(new SMEAdSelector());
      if (DegradeManager::instance().IsEnable("left_ad_edm_sender")){
        selector_list.push_back(new LeftSMEAdSelector());
      }
      selector_list.push_back(new ProductAdSelector());
      break;
    //case 4:   //推左引擎测试使用
    //  selector_list.push_back(new LeftSMEAdSelector());
    //  break;
    default:
      MCE_WARN("SelectHandler::handle--->Unexpected user status.");
  }
  bool choose_flag = false;
  for(size_t i = 0; i < selector_list.size(); ++i) {
    choose_flag = selector_list[i]->handle(para);
    if(choose_flag){
      break;
    }
  }
  return choose_flag;
}

bool LeftSMEAdSelector::GetAdByEdmCtr(set<AdGroupPtr>& groups, set<AdGroupPtr>& left_groups,const UserProfile& user, map<Ice::Long, double>& ecpm_map){
  string str_left_by_edm_threshold = EngineParametersCache::instance().GetParameterByName(LEFT_AD_BY_EDM_THRESHOLD);
  if (str_left_by_edm_threshold.empty()){
    MCE_WARN("LeftSMEAdSelector::FindGoodAD --> left ad by edm threshold: " << str_left_by_edm_threshold);
    return false;
  }
  double left_by_edm_threshold = 0.0;
  try{
    left_by_edm_threshold = boost::lexical_cast<double>(str_left_by_edm_threshold);
  } catch (...){
    MCE_WARN("LeftSMEAdSelector::FindGoodAD --> boost cast left ad max count error: " << str_left_by_edm_threshold);
    return false;
  }

  ecpm_map.clear();

  set<AdGroupPtr>::const_iterator s_iter = groups.begin();
  for (; s_iter != groups.end(); ++s_iter){
    double edm_ecpm = 0.0;
    if (!UserDemographicCache::instance().GetAdGroupCtrFromEdm(user, (*s_iter)->group_id(), edm_ecpm)){
      continue;
    }
    edm_ecpm *= (*s_iter)->max_price();
    if (edm_ecpm >= left_by_edm_threshold){
      ecpm_map[(*s_iter)->group_id()] = edm_ecpm;
      left_groups.insert(*s_iter);
    }
  }

  if (left_groups.empty()){
    return false;
  }

  return true;
}

bool LeftSMEAdSelector::GetAdByADCtr(set<AdGroupPtr>& groups, set<AdGroupPtr>& left_groups, const UserProfile& user, map<Ice::Long, double>& ecpm_map){
  string str_left_by_ad_threshold = EngineParametersCache::instance().GetParameterByName(LEFT_AD_BY_AD_THRESHOLD);
  if (str_left_by_ad_threshold.empty()){
    MCE_WARN("LeftSMEAdSelector::GetAdByADCtr --> left ad by ad threshold: " << str_left_by_ad_threshold);
    return false;
  }

  double left_by_ad_threshold = 0.0;
  try{
    left_by_ad_threshold = boost::lexical_cast<double>(str_left_by_ad_threshold);
  } catch (...){
    MCE_WARN("LeftSMEAdSelector::GetAdByADCtr --> boost cast left ad max count error: " << str_left_by_ad_threshold);
    return false;
  }

  ecpm_map.clear();

  set<AdGroupPtr>::const_iterator s_iter;
  for (s_iter = groups.begin(); s_iter != groups.end(); ++s_iter){
    double ad_ecpm = 0.0;
    if (!UserDemographicCache::instance().GetAdGroupCtrFromAd(user, (*s_iter)->group_id(), ad_ecpm)){
      continue;
    }
    ad_ecpm *= (*s_iter)->max_price();
    if (ad_ecpm >= left_by_ad_threshold){
      ecpm_map[(*s_iter)->group_id()] = ad_ecpm;
      left_groups.insert(*s_iter);
    }
  }

  if (left_groups.empty()){
    return false;
  }

  return true;
}

bool LeftSMEAdSelector::GetLeftAD(set<AdGroupPtr>& groups, set<AdGroupPtr>& left_groups,const UserProfile& user, map<Ice::Long, double>& ecpm_map){
  if (GetAdByEdmCtr(groups, left_groups, user, ecpm_map)){
    return true;
  } else if (GetAdByADCtr(groups, left_groups, user, ecpm_map)){
    return true;
  } 
  
  return false;
}

void Selector::Filter(const set<AdGroupPtr>& bid_groups, set<AdGroupPtr>& filtered_groups){
  ostringstream adGroupIDs;
  set<AdGroupPtr>::const_iterator s_iter = bid_groups.begin();
  EdmSeq rawSeq;
  for (; s_iter != bid_groups.end(); s_iter++){
    EdmInfo edmInfo;
    switch (pub_type_){
        case Brand_ad:
            edmInfo.type = EdmBrand;
            break;
        case Self_ad:
                edmInfo.type = EdmSelf;
                break;
        case Left_ad:
                edmInfo.type = EdmLeftAd;
                break;
        case Product_ad:
                edmInfo.type = EdmProduct;
                break;
        default:
            edmInfo.type = EdmSelf;
    }
    edmInfo.campaignID = (*s_iter)->campaign_id();
    edmInfo.adGroupID = (*s_iter)->group_id();
    edmInfo.memberID = (*s_iter)->member_id();
    rawSeq.push_back(edmInfo);

    adGroupIDs << (*s_iter)->group_id() << " ";
  }

  //15.14 how many times ad appears in candidate list?
  MCE_INFO("Selector::Filter--> Candidate AdGroupIDs: " << adGroupIDs.str());
  EdmSeq filteredSeq;
  bool filterTag = true;
  try{
    filteredSeq = xce::edm::AdEdmMonitorAdapter::instance().Filter(rawSeq);
  }  catch (Ice::Exception& e) {
    filterTag = false;
    MCE_WARN("Selector::Filter: " << e << " AdEdmMonitorAdater ICE Failed!");
  } catch (std::exception& e) {
    filterTag = false;
    MCE_WARN("Selector::Filter: " << e.what() << " AdEdmMonitorAdapter STD Failed!");
  }

  if (!filterTag){
    //filtered_groups.swap(bid_groups);
    filtered_groups = bid_groups;
  } else {
    EdmSeq::const_iterator vIter = filteredSeq.begin();
    for (; vIter != filteredSeq.end(); vIter++){
      s_iter = bid_groups.begin();
      for (; s_iter != bid_groups.end(); s_iter++){
        if (vIter->adGroupID == (*s_iter)->group_id()){
          filtered_groups.insert(*s_iter);
	      }
      }
    }
  }
}

bool LeftSMEAdSelector::handle(MatchHandlerParamter& para){
  MCE_DEBUG("LeftSMEAdSelector::handle begin " << " user_id: " << para.user_id());
  set<AdGroupPtr> bid_groups;
  EdmInvertedIndex::instance().GetBidGroups(para.userProfile(), pub_type_, bid_groups);// get ad list
  // 15.2, 15.3, 15.4 according to pub_type, we will get brand , sme and product valid user count.
  // 15.9 no ad source! bid_groups.size() == 0
  MCE_INFO("Selector::handle-->user_id:" << para.user_id() << ",pub_type:" << pub_type_ <<",send_type:" << send_type_ <<",target groups size:" << bid_groups.size());

  set<AdGroupPtr> filtered_groups;
  Filter(bid_groups, filtered_groups);
 
  set<AdGroupPtr> left_groups;
  map<Ice::Long, double> ecpm_map;
  if (!GetLeftAD(filtered_groups, left_groups, para.userProfile(), ecpm_map)){
    return false;
  } 
  
  set<AdGroupPtr> selected_groups;
  bool select_flag = SelectByEcpm(left_groups, selected_groups, ecpm_map);
  
  if(!select_flag) {
    return false;
  }
  //all send sum, brand/sme/product send sum, 
  set<AdGroupPtr>::iterator aIter = selected_groups.begin();
  MCE_INFO("Selector::Handle --> UserID:" << para.user_id() << ",PubType:" << pub_type_ << ",AdGroupID:" << (*aIter)->group_id()); 
  build(para, selected_groups, send_type_);
  return true;

}
/***************************************************************************
 * 基础模块select 、handle
 * *************************************************************************/
bool Selector::handle(MatchHandlerParamter& para){
  MCE_DEBUG("base selector::handle begin "<<",user_id:" << para.user_id());
  if ((Self_ad == pub_type_) && BrandKeeper::instance().reserved(para)) {
    MCE_INFO("SMEAdSelector::handle-->reserve for brand ,not select:" << para.user_id());
    return false;
  }
  ostringstream adGroupIDs;
  set<AdGroupPtr> bid_groups;
  set<AdGroupPtr> selected_groups;
  EdmInvertedIndex::instance().GetBidGroups(para.userProfile(),pub_type_,bid_groups);
  // 15.2, 15.3, 15.4 according to pub_type, we will get brand , sme and product valid user count.
  // 15.9 no ad source! bid_groups.size() == 0
  MCE_INFO("Selector::handle-->user_id:" << para.user_id() << ",pub_type:" << pub_type_ <<",send_type:" << send_type_ <<",target groups size:" << bid_groups.size());

  set<AdGroupPtr> filtered_groups;
  Filter(bid_groups, filtered_groups);

  bool select_flag = select(filtered_groups, selected_groups);
  if(!select_flag) {
    return false;
  }
  //all send sum, brand/sme/product send sum, 
  set<AdGroupPtr>::iterator aIter = selected_groups.begin();
  MCE_INFO("Selector::Handle --> UserID:" << para.user_id() << ",PubType:" << pub_type_ << ",AdGroupID:" << (*aIter)->group_id()); 
  build(para, selected_groups, send_type_);
  return true;
}

bool Selector::select(const set<AdGroupPtr>& bid_gids, set<AdGroupPtr>&result){
  return SelectByRand(bid_gids, result);
}


void Selector::build(MatchHandlerParamter& para, const set<AdGroupPtr>&groups, SendType send_type){//扣费、组装
  set<SelectedAdGroupPtr> result;
  for(set<AdGroupPtr>::iterator itg = groups.begin(); itg != groups.end(); ++itg) {
    AdGroupPtr m_ptr = *itg;
    SelectedAdGroupPtr selected = new SelectedAdGroup(m_ptr);
    selected->set_cost(m_ptr->cost());//TO DO 价格怎么计算？cpm的max_price是发送一次的价格还是一千次的价格
    selected->set_send_type(send_type);
    result.insert(selected);
  }
  para.set_selected_groups(result);

}
/*****************************************************
 * SME重新定义选择方式
 */
bool SMEAdSelector::select(const set<AdGroupPtr>& bid_gids, set<AdGroupPtr>&result){
  return SelectByTopN(bid_gids, result);
}

}
}
