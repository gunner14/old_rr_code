#include <bitset>
#include <boost/tokenizer.hpp>

#include "CachePool.h"
#include "InvertedIndex.h"

using namespace boost;
using namespace xce::ad;
using namespace MyUtil;
using namespace com::xiaonei::xce;

void GroupPool::TotalLoad() {
	TotalLoadAdGroup();
	TotalLoadAdCreative();
	FilterNoCreativeAdGroups();
	TotalLoadAdGroupAdZoneRef();
	InvertedIndex::instance().TotalLoad();
}

void GroupPool::TotalLoadAdGroup() {
	MCE_INFO("GroupPool::TotalLoad --> start");
	Statement sql; 
	map<Ice::Long, AdGroupPtr> local_pool;
	sql << " SELECT    adgroup_id, campaign_id, member_id, max_price, display_type, trans_type, "
			<< "           day_points,hour_points, rotation_number "
			<< " FROM      adgroup  "
			<< " WHERE     member_status = 1 AND am_status = 1 AND audit_status = 1 "
			<< " AND       delete_flag = 1 AND daily_sys_status = 1 AND total_sys_status = 1 "
			<< " AND       display_type in (9, 10) ";

	mysqlpp::StoreQueryResult res;
	try {
		Util::ExecuteQuery(sql, GetDB(), "adgroup", res);
		if ( res && res.num_rows() > 0 ) {
			for( size_t i = 0; i < res.num_rows(); i++){
				AdGroupPtr group = new AdGroup;
				mysqlpp::Row row = res.at(i);
				group->set_group_id( mysql_long(row["adgroup_id"]) );
				group->set_campaign_id( mysql_long(row["campaign_id"]) );
				group->set_member_id( mysql_long(row["member_id"]) );
				group->set_max_price( mysql_int(row["max_price"]) );
				group->set_display_type( mysql_int(row["display_type"]) );
				group->set_trans_type( mysql_int(row["trans_type"]) );
				group->set_rotation_number( mysql_int(row["rotation_number"]) );

				string day_points = mysql_string(row["day_points"]);
				string hour_points = mysql_string(row["hour_points"]);


				AdCampaignPtr campaign_ptr = CampaignPool::instance().GetAdCampaign( group->campaign_id() );
				if (campaign_ptr) {
					group->set_campaign_status( 1 );
				} else {
					//MCE_DEBUG("GroupPool::TotalLoadAdGroup --> cannot find campaign: adgroup="<<group->group_id()<<" campaign:"<<group->campaign_id());
					continue;
				}

				AdMemberPtr member_ptr = MemberPool::instance().GetAdMember( group->member_id() );
				if (member_ptr) {
					group->set_member_table_status( member_ptr->HaveMoney() );
					group->set_member_category( member_ptr->category() );
					group->set_member_industry( member_ptr->industry() );
				} else {
					//MCE_DEBUG("GroupPool::TotalLoadAdGroup --> cannot find member: adgroup="<<group->group_id()<<" member:"<<group->member_id());
					continue;
				}

				bool point_valid = PointsChecker::Check(day_points, hour_points);
				group->set_point_valid(point_valid);

				if ( true == group->Available() ) {  
					local_pool[group->group_id()] = group;
				}
			}
			MCE_INFO("GroupPool::TotalLoadAdGroup --> end <DB Table adgroup> size: " << res.size() << " adgroup_pool size:" << local_pool.size() );
			{
				IceUtil::RWRecMutex::WLock lock(mutex_);
				pool_.swap(local_pool);
			}
		} else {
			MCE_WARN("GroupPool::TotalLoadAdGroup --> end <DB> adgroup have no data");
		}
	} catch (Ice::Exception& e) {
		MCE_WARN("GroupPool::TotalLoadAdGroup : Ice Exception: " << __LINE__ <<" "<< e);
	} catch (std::exception& e) {
		MCE_WARN("GroupPool::TotalLoadAdGroup : std exception: " << __LINE__ <<" "<< e.what());
	}
}

AdGroupPtr GroupPool::GetAdGroup(Ice::Long group_id) {
	AdGroupPtr ans = NULL;
	IceUtil::RWRecMutex::RLock lock(mutex_);
	map<Ice::Long, AdGroupPtr>::iterator it = pool_.find(group_id);
	if (it != pool_.end()) {
		ans = it->second;
	}
	return ans;
}

map<Ice::Long, vector<AdGroupPtr> > GroupPool::GetRotateAds() {
	IceUtil::RWRecMutex::RLock lock(mutex_);
	return zone2rotate_;
}

void GroupPool::TotalLoadAdGroupAdZoneRef() {
	MCE_INFO("GroupPool::TotalLoadAdGroupAdZoneRef --> start");
	Statement sql; 
	map<Ice::Long, AdGroupPtr> local_pool;
	sql << " SELECT    adgroup_id, adzone_id, max_price "
			<< " FROM      adgroup_adzone_ref  "
			<< " WHERE     member_status =1 AND am_status = 1 AND delete_flag = 1 AND show_flag = 1 "
			<< " AND       adzone_id IN (100000000098, 100000000099) ";

	mysqlpp::StoreQueryResult res;
	map< Ice::Long, vector<AdGroupPtr> > zone2rotate;
	try {
		Util::ExecuteQuery(sql, GetDB(), "adgroup", res);
		if ( res && res.num_rows() > 0 ) {
			for( size_t i = 0; i < res.num_rows(); i++){
				mysqlpp::Row row = res.at(i);
				Ice::Long adgroup_id = mysql_long(row["adgroup_id"]);
				Ice::Long adzone_id = mysql_long(row["adzone_id"]);

				map<Ice::Long, AdGroupPtr>::iterator it = pool_.find( adgroup_id );
				if ( it != pool_.end() ) {
					AdGroupPtr group = it->second;
					group->add_adzone_id(adzone_id);
					if ( ROTATE3G == group->display_type() ) {
						MCE_DEBUG("GroupPool::TotalLoadAdGroupAdZoneRef --> Get a rotate group:" << group->group_id() );
						map<Ice::Long, vector<AdGroupPtr> >::iterator it = zone2rotate.find(adzone_id);
						if (it == zone2rotate.end()) {
							vector<AdGroupPtr> v;
							zone2rotate.insert(make_pair<Ice::Long, vector<AdGroupPtr> > (adzone_id, v));
						}
						int rotation_number = group->rotation_number();
						for (int rotation_index = 0; rotation_index < rotation_number; ++rotation_index) {
							zone2rotate[adzone_id].push_back(group);
						}
					}
				} else {
					//MCE_DEBUG("GroupPool::TotalLoadAdGroupAdZoneRef --> adgroup_id:"<<adgroup_id<<" zone_id:"<<adzone_id << " not find in grouppool");
				}
			}
			MCE_INFO("GroupPool::TotalLoadAdGroupAdZoneRef --> zone2rotate.size:" << zone2rotate.size());
			{
				IceUtil::RWRecMutex::WLock lock(mutex_);
				zone2rotate_.swap(zone2rotate);
			}
			MCE_INFO("GroupPool::TotalLoadAdGroupAdZoneRef --> end <DB> adgroupadzoneRef size: " << res.size() );
		} else {
			MCE_WARN("GroupPool::TotalLoadAdGroupAdZoneRef --> end <DB> adgroupadzoneRef have no data");
		}
	} catch (Ice::Exception& e) {
		MCE_WARN("GroupPool::TotalLoadAdGroupAdZoneRef : Ice Exception: " << e);
	} catch (std::exception& e) {
		MCE_WARN("GroupPool::TotalLoadAdGroupAdZoneRef : std exception: " << e.what());
	}
}

void GroupPool::TotalLoadAdCreative() {
	MCE_INFO("GroupPool::TotalLoadAdCreative --> start");
	Statement sql;
	map<Ice::Long, AdGroupPtr> local_pool;
	sql << " SELECT    c.creative_id, c.adgroup_id, c.widget_id, c.widget "
			<< " FROM      creative c, adgroup_adzone_ref ref"
			<< " WHERE     c.adgroup_id = ref.adgroup_id "
			<< " AND       c.member_status =1 AND c.am_status = 1 AND c.delete_flag = 1 AND c.audit_status = 1 "
			<< " AND       ref.adzone_id IN (100000000098, 100000000099) ";
	mysqlpp::StoreQueryResult res;
	map< Ice::Long, vector<AdGroupPtr> > zone2rotate;
	try {
		Util::ExecuteQuery(sql, GetDB(), "adcreative", res);
		for( size_t i = 0; i < res.num_rows(); i++){
			mysqlpp::Row row = res.at(i);
			Ice::Long creative_id = mysql_long(row["creative_id"]);
			Ice::Long group_id = mysql_long(row["adgroup_id"]);
			Ice::Long widget_id = mysql_long(row["widget_id"]);
			string widget = mysql_string(row["widget"]);

			map<Ice::Long, AdGroupPtr>::iterator it = pool_.find( group_id );
			if ( it != pool_.end() ) {
				AdCreativePtr creative = new AdCreative(creative_id, widget_id, widget);
				it->second->add_creative(creative);
				it->second->add_creative_id(creative_id);
				it->second->add_widget_id(creative_id, widget_id);
			}
		}
		MCE_DEBUG("GroupPool::TotalLoadAdCreative --> Load Creatives Done");
	} catch (Ice::Exception& e) {
		MCE_WARN("GroupPool::TotalLoadAdCreative : Ice Exception: " << e);
	} catch (std::exception& e) {
		MCE_WARN("GroupPool::TotalLoadAdCreative : std exception: " << e.what());
	}
	MCE_INFO("GroupPool::TotalLoadAdCreative --> end");
}

void GroupPool::FilterNoCreativeAdGroups() {
	map<Ice::Long, AdGroupPtr>::iterator it = pool_.begin();
	for(; it != pool_.end(); ) {
		if ( false == it->second->have_creative() ) {
			MCE_DEBUG("GroupPool::FilterNoCreativeAdGroups --> Erase AdGroup:" << it->second->group_id());
			IceUtil::RWRecMutex::WLock lock(mutex_);
			pool_.erase(it++);
		} else {
			++it;
		}
	}
}

size_t GroupPool::Size() {
	IceUtil::RWRecMutex::RLock lock(mutex_);
	return pool_.size();
}

string GroupPool::Print(const int size) {
	stringstream result_brand;
	stringstream result_self;
	result_brand << "[brand group pool]:\n";
	result_self << "[self group pool]:\n";
	int size_local = (size>0)?size:65535;
	IceUtil::RWRecMutex::RLock lock(mutex_);
	map<Ice::Long, AdGroupPtr>::iterator it = pool_.begin();
	for(int i=1,j=1; it != pool_.end() && (i+j) <= size_local; ++it) {
		if ( ROTATE3G == it->second->display_type() || MEMBER_BRAND == it->second->member_category() ) {
			result_brand << "["<<i<<"]"<<it->second->group_id()<<" ";
			if ( i % 5 == 0 ) {
				result_brand << "\n";
			}
			++i;
		} else if ( MEMBER_BRAND != it->second->member_category() ) {
			result_self << "["<<i<<"]"<<it->second->group_id()<<" ";
			if ( j % 5 == 0 ) {
				result_self << "\n";
			}
			++j;
		} else {
			MCE_WARN("GroupPool::Print --> Unknown group type found, group_id:" << it->second->group_id());
		}
	}
	result_brand << "\n" << result_self.str();
	return result_brand.str();
}

bool GroupPool::PointsChecker::Check(const string & day_points, const string & hour_points) {
	bool valid = CheckDay(day_points) && CheckHour(hour_points);
	return valid;
}

bool GroupPool::PointsChecker::CheckHour(const string & hour_point_str) {
	if (hour_point_str.empty()) {
		return true;
	}
	bitset < 24 > hour_points;
	try {
		hour_points = lexical_cast<int> (hour_point_str);
	} catch (...) {
		MCE_WARN("PointsChecker::CheckHour --> boost cast hour_point_str = " << hour_point_str << " error");
	}
	Date date = Date(time(NULL));
	int this_hour = date.hour();
	bool result = hour_points.test(this_hour);
	return result;
}

bool GroupPool::PointsChecker::CheckDay(const string & day_point_str) {
	if (day_point_str.empty()) {
		return true;
	}

	tokenizer<> tok(day_point_str);
	tokenizer<>::iterator tit = tok.begin();

	try{
		map<int, bitset<32> > yearmonth_day;
		int year = lexical_cast<int> ((*(tit)).substr(0, 4));
		int month = lexical_cast<int> ((*(tit++)).substr(4, 2));
		for (; tit != tok.end(); ++tit) {
			if ((*tit) != "0") {
				yearmonth_day[year * 100 + month] = lexical_cast<int> (*tit);
			}
			if (month == 12) {
				++year;
				month = 1;
			} else {
				++month;
			}
		}

		Date date = Date(time(NULL));
		int this_day = date.day();
		int this_month = date.month();
		int this_year = date.year();
		int yearmonth = this_year * 100 + this_month;
		map<int, bitset<32> >::iterator mit = yearmonth_day.find(yearmonth);
		if (mit != yearmonth_day.end()) {
			bool result = mit->second.test(this_day - 1); //0代表1号，以此类推
			return result;
		}
	} catch(...)  {
		MCE_WARN("PointsChecker::CheckDay --> boost cast day_point_str = " << day_point_str << " error");
	}
	return false;
}
