#include "../include/db_quota_loader.h"

using namespace std;
using namespace mysqlpp;
using namespace xce::ad;
using namespace com::xiaonei::xce;

bool DBQuotaLoader::Load(map<Ice::Long, DummyRecordPtr>& records, const vector<string>& node_name, const Statement& sql, ComputeRatioPtr computer, time_t& last_update_time, const string& table_name){
  StoreQueryResult res;
  if (!GetDataFromDB(sql, res, table_name)){
    return false;
  }
  
  if (!Parse(res, node_name, records, computer, last_update_time)){
    return false;
  }

  return true;
}

bool DBQuotaLoader::GetDataFromDB(const Statement& sql, StoreQueryResult& res, const string& table_name){
  try{
    res = QueryRunner(db_instance_, CDbRServer, table_name).store(sql);
  } catch (Ice::Exception& e){
    MCE_WARN("DBQuotaLoader::GetDataFromDB Error! " << table_name << " " << e.what());
    return false;
  } catch (std::exception& e){
    MCE_WARN("DBQuotaLoader::GetDataFromDB std error! " << table_name << " "  << e.what());
    return false;
  }

  if (!res){
    MCE_WARN("Query Result Error!");
    return false;
  }
  else if (res.empty()){
    MCE_INFO("nothing in db!");
    return false;
  }

  return true; 
}

bool DBQuotaLoader::Parse(const StoreQueryResult& res, const vector<string>& node_name, map<Ice::Long, DummyRecordPtr>& records, ComputeRatioPtr computer, time_t& last_update_time){
  time_t max_time = 0;
  //MCE_DEBUG("DBQuotaLoader::Parse db size: " << res.size() << " records size: " << records.size());
  for (unsigned int i = 0; i < res.size(); ++i){
    vector<string>::const_iterator v_iter = node_name.begin();
    Ice::Long key_id = boost::lexical_cast<Ice::Long> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)res.at(i)[v_iter->c_str()]);
    time_t update_time = boost::lexical_cast<time_t> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)res.at(i)[UPDATE_TIME.c_str()]);
    //ostringstream os;
    map<Ice::Long, DummyRecordPtr>::iterator m_iter = records.find(key_id);
    /*if (m_iter != records.end()){
      MCE_DEBUG("DBQuotaLoader::Parse --> id: key_id " << key_id << " update_time: " << update_time << "cache update_time: " << m_iter->second->update_time_);
    }*/
    if (m_iter != records.end()
        && m_iter->second->update_time_ < update_time){
      map<string, double> data_map;
      for (; v_iter != node_name.end(); ++v_iter){
	data_map[*v_iter] = boost::lexical_cast<double> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)res.at(i)[v_iter->c_str()]);
        //os << "name: " << *v_iter << " value: " << data_map[*v_iter];
      }
      m_iter->second->value_ = computer->Compute(data_map);
      //os << " ratio: " << m_iter->second->value_;
      m_iter->second->update_time_ = update_time;
      //MCE_DEBUG("DBQuotaLoader::Parse --> id: " << key_id << " " << os.str() << " update_time: " << update_time);
    }
    max_time = (update_time > max_time ? update_time : max_time);
  }

  //if there is no data update in db at this moment, we will update through http
  if (max_time > last_update_time){
    last_update_time = max_time;
    //MCE_INFO("DBQuotaLoader::Parse --> last_update_time: " << last_update_time);
    return true;
  }

  MCE_WARN("DBQuotaLoader::Parse --> there is no update in db! update_time: " << last_update_time);
  return false;
}

