#ifndef AD_MACHER_HANDLERS_H_
#define AD_MACHER_HANDLERS_H_

#include <IceUtil/Shared.h>
#include <boost/algorithm/string/regex.hpp>
#include "AdEngine.h"
#include "AdStruct.h"
#include "AdConfig.h"
#include "../../util/src/AdCommonUtil.h"
#include "IndexService.h"
//#include "AdEffect.h"

#define BIG_WIDGET_TYPE 2
#define SMALL_WIDGET_TYPE 1
namespace xce {
namespace ad {

using namespace MyUtil;
using namespace std;
using namespace boost;

//NORMAL:默认 VIP:选择了vip大图   SMALLNEW:选择过新广告小图  BIGNEW:选择过新广告大图
enum SelectedType{SELECT_NORMAL=0, SELECT_VIP, SELECT_NEW_SMALL, SELECT_NEW_BIG};

struct UserIndexSelectPara {
	void Debug(){
		MCE_INFO("double_equal_ratio=" << double_equal_ratio << " ctr_alpha=" << ctr_alpha << " ctr_beta="<<ctr_beta <<" count=" <<count<<" use_lr_ctr=" <<(use_lr_ctr?1:0) <<" use_rt_ctr="<<(use_rt_ctr?1:0)<<" fresh_selected="<<(fresh_selected?1:0)<<" is_reserved="<< (is_reserved?1:0) << "lr_bayes_confidence_th" <<lr_bayes_confidence_th <<"lr_bayes_discount" <<lr_bayes_discount);
	}
	double double_equal_ratio;
	double lr_bayes_confidence_th;
	double lr_bayes_discount;
	double lr_confidence_th0;
	double lr_confidence_th1;
	double lr_weight_0;
	double lr_weight_1;
	double lr_weight_2;
	double ctr_alpha;
	double ctr_beta;
	int count;
	bool use_lr_ctr;
	bool use_rt_ctr;
	bool fresh_selected;//是否选出了新广告 
	bool is_reserved;
};

struct MatchHandlerParamter {
	MatchHandlerParamter(const UserProfile& userProfile, const string& append_para): userProfile_(userProfile), rotate_index_(0), 
	has_re_(0), tag_(""), photo_(false), count_(0),selected_type_(SELECT_NORMAL) {
		vector < string > para_strs;
		boost::algorithm::split_regex(para_strs, append_para, regex("\\^C"));
		if (para_strs.size() >= 3) {
			referer_ = para_strs[0];
			next_load_time_ = para_strs[1];
			refresh_idx_ = para_strs[2];
			if (para_strs.size() >= 4) {      
				refresh_source_ = para_strs[3]; 
			}  
		} else if (para_strs.size() >= 1) {
			referer_ = para_strs[0];
		}
		vector < string > referer_strs;   //add 2012.05.27
		vector < string > refresh_strs;
		boost::algorithm::split_regex(referer_strs, referer_, regex("refresh_idx="));
		if (referer_strs.size() > 1) {
			boost::algorithm::split_regex(refresh_strs , referer_strs[1], regex("&"));
			if (refresh_strs.size() > 0) {
				refresh_idx_ = refresh_strs[0];
			}
		}
		referer_strs.clear();
		refresh_strs.clear();
		boost::algorithm::split_regex(referer_strs, referer_, regex("refresh_source="));
		if (referer_strs.size() > 1) {
			vector < string > refresh_strs;
			boost::algorithm::split_regex(refresh_strs , referer_strs[1], regex("&"));
			if (refresh_strs.size() > 0) {
				refresh_source_ = refresh_strs[0];
			}
		}
		//MakeSessionId
		struct timeval t;
		gettimeofday(&t, NULL);
		int rand_len = 20;//20bit放随机数 
		Ice::Long time_field = (Ice::Long)(t.tv_sec) * 1000L + t.tv_usec / 1000L;//毫秒数 
		session_id_ =  ((time_field << rand_len) | (rand() & ((1 << rand_len) - 1)));   
	};

	int count() const {
		if (EngineConfig::instance().IsForBrand()) {
			return adzone_->brand_count();
		} else if(photo_ && adzone_->id() == 100000000072) {
			return EngineConfig::instance().photo_wb1_count();
		} else if(count_ && adzone_->id() == 100000000063) {
			return count_;
		} else {
			int cnt = adzone_->bp_count();
			if( style_ ){
				cnt = style_->ad_cnt();
			}
			return cnt;
		}
	}

	unsigned int PickCount() const {  // 返回还需要选取的广告数
		return count() - zone_selected_groups_.size();
	}

	SelectedType GetSelectedType() const {
		return selected_type_;
	}

	UserIndexSelectPara SelectPara(){
		return select_para_;
	} 

	void SetSelectType(const SelectedType & type){
		selected_type_ = type;
	}

	string OutPutPara(){
		ostringstream strpara;
		strpara << "userID:" << userProfile_.id() << " adzone:" << adzone_->id() << " need_count:"<< count() << " selected_type: "<< selected_type_ << " selected size:"<< zone_selected_groups_.size() << " grouplist:";
		set<SelectedAdGroupPtr>::iterator it = zone_selected_groups_.begin();
		for(; it != zone_selected_groups_.end(); ++it){
			strpara << (*it)->group_->group_id() << " ";
		}
		return strpara.str();
	}

	UserProfile userProfile_;
	string referer_;
	string next_load_time_;
	string refresh_idx_;
	string refresh_source_; //add 2012.05.27
	AdStylePtr style_;
	AdZonePtr adzone_;
	int rotate_index_;
	set<SelectedAdGroupPtr> total_selected_groups_;
	set<SelectedAdGroupPtr> zone_selected_groups_;
	set<AdGroupPtr> all_selected_groups_;
	vector<SelectedAdGroupPtr> final_selected_groups_;
	bool has_re_;
	string tag_;
	bool photo_;
	int count_;
	Ice::Long session_id_;
	GroupDict group_info_dict_;
	SelectedType selected_type_;
	UserIndexSelectPara  select_para_;
	set<AdGroupPtr> default_selected_groups_;
};

class AdMatchHandler : public IceUtil::Shared {
	public:
		virtual bool handle(MatchHandlerParamter& para, AdResultMap& result) = 0;
};

class DefaultHandler : public AdMatchHandler {
	public:
		virtual bool handle(MatchHandlerParamter& para, AdResultMap& result);
};

class PosAdjustHandler : public AdMatchHandler {
	public:
		virtual bool handle(MatchHandlerParamter& para, AdResultMap& result);
};

class StyleHandler : public AdMatchHandler {
	public:
		virtual bool handle(MatchHandlerParamter& para, AdResultMap& result);
	public :
		bool isBig(int widget_id);
};

class LogHandler : public AdMatchHandler {
	public:
		virtual bool handle(MatchHandlerParamter& para, AdResultMap& result);
};

}
}

#endif // AD_MACHER_HANDLERS_H_
