#ifndef AD_EXPERIMENT_ICE
#define AD_EXPERIMENT_ICE

module xce{
module ad{ 

/***********************ExpDataStruct************************/
//点击率模型数据
struct CtrOnlineModel {
  int id;
  long pv;
  long click;
  double ctr;
  double weight;
};
dictionary<string, CtrOnlineModel> CtrOnlineModelMap;
//广告分类数据
struct AdClass{
  string classify;
  double pvalue;
}; 
sequence<AdClass> AdClassList;
dictionary<long, AdClassList> AdClassListMap;

//广告额外特征数据
dictionary<string, string> AdExtraData;
dictionary<long, AdExtraData> AdExtraDataMap;

//点击率特征数据
struct AdCtrFeatureItem{
  int id;
  int vlen;
  int len;
  int type;
};
sequence<AdCtrFeatureItem> AdCtrFeatureItems;
struct AdCtrFeature{
  AdCtrFeatureItems items;
  int id; 
  int type;
  int useDefault;
  int filterLen;
  double confidence;
  string idstr;
};
sequence<AdCtrFeature> AdCtrFeatureList;
//点击率实时模型数据
struct AdCtrRealTimeItem {
	long pv;
	int click;
	double ctr;	
  double weight;	
};
dictionary<string, AdCtrRealTimeItem> AdCtrRealTimeMap; 
dictionary<string, double> AdRefreshCtrRatioMap;
dictionary<long, int> AdMemberIndustryMap;

/***********************InterFace************************/
interface AdExperimentManager {
  AdCtrRealTimeMap GetAdCtrRealTimePool(); 
  CtrOnlineModelMap GetCtrOnlineModelPoolById(int id);
  AdClassListMap GetAdClassListPool();
  AdExtraDataMap GetAdExtraDataPool();
  AdCtrFeatureList GetAdCtrFeaturePool();
  AdRefreshCtrRatioMap GetAdRefreshCtrRatioPoolById(int id);
  AdMemberIndustryMap GetAdMemberIndustryPool(); 
  string GetModelMd5sumById(int id);
};

};
};

#endif
