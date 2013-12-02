#include "InvertedIndex.h"
#include "client/linux/handler/exception_handler.h"
#include "ad/util/src/AdCommonUtil.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/regex.hpp>

#include "CachePool.h"
#include "Translator.h"
#include "Config.h"

using namespace xce::ad;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace boost;

void AreaUIntIndex::GetWithAll(const uint64_t key, set<AdGroupPtr>& result) {
	uint64_t province = (key/10000000000)*10000000000;
	IceUtil::RWRecMutex::RLock lock(mutex_);
	map<uint64_t, set<AdGroupPtr> >::iterator it = index_.find(key);
	if(it != index_.end()) {
		set<AdGroupPtr>& key_result =it->second;
		result.insert(key_result.begin(), key_result.end());
	}
	if(province != key) {
		it = index_.find(province);
		if(it != index_.end()) {
			set<AdGroupPtr>& key_result =it->second;
			result.insert(key_result.begin(), key_result.end());
		}
	}
	if(province != COUNTRY) {
		it = index_.find(COUNTRY);
		if(it != index_.end()) {
			set<AdGroupPtr>& key_result =it->second;
			result.insert(key_result.begin(), key_result.end());
		}
	}
	it = index_.find(ALL);
	if (it != index_.end()) {
		set<AdGroupPtr>& key_result =it->second;
		result.insert(key_result.begin(), key_result.end());
	}
}

void RectangleIndex::Get(const Rectangle key, set<AdGroupPtr>& result) {
	IceUtil::RWRecMutex::RLock lock(mutex_);
	KEY_VALUE_ITERATOR it = index_.begin();
	for(;it != index_.end();it++) {
		if ( key.IsInRec(it->first) ) {
			const set<AdGroupPtr>& key_result = it->second;
			result.insert(key_result.begin(), key_result.end());
		}
	}
}

void RectangleIndex::GetWithAll(const Rectangle key, set<AdGroupPtr>& result) {
	IceUtil::RWRecMutex::RLock lock(mutex_);
	KEY_VALUE_ITERATOR it = index_.begin();
	for(;it != index_.end();it++) {
		if ( key.IsInRec(it->first) ) {
			const set<AdGroupPtr>& key_result = it->second;
			result.insert(key_result.begin(), key_result.end());
		}
	}
	it = index_.find(LBS_ALL);
	if ( it != index_.end() ) {
		const set<AdGroupPtr>& key_result = it->second;
		result.insert(key_result.begin(), key_result.end());
	}
}

DimIndex::DimIndex() {
	gender_index_ = new GenderIndex();
	stage_index_ = new StageIndex();
	grade_index_ = new GradeIndex();
	age_index_ = new AgeIndex();
	network_index_ = new NetworkIndex();

	school_index_ = new SchoolIndex();
	area_index_ = new AreaIndex();
	school_area_index_ = new SchoolAreaIndex();
	platform_index_ = new PlatformIndex();
	brand3G_index_ = new Brand3GIndex();
	resolution_index_ = new ResolutionIndex();

	lbs_index_ = new LBSIndex();

	index_dict_[AdMobIndex::GENDER] = gender_index_;
	index_dict_[AdMobIndex::STAGE] = stage_index_;
	index_dict_[AdMobIndex::AGE] = age_index_;
	index_dict_[AdMobIndex::GRADE] = grade_index_;
	index_dict_[AdMobIndex::SCHOOL] = school_index_;
	index_dict_[AdMobIndex::AREA] = area_index_;
	index_dict_[AdMobIndex::SCHOOLAREA] = school_area_index_;
	index_dict_[AdMobIndex::PLATFORM] = platform_index_;
	index_dict_[AdMobIndex::NETWORK] = network_index_;
	index_dict_[AdMobIndex::BRAND3G] = brand3G_index_;
	index_dict_[AdMobIndex::RESOLUTION] = resolution_index_;
	index_dict_[AdMobIndex::LBS] = lbs_index_;
};

void InvertedIndex::TotalLoad() {
	MCE_INFO("InvertedIndex::TotalLoad --> start");
	Statement sql; 
	map<Ice::Long, AdBidUnitPtr> local_pool;
	sql << " SELECT    b.adgroup_id, b.bid_unit_id, b.member_id, b.cast_gender, b.cast_stage, "
			<< "           b.cast_age, b.cast_grade, b.cast_school, b.cast_area, b.cast_platform_3G, b.cast_network_3G, "
			<< "           b.cast_brand_3G, b.cast_resolution_3G, b.cast_lbs_3G, "
			<< "           bid_unit_lbs.cast_radius, bid_unit_lbs.cast_pos "
			<< " FROM      (bid_unit b, adgroup ad) "
			<< "           left outer join bid_unit_lbs on b.cast_lbs_3G = bid_unit_lbs.id "
			<< " WHERE     b.adgroup_id = ad.adgroup_id "
			<< " AND       b.delete_flag = 1  AND ad.display_type IN (9,10) ";
	mysqlpp::StoreQueryResult res;
	map<Ice::Long, DimIndexPtr> zone_brand_index_local;
	map<Ice::Long, DimIndexPtr> zone_self_index_local;
	try {
		Util::ExecuteQuery(sql, GetDB(), "bid_unit b,adgroup ad, member m", res);
		if ( res && res.num_rows() > 0 ) {
			for( size_t i = 0; i < res.num_rows(); i++){
				mysqlpp::Row row = res.at(i);
				AdBidUnit bid_unit;
				bid_unit.set_group_id( mysql_long(row["adgroup_id"]) );
				bid_unit.set_bid_unit_id( mysql_long(row["bid_unit_id"]) );
				bid_unit.set_member_id( mysql_long(row["member_id"]) );
				bid_unit.set_gender( mysql_short(row["cast_gender"]) );
				bid_unit.set_stage( mysql_short(row["cast_stage"]) );
				bid_unit.set_age( mysql_string(row["cast_age"]) );
				bid_unit.set_grade( mysql_string(row["cast_grade"]) );
				bid_unit.set_school( mysql_string(row["cast_school"]) );
				bid_unit.set_area( mysql_string(row["cast_area"]) );

				bid_unit.set_platform( mysql_string(row["cast_platform_3G"]) );
				bid_unit.set_network( mysql_string(row["cast_network_3G"]) );
				bid_unit.set_brand( mysql_string(row["cast_brand_3G"]) );
				bid_unit.set_resolution( mysql_string(row["cast_resolution_3G"]) );
				bid_unit.set_lbs_flag( mysql_int(row["cast_lbs_3G"]) );
				bid_unit.set_lbs_pos( mysql_string(row["cast_pos"]) );
				bid_unit.set_lbs_radius( mysql_double(row["cast_radius"]) );

				AdMemberPtr member = MemberPool::instance().GetAdMember( bid_unit.member_id() );
				if( !member ) {
					continue;
				}
				bid_unit.set_member_category( member->category() );
				//MCE_DEBUG("InvertedIndex::TotalLoad --> bidunit:"<<bid_unit.ToString());
				AdGroupPtr group = GroupPool::instance().GetAdGroup( bid_unit.group_id() );
				if( !group ) {
					//MCE_DEBUG("InvertedIndex::TotalLoad --> cannot find bid_uint.group_id:"<<bid_unit.group_id()<<" in AdGroupPool");
					continue;
				}
				group->set_bid_unit_id(bid_unit.bid_unit_id());
				if ( ROTATE3G == group->display_type() ) {
					MCE_DEBUG("InvertedIndex::TotalLoad --> Get a Rotation Adgroup:" << group->group_id());
					AdTargetUnitPtr target_unit = new AdTargetUnit(bid_unit);
					group->set_target_unit(target_unit);
				} else if ( MEMBER_BRAND == bid_unit.member_category() ) { 
					BuildIndex( bid_unit, group, zone_brand_index_local );
				} else {
					BuildIndex( bid_unit, group, zone_self_index_local );
				}
			}
			MCE_INFO("InvertedIndex::TotalLoad --> end <DB> bid_unit size: " << res.size() );
			{
				IceUtil::RWRecMutex::WLock lock(mutex_);
				zone_brand_index_.swap( zone_brand_index_local );
				zone_self_index_.swap( zone_self_index_local );
			}
			ShowIndexSize();
		} else {
			MCE_WARN("InvertedIndex::TotalLoad --> end <DB> bid_unit have no data");
		}
	} catch (Ice::Exception& e) {
		MCE_WARN("InvertedIndex::TotalLoad : Ice Exception: " << e);
	} catch (std::exception& e) {
		MCE_WARN("InvertedIndex::TotalLoad : std exception: " << e.what());
	}
}

void InvertedIndex::IncLoad() {
	TotalLoad();
}

inline void InvertedIndex::BuildIndex(const AdBidUnit& bid_unit, AdGroupPtr group, map<Ice::Long, DimIndexPtr>& zone_index ) {
	set<Ice::Long> zones = group->adzone_ids();
	vector<short> short_keys;
	vector<uint64_t> uint_keys;
	vector<uint64_t> uint_keys2;
	vector<Rectangle> rec_keys;
	MCE_DEBUG("InvertedIndex::BuildIndex --> adgroup_id:"<<group->group_id() << " zones.size:" << zones.size());
	for (set<Ice::Long>::iterator sit = zones.begin(); sit != zones.end(); ++sit) {
		Ice::Long zone_id = *sit;
		MCE_DEBUG("InvertedIndex::BuildIndex --> zone_id:"<<zone_id);
		if (zone_index.find(zone_id) == zone_index.end()) {
			zone_index.insert(make_pair(zone_id, new DimIndex()));
		}

		DimIndexPtr dim_index = zone_index[zone_id];
		if(Translator::ProcessGender(bid_unit.gender(), short_keys)) {
			dim_index->gender_index_->Add(short_keys, group);
			short_keys.clear();
		}
		if(Translator::ProcessStage(bid_unit.stage(), short_keys)) {
			dim_index->stage_index_->Add(short_keys, group);
			short_keys.clear();
		}
		if(Translator::ProcessAge(bid_unit.age(), short_keys)) {
			dim_index->age_index_->Add(short_keys, group);
			short_keys.clear();
		}
		if( bid_unit.stage() == STAGE_COLLEGE ) {
			if(Translator::ProcessSchool(bid_unit.school(), uint_keys, uint_keys2)) {
				dim_index->school_area_index_->Add(uint_keys, group);
				dim_index->school_index_->Add(uint_keys2, group);
				uint_keys.clear();
				uint_keys2.clear();
			}
			if(Translator::ProcessGrade(bid_unit.grade(), short_keys)) {
				dim_index->grade_index_->Add(short_keys, group);
				short_keys.clear();
			}
		} else if( (bid_unit.stage() == STAGE_STAFF) && Translator::ProcessArea(bid_unit.area(), uint_keys)) {
			dim_index->area_index_->Add(uint_keys, group);
			uint_keys.clear();
		} else if( (bid_unit.stage() == STAGE_ALL) && Translator::ProcessArea(bid_unit.area(), uint_keys)) {
			dim_index->area_index_->Add(uint_keys, group);
			uint_keys.clear();
			short_keys.push_back(ALL);
			dim_index->grade_index_->Add(short_keys, group);
			short_keys.clear();
		}
		if(Translator::ProcessPlatform(bid_unit.platform(), uint_keys)) {
			dim_index->platform_index_->Add(uint_keys, group);
			uint_keys.clear();
		}
		if(Translator::ProcessNetwork(bid_unit.network(), short_keys)) {
			dim_index->network_index_->Add(short_keys, group);
			short_keys.clear();
		}
		/*
			 if(Translator::ProcessBrand3G(bid_unit.brand(), uint_keys)) {
			 dim_index->brand3G_index_->Add(uint_keys, group);
			 uint_keys.clear();
			 }
			 */
		if(Translator::ProcessResolution(bid_unit.resolution(), short_keys)) {
			dim_index->resolution_index_->Add(short_keys, group);
			short_keys.clear();
		}
		if(Translator::ProcessLBS(bid_unit.lbs_pos(), bid_unit.lbs_radius(), rec_keys)) {
			dim_index->lbs_index_->Add(rec_keys, group);
			rec_keys.clear();
		}
	}
	MCE_DEBUG("InvertedIndex::BuildIndex end --> adgroup_id:"<<group->group_id() );
}

void InvertedIndex::Get(const HandlerPara& para, const Ice::Long zone_id, const InvertedIndexType type, set<AdGroupPtr>& result_set) {
	DimIndexPtr dim_index;
	{
		map<Ice::Long, DimIndexPtr>::const_iterator it;
		IceUtil::RWRecMutex::RLock lock(mutex_);
		if ( BRAND_INDEX == type ) {
			it = zone_brand_index_.find(zone_id);
			if (it != zone_brand_index_.end()) {
				dim_index = it->second;
			}
		} else if ( SELF_INDEX == type ) {
			it = zone_self_index_.find(zone_id);
			if (it != zone_self_index_.end()) {
				dim_index = it->second;
			}
		} else {
			MCE_INFO("[YELLOW] InvertedIndex::Get --> undefined index, type:" << type << " zone:" << zone_id);
			return;
		}
	}
	if (!dim_index) {
		MCE_INFO("[YELLOW] InvertedIndex::Get --> index not found, type:" << type << " zone:" << zone_id);
		return;
	}

	const UserProfile& profile =  para.userProfile_;

	set<AdGroupPtr> gender;
	set<AdGroupPtr> stage;
	set<AdGroupPtr> age;

	dim_index->gender_index_->Get(profile.gender(), gender);			//gender set
	dim_index->stage_index_->Get((short)profile.stage(), stage);	    //stage set
	dim_index->age_index_->GetWithAll((short)profile.age(), age);		//age set

	// gender stage age 交集
	vector<AdGroupPtr> result;
	result.resize(min(gender.size(),stage.size()), NULL);

	vector<AdGroupPtr>::iterator it = set_intersection(gender.begin(), gender.end(), stage.begin(), stage.end(), result.begin());
	result_set.insert(result.begin(), it);
	MCE_DEBUG("InvertedIndex::Get --> gender & stage result.size" << result_set.size());

	it = set_intersection(result_set.begin(), result_set.end(), age.begin(), age.end(), result.begin());
	result_set.clear();
	result_set.insert(result.begin(), it);
	MCE_DEBUG("InvertedIndex::Get --> & age result.size" << result_set.size());

	// 符合地域的广告 : 学生按school/current_area, 白领按ip_area.
	MCE_DEBUG("InvertedIndex::Get --> uid:" << profile.id() << " ip_area:" << profile.ip_area() );
	MCE_DEBUG("InvertedIndex::Get --> uid:" << profile.id() << " current_area:" << profile.current_area());
	if ( (int)profile.stage() == STAGE_STAFF ) {
		uint64_t ip_area = lexical_cast<uint64_t> ( profile.ip_area() );
		set<AdGroupPtr> area;
		dim_index->area_index_->GetWithAll(ip_area, area);

		it = set_intersection(result_set.begin(), result_set.end(), area.begin(), area.end(), result.begin());
		result_set.clear();
		result_set.insert(result.begin(), it);
		MCE_DEBUG("InvertedIndex::Get --> & worker area area.size" << area.size());
		MCE_DEBUG("InvertedIndex::Get --> & worker area result.size" << result_set.size());
	} else {
		set<AdGroupPtr> grade;
		set<AdGroupPtr> school;
		set<AdGroupPtr> school_area;
		set<AdGroupPtr> area;
		uint64_t school_id = lexical_cast<uint64_t>(profile.school());
		uint64_t school_scope = lexical_cast<uint64_t> (profile.current_area());
		uint64_t ip_area = lexical_cast<uint64_t> ( profile.ip_area() );

		dim_index->grade_index_->GetWithAll((short)profile.grade(), grade);
		dim_index->school_index_->GetWithAll(school_id, school);
		dim_index->school_area_index_->GetWithAll(school_scope, school_area);
		dim_index->area_index_->GetWithAll(ip_area, area);

		MCE_DEBUG("InvertedIndex::Get --> & grade.size" << grade.size());
		it = set_intersection(result_set.begin(), result_set.end(), grade.begin(), grade.end(), result.begin());
		result_set.clear();
		result_set.insert(result.begin(), it);

		school.insert(school_area.begin(), school_area.end());
		school.insert(area.begin(), area.end());

		it = set_intersection(result_set.begin(), result_set.end(), school.begin(), school.end(), result.begin());
		result_set.clear();
		result_set.insert(result.begin(), it);

		MCE_DEBUG("InvertedIndex::Get --> & student area school.size" << school.size());
		MCE_DEBUG("InvertedIndex::Get --> & student area grade.size" << grade.size());
		MCE_DEBUG("InvertedIndex::Get --> & student area school_area.size" << school_area.size());
		MCE_DEBUG("InvertedIndex::Get --> & student area result.size" << result_set.size());
	}
	//3G定向
	const AdMobClientInfo& client = para.request_.client;

	//Platform定向 查找精确的操作系统型号定向 iPhone OS4.3
	Ice::Long platform_code = PlatformMap::instance().GetPlatform( client.osVersion );
	set<AdGroupPtr> platform_set;
	dim_index->platform_index_->GetWithAll( platform_code, platform_set );

	//Platform定向 查找模糊的操作系统型号定向 iPhone OS4.x
	Ice::Long platform_codex = PlatformMap::instance().GetPlatformX( client.osVersion );
	set<AdGroupPtr> platformx_set;
	dim_index->platform_index_->Get( platform_codex, platformx_set );

	platform_set.insert(platformx_set.begin(),platformx_set.end());

	it = set_intersection(result_set.begin(), result_set.end(), platform_set.begin(), platform_set.end(), result.begin());
	result_set.clear();
	result_set.insert(result.begin(), it);
	MCE_DEBUG("InvertedIndex::Get --> platform result.size" << result_set.size());

	//Network定向 目前只有wifi 0100 和 运营商 0200的定向
	short network_code = lexical_cast<short>( para.request_.netStatus );
	network_code = network_code /100 *100;
	set<AdGroupPtr> network_set;
	dim_index->network_index_->GetWithAll( network_code, network_set );

	it = set_intersection(result_set.begin(), result_set.end(), network_set.begin(), network_set.end(), result.begin());
	result_set.clear();
	result_set.insert(result.begin(), it);
	MCE_DEBUG("InvertedIndex::Get --> network result.size" << result_set.size());

	/*
		 Ice::Long brand3G_code = Brand3GMap::instance().GetBrand3G( client.model );
		 set<AdGroupPtr> brand3G_set;
		 dim_index->brand3G_index_->GetWithAll( brand3G_code, brand3G_set );			  //brand3G

		 it = set_intersection(result_set.begin(), result_set.end(), brand3G_set.begin(), brand3G_set.end(), result.begin());
		 result_set.clear();
		 result_set.insert(result.begin(), it);
		 MCE_DEBUG("InvertedIndex::Get --> brand3g result.size" << result_set.size());
		 */

	//Resolution定向 只关心分辨率中的宽度 获取能在该手机上显示的所有分辨率的广告
	vector<string> fields;
	boost::algorithm::split_regex( fields, client.screenSize, boost::regex("[\\*|X]"));
	vector<string>::iterator it_resolution = fields.begin();
	short screen_width = atoi( it_resolution->c_str() );
	short floor_width = ResolutionMap::instance().GetFloorWidth(screen_width);
	set<AdGroupPtr> resolution_set;
	dim_index->resolution_index_->GetWithAll( floor_width, resolution_set );		  //Resolution

	it = set_intersection(result_set.begin(), result_set.end(), resolution_set.begin(), resolution_set.end(), result.begin());
	result_set.clear();
	result_set.insert(result.begin(), it);
	MCE_DEBUG("InvertedIndex::Get --> resolution result.size" << result_set.size());

	if ( BRAND_INDEX == type ) {  //品牌包含LBS定向功能
		Rectangle lbs_point(para.request_.locationRealTime.longitude, para.request_.locationRealTime.latitude, 0.0);		  //LBS
		set<AdGroupPtr> lbs_set;
		dim_index->lbs_index_->GetWithAll( lbs_point, lbs_set );

		it = set_intersection(result_set.begin(), result_set.end(), lbs_set.begin(), lbs_set.end(), result.begin());
		result_set.clear();
		result_set.insert(result.begin(), it);
		MCE_DEBUG("InvertedIndex::Get --> & lbs result.size" << result_set.size());
	}
}

void InvertedIndex::ShowIndexSize() {
	MCE_INFO("InvertedIndex::ShowIndexSize --> call");
	MCE_INFO("InvertedIndex::zone_brand_index_.size:"<<zone_brand_index_.size());
	MCE_INFO("InvertedIndex::zone_self_index_.size:"<<zone_self_index_.size());
	for (map<Ice::Long, DimIndexPtr>::iterator sit = zone_brand_index_.begin(); sit != zone_brand_index_.end(); ++sit) {
		DimIndexPtr dim_index = sit->second;
		MCE_INFO("InvertedIndex::ShowIndexSize --> brand zone:"<<sit->first);
		map<int, IndexPtr>::iterator it_index = dim_index->index_dict_.begin();
		for (int i=0; it_index != dim_index->index_dict_.end(); ++it_index,++i) {
			MCE_INFO("InvertedIndex::ShowIndexSize --> brand[" << i << "]" << it_index->second->Size());
		}
	}
	for (map<Ice::Long, DimIndexPtr>::iterator sit = zone_self_index_.begin(); sit != zone_self_index_.end(); ++sit) {
		DimIndexPtr dim_index = sit->second;
		MCE_INFO("InvertedIndex::ShowIndexSize --> self zone:"<<sit->first);
		map<int, IndexPtr>::iterator it_index = dim_index->index_dict_.begin();
		for (int i=0; it_index != dim_index->index_dict_.end(); ++it_index,++i) {
			MCE_INFO("InvertedIndex::ShowIndexSize --> self[" << i << "]" << it_index->second->Size());
		}
	}
}
