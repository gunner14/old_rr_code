#include "CachePool.h"
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace boost;
using namespace xce::ad;
using namespace MyUtil;
using namespace com::xiaonei::xce;

void xce::ad::ZonePool::TotalLoad() {
	MCE_INFO("ZonePool::TotalLoad --> start");
	Statement sql; 
	map<Ice::Long, AdZonePtr> local_pool;
	sql << " SELECT    adzone_id, member_type, count, priority, rotation_number, min_price "
			<< " FROM      adzone "
			<< " WHERE     member_online_status =1 AND am_online_status = 1 AND audit_status = 1 "
			<< " AND	     delete_flag = 1 AND adzone_id in (100000000098, 100000000099) ";
	mysqlpp::StoreQueryResult res;
	try {
		Util::ExecuteQuery(sql, GetDB(), "adzone", res);
		if ( res && res.num_rows() > 0 ) {
			for( size_t i = 0; i < res.num_rows(); i++){
				AdZonePtr adzone = new AdZone;
				mysqlpp::Row row = res.at(i);
				adzone->set_zone_id( mysql_long(row["adzone_id"]) );
				adzone->set_member_type( mysql_int(row["member_type"]) );
				adzone->set_brand_count( mysql_int(row["count"]) );
				adzone->set_priority( mysql_int(row["priority"]) );
				adzone->set_rotation_number( mysql_string(row["rotation_number"]) );
				adzone->set_min_price( mysql_double(row["min_price"]) );
				if( true == adzone->rotation_number().empty() ) {
					adzone->ResizeRotateList(0);
				} else {
					vector<string> fields;
					split( fields, adzone->rotation_number(), is_any_of(","));
					int sum = 0;
					for( vector<string>::iterator it = fields.begin(); it != fields.end(); ++it ) {
						sum += lexical_cast<int>(*it);
					}
					adzone->ResizeRotateList(sum);
				}

				local_pool[adzone->zone_id()] = adzone;
				MCE_INFO("ZonePool::TotalLoad --> build zone pool, zone:"<<adzone->zone_id());
			}
			MCE_INFO("ZonePool::TotalLoad --> end <DB> adzone size: " << res.size() );
			MCE_INFO("ZonePool::TotalLoad --> end zone_pool size: " << local_pool.size() );
			{
				IceUtil::RWRecMutex::WLock lock(mutex_);
				pool_.swap(local_pool);
			}
			FillRotateGroups();
		} else {
			MCE_WARN("ZonePool::TotalLoad --> end <DB> adzone have no data");
		}
	} catch (Ice::Exception& e) {
		MCE_WARN("ZonePool::TotalLoad : Ice Exception: " << e);
	} catch (std::exception& e) {
		MCE_WARN("ZonePool::TotalLoad : std exception: " << e.what());
	}
}

AdZonePtr ZonePool::GetAdZone(Ice::Long adzone_id) {
	IceUtil::RWRecMutex::RLock lock(mutex_);
	AdZonePtr ans = NULL;
	map<Ice::Long, AdZonePtr>::iterator it = pool_.find(adzone_id);
	if (it != pool_.end()) {
		ans = it->second;
	}
	return ans;
}

inline void ZonePool::FillRotateGroups() {
	const map<Ice::Long, vector<AdGroupPtr> > zone2rotate = GroupPool::instance().GetRotateAds();
	MCE_INFO("ZonePool::zone2rotate.size:"<<zone2rotate.size());
	if ( false == zone2rotate.empty() ) {
		for (map<Ice::Long, vector<AdGroupPtr> >::const_iterator it = zone2rotate.begin(); it != zone2rotate.end(); ++it) {
			Ice::Long zone_id = it->first;
			int groups_size = it->second.size();
			AdZonePtr zone = GetAdZone(zone_id);
			if ( zone ) {
				int rotate_size = zone->GetRotateSize();
				if ( groups_size >= rotate_size * zone->brand_count() ) {
					zone->ResizeRotateList(groups_size);
				}
				int index=0;
				vector<AdGroupPtr>::const_iterator groupit = it->second.begin();
				for (; groupit != it->second.end(); ++groupit, ++index) {
					if ( *groupit && !zone->AddGroupToRotateList(index, *groupit) ) {
						;
					}
				}
				zone->RandomRotateList();
				MCE_INFO("ZonePool::FillRotateGroups --> zone: " << zone_id << " zone2rotate size:" << zone->GetRotateSize() );
			} else {
				MCE_INFO("ZonePool::FillRotateGroups --> adgroup_adzone_ref zoneid:"<< zone_id << " miss in adzone");
			}
		}
	}
}

string ZonePool::Print(const int size) {
	stringstream result;
	result << "[zone pool]:\n";
	int size_local = (size>0)?size:65535;
	IceUtil::RWRecMutex::RLock lock(mutex_);
	map<Ice::Long, AdZonePtr>::iterator it = pool_.begin();
	for(int i=1; it != pool_.end() && i <= size_local;it++,i++) {
		result << "["<<i<<"]"<<it->second->zone_id()<<" ";
		if ( i % 5 == 0 ) {
			result << "\n";
		}
	}
	return result.str();
}

size_t ZonePool::Size() {
	IceUtil::RWRecMutex::RLock lock(mutex_);
	return pool_.size();
}

