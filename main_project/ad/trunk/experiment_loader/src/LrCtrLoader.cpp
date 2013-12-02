/*
 *  LrCtrLoader.cpp
 *  @Author: jieyu.zeng
 *  @Date: 2012-01-05
 *  @Description: 实验数据cache
 *
 */

#include "LrCtrLoader.h"
#include "ExperimentBaseUtil.h"
#include <query.h>
#include <string>
#include <algorithm>
#include "AdConfig.h"
#include "Util.h"
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <connection.h>
#include <query.h>

using namespace boost;
namespace xce{
namespace ad{
using namespace mysqlpp;

void LrCtrLoader::LoadCtrOnlineModelPool(){
  string file_name = AdExperimentConfig::instance().CtrOnlineModelPath();
  ifstream buffer;
  try {
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    string line;
    buffer.open(file_name.c_str());
    if (buffer) {
      CtrOnlineModelMap ctr_online_model;
      while (buffer >> line) {
        boost::char_separator<char> sep("|");
        tokenizer tokens(line, sep);
        string key;
        CtrOnlineModel value;
        value.id = 0;
        value.pv = 0;
        value.click = 0;
        value.ctr = 0.0;
        value.weight = 0.0;
        int cnt = 0;
        try {
          for(tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it, ++cnt) {
            switch (cnt) {
              case 0 : key = *it; 
                       break;
              case 1 : value.id = boost::lexical_cast<Ice::Long>(*it); 
                       break;
              case 2 : value.pv = boost::lexical_cast<Ice::Long>(*it); 
                       break;
              case 3 : value.click = boost::lexical_cast<Ice::Long>(*it); 
                       break;
              case 4 : value.ctr = boost::lexical_cast<double>(*it); 
                       break;
              case 5 : value.weight = boost::lexical_cast<double>(*it);
                       break;
              default: ;
            }
          }
          if (6 == cnt) { //记录完整
            ctr_online_model[key] = value; 
          //  MCE_DEBUG(""<< value.id << " key:" << key << " pv:" << value.pv  << " click:" << value.click << " ctr:"<< setprecision(20) << value.ctr << " weight:" << setprecision(30) << value.weight);
          } else {
            MCE_WARN("LrCtrLoader::LoadCtrOnlineModelPool : bad data, key = " << key);
          }
        } catch (...) {
          MCE_WARN("LrCtrLoader::LoadCtrOnlineModelPool parse online model data error cnt = " << cnt);
        } 
      }
      IceUtil::RWRecMutex::WLock lock(mutex_);
      ctr_online_model_.swap(ctr_online_model);          
      MCE_INFO("LrCtrLoader::LoadCtrOnlineModelPool : pool size = " << ctr_online_model_.size());
    } else {
      MCE_WARN("LrCtrLoader::LoadCtrOnlineModelPool error : " << "no file " << file_name);
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("LrCtrLoader::LoadCtrOnlineModelPool error : " << e);
  } catch (...) {
    MCE_WARN("LrCtrLoader::LoadCtrOnlineModelPool error : unkown error \"...\"");
  }
  if (buffer.is_open()) {
    buffer.close();
  }
}

void LrCtrLoader::LoadAdClassListPool(){
  string file_name = AdExperimentConfig::instance().AdClassListPath();
  ifstream buffer;
  try {
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    buffer.open(file_name.c_str());
    string line;

    if (buffer) {
      AdClassListMap ad_class;
      while (buffer >> line) {
        boost::char_separator<char> sep("|");
        tokenizer tokens(line, sep);
        long key;
        vector<AdClass> value;
        string classify;
        int max_cnt = (AdExperimentConfig::instance().MaxAdClassNum() << 1) + 1 ;//限制分类数
        int cnt = 0;
        try {
          for(tokenizer::iterator it = tokens.begin(); it != tokens.end() && (cnt < max_cnt); ++it, ++cnt) {
            switch (cnt) {
              case 0 : key = boost::lexical_cast<Ice::Long>(*it); 
                       break;
              default: 
                if (cnt & 1) {
                  classify = *it; 
                } else {
                  AdClass item;
                  item.classify = classify;
                  item.pvalue = boost::lexical_cast<double>(*it);                             
                  value.push_back(item); 
                }
            }
          }
          if (cnt & 1) { //奇数项，表示记录完整
            ad_class[key] = value; 
          } else {
            MCE_WARN("LrCtrLoader::LoadAdClassListPool : bad data, key = " << key);
          }
        } catch (...) {
          MCE_WARN("LrCtrLoader::LoadAdClassListPool parse ad class data error cnt = " << cnt);
        } 
      }
      IceUtil::RWRecMutex::WLock lock(mutex_);
      ad_class_.swap(ad_class);          
      MCE_INFO("LrCtrLoader::LoadAdClassListPool : pool size = " << ad_class_.size());
    } else {
      MCE_WARN("LrCtrLoader::LoadAdClassListPool error : " << "no file " << file_name);
    }
    if (buffer) {
      buffer.close();
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("LrCtrLoader::LoadAdClassListPool error : " << e);
  } catch (...) {
    MCE_WARN("LrCtrLoader::LoadAdClassListPool error : unkown error \"...\"");
  }
  if (buffer.is_open()) {
    buffer.close();
  }
} 

void LrCtrLoader::LoadAdCtrFeaturePool() {
  MCE_INFO("LrCtrLoader::LoadAdCtrFeaturePool begin");
  AdCtrFeatureList pool;
  try{ 
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep1("|");
    boost::char_separator<char> sep2("^");
    boost::char_separator<char> sep3("-");
    const char* db_host = AdExperimentConfig::instance().ConfigDbHost().c_str();
    const char* db_name = AdExperimentConfig::instance().ConfigDbName().c_str();
    Connection conn(db_name, db_host,"ad","rrad");
    MCE_INFO("LrCtrLoader::LoadAdCtrFeaturePool, db host:" << db_host<< ",db_name:" << db_name);
    stringstream sql; 
    sql.str("");
    sql << " select id, item_num, id_str, id_set, type, switch from ctr_online_model_feature where switch = 1";
    MCE_INFO("try to excute " << sql.str());
    string s= sql.str();
    Query q(&conn,true,s.c_str());
    mysqlpp::StoreQueryResult res = q.store();
    if (!res || res.num_rows() <= 0) { 
      MCE_WARN("LrCtrLoader::LoadAdCtrFeaturePool Error " << sql.str() << ".  select  fail!");
    }    
    MCE_DEBUG("LrCtrLoader::LoadAdCtrFeaturePool res size : " << res.num_rows());
    for (size_t i = 0; i < res.num_rows(); ++i) {
      mysqlpp::Row row = res.at(i);
      int key = (int) row["id"]; 
      int type = (int) row["type"];
      unsigned int item_num = (unsigned int) row["item_num"];
      string id_str = (string) row["id_str"];
      string ids = (string) row["id_set"];
      tokenizer token_str1(ids, sep1); //解析特征组合
 
      AdCtrFeature value;
      value.type = type;
      value.id = key;
      value.idstr = id_str;
      for(tokenizer::iterator it1 = token_str1.begin(); it1 != token_str1.end(); ++it1) {
        AdCtrFeatureItem item;
        tokenizer token_str2((*it1), sep2);//解析特征补足长度 
        
        if (token_str2.begin() != token_str2.end()) {
          tokenizer::iterator it2 = token_str2.begin(); 

          tokenizer token_str3((*it2), sep3);//解析特征id和最小有效长度 
          tokenizer::iterator it3 = token_str3.begin(); 
          item.id = boost::lexical_cast<int>(*it3); 
          if ((++it3)!= token_str3.end()) {
            item.vlen = boost::lexical_cast<int>(*it3); 
          } else {
            item.vlen = 0;
          }      
           
          if ((++it2) != token_str2.end()) {
            item.len = boost::lexical_cast<int>(*it2); 
          } else {
            item.len = 0;
          }   
        } else {
          continue; 
        }
        MCE_DEBUG("LrCtrLoader::LoadAdCtrFeaturePool id = " << item.id << " vlen = " << item.vlen << " len " << item.len); 
        value.items.push_back(item);
      }
      
      if (item_num != value.items.size()) {
        MCE_WARN("LrCtrLoader::LoadAdCtrFeaturePool config error: item_num = " << item_num << " but valid size  = " << value.items.size());
        continue;
      }
      
      pool.push_back(value);
    }    
    IceUtil::RWRecMutex::WLock lock(mutex_);
    ad_ctr_feature_.swap(pool);
    MCE_DEBUG("LrCtrLoader::LoadAdCtrFeaturePool pooll size is " << ad_ctr_feature_.size());
  } catch (std::exception& e) { 
    MCE_WARN("LrCtrLoader::LoadAdCtrFeaturePool error: " << e.what());
  } catch (...) {
    MCE_WARN("LrCtrLoader::LoadAdCtrFeaturePool error: unknown error!");
  }

  MCE_INFO("LrCtrLoader::LoadAdCtrFeaturePool stop, pool size:" << ad_ctr_feature_.size());
}


void LrCtrLoader::LoadAdCtrRealTimePool() {
  MCE_INFO("LrCtrLoader]::try to load.LoadPool begin");
  MyUtil::Date begin = MyUtil::Date::seconds(time(NULL));
  string begin_time = begin.str("%Y-%m-%d %H:%M:%S");
  Ice::Long begin_time_pos = GetTime(begin.year(),begin.month(),begin.day(),begin.hour());
  hour_ = begin.hour();
  MyUtil::Date end = MyUtil::Date::seconds(time(NULL) - AdExperimentConfig::instance().RealTimeWindow()*3600); //设定时间窗
  Ice::Long end_time_pos = GetTime(end.year(),end.month(),end.day(),end.hour());
  map<Ice::Long , Ice::Long> gid_spid_map;
  AdCtrRealTimeMap res_pool;

  try {
    const char* db_host = AdExperimentConfig::instance().ConfigDbHost().c_str();
    const char* db_name = AdExperimentConfig::instance().ConfigDbName().c_str();
    Connection conn(db_name,db_host,"ad","rrad");
    MCE_INFO("LrCtrLoader::LoadPool,db host:" << AdExperimentConfig::instance().ConfigDbHost()<< ",db_name:" << db_name);
    stringstream sql;
    {
      Ice::Long max_time = begin_time_pos;
      max_time =(((max_time * 100 + begin.hour()) * 100 + begin.minute())* 100 + begin.second());
      MCE_INFO("[LrCtrLoader]::LoadPool begin,start time:"<< max_time <<",begin time:" << begin_time);
    }

    {/*******************************************/
      sql.str("");
      sql << "select adgroup_id, adzone_id, abs_pos, stage, gender, sum(pv_count) as sum_impr, sum(click_count) as sum_click from rt_HZPMGGS where time <= " << begin_time_pos << " and time >= "<< end_time_pos << " and adzone_id in (100000000072,100000000001,100000000063,100000000070, 100000000075, 100000000065, 100000000093)  "<< " group by adgroup_id,adzone_id,abs_pos,stage,gender ";
      MCE_INFO("try to excute " << sql.str());
      string s= sql.str();
      Query q(&conn,true,s.c_str());
      mysqlpp::StoreQueryResult res = q.store();
      if (!res || res.num_rows() <= 0) {
        MCE_WARN("LrCtrLoader Load Error " << sql.str() << ".  select  fail!");
      }
      MCE_INFO("[LrCtrLoader]::Load   res size : " << res.num_rows());
      for (size_t i = 0; i < res.num_rows(); ++i) {
        mysqlpp::Row row = res.at(i);
        Ice::Long group_id = (Ice::Long) row["adgroup_id"];
        Ice::Long zone_id = (Ice::Long) row["adzone_id"];
        int pos_id = (int) row["abs_pos"];
        Ice::Int gender = (Ice::Int) row["gender"];
        Ice::Int stage = (Ice::Int) row["stage"];
        Ice::Long impr = (Ice::Long) row["sum_impr"];
        int click = (int) row["sum_click"];

        AdCtrRealTimeItem item;
        item.pv = impr;
        item.click = click;
        for (int i = 1; i <= 6; ++i) { 
		      string key = GetKeyByIndex(gender, stage, zone_id, pos_id, group_id, i);
				  AddAdCtrRealTimeItem(res_pool, key, item);	 
		      key = GetKeyByIndex(gender, stage, zone_id, pos_id, group_id, -i);
				  AddAdCtrRealTimeItem(res_pool, key, item);	 
        }
        string key = GetKeyByIndex(gender, stage, zone_id, pos_id, group_id, 1); //计算最细粒度
				AddAdCtrRealTimeItemAdZoneDiscount(res_pool, key, item);
  	  }

			ExpandCtrByPos(res_pool);
			CalcWeight(res_pool);

			for(AdCtrRealTimeMap::iterator it = res_pool.begin(); it != res_pool.end(); ){
        if (it->second.weight <= 0.0) {
				 // MCE_DEBUG("[LrCtrLoader]:: erase item : key = "<< it->first << ":" <<it->second.pv <<" " << it->second.click << " " << it->second.ctr << " " << it->second.weight);
          res_pool.erase(it++);
        } else {
				  MCE_DEBUG("[LrCtrLoader]:: save item : key = "<< it->first << ":" <<it->second.pv <<" " << it->second.click << " " << it->second.ctr << " " << it->second.weight);
          ++it;
        }
			}
  	}
    MCE_INFO("[LrCtrLoader]:: stop res_pool size is " << res_pool.size());
  	{
  	  IceUtil::RWRecMutex::WLock lock(mutex_);
  	  real_time_ctr_online_model_.swap(res_pool);
  	  MCE_INFO("[LrCtrLoader]::LoadPool stop  size is " << real_time_ctr_online_model_.size());
  	}
  } catch (std::exception& e) {
    MCE_WARN("LoadByTime --> load demo pool error: " << e.what());
  } catch (...) {
    MCE_WARN("LoadByTime --> load demo pool error: unknown error!");
  }

}


void LrCtrLoader::CalcWeight(AdCtrRealTimeMap& temp_res_pool){
  MCE_INFO("LrCtrLoader::CalcWeight begin:temp size:" << temp_res_pool.size());
	// 计算在不同维度折算的数量	
  try {
	  for(AdCtrRealTimeMap::iterator it = temp_res_pool.begin(); it != temp_res_pool.end(); it++){
		  vector<string> tmp;	
		  boost::algorithm::split_regex(tmp, it->first, regex("\\|"));
		  Ice::Long adgroup_id = boost::lexical_cast<Ice::Long>(tmp[0]);
		  Ice::Long adzone_id = boost::lexical_cast<Ice::Long>(tmp[1]);			
		  int pos = boost::lexical_cast<int>(tmp[2]);
  	  int gender = boost::lexical_cast<int>(tmp[3]);
  	  int stage = boost::lexical_cast<int>(tmp[4]);
      Ice::Long min_th = 100000;
      Ice::Long max_th = 1000000;
      Ice::Long pv_th = min_th;	
      string total_key = GetKeyByIndex(gender, stage, adzone_id, pos, adgroup_id, -1);
        //MCE_DEBUG("LrCtrLoader::CalcWeight total_key = " << total_key);	
		  AdCtrRealTimeMap::iterator itf = temp_res_pool.find(total_key);
		  if(itf != temp_res_pool.end()){
			  pv_th = (Ice::Long)(itf->second.pv * AdExperimentConfig::instance().ThresholdRatio(hour_));
        if (pv_th < min_th) { 
          if (total_key != it->first) {
            pv_th = min_th;
          }
        }
        if (pv_th > max_th) {
          pv_th = max_th;
        }
        //MCE_DEBUG("LrCtrLoader::CalcWeight pv_th = "<< pv_th << "total_pv = " << itf->second.pv);	
      }
      it->second.weight = CalcRealTimeWeight(it->second, pv_th);      
      if (it->second.weight > 0.0){ 
        MCE_DEBUG("LrCtrLoader::CalcWeight key = "<< it->first << "weight = " << it->second.weight);	
      }
    }
  } catch (...){
    MCE_WARN("LrCtrLoader::CalcWeight ERROR uknown");
  }
}

void LrCtrLoader::ExpandCtrByPos(AdCtrRealTimeMap& temp_res_pool){
  MCE_INFO("LrCtrLoader::ExpandByPos,begin:temp size:" << temp_res_pool.size());
	
  map<string,double> click_discount_ratio_table;
  click_discount_ratio_table[pos_751] = 1;
  click_discount_ratio_table[pos_752] = 1.920088852;
  click_discount_ratio_table[pos_753] = 5.376857573;
  click_discount_ratio_table[pos_721] = 1;
  click_discount_ratio_table[pos_722] = 1.647276818;
  click_discount_ratio_table[pos_701] = 1;
  click_discount_ratio_table[pos_702] = 2.047550947;
  click_discount_ratio_table[pos_631] = 1;
  click_discount_ratio_table[pos_632] = 1.358939264;
  click_discount_ratio_table[pos_633] = 1.711976435;
  click_discount_ratio_table[pos_011] = 1;
  click_discount_ratio_table[pos_012] = 0.775770392;
  click_discount_ratio_table[pos_013] = 1.362162384;
	// 计算在不同维度折算的数量	
	int count_discount = 0;

	for(AdCtrRealTimeMap::iterator it = temp_res_pool.begin(); it != temp_res_pool.end(); it++){
		vector<string> tmp;	
		boost::algorithm::split_regex(tmp, it->first, regex("\\|"));
		Ice::Long adgroup_id = boost::lexical_cast<Ice::Long>(tmp[0]);
		Ice::Long adzone_id = boost::lexical_cast<Ice::Long>(tmp[1]);			
		int pos = boost::lexical_cast<int>(tmp[2]);
  	int gender = boost::lexical_cast<int>(tmp[3]);
  	int stage = boost::lexical_cast<int>(tmp[4]);
		int click = it->second.click;
		Ice::Long pv = it->second.pv;
		int sum_click = 0;
		Ice::Long sum_pv = 0;
		
    string key = GetKey(adzone_id,pos);
		map<string, double>::iterator itc = click_discount_ratio_table.find(key);
		double ratio = 1.0;
		if(itc != click_discount_ratio_table.end()) {
			ratio = itc->second;
		}

		if( adgroup_id != -1 && adzone_id != -1 && pos != -1 && gender != -1 && stage != -1 ){
			int click_th = 20;
			Ice::Long pv_th = 100000;	
			if(pv < pv_th && click < click_th){
        string zone_key = GetKeyByIndex(gender, stage, adzone_id, -1, adgroup_id, 1);
				AdCtrRealTimeMap::iterator itf = temp_res_pool.find(zone_key);
				if(itf != temp_res_pool.end()){
					sum_click = itf->second.click;
					sum_pv = itf->second.pv;	
				}
				if(sum_click > click_th || sum_pv > pv_th){
				  count_discount++;
					double pv_weight = (double) pv / (double) pv_th;
					double click_weight = (double) click / (double) click_th;
 					double weight = pv_weight > click_weight ? pv_weight : click_weight;
					it->second.ctr = weight * ( (double) click / (double) pv) + (1 - weight) * ((double) sum_click / (double) (sum_pv * ratio) );	      
					it->second.click = (int)(weight * (double) click + (1 - weight) * (double) sum_click / ratio);	      
          it->second.pv += pv_th;
				}else{
					 
				}
			}
		}
	}
	MCE_INFO("LrCtrLoader::ExpandByPos,end --> the discount is: "<< count_discount);
}

void LrCtrLoader::AddAdCtrRealTimeItemAdZoneDiscount(AdCtrRealTimeMap& temp_adzone_data, string key, AdCtrRealTimeItem& item){
	
  map<string,double> click_discount_ratio_table;
  click_discount_ratio_table[pos_751] = 1;
  click_discount_ratio_table[pos_752] = 1.920088852;
  click_discount_ratio_table[pos_753] = 5.376857573;
  click_discount_ratio_table[pos_721] = 1;
  click_discount_ratio_table[pos_722] = 1.647276818;
  click_discount_ratio_table[pos_701] = 1;
  click_discount_ratio_table[pos_702] = 2.047550947;
  click_discount_ratio_table[pos_631] = 1;
  click_discount_ratio_table[pos_632] = 1.358939264;
  click_discount_ratio_table[pos_633] = 1.711976435;
  click_discount_ratio_table[pos_011] = 1;
  click_discount_ratio_table[pos_012] = 0.775770392;
  click_discount_ratio_table[pos_013] = 1.362162384;

	vector<string> tmp;	
	boost::algorithm::split_regex(tmp, key, regex("\\|"));
	Ice::Long adgroup_id = boost::lexical_cast<Ice::Long>(tmp[0]);
	Ice::Long adzone_id = boost::lexical_cast<Ice::Long>(tmp[1]);			
	int pos = boost::lexical_cast<int>(tmp[2]);
  int gender = boost::lexical_cast<int>(tmp[3]);
  int stage = boost::lexical_cast<int>(tmp[4]);

  string ratio_key = GetKey(adzone_id,pos);
	map<string, double>::iterator itc = click_discount_ratio_table.find(ratio_key);
	double ratio = 1.0;
	if(itc != click_discount_ratio_table.end()) {
		ratio = itc->second;
	}
	
  string zone_key = GetKeyByIndex(gender, stage, adzone_id, -1, adgroup_id, 1);
	AdCtrRealTimeMap::iterator itm = temp_adzone_data.find(zone_key);
	item.click = (int)(item.click * ratio);

	AddAdCtrRealTimeItem(temp_adzone_data, zone_key, item);
}
 
void LrCtrLoader::AddAdCtrRealTimeItem(AdCtrRealTimeMap& temp_adzone_data, string& key, AdCtrRealTimeItem& item){
	AdCtrRealTimeMap::iterator itm = temp_adzone_data.find(key);
  AdCtrRealTimeItem temp_item;
	if( itm == temp_adzone_data.end() ){
	  temp_item.pv = item.pv;
		temp_item.click = item.click;	
	}else{
		temp_item.pv = itm->second.pv + item.pv;
		temp_item.click = itm->second.click + item.click;
	}
  temp_item.ctr = CalcRealTimeCtr(temp_item);  
	temp_adzone_data[key] = temp_item;         
}	

double LrCtrLoader::CalcRealTimeCtr(AdCtrRealTimeItem &item){
  if (item.pv <= 0) {
    return 0.0;
  }
  return (double)item.click / (double)item.pv;
}

double LrCtrLoader::CalcRealTimeWeight(AdCtrRealTimeItem &item, Ice::Long pv_th){ 
  if (item.pv <= pv_th) {
    return (item.pv - pv_th) / (pv_th + 1.0); //置负权
  }
  double weight_limit = AdExperimentConfig::instance().RealTimeWeightLimit();
  double log2 = log(item.pv / (pv_th + 1.0)) / log(2); //TODO 根据pv调权
  return weight_limit * (1.0 - 1.0 / log2);
}

/**************************************************
* 根据index，返回对应维度的实时模型key 
*
**************************************************/
string LrCtrLoader::GetKeyByIndex(int gender, int stage, long adzone, int pos, long group_id, int index){
	stringstream key;
  switch (index) {
	  case 6 : key << group_id <<"|-1|-1|-1|-1";	
             break;
	  case 5 : key << group_id <<"|"<< adzone << "|-1|-1|-1"; 
             break;
	  case 4 : key << group_id <<"|"<< adzone <<"|"<< pos << "|-1|-1"; 
             break;
	  case 3 : key << group_id <<"|"<< adzone <<"|"<< pos << "|-1|" << stage; 
             break;
	  case 2 : key << group_id <<"|"<< adzone <<"|"<< pos <<"|"<< gender <<"|-1"; 
             break;
	  case 1 : key << group_id <<"|"<< adzone <<"|"<< pos <<"|"<< gender <<"|"<< stage; 
             break;
	  case -6 : key << "-1|-1|-1|-1|-1";	
             break;
	  case -5 : key << "-1|"<< adzone << "|-1|-1|-1"; 
             break;
	  case -4 : key << "-1|"<< adzone <<"|"<< pos << "|-1|-1"; 
             break;
	  case -3 : key << "-1|"<< adzone <<"|"<< pos << "|-1|" << stage; 
             break;
	  case -2 : key << "-1|"<< adzone <<"|"<< pos <<"|"<< gender <<"|-1"; 
             break;
	  case -1 : key << "-1|"<< adzone <<"|"<< pos <<"|"<< gender <<"|"<< stage; 
             break;
	  default:  key << "-1|-1|-1|-1|-1";	
	}

	return key.str();
} 


}
}

