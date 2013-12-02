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
	  
    if (pool.size() > 0) {
      IceUtil::RWRecMutex::WLock lock(mutex_);
      ad_ctr_realtime_.swap(pool);
    }
    MCE_INFO("CtrOnlineModel::LoadAdCtrRealTimePool -->  new size is: "<<pool.size() << "original size is:"  << ad_ctr_realtime_.size());          
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
*  TODO 添加diff判断，避免经常切换数据
**************************************************/
void CtrOnlineModelCache::LoadCtrOnlineModelPool(){
  MCE_INFO("CtrOnlineModel::LoadCtrOnlineModelPool --> start");
  hash_map<string, CtrOnlineModel> pool;
  try {
    int model_id = EngineConfig::instance().lr_model_id();
    string new_md5sum 
      = AdExperimentAdapter::instance().GetModelMd5sumById(EngineConfig::instance().server_index(), model_id);
    if (new_md5sum.empty() || new_md5sum == model_md5sum_) { //模型有变化时才需重新读取
      MCE_INFO("CtrOnlineModel::LoadCtrOnlineModelPool --> model has no change, give up this load attempt.");
      return ;
    }
    AdRefreshCtrRatioMap ratio_pool  //refresh idx 折算数据
      = AdExperimentAdapter::instance().GetAdRefreshCtrRatioPoolById(EngineConfig::instance().server_index(), model_id);

    CtrOnlineModelMap temp  // lr 模型数据
      = AdExperimentAdapter::instance().GetCtrOnlineModelPoolById(EngineConfig::instance().server_index(), model_id);

    for (CtrOnlineModelMap::iterator it = temp.begin(); it != temp.end(); ++it){
      pool[it->first] = it->second;
    }
    
    double init_weight = -10.0;
    hash_map<string, CtrOnlineModel>::iterator itp = pool.find(init_key_);
    if (itp != pool.end()) { 
      init_weight = itp->second.weight;
    }

    if (pool.size() > 0) {
      {
        IceUtil::RWRecMutex::WLock lock(mutex_);
        refresh_ctr_ratio_.swap(ratio_pool);
        ctr_online_model_.swap(pool);
        model_md5sum_ = new_md5sum;
        init_weight_ = init_weight; //更新0号特征权重
      }
      AdCtrFeatureDataCache::instance().Clear(); //重新缓存广告特征数据
    }
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
    if (pool.size() > 0) {
      IceUtil::RWRecMutex::WLock lock(mutex_);
      ad_class_.swap(pool);
    }
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
*  Load 广告商行业折衷数据  
*
**************************************************/
void CtrOnlineModelCache::LoadAdMemberIndustryPool(){
  MCE_INFO("CtrOnlineModel::LoadAdMemberIndustryPool --> start");
  try {
    AdMemberIndustryMap pool
       = AdExperimentAdapter::instance().GetAdMemberIndustryPool(EngineConfig::instance().server_index());
    if (pool.size() > 0) {
      IceUtil::RWRecMutex::WLock lock(mutex_);
      member_industry_.swap(pool);
    }
    MCE_INFO("CtrOnlineModel::LoadAdMemberIndustryPool --> pool size is:"  << member_industry_.size());          
  } catch (Ice::Exception& e) {
    MCE_WARN("CtrOnlineModel::LoadAdMemberIndustryPool --> call AdExperimentAdapter ERR  line:"<< __LINE__ << " err:" << e); 
  } catch (std::exception& e) {
    MCE_WARN("CtrOnlineModel::LoadAdMemberIndustryPool --> call AdExperimentAdapter ERR  line:"<< __LINE__ << " err:" << e.what());
  } catch (...) {
    MCE_WARN("CtrOnlineModel::LoadAdMemberIndustryPool --> call AdExperimentAdapter ERR : unknown error.");
  } 
  MCE_INFO("CtrOnlineModel::LoadAdMemberIndustryPool --> stop");
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
    if (pool.size() > 0){ 
      IceUtil::RWRecMutex::WLock lock(mutex_);
      all_feature_.swap(pool);//全部特征集合
      common_feature_.swap(common_feature); //公共特征集合
      ad_feature_.swap(ad_feature); //广告特征集合
      complex_feature_.swap(complex_feature); //复合特征集合
    }
    MCE_INFO("CtrOnlineModel::LoadAdCtrFeaturePool --> pool size = "<< pool.size()
                                        << "all_feature size = "  << all_feature_.size() 
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
* 更新实验比例/模型比例/等参数  TODO 优化这部分的实现
*
**************************************************/
void CtrOnlineModelCache::RefreshLrCtrParameters(){
  map<Ice::Long, int> lr_ctr_pool; 
  map<Ice::Long, int> real_time_pool; 
  ostringstream buff;
  string key;
  buff << "use_adgroup_member_industry";
  key = buff.str();
  int use_adgroup_member_industry 
    = EngineParametersCache::instance().GetParameterByName(key, (use_adgroup_member_industry_ ? 1:0 ));

  buff.str("");
  buff << "need_refresh_discount";
  key = buff.str();
  int need_refresh_discount 
    = EngineParametersCache::instance().GetParameterByName(key, (need_refresh_discount_ ? 1:0 ));

  for (vector<Ice::Long>::iterator it = adzone_list_.begin(); it != adzone_list_.end(); ++it) {
    buff.str("");
    buff << "lr_ctr_ratio_" << (*it);
    key = buff.str();
    int lr_ctr_ratio = EngineParametersCache::instance().GetParameterByName(key, 0);
    lr_ctr_pool[(*it)] = lr_ctr_ratio;
    buff.str("");
    buff << "real_time_ratio_" << (*it);
    key = buff.str();
    int real_time_ratio = EngineParametersCache::instance().GetParameterByName(key, 0);
    real_time_pool[(*it)] = real_time_ratio;
  }
  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    lr_ctr_ratio_.swap(lr_ctr_pool); 
    real_time_ratio_.swap(real_time_pool); 
    use_adgroup_member_industry_ = use_adgroup_member_industry; 
    need_refresh_discount_ = need_refresh_discount;
  }
  MCE_INFO("CtrOnlineModelCache::RefreshRatio --> adzone_list_.size=" << adzone_list_.size() << " real_time_ratio_.size=" << real_time_ratio_.size() << "lr_ctr_ratio_.size=" << lr_ctr_ratio_.size());
}
/**************************************************
* 决定是否使用lr-点击率，按比例随机 
*
**************************************************/
bool CtrOnlineModelCache::UseLrCtr(Ice::Long zone_id){
  int ratio = 0;
  IceUtil::RWRecMutex::RLock lock(mutex_);
  map<Ice::Long, int>::iterator it = lr_ctr_ratio_.find(zone_id); 
  if (it != lr_ctr_ratio_.end())  {
    ratio = it->second;
  }
  return ((rand() % 100) < ratio);   
}
/**************************************************
* 决定是否使用lr-点击率，按比例(uid/100)后分流
*
**************************************************/
bool CtrOnlineModelCache::UseLrCtr(Ice::Long zone_id, Ice::Long uid){
  int ratio = 0;
  IceUtil::RWRecMutex::RLock lock(mutex_);
  map<Ice::Long, int>::iterator it = lr_ctr_ratio_.find(zone_id); 
  if (it != lr_ctr_ratio_.end())  {
    ratio = it->second;
  }
  if (uid < 10) { // 处理异常uid, uid < 10 认为是异常, uid可能<0, 无uid时指定uid=2
    uid = rand() % 10000;
  }
  return ((uid / 100) % 100) < ratio;   
}
/**************************************************
* 决定是否使用realtime-点击率，按比例随机 
*
**************************************************/
bool CtrOnlineModelCache::UseRealTimeCtr(Ice::Long zone_id){
  int ratio = 0;
  IceUtil::RWRecMutex::RLock lock(mutex_);
  map<Ice::Long, int>::iterator it = real_time_ratio_.find(zone_id); 
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
pair<double, int> CtrOnlineModelCache::GetCtr(
    map<int, string>& common_feature_data,
    const AdGroupPtr& gptr, const pair<double, double>& common_w_c,
    AdSelectZoneInfoPtr& info, double& confidence){
  
  AdCtrFeatureList feature_list = GetComplexFeatureList();
  //拼接公共部分特征和广告特征的组合特征的特征  
  AdCtrFeatureDataPtr gdata = AdCtrFeatureDataCache::instance().Get(gptr);
  map<int, string> feature_data = gdata->FeatureData(); //读取广告特征
  feature_data.insert(common_feature_data.begin(), common_feature_data.end());
  pair<double, double> weight_confidence = GetFeatureWeightByList(feature_data, feature_list); //计算公共特征和广告特征的组合特征权重和 
  double total_w = GetInitWeight() //0号特征权重
                 + common_w_c.first //公共特征的权重和
                 + gdata->SumWeight() //广告特征的权重和   
                 + weight_confidence.first;

  double ctr = 1.0/(1.0 + exp(-total_w));
  if (1 == need_refresh_discount_) {
    double ratio = GetRefreshCtrRatio(info); 
    //MCE_DEBUG("before discount ctr = " << ctr << ", after discount ctr = " << ctr * ratio);
    ctr *= ratio;
  }

  // 置信度检查
  confidence = common_w_c.second + gdata->SumConfidence() + weight_confidence.second;
  double lr_confidence_th = EngineConfig::instance().lr_confidence_th();
  if (confidence < lr_confidence_th) { //置信度不足的，进行ctr打压
    double lr_confidence_discount = EngineConfig::instance().lr_confidence_discount();
    ctr *= lr_confidence_discount;
  }
  //MCE_INFO("DEBUG ctr : gid = " << gptr->group_id()<<  " ctr = " << ctr << " total_w = " << total_w << " common_w = " << common_weight);

  int lr_demo_state = EngineConfig::instance().lr_demo_state();
  return pair<double, int>(ctr, lr_demo_state);
}

/**************************************************
*  传入特征list，根据特征list，累加权重和  
*
**************************************************/
pair<double, double> CtrOnlineModelCache::GetFeatureWeightByList(map<int, string>& feature_data, 
    AdCtrFeatureList& feature_list){
  pair<double, double> res(0.0, 0.0);
  for(AdCtrFeatureList::iterator it = feature_list.begin(); it != feature_list.end(); ++it) {
    string key = GetFeatureKey(*it, feature_data);
    if (!key.empty()) {
      pair<double, double> w_c = GetWeightByKey(key, *it);
      res.first += w_c.first;
      res.second += w_c.second;
    }
  }   
  return res;
}
/**************************************************
*  获取特征key对应的权重及置信度 
*
**************************************************/
pair<double, double> CtrOnlineModelCache::GetWeightByKey(const string& key, const AdCtrFeature& feature){
  double weight = 0.0;
  double confidence = 0.0;
  IceUtil::RWRecMutex::RLock lock(mutex_);
  hash_map<string, CtrOnlineModel>::iterator itc = ctr_online_model_.find(key);
  if (itc == ctr_online_model_.end()) {
    if (feature.useDefault > 0) {  //找不到的时候去取该特征的默认值特征值
      if (feature.filterLen > 0) { 
        int default_len = key.length() - feature.filterLen;
        if (default_len > 0) {
          itc = ctr_online_model_.find(key.substr(0, default_len)); //使用精细默认特征
        }
      } else {
        itc = ctr_online_model_.find(feature.idstr + DEF_FEATURE9); //使用全局默认特征
      }
    } 
  } else {
    confidence = feature.confidence; //命中特征，获取置信度权重
  }
  if (itc != ctr_online_model_.end()) {
    weight = itc->second.weight; 
  } 
  return pair<double, double>(weight, confidence);
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
string CtrOnlineModelCache::GetFeatureKey(AdCtrFeature& feature, map<int, string>& feature_data){

  string key = feature.idstr;
  for (vector<AdCtrFeatureItem>::iterator it = feature.items.begin(); it != feature.items.end(); ++it) {
    string str;
    int len;
    map<int, string>::iterator itf = feature_data.find(it->id);
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

/*************************************************
* 小时分段特征, 4段，0~8 9~14 15~18 19~23
*
*************************************************/
string Hour4Seg(int hour) {
  string hour_seg = "0"; //[0, 8]
  if (hour > 8 ) {
    if (hour < 15) { //[9, 14]
      hour_seg = "1";
    } else if ( hour < 19) { //[15, 18]
      hour_seg = "2";
    } else { //[19, 23]
      hour_seg = "3";
    }
  }
  return hour_seg;
}

/*************************************************
* 年龄分段特征: ~16 17~20 21~24 25~29 其它
*
*************************************************/
string AgeSeg(int age){
  string age_seg = "5"; //其它
  if (age > 20) { //先判断主要用户群
    if (age < 25) {
      age_seg = "3"; //[21, 24]
    } else if (age < 30) {
      age_seg = "4"; //[25, 29]
    }
  } else if (age > 16){
    age_seg = "2"; //[17, 20]
  } else {
    age_seg = "1"; //[ , 16]
  }
  return age_seg;
}

/**************************************************
* 填充公共的基础feature: 用户信息、时间、刷新等
*
**************************************************/
void CtrOnlineModelCache::FillCommonFeatureData(map<int, string>& feature_data, 
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
    if (1 == need_refresh_discount_) { //如果启用折算，这refresh_idx特征使用0
      feature_data[19] = "0";
    } else {
      if (!para.refresh_idx_.empty()) {
        feature_data[19] = para.refresh_idx_;
      } else {
        feature_data[19] = DEF_FEATURE0; // 15个0  
      }
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
    //age_seg
    feature_data[120] = AgeSeg(upf.age());
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
    
    //province
    if (area.empty()) {
      feature_data[95] = DEF_FEATURE9; 
    } else {
      feature_data[95] = (area.length() >= 6 && area.substr(0, 4) == "0086") ? area.substr(0, 6) : DEF_FEATURE9.substr(0, 6);
    }
 
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

    //refer_pattern 为了性能和内存，暂时不支持

    //refer 为了性能和内存，暂时不支持
    
    //refer_hash
    buff.str("");
    buff << ReferHash(para.referer_, zone_id);   
    feature_data[102] = buff.str();

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
    //hour_4
    feature_data[115] = Hour4Seg(tt->tm_hour); 
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
void CtrOnlineModelCache::FillAdCtrFeatureData(map<int, string>& feature_data, const AdGroupPtr& gptr){
  //填充广告特征
  ostringstream buff;
  try {
    //custom_id
    buff.str("");
    buff << gptr->member_id();
    feature_data[1] = buff.str(); 
    //ad_id
    buff.str("");
    buff << gptr->creative_id_;
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
    //level_2_industry  二级行业 
    int member_industry;
    if (1 == use_adgroup_member_industry_) {
      member_industry = gptr->member_industry();
    } else { // 使用折衷方案的member行业数据
      member_industry = GetMemberIndustry(gptr->member_id());
    }
    if (member_industry < 0) {
      member_industry = 0;
    }
    buff.str("");
    buff << member_industry;
    feature_data[98] = buff.str();
    //level_1_industry  一级行业
    if (member_industry > 99) {
      member_industry /= 100;
    } 
    buff.str("");
    buff << member_industry;
    feature_data[97] = buff.str();
    
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
/*
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
*/
/**************************************************
* 根据index，返回对应维度的实时模型key 
*
**************************************************/
/*
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
*/
 
/**************************************************
*  获取特征key对应的实时模型 
*
**************************************************/
/*
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
*/

/*******************************************
* DJBHash， 用来hash refer_pattern
*
*******************************************/
int DJBHash(const string& input){
  int length = input.length(); 
  int hash = 5381;    
  for(int i = 0; i < length; ++i) {    
    hash = (((hash << 5) + hash) + input[i]) & 0x7FFFFFFF;    
  }
  return hash;   
} 
/*******************************************
* 替换pattern中的uid
*
*******************************************/
void ReplaceUid(string& pattern){
  boost::regex reg("(/[0-9]+/)"); 
  pattern = boost::regex_replace(pattern, reg, "/uid/"); 
} 
/******************************************
* boost正则捕获
*
*******************************************/
bool RegexFetch(const string& str, boost::regex& expr, string& res){
  boost::smatch what;
  string::const_iterator start = str.begin();
  string::const_iterator end = str.end();
  if (boost::regex_search(start, end, what, expr)) {
    string msg(what[0].first, what[0].second);
    res = msg;
    return true;
  }
  return false;
}

/*******************************************
* 获取refer_pattern, 并hash之
*
*******************************************/
int CtrOnlineModelCache::ReferHash(const string& refer, Ice::Long zone_id) {
  string pattern;
  bool fetched = false;
  int cnt = 0;
  for (vector<boost::regex>::iterator it = refer_pattern_regex_.begin(); it != refer_pattern_regex_.end(); ++it, ++cnt) { 
    if (RegexFetch(refer, *it, pattern)){
      if (cnt < 3) { // need replace uid
        ReplaceUid(pattern);
      }
      if ((!pattern.empty()) && (0 == cnt || 4 == cnt)) {// need erase last char
        pattern.erase(pattern.end() - 1);
      }
      fetched = true;
      break;
    }
  }  
  int hash = DJBHash(fetched ? pattern : refer);
  //MCE_DEBUG("DEBUG ctr: refer = " << refer << " regex pattern =" << pattern << " hash = " << hash << " fetched = " << fetched << " cnt = " << cnt);
  return hash;
}
/*******************************************
* 初始化refer_pattern 的正则
*
*******************************************/
void CtrOnlineModelCache::InitReferPatternRegex(){
  vector<string> regexSet;
  //home page refer with uid pattern
  regexSet.push_back("http://www.renren.com/([0-9]+)#//(.*?)[:/\?#]"); //special home page 
  regexSet.push_back("http://www.renren.com/([0-9]+)"); //nomal home page
  //photo page refer pattern
  regexSet.push_back("http://photo.renren.com/photo/([0-9]+)/(tag/|photo/|photo-|album/|photonew-|album-|latest/photo-)");  
  regexSet.push_back("http://photo.renren.com/get(album|photo).do");
  //normal page refer pattern
  regexSet.push_back("http://.*?renren.com/.*?[/\?#:]");   

  for (vector<string>::iterator it = regexSet.begin(); it != regexSet.end(); ++it) {
    boost::regex expression(*it);
    refer_pattern_regex_.push_back(expression);  
  }
}

/********************************************
* 重新指定rank，即pos
*
********************************************/
void CtrOnlineModelCache::ReassignPos(map<int, string>& feature_data, int pos){
  //rank
  ostringstream buff;  
  buff << pos;
  feature_data[15] = buff.str();   
}

int CtrOnlineModelCache::GetMemberIndustry(Ice::Long member_id){
  int res = 0;
  IceUtil::RWRecMutex::RLock lock(mutex_);
  AdMemberIndustryMap::iterator it = member_industry_.find(member_id);
  if (it != member_industry_.end()) {
    res = it->second;
  }
  return res;
}
/***************************************
* 获取点击率按刷新次数的折算比例
*
****************************************/
double CtrOnlineModelCache::GetRefreshCtrRatio(AdSelectZoneInfoPtr& info){
  double res = 1.0; //默认不折算
  ostringstream buff;
  buff << info->AdZoneId() << info->Pos() << info->RefreshIdx();
  string key = buff.str();
  IceUtil::RWRecMutex::RLock lock(mutex_);
  AdRefreshCtrRatioMap::iterator it = refresh_ctr_ratio_.find(key); 
  if (it != refresh_ctr_ratio_.end()) {
    res = it->second;
  } 
  return res;
}
/*************************************
* 清空特征数据缓存
*
**************************************/
void AdCtrFeatureDataCache::Clear(){
  hash_map<Ice::Long, AdCtrFeatureDataPtr> temp;
  {  
    IceUtil::RWRecMutex::WLock lock(mutex_);
    cache_.swap(temp);
  }
  MCE_INFO("AdCtrFeatureDataCache::Clear --> clear old data , size = "<< temp.size());
}
/*************************************
* 特征数据缓存获取接口
*
**************************************/
AdCtrFeatureDataPtr AdCtrFeatureDataCache::Get(const AdGroupPtr& gptr){
  Ice::Long key = gptr->group_id();
  {
    IceUtil::RWRecMutex::RLock lock(mutex_); 
    hash_map<Ice::Long, AdCtrFeatureDataPtr>::iterator it = cache_.find(key);
    if (it != cache_.end()) {
      return it->second;
    } 
  }
  MCE_INFO("AdCtrFeatureDataCache::Get --> group_id = " << key << " not found, recache it." );
  //若没有数据,则填充并缓存
  AdCtrFeatureDataPtr gdata = new AdCtrFeatureData();
  FillOneFeatureData(gdata, gptr);
  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    cache_.insert(pair<Ice::Long, AdCtrFeatureDataPtr> (key, gdata));
  }
  return gdata;
}
/*****************************************
* 填充广告维度的特征数据, 并计算这部分的weight和置信度
*
*****************************************/
void AdCtrFeatureDataCache::FillOneFeatureData(AdCtrFeatureDataPtr& gdata, const AdGroupPtr& gptr) {
  map<int, string> feature_data; 
  CtrOnlineModelCache::instance().FillAdCtrFeatureData(feature_data, gptr);
  AdCtrFeatureList feature_list = CtrOnlineModelCache::instance().GetAdFeatureList(); 
  pair<double, double> w_c= CtrOnlineModelCache::instance().GetFeatureWeightByList(feature_data, feature_list);//获取广告特征权重和 
  gdata->SetSumWeight(w_c.first);
  gdata->SetSumConfidence(w_c.second);
  gdata->SetFeatureData(feature_data); 
}

/*****************************************
* 广告额外特征load, 从AdExperimentLoaderCache服务获取
*
*****************************************/
void AdExtraDataCache::LoadAdExtraDataPool(){
  MCE_INFO("AdExtraDataCache::LoadAdExtraDataPool --> start");
  try {
    AdExtraDataMap pool
       = AdExperimentAdapter::instance().GetAdExtraDataPool(EngineConfig::instance().server_index());
    if (pool.size() > 0) {
      IceUtil::RWRecMutex::WLock lock(mutex_);
      ad_extra_data_.swap(pool);
    }
    MCE_INFO("AdExtraDataCache::LoadAdExtraDataPool --> pool size is:"  << ad_extra_data_.size());
  } catch (Ice::Exception& e) {
    MCE_WARN("AdExtraDataCache::LoadAdExtraDataPool --> call AdExperimentAdapter ERR  line:"<< __LINE__ << " err:" << e);
  } catch (std::exception& e) {
    MCE_WARN("AdExtraDataCache::loadAdExtraDataPool --> call AdExperimentAdapter ERR  line:"<< __LINE__ << " err:" << e.what())
;
  } catch (...) {
    MCE_WARN("AdExtraDataCache::loadAdExtraDataPool --> call AdExperimentAdapter ERR : unknown error.");
  }
  MCE_INFO("AdExtraDataCache::LoadAdExtraDataPool --> stop");
}
/******************************************
* 广告额外特征获取接口, Get
*
******************************************/
AdExtraData AdExtraDataCache::Get(const AdGroupPtr& gptr){
  Ice::Long key = gptr->group_id();
  {
    IceUtil::RWRecMutex::RLock lock(mutex_);
    AdExtraDataMap::iterator it = ad_extra_data_.find(key);
    if (it != ad_extra_data_.end()) {
      return it->second;
    }
  }
  //MCE_DEBUG("AdExtraDataCache::Get --> group_id = " << key << " not found." );
  AdExtraData extra_data;
  return extra_data;
}

/*******************************************
* 判断bayes点击率是否可用, state见UserDemographicCache.cpp 
*
*******************************************/
bool noBayesCtr(int bayes_state) {
  return 3 == bayes_state; 
}
/**********************************************
*  A. 优先lr置信度不足, 直接bayes替换并进行打折
*  B. bayes点击率很差时, 放弃调整操作, 用lr原始值
*  C. 按p*ctr_lr + (1-p)*ctr_bayes, 其中p 与lr的置信度相关
*
**********************************************/
void CtrOnlineModelCache::AdjustCtr(
    pair<double, int>& ctr_state, const pair<double, int>& lr_state, const pair<double, int>& bayes_state, 
    const UserIndexSelectPara& select_para, double lr_confidence){
  
  if (lr_confidence < select_para.lr_bayes_confidence_th) { //直接替换并打折策略
    ctr_state.first = bayes_state.first * select_para.lr_bayes_discount;
    ctr_state.second = 1025;
    return ;
  }
 
  if (noBayesCtr(bayes_state.second)) {//没有bayes点击率, 无需调权 
    ctr_state = lr_state;
    return ;
  }

  double p = 1.0;   //融合权重
  if (lr_confidence > select_para.lr_confidence_th0) { //完全置信
    p = select_para.lr_weight_0;
  } else if (lr_confidence > select_para.lr_confidence_th1){ //部分置信
    p = select_para.lr_weight_1;
  } else {  //完全不置信
    p = select_para.lr_weight_2;
  }
  if (p > 1.0 || p < 0.0) { //异常参数
    p = 1.0;
  }
  ctr_state.first = p * lr_state.first + (1.0 - p) * bayes_state.first; // p*ctr1 + (1-p)*ctr2
  ctr_state.second = lr_state.second;
}


