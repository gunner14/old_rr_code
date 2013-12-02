#include <ctemplate/template.h>
#include <curl/curl.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <algorithm>

#include "ServiceI.h"
#include "Handler.h"
#include "Util.h"
#include "CachePool.h"
#include "InvertedIndex.h"
#include "Config.h"
#include "AdMobLogBAdapter.h"
#include "AdMobLogSAdapter.h"
#include "AdMobLogRAdapter.h"

using namespace xce::ad;
using namespace boost;

/*
 *     Function: HandlerPara::set_seleted_group
 *  Description: 选到广告后对全局变量的填充
 */
inline void HandlerPara::set_seleted_group(Ice::Long group_id, AdGroupPtr group, AdType type, HandleFlagType handle_flag) {
	this->response_.adgroupId = group->group_id();
	this->selected_group_ptr_ = group;
	this->selected_group_type_ = type;
	this->handle_flag_ = handle_flag;
}

void HandlerPara::ParseDeviceType() {
  MCE_DEBUG("<" << request_.uid << "> HandlerPara::ParseDeviceType-->res2::" << this->request_.res2);
  string type = this->request_.res2;
  if (type.empty()) {
    this->dev_type_ = HandlerPara::DEV_UNKNOWN;
    return;
	}
  
	int typeInt = atoi(type.c_str());
  switch (typeInt) {
	  case 201:
	    this->dev_type_ = HandlerPara::IOS_IPAD;
	    break;
	  case 202:
      this->dev_type_ = HandlerPara::IOS_IPHONE;
  	  break;
	  case 203:
		  this->dev_type_ = HandlerPara::IOS_ITOUCH;
  	  break;
    case 200:
      this->dev_type_ = HandlerPara::IOS_UNKNOWN;
  		break;
    case 0:
  	  this->dev_type_ = HandlerPara::DEV_DEFAULT;
  	  break;
	  default:
  	  this->dev_type_ = HandlerPara::DEV_UNKNOWN;
  		break;
  }
}

/*
 *     Function: ParaCheckHandler::handle
 *  Description: 3G传入参数的合法性检查、用户信息获取、默认参数的构造
 */
void ParaCheckHandler::handle(HandlerPara& para) {
	AdMobReqPara& request = para.request_;
	MCE_DEBUG("<" << request.uid << "> ParaCheckHandler::handle --> Start");
	//Parameter 合法性检查
	if ( request.uid < 0 ) {
		request.uid = DEFAULT_USER_ID;
		MCE_INFO("<" << request.uid << "> ParaCheckHandler::handle --> unknown uid, set default:" << DEFAULT_USER_ID);
	} else {
		MCE_INFO("<" << request.uid << "> Get A Request From Mobile, zoneId:" << request.adzoneId
				          << " platform:" << request.client.osVersion);
	}

	AdZonePtr adzone = ZonePool::instance().GetAdZone(request.adzoneId);
	if ( !adzone ) {
		MCE_WARN("<" << request.uid << "> ParaCheckHandler::handle --> undefined zone id:" << request.adzoneId);
		request.adzoneId = MOB_ZONE_TOP;		//默认为顶部广告位
	}

	//用户信息获取
	string serialized;
	bool IsGetSuccess = false;
	int uid = request.uid;
	try {
		IsGetSuccess = UserProfileClient::instance().GetSerialized( uid, &serialized );
		if ( false == IsGetSuccess ) {
			MCE_INFO("<" << uid << "> ParaCheckHandler::handle --> profile not found in memcache. id = " << uid);
			serialized = AdUserCacheLoaderAdapter::instance().GetById( uid );
		}
	}catch(Ice::Exception& e) {
		MCE_WARN("<" << uid << "> ParaCheckHandler::handle --> AdUserCacheLoaderAdapter GetById error : " << e << " uid=" << uid);
	} catch(std::exception& e) {
		MCE_WARN("<" << uid << "> ParaCheckHandler::handle --> AdUserCacheLoaderAdapter GetById error : " << e.what() << " uid=" << uid);
	}


	UserProfile& profile = para.userProfile_;
	if ( true == serialized.empty() ) {
		profile.set_stage(4);
		profile.set_gender(0);
		profile.set_age(25);
		profile.set_school(0);
		profile.set_major(0);
		profile.set_grade(0);
		profile.set_home_area("0086000000000000");
		profile.set_current_area("0086000000000000");
		MCE_WARN("<" << uid << "> ParaCheckHandler::handle --> profile not found. uid = " << uid);
	} else {
		profile.ParseFromString(serialized);
	}
  
	if ( true == request.ipArea.empty() ) {
		profile.set_ip_area( "0086000000000000" );
	} else {
		profile.set_ip_area( request.ipArea );
	}
  
  if ( true == profile.current_area().empty() ) {
		profile.set_current_area("0086000000000000");
	}

	profile.set_id( request.uid );

	CheckRequestPara(request);
	//查看3G传入参数和UserProfile参数 用于问题追查
	PrintPara(request);
	PrintUserProfile(profile);
	PrintReqLog(para);
	MCE_DEBUG("<" << uid << "> ParaCheckHandler::handle --> End");
}


inline void ParaCheckHandler::CheckRequestPara(AdMobReqPara& request) {
	if ( true == request.netStatus.empty() ) {
		request.netStatus = "0100";
	} else {
		erase_all_regex(request.netStatus, boost::regex("[\n|\t|\r]"));
	}
	if ( true == request.client.model.empty() ) {
		request.client.model = "0";
	} else {
		replace_if(request.client.model.begin(), request.client.model.end(), IsToReplace, '.');
		erase_all_regex(request.client.model, boost::regex("[\n|\t|\r]"));
	}
	if ( true == request.client.osVersion.empty() ) {
    request.client.osVersion = "0";
	} else {
		erase_all_regex(request.client.osVersion, boost::regex("[\n|\t|\r]"));
		replace_if(request.client.osVersion.begin(), request.client.osVersion.end(), IsToReplace, '.');
	}
	if ( true == request.client.screenSize.empty() ) {
		request.client.screenSize = "320*480";
	}
	if ( true == request.client.version.empty() ) {
		request.client.version = "0.0.0";
	}
	if ( true == request.client.type.empty() ) {
		request.client.type = "000000000";
	}
	if ( true == request.client.mac.empty() ) {
		request.client.mac = "00:00:00:00:00:00";
	} else {
		erase_all_regex(request.client.mac, boost::regex("[\n|\t|\r]"));
		replace_if(request.client.mac.begin(), request.client.mac.end(), IsToReplace, '.');
	}
}

bool ParaCheckHandler::IsToReplace(char ch) {
	return ','==ch;
}

/*
 *  Func: ParaCheckHandler::PrintReqLog
 *  Desc: 3G请求信息输出，便于其他人员追查问题
 */
void ParaCheckHandler::PrintReqLog(const HandlerPara& para) {
  int print_flag = EngineConfig::instance().GetPropertyAsIntWithDefault("PRINT_REQLOG_FLAG", 0);
	if (0 == print_flag) {
	  return;
	}
	ostringstream log_buffer;
	//请求信息
	const AdMobReqPara& req = para.request_;
  log_buffer << "||" << req.uid << "," << req.requestTime << "," << req.adzoneId 
		 << "," << req.requestId << "," << req.netStatus << "," << req.appid << "," << req.sid
		 << "," << req.locationHistory << "," << req.ipArea << "," << req.locationRealTime.longitude
		 << "," << req.locationRealTime.latitude << "," << req.locationRealTime.precision;
	//客户端信息
	log_buffer << "||" << req.client.model << "," << req.client.uniqid << "," << req.client.osVersion
		<< "," << req.client.screenSize << "," << req.client.from << "," << req.client.version 
		<< "," << req.client.mac << "," << req.client.type;
	//用户信息
	const UserProfile& profile = para.userProfile_;
  log_buffer << "||" << profile.id() << "," << profile.gender() << "," << profile.grade()
		<< "," << profile.age() << "," << profile.current_area() << "," << profile.home_area()
		<< "," << profile.ip_area() << "," << profile.stage() << "," <<	profile.school();
  
	string log = log_buffer.str();
	try {
		AdMobLogRAdapter::instance().ReqLog(log);
	} catch (Ice::Exception& e) {
		MCE_WARN("<" << para.request_.uid << "> print mobile request log error -> " << e.what());
	} catch (std::exception& ex) {
    MCE_WARN("<" << para.request_.uid << "> print mobile request log exception error -> " << ex.what());
	} catch (...) {
	  MCE_WARN("<" << para.request_.uid << "> print mobile request log failed");
	}

}

/*
 *     Function: ParaCheckHandler::PrintPara
 *  Description: 3G传入参数打印到log，用于测试及问题追查的参数校验
 */
void ParaCheckHandler::PrintPara(AdMobReqPara& request) {
	int print_flag = EngineConfig::instance().GetPropertyAsIntWithDefault("PRINT_PARA_FLAG", 0);
	if (0 == print_flag) {
		return;
	}

	MCE_INFO("<" << request.uid << "> ParaCheckHandler::PrintPara --> Print Start");
	MCE_INFO("<" << request.uid << "> [PrintPara] uid:" << request.uid);                              //用户唯一标识
	MCE_INFO("<" << request.uid << "> [PrintPara] requestTime:" << request.requestTime);              //请求时间 yyyymmddhh24miss
	MCE_INFO("<" << request.uid << "> [PrintPara] adzoneId:" << request.adzoneId);                    //广告位唯一标识 网站8位+广告位4位 100000000099
	MCE_INFO("<" << request.uid << "> [PrintPara] requestId:" << request.requestId);                  //广告请求唯一标识 UID+时间 34326785520120308230121
	MCE_INFO("<" << request.uid << "> [PrintPara] netStaus:" << request.netStatus);           //AABB 网络状态 wifi:01 运营商移动:02  + 移动01 联通02 电信03
	MCE_INFO("<" << request.uid << "> [PrintPara] appid:" << request.appid);                          //应用编号131826
	MCE_INFO("<" << request.uid << "> [PrintPara] sid:" << request.sid);                              //用户身份密钥
	MCE_INFO("<" << request.uid << "> [PrintPara] location_history:" << request.locationHistory);     //历史位置数据
	MCE_INFO("<" << request.uid << "> [PrintPara] ipArea:" << request.ipArea);                        //用户的ip area code
	MCE_INFO("<" << request.uid << "> [PrintPara] location:" << " x:" << request.locationRealTime.longitude 
							 << " y:" << request.locationRealTime.latitude 
							 << " precision:" << request.locationRealTime.precision);		//实时位置数据
	//GroupIdSeq lastGroupIds;      //历史展示的广告
	MCE_INFO("<" << request.uid << "> [PrintPara] client.model:" << request.client.model);
	MCE_INFO("<" << request.uid << "> [PrintPara] client.uniqid:" << request.client.uniqid);
	MCE_INFO("<" << request.uid << "> [PrintPara] client.osVersion:" << request.client.osVersion);
	MCE_INFO("<" << request.uid << "> [PrintPara] client.screenSize:" << request.client.screenSize);
	MCE_INFO("<" << request.uid << "> [PrintPara] client.from:" << request.client.from);
	MCE_INFO("<" << request.uid << "> [PrintPara] client.version:" << request.client.version);
	MCE_INFO("<" << request.uid << "> [PrintPara] client.mac:" << request.client.mac);
	MCE_INFO("<" << request.uid << "> [PrintPara] client.type:" << request.client.type);
	MCE_INFO("<" << request.uid << "> ParaCheckHandler::PrintPara --> Print End");
}

inline void ParaCheckHandler::PrintUserProfile(UserProfile& profile) {
	int print_flag = EngineConfig::instance().GetPropertyAsIntWithDefault("PRINT_USERPROFILE_FLAG", 0);
	if (0 == print_flag) {
		return;
	}

	MCE_INFO("<" << profile.id() << "> ParaCheckHandler::PrintUserProfile --> Print Start");
	MCE_INFO("<" << profile.id() << "> [PrintUserProfile] uid:" << profile.id());
	MCE_INFO("<" << profile.id() << "> [PrintUserProfile] gender:" << profile.gender());
	MCE_INFO("<" << profile.id() << "> [PrintUserProfile] grade:" << profile.grade());
	MCE_INFO("<" << profile.id() << "> [PrintUserProfile] age:" << profile.age());
	MCE_INFO("<" << profile.id() << "> [PrintUserProfile] current area:" << profile.current_area());
	MCE_INFO("<" << profile.id() << "> [PrintUserProfile] home area:" << profile.home_area());
	MCE_INFO("<" << profile.id() << "> [PrintUserProfile] ip area:" << profile.ip_area());
	MCE_INFO("<" << profile.id() << "> [PrintUserProfile] stage:" << profile.stage());
	MCE_INFO("<" << profile.id() << "> [PrintUserProfile] school:" << profile.school());
	MCE_INFO("<" << profile.id() << "> ParaCheckHandler::PrintUserProfile --> Print End");
}

/*
 *     Function: UserBindHandler::handle
 *  Description: 大客户截屏功能
 */
void UserBindHandler::handle(HandlerPara& para) {
	MCE_DEBUG("<" << para.request_.uid << "> UserBindHandler::handle  --> Start");
	AdUserBindPtr userbind;
	userbind = UserBindPool::instance().GetUserBind( para.request_.uid, para.request_.adzoneId );
	if( userbind ) {
		AdGroupPtr group = NULL;
		Ice::Long group_id = userbind->creative_id()/100000;
		group = GroupPool::instance().GetAdGroup( group_id );
		if ( group ) {
			para.response_.adgroupId = group->group_id();
			para.selected_group_ptr_ = group;
			para.selected_creative_id_ = userbind->creative_id();
			para.selected_group_type_ = BRAND_AD;
			para.handle_flag_ = HandlerPara::HANDLED_BY_USERBIND;
		}
	}
	MCE_DEBUG("<" << para.request_.uid << "> UserBindHandler::handle  --> End");
}

/*
 *     Function: BrandRotateHandler::handle
 *  Description: 【品牌轮播广告】选择模块
 */
void BrandRotateHandler::handle(HandlerPara& para) {
	if ( para.handle_flag_ != HandlerPara::UNHANDLED ) {
		return;
	}
	MCE_DEBUG("<" << para.request_.uid << "> BrandRotateHandler::handle  --> Start");
	AdZonePtr zone = NULL;
	AdGroupPtr group = NULL;
	zone = ZonePool::instance().GetAdZone(para.request_.adzoneId);
	if( !zone ) {
		return;
	}
	int index = rand()%zone->GetRotateSize();
	group = zone->GetRotateGroups(index);
	if ( group ) {
		AdTargetUnitPtr target =  group->target_unit();
		if ( target ) {
			MCE_DEBUG("BrandRotateHandler::handle  --> Before Target Match Process, Rotate Group:" << group->group_id());
			if ( true == group->target_unit()->Match(para.userProfile_, para.request_) ) {	
				para.response_.adgroupId = group->group_id();
				para.selected_group_ptr_ = group;
        AdCreativePtr creative_ptr = group->creative();
        para.selected_creative_id_ = (creative_ptr ? creative_ptr->creative_id() : -1);
				para.selected_group_type_ = BRAND_AD;
				para.handle_flag_ = HandlerPara::HANDLED_BY_ROTATE;
				MCE_INFO("<" << para.request_.uid << "> BrandRotateHandler::handle --> Get Rotate:" << group->group_id());
			}
		} else {
			MCE_WARN("<" << para.request_.uid << "> BrandRotateHandler::handle --> cannot find target, groupid:" << group->group_id());
		}
	}
	MCE_DEBUG("<" << para.request_.uid << "> BrandRotateHandler::handle  --> End");
}

/*
 *     Function: BrandCpmCpcHandler::handle
 *  Description: 【品牌Cpm和Cpc广告】选择模块
 */
void BrandCpmCpcHandler::handle(HandlerPara& para) {
	MCE_DEBUG("<" << para.request_.uid << "> BrandCpmCpcHandler::handle  --> Start");
	if ( para.handle_flag_ != HandlerPara::UNHANDLED ) {
		return;
	}
	set<AdGroupPtr> selected;
	selected = GetAds( para , BRAND_INDEX );
	FilterGroups(selected, para.request_.lastGroupIds);
	if ( false == selected.empty() ) {
		AdGroupPtr group;
		RandomSelect(selected, group);
		if ( group ) {
			para.response_.adgroupId = group->group_id();
			para.selected_group_ptr_ = group;
      AdCreativePtr creative_ptr = group->creative();
      para.selected_creative_id_ = (creative_ptr ? creative_ptr->creative_id() : -1);
			para.selected_group_type_ = BRAND_AD;
			para.handle_flag_ = HandlerPara::HANDLED_BY_BRAND;
		}
	}
	MCE_DEBUG("<" << para.request_.uid << "> BrandCpmCpcHandler::handle  --> End");
}

/*
 *     Function: SelfHandler::handle
 *  Description: 【中小广告】选择模块
 */
void SelfHandler::handle(HandlerPara& para) {
	MCE_DEBUG("<" << para.request_.uid << "> SelfHandler::handle  --> Start");
	if ( para.handle_flag_ != HandlerPara::UNHANDLED ) {
		return;
	}
	set<AdGroupPtr> selected;
	selected = GetAds( para , SELF_INDEX );
	FilterGroups(selected, para.request_.lastGroupIds);
	if ( false == selected.empty() ) {
		AdGroupPtr group;
		RandomSelect(selected, group);
		if ( group ) {
			para.response_.adgroupId = group->group_id();
			para.selected_group_ptr_ = group;
      AdCreativePtr creative_ptr = group->creative();
      para.selected_creative_id_ = (creative_ptr ? creative_ptr->creative_id() : -1);
			para.selected_group_type_ = SELF_AD;
			para.handle_flag_ = HandlerPara::HANDLED_BY_SELF;
		}
	}
	MCE_DEBUG("<" << para.request_.uid << "> SelfHandler::handle  --> End");
}

/*
 *     Function: Selector::GetAds
 *  Description: 从倒排索引中选择候选广告
 */
set<AdGroupPtr> Selector::GetAds(HandlerPara& para, InvertedIndexType type) {
	set<AdGroupPtr> selected;
	InvertedIndex::instance().Get(para, para.request_.adzoneId, type, selected);
	return selected;
}

/*
 *     Function: Selector::RandomSelect
 *  Description: 随机竞价模块，按价格作为比例随机
 */
void Selector::RandomSelect(const set<AdGroupPtr>& groups, AdGroupPtr& result) {
	multimap<double, AdGroupPtr> candicates;
	set<AdGroupPtr>::const_iterator it = groups.begin();
	double sum = 0.0;
	for(; it != groups.end(); ++it) {
		double weight = ComputeWeight(*it);
		int print_flag = EngineConfig::instance().GetPropertyAsIntWithDefault("PRINT_RANPRICE_FLAG", 0);
    if (print_flag == 1){
			MCE_INFO("Selector::RandomSelect --> groupId:" << (*it)->group_id() << " weight:" << weight );
		}
		candicates.insert( make_pair(weight, *it) );
		sum += weight;
	}
	double random = ((double)(rand() % 10000)/10000.0) * sum;
	multimap<double, AdGroupPtr>::reverse_iterator itm = candicates.rbegin();
	bool selectFlag = false;
	for(; itm != candicates.rend(); ++itm) {
		random -= itm->first;
		if ( random < 0.00001 ) {
			result = itm->second;
			selectFlag = true;
			break;
		}
	}
	if ( false == selectFlag && false == candicates.empty() ) {
		result = candicates.rbegin()->second;
	}
}

/*
*  Function: Selector::ComputeWeight
*  Desc: 计算广告组的权重
*/
double Selector::ComputeWeight(const AdGroupPtr& adgroupPtr) {
	double discount_ratio = DiscountPool::instance().GetDiscountRatio(adgroupPtr->member_id());
	MCE_DEBUG("Selector::ComputeWeight --> memberId:" << adgroupPtr->member_id() 
			<< ", groupId:" << adgroupPtr->group_id() << ", discount_ratio:" << discount_ratio);
	if (CPC == adgroupPtr->trans_type()) {
		double weight = EngineConfig::instance().GetPropertyAsDoubleWithDefault("CPC_WEIGHT_LAMBADA", 3.0);
		return adgroupPtr->max_price() * weight * discount_ratio;
	}
	return adgroupPtr->max_price() * discount_ratio;
}

/*
 *     Function: Selector::FilterGroups
 *  Description: 竞品过滤
 */
void Selector::FilterGroups(set<AdGroupPtr>& selected, const vector<Ice::Long>& last_selected) {
	if (selected.empty()||last_selected.empty()) {
	  return;
	}
	set<Ice::Long> last;
	last.insert(last_selected.begin(), last_selected.end());
	FilterGroups(selected, last);
}

void Selector::FilterGroups(set<AdGroupPtr>& selected, const set<Ice::Long>& last_selected) {
	set<AdGroupPtr>::iterator first1 = selected.begin();
	set<AdGroupPtr>::iterator last1 = selected.end();
	set<Ice::Long>::const_iterator first2 = last_selected.begin();
	set<Ice::Long>::const_iterator last2 = last_selected.end();
	while ( first1 != last1 && first2 != last2 ) {
		 if ( (*first1)->group_id() < *first2 ) {
			++first1;
		} else if ( *first2 < (*first1)->group_id() ) {
			++first2;
		} else {
			int print_flag = EngineConfig::instance().GetPropertyAsIntWithDefault("PRINT_FLAST_FLAG", 0);
			if (print_flag == 1){
				MCE_INFO("Selector::FilterLastSelected --> erase adgroup:" << (*first1)->group_id());
			}
			selected.erase(first1++);
			++first2;
		}
	}
  
	if (selected.empty()) {
	  MCE_INFO("Selector::FilterLastSelected --> all adgroups are erased");
	}
}

/*
 *     Function: LogHandler::handle
 *  Description: Log模块，log的组装与pvlog、cpm_charge_log的输出
 */
void LogHandler::handle(HandlerPara& para) {
	MCE_DEBUG("<" << para.request_.uid << "> LogHandler::handle  --> Start");
	if ( para.handle_flag_ == HandlerPara::UNHANDLED ) {
		MCE_INFO("<" << para.request_.uid << "> [YELLOW] LogHandler::handle --> Get No Ads, uid:"<<para.request_.uid);
		return;
	} 

	BuildPvLog(para);
	if ( false == para.pv_log_.empty() ) {
		try {
			if ( BRAND_AD == para.selected_group_type_ ) {
				AdMobLogBAdapter::instance().Pv(para.pv_log_);
			} else if ( SELF_AD == para.selected_group_type_ ) {
				AdMobLogSAdapter::instance().Pv(para.pv_log_);
			} else {
				MCE_WARN("<" << para.request_.uid << "> LogHandler::handle --> unknown adgroup type:" << para.selected_group_type_);
			}
		} catch (Ice::Exception& e) {
			MCE_WARN("<" << para.request_.uid << "> print pv log error -> "<< para.selected_group_type_ << " " << e.what());
		} catch (std::exception& ex) {
			MCE_WARN("<" << para.request_.uid << "> print pv log exception error -> "<< para.selected_group_type_ << " "<< ex.what());
		} catch (...) {
			MCE_WARN("<" <<para.request_.uid << "> print pv log failed");
		}
	}

	BuildClickLog(para);
	if ( false == para.click_log_.empty() ) {
		para.response_.pageData.clickUrl += "&clicklog=";
		para.response_.pageData.clickUrl += para.click_log_;
	}
	if ( CPM == para.selected_group_ptr_->trans_type() ) {
		BuildCpmChargeLog(para);
		try {
			if ( BRAND_AD == para.selected_group_type_ ) {
				AdMobLogBAdapter::instance().CpmCharge(para.cpm_charge_log_);
			} else if ( SELF_AD == para.selected_group_type_ ) {
				AdMobLogSAdapter::instance().CpmCharge(para.cpm_charge_log_);
			} else {
				MCE_WARN("<" << para.request_.uid << "> LogHandler::handle --> unknown adgroup type:" << para.selected_group_type_);
			}
		} catch (Ice::Exception& e) {
			MCE_WARN("<" << para.request_.uid << "> print cpm Log error -> " << para.selected_group_type_ << " "<<e.what());
		} catch (std::exception& ex) {
			MCE_WARN("<" << para.request_.uid << "> print cpm Log exception error -> "<< para.selected_group_type_ << " "<<ex.what());
		} catch (...) {
			MCE_WARN("<" << para.request_.uid << "> print cpm Log failed");
		}
	} else if ( CPC == para.selected_group_ptr_->trans_type() ) {
		BuildCpcChargeLog(para);
	}

	MCE_DEBUG("<" << para.request_.uid << "> LogHandler::handle  --> End");
}

inline void LogHandler::BuildDefault3GLog(HandlerPara& para, string& log) {
	ostringstream logbuffer;
	const AdMobReqPara& request = para.request_;
	const AdMobClientInfo& client = request.client;
	const AdGroupPtr& group_ptr = para.selected_group_ptr_;
	const UserProfile& profile = para.userProfile_;
	try {
    AdCreativePtr creative_ptr = group_ptr->creative(para.selected_creative_id_);
    int widget_id = creative_ptr ? creative_ptr->widget_id() : -1; 
		logbuffer << EngineConfig::instance().server_name() << "," << profile.id() << "," << profile.stage() << "," << profile.age() << ","
			<< profile.gender() << "," << profile.grade() << "," 
			<< ((profile.current_area().empty()) ? "-1" : profile.current_area()) << "," 
			<< ((request.ipArea.empty()) ? "-1" : request.ipArea) << ","
			<< ((request.netStatus.empty()) ? "-1" : request.netStatus) << ","
			<< ((client.model.empty()) ? "-1" : client.model) << ","
			<< ((client.uniqid.empty()) ? "-1" : client.uniqid) << ","
			<< ((client.osVersion.empty()) ? "-1" : client.osVersion) << ","
			<< ((client.screenSize.empty()) ? "-1" : client.screenSize) << ","
			<< ((client.from.empty()) ? "-1" : client.from) << ","
			<< ((client.version.empty()) ? "-1" : client.version) << ","
			<< ((para.request_.client.type.empty()) ? "-1" : para.request_.client.type) << ","
			<< group_ptr->member_id() << "," << group_ptr->member_category() << "," << group_ptr->member_industry() << "," 
			<< para.selected_creative_id_ << "," << group_ptr->group_id() << "," << group_ptr->campaign_id() << "," 
			<< group_ptr->max_price() << ","  << "COST" << "," << group_ptr->display_type() << ","
			<< group_ptr->trans_type() << "," << "1" << "," << group_ptr->bid_unit_id() << "," << request.adzoneId << "," << "0" << ","
			<< "$DATE$" << "," << ((client.mac.empty()) ? "-1" : client.mac) << "," << widget_id << "," 
      << ((request.requestId.empty()) ? "-1" : request.requestId) << "," << "-1";
		log = logbuffer.str();
	} catch (Ice::Exception& e) {
		MCE_WARN("<" << para.request_.uid << "> LogHandler::BuildLog --> exception " << e.what());
	} catch (std::exception& ex) {
		MCE_WARN("<" << para.request_.uid << "> LogHandler::BuildLog --> exception error -> " << ex.what());
	} catch (...) {
		MCE_WARN("<" << para.request_.uid << "> LogHandler::BuildLog --> unknown exception");
	}
}

inline void LogHandler::BuildPvLog(HandlerPara& para) {
	MCE_DEBUG("<" << para.request_.uid << "> LogHandler::BuildPvLog --> call");
	BuildDefault3GLog(para, para.pv_log_);

  size_t date_pos = para.pv_log_.find("$DATE$");
  if (date_pos != string::npos) {
    MyUtil::Date date(time(NULL));
		para.pv_log_.replace(date_pos, 6, date.toDateTime());
	}

	size_t cost_pos = para.pv_log_.find("COST");
	if ( cost_pos == string::npos ) {
		para.pv_log_.erase();
		return;
	}

	if( para.selected_group_ptr_->display_type() == ROTATE3G ) {
		para.pv_log_.replace(cost_pos, 4, "0");
	} else if( para.selected_group_ptr_->trans_type() == CPC ) {
		para.pv_log_.replace(cost_pos, 4, "0");
	} else if( para.selected_group_ptr_->trans_type() == CPM ) {
		para.pv_log_.replace(cost_pos, 4, boost::lexical_cast<std::string>(para.selected_group_ptr_->cost()/1000) );
	} else {
		para.pv_log_.replace(cost_pos, 4, boost::lexical_cast<std::string>(para.selected_group_ptr_->cost()) );
	}
}

inline void LogHandler::BuildClickLog(HandlerPara& para) {
	MCE_DEBUG("<" << para.request_.uid << "> LogHandler::BuildClickLog --> call");
	BuildDefault3GLog(para, para.click_log_);
	size_t cost_pos = para.click_log_.find("COST");
	if ( cost_pos == string::npos ) {
		para.click_log_.erase();
		return;
	}
	if( para.selected_group_ptr_->display_type() == NORMAL3G && para.selected_group_ptr_->trans_type() == CPM ) {
		para.click_log_.replace(cost_pos, 4, "0");
	} else {
		para.click_log_.replace(cost_pos, 4, boost::lexical_cast<std::string>(para.selected_group_ptr_->cost()) );
	}
}

inline void LogHandler::BuildCpcChargeLog(HandlerPara& para) {
	MCE_DEBUG("<" << para.request_.uid << "> LogHandler::BuildCpcChargeLog --> call");
	//-----------------CPC CHARGE LOG--------------------
	//lineNO,creative_id,group_id,campaign_id,member_id,adzone_id,0(site_id),0(site_id),adzone_id,
	//实际扣费,点击时间,实际扣费,实际扣费,1,lm,系统最低出价,bidunit_id,ip,uid,实时反作弊标志[0、非0],
	//该记录所在时间段的起始时间,该记录所在时间段的结束时间
	ostringstream logbuffer;
	const AdMobReqPara& request = para.request_;
	const AdGroupPtr& group_ptr = para.selected_group_ptr_;
	const UserProfile& profile = para.userProfile_;
	try {
		logbuffer << para.selected_creative_id_ << "," << group_ptr->group_id() << "," << group_ptr->campaign_id() << ","
			<< group_ptr->member_id() << "," << request.adzoneId << "," << "0" << "," << "0" << ","
			<< request.adzoneId << "," << group_ptr->cost() << "," << "CLICKTIME" << "," << group_ptr->max_price() << "," 
			<< group_ptr->max_price() << "," << "1" << "," << "lm" << ","
			<< ZonePool::instance().GetAdZone(request.adzoneId)->min_price() << "," << group_ptr->bid_unit_id() << ","
			<< "0.0.0.0" << "," << profile.id() << "," << "0" << "," << "STARTTIME" << "," << "ENDTIME";
		para.cpc_charge_log_ = logbuffer.str();
	} catch (Ice::Exception& e) {
		MCE_WARN("<" << para.request_.uid << "> LogHandler::BuildCpc3GChargeLog --> exception "<<e.what());
	} catch (std::exception& ex) {
		MCE_WARN("<" << para.request_.uid << "> LogHandler::BuildCpc3GChargeLog --> exception error -> "<<ex.what());
	} catch (...) {
		MCE_WARN("<" << para.request_.uid << "> LogHandler::BuildCpc3GChargeLog --> unknown exception");
	}
}

inline void LogHandler::BuildCpmChargeLog(HandlerPara& para) {
	MCE_DEBUG("<" << para.request_.uid << "> LogHandler::BuildCpmChargeLog --> call");
	//-----------------CPM CHARGE LOG--------------------
	//lineNO, creativeID, adgroupID, campaignID, memberID,adzoneID, pvTime, pvPrice, bidUnitID, userID, cheatFlag
	ostringstream logbuffer;
	const AdMobReqPara& request = para.request_;
	const AdGroupPtr& group_ptr = para.selected_group_ptr_;
	const UserProfile& profile = para.userProfile_;
	MyUtil::Date date(time(NULL));
	try {
		logbuffer << para.selected_creative_id_ << "," << group_ptr->group_id() << "," << group_ptr->campaign_id() << ","
			<< group_ptr->member_id() << "," << request.adzoneId << "," << date.toDateTime() << "," << group_ptr->cost()/1000 << ","
			<< group_ptr->bid_unit_id() << "," << profile.id() << "," << "0";
		para.cpm_charge_log_ = logbuffer.str();
	} catch (Ice::Exception& e) {
		MCE_WARN("<" << para.request_.uid << "> LogHandler::BuildCpmChargeLog --> exception "<<e.what());
	} catch (std::exception& ex) {
		MCE_WARN("<" << para.request_.uid << "> LogHandler::BuildCpmChargeLog --> exception error -> "<<ex.what());
	} catch (...) {
		MCE_WARN("<" << para.request_.uid << "> LogHandler::BuildCpmChargeLog --> unknown exception");
	}
}

FormatHandler::FormatHandler() {
	ServiceI& service = ServiceI::instance();
	ClickUrlHead = service.getCommunicator()->getProperties()->getPropertyWithDefault("ClickUrlHead","http://admobile.renren.com/mclick/click?");
}

/*
 *     Function: FormatHandler::handle
 *  Description: 对即将返回给3G前端的数据格式化填充、分辨率匹配、图片处理、html生成、log参数追加
 */
void FormatHandler::handle(HandlerPara& para) {
	MCE_DEBUG("<" << para.request_.uid << "> FormatHandler::handle --> Start");
	para.response_.uid = para.request_.uid;
	para.response_.adzoneId = para.request_.adzoneId;
	para.response_.requestId = para.request_.requestId;
  
	string date_time;
	GetDateTime(date_time);
	para.response_.sendTime = atol(date_time.c_str());

	if ( HandlerPara::UNHANDLED == para.handle_flag_ ) {
		MCE_INFO("<" << para.request_.uid << "> [YELLOW] FormatHandler::handle --> [No selected adgroups] user:" << para.request_.uid);
		return;
	}
  //检验selected_group，creative有效性
	para.selected_group_ptr_ = GroupPool::instance().GetAdGroup(para.response_.adgroupId);
	if (false == para.selected_group_ptr_) {
	  MCE_WARN("FormatHandler::handle --> selected group is NULL");
		para.response_.adgroupId = -1;
		return;
	}
	AdCreativePtr creative = para.selected_group_ptr_->creative(para.selected_creative_id_);
	if (false == creative) {
	  MCE_WARN("FormatHandler::handle --> selected group's creative is NULL");
	  para.response_.adgroupId = -1;
		return;
	}

	//HTML模板填充
	ctemplate::TemplateDictionary dict("html"); 

  string pic_url;
  string pic_width;
  GetPicInfo(para, pic_url, pic_width);
  dict.SetValue("PIC_URL", pic_url);
  dict.SetValue("PIC_WIDTH", pic_width);
  
	string redirect_info;
	BuildClickUrlInfo(para, redirect_info);
	para.response_.pageData.clickUrl = "/redirect?url=" + redirect_info;
	dict.SetValue("AD_URL", para.response_.pageData.clickUrl );

	string output;
	ctemplate::ExpandTemplate("../etc/ad_mob_tpl/html.tpl", ctemplate::DO_NOT_STRIP, &dict, &output);
	para.response_.pageData.htmlData = output;
	
	MCE_DEBUG("<" << para.request_.uid << "> FormatHandler::handle --> End");
}

/*
 *  Function:FormatHandler::GetDateTime
 *  Desc: 获得符合格式要求的当前时间 
 *
 */
void FormatHandler::GetDateTime(string& date_time) {
 	MyUtil::Date date(time(NULL));
	timeval tv;
	gettimeofday(&tv,NULL);
	date_time = date.toDateTime("%Y%m%d%H%M%S");
	char milisecond[10];
	sprintf(milisecond,"%3d",(int)(tv.tv_usec));
	milisecond[3] = '\0';
	date_time += milisecond;
}

/*
 *    Function: FormatHandler::BuildClickUrlInfo
 * Description: 为页面生成clickUrl: 广告信息、跳转路径、结算log
 */
void FormatHandler::BuildClickUrlInfo(HandlerPara& para, string& redirect_info) {
  ostringstream url_buffer;
	url_buffer << ClickUrlHead << "uid=" << para.request_.uid 
		<< "&groupid=" << para.response_.adgroupId << "&grouptype=" << para.selected_group_type_
		<< "&displaytype=" << para.selected_group_ptr_->display_type()
		<< "&transtype=" << para.selected_group_ptr_->trans_type();

	//加入点击跳转url
	AdCreativePtr creative = para.selected_group_ptr_->creative(para.selected_creative_id_);
	string click_url = creative->GetValue("picClickUrl");
	
	if ( click_url.find("renren.com") != string::npos ) {
		para.response_.pageData.flag = "01";
		size_t pos = click_url.find("?");
		if ( pos == string::npos ) {
			click_url += "?sid=";
			click_url += para.request_.sid;
		} else if ( pos == click_url.size()-1 ) {
			click_url += "sid=";
			click_url += para.request_.sid;
		} else {
			click_url += "&sid=";
			click_url += para.request_.sid;
		}
	} else {
		para.response_.pageData.flag = "00";
	}
	
	string encode_buffer;
	UrlEncode(click_url, encode_buffer);
	url_buffer << "&clickurl=" << encode_buffer;

	encode_buffer.clear();
	UrlEncode(para.click_log_, encode_buffer);
	url_buffer << "&clicklog=" << encode_buffer;

	if ( CPC == para.selected_group_ptr_->trans_type() ) {
		encode_buffer.clear();
		UrlEncode(para.cpc_charge_log_, encode_buffer);
		url_buffer << "&cpcchargelog=" << encode_buffer;
	}

	url_buffer << "&res=xxx";

	string decode_url = url_buffer.str();
	UrlEncode(decode_url, redirect_info);
}

/*
 * Function: FormatHandler::GetPicInfo
 * Desc: 为HTML模板生成图片信息:路径，宽度
 *
 */
void FormatHandler::GetPicInfo(HandlerPara& para, string& pic_url, string& pic_width) {
	AdCreativePtr creative = para.selected_group_ptr_->creative(para.selected_creative_id_);

	short width = 0, fixed_width = 0;
	vector<string> fields;
	boost::algorithm::split_regex( fields, para.request_.client.screenSize, boost::regex("[\\*|X]"));
	if ( false == fields.empty() ) {
		width = atoi( fields.begin()->c_str() );
	}
	fixed_width = ResolutionMatcher::instance().GetFixedWidth(width);

	ostringstream pic_uri_key;
	pic_uri_key << "img" << fixed_width << "_url";
	MCE_DEBUG("<" << para.request_.uid << "> pic_uri_key:" << pic_uri_key.str());
	
	if ( false == creative->GetValue(pic_uri_key.str()).empty() ) {
		pic_url = MEDIA_URI+creative->GetValue(pic_uri_key.str()); 
	} else {
		pic_url = MEDIA_URI+creative->GetValue("picUrl");
		MCE_INFO("<" << para.request_.uid << "> [YELLOW] FormatHandler::handle --> Get Pic Url Error, pic_uri_key:" << pic_uri_key.str());
	}
	
	if ( fixed_width != width || 0 == width) {
		ostringstream picWidth;
		picWidth << "width=\"" << width << "\"";
	  pic_width = picWidth.str();
	}
}


/*
 *     Function: FormatHandler::UrlEncode
 *  Description: Url中的参数编码
 */
void FormatHandler::UrlEncode(const string& input, string& output) {
	CURL *handle = curl_easy_init();
	curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
	char *encodedURL = curl_easy_escape(handle, input.c_str(), 0);
	if (encodedURL){
		int flag = EngineConfig::instance().GetPropertyAsIntWithDefault("PRINT_ENCODEURL_FLAG", 0);
		if (flag == 1) {
			MCE_INFO("Encoded URL: " << encodedURL);
		}
		output = encodedURL;
		curl_free(encodedURL);
	} else {
		MCE_DEBUG("URL encoding failed");
	}
	curl_easy_cleanup(handle);
}

ZoneHandler::ZoneHandler() {
	handler_list_.push_back(new ParaCheckHandler());
	handler_list_.push_back(new UserBindHandler());
	handler_list_.push_back(new BrandRotateHandler());
	handler_list_.push_back(new BrandCpmCpcHandler());
	handler_list_.push_back(new SelfHandler());
	handler_list_.push_back(new LogHandler());
	handler_list_.push_back(new FormatHandler());
}

void ZoneHandler::handle(HandlerPara& para) {
	for(size_t i = 0; i < handler_list_.size(); ++i) {
		MCE_DEBUG("<" << para.request_.uid << "> ZoneHandler::handle --> call handle [i]:" << i);
		handler_list_[i]->handle(para);
	}
}

ZoneTestHandler::ZoneTestHandler() {
	handler_list_.clear();
	handler_list_.push_back(new ParaCheckHandler());
	handler_list_.push_back(new TestHandler());
}

void TestHandler::handle(HandlerPara& para) {
	MCE_DEBUG("TestHandler::handle  --> Start");
	int uid = para.request_.uid;
	para.response_.uid = para.request_.uid;
	para.response_.adzoneId = para.request_.adzoneId;
	para.response_.requestId = para.request_.requestId;

	para.response_.sendTime = time(NULL);

	int index = 1;
	map<int,int>::iterator it = test_map_.find(uid);
	if( it != test_map_.end() ) {
		index = (it->second)%11;
		++index;
		it->second = index;
		MCE_INFO("TestHandler::handle --> a call from uid:"<<uid<<" index:"<<index);
	} else {
		MCE_INFO("TestHandler::handle --> a call from uid:"<<uid<<" new call");
		test_map_.insert( pair<int,int> (uid,1) );
	}

	MCE_INFO("test_map.size is:"<<test_map_.size());
	ctemplate::TemplateDictionary dict("example");

	//dict.SetValue("AD_URL", "http://pagetab.yx.renren.com/wap20111216/index");

	ostringstream picUrl;
	int imageSize = 0;
	int imageRealSize;
	bool needCompress = false;
	ImageSelect(para.request_, imageSize, imageRealSize, needCompress);

	index = 5;

	//if ( index != 11) {
	//  picUrl << "http://10.3.20.204:8080/render/render/render/"<< imageSize <<"/conew_banner" << index << ".jpg";
	//} else {
	//  picUrl << "http://jebe.xnimg.cn/3gtest/" << imageSize << "/conew_banner1.jpg";
	//}
	picUrl << "http://jebe.xnimg.cn/3gtest/" << imageSize << "/conew_banner5.jpg";
	dict.SetValue( "PIC_URL", picUrl.str() );

	if ( 1 == index ) {
		dict.SetValue("AD_URL", "http://mini.renren.com/bocebanking/wap");
		para.response_.adgroupId=10000505720001;
	} else if ( 2 == index ) {
		dict.SetValue("AD_URL","http://mpage.renren.com/home/page.do?a=show&pageid=601091210");
		para.response_.adgroupId=10000505720002;
	} else if ( 3 == index ) {
		dict.SetValue("AD_URL", "http://mpage.renren.com/home/blog.do?a=show&id=813862079&pageid=601091210");
		para.response_.adgroupId=10000505720003;
	} else if ( 4 == index ) {
		dict.SetValue("AD_URL", "http://mpage.renren.com/home/album.do?a=show&pageid=601091210&albumid=582404348");
		para.response_.adgroupId=10000505720004;
	} else if ( 5 == index ) {
		dict.SetValue("AD_URL", "http://m.taobao.com/");
		para.response_.adgroupId=10000505720005;
	} else if ( 6 == index ) {
		dict.SetValue("AD_URL", "http://m.nuomi.com/voucher?code=MOBUnt&areaId=100010000");
		para.response_.adgroupId=10000505720006;
	} else if ( 7 == index ) {
		dict.SetValue("AD_URL", "http://3g.renren.com/place/index/poi_data_ugc_37636?htf=1057");
		para.response_.adgroupId=10000505720007;
	} else if ( 8 == index ) {
		dict.SetValue("AD_URL", "tel:01084481818");
		para.response_.adgroupId=10000505720008;
	} else if ( 9 == index ) {
		dict.SetValue("AD_URL", "http://itunes.apple.com/cn/app/ren-ren/id316709252?mt=8");
		para.response_.adgroupId=10000505720009;
	} else if ( 10 == index) {
		dict.SetValue("AD_URL", "http://mini.renren.com/bocebanking/wap_test");
		para.response_.adgroupId=10000505720010;
	} else if ( 11 == index) {
		dict.SetValue("AD_URL", "http://itunes.apple.com/cn/app/talkbox-voice-messenger/id412710421");
		para.response_.adgroupId=10000505720011;
	}

	ostringstream picWidth;
	picWidth << "width=\"" << imageRealSize << "\"";
	if ( true == needCompress ) {
		dict.SetValue( "PIC_WIDTH", picWidth.str() );
	}

	std::string output;
	ctemplate::ExpandTemplate("../etc/ad_mob_tpl/example.tpl", ctemplate::DO_NOT_STRIP, &dict, &output);

	para.response_.pageData.htmlData = output;
	para.response_.pageData.clickUrl = "http://admobile.renren.com/mclick/click?id=336101589";

	MCE_DEBUG("TestHandler::handle  --> End");
}


void TestHandler::ImageSelect(const AdMobReqPara& request, int& imageSize, int& imageRealSize, bool& needCompress) {
	vector<string> fields;
	try {
		MCE_INFO("screenSize"<<request.client.screenSize);
		split( fields, request.client.screenSize, is_any_of("*"));
		vector<string>::iterator it = fields.begin();
		imageRealSize = atoi( it->c_str() );
		int result = 960;
		vector<int>::reverse_iterator rit = imageWidthSeq_.rbegin();
		for(; rit != imageWidthSeq_.rend(); rit++) {
			if( imageRealSize < *rit) {
				result = *rit;
			} else if ( imageRealSize == *rit ) {
				result = *rit;
				needCompress = false;
				imageSize = result;
				break;
			} else {
				needCompress = true;
				imageSize = result;
				break;
			}
		}
	}catch(...) {
		MCE_WARN("split  error");
		imageSize = 960;
	}
}
