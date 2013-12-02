#include "CachePool.h"

using namespace boost;
using namespace xce::ad;
using namespace MyUtil;
using namespace com::xiaonei::xce;

void xce::ad::CampaignPool::TotalLoad() {
	MCE_INFO("CampaignPool::TotalLoad --> start");
	Statement sql; 
	map<Ice::Long, AdCampaignPtr> local_pool;
	sql << " SELECT    campaign_id, member_id, begin_time, end_time "
			<< " FROM      campaign "
			<< " WHERE     delete_flag <> 0 AND audit_status = 1 AND member_status = 1 "
			<< " AND       am_status = 1 AND sys_status = 1 "
			<< " AND       now() > begin_time "
			<< " AND       (now() < end_time OR end_time is NULL) ";
	mysqlpp::StoreQueryResult res;
	try {
		Util::ExecuteQuery(sql, GetDB(), "campaign", res);
		if ( res && res.num_rows() > 0 ) {
			for( size_t i = 0; i < res.num_rows(); ++i) {
				AdCampaignPtr campaign = new AdCampaign;
				mysqlpp::Row row = res.at(i);
				campaign->set_campaign_id( mysql_long(row["campaign_id"]) );
				campaign->set_member_id( mysql_long(row["member_id"]) );
				local_pool[campaign->campaign_id()] = campaign;
			}
			MCE_INFO("CampaignPool::TotalLoad --> end <DB> campaign size: " << res.size() );
			MCE_INFO("CampaignPool::TotalLoad --> end campaign_pool size: " << local_pool.size() );
			{
				IceUtil::RWRecMutex::WLock lock(mutex_);
				pool_.swap(local_pool);
			}
		} else {
			MCE_WARN("CampaignPool::TotalLoad --> end <DB> member have no data");
		}
	} catch (Ice::Exception& e) {
		MCE_WARN("CampaignPool::TotalLoad : Ice Exception: " << e);
	} catch (std::exception& e) {
		MCE_WARN("CampaignPool::TotalLoad : std exception: " << e.what());
	}
}

AdCampaignPtr CampaignPool::GetAdCampaign(Ice::Long campaign_id) {
	IceUtil::RWRecMutex::RLock lock(mutex_);
	AdCampaignPtr ans = NULL;
	map<Ice::Long, AdCampaignPtr>::iterator it = pool_.find(campaign_id);
	if (it != pool_.end()) {
		ans = it->second;
	}
	return ans;
}

size_t CampaignPool::Size() {
	IceUtil::RWRecMutex::RLock lock(mutex_);
	return pool_.size();
}

string CampaignPool::Print(const int size) {
	stringstream result;
	result << "[campaign pool]:\n";
	int size_local = (size>0)?size:65535;
	IceUtil::RWRecMutex::RLock lock(mutex_);
	map<Ice::Long, AdCampaignPtr>::const_iterator it = pool_.begin();
	for(int i=0; it != pool_.end() && i <= size_local; ++it,++i) {
		result << "["<<i<<"]"<<it->second->campaign_id()<<" ";
		if ( i % 5 == 0 ) {
			result << "\n";
		}
	}
	return result.str();
}

