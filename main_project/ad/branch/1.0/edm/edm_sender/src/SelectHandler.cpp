/*
 * SelectHandler.cpp
 *
 *  Created on: 2012-2-26
 *      Author: ping.zhang
 */

#include "SelectHandler.h"
#include "BrandKeeper.h"
#include "EdmInvertedIndex.h"
#include "EdmStructCache.h"
#include "AdEdmUserTargetCacheAdapter.h"

using namespace xce::ad;

namespace xce {
namespace ad {

bool IsSameEcpmRandom(double max_ecpm, double ecpm){//相同竞价随机选
  return (fabs(max_ecpm - ecpm) < 1e-10 && (rand() & 1));
}

bool SelectByTopN(const set<AdGroupPtr>& bid_gids, set<AdGroupPtr>&result){//topN 方法
  MCE_DEBUG("Selector::select-->SelectByTopN selector.");

  double max_ecpm = -1;
  AdGroupPtr m_ptr = NULL;
  for(set<AdGroupPtr>::iterator itb = bid_gids.begin(); itb != bid_gids.end(); ++itb) {
    if((*itb)->IsCpm()) {
      double ecpm = (*itb)->max_price();
      if((ecpm > max_ecpm)
                    || IsSameEcpmRandom(max_ecpm, ecpm)) {
        max_ecpm = ecpm;
        m_ptr = *itb;//TO DO
      }
    } else if ((*itb)->IsCpc()) {
    }
  }
  if(m_ptr) {
    result.insert(m_ptr);
  }
  return !result.empty();
}

bool SelectByRand(const set<AdGroupPtr>& bid_gids, set<AdGroupPtr>&result) {//从定向广告中按权重随机选出广告
  MCE_DEBUG("SelectByRand::select");
  vector<pair<int, AdGroupPtr> > sort_gids;
  int total = 0;
  for(set<AdGroupPtr>::iterator itb = bid_gids.begin(); itb != bid_gids.end(); ++itb) {
    int w = (*itb)->edm_send_level();
    sort_gids.push_back(make_pair(w, (*itb)));
    total += w;
  MCE_DEBUG("SelectByRand--->w =" << w );
  }
  if (total > 0) {
    int randnum = rand() % total;
    int sum = 0;
    MCE_DEBUG("SelectByRand--->total =" << total << ",randnum = " << randnum);
    for (vector<pair<int, AdGroupPtr> >::iterator its = sort_gids.begin();
        its != sort_gids.end(); ++its) {
      sum += (*its).first;
      if (randnum <= sum) {
        result.insert((*its).second);
        break;
      }
    }
  }
  return !result.empty();
}


/*********************************************************
 *选择框架
 ********************************************************/

bool SelectHandler::handle(MatchHandlerParamter& para) {
  TimeStat ts; 
  TimeStat subts;
  int user_status = para.userStatus();
  vector<SelectorPtr> selector_list;
  MCE_INFO("SelectHandler::handle begin,user_status:" << user_status <<",user_id:" << para.user_id() << ",user_ip:" <<(unsigned int) para.userProfile().ip());
  switch(user_status) {
    case 1:
      selector_list.push_back(new BrandAdSelector());
      selector_list.push_back(new SMEAdSelector());
      break;
    case 2:
      selector_list.push_back(new ProductAdSelector());
      break;
    case 3:
      selector_list.push_back(new BrandAdSelector());
      selector_list.push_back(new SMEAdSelector());
      selector_list.push_back(new ProductAdSelector());
      break;
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
/***************************************************************************
 * 基础模块select 、handle
 * *************************************************************************/
bool Selector::handle(MatchHandlerParamter& para){
  MCE_DEBUG("base selector::handle begin "<<",user_id:" << para.user_id());
  if ((Self_ad == pub_type_) && BrandKeeper::instance().reserved(para)) {
    MCE_INFO("SMEAdSelector::handle-->reserve for brand ,not select:" << para.user_id());
    return false;
  }
  set<AdGroupPtr> bid_groups;
  set<AdGroupPtr> selected_groups;
  EdmInvertedIndex::instance().GetBidGroups(para.userProfile(),pub_type_,bid_groups);

  MCE_INFO("Selector::handle-->user_id:" << para.user_id() << ",pub_type:" << pub_type_ <<",send_type:" << send_type_ <<",target groups size:" << bid_groups.size());
  bool select_flag = select(bid_groups, selected_groups);
  if(!select_flag) {
    return false;
  }
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
