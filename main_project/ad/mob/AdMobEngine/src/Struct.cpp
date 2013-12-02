#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <json/json.h>

#include "Struct.h"
#include "Translator.h"
#include "Config.h"
#include <string>

using namespace std;
using namespace MyUtil;
using namespace xce::ad;

bool AdMember::HaveMoney() {
	return have_money_ & 1;
}

string xce::ad::AdMember::ToString() const {
	ostringstream value;
	value <<"[member] --> " << " member_id:" << member_id() << " have_money:"
				<< have_money() << " category:" << category();
	return value.str(); 
}

string AdCampaign::ToString() const {
	ostringstream value;
	value <<"[campaign] --> " << " campaign_id:" << campaign_id() << " member_id:"
				<< member_id() << " begin_time:" << begin_time() << " end_time:"<<end_time();
	return value.str();
}

bool AdCreative::operator<( const AdCreative& r) const {
	return this->creative_id() < r.creative_id();
}

/*
 *  Function: AdCreative::ParseWidget
 *  Description: widget解析
 */
bool AdCreative::ParseWidget(const string& json_str) {
	Json::Value value;
	Json::Reader reader;
	Json::Value::iterator iter;
	Json::Value::Members members;
	bool parsingSuccessful = reader.parse( json_str, value );
	if (parsingSuccessful) {
		members = value.getMemberNames();
		try {
			for(Json::Value::Members::iterator it = members.begin();it != members.end(); ++ it) {
        std::string name = *it;
        std::string value_str = value[name].asString();
				widgetMap[name] = value_str;
			}
		} catch(std::exception& e) {
			MCE_WARN("AdCreative::ParseWidget --> JsonToMap boost::lexical_cast error :" << e.what()
          << " json_str:" << json_str);
			return false;
		} catch(...) {
			MCE_WARN("AdCreative::ParseWidget --> JsonToMap boost::lexical_cast error");
			return false;
		}
	}
	return true;
}

string AdCreative::GetValue(const string& key) {
  map<string,string>::iterator it = widgetMap.find(key);
  if (it == widgetMap.end()){
    return "";
	}
  return it->second;
}

void AdGroup::add_adzone_id(Ice::Long adzone_id) {
	adzone_id_set_.insert( adzone_id );
}

set<Ice::Long> AdGroup::adzone_ids() {
	return adzone_id_set_;
}

bool AdGroup::Available() {
	bool result = false;
	result = member_table_status_ && campaign_status_ &&  point_valid_;
	return result;
}

string AdGroup::ToString() const {
	ostringstream value;
	value <<"[group] --> " << " group_id:" << group_id() << " bid_unit_id:"
				<< bid_unit_id() << " member_id:" << member_id() << " campaign_id:"<<campaign_id()
				<< " display_type:"<< display_type() << " trans_type:"<<trans_type() <<" max_price:"
				<< max_price() << " campaign_status:"<<campaign_status()<<" member_table_status:"
				<< member_table_status() << " member_category:"<<member_category() <<" point_valid:"<<point_valid();
	return value.str();
}

bool AdGroup::operator<( const AdGroup& r ) const{
	return this->group_id() < r.group_id();
}

AdTargetUnitPtr AdGroup::target_unit() {
	return target_unit_;
}

void AdGroup::set_target_unit(AdTargetUnitPtr target_unit) {
	target_unit_ = target_unit;
}

Ice::Long AdGroup::creative_id() {
  if (false == creative_seq_.empty()){
    return (*(creative_seq_.begin()))->creative_id();
  } else {
    return 0;
  }
}

void AdGroup::add_creative(AdCreativePtr creative) {
  creative_seq_.push_back(creative);
}

bool AdGroup::have_creative() {
  return false == creative_seq_.empty();
}

AdCreativePtr AdGroup::creative() {
	if( false == creative_seq_.empty() ){
		if ( ROTATE3G == display_type() ) { //轮播广告随机出
			int random = rand() % creative_seq_.size();
			if (creative_seq_[random]) {
				return creative_seq_[random];
			}
		} else { //自助广告出第一个
			if (*creative_seq_.begin()) {
				return *creative_seq_.begin();
			}
		}
	}
	return NULL;
}

AdCreativePtr AdGroup::creative(Ice::Long creative_id) {
	if( false == creative_seq_.empty() ){
		vector<AdCreativePtr>::const_iterator it = creative_seq_.begin();
		for(; it != creative_seq_.end(); ++it) {
			if( (*it)->creative_id() == creative_id ) {
				return *it;
			}
		}
	}
	return NULL;
}

double AdGroup::cost() {
	return max_price();
}

/*double AdGroup::weight() {
	if ( CPC == trans_type() ) {
		return max_price() * EngineConfig::instance().GetPropertyAsDoubleWithDefault("CPC_WEIGHT_LAMBADA", 3.0);
	}
	return max_price();
}*/


bool AdZone::AddGroupToRotateList(int rotate_index, const AdGroupPtr& group) {
	if ( rotate_index >= 0 && rotate_index < boost::lexical_cast<int>(rotate_list_.size()) ) {
		rotate_list_[rotate_index] = group;
		return true;
	}
	return false;
}

AdGroupPtr AdZone::GetRotateGroups(int rotate_index) {
	AdGroupPtr ans = NULL;
	if ( rotate_index >= 0 && rotate_index < boost::lexical_cast<int>(rotate_list_.size()) ) {
		ans = rotate_list_[rotate_index];
		if (ans) {
			return ans;
		} else {
			MCE_DEBUG("AdZone::GetRotateGroups --> index = " << rotate_index << " no rotate ad");
		}
	}
	return ans;
}

void AdZone::GetAllRotateGroups(map<AdGroupPtr, int>& result) {
	for(size_t i=0; i<rotate_list_.size(); ++i) {
		if ( result.find(rotate_list_[i]) != result.end() ) {
			result[rotate_list_[i]] += 1;
		} else {
			result[rotate_list_[i]] = 1;
		}
	}
}

void AdZone::ResizeRotateList(int size) {
	rotate_list_.resize(size);
}

void AdZone::RandomRotateList() {
	//有些广告需要放在头部，用户在第一次pv时展示
	if(member_type() == 1 && rotate_list_.size() > 0) {
		int num = rand()%rotate_list_.size();
		for(int i = 0; i < num; ++i) {
			AdGroupPtr group = rotate_list_.front();
			rotate_list_.push_back(group);
			rotate_list_.erase(rotate_list_.begin());
		}
	}
}

int AdZone::GetRotateSize() const {
	return rotate_list_.size();
}

string AdZone::ToString() const {
	ostringstream value;
	value <<"[zone] --> " << " zone_id:" << zone_id() << " count:"
				<< brand_count() << " priority:" << priority();
	return value.str();
}

bool AdUserBind::operator <(const AdUserBind& r) const {
	if( this->user_id() != r.user_id() )
		return this->user_id() < r.user_id();
	return this->zone_id() < r.zone_id();
}

string AdBidUnit::ToString() const {
	ostringstream value;
	value <<group_id()<<" "<<bid_unit_id()<<" "<<member_id()<<" "<<gender()<<" "<<stage()<<" "
				<<grade()<<" "<<age()<<" "<<school()<<" "<<area()<<" "<<platform()<<" "<<network()<<" "
				<<brand()<<" "<<resolution()<<" "<<lbs_pos()<<" "<<lbs_radius()<<" "<<member_category();
	return value.str();
}

//用于在map中做key时比较用，无业务意义
bool Rectangle::operator<( const Rectangle& r ) const{
	if (this->x_min() - r.x_min() > 0.0001){
		return false;
	}	else if (r.x_min() - this->x_min() > 0.0001){
		return true;
	} else {
		if (this->y_min() - r.y_min() > 0.0001){
			return false;
		}	else if (r.y_min() - this->y_min() > 0.0001){
			return true;
		}	else {
			if (this->x_max() - r.x_max() > 0.0001){
				return false;
			} else if (r.x_max() - this->x_max() > 0.0001){
				return true;
			} else {
				if (this->y_max() - r.y_max() > 0.0001){
					return false;
				} else if (r.y_max() - this->y_max() > 0.0001){
					return true;
				} else {
					return false;
				}
			}
		}
	}
}

bool Rectangle::operator==( const Rectangle& r ) const{
	bool result = false;
	result = (fabs(this->x_min() - r.x_min()) <= 0.0001) && (fabs(this->y_min() - r.y_min()) <= 0.0001) &&
					 (fabs(this->x_max() - r.x_max()) <= 0.0001) && (fabs(this->y_max() - r.y_max()) <= 0.0001);
	return result;
}

//子集
bool Rectangle::IsInRec( const Rectangle& r ) const{
	bool result = false;
	result = this->x_min() >= r.x_min() && this->y_min() >= r.y_min() &&
					 this->x_max() <= r.x_max() && this->y_max() <= r.y_max();
	return result;
}

string Rectangle::ToString() const {
	ostringstream result;
	result << "(" << x_min() <<"," <<y_min() <<")--(" << x_max() <<"," << y_max() << ")";
	return result.str();
}

ostream& xce::ad::operator<< (ostream &os, const Rectangle &rec) {
	os << "(" << rec.x_min() << "," << rec.y_min() << ")--"
		 << "(" << rec.x_max() << "," << rec.y_max() << ")";
	return os;
}

AdTargetUnit::AdTargetUnit(const AdBidUnit& bid_unit) {
	gender_ = bid_unit.gender();
	stage_ = bid_unit.stage();
	vector<short> short_keys;
	vector<uint64_t> uint_keys;
	Translator::ProcessAge(bid_unit.age(), age_);
	if(bid_unit.stage() == STAGE_COLLEGE ) {
		Translator::ProcessSchool(bid_unit.school(), school_area_, school_);
		Translator::ProcessGrade(bid_unit.grade(), grade_);
	} else if(bid_unit.stage() == STAGE_STAFF ) {
		Translator::ProcessArea(bid_unit.area(), area_);
	} else if(bid_unit.stage() == STAGE_ALL ) {
		Translator::ProcessArea(bid_unit.area(), area_);
		grade_.push_back(ALL);
	}
	Translator::ProcessPlatform(bid_unit.platform(), platform_);
	Translator::ProcessNetwork(bid_unit.network(), network_);
	Translator::ProcessBrand3G(bid_unit.brand(), brand_);
	Translator::ProcessResolution(bid_unit.resolution(),resolution_);
	Translator::ProcessLBS(bid_unit.lbs_pos(), bid_unit.lbs_radius(), lbs_);
}

bool AdTargetUnit::Match(const UserProfile& userProfile, const AdMobReqPara& request) {
	try {
		MCE_DEBUG("AdTargetUnit::Match --> Before gender");
		if(gender_ != GENDER_ALL && userProfile.gender() != gender_) {
			return false;
		} 
		int user_stage = userProfile.stage();
		MCE_DEBUG("AdTargetUnit::Match --> Before stage");
		if(!(user_stage & stage_)) {
			return false;
		}
		MCE_DEBUG("AdTargetUnit::Match --> Before age");
		if( (!age_.empty()) && age_.front() != ALL && find(age_.begin(), age_.end(), (short)userProfile.age()) == age_.end()) {
			return false;
		}
		MCE_DEBUG("AdTargetUnit::Match --> Before stage");
		if(user_stage == STAGE_COLLEGE) {
			//大学用户，用学校或者地域做匹配
			MCE_DEBUG("AdTargetUnit::Match --> Before student school");
		  
      if( school_.empty() || (!school_.empty() && school_.front() == ALL ) || 
					(!school_.empty() && (find(school_.begin(), school_.end(), (uint64_t)userProfile.school()) != school_.end()) ) ||
					(SchoolAreaMatch(boost::lexical_cast<uint64_t>(userProfile.current_area()), school_area_))) {
				MCE_DEBUG("AdTargetUnit::Match --> Before student grade ");
				bool is_matched = grade_.empty() || grade_.front() == ALL || find(grade_.begin(), grade_.end(), userProfile.grade()) != grade_.end();
				MCE_DEBUG("AdTargetUnit::Match --> Before student school grade");
				if ( false == is_matched ) {
					return false;
				} else {
					//grade matched, go on
				}
			} else {
				return false;
			}
		} else if(user_stage == STAGE_STAFF) {
			//白领用户，仅用地域做匹配
			MCE_DEBUG("AdTargetUnit::Match --> Before staff area, ip_area:" << userProfile.ip_area());
			if (!AreaMatch(boost::lexical_cast<uint64_t>(userProfile.ip_area()), area_) ) {
				return false;
			}
		} else {
			return false;
		}
		MCE_DEBUG("AdTargetUnit::Match --> Before platform");
		if( !platform_.empty() && platform_.front() != ALL ) {
			Ice::Long platform_code = PlatformMap::instance().GetPlatform( request.client.osVersion );
			Ice::Long platform_codex = PlatformMap::instance().GetPlatformX( request.client.osVersion );
			bool flag = false;
			if( find( platform_.begin(), platform_.end(), boost::lexical_cast<uint64_t>(platform_code) ) != platform_.end() ) {
				flag = true;
			} else if ( find( platform_.begin(), platform_.end(), boost::lexical_cast<uint64_t>(platform_codex) ) != platform_.end() ) {
				flag = true;
			}
			if ( false == flag) {
				return false;
			}
		}
		MCE_DEBUG("AdTargetUnit::Match --> Before net status");
		short network_code = lexical_cast<short>( request.netStatus );
		if( !network_.empty() && network_.front() != ALL && find(network_.begin(), network_.end(), network_code) == network_.end() ) {
			return false;
		}
		/*
		MCE_INFO("AdTargetUnit::Match --> Before brand of 3G");
		Ice::Long brand3G_code = Brand3GMap::instance().GetBrand3G( request.client.model );
		if( !brand_.empty() && brand_.front() != ALL && find(brand_.begin(), brand_.end(), (uint64_t)brand3G_code) == brand_.end() ) {
			return false;
		}
		*/
		MCE_DEBUG("AdTargetUnit::Match --> Before screen size");
		vector<string> fields;
		boost::algorithm::split_regex( fields, request.client.screenSize, boost::regex("[\\*|X]"));
		vector<string>::iterator it_resolution = fields.begin();
		short screen_width = atoi( it_resolution->c_str() );
		short floor_width = ResolutionMap::instance().GetFloorWidth(screen_width);
		if( !resolution_.empty() && resolution_.front() != ALL && find(resolution_.begin(), resolution_.end(), floor_width) == resolution_.end() ) {
			return false;
		}
		MCE_DEBUG("AdTargetUnit::Match --> Before lbs");
		Rectangle lbs_point(request.locationRealTime.longitude, request.locationRealTime.latitude, 0.0);
		if( !lbs_.empty() && !LbsMatch(lbs_point, lbs_) ) {
			return false;
		}
		MCE_DEBUG("AdTargetUnit::Match --> Return True");
		return true;
	} catch (std::exception e) {
		MCE_WARN("AdTargetUnit::Match() --> " << e.what());
	} catch (...) {
		MCE_WARN("AdTargetUnit::Match() --> unkown exception");
	}
	return false;
}


bool AdTargetUnit::AreaMatch(uint64_t key, const vector<uint64_t>& area) {
	if(area.empty() || area.front() == ALL) {
		return true;
	}
	uint64_t province = (key/10000000000)*10000000000;
	uint64_t COUNTRY = 86000000000000; 
	if(find(area.begin(), area.end(), key) != area.end()) {
		return true;
	}
	if(province != key) {
		if(find(area.begin(), area.end(), province) != area.end()) {
			return true;
		}
	}
	if(province != COUNTRY) {
		if(find(area.begin(), area.end(), COUNTRY) != area.end()) {
			return true;
		}
	}
	return false;
}

bool AdTargetUnit::SchoolAreaMatch(uint64_t key, const vector<uint64_t>& area) {
	if( area.empty() ) {
		return false;
	}
	uint64_t province = (key/10000000000)*10000000000;
	uint64_t COUNTRY = 86000000000000; 
	if(find(area.begin(), area.end(), key) != area.end()) {
		return true;
	}
	if(province != key) {
		if(find(area.begin(), area.end(), province) != area.end()) {
			return true;
		}
	}
	if(province != COUNTRY) {
		if(find(area.begin(), area.end(), COUNTRY) != area.end()) {
			return true;
		}
	}
	return false;
}


bool AdTargetUnit::LbsMatch(Rectangle rec_key, const vector<Rectangle>& rec_list) {
	if(rec_list.empty() == true || rec_list.front() == LBS_ALL) {
		return true;
	}
	vector<Rectangle>::const_iterator it = rec_list.begin();
	for(; it != rec_list.end(); ++it) {
		if ( rec_key.IsInRec(*it) ) {
			return true;
		}
	}
	return false;
}
