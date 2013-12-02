#include "CachePool.h"
#include "AdMobAdapter.h"

using namespace xce::ad;

void xce::ad::UserBindPool::TotalLoad() {
	MCE_INFO("UserBindPool::TotalLoad --> start");
	Statement sql; 
	set<AdUserBindPtr> local_pool;
	sql << " SELECT    user_id, creative_id, adzone_id "
			<< " FROM      ad_copyscrn_detail "
			<< " WHERE     adzone_id IN (100000000098, 100000000099) "
			<< " AND       expire_time > now() ";

	mysqlpp::StoreQueryResult res;
	try {
		Util::ExecuteQuery(sql, GetDB(), "ad_user_bind", res);
		if ( res && res.num_rows() > 0 ) {
			for( size_t i = 0; i < res.num_rows(); i++){
				AdUserBindPtr userbind = new AdUserBind;
				mysqlpp::Row row = res.at(i);
				userbind->set_user_id( mysql_int(row["user_id"]) );
				userbind->set_zone_id( mysql_long(row["adzone_id"]) );
				userbind->set_creative_id( mysql_long(row["creative_id"]) );
				local_pool.insert(userbind);
			}
			MCE_INFO("UserBindPool::TotalLoad --> end <DB> ad_user_bind size: " << res.size() );
			MCE_INFO("UserBindPool::TotalLoad --> end ad_user_bind_pool size: " << local_pool.size() );
			
		} else {
			MCE_INFO("[YELLOW] UserBindPool::TotalLoad --> end <DB> ad_user_bind have no data");
		}
    {
      IceUtil::RWRecMutex::WLock lock(mutex_);
      pool_.swap(local_pool);
    }
	} catch (Ice::Exception& e) {
		MCE_WARN("UserBindPool::TotalLoad : Ice Exception: " << e);
	} catch (std::exception& e) {
		MCE_WARN("UserBindPool::TotalLoad : std exception: " << e.what());
	}
}

AdUserBindPtr UserBindPool::GetUserBind(const int user_id, const Ice::Long zone_id) {
	AdUserBindPtr userbind = new AdUserBind;
	userbind->set_user_id(user_id);
	userbind->set_zone_id(zone_id);
	IceUtil::RWRecMutex::RLock lock(mutex_); 
	set<AdUserBindPtr>::iterator it = pool_.find(userbind);
	if (it != pool_.end()) {
		return (*it);
	}
	return NULL;
}

size_t UserBindPool::Size() {
  IceUtil::RWRecMutex::RLock lock(mutex_);
	return pool_.size();
}

string UserBindPool::Print(const int size) {
  stringstream result;
  result << "[UserBind pool]:\n";
  int size_local = (size>0)?size:65535;
  IceUtil::RWRecMutex::RLock lock(mutex_);
  set<AdUserBindPtr>::iterator it = pool_.begin();
  for(int i=1; it != pool_.end() && i <= size_local;it++,i++) {
		result << "["<<i<<"]" << (*it)->user_id() << "_" << (*it)->zone_id() << "_" << (*it)->creative_id();
		if ( i % 3 == 0 ) {
		  result << "\n";
		}
	}
	return result.str();
}
