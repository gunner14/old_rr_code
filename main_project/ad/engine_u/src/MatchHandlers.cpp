#include "MatchHandlers.h"
#include "AdMatchManagerI.h"
#include "Log.h"
#include "Date.h"
#include "AdLogSReplicaAdapter.h"
#include "UserRankHandler.h"
using namespace xce::ad;

namespace xce {
namespace ad {

bool DefaultHandler::handle(MatchHandlerParamter& para, AdResultMap& result) {
	if(para.PickCount() <= 0 || para.GetSelectedType() == SELECT_VIP || para.GetSelectedType() == SELECT_NEW_BIG){
		return true;
	}
	set<AdGroupPtr> default_groups;
	//获取广告位上的默认选择逻辑
	vector<int> defaults = StrategyConfig::instance().GetDefaultVecByAdzone(para.adzone_->id());
	if (defaults.size() != 1) {
		MCE_WARN("DefaultHandler config wrong!");
		return true;
	}
	int config = *(defaults.begin());
	//获取广告位上的默认广告集合
	vector<AdGroupPtr> groups = AdZonePool::instance().GetDefaultGroups(para.adzone_->id());
	vector<AdGroupPtr> groups_big;   
	vector<AdGroupPtr> groups_small;        
	for(vector<AdGroupPtr>::iterator itg = groups.begin(); itg != groups.end(); ++itg) {
		Ice::Long widget_id = (*itg)->widget_id();
		if(Widget2TypeCache::instance().GetWidgetType(widget_id) == BIG_WIDGET_TYPE) {  //VIP
			groups_big.push_back(*itg);
			MCE_DEBUG("WIDGET_TYPE: adgroup_id:" << (*itg)->group_id() <<" widget_id:"<< widget_id << " TYPE2");
		} else if( Widget2TypeCache::instance().GetWidgetType(widget_id) == SMALL_WIDGET_TYPE) {                               //小图
			groups_small.push_back(*itg);
			MCE_DEBUG("WIDGET_TYPE: adgroup_id:" << (*itg)->group_id() <<" widget_id:"<< widget_id << " TYPE1");
		} else {
			MCE_WARN("WIDGET_TYPE: adgroup_id:" << (*itg)->group_id() <<" widget_id:"<< widget_id << " TYPE3");
		}        
	}
	if (config == BOTH) {
		if(para.zone_selected_groups_.size() > 0)	{
			AdZonePool::instance().GetDefaultGroupsCompete(para.adzone_->id(), para.PickCount(), para.default_selected_groups_, groups_small, default_groups);
		}else{
			if( 1 == rand()%2 ){
				AdZonePool::instance().GetDefaultGroupsCompete(para.adzone_->id(),1, para.default_selected_groups_, groups_big, default_groups);
			}else{
				AdZonePool::instance().GetDefaultGroupsCompete(para.adzone_->id(), para.PickCount(), para.default_selected_groups_, groups_small, default_groups);
			}
		}
	} else if (config == BIG) {
		AdZonePool::instance().GetDefaultGroupsCompete(para.adzone_->id(), 1, para.default_selected_groups_, groups_big, default_groups);
	} else if (config == SMALL) {
		AdZonePool::instance().GetDefaultGroupsCompete(para.adzone_->id(), para.PickCount(), para.default_selected_groups_, groups_small, default_groups);
	}
	for(set<AdGroupPtr>::iterator it = default_groups.begin(); it != default_groups.end(); ++it) {
		SelectedAdGroupPtr p = new SelectedAdGroup(*it);
		p->pos_ = para.zone_selected_groups_.size() + 1;  //cost = 0
		para.zone_selected_groups_.insert(p);
		para.total_selected_groups_.insert(p);
		para.default_selected_groups_.insert(*it);
	}
	MCE_INFO("DefaultHandler::handle Selected Result:"<< para.OutPutPara());
	return true;
}

// 调整广告展示位置
bool PosAdjustHandler::handle(MatchHandlerParamter& para, AdResultMap& result) {
  TimeStat ts;
  // 调整广告展示的位置
  vector<SelectedAdGroupPtr> zone_selected(para.zone_selected_groups_.begin(), para.zone_selected_groups_.end());
  MCE_DEBUG("PosAdjustHandler::handle Selected ads num: " <<  zone_selected.size());
  /*  if(para.adzone_->id() == 100000000001) {
      Adjust(zone_selected, para.zone_selected_groups_);
    } else if ((para.adzone_->id() == 100000000063) && para.has_re_) {
      AdjustForInner(zone_selected, para.zone_selected_groups_);
    }*/
   //调整后的
  para.final_selected_groups_.swap(zone_selected);
  STATISTICS("PosAdjustHandler::handle --> time = " << ts.getTime());
  return true;
}

//样式实验进行样式替换
bool StyleHandler::isBig( int widget_id){
	return widget_id == EngineConfig::instance().big_pic_id();
}
bool StyleHandler::handle(MatchHandlerParamter& para, AdResultMap& result) {
  if( (para.GetSelectedType() == SELECT_VIP || para.GetSelectedType() == SELECT_NEW_BIG) && (100000000070 == para.adzone_->id() || 100000000072 == para.adzone_->id() ||
    100000000065 == para.adzone_->id() || 100000000093 == para.adzone_->id()) ) {
    para.SetSelectType(SELECT_NORMAL);
    return true;
  }
  TimeStat ts;
  MCE_DEBUG("StyleHandler::handle() --> user_id = " << para.userProfile_.id()
            << ", adzone_id = " << para.adzone_->id()
            << ", para.final_selected_groups_.size() = " << para.final_selected_groups_.size());
  AdStylePtr style = para.style_;
  if( style ){
    MCE_DEBUG("style replace with styleId=" << style->style_id());
    size_t abs_pos = 0;
    vector<int> bigs = style->big_widgets();
    vector<int> smalls = style->small_widgets();
    for(vector<SelectedAdGroupPtr>::iterator sit = para.final_selected_groups_.begin();
      sit != para.final_selected_groups_.end(); ++sit, ++abs_pos){
      AdGroupPtr group_ptr = (*sit)->group_;
      if(group_ptr->member_id() == 999) {
        continue;
      }

	  (*sit)->selected_creative_id_ = group_ptr->creative_id_;
	  int widget_id = group_ptr->widget_id_;

      int newId = 0;
      if( StyleHandler::isBig(widget_id) ){
    	if( abs_pos < bigs.size() && bigs[abs_pos] > 0 ) {
          newId = bigs[abs_pos];
        }
      }else{
        if( abs_pos < smalls.size() && smalls[abs_pos] > 0) { 
          newId = smalls[abs_pos];
        }
      }
      MCE_DEBUG("adgroup_id " << group_ptr->group_id() << " widget replace: " << widget_id <<" --> " << newId);
      if( newId != 0 ){
        (*sit)->selected_widget_id_ = newId;
        (*sit)->extra_ = "style_test";
      }
    }
  }
  STATISTICS("StyleHandler::handle --> time = " << ts.getTime());
  return true;
}

bool sortByPos(const SelectedAdGroupPtr& g1, const SelectedAdGroupPtr& g2) {
    return g1->pos_ < g2->pos_;
}
bool LogHandler::handle(MatchHandlerParamter& para, AdResultMap& result) {
  TimeStat ts;
  TimeStat subts;
  AdInfoSeq ad_info_seq;

  ::MyUtil::StrSeq pv_log_seq;
  //::MyUtil::StrSeq engine_log_seq;
  ::MyUtil::StrSeq cpm_log_seq;

  UserProfile & profile = para.userProfile_;
  ostringstream log;
  //ostringstream engine_log;
  ostringstream cpm_log;
  MyUtil::LongSeq group_id_seq;
  vector<SelectedAdGroupPtr> zone_selected  =para.final_selected_groups_;
  std::sort(zone_selected.begin(), zone_selected.end(), sortByPos);
  for(vector<SelectedAdGroupPtr>::iterator sit = zone_selected.begin();
      sit != zone_selected.end(); ++sit){
    AdInfo ad_info;
    MyUtil::Date date(time(NULL));
    AdGroupPtr group_ptr = (*sit)->group_;
    group_id_seq.push_back(group_ptr->group_id());
    ad_info.creativeid = group_ptr->creative_id_;
    ad_info.groupid = group_ptr->group_id();
    ad_info.updateTime = 0;
    LogParas log_para;
    log_para.cost_ = (*sit)->cost_;
    log_para.user_profile_ = para.userProfile_; //profile;
    log_para.pv_time_ = Date(time(NULL)).time();
    log_para.creative_id_ = ad_info.creativeid;
    log_para.zone_id_ = para.adzone_->id();
    log_para.max_price_ = lexical_cast<int>( group_ptr->max_price() );
    log_para.ad_count_ = para.count();
    log_para.next_load_time_ = para.next_load_time_;
    log_para.refresh_idx_ = para.refresh_idx_;
	  log_para.member_id_ = group_ptr->member_id_;
	  log_para.campaign_id_ = group_ptr->campaign_id_;
	  log_para.display_type_ = group_ptr->display_type_;
	  log_para.bid_unit_id_ = group_ptr->bid_unit_id_;
	  log_para.trans_type_ = group_ptr->trans_type_;
    Ice::Long group_id = ad_info.creativeid / 100000;
    ostringstream append_info;
    if(!para.tag_.empty()) {
      append_info << para.tag_;
    } else {
      append_info << "rr_0";
    }
    //展示的绝对位置
    log_para.pos_ = (*sit)->pos_;
    log_para.demo_state_ = (*sit)->demo_state_;
    log_para.demo_ctr_ = (*sit)->demo_ctr_;
    log_para.widget_id_ = (*sit)->selected_widget_id_;    
    append_info << '_' << log_para.widget_id_;
    log_para.append_info_ = append_info.str();
    log_para.session_id_ = para.session_id_;
    log_para.demo_ctr_ = LogHelper::instance().TransCtr(log_para); //隐藏点击率
    ad_info.text = LogHelper::instance().MakeText(log_para);
    if(!(*sit)->extra_.empty()) {
      ad_info.updateTime = -1;
    } else {
      ad_info.updateTime = 0;
    }
    ad_info_seq.push_back(ad_info);

    //call AdLogReplicaAdapter
    Ice::Long zone_id = log_para.zone_id_;
    Ice::Long creativeid = log_para.creative_id_;
    string http = LogHelper::instance().MakeHttp(log_para);
    string ip = LogHelper::instance().MakeIp(profile.ip());
    int uid = profile.id();

    string deal_referer = para.referer_;
    LogHelper::instance().Process(deal_referer);
    MCE_DEBUG("LogHandler::handle-->gid:"<<(*sit)->group_->group_id()<<",pos:" <<(*sit)->pos_);

    //int sort_type = EngineConfig::instance().sort_type();
    int sort_type = EngineConfig::instance().server_index();
    //transtype修改，现在写的是数据库里读出来的，不再是写死的
    //display_type:1=轮播, 2= cpm ,3=余量, 4= cpc, 5=产品
    double cost = log_para.cost_;
    if(group_ptr->IsCpm()){
      log << setprecision(5);
    } else if(group_ptr->IsCpc()){//cpc的pv cost=0，cpm为相应扣费
      log << setprecision(0);
      cost = 0;
    }

    log << EngineConfig::instance().server_name() << "||" << ip << "||" << date.toDateTime() << "||\"GET "
        << http << " HTTP/1.0\"||200||undefined||" << log_para.session_id_ << "||undefined||" << group_ptr->trans_type() << "||" << sort_type << "||none||"
        << deal_referer << "||" << zone_id << "^B^B^B^B^B^B^B||" << creativeid << "^B" << group_ptr->member_id()
        << "^B" << group_ptr->bid_unit_id() << "^B" <<  setiosflags(ios::fixed) << cost  << setiosflags(ios::scientific) << setprecision(6)
        << "^B" << group_ptr->max_price() << "^B" << deal_referer
        << "^B" << log_para.pos_ <<"^B" << log_para.demo_state_ << "^B2^B" << setprecision(8) << log_para.demo_ctr_<<"||" << group_ptr->display_type();
    if((!log_para.append_info_.empty()) && (log_para.append_info_.compare("-") != 0)) {
      log << "||" << log_para.append_info_;
    }
    pv_log_seq.push_back(log.str());
    log.str("");

    if(group_ptr->IsCpm()){//cpm
     //cpm价格具体逻辑写在caculate_cost里面，要/1000
      cpm_log << log_para.creative_id_ << ',' << group_id << ',' 
              << group_ptr->campaign_id() << ',' << group_ptr->member_id() << ',' << log_para.zone_id_ << ','
	      << LogHelper::instance().FormatedClickTime(time(NULL)) << ',' << setiosflags(ios::fixed) << setprecision(5)
	      << log_para.cost_ << ',' << group_ptr->bid_unit_id() << ',' << uid << ',' << '0';
      cpm_log_seq.push_back(cpm_log.str());
      cpm_log.str("");
    }
  }
  /*engine_log << "pvlog=1\tuserid=" << profile.id() << "\tage="<< profile.age()
    << "\tgender="<< profile.gender() << "\tstage=" << profile.stage()
    << "\tgrade=" << profile.grade() << "\tcurrent_area="
    <<  ((profile.current_area().empty()) ? "-" : profile.current_area())
    << "\tip_area=" << ((profile.ip_area().empty()) ? "-" : profile.ip_area())
    << "\tschool=" << profile.school() << "\tzone_id=" << para.adzone_->id()
    << "\tgroup_id=" << group_ptr->group_id() << "\tmax_price=" << group_ptr->max_price(zone_id)
    << "\tapp_id=" << LogHelper::instance().GetAppId(para.referer_)
    << "\tmember_id=" << group_ptr->member_id() << "\tsystem_time=" << date.toDateTime();
    engine_log_seq.push_back(engine_log.str());
    engine_log.str("");*/

  result[para.adzone_->id()] = ad_info_seq;
  try {
    subts.reset();
    AdLogReplicaAdapter::instance().PvBatch(pv_log_seq);
    STATISTICS("LogHandler::handle --> TIME_TEST write pv_log : " << subts.getTime());
  } catch (Ice::Exception& e) {
    MCE_WARN("LogHandler::handle AdLogReplicaAdapter.PvBatch ERR line:" << __LINE__ << " err:" << e);
  } catch (std::exception& e) {
    MCE_WARN("LogHandler::handle AdLogReplicaAdapter.PvBatch ERR line:" << __LINE__ << " err:" << e.what());
  }
  ////////////////
  if(!cpm_log_seq.empty()){//cpm单独打charge_pv
    try {
      AdLogReplicaAdapter::instance().PvForCharge(cpm_log_seq);
    } catch (Ice::Exception& e) {
      MCE_WARN("LogHandler::handle AdLogReplicaAdapter.CpmBatch ERR line:" << __LINE__ << " err:" << e);
    } catch (std::exception& e) {
      MCE_WARN("LogHandler::handle AdLogReplicaAdapter.CpmBatch ERR line:" << __LINE__ << " err:" << e.what());
    }
  }

  /*try {
    subts.reset();
    AdLogReplicaAdapter::instance().EngineLogBatch(engine_log_seq);
    STATISTICS("LogHandler::handle --> TIME_TEST write engine_log : "
    << subts.getTime());
    } catch (Ice::Exception& e) {
    MCE_WARN("LogHandler::handle AdLogReplicaAdapter.EngineLogBatch ERR line:"
    << __LINE__ << " err:" << e);
    } catch (std::exception& e) {
    MCE_WARN("LogHandler::handle AdLogReplicaAdapter.EngineLogBatch ERR line:"
    << __LINE__ << " err:" << e.what());
    }*/

  //STATISTICS("LogHandler::handle --> time = " << ts.getTime());
  return true;
}

}
}

