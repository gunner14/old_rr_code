#include <sstream>
#include "LogWrapper.h"
#include <boost/lexical_cast.hpp>
#include "ad/indexService/lower_ad_send_speed/include/config.h"
#include "../common/constant.h"

using namespace std;
using namespace mysqlpp;
using namespace xce::ad;
using namespace com::xiaonei::xce;

const char* CONFIG_KEY = "config_key";
const char* CONFIG_VALUE = "config_value";

void LowerConfig::UpdateData(){
    Statement sql;
    sql << " SELECT * FROM ad_send_speed_lower_config "; 

    StoreQueryResult res;
    try{
        res = QueryRunner(db_instance_, CDbRServer, CONFIG_TABLE_NAME).store(sql);
    } catch (Ice::Exception& e){
        MCE_WARN("LowerConfig::UpdateData ad_send_speed_lower_config Error! " << e.what());
        return;
    } catch (std::exception& e){
        MCE_WARN("LowerConfig::UpdateData ad_send_speed_lower_config std error! " << e.what());
        return;
    }

    if (!res){
        MCE_WARN("Query Result Error!");
        return;
    }
    else if (res.empty()){
        MCE_WARN("nothing in db!");
        return;
    }

    is_loading_ = true;
    try{
      for (unsigned int i = 0; i < res.size(); ++i){
	string key = boost::lexical_cast<string>(res.at(i)[CONFIG_KEY]);
	double value = boost::lexical_cast<double> ((mysqlpp::Null<string, mysqlpp::NullIsZero>) res.at(i)[CONFIG_VALUE]);
	if (CONFIG_SET.find(key) != CONFIG_SET.end()){
	  config_[key] = value;
	  //MCE_INFO("LowConfig --> name: " << key << " value: " << value);
	}
      }
    } catch (...) {
	MCE_WARN("LowConfig DB Error!");
    }
    is_loading_ = false;
    return;
}

double LowerConfig::GetValue(const string& name){
    if (is_loading_){
	//MCE_INFO("LowerConfig::GetValue Failed --> is Loading from sql.");
	return FAILED;
    }

    if (config_.find(name) != config_.end()){
        //MCE_DEBUG("LowerConfig::GetValue() --> name: " << name << " value: " << config_[name]);
	return config_[name];
    }
    MCE_WARN("LowerConfig::GetValue --> name: " << name << " not exist in config_ map!");
    return FAILED;
}
