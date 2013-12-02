/********************************
*  广告位质量数据的读取和cache
*  
*
********************************/
#include "AdZoneLoader.h"
#include "QueryRunner.h"
#include "AdQualityUtil.h"
#include "AdConfig.h"
#include <vector>
#include <map>
#include <connection.h>
#include <query.h>
#include <string>
namespace xce{
namespace ad{
using namespace mysqlpp;
using namespace com::xiaonei::xce;
/************************************
*  从db中计算出广告位质量数据, 并得出序
*
************************************/
void AdZoneLoader::LoadAdZoneQualityPool(){
  MCE_INFO("AdZoneLoader::LoadAdZoneQualityPool begin");
  try {
    MyUtil::Date begin = MyUtil::Date::seconds(time(NULL) - 14 * 24 * 3600);
    string begin_time = begin.str("%Y%m%d%H");
    MCE_INFO("AdZoneLoader::LoadAdZoneQualityPool begin_time=" << begin_time);
    Statement sql;
    string db = AdDemoConfig::instance().GetDBSource_StatBigt(); 
    string data_table = "Log_HSDGSAAN";
    sql << " SELECT adzone_id, FLOOR(time % 100) AS hour, gender, stage, "
        << " SUM((abs_pos=1)*pv_count) AS pv,  SUM(click_count) AS click, SUM(sum_click_price) AS cost"
        << " FROM " << data_table
        << " WHERE time >= " << begin_time
        << " GROUP BY adzone_id, hour, gender, stage";
    mysqlpp::StoreQueryResult res = QueryRunner(db.c_str(), CDbRServer, data_table).store(sql);
    if (!res || res.num_rows() <= 0) {
      MCE_WARN("AdZoneLoader::LoadAdZoneQualityPool sql query error!");
      return ;
    } 
    MCE_INFO("AdZoneLoader::LoadAdZoneQualityPool, query size:" << res.num_rows() );
    AdZoneQualityMap pool;
    multimap<double, string> sort_map;
    for (size_t i = 0; i < res.num_rows(); ++i) {
      mysqlpp::Row row = res.at(i);
      Ice::Long adzone_id = (Ice::Long)row["adzone_id"];
      int hour = (int) row["hour"]; 
      int gender = (int) row["gender"];
      int stage = (int) row["stage"];
      Ice::Long pv = (Ice::Long) row["pv"];
      if (pv <= 0) { 
        continue;
      }
      int click = (int) row["click"];
      double cost = (double) row["cost"];
      double ctrz = click / pv;
      double cpmz = cost / pv;
      ostringstream os;
      os << adzone_id <<"_"<< hour <<"_" <<gender<<"_"<<stage;
      string key = os.str();
      AdZoneQuality value;
      value.pv = pv;
      value.click = click;
      value.ctrz = ctrz;
      value.cpmz = cpmz;
      value.rank = -1;
      pool[key] = value;
      sort_map.insert(make_pair(cpmz, key));   
    }
   
    int rank = (int)pool.size();
    for (multimap<double, string>::iterator it = sort_map.begin(); it != sort_map.end(); ++it) {
      pool[it->second].rank = --rank;
      MCE_DEBUG("AdZoneLoader::LoadAdZoneQualityPool key=" << it->second << " rank=" << rank);
    }
    
    MCE_INFO("AdZoneLoader::LoadAdZoneQualityPool, load size = " << pool.size());
    if (pool.size() > 0) {
      IceUtil::RWRecMutex::WLock lock(mutex_);
      quality_pool_.swap(pool);
      ready_ = true;
    }
  } catch (Ice::Exception& e){
    MCE_WARN("AdZoneLoader::LoadAdZoneQualityPool error:" << e.what()); 
  } catch (...){ 
    MCE_WARN("AdZoneLoader::LoadAdZoneQualityPool error: unknown error"); 
  }
}


} /*  end of namespace ad */
} /*  end of namespace xce */

