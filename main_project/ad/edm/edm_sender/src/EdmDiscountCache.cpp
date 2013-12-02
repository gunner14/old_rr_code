/*
 * DiscountCache.cpp
 *
 * module role :通过member_id获取折扣
 * Created on: 2012-8-8
 * Author: 
 *
 */
#include "EdmDiscountCache.h"
#include "ServiceI.h"
#include "QueryRunner.h"
//#include "ParametersController.h"
//#include <connection.h>
//#include <query.h>

using namespace std;
using namespace xce::ad;
using namespace com::xiaonei::xce;
using namespace MyUtil;

void EdmDiscountCache::LoadEdmDiscount() {
  MCE_INFO("EdmDiscountCache,DiscountCache::Load start");
  try {
    //从数据库把表load过来
    const string db_instance = "ad_st"; 
    Statement sql;
    sql << "select * from discount_ratio";
    mysqlpp::StoreQueryResult res = QueryRunner(db_instance, CDbRServer).store(sql);
    if (!res) {
	  MCE_WARN("select table discount error.");
	  return ;
    }
    MCE_INFO("EdmDiscountCache::Load EdmDiscountCache, Discount_ratio Size=: " << res.num_rows());
    map<Ice::Long, double> pool;
    for (size_t i = 0; i < res.num_rows(); ++i) {
      mysqlpp::Row row = res.at(i);
	  Ice::Long member_id =   boost::lexical_cast<Ice::Long> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["member_id"]);
	  double discount_ratio = boost::lexical_cast<double> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["discount_ratio"]);
	  MCE_INFO("EdmDiscountCache::Load member_id:"<<member_id<<"--->"<<discount_ratio);
	  pool[member_id] = discount_ratio;
    }
    {
      IceUtil::RWRecMutex::WLock lock(mutex_);
      discount_map_.swap(pool);
    }
    MCE_INFO("End: EdmDiscountCache::Load size = " << discount_map_.size());

  }catch (Ice::Exception& e) {
    MCE_WARN("EdmDiscountCache::load --> call Database ERR  line:" << __LINE__ << " err:" << e);
  } catch (std::exception& e) {
    MCE_WARN("EdmDiscountCache::load --> call Database ERR  line:" << __LINE__ << " err:" << e.what());
  } catch (...) {
    MCE_WARN("EdmDiscountCache::LoadEdmDiscountCache error!");
  }
}



