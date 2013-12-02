#include "CachePool.h"

using namespace boost;
using namespace xce::ad;
using namespace MyUtil;
using namespace com::xiaonei::xce;

void xce::ad::MemberPool::TotalLoad() {
	MCE_INFO("MemberPool::TotalLoad --> start");
	Statement sql; 
	map<Ice::Long, AdMemberPtr> local_pool;
	sql << " SELECT    member_id, have_money, category, industry "
			<< " FROM      member "
			<< " WHERE     validity = 1 AND audit_status = 1 "
			<< " AND       is_formal = 1 AND no_exceed_max = 1 AND am_online_status = 1 "
			<< " AND       ma_online_status =1 AND delete_flag = 1 ";

	mysqlpp::StoreQueryResult res;
	try {
		Util::ExecuteQuery(sql, GetDB(), "member", res);
		if ( res && res.num_rows() > 0 ) {
			for( size_t i = 0; i < res.num_rows(); i++){
				AdMemberPtr member = new AdMember;
				mysqlpp::Row row = res.at(i);
				member->set_member_id( mysql_long(row["member_id"]) );
				member->set_have_money( mysql_int(row["have_money"]) );
				member->set_category( mysql_int(row["category"]) );
				member->set_industry( mysql_int(row["industry"]) );
				local_pool[member->member_id()] = member;
			}
			MCE_INFO("MemberPool::TotalLoad --> end <DB> member size: " << res.size() );
			MCE_INFO("MemberPool::TotalLoad --> end member_pool size: " << local_pool.size() );
			{
				IceUtil::RWRecMutex::WLock lock(mutex_);
				pool_.swap(local_pool);
			}
		} else {
			MCE_WARN("MemberPool::TotalLoad --> end <DB> member have no data");
		}
	} catch (Ice::Exception& e) {
		MCE_WARN("MemberPool::TotalLoad : Ice Exception: " << e);
	} catch (std::exception& e) {
		MCE_WARN("MemberPool::TotalLoad : std exception: " << e.what());
	}
}

AdMemberPtr MemberPool::GetAdMember(Ice::Long member_id) {
	IceUtil::RWRecMutex::RLock lock(mutex_);
	AdMemberPtr ans = NULL;
	map<Ice::Long, AdMemberPtr>::iterator it = pool_.find(member_id);
	if (it != pool_.end()) {
		ans = it->second;
	}
	return ans;
}

size_t MemberPool::Size() {
	IceUtil::RWRecMutex::RLock lock(mutex_);
	return pool_.size();
}

string MemberPool::Print(const int size) {
	stringstream result;
	result << "[member pool]:\n";
	int size_local = (size>0)?size:65535;
	IceUtil::RWRecMutex::RLock lock(mutex_);
	map<Ice::Long, AdMemberPtr>::iterator it = pool_.begin();
	for(int i=1; it != pool_.end() && i <= size_local; ++it,++i) {
		result << "["<<i<<"]"<<it->second->member_id()<<" ";
		if ( i % 5 == 0 ) {
			result << "\n";
		}
	}
	return result.str();
}
