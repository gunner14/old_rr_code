/*
 *  ExperimentCache.cpp
 *  @Author: jieyu.zeng
 *  @Date: 2012-01-05
 *  @Description:
 *
 */

#include "ExperimentCache.h"
#include "AdExperimentAdapter.h"
#include "AdConfig.h"
#include <string>
#include <algorithm> 
#include <cmath>

using namespace std;
/**************************************************
*  Load 点击率模型数据  
*
**************************************************/
void CtrOnlineModelCache::LoadAdCtrRealTimePool(){
  MCE_INFO("CtrOnlineModel::LoadAdCtrRealTimePool --> start");
  hash_map<string, AdCtrRealTimeItem> pool;
  try {
    AdCtrRealTimeMap temp 
      = AdExperimentAdapter::instance().GetAdCtrRealTimePool(EngineConfig::instance().server_index());
			
		MCE_INFO("CtrOnlineModel::LoadAdCtrRealTimePool --> temp pool size is: "<<temp.size());
    for (AdCtrRealTimeMap::iterator it = temp.begin(); it != temp.end(); ++it){
      pool[it->first] = it->second;
    }

    IceUtil::RWRecMutex::WLock lock(mutex_);
    ad_ctr_realtime_.swap(pool);
		MCE_INFO("CtrOnlineModel::LoadAdCtrRealTimePool --> original pool size is: "<<pool.size());
    MCE_INFO("CtrOnlineModel::LoadAdCtrRealTimePool --> pool size is:"  << ad_ctr_realtime_.size());          
  } catch (Ice::Exception& e) {
    MCE_WARN("CtrOnlineModel::LoadAdCtrRealTimePool --> call AdExperimentAdapter ERR  line:"<< __LINE__ << " err:" << e); 
  } catch (std::exception& e) {
    MCE_WARN("CtrOnlineModel::LoadAdCtrRealTimePool --> call AdExperimentAdapter ERR  line:"<< __LINE__ << " err:" << e.what());
  } catch (...) {
    MCE_WARN("CtrOnlineModel::LoadAdCtrRealTimePool --> call AdExperimentAdapter ERR : unknown error.");
  } 
  MCE_INFO("CtrOnlineModel::LoadAdCtrRealTimePool --> stop");          
}

/**************************************************
*  Load 点击率模型数据  
*
**************************************************/
void CtrOnlineModelCache::LoadCtrOnlineModelPool(){
  MCE_INFO("CtrOnlineModel::LoadCtrOnlineModelPool --> start");
  hash_map<string, CtrOnlineModel> pool;
  try {
    CtrOnlineModelMap temp 
      = AdExperimentAdapter::instance().GetCtrOnlineModelPool(EngineConfig::instance().server_index());

    for (CtrOnlineModelMap::iterator it = temp.begin(); it != temp.end(); ++it){
      pool[it->first] = it->second;
    }
    
    double init_weight = -10.0;
    hash_map<string, CtrOnlineModel>::iterator itp = pool.find(init_key_);
    if (itp != pool.end()) { 
      init_weight = itp->second.weight;
    }
    IceUtil::RWRecMutex::WLock lock(mutex_);
    ctr_online_model_.swap(pool);
    init_weight_ = init_weight; //更新0号特征权重
    MCE_INFO("CtrOnlineModel::LoadCtrOnlineModelPool --> pool size is:"  << ctr_online_model_.size() << " init_weight_ = " << init_weight_);          
  } catch (Ice::Exception& e) {
    MCE_WARN("CtrOnlineModel::LoadCtrOnlineModelPool --> call AdExperimentAdapter ERR  line:"<< __LINE__ << " err:" << e); 
  } catch (std::exception& e) {
    MCE_WARN("CtrOnlineModel::LoadCtrOnlineModelPool --> call AdExperimentAdapter ERR  line:"<< __LINE__ << " err:" << e.what());
  } catch (...) {
    MCE_WARN("CtrOnlineModel::LoadCtrOnlineModelPool --> call AdExperimentAdapter ERR : unknown error.");
  } 
  MCE_INFO("CtrOnlineModel::LoadCtrOnlineModelPool --> stop");          
}

/**************************************************
*  Load 广告分类数据  
*
**************************************************/
void CtrOnlineModelCache::LoadAdClassListPool(){
  MCE_INFO("CtrOnlineModel::LoadAdClassListPool --> start");
  try {
    AdClassListMap pool
       = AdExperimentAdapter::instance().GetAdClassListPool(EngineConfig::instance().server_index());

    IceUtil::RWRecMutex::WLock lock(mutex_);
    ad_class_.swap(pool);
    MCE_INFO("CtrOnlineModel::LoadAdClassListPool --> pool size is:"  << ad_class_.size());          
  } catch (Ice::Exception& e) {
    MCE_WARN("CtrOnlineModel::LoadAdClassListPool --> call AdExperimentAdapter ERR  line:"<< __LINE__ << " err:" << e); 
  } catch (std::exception& e) {
    MCE_WARN("CtrOnlineModel::loadAdClassListPool --> call AdExperimentAdapter ERR  line:"<< __LINE__ << " err:" << e.what());
  } catch (...) {
    MCE_WARN("CtrOnlineModel::loadAdClassListPool --> call AdExperimentAdapter ERR : unknown error.");
  } 
  MCE_INFO("CtrOnlineModel::LoadAdClassListPool --> stop");          
}

/**************************************************
*  Load 点击率特征配置，并分组：公共特征、广告特征、复合特征  
*
**************************************************/
void CtrOnlineModelCache::LoadAdCtrFeaturePool(){
  MCE_INFO("CtrOnlineModel::LoadAdCtrFeaturePool --> start");
  try {
    AdCtrFeatureList common_feature;
    AdCtrFeatureList complex_feature;
    AdCtrFeatureList ad_feature;
    AdCtrFeatureList pool
      = AdExperimentAdapter::instance().GetAdCtrFeaturePool(EngineConfig::instance().server_index());
    
    for (AdCtrFeatureList::iterator it = pool.begin(); it != pool.end(); ++it) {
      AdCtrFeature feature = (*it);
      switch (feature.type) {
        case 1: common_feature.push_back(feature); break;
        case 2: ad_feature.push_back(feature); break;
        case 3: complex_feature.push_back(feature); break;
        default:;
      }
    }  
    
    IceUtil::RWRecMutex::WLock lock(mutex_);
    all_feature_.swap(pool);//全部特征集合
    common_feature_.swap(common_feature); //公共特征集合
    ad_feature_.swap(ad_feature); //广告特征集合
    complex_feature_.swap(complex_feature); //复合特征集合
    MCE_INFO("CtrOnlineModel::LoadAdCtrFeaturePool --> all_feature size = "  << all_feature_.size() 
                                        << "complex_feature size = " << complex_feature_.size() 
                                        << "ad_feature size = " << ad_feature_.size()
                                        << "common_feature size = " << common_feature_.size());          
  } catch (Ice::Exception& e) {
    MCE_WARN("CtrOnlineModel::LoadAdCtrFeaturePool --> call AdExperimentAdapter ERR  line:"<< __LINE__ << " err:" << e); 
  } catch (std::exception& e) {
    MCE_WARN("CtrOnlineModel::loadAdCtrFeaturePool --> call AdExperimentAdapter ERR  line:"<< __LINE__ << " err:" << e.what());
  } catch (...) {
    MCE_WARN("CtrOnlineModel::loadAdCtrFeaturePool --> call AdExperimentAdapter ERR : unknown error.");
  } 
  MCE_INFO("CtrOnlineModel::LoadAdCtrFeaturePool --> stop");          
}

/**************************************************
* 更新实验比例和模型比例 
*
**************************************************/
void CtrOnlineModelCache::RefreshRatio(){
  hash_map<Ice::Long, int> lr_ctr_pool; 
  hash_map<Ice::Long, int> real_time_pool; 
  for (vector<Ice::Long>::iterator it = adzone_list_.begin(); it != adzone_list_.end(); ++it) {
    ostringstream buff;
    string key;
    
    buff << "lr_ctr_ratio_" << (*it);
    key = buff.str();
    int lr_ctr_ratio = EngineParametersCache::instance().GetIntParameterByName(key, 0);
    lr_ctr_pool[(*it)] = lr_ctr_ratio;
    buff.str("");
    buff << "real_time_ratio_" << (*it);
    key = buff.str();
    int real_time_ratio = EngineParametersCache::instance().GetIntParameterByName(key, 0);
    real_time_pool[(*it)] = real_time_ratio;
  }
  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    lr_ctr_ratio_.swap(lr_ctr_pool); 
    real_time_ratio_.swap(real_time_pool); 
  }
  MCE_INFO("CtrOnlineModelCache::RefreshRatio --> adzone_list_.size=" << adzone_list_.size() << " real_time_ratio_.size=" << real_time_ratio_.size() << "lr_ctr_ratio_.size=" << lr_ctr_ratio_.size());
}
/**************************************************
* 决定是否使用lr-点击率，按比例随机 
*
**************************************************/
bool CtrOnlineModelCache::UseLrCtr(Ice::Long zone_id){
  int ratio = 0;
  hash_map<Ice::Long, int>::iterator it = lr_ctr_ratio_.find(zone_id); 
  if (it != lr_ctr_ratio_.end())  {
    ratio = it->second;
  }
  return ((rand() % 100) < ratio);   
}
/**************************************************
* 决定是否使用realtime-点击率，按比例随机 
*
**************************************************/
bool CtrOnlineModelCache::UseRealTimeCtr(Ice::Long zone_id){
  int ratio = 0;
  hash_map<Ice::Long, int>::iterator it = real_time_ratio_.find(zone_id); 
  if (it != real_time_ratio_.end())  {
    ratio = it->second;
  }
  return ((rand() % 100) < ratio);   
}

/***********************
* 根据特征组合出点击率，特征配置见数据库
* 点击率state值为1024, 记入pvlog
*
************************/
pair<double, int> CtrOnlineModelCache::GetCtr(hash_map<int, string>& common_feature_data, 
    const AdGroupPtr& gptr, const double& common_weight){   
  
  AdCtrFeatureList feature_list = GetComplexFeatureList();
  //拼接公共部分特征和广告特征的组合特征的特征  
  hash_map<int, string> feature_data = gptr->GetAdCtrFeatureData(); //读取广告特征
  MergeCtrFeatureData(feature_data, common_feature_data);  
 
  double total_w = GetInitWeight() //0号特征权重
                 + common_weight //公共特征的权重和
                 + gptr->GetAdCtrFeatureWeight() //广告特征的权重和   
                 + GetFeatureWeightByList(feature_data, feature_list); //计算公共特征和广告特征的组合特征权重和 
  double ctr = 1.0/(1.0 + exp(-total_w));

  //MCE_INFO("DEBUG ctr : gid = " << gptr->group_id()<<  " ctr = " << ctr << " total_w = " << total_w << " common_w = " << common_weight);

  /******
   * test feature
   *
   feature_list = GetAllFeatureList();
   double test_w = GetInitWeight(); 
   MCE_INFO("Init key = 0 weight = " << test_w);
   test_w += GetTestFeatureWeightByList(feature_data, feature_list); //计算公共特征和广告特征的组合特征权重和
   double test_ctr = 1.0/(1.0 + exp(-test_w));
   MCE_INFO("DEBUG ctr : ctr = " << test_ctr << " total_w = " << test_w); 
  ******/
  return pair<double, int>(ctr, 1024);
}

/**************************************************
*  TEST 传入特征list，根据特征list，累加权重和 
*
**************************************************/
double CtrOnlineModelCache::GetTestFeatureWeightByList(hash_map<int, string>& feature_data, 
    AdCtrFeatureList& feature_list){
  double w = 0.0;
  for(AdCtrFeatureList::iterator it = feature_list.begin(); it != feature_list.end(); ++it) {
    string key = GetFeatureKey(*it, feature_data);
    if (!key.empty()) {
      double tmp = GetWeightByKey(key);
      MCE_INFO("DEBUG ctr: key = " << key << " w = " << tmp);
      w += tmp;
    } else {
      MCE_INFO("DEBUG ctr: key = " << key << " not found");
    }
  }   
  return w;
}
/**************************************************
*  传入特征list，根据特征list，累加权重和  
*
**************************************************/
double CtrOnlineModelCache::GetFeatureWeightByList(hash_map<int, string>& feature_data, 
    AdCtrFeatureList& feature_list){
  double w = 0.0;
  for(AdCtrFeatureList::iterator it = feature_list.begin(); it != feature_list.end(); ++it) {
    string key = GetFeatureKey(*it, feature_data);
    if (!key.empty()) {
      w += GetWeightByKey(key);
    }
  }   
  return w;
}

/**************************************************
*  获取特征key对应的权重 
*
**************************************************/
double CtrOnlineModelCache::GetWeightByKey(const string& key){
  IceUtil::RWRecMutex::RLock lock(mutex_);
  hash_map<string, CtrOnlineModel>::iterator itc = ctr_online_model_.find(key);
  if (itc != ctr_online_model_.end()) {
    return itc->second.weight;
  } else {
    return 0.0;
  }
}

/**************************************************
*  获取广告分类，目前是group_id
*
**************************************************/
AdClass CtrOnlineModelCache::GetAdClass(Ice::Long key){
  AdClass adclass;
  IceUtil::RWRecMutex::RLock lock(mutex_);
  AdClassListMap::iterator it = ad_class_.find(key);
  if (it != ad_class_.end()) {
    if (!it->second.empty()) {
      adclass = *(it->second.begin());
    }
  } else { //置空
    adclass.classify = string();
    adclass.pvalue = 0.0;
  }
  return adclass; 
}

/**************************************************
*  根据特征配置、特征数据组合出特征的key 
*
**************************************************/
string CtrOnlineModelCache::GetFeatureKey(AdCtrFeature& feature, hash_map<int, string>& feature_data){

  string key = feature.idstr;
  for (vector<AdCtrFeatureItem>::iterator it = feature.items.begin(); it != feature.items.end(); ++it) {
    string str;
    int len;
    hash_map<int, string>::iterator itf = feature_data.find(it->id);
    if (itf != feature_data.end()){
      str = itf->second;;
      len = str.length();
    } else {
      str = "";
      len = 0;
    }
    int suffix_len = ((len < it->vlen) ? (it->vlen - len) : 0);
    len += suffix_len;
    int prefix_len = ((len < it->len) ? (it->len - len) : 0); 
    if (prefix_len > 0){
      key += DEF_FEATURE0.substr(0, prefix_len);
    }
    key += str;
    if (suffix_len > 0) {
      key += DEF_FEATURE9.substr(0, suffix_len); 
    }
  }
  return key;
}

/**************************************************
* 填充公共特征、广告特征数据，用于复合特征权重的获取
*
**************************************************/
void CtrOnlineModelCache::MergeCtrFeatureData(hash_map<int, string>& feature_data, 
    hash_map<int, string>& common_feature_data){
  feature_data.insert(common_feature_data.begin(), common_feature_data.end());
}

/**************************************************
* 填充公共的基础feature: 用户信息、时间、刷新等
*
**************************************************/
void CtrOnlineModelCache::FillCommonFeatureData(hash_map<int, string>& feature_data, 
    const MatchHandlerParamter& para, AdSelectZoneInfoPtr& info) {  
  ostringstream buff;
  try{
    //ad_zone_id
    Ice::Long zone_id = info->AdZoneId();
    buff.str("");
    buff << zone_id;
    feature_data[3] = buff.str(); 
    //rank
    buff.str("");
    buff << info->Pos();
    feature_data[15] = buff.str(); 
    //site_id
    buff.str("");
    buff << zone_id / 10000; 
    feature_data[8] = buff.str(); 
  
    //refresh_idx
    if (!para.refresh_idx_.empty()) {
      feature_data[19] = para.refresh_idx_;
    } else {
      feature_data[19] = DEF_FEATURE0; // 15个0  
    }
 
    //填充用户特征
    UserProfile upf = para.userProfile_; 
    //gender
    if (0 == upf.gender() || 1 == upf.gender()) {
      buff.str("");
      buff << upf.gender();
      feature_data[9] = buff.str(); 
    } else {
      feature_data[9] = DEF_FEATURE9; // 默认15个9
    }
    //age
    if (upf.age() > 0) {
      buff.str("");
      buff << upf.age();
      feature_data[10] = buff.str();
    } else {
      feature_data[10] = DEF_FEATURE9; // 默认15个9
    }
    //stage
    if (2 == upf.stage() || 4 == upf.stage()) {
      buff.str("");
      buff << upf.stage();
      feature_data[11] = buff.str(); //stage
    } else {
      feature_data[11] = DEF_FEATURE9;  // 默认15个9
    }
    //school
    if (upf.school() > 0){
      buff.str("");
      buff << upf.school();
      feature_data[12] = buff.str(); //school
    } else {
      feature_data[12] = DEF_FEATURE9;  // 默认15个9
    }
    //area
    //string area = ((4 == upf.stage()) ? upf.ip_area() : upf.current_area()); 
    string area = upf.ip_area();
    feature_data[13] = (area.empty() ? DEF_FEATURE9 : area);   // 默认15个9
   
    //grade
    if (upf.grade() >= 0) {
      buff.str("");
      buff << upf.grade();
      feature_data[14] = buff.str(); 
    } else {
      feature_data[14] = DEF_FEATURE9;  // 默认15个9
    }
    //userid
    if (upf.id() > 0) {
      buff.str("");
      buff << upf.id(); 
      feature_data[17] = buff.str(); 
    } else {
      feature_data[17] = DEF_FEATURE9;  // 默认15个9
    } 
    // sum_ads
    buff.str("");
    buff << info->Count();
    feature_data[41] = buff.str();  
    // app_name 
    /*
    string app_name = info->AppName();
    if (app_name.empty()) {
      feature_data[47] = DEF_FEATURE9;  
    } else {
      feature_data[47] = app_name;  
    }
    */

    // 填充时间信息 
    struct tm *tt;
    time_t t = time(NULL);
    tt = gmtime(&t);
    //half_hour
    buff.str("");
    buff << (tt->tm_hour * 2 + tt->tm_min / 30);
    feature_data[4] = buff.str(); 
    //hour
    buff.str("");
    buff << tt->tm_hour;
    feature_data[16] = buff.str();  
    //week_day
    buff.str("");
    buff << tt->tm_wday;
    feature_data[5] = buff.str();
    //weekend
    buff.str("");
    buff << ((0 == tt->tm_wday || 6 == tt->tm_wday) ? 1 : 0);
    feature_data[50] = buff.str();
  } catch (Ice::Exception& e) {
    MCE_WARN("CtrOnlineModel::FillCommonFeatureData error : " << e.what()); 
  } catch (std::exception& e) {
    MCE_WARN("CtrOnlineModel::FillCommonFeatureData error : " << e.what()); 
  } catch (...) {
    MCE_WARN("CtrOnlineModel::FillCommonFeatureData error : unknown error."); 
  }
}

/**************************************************
* 填充广告的基础feature:
*
**************************************************/
void CtrOnlineModelCache::FillAdCtrFeatureData(hash_map<int, string>& feature_data, const AdGroupPtr& gptr){
  //填充广告特征
  ostringstream buff;
  try {
    //custom_id
    buff.str("");
    buff << gptr->member_id();
    feature_data[1] = buff.str(); 
    //ad_id
    buff.str("");
    Ice::Long creative_id = 0;
    if(!gptr->creative_ids().empty()){
      creative_id = *(gptr->creative_ids().begin());
    }
    buff << creative_id;
    feature_data[2] = buff.str(); 
    //FIXME group_id
    buff.str("");
    buff << gptr->group_id(); // group_id
    feature_data[6] = buff.str();  
    // FIXME plan_id
    buff.str("");
    buff << gptr->campaign_id(); // campaign_id
    feature_data[7] = buff.str();  
    //ctr  
    feature_data[18] = DEF_FEATURE0; // 15个0 
    //display_type
    buff.str("");
    buff << gptr->display_type();
    feature_data[27] = buff.str();
    //widget_id
    buff.str("");
    if (gptr->widget_id() > 0) {
      buff << gptr->widget_id();
    } else {
      buff << DEF_FEATURE9;
    }
    feature_data[46] = buff.str();
 
    // 填充分类信息 
    AdClass adclass = GetAdClass(gptr->group_id());
    //classify-1 ~ 5
    for (int i = 1; i <= 5; ++i) { 
      buff.str(""); 
      unsigned int vlen = 3 * i;
      if (adclass.classify.length() >= vlen){
        buff << adclass.classify.substr(0, vlen);      
      } else { 
        buff << adclass.classify << DEF_FEATURE9.substr(0, vlen - adclass.classify.length());
      }    
      feature_data[30 + i] = buff.str();   
    }
    //classify_pvalue
    buff.str("");
    if ((!adclass.classify.empty()) && adclass.pvalue > 0) {
      buff << adclass.classify;
      int pvalue = (int)(adclass.pvalue * 1000);
      if (pvalue < 10) {
        buff << "00";
      } else if (pvalue < 100) {
        buff << 0;
      }
      buff << pvalue;  
    } else {
      buff << DEF_FEATURE9 <<"000"; 
    } 
    feature_data[36] = buff.str();
    //classify
    buff.str("");
    if ((!adclass.classify.empty()) && adclass.pvalue > 0) {
      buff << adclass.classify;        
    } else {
      buff << DEF_FEATURE9;
    }
    feature_data[37] = buff.str();
  } catch (Ice::Exception& e) {
    MCE_WARN("CtrOnlineModel::FillAdCtrFeatureData error : " << e.what()); 
  } catch (std::exception& e) {
    MCE_WARN("CtrOnlineModel::FillAdCtrFeatureData error : " << e.what()); 
  } catch (...) {
    MCE_WARN("CtrOnlineModel::FillAdCtrFeatureData error : unknown error."); 
  }
}


/**************************************************
* 将lr产生的点击率和实时模型给出的点击率进行合并
* 返回最终的点击率 
*
**************************************************/
pair<double,int> CtrOnlineModelCache::GetMergedCtr(const UserProfile & user_profile, 
    long adzone, int pos, long group_id, pair<double,int> lr_ctr){
    
	// 按粒度由细到粗查找匹配的key
	for(int i = 1; i <= 6; ++i){
		string key = GetKeyByIndex(user_profile.gender(), user_profile.stage(), adzone, pos, group_id, i);
		AdCtrRealTimeItem real_time = GetRealTimeCtrByKey(key);
    if(real_time.weight > 0.0) {  
      string avg_key = GetKeyByIndex(user_profile.gender(), user_profile.stage(), adzone, pos, group_id, -i);
      AdCtrRealTimeItem average = GetRealTimeCtrByKey(avg_key);
      double merged_ctr = lr_ctr.first;
      int state = 10253;
      if (average.ctr < real_time.ctr){  
        state = 10255; 
      }
      if(lr_ctr.first < real_time.ctr) {
        state += 1;
      } else {
   			merged_ctr = pow(real_time.ctr, real_time.weight) * pow(lr_ctr.first, 1.0 - real_time.weight);
      }
 
      return pair<double, int>(merged_ctr, state);      
		}
	}
  
  string avg_key = GetKeyByIndex(user_profile.gender(), user_profile.stage(), adzone, pos, group_id, -1);
  AdCtrRealTimeItem average = GetRealTimeCtrByKey(avg_key);
  if (average.ctr < lr_ctr.first) {
    lr_ctr.second = 10251; 
  } else {
    lr_ctr.second = 10250; 
  }
	return lr_ctr;	
}

/**************************************************
* 根据index，返回对应维度的实时模型key 
*
**************************************************/
string CtrOnlineModelCache::GetKeyByIndex(int gender, int stage, long adzone, int pos, long group_id, int index){
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
	  default:  key << "total";	
	}

	return key.str();
} 
/**************************************************
*  获取特征key对应的实时模型 
*
**************************************************/
AdCtrRealTimeItem CtrOnlineModelCache::GetRealTimeCtrByKey(const string& key){
  AdCtrRealTimeItem res;
  res.pv = res.click = -1;
  res.weight = -1.0; 
  res.ctr = 0.0;
  IceUtil::RWRecMutex::RLock lock(mutex_);
  hash_map<string, AdCtrRealTimeItem>::iterator itc = ad_ctr_realtime_.find(key);
  if (itc != ad_ctr_realtime_.end()) {
    res = itc->second;
  }
  return res;
}

