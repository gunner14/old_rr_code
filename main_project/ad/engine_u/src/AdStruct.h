/*
 * AdStruct.h
 *
 *  Created on: Aug 25, 2010
 *      Author: antonio
 */

#ifndef ADSTRUCT_H_
#define ADSTRUCT_H_

#include <time.h>
#include <vector>
#include <map>
#include <ext/hash_map>
#include <set>
#include <bitset>
#include <IceUtil/RWRecMutex.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>

#include "TaskManager.h"
#include "AdTranslator.h"
#include "QueryRunner.h"
#include "Singleton.h"
#include "Util.h"
#include "AdConfig.h"
#include "FeedMemcProxy/client/user_profile_client.h"
#include "AdEngine.h"

namespace xce {
namespace ad {
using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace __gnu_cxx;

class StatFunc {
  string name_;
  timeval start_, end_;
  //static int count;
public:
  StatFunc(string name) : name_(name) {
    gettimeofday(&start_, NULL);
  }

  ~StatFunc() {
    gettimeofday(&end_, NULL);
    float timeuse=1000000*(end_.tv_sec-start_.tv_sec)+end_.tv_usec
        -start_.tv_usec;
    timeuse/=1000;
    if(timeuse >= 20){
      MCE_INFO(name_ << " time usage:" << timeuse << " ms");
    } else {
      MCE_DEBUG(name_ << " time usage:" << timeuse << " ms");
    }
  }
};

class AdZone;
typedef IceUtil::Handle<AdZone> AdZonePtr;
typedef vector<AdZone> AdZoneSeq;
typedef map<int, AdZonePtr> AdZoneDict;

class DictNode;
class DictTree;
typedef IceUtil::Handle<DictNode> DictNodePtr;
typedef IceUtil::Handle<DictTree> DictTreePtr;
typedef hash_map<char, DictNodePtr> Char2NodeMap;

class DictNode : public IceUtil::Shared {
public:
  DictNode() {
    wordend_ = false;
  }
  DictNodePtr insert(const char& c, bool wordend);
  void GetNext2(const char& c, DictNodePtr&, bool lastisinteger , bool search_end = false);
  void GetNext(const char& c, DictNodePtr&, bool search_end = false);

  vector<AdZonePtr> referer_zones_;
  Char2NodeMap node_;
  bool wordend_;
};

class DictTree : public IceUtil::Shared {
public:
  DictTree() {
    root_ = new DictNode();
  }
  void insert(const string& str, const AdZonePtr& zone_ptr);
  bool search(const string& str, vector<AdZonePtr>& zones);
private:
  DictNodePtr root_;
};

/***********************************************************************************************************************/
class AdGroup;
typedef IceUtil::Handle<AdGroup> AdGroupPtr;
typedef vector<AdGroupPtr> AdGroupSeq;

class SelectedAdGroup;
typedef IceUtil::Handle<SelectedAdGroup> SelectedAdGroupPtr;
typedef vector<SelectedAdGroupPtr> SelectedAdGroupSeq;

class AdZone : public IceUtil::Shared {
public:
  AdZone() {
    member_online_status_ = 0;
    am_online_status_ = 0;
    audit_status_ = 0;
    delete_flag_ = 0;
  }
  bool Available();

  bool FindStage(int stage);

  Ice::Long id();
  int bp_count() const {
    return bp_count_;
  }
  void set_bp_count(int bp_count) {
    this->bp_count_ = bp_count;
  }
  int brand_count() const {
    return brand_count_;
  }
  void set_brand_count(int brand_count) {
    this->brand_count_ = brand_count;
  }
  int member_type() const {
    return member_type_;
  }
  void set_member_type(int member_type) {
    this->member_type_ = member_type;
  }
  int weight();
  set<string> referers();
  void set_id(Ice::Long id);
  void add_stage(int stage);
  void set_weight(int weight);
  void set_referer(const string& ref);
  void set_member_online_status(int member_online_status);
  void set_am_online_status(int am_online_status);
  void set_audit_status(int audit_status);
  void set_delete_flag(int delete_flag);
private:
  Ice::Long id_; //zone id
  int brand_count_; // 可显示的品牌广告数
  int bp_count_; // 可显示的中小自助广告数
  int weight_; //广告位展示权重
  set<int> stage_;//能展示这个广告位的阶段
  set<string> referer_;//这个广告位请求的referer

  int member_type_;

  int member_online_status_;
  int am_online_status_;
  int audit_status_;
  int delete_flag_;
};

const static int zone_reload_timestamp = 1 * 60 * 1000;

class AdZonePool : public MyUtil::Singleton<AdZonePool> {
public:
  AdZonePool() {
    trie_ = new DictTree();
  }
  void Init();
  void LoadDefaultAdGroup(map<Ice::Long, vector<AdGroupPtr> > &zone_groups);
  AdZoneDict GetAdZoneOrdered(const string& referer, const UserProfile& userprofile, bool hide = false);
  AdZoneDict GetAdZoneOrdered(const ZoneInfoMap& zone_info_map, const UserProfile& userprofile);
  vector<AdGroupPtr>  GetDefaultGroups(Ice::Long zone_id);
	bool IsCompetedDefaultGroup(AdGroupPtr group, set<AdGroupPtr> &selected_ids);
	void GetDefaultGroupsCompete(const Ice::Long adzone,const int count, set<AdGroupPtr> selected,const vector<AdGroupPtr> &groups, set<AdGroupPtr> &ans);

  class ReloadTimer : public MyUtil::Timer {
  public:
    ReloadTimer() :
      Timer(zone_reload_timestamp) {
    }
    virtual void handle();
  };
  AdZonePtr GetAdZone(Ice::Long id) {
    map<Ice::Long, AdZonePtr>::iterator it = zone_pool_.find(id);
    if (it != zone_pool_.end()) {
      return it->second;
    } else {
      return NULL;
    }
  }
private:
  DictTreePtr trie_;
  multimap<string, AdZonePtr> ref_zone_map_;
  map<Ice::Long, AdZonePtr> zone_pool_;
  map<Ice::Long, std::vector<AdGroupPtr> > default_group_pool_;
  IceUtil::RWRecMutex mutex_;

  void LoadAdzone(map<Ice::Long, AdZonePtr> & zones);
  void LoadAdgroupAdzoneRef(map<Ice::Long, AdZonePtr> & zones);
  void LoadUrlAdzoneMaping(map<Ice::Long, AdZonePtr> & zones, DictTreePtr & trie, multimap<string, AdZonePtr> & ref_zone_map);
};

/***********************************************************************************************************************/
class AdStyle : public IceUtil::Shared {
public:

  Ice::Long style_id() const {
    return style_id_;
  }
  void set_style_id(Ice::Long style_id) {
    this->style_id_ = style_id;
  }
  void set_ad_cnt( int ad_cnt){
    this->ad_cnt_ = ad_cnt;
  }
  int ad_cnt() const{
    return ad_cnt_; 
  }
  string note() const{
    return this->note_;
  }
  void set_note(string note ){
    this->note_ = note;
  }
  void set_submit_time(string submit_time){
    this->submit_time_ = submit_time;
  }
  string submit_time() const{
    return submit_time_;
  }
  void set_big_widgets(vector<int>& big_widgets){
    this->big_widgets_ = big_widgets;
  }
  vector<int> big_widgets() const{
    return this->big_widgets_; 
  }
  void set_small_widgets(vector<int>& small_widgets){
    this->small_widgets_ = small_widgets;
  }
  vector<int> small_widgets() const{
    return this->small_widgets_; 
  }
  static vector<int> String2intvec(string str , string sep);

private:
  Ice::Long style_id_;
  int ad_cnt_;
  string note_;
  string submit_time_;
  vector<int> big_widgets_;
  vector<int> small_widgets_;
};
typedef IceUtil::Handle<AdStyle> AdStylePtr;
class AdStylePool : public MyUtil::Singleton<AdStylePool> {
public:
  void Init();
  AdStylePtr GetAdStyle(Ice::Long style_id);
  class UpdateCheckTimer : public MyUtil::Timer {
  public:
    UpdateCheckTimer() :
      Timer(1 * 60 * 1000) {
    }
    virtual void handle();
  };
private:
  map<Ice::Long, AdStylePtr> style_pool_;
  IceUtil::RWRecMutex mutex_;
};

/***********************************************************************************************************************/

class AdExperiment : public IceUtil::Shared {
public:

  Ice::Long experiment_id() const {
    return experiment_id_;
  }
  void set_experiment_id(Ice::Long experiment_id) {
    this->experiment_id_ = experiment_id;
  }
  void set_style_id( Ice::Long style_id){
    this->style_id_ = style_id;
  }
  Ice::Long style_id() const{
    return style_id_; 
  }
  void set_status( int status){
    this->status_ = status;
  }
  int status() const{
    return status_; 
  }
  void set_traffic( int traffic){
    this->traffic_ = traffic;
  }
  int traffic() const{
    return traffic_; 
  }
  void set_zone_id( Ice::Long zone_id){
    this->zone_id_ = zone_id;
  }
  Ice::Long zone_id() const{
    return zone_id_; 
  }
  void set_submit_time(string submit_time){
    this->submit_time_ = submit_time;
  }
  string submit_time() const{
    return submit_time_;
  }
  void set_modify_time(string modify_time){
    this->modify_time_ = modify_time;
  }
  string modify_time() const{
    return modify_time_;
  }
private:
  Ice::Long experiment_id_;
  Ice::Long zone_id_;
  Ice::Long style_id_;
  int traffic_;
  int status_;
  string modify_time_;
  string submit_time_;
};
typedef IceUtil::Handle<AdExperiment> AdExperimentPtr;
class AdExperimentPool : public MyUtil::Singleton<AdExperimentPool> {
public:
  void Init();
  AdExperimentPtr GetAdExperiment(Ice::Long experiment_id);
  vector<AdExperimentPtr> ListAdExperiment();
  static bool compare(AdExperimentPtr exp1 , AdExperimentPtr exp2){
    if( exp1->zone_id() == exp2->zone_id()){
      return exp1->traffic() < exp2->traffic();
    }else{
      return exp1->zone_id()< exp2->zone_id(); 
    }
  }
  class UpdateCheckTimer : public MyUtil::Timer {
  public:
    UpdateCheckTimer() :
      Timer(1 * 60 * 1000) {
    }
    virtual void handle();
  };
private:
  map<Ice::Long, AdExperimentPtr> experiment_pool_;
  IceUtil::RWRecMutex mutex_;
};


class AdGroup : public IceUtil::Shared {
public:
  void set_group_id(Ice::Long group_id) {
    group_id_ = group_id;
  }
  void set_widget_id(Ice::Long widget_id) {
    widget_id_ = widget_id;
  }
  void set_member_id(Ice::Long member_id) {
    member_id_ = member_id;
  }
  void set_campaign_id(Ice::Long campaign_id) {
    campaign_id_ = campaign_id;
  }
  void set_member_category(int member_category) {
    member_category_ = member_category;
  }
  void set_member_industry(int member_industry) {
    member_industry_ = member_industry;
  }
  void set_display_type(int display_type) {
    display_type_ = display_type;
  }
  void set_max_price(int max_price) {
    max_price_ = max_price;
  }
    
  void set_bid_unit_id(Ice::Long bid_unit_id) {
    bid_unit_id_ = bid_unit_id;
  }
  Ice::Long group_id() {
    return group_id_;
  }
  Ice::Long member_id() {
    return member_id_;
  }
  Ice::Long widget_id() {
    return widget_id_;
  }
  Ice::Long campaign_id() {
    return campaign_id_;
  }
  int member_category() {
    return member_category_;
  }
  int member_industry() {
    return member_industry_;
  }
  int display_type() {
    return display_type_;
  }
  double max_price() {
    return max_price_;
  }
  double calculate_cost(Ice::Long zone_id) {
    float cost_discount = EngineConfig::instance().GetCostDiscount();
    int rand_seed = EngineConfig::instance().GetRandSeed();
    double max_price = this->max_price();
    double cost = ((cost_discount + rand() % rand_seed / 10000.0) * (max_price));
    if(IsCpc()){	//cpc
    	if (cost < EngineConfig::instance().sys_min_price()) { //不能低于系统最小出价
    		cost = EngineConfig::instance().sys_min_price();
    	}
    	if (cost > max_price) { //不能大于出价 
    		cost = max_price;
    	}
    	if((widget_id() == 31) && (cost < 200)) {//自助大图底价2元
    		cost = 200;
    	}
    } else if(IsCpm()){	//cpm
    	if (cost > max_price) { //不能大于出价
    		cost = max_price;
    	}
   		cost *= 0.001; //cpm每次pv扣费
    }
    return cost;
  }
  
  Ice::Long bid_unit_id() {
    return bid_unit_id_;
  }
  int trans_type() const {
    return trans_type_;
  }
  void set_trans_type(int trans_type) {
    this->trans_type_ = trans_type;
  }
  bool operator >(const AdGroup& group) const {
    return group_id_ > group.group_id_;
  }
  bool operator ==(const AdGroup& group) const {
    return group_id_ == group.group_id_;
  }
  bool operator <(const AdGroup& group) const {
    return group_id_ < group.group_id_;
  }
  AdGroup() {
    bid_unit_id_ = 0;
  }

  AdGroup(Ice::Long group_id) : group_id_(group_id), member_id_(0), campaign_id_(0),widget_id_(0) {
    bid_unit_id_ = 0;
	trigger_type_ = 0;
  }

  bool IsCpc() {
    return trans_type_ == 10;
  }
  bool IsCpm() {
    return trans_type_ == 20;
  }

public:
  IceUtil::RWRecMutex mutex_;

  Ice::Long bid_unit_id_;
  Ice::Long group_id_;
  Ice::Long member_id_;
  Ice::Long campaign_id_;
  Ice::Long widget_id_;
  Ice::Long creative_id_;
  int member_category_;
  int member_industry_;
  int display_type_;//轮播,cpm,cpc
  int trans_type_; // 结算方式：10:cpc，20：cpm, 0: 其他
  int max_price_;
  double demo_ctr_;
  int trigger_type_;

};


class CompeteRef : public MyUtil::Singleton<CompeteRef> {
public:
  CompeteRef() :
    is_ready_(false) {
  }
  void Init();
  void Load();
  vector<Ice::Long> Get(Ice::Long campaign);
  bool is_ready();

  class ReloadTimer : public MyUtil::Timer {
  public:
    ReloadTimer() :
      Timer(5 * 60 * 1000) {
    }
    virtual void handle();
  };

private:
  map<Ice::Long, vector<Ice::Long> > ref_;
  IceUtil::RWRecMutex mutex_;
  bool is_ready_;
};

class UserBind {
public:
  Ice::Long adzone_id() const {
    return adzone_id_;
  }
  void set_adzone_id(Ice::Long adzone_id) {
    this->adzone_id_ = adzone_id;
  }
  Ice::Long user_id() const {
    return user_id_;
  }
  void set_user_id(Ice::Long user_id) {
    this->user_id_ = user_id;
  }
  bool operator >(const UserBind& ul) const {
    if (user_id_ != ul.user_id_)
      return user_id_ > ul.user_id_;
    return adzone_id_ > ul.adzone_id_;
  }
  bool operator ==(const UserBind& ul) const {
    return ( (user_id_ == ul.user_id_) && (adzone_id_ == ul.adzone_id_));
  }
  bool operator <(const UserBind& ul) const {
    if(user_id_ != ul.user_id_)
      return user_id_ < ul.user_id_;
    return adzone_id_ < ul.adzone_id_;
  }
private:
  Ice::Long user_id_;
  Ice::Long adzone_id_;
};

class UserBindPool : public MyUtil::Singleton<UserBindPool> {
private:
  typedef map<UserBind, Ice::Long> UserBindMap;
public:
  void BindUser(int user_id, Ice::Long creative_id, Ice::Long zone_id);
  void UnbindUser(int user_id, Ice::Long zone_id);
  void NotifyBindUser(int user_id, Ice::Long creative_id, Ice::Long zone_id);
  void NotifyUnbindUser(int user_id, Ice::Long zone_id);
  void Init();
  typedef UserBindMap::iterator iterator;
  iterator find(UserBind user_bind);
//  iterator begin();
  iterator end();
private:
  UserBindMap user_bind_cache_; //UserBind --> creative_id
  IceUtil::RWRecMutex mutex_;
};

class SelectedAdGroup : public IceUtil::Shared {
public:
  SelectedAdGroup(AdGroupPtr p) : group_(p), cost_(0), pos_(0),
                                  selected_creative_id_(0), selected_widget_id_(0),
                                  ecpm_(0), demo_state_(-1), demo_ctr_(-1.0) {

					selected_creative_id_ = p->creative_id_;
	 				selected_widget_id_ = p->widget_id_;
  }

  bool operator<(const SelectedAdGroup& right) const {
    return group_ < right.group_;
  }
  bool operator==(const SelectedAdGroup& right) const {
    return group_ < right.group_;
  }
  AdGroupPtr group_;
  double cost_;	//cost_改成double类型，否则cpm扣费可能有问题
  int pos_;
  Ice::Long selected_creative_id_; //样式实验选中的创意
  Ice::Long selected_widget_id_;   //样式实验选中的widget_id
  double ecpm_;
  string append_info_;
  string extra_;
  int demo_state_;
  double demo_ctr_;
};

class FilteredGroupList: public MyUtil::Singleton<FilteredGroupList> {
public:
  void Init() {
    MCE_INFO("FilteredGroupList init start");
    string file_name = "../etc/filtered_group_list";
    try {
      ifstream config(file_name.c_str());
      string line;
      if (config) {
        while (config >> line) {
          MCE_INFO("FilteredGroupList init group id : " << line);
          set_.insert(boost::lexical_cast<Ice::Long>(line));
        }
      }
    } catch (Ice::Exception& e) {
      MCE_WARN("FilteredGroupList init error : " << e);
    } catch (...) {
      MCE_WARN("FilteredGroupList init error : unkown error \"...\"");
    }
    MCE_INFO("FilteredGroupList init end");
  }
  bool IsNotExist(Ice::Long gid) {
    return set_.find(gid) == set_.end();
  }
private:
  set<Ice::Long> set_;
};
} // end namespace ad
} // end namespace xce

#endif /* ADSTRUCT_H_ */

