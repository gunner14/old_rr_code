#include <vector>
#include "UserRankHandler.h"
#include "AdMatchManagerI.h"
#include "AdConfig.h"
#include "AdBlindness.h"
#include "UserDemographicCache.h"
#include "ExperimentCache.h"
#include "MemcacheClient.h"

using namespace std;
using namespace mysqlpp;
namespace xce {
namespace ad {

bool UserRankHandler::handle(MatchHandlerParamter& para, AdResultMap& result) {
	if (para.PickCount() <= 0) {
		return true;
	}
	InitSelectPara(para);	
	long zone_id = para.adzone_->id();
	vector<AdSelectHandlerPtr> ad_select_handler_list;
	if(Adzone2ChooseLogicPool::instance().GetHandleListByAdzoneID(zone_id, ad_select_handler_list,para)){
		set<AdGroupPtr> bid_gids;
		//TODO 获取用户定向广告集合
		GetAds(para, zone_id, bid_gids);
		for(size_t i = 0; i < ad_select_handler_list.size(); ++i){
			ad_select_handler_list[i]->handle(bid_gids, para);
		}
	}
	MCE_INFO("UserRankHandler::handle Selected Result:"<< para.OutPutPara());
	return true;
}

//获取用户定向广告集
void UserRankHandler::GetAds(MatchHandlerParamter &para, long adzone, set<AdGroupPtr> &groups){
	const vector<GroupInfo> &zone_groups = para.group_info_dict_[adzone];
	for(vector<GroupInfo>::const_iterator it = zone_groups.begin(); it != zone_groups.end(); ++it){
		const GroupInfo &gInfo = *it;
		AdGroupPtr group = new AdGroup;
		group->member_id_ = gInfo.memberID;
		group->group_id_ = gInfo.groupID;
		group->campaign_id_ = gInfo.campaignID;
		group->creative_id_ = gInfo.creatives[0];
		group->widget_id_ = gInfo.widgetID;
		group->trans_type_ = gInfo.transType;
		group->member_category_ = gInfo.memberCategory;
		group->member_industry_ = gInfo.memberIndustry;
		group->bid_unit_id_ = gInfo.bidUnitID;
		group->trigger_type_ = gInfo.triggerType;
		group->max_price_ = lexical_cast<int>( gInfo.price );
		group->display_type_ = 4;
		groups.insert(group);
	}
//	MCE_DEBUG("UserRankHandle::GetAds size:"<< groups.size());
}

//初始化select_para参数
void UserRankHandler::InitSelectPara(MatchHandlerParamter &para){
	Ice::Long adzone = para.adzone_->id();
	UserProfile user_profile = para.userProfile_;
	bool is_reserved = false;
	if(para.tag_ != "" && para.tag_.find("REMAIN") != string::npos && (adzone == 100000000070 || adzone == 100000000072 || adzone == 100000000075)){
		is_reserved = true;
	}
	para.select_para_.is_reserved = is_reserved;
	para.select_para_.fresh_selected = false;
	para.select_para_.use_lr_ctr = CtrOnlineModelCache::instance().UseLrCtr(adzone, user_profile.id()); //TODO做好分流管理
	para.select_para_.use_rt_ctr = CtrOnlineModelCache::instance().UseRealTimeCtr(adzone);
	string key = "double_equal_ratio"; //double值相近比例系数
	para.select_para_.double_equal_ratio = EngineParametersCache::instance().GetParameterByName(key, 1e-2);
	ostringstream key_buff;
	key_buff << "ctr_regular_alpha_" << adzone; //ctr alpha值
	key = key_buff.str();
	para.select_para_.ctr_alpha = EngineParametersCache::instance().GetParameterByName(key, 1.0);
	key_buff.str("");
	key_buff << "ctr_regular_beta_" << adzone; //ctr belta值
	key = key_buff.str();
	para.select_para_.ctr_beta = EngineParametersCache::instance().GetParameterByName(key, 1.0);
	key_buff.str("");
	key_buff << "lr_bayes_confidence_th_" << adzone; // 分广告位lr_bayes_confidence_th值
  key =  key_buff.str();
	para.select_para_.lr_bayes_confidence_th = EngineParametersCache::instance().GetParameterByName(key, -1.0);
  if (para.select_para_.lr_bayes_confidence_th < 0.0) { //分广告位取不到数据
	  key = "lr_bayes_confidence_th"; // 全局通用默认lr_bayes_confidence_th值 
	  para.select_para_.lr_bayes_confidence_th = EngineParametersCache::instance().GetParameterByName(key, 0.0);
  }
	key = "lr_bayes_discount"; // lr_bayes_discount值
	para.select_para_.lr_bayes_discount = EngineParametersCache::instance().GetParameterByName(key, 1.0);

  //lr bayes 线上融合调权系数决策参数
  key = "lr_confidence_th0";
  para.select_para_.lr_confidence_th0 = EngineConfig::instance().GetConfigByName(key, 1.0);
  key = "lr_confidence_th1";
  para.select_para_.lr_confidence_th1 = EngineConfig::instance().GetConfigByName(key, 1.0);
  key = "lr_weight_0";
  para.select_para_.lr_weight_0 = EngineConfig::instance().GetConfigByName(key, 1.0);
  key = "lr_weight_1";
  para.select_para_.lr_weight_1 = EngineConfig::instance().GetConfigByName(key, 1.0);
  key = "lr_weight_2";
  para.select_para_.lr_weight_2 = EngineConfig::instance().GetConfigByName(key, 1.0);
}

string GetCpmMemberKey(Ice::Long zone_id, Ice::Long member_id) {
	stringstream s;
	s << member_id << "_member_" << zone_id;
	return s.str();
}

//过滤vip广告
set<AdGroupPtr> AdSelectHandler::FilterVipGroups(set<AdGroupPtr> & bid_gids,MatchHandlerParamter& para){
//MCE_DEBUG("FilterVipGroups bid_groups:"<<bid_gids.size());
	set<AdGroupPtr> cand_groups;   //自助
	for(set<AdGroupPtr>::iterator itg = bid_gids.begin(); itg != bid_gids.end(); ++itg) {
		if(!IsCompetedGroup((*itg), para.all_selected_groups_)) {//预过滤竞品
			Ice::Long widget_id = (*itg)->widget_id();
			if( Widget2TypeCache::instance().GetWidgetType(widget_id) == 1 ){                               //自助小图
				cand_groups.insert(*itg);
//			MCE_DEBUG("WIDGET_TYPE: adgroup_id:" << (*itg)->group_id() <<" widget_id:"<< widget_id << " TYPE1");
			} 
		}
	}
//MCE_DEBUG("FilterVipGroups cand_groups:"<<cand_groups.size());
  return cand_groups;
}

double AdSelectHandler::GetAllPkLambada(Ice::Long adzone){
	ostringstream buff;
	buff  << "new_material_lambada_" << (adzone % 100);
	string key = buff.str();
	return EngineParametersCache::instance().GetParameterByName(key, 0.0);
}

int AdSelectHandler::CompareDouble(double x, double y, double equal_ratio){
	if (fabs(x - y) < max(x, y) * equal_ratio) { //两个值非常接近
		return 0;
	} else {
		return (x > y) ? 1 : -1;
	}
}

//考虑是否与已选广告组构成竞争关系
bool AdSelectHandler::IsCompetedGroup(AdGroupPtr candidate_group, set<AdGroupPtr> & selected_ids) {
	vector<Ice::Long> compete_campaigns_seq = CompeteRef::instance().Get(candidate_group->campaign_id());
	vector<Ice::Long> compete_members_seq = CompeteRef::instance().Get(candidate_group->member_id());
	set<Ice::Long> compete_campaign_members;
	compete_campaign_members.insert(compete_campaigns_seq.begin(), compete_campaigns_seq.end());
	compete_campaign_members.insert(compete_members_seq.begin(), compete_members_seq.end());
//MCE_DEBUG("AdSelectHandler::IsCompetedGroup-->gid:" << candidate_group->group_id()<<",compaign_members size:"<<compete_campaign_members.size());

	for(set<AdGroupPtr>::iterator its = selected_ids.begin(); its != selected_ids.end(); ++its) {
		if(candidate_group->member_id() == (*its)->member_id()) { //候选广告和已选广告列表是否存在同一广告主，存在视为竞品
//		MCE_DEBUG("AdSelectHandler::IsCompetedGroup candidate_group:" <<candidate_group->group_id() <<",selected_group:"<<(*its)->group_id()<<" has same memeber");
			return true;
		}
		//候选广告的广告计划的竞品是否是已选广告的广告计划
		set<Ice::Long>::iterator itc= compete_campaign_members.find((*its)->campaign_id()); 
		if(itc != compete_campaign_members.end()) {
//		MCE_DEBUG("AdSelectHandler::IsCompetedGroup candidate_group:" <<candidate_group->group_id() <<",selected_group:"<<(*its)->group_id()<<" has campaign compete ref");
			return true;
		}
		//候选广告的member_id的竞品是否是已选广告的member_id中
		itc= compete_campaign_members.find((*its)->member_id()); 
    if(itc != compete_campaign_members.end()) {
//    MCE_DEBUG("AdSelectHandler::IsCompetedGroup candidate_group:" <<candidate_group->group_id() <<",selected_group:"<<(*its)->group_id()<<" has member compete ref");
      return true;
    }
  }
  return false;
}

//检查是否还需要继续选择广告
bool AdSelectHandler::CheckNeedSelectMore(MatchHandlerParamter & para){
	if(para.PickCount() <= 0 ){
		return false;
	}
	if(para.GetSelectedType() == SELECT_VIP || para.GetSelectedType() == SELECT_NEW_BIG){
		return false;
	}
	return true;
}

//随机选择广告
vector<AdGroupPtr> AdSelectHandler::RandomSelectAdGroup(
    const UserProfile & user_profile, set<AdGroupPtr>& bid_gids, set<AdGroupPtr>& all_selected_gids, 
    const Ice::Long zone_id ,const int need_count) {
//MCE_DEBUG("AdSelectHandler::RandomSelectAdgroup-->bid gid size:" << bid_gids.size() << ",all_selected_gids size:" << all_selected_gids.size() << ",adzone:" <<zone_id <<",need_count:" << need_count );

	set<AdGroupPtr> selected_gids(all_selected_gids.begin(), all_selected_gids.end());
	vector<AdGroupPtr> result;
//MCE_DEBUG("AdSelectHandler::RandomSelectAdgroup-->bid gid size:" << bid_gids.size() << ",all_selected_gids size:" << all_selected_gids.size() << ",adzone:" << zone_id << ",need_count:" << need_count);
	for(int i = 0; i < need_count; ++i) {
		vector<pair<double, AdGroupPtr> > sort_gids;
		double total = 0;
		for(set<AdGroupPtr>::iterator itb = bid_gids.begin(); itb != bid_gids.end(); ++itb) {
//		MCE_DEBUG("RandomSelectAdGroup-->gid:" << (*itb)->group_id() <<",w:" << (*itb)->max_price());
			if(!IsCompetedGroup((*itb), selected_gids)){
				double quality_score =  AdsQualityCache::instance().GetAdQuScore((*itb)->group_id(),zone_id);  // 获取score
				//部分二跳计价用户ecpm打折
				double discount = DiscountCache::instance().GetDiscountRatio((*itb)->member_id());
				double w = (*itb)->max_price() * discount * quality_score;
				if((*itb)->IsCpm())	{	//cpm
					w *= 10;
				}
				sort_gids.push_back(make_pair(w, (*itb)));
				total += w;
			}
		}
		if(total < 0) {
			break;
		}
		double randnum = ((double)(rand() % 1000) / 1000.0) * total;
		double sum = 0;
		for(vector<pair<double, AdGroupPtr> >::iterator its = sort_gids.begin(); its != sort_gids.end(); ++its) {
			sum += (*its).first;
			if(randnum <= sum) {
				result.push_back((*its).second);
				selected_gids.insert((*its).second);
				break;
			}
		}
	}
	return result;
}
/************************************************
 *  N次TOP1获取选取广告
 *  @return 结果集合
 *
 *************************************************/
bool AdSelectHandler::Bidding(
    SelectedAdGroupSeq& result,
		set<AdGroupPtr>& groups, set<AdGroupPtr>& all_selected_groups_local, 
    MatchHandlerParamter& para, UserIndexSelectPara& select_para) {
	Ice::Long adzone = para.adzone_->id();
	int count = select_para.count;
	const UserProfile & user_profile = para.userProfile_;

//MCE_DEBUG("Bidding zone_id:"<<adzone << " need_count:"<<count);
	set<AdGroupPtr> selected_ids;
	double price_power = EngineConfig::instance().price_power();
	double ctr_power = EngineConfig::instance().ctr_power();
//MCE_DEBUG("Bidding, ctr_power:" << ctr_power << ",price_power:" << price_power);
	int pos = result.size() + 1;

	AdSelectZoneInfoPtr info = new AdSelectZoneInfo(para.refresh_idx_, adzone, count);
	map<int, string> common_feature_data;
	if (select_para.use_lr_ctr) {
		CtrOnlineModelCache::instance().FillCommonFeatureData(common_feature_data, para, info);
	}
	for(int index = 0; index < count; ++index) {
		info->SetPos(pos);
		double select_ecpm = -1;
		AdGroupPtr m_ptr = NULL;
		int state = 0;
		double ctr = -1.0;
		pair<double, double> common_w_c(0.0, 0.0);
		if (select_para.use_lr_ctr) {
			CtrOnlineModelCache::instance().ReassignPos(common_feature_data, info->Pos());
			AdCtrFeatureList feature_list = CtrOnlineModelCache::instance().GetCommonFeatureList(); 
			common_w_c = CtrOnlineModelCache::instance().GetFeatureWeightByList(common_feature_data, feature_list);
		}
		vector<UserIndexSelectItem> candidates;
		double max_ecpm = -1.0; 
//	MCE_DEBUG("Bidding bid_gids:"<<groups.size());
		for(set<AdGroupPtr>::const_iterator it = groups.begin(); it != groups.end(); ++it) {
			if(!IsCompetedGroup((*it), all_selected_groups_local)) { //是否与已选存在竞争，不存在进行下边操作,即：找最大的ecpm
				pair<double,int> ctr_state;
				if (select_para.use_lr_ctr) { // lr ctr
					double lr_confidence = 0.0;
					pair<double,int> lr_state = 
						CtrOnlineModelCache::instance().GetCtr(common_feature_data, (*it), common_w_c, info, lr_confidence);//计算lrctr
					pair<double,int> bayes_state = 
            UserDemographicCache::instance().GetUserAdGroupsCtr(user_profile, adzone ,pos ,(*it)->group_id(),(*it)->member_id(),""); 
          CtrOnlineModelCache::instance().AdjustCtr(ctr_state, lr_state, bayes_state, select_para, lr_confidence);
				} else { // userdemo ctr
					ctr_state = 
            UserDemographicCache::instance().GetUserAdGroupsCtr(user_profile, adzone ,pos ,(*it)->group_id(),(*it)->member_id(),"");
				}

				double quality_score =  AdsQualityCache::instance().GetAdQuScore((*it)->group_id(),adzone);  // 获取score
				double discount =  DiscountCache::instance().GetDiscountRatio((*it)->member_id());
				double ecpm = -1.0;
//			MCE_DEBUG("Bidding quality_score:"<<quality_score<< "discount:"<<discount );
				if((*it)->IsCpc())	{	//cpc
					if (select_para.use_lr_ctr){
						ecpm = select_para.ctr_alpha * pow(ctr_state.first, select_para.ctr_beta) * (*it)->max_price();  
					} else {
						ecpm = pow(ctr_state.first, ctr_power) * pow((*it)->max_price(), price_power); 
					}
					ecpm *= discount * quality_score;
				} else if((*it)->IsCpm())	{	//cpm
					ecpm = (*it)->max_price() * 0.001 *  discount * quality_score; 
				}
				if (CompareDouble(ecpm, max_ecpm, select_para.double_equal_ratio)>= 0) { //比当前最大值大或者接近才有必要继续
					UserIndexSelectItem cand;
					cand.gptr = (*it);
					cand.ecpm = ecpm; 
					cand.ctr = ctr_state.first;
					cand.state = ctr_state.second;
					candidates.push_back(cand);
					if (max_ecpm < ecpm){ // 获取最大值
						max_ecpm = ecpm;
          } 
        }
      }
    }
//  MCE_DEBUG("Bidding candidates size:"<<candidates.size());
    //随机抽样过程
    unsigned int rand_count = 0;
    for (vector<UserIndexSelectItem>::iterator it = candidates.begin(); it != candidates.end(); ++it) {
      if (0 == CompareDouble(max_ecpm, it->ecpm, select_para.double_equal_ratio)) {
        if (0 == rand() % (++rand_count)) {
          m_ptr = it->gptr;
          select_ecpm = it->ecpm;
          ctr = it->ctr;
          state = it->state;
        }
      }
    } 
    //MCE_INFO("DEBUG select: adzone = " << adzone << " select from " << groups.size() << " ads when rand_count=" << rand_count ); 
    //找到该pos的最佳广告后在列表中插入广告
    if(m_ptr) {
      SelectedAdGroupPtr selected = new SelectedAdGroup(m_ptr);
      //计价调用calculate_cost()方法
      selected->cost_ = m_ptr->calculate_cost(adzone);
      selected->ecpm_ = select_ecpm;
      selected->demo_state_ = state;
      selected->demo_ctr_ = ctr;
      selected->pos_ = pos++;
      result.push_back(selected);    
      all_selected_groups_local.insert(m_ptr);
//    MCE_DEBUG("debug_uid uid=" << user_profile.id() << "'debug info:" << "[" << selected->pos_ << "] id" << selected->group_->group_id()  <<" cost=" << selected->cost_ << " ecpm=" << max_ecpm<<",state:" <<selected->demo_state_<<",ctr:"<<selected->demo_ctr_<<",pos:" << selected->pos_ <<",zone:"<<adzone);
    }
  }
  return (result.size() > 0);
}

//////////////////////////////////////////////////////////////////////////
void TestAdImprRatioAssignCache::Load() {
	try {
    Statement sql;
		sql << "select adzone_id,test_ratio from test_impr_ratio";
		MCE_INFO("TestAdImprRatioAssignCache::Load try to excute ");
    mysqlpp::StoreQueryResult res = QueryRunner(EngineConfig::instance().ConfigDB(), CDbRServer, "test_impr_ratio").store(sql);
		if (!res) {
			MCE_WARN("FreshAdCache::LoadAuditedOnlineFreshAd Error.  select  fail!");
			return;
		}
		MCE_INFO("FreshAdCache::LoadAuditedOnlineFreshAd,audit gids Size=: " << res.num_rows());
    map<Ice::Long, double> zone2ratio;
		for (size_t i = 0; i < res.num_rows(); ++i) {
			mysqlpp::Row row = res.at(i);
			Ice::Long adzone = boost::lexical_cast<Ice::Long> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["adzone_id"]);
			double ratio = boost::lexical_cast<double> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["test_ratio"]);
      zone2ratio[adzone] = ratio;
		}
    IceUtil::RWRecMutex::WLock lock(mutex_);
    adzone2testratio_.swap(zone2ratio);
	}catch (std::exception& e) {
		MCE_WARN("FreshAdCache::LoadAuditedOnlineFreshAd audit error: " << e.what());
	} catch (...) {
		MCE_WARN("FreshAdCache::LoadAuditedOnlineFreshAd audit error!");
	}
  
}

/*
void TestAdImprRatioAssignCache::Load() {
	MCE_INFO("TestAdImprRatioAssignCache::Load");
	try {
		MyUtil::Date yestoday = MyUtil::Date::seconds(time(NULL) - 1 * 24 * 3600);  //昨天
		Ice::Long yestoday_time = ((yestoday.year() * 100 + yestoday.month())*100 + yestoday.day());
		Ice::Long begin_time = yestoday_time * 100;
		const char* db_host = "statdb.db.jebe.d.xiaonei.com";
		const char* db_name = "statdb";
		Connection conn(db_name,db_host,"ad","adjb###");
		stringstream sql;
		sql << "select yl_type , FLOOR(TIME/100) as day ,SUM(pv_count)as sum_impr from Log_HMZD where adzone_id in (100000000070,100000000072) and time >= "
			<< begin_time << " group by adzone_id, yl_type, FLOOR(TIME/100)";
		MCE_INFO("TestAdImprRatioAssignCache::Load try to excute " << sql.str());
		string s= sql.str();
		Query q(&conn,true,s.c_str());
		mysqlpp::StoreQueryResult res = q.store();
		if (!res) {
			MCE_WARN("FreshAdCache::LoadAuditedOnlineFreshAd Error " << sql.str() << ".  select  fail!");
			return;
		}
		MCE_INFO("FreshAdCache::LoadAuditedOnlineFreshAd,audit gids Size=: " << res.num_rows());
		for (size_t i = 0; i < res.num_rows(); ++i) {
			mysqlpp::Row row = res.at(i);
			int yl_type = (int) row["yl_type"];
			Ice::Long day = (Ice::Long) row["day"];
			Ice::Long impr = (Ice::Long) row["sum_impr"];
			if(impr > 0) { // 如果数据库有问题，按照默认或之前的数据，保证奇异值（impr=0）不出现
				if(yl_type == 0) {
					if(yestoday_time == day) {
						yestoday_unreserve_impr_ = impr;
					} else {
						today_hour_unreserve_impr_ = impr;
					}
				} else {
					if(yestoday_time == day) {
						yestoday_reserve_impr_ = impr;
					} else {
						today_hour_reserve_impr_ = impr;
					}
				}
			}
		}
	}catch (std::exception& e) {
		MCE_WARN("FreshAdCache::LoadAuditedOnlineFreshAd audit error: " << e.what());
	} catch (...) {
		MCE_WARN("FreshAdCache::LoadAuditedOnlineFreshAd audit error!");
	}
	//////////////////////////预测今天的余量
	double yestoday_ratio = (double)yestoday_reserve_impr_ / (double) yestoday_unreserve_impr_;
	double today_ratio = (double)today_hour_reserve_impr_ / (double) today_hour_unreserve_impr_;
	double ratio = today_ratio / yestoday_ratio;
	today_predict_reserve_impr_ = lexical_cast<Ice::Long>( yestoday_reserve_impr_ * ratio );
	MCE_INFO("TestAdImprRatioAssignCache::Load->over,yestoday_unre:" <<yestoday_unreserve_impr_ << ",yestoday_re:" <<yestoday_reserve_impr_
			<<",today_hour_unreserve_impr:" << today_hour_unreserve_impr_ << ",today_hour_reserve_impr:" << today_hour_reserve_impr_ <<
			",today_predice_re_impr:" << today_predict_reserve_impr_ << ",ratio:" << ratio);
}
*/
string GetFreshKey(Ice::Long zone_id, int pos) {
	Ice::Long key = zone_id * 10 + pos;
	ostringstream sin;
	sin << key;
	return sin.str();
}

//AdGroupPtr AdSelectHandler::GetZonePosFreshGroup(int userid, FreshAdQualityMap & fresh_groups, set<AdGroupPtr>& bid_ids, 
//		set<AdGroupPtr>& selected_ids, long adzone, int pos, bool is_reserved) {
AdGroupPtr AdSelectHandler::GetZonePosFreshGroup(int userid, FreshAdQualityMap & fresh_groups, set<AdGroupPtr>& bid_ids, 
		set<AdGroupPtr>& selected_ids, long adzone) {
	AdGroupPtr r_ptr = NULL;
	double randnum = (rand() % 100) / 100.0;
	double test_ratio = 0.0;
/*	if(is_reserved){
		test_ratio = EngineConfig::instance().GetReservedStateTestImprRatio(adzone, pos);
	}else {
		test_ratio = EngineConfig::instance().GetStateTestImprRatio(adzone, pos);
	}*/
//	test_ratio = TestAdImprRatioAssignCache::instance().GetTestRatio(is_reserved);
	test_ratio = TestAdImprRatioAssignCache::instance().GetTestRatio(adzone);
/*	if(100000000001 == adzone) {
		test_ratio = 0.05;
	}*/
//	MCE_DEBUG("[Fresh:],GetZonePosFreshGroup,userid:" << userid <<" adzone:" << adzone <<",pos:" << pos << "fresh_map size:" << fresh_groups.size() <<",bid size:" << bid_ids.size() <<",randnum:" << randnum <<",test ratio:" << test_ratio<<"is_reserved:" << is_reserved);
//MCE_DEBUG("[Fresh:],GetZonePosFreshGroup,userid:" << userid <<" adzone:" << adzone << "fresh_map size:" << fresh_groups.size() <<",bid size:" << bid_ids.size() <<",randnum:" << randnum <<",test ratio:" << test_ratio);
	if(randnum >= test_ratio) {
		return r_ptr;
	}
	string key = "reserved";
	FreshAdQualityMap::iterator itf = fresh_groups.find(key); 
	if(itf != fresh_groups.end()) {
		FreshAdQualitySet& f_groups = itf->second;
//		MCE_DEBUG("[Fresh:],GetZonePosFreshGroup,userid:" << userid <<" adzone:" << adzone <<",pos:" << pos << "fresh groups size:" << fresh_groups.size() <<",bid size:" << bid_ids.size());
//	MCE_DEBUG("[Fresh:],GetZonePosFreshGroup,userid:" << userid <<" adzone:" << adzone << "fresh groups size:" << fresh_groups.size() <<",bid size:" << bid_ids.size());
		double total = 0.0;
		vector<pair<double, AdGroupPtr> > rand_result;
		for(set<AdGroupPtr>::iterator itb = bid_ids.begin(); itb != bid_ids.end(); ++ itb) {
			FreshAdQualitySet::iterator it = f_groups.find((*itb)->group_id());
			if(it != f_groups.end()) {//是否在新广告列表中，如果在，参与随机
//			MCE_DEBUG("[Fresh:],exist in fresh groups ,gid:" << it->first<<",userid:" << userid<<",ratio:" << it->second);
				if(!IsCompetedGroup((*itb), selected_ids)) { //是否与已选存在竞争，不存在进行下边操作,即:参与随机
					double weight = it->second;
					if((*itb)->widget_id() == 31 && adzone == 100000000070) {
						weight *= 10;
					}
					total += weight;
					rand_result.push_back(make_pair(weight,(*itb)));
				}
			}
		}
//		MCE_DEBUG("[Fresh:]:userid:" <<userid <<",total score" << total << ",random:" << randnum <<",adzone:" << adzone<< ",pos:" << pos<<"test ratio:" <<test_ratio);
//	MCE_DEBUG("[Fresh:]:userid:" <<userid <<",total score" << total << ",random:" << randnum <<",adzone:" << adzone << "test ratio:" <<test_ratio);
		if(total > 0.0) {
			double ratio = 1.0;
			ratio = test_ratio / total;
//			MCE_DEBUG("[Fresh:]:userid" << userid << ",total score" << total << ",random:" << randnum <<",adzone:" << adzone<< ",pos:" << pos<<",test ratio:" << test_ratio <<",ratio:" << ratio <<",is_reserved:" << is_reserved);
//		MCE_DEBUG("[Fresh:]:userid" << userid << ",total score" << total << ",random:" << randnum <<",adzone:" << adzone << ",test ratio:" << test_ratio <<",ratio:" << ratio);
			double sum = 0.0;
			for(vector<pair<double,AdGroupPtr> >::iterator itr = rand_result.begin(); itr != rand_result.end(); ++itr) {
				sum += (*itr).first * ratio;
				if(randnum < sum) {
					return (*itr).second;
				}
      }
    }
  }
  return r_ptr;
}



void DiscountCache::Load() {  
	MCE_INFO("[Discount:],DiscountCache::Load start");
	try {
		mysqlpp::StoreQueryResult res;
		Statement sql;
		sql << "select * from discount_ratio";
		res = QueryRunner(EngineConfig::instance().ConfigDB(), CDbRServer, "discount_ratio").store(sql);
		if (!res) {
			MCE_WARN("DiscountCache::Load DiscountCache Error select  fail!");
			return;
		}
		MCE_INFO("DiscountCache::Load DiscountCache, Discount_ratio Size=: " << res.num_rows());
		map<Ice::Long, double> pool;
		for (size_t i = 0; i < res.num_rows(); ++i) {
			mysqlpp::Row row = res.at(i);
			Ice::Long member_id = (Ice::Long) row["member_id"];
			double discount_ratio = (double) row["discount_ratio"];
			pool[member_id] = discount_ratio;
		}
		IceUtil::RWRecMutex::WLock lock(mutex_);
		discount_map_.swap(pool);
		MCE_INFO("End: DiscountCache::Load size = " << discount_map_.size());
	}catch (Ice::Exception& e) {
		MCE_WARN("DiscountCache::load --> call Database ERR  line:"
				<< __LINE__ << " err:" << e);
	} catch (std::exception& e) {
		MCE_WARN("DiscountCache::load --> call Database ERR  line:"
				<< __LINE__ << " err:" << e.what());
	} catch (...) {
		MCE_WARN("DiscountCache::LoadDiscountCache error!");
  }
}

void Widget2TypeCache::Load() {
	MCE_INFO("[Widget2Type:],Widget2TypeCache::Load start");
	try {
		const char* db_host = "statdb.db.jebe.d.xiaonei.com";
		const char* db_name = "jebe_main";
		Connection conn(db_name,db_host,"ad","adjb###");
		stringstream sql;
		sql << "select * from widget_type";
		MCE_INFO("Widget2TypeCache::Load try to excute " << sql.str());
		string s= sql.str();
		Query q(&conn,true,s.c_str());
		mysqlpp::StoreQueryResult res = q.store();
		if (!res) {
			MCE_WARN("Widget2TypeCache::Load Widget2TypeCache Error " << sql.str() << ".  select  fail!");
			return;
		}
		MCE_INFO("Widget2TypeCache::Load Widget2TypeCache, Widget2TypeCache Size=: " << res.num_rows());
		map<Ice::Long, int> pool;
		for (size_t i = 0; i < res.num_rows(); ++i) {
			mysqlpp::Row row = res.at(i);
			Ice::Long widget_id = (Ice::Long) row["widget_id"];
			int type = (int)row["type"];
			pool[widget_id] = type;
		}
		IceUtil::RWRecMutex::WLock lock(mutex_);
		widget2Type_map_.swap(pool);
		MCE_INFO("End: Widget2TypeCache::Load size = " << widget2Type_map_.size());

	}catch (Ice::Exception& e) {
		MCE_WARN("Widget2TypeCache::load --> call Database ERR  line:"
        << __LINE__ << " err:" << e);
  } catch (std::exception& e) {
    MCE_WARN("Widget2TypeCache::load --> call Database ERR  line:"
        << __LINE__ << " err:" << e.what());
  } catch (...) {
    MCE_WARN("Widget2TypeCache::LoadDiscountCache error!");
  }
}

//根据各个广告位的配置填空对应的handle
void Adzone2ChooseLogicPool::FillHandleListByConfig(const vector<int> & vechandle, vector<AdSelectHandlerPtr> &ad_select_handler_list){
	vector<int>::const_iterator it = vechandle.begin();
	for (; it != vechandle.end(); ++it ) {
		switch (*it){
			case CPMMEMBER: //糯米保量
				ad_select_handler_list.push_back(new CpmMemberAdSelectHandle());
				break;
			case NEWAD:    //新广告
				ad_select_handler_list.push_back(new FreshAdSelectHandle());
				break;
			case ALLPK:   //全竞价
				ad_select_handler_list.push_back(new AllPkSelectHandle());
				break;
			case BIDDINGN:  //N次Top1
				ad_select_handler_list.push_back(new BiddingSelectHandle());
				break;
			case RANDOM:    //随机
				ad_select_handler_list.push_back(new RandomSelectHandle());
				break;
			case REFRESH:   //下一个更精彩,特殊处理,
				break;
			default:
				MCE_WARN("Adzone2ChooseLogicPool::FillHandleListByConfig : Config for Adzone handlelist is ERROR :"<< *it);
				break;
		}
	}
}

bool Adzone2ChooseLogicPool::GetHandleListByAdzoneID(const Ice::Long & adzone, vector<AdSelectHandlerPtr> & ad_select_handler_list, MatchHandlerParamter & para){
	int refresh_source = lexical_cast<int> (para.refresh_source_);
	int refresh_idx = lexical_cast<int> (para.refresh_idx_);
	//如果是下一个更精彩，只需要选择ReFreshSelectHandle
	if(StrategyConfig::instance().IsIncludeReFresh(adzone) && (refresh_source == 3 && refresh_idx == 0)){
		ad_select_handler_list.push_back(new RefreshSelectHandle());
		return true;
	}
	IceUtil::RWRecMutex::RLock lock(mutex_);
	map<Ice::Long,vector<AdSelectHandlerPtr> >::iterator it = adzone2handlelists_.find(adzone);
	if(it != adzone2handlelists_.end()){
		ad_select_handler_list = it->second;
		return true;
	}else{
		//根据默认逻辑选择（100000000000代表默认）
		it = adzone2handlelists_.find(100000000000);
		if(it != adzone2handlelists_.end()){
			ad_select_handler_list = it->second;
			return true;
		}
	}
	return false;
}

//从配置获取每个广告位对应的handle数字数组，填充对应的handle
void Adzone2ChooseLogicPool::Load(){
	map<Ice::Long,vector<AdSelectHandlerPtr> > poolselect;
	map<Ice::Long, vector<int> > mapAdzone2Vec = StrategyConfig::instance().GetSelectList();
	map<Ice::Long, vector<int> >::iterator it = mapAdzone2Vec.begin();
	for ( ; it != mapAdzone2Vec.end(); ++it){
		vector<AdSelectHandlerPtr> vecHandleList;
		FillHandleListByConfig(it->second,vecHandleList);
		poolselect[it->first] = vecHandleList; 
	}
	{
		IceUtil::RWRecMutex::WLock lock(mutex_);
		adzone2handlelists_.swap(poolselect);
	}
}

//获取并过滤保量广告
set<AdGroupPtr> AdSelectHandler::FilterAndGetCpmGroups(set<AdGroupPtr>& candidate_groups,MatchHandlerParamter & para){
	set<AdGroupPtr>  cpm_member_groups;
	Ice::Long adzone = para.adzone_->id();
	CpmAdgroupMap cpm_pool = CpmLoaderCache::instance().GetCpmAdGroupMap();
	for(set<AdGroupPtr>::iterator itg = candidate_groups.begin(); itg != candidate_groups.end();) {//查找符合可投的按广告商投放的广告
		CpmAdgroupMap::iterator itc;
		stringstream s;
		s << (*itg)->member_id();
		itc = cpm_pool.find(s.str());
		if(itc != cpm_pool.end()) {
			string key = GetCpmMemberKey(adzone, (*itg)->member_id());
			itc = cpm_pool.find(key);
			if(itc != cpm_pool.end()) {
				CpmAdgroup cpm_group = itc->second;
				if(cpm_group.pv > 0 && !IsCompetedGroup((*itg), para.all_selected_groups_)) {//如果还需要保量,并且和其他没有竞品关系
					cpm_member_groups.insert(*itg);
				}
			}
			candidate_groups.erase(itg++);
		} else {
			++itg;
		}
	}
	return cpm_member_groups;
}

//////////////////////////////////////////////////////////////////
void BiddingSelectHandle::handle(set<AdGroupPtr>& bid_gids,MatchHandlerParamter& para){
//MCE_DEBUG("BiddingNSelectHandle zone_id:" << para.adzone_->id() << " PickCount:"<< para.PickCount()<<" bid_gids:"<< bid_gids.size());
	if( !CheckNeedSelectMore(para) ){
		return;
	}
	FilterAndGetCpmGroups(bid_gids,para); //过滤保量广告
	int count = para.PickCount();
	set<AdGroupPtr> candidate_groups = bid_gids;
	if(para.GetSelectedType() == SELECT_NEW_SMALL){
		candidate_groups = FilterVipGroups(bid_gids,para); 
//	MCE_DEBUG("BiddingSelectHandle::handle para.selected_type_ = smallnew ,after filter bid_gids:"<<candidate_groups.size());
	}
	SelectedAdGroupSeq tempResult;
	Ice::Long adzone = para.adzone_->id();
	const UserProfile & user_profile = para.userProfile_;
	UserIndexSelectPara select_para = para.SelectPara();
	select_para.count = count; 
//MCE_DEBUG("BiddingSelectHandle::handle Bidding");
  if(!Bidding(tempResult, candidate_groups, para.all_selected_groups_, para, select_para)){
		return;
	}	
//MCE_DEBUG("BiddingSelectHandle::handle selected size:"<<tempResult.size());
	SelectedAdGroupSeq::iterator itRes = tempResult.begin();
  for (; itRes != tempResult.end(); ++itRes){
	  para.zone_selected_groups_.insert(*itRes);
//  MCE_DEBUG("[BiddingSelectHandle] ,uid=" << para.userProfile_.id() <<",zone:" << adzone <<" pos:"<< (*itRes)->pos_ << " selected groups:" << (*itRes)->group_->group_id());
  }
  return;
}

//随机选择广告handle
void RandomSelectHandle::handle(set<AdGroupPtr>& bid_gids,MatchHandlerParamter& para){
//MCE_DEBUG("RandomSelectHandle zone_id:" << para.adzone_->id() << " PickCount:"<< para.PickCount()<<" bid_gids:"<< bid_gids.size());
	if(!CheckNeedSelectMore(para)){
		return;
	}
	int count = para.PickCount();
	FilterAndGetCpmGroups(bid_gids,para); //过滤保量广告
	set<AdGroupPtr> candidate_groups = bid_gids;
	if(para.GetSelectedType() == SELECT_NEW_SMALL){
		candidate_groups = FilterVipGroups(bid_gids,para);
	}
	const UserProfile & user_profile = para.userProfile_;
	Ice::Long adzone = para.adzone_->id();
	int pos = para.zone_selected_groups_.size() + 1;
	vector<AdGroupPtr> result_select = RandomSelectAdGroup(user_profile, candidate_groups, para.all_selected_groups_, adzone, count);
	for (vector<AdGroupPtr>::iterator it = result_select.begin(); it != result_select.end(); ++it) {
		SelectedAdGroupPtr select = new SelectedAdGroup(*it);
		select->cost_ = (*it)->calculate_cost(adzone);
		select->pos_ = pos++;
		select->demo_state_ = 100000 + 1;
		para.all_selected_groups_.insert(*it);
		para.zone_selected_groups_.insert(select);
//  MCE_DEBUG("[RandomSelectHandle] ,uid=" << para.userProfile_.id() <<",zone:" << adzone << " pos:"<<select->pos_  << "demo_state" << select->demo_state_ << " selected groups:"<< (*it)->group_id());
	}
}

//全竞价选择广告handle
void AllPkSelectHandle::handle(set<AdGroupPtr>& bid_gids,MatchHandlerParamter& para){
//MCE_DEBUG("AllPkSelectHandle zone_id:" << para.adzone_->id() << " PickCount:"<< para.PickCount()<<" bid_gids:"<< bid_gids.size());
	if(!CheckNeedSelectMore(para)){
		return;
	}
	int count = para.PickCount();
	FilterAndGetCpmGroups(bid_gids,para); //过滤保量广告
	set<AdGroupPtr> candidate_groups = bid_gids;
	if(para.GetSelectedType() == SELECT_NEW_SMALL){
		candidate_groups = FilterVipGroups(bid_gids,para);
	}

	SelectedAdGroupSeq tempResult;
	Ice::Long adzone = para.adzone_->id();
	const UserProfile & user_profile = para.userProfile_;
	UserIndexSelectPara select_para = para.SelectPara();
	select_para.count = count;
	SelectedAdGroupSeq result_self, result_vip;
	set<AdGroupPtr> all_selected_groups_local_self = para.all_selected_groups_,
		all_selected_groups_local_vip = para.all_selected_groups_;
	set<AdGroupPtr> groups_self;   //自助
	set<AdGroupPtr> groups_vip;   //vip 
	for(set<AdGroupPtr>::iterator itg = candidate_groups.begin(); itg != candidate_groups.end(); ++itg) {
		if(!IsCompetedGroup((*itg), para.all_selected_groups_)) {//预过滤竞品
			Ice::Long widget_id = (*itg)->widget_id();
			if(Widget2TypeCache::instance().GetWidgetType(widget_id) == 2) {  //擎天柱或VIP
				groups_vip.insert(*itg);
			//MCE_DEBUG("WIDGET_TYPE: adgroup_id:" << (*itg)->group_id() <<" widget_id:"<< widget_id << " TYPE2");
			} else if( Widget2TypeCache::instance().GetWidgetType(widget_id) == 1 ){                               //自助小图
				groups_self.insert(*itg);
			//MCE_DEBUG("WIDGET_TYPE: adgroup_id:" << (*itg)->group_id() <<" widget_id:"<< widget_id << " TYPE1");
			} else {
				MCE_WARN("WIDGET_TYPE: adgroup_id:" << (*itg)->group_id() <<" widget_id:"<< widget_id << " TYPE3");
			}
		}
	}
//MCE_DEBUG("AllPkSelectHandle: selfgroup size:"<< groups_self.size()<< " vipgroup size:"<< groups_vip.size());
  Bidding(result_self, groups_self, all_selected_groups_local_self, para, select_para);
	select_para.count = 1;//vip 只选择一个
  Bidding(result_vip, groups_vip, all_selected_groups_local_vip, para, select_para);

	double ecpm_groups_small = 0;
	double ecpm_groups_vip = 0;

	for (SelectedAdGroupSeq::iterator it = result_self.begin(); it != result_self.end(); ++it) {
		ecpm_groups_small += (*it)->ecpm_;
	}
	for (SelectedAdGroupSeq::iterator it = result_vip.begin(); it != result_vip.end(); ++it) {
		ecpm_groups_vip += (*it)->ecpm_;
	}
	double lambada = GetAllPkLambada(adzone); //VIP加权值
	if (ecpm_groups_vip + lambada > ecpm_groups_small) {
		//全竞价决策应该出vip
		para.SetSelectType(SELECT_VIP);
		para.all_selected_groups_ = all_selected_groups_local_vip;
		tempResult = result_vip;
//  MCE_DEBUG("AllPkSelectHandle::handle all pk decide vip, ecpm_vip:"<<ecpm_groups_vip<<"  ecpm_small"<<ecpm_groups_small<<" lambada:"<<lambada<<" adzone:"<<adzone);
	} else {   //全竞价决策出自助小图
		para.all_selected_groups_ = all_selected_groups_local_self;
		tempResult = result_self;
//  MCE_DEBUG("AllPkSelectHandle::handle all pk decide small, ecpm_vip:"<<ecpm_groups_vip<<" ecpm_small"<<ecpm_groups_small<<" lambada:"<<lambada<<" adzone:"<<adzone<<" while fresh_selected=" << (select_para.fresh_selected ? 1 : 0));
	}
	SelectedAdGroupSeq::iterator itRes = tempResult.begin();
	for (; itRes != tempResult.end(); ++itRes){
		para.zone_selected_groups_.insert(*itRes);
//  MCE_DEBUG("[AllPkSelectHandle] ,uid=" << para.userProfile_.id() <<",zone:" << adzone <<" pos:"<< (*itRes)->pos_ << " selected groups:" << (*itRes)->group_->group_id());
	}
}

//糯米保量handle
void CpmMemberAdSelectHandle::handle(set<AdGroupPtr>& bid_gids,MatchHandlerParamter& para) {
//MCE_DEBUG("CpmMemberSelectHandle zone_id:" << para.adzone_->id() << " PickCount:"<< para.PickCount()<<" bid_gids:"<< bid_gids.size());
	if(!CheckNeedSelectMore(para)){
		return;	
	}
	Ice::Long adzone = para.adzone_->id();
	set<AdGroupPtr> cpm_member_groups; 
	cpm_member_groups  = FilterAndGetCpmGroups(bid_gids,para);//获取保量广告
	vector<AdGroupPtr> selected_ids =  RandomSelectAdGroup(para.userProfile_, cpm_member_groups, para.all_selected_groups_, adzone, para.PickCount());
//MCE_DEBUG("CpmMemberSelectHandle cpm bid gid size:" << selected_ids.size()<< " cpm_member_groups size:"<< cpm_member_groups.size()<<",adzone:" << adzone);
	int pos = para.zone_selected_groups_.size() + 1;
	for (vector<AdGroupPtr>::iterator itg = selected_ids.begin(); itg != selected_ids.end(); ++itg) {
		para.all_selected_groups_.insert(*itg);
		AdGroupPtr m_ptr = *itg;
		SelectedAdGroupPtr select = new SelectedAdGroup(m_ptr);
		double max_cost = m_ptr->calculate_cost(adzone);
		if((*itg)->max_price() > max_cost) {
			select->cost_ = max_cost;
		} else {
			select->cost_ = (*itg)->max_price();
		}
		select->pos_ = pos++;
		select->demo_state_ = (adzone % 100) * 1000 + select->pos_ ;
		para.SetSelectType(SELECT_VIP);
		para.zone_selected_groups_.insert(select);
//	MCE_DEBUG("[CpmMemberSelectHandle] ,uid=" << para.userProfile_.id() <<",zone:" << " pos:" << select->pos_ << " demo_state:" << select->demo_state_ << adzone << " selected group:"<< m_ptr->group_id());
	}
	return;  
}

//新广告handle
void FreshAdSelectHandle::handle(set<AdGroupPtr>& bid_gids,MatchHandlerParamter& para) {
//MCE_DEBUG("FreshSelectHandle zone_id:" << para.adzone_->id() << " PickCount:"<< para.PickCount()<<" bid_gids:"<< bid_gids.size());
	if(!CheckNeedSelectMore(para)){
		return;
	}
	FilterAndGetCpmGroups(bid_gids,para);
	set<AdGroupPtr> candidate_groups = bid_gids;
	//如果之前选择过新广告，并且是小图，则只能从自助小图中选择
	if(para.GetSelectedType() == SELECT_NEW_SMALL){
		candidate_groups = FilterVipGroups(bid_gids,para);
	}
	const UserProfile & user_profile = para.userProfile_;
	Ice::Long adzone = para.adzone_->id();
	int pos = para.zone_selected_groups_.size() + 1;
	UserIndexSelectPara select_para = para.SelectPara();
	AdGroupPtr m_ptr = NULL;
	FreshAdQualityMap fresh_groups = FreshAdGroupsCache::instance().GetFreshGroups();
//	m_ptr = GetZonePosFreshGroup(user_profile.id(), fresh_groups, candidate_groups, para.all_selected_groups_, adzone, pos, select_para.is_reserved);
	m_ptr = GetZonePosFreshGroup(user_profile.id(), fresh_groups, candidate_groups, para.all_selected_groups_, adzone);
	if(m_ptr) {
		SelectedAdGroupPtr select = new SelectedAdGroup(m_ptr);
		//计价调用calculate_cost()方法
		select->cost_ = m_ptr->calculate_cost(adzone);
		//select->ecpm_ = m_ptr->ecpm;
		int factor = 10;
		if(select_para.is_reserved) {
			factor = 100;
		}
		if (100000000001 == adzone) { //01位置新广告和预估部分目前重复，特判下，TODO: 制定demo_state使用体系
			factor *= 10;
		}
    select->pos_ = pos;
		select->demo_state_ = (adzone % 100) * factor + select->pos_ ;
    para.all_selected_groups_.insert(m_ptr);
		para.zone_selected_groups_.insert(select);
/*
    MCE_DEBUG("[FreshSelectHandle] getfresh,uid=" << user_profile.id() <<",zone:" << adzone << ",pos:" << select->pos_ 
        <<"selected gid:" << select->group_->group_id() <<",state:" <<select->demo_state_
        <<",widdget:" << m_ptr->widget_id() << ",cost:" << select->cost_ <<",maxprice:" << m_ptr->max_price());
*/
    Ice::Long widget_id = m_ptr->widget_id();
    Ice::Int widget_type = Widget2TypeCache::instance().GetWidgetType(widget_id);
  	if(2 == widget_type) {
			para.SetSelectType(SELECT_NEW_BIG);
	  }
	  if(1 == widget_type){
			para.SetSelectType(SELECT_NEW_SMALL);
	  }
  }
  return;
}

//下一个更精彩
void RefreshSelectHandle::handle(set<AdGroupPtr>& bid_gids,MatchHandlerParamter& para) {
//MCE_DEBUG("RefreshSelectHandle zone_id:" << para.adzone_->id() << " PickCount:"<< para.PickCount()<<" bid_gids:"<< bid_gids.size());
	if(!CheckNeedSelectMore(para)){
		return;
	}
	FilterAndGetCpmGroups(bid_gids,para);
	int count = para.PickCount();
	long adzone = para.adzone_->id();
	int pos = para.zone_selected_groups_.size() + 1;
	set<AdGroupPtr> groups_self;   //自助
	set<AdGroupPtr> groups_vip;   //vip 
	for(set<AdGroupPtr>::iterator itb = bid_gids.begin(); itb != bid_gids.end(); ++itb){
		Ice::Long widget_id = (*itb)->widget_id();
		Ice::Int widget_type = Widget2TypeCache::instance().GetWidgetType(widget_id);
		if((widget_type == 1) && (!IsCompetedGroup((*itb), para.all_selected_groups_))) {
			groups_self.insert(*itb);
		} else if((widget_type == 2) && (!IsCompetedGroup((*itb), para.all_selected_groups_))) { //vip新尺寸没有默认，先过滤竞品
			groups_vip.insert(*itb);
		}
	}
	if((groups_self.size() < 2) || ((groups_vip.size() > 0) && ((rand() % 2) == 1))) {
		bid_gids = groups_vip;
		count = 1; //只选择一个vip广告
		if(groups_vip.size() > 0) {
			para.SetSelectType(SELECT_VIP);
		}
	} else {
		bid_gids = groups_self;
	}
	vector<AdGroupPtr> selected_ids;
	selected_ids = RandomSelectAdGroup(para.userProfile_, bid_gids, para.all_selected_groups_ , adzone, count);
	for(vector<AdGroupPtr>::iterator its = selected_ids.begin(); its != selected_ids.end(); ++its) {
		AdGroupPtr m_ptr = *its;
		SelectedAdGroupPtr select = new SelectedAdGroup(m_ptr);
		select->cost_ = m_ptr->calculate_cost(adzone);
		select->pos_ = pos++;
		select->demo_state_ = 100000 + 3;
		para.all_selected_groups_.insert(m_ptr);
		para.zone_selected_groups_.insert(select);
//  MCE_DEBUG("[RefreshSelectHandle] ,uid=" << para.userProfile_.id() <<",zone:" << adzone << " pos:"<< select->pos_ <<" selected group:"<< m_ptr->group_id());
	}
}
}//namespace ad end
}//namespace xce end
