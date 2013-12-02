#include "CachePool.h"

using namespace boost;
using namespace xce::ad;
using namespace MyUtil;
using namespace com::xiaonei::xce;

void xce::ad::DiscountPool::TotalLoad() {
	MCE_INFO("DiscountPool::TotalLoad --> start");
	Statement sql; 
	map<Ice::Long, double> local_pool;
	sql << " SELECT member_id, discount_ratio "
			<< " FROM  3g_discount_ratio ";
	mysqlpp::StoreQueryResult res;
	try {
		Util::ExecuteQuery(sql, GetDB(), "3g_discount_ratio", res);
		if ( res && res.num_rows() > 0 ) {
			for( size_t i = 0; i < res.num_rows(); ++i) {
				mysqlpp::Row row = res.at(i);
				Ice::Long member_id = mysql_long(row["member_id"]);
        //品牌客户不进行账户折扣
				AdMemberPtr member_ptr = MemberPool::instance().GetAdMember(member_id);
				if ( member_ptr && MEMBER_BRAND != member_ptr->category() ) {
					double ratio = mysql_double(row["discount_ratio"]);
					local_pool[member_id] = ratio;
				}
			}
			MCE_INFO("DiscountPool::TotalLoad --> end DiscountPool DB size: " << res.num_rows());
			MCE_INFO("DiscountPool::TotalLoad --> end DiscountPool size: " << local_pool.size());
			{
				IceUtil::RWRecMutex::WLock lock(mutex_);
				pool_.swap(local_pool);
			}
		} else {
			MCE_INFO("DiscountPool::TotalLoad --> end <DB> discount_ratio have no data");
		}
	} catch (Ice::Exception& e) {
		MCE_WARN("DiscountPool::TotalLoad : Ice Exception: " << e);
	} catch (std::exception& e) {
		MCE_WARN("DiscountPool::TotalLoad : std exception: " << e.what());
	}
}

double DiscountPool::GetDiscountRatio(const Ice::Long& member_id) {
  IceUtil::RWRecMutex::RLock lock(mutex_);
	map<Ice::Long, double>::iterator it = pool_.find(member_id);
	if (it != pool_.end()) {
		return it->second;
	}
	return 1.0;
}

size_t DiscountPool::Size() {
	IceUtil::RWRecMutex::RLock lock(mutex_);
	return pool_.size();
}

string DiscountPool::Print(const int size) {
	stringstream result;
	result << "[discount pool]:\n";
	int size_local = (size>0)?size:65535;
	IceUtil::RWRecMutex::RLock lock(mutex_);
	map<Ice::Long, double>::iterator it = pool_.begin();
	for(int i=0; it != pool_.end() && i <= size_local; ++it,++i) {
		result << "[" << i << "]" << it->first << ":" << it->second;
		if ( i % 5 == 0 ) {
			result << "\n";
		}
	}
	return result.str();
}

