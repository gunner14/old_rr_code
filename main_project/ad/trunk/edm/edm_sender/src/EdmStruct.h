/*
 * EdmStruct.h
 *
 *  Created on: 2012-2-20
 *      Author: sunzz
 */

#ifndef EDMSTRUCT_H_
#define EDMSTRUCT_H_
#include <time.h>
#include <vector>
#include <map>
#include <ext/hash_map>
#include <set>
#include <IceUtil/RWRecMutex.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>

#include "TaskManager.h"
#include "QueryRunner.h"
#include "Singleton.h"
#include "FrequencyCheck.h"


#include "FeedMemcProxy/client/user_profile_client.h"


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

/**************************************************************************************
 * 客户类型：品牌、自助、产品广告
 * ******************************8*****************************************************/
enum PubType { Brand_ad = 1, Self_ad, Product_ad };

/**************************************************************************************
 * 广告商信息
 *************************************************************************************/
class AdMember : public IceUtil::Shared {
public:
  bool Available(int display_type = 1);

  Ice::Long member_id() const {
    return member_id_;
  }
  void set_member_id(Ice::Long member_id) {
    this->member_id_ = member_id;
  }
  int delete_flag() const {
    return delete_flag_;
  }
  void set_delete_flag(int delete_flag) {
    this->delete_flag_ = delete_flag;
  }
  int ma_online_status() const {
    return ma_online_status_;
  }
  void set_ma_online_status(int ma_online_status) {
    this->ma_online_status_ = ma_online_status;
  }
  int no_exceed_max() const {
    return no_exceed_max_;
  }
  void set_no_exceed_max(int no_exceed_max) {
    this->no_exceed_max_ = no_exceed_max;
  }
  int am_online_status() const {
    return am_online_status_;
  }
  void set_am_online_status(int am_online_status) {
    this->am_online_status_ = am_online_status;
  }
  int have_money() const {
    return have_money_;
  }
  void set_have_money(int have_money) {
    this->have_money_ = have_money;
  }
  int validity() const {
    return validity_;
  }
  void set_validity(int validity) {
    this->validity_ = validity;
  }
  int is_formal() const {
    return is_formal_;
  }
  void set_is_formal(int is_formal) {
    this->is_formal_ = is_formal;
  }
  int category() const {
    return category_;
  }
  void set_category(int category) {
    this->category_ = category;
  }
  int industry() const {
    return industry_;
  }
  void set_industry(int industry) {
    this->industry_ = industry;
  }
private:
  int is_formal_;
  int validity_;
  int have_money_;
  int no_exceed_max_;
  int am_online_status_;
  int ma_online_status_;
  int delete_flag_;
  int category_;
  int industry_;
  Ice::Long member_id_;
};
typedef IceUtil::Handle<AdMember> AdMemberPtr;

/****************************************************************************
 * 广告计划信息
 * *************************************************************************/
//如果Member不可用，请业务系统update campign and ad_group表的status字段为不可用
class AdCampaign : public IceUtil::Shared {
public:
  AdCampaign() {
    end_time_ = 0;
    audit_status_ = 0;
    delete_flag_ = 0;
    sys_status_ = 0;
    member_status_ = 0;
    am_status_ = 0;
  }
  bool Available();

  bool delete_flag() const {
    return delete_flag_;
  }
  void set_delete_flag(bool delete_flag) {
    this->delete_flag_ = delete_flag;
  }
  int audit_status() const {
    return audit_status_;
  }
  void set_audit_status(int audit_status) {
    this->audit_status_ = audit_status;
  }
  int am_status() const {
    return am_status_;
  }
  void set_am_status(int am_status) {
    this->am_status_ = am_status;
  }
  int member_status() const {
    return member_status_;
  }
  void set_member_status(int member_status) {
    this->member_status_ = member_status;
  }
  int sys_status() const {
    return sys_status_;
  }
  void set_sys_status(int sys_status) {
    this->sys_status_ = sys_status;
  }
  time_t end_time() const {
    return end_time_;
  }
  void set_end_time(time_t end_time) {
    this->end_time_ = end_time;
  }
  time_t begin_time() const {
    return begin_time_;
  }
  void set_begin_time(time_t begin_time) {
    this->begin_time_ = begin_time;
  }
  Ice::Long member_id() const {
    return member_id_;
  }
  void set_member_id(Ice::Long member_id) {
    this->member_id_ = member_id;
  }
  Ice::Long campaign_id() const {
    return campaign_id_;
  }
  void set_campaign_id(Ice::Long campaign_id) {
    this->campaign_id_ = campaign_id;
  }

private:
  Ice::Long campaign_id_;
  Ice::Long member_id_;
  time_t begin_time_;
  time_t end_time_;
  int sys_status_;
  int member_status_;
  int am_status_;
  int audit_status_;
  bool delete_flag_;
};
typedef IceUtil::Handle<AdCampaign> AdCampaignPtr;

/*******************************************************************
 * 创意表
***********************************************************************/
class AdCreative : public IceUtil::Shared {
private:
  Ice::Long id_;
  Ice::Long widget_id_;
  string widget_;
public:
  AdCreative() {
    id_ = 0;
    widget_id_ = 0;
    widget_ = "";
  }

  bool operator >(const AdCreative& creative) const {
    return id_ > creative.id_;
  }

  bool operator ==(const AdCreative& creative) const {
    return id_ == creative.id_;
  }

  bool operator <(const AdCreative& creative) const {
    return id_ < creative.id_;
  }

  AdCreative(Ice::Long id, Ice::Long widget_id, string widget) :
    id_(id), widget_id_(widget_id), widget_(widget){
  }

  string widget(){
    return widget_;
  }

  Ice::Long creative_id(){
    return id_;
  }


};
typedef IceUtil::Handle<AdCreative> AdCreativePtr;

/*****************************************************************************
 * 广告组信息
 * ****************************************************************************/
class AdGroup : public IceUtil::Shared {
public:
  Ice::Long GetCreative(int uid, int rotate_index);
  bool Available();
  bool HasCreatives();
 // void UpdateCampainStatus();
  void set_group_id(Ice::Long group_id) {
    group_id_ = group_id;
  }

  void set_member_id(Ice::Long member_id) {
    member_id_ = member_id;
  }
  void set_campaign_id(Ice::Long campaign_id) {
    campaign_id_ = campaign_id;
  }
  void set_member_status(int member_status) {
    member_status_ = member_status;
  }

  void set_am_status(int am_status) {
    am_status_ = am_status;
  }
  void set_audit_status(int audit_status) {
    audit_status_ = audit_status;
  }
  void set_delete_flag(int delete_flag) {
    delete_flag_ = delete_flag;
  }
  void set_display_type(int display_type) {
    display_type_ = display_type;
  }
  void set_max_price(int max_price) {
    max_price_ = max_price;
  }

  void set_campaign_status(bool campaign_status) {
    campaign_status_ = campaign_status;
  }
  void set_member_table_status(bool member_table_status) {
    member_table_status_ = member_table_status;
  }
  void add_creative_seq(AdCreativePtr creative) {
    creative_seq_.insert(creative);
  }
  void add_creative_id(Ice::Long creative_id) {
    creative_ids_.insert(creative_id);
  }
  void add_widget_id(Ice::Long creative_id,Ice::Long widget_id) {
    MCE_DEBUG("add_widget_id: creative_id = " << creative_id << ", widget_id = " << widget_id);
    creative2widget_[creative_id]=widget_id;
  }
  void set_bid_unit_id(Ice::Long bid_unit_id) {
    bid_unit_id_ = bid_unit_id;
  }
  void set_point_valid(bool valid) {
    point_valid_ = valid;
  }
  void set_daily_sys_status(int daily_sys_status) {
    daily_sys_status_ = daily_sys_status;
  }
  void set_total_sys_status(int total_sys_status) {
    total_sys_status_ = total_sys_status;
  }
  void set_edm_send_level(int edm_send_level) {
    edm_send_level_= edm_send_level;
  }
  int edm_send_level(){
    return edm_send_level_;
  }
  void set_edm_cast_type(int edm_cast_type) {
    edm_cast_type_ = edm_cast_type;
  }
  void set_member_category(int member_category) {
    member_category_ = member_category;
  }
  int member_category() {
    return member_category_; 
  }
  void set_member_industry(int member_industry) {
    member_industry_ = member_industry;
  }
  int member_industry() {
    return member_industry_; 
  }
  int edm_cast_type(){
    return edm_cast_type_;
  }

  int edm_member_type();

  Ice::Long group_id() {
    return group_id_;
  }
  Ice::Long member_id() {
    return member_id_;
  }

  Ice::Long campaign_id() {
    return campaign_id_;
  }
  int member_status() {
    return member_status_;
  }
  double cost() {
    return max_price_;
  }
  int am_status() {
    return am_status_;
  }
  int audit_status() {
    return audit_status_;
  }
  int delete_flag() {
    return delete_flag_;
  }
  int display_type() {
    return display_type_;
  }
  int max_price() {
    return max_price_;
  }

  string widget() {
    if(!creative_seq_.empty()){
      if(*creative_seq_.begin()){
        return (*creative_seq_.begin())->widget();
      }
    }
    return "";
  }
  Ice::Long creative_id() {
    if(!creative_ids_.empty()) {
      return *(creative_ids_.begin());
    } else {
      return 0;
    }
  }
  Ice::Long widget_id() {
    if(!creative2widget_.empty()){
      return creative2widget_.begin()->second;
    }
    return 0;
  }
  Ice::Long bid_unit_id() {
    return bid_unit_id_;
  }
  int daily_sys_status() {
    return daily_sys_status_;
  }
  int total_sys_status() {
    return total_sys_status_;
  }
  bool campaign_status() {
    return campaign_status_;
  }
  bool member_table_status() {
    return member_table_status_;
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
    am_status_ = 0;
    audit_status_ = 0;
    delete_flag_ = 0;
    campaign_status_ = 0;
    member_table_status_ = 0;
    daily_sys_status_ = 0;
    total_sys_status_ = 0;
    bid_unit_id_ = 0;

  }

  AdGroup(Ice::Long group_id) : group_id_(group_id), member_id_(0), campaign_id_(0),widget_id_(0) {
    am_status_ = 0;
    audit_status_ = 0;
    delete_flag_ = 0;
    campaign_status_ = 0;
    member_table_status_ = 0;
    daily_sys_status_ = 0;
    total_sys_status_ = 0;
    bid_unit_id_ = 0;

  }

  bool IsCpc() {
    return trans_type_ == 10;
  }
  bool IsCpm() {
    return trans_type_ == 20;
  }
private:
  IceUtil::RWRecMutex mutex_;

  Ice::Long bid_unit_id_;
  Ice::Long group_id_;
  Ice::Long member_id_;
  Ice::Long campaign_id_;
  Ice::Long widget_id_;

  int member_status_; // 客户自己设置的该adgroup的状态
  int member_category_;
  int member_industry_;
  int am_status_;
  int audit_status_;
  int delete_flag_;
  int display_type_;//8是EDM广告
  int trans_type_; // 结算方式：10:cpc，20：cpm, 0: 其他
  int max_price_;
  int daily_sys_status_;
  int total_sys_status_;
  int edm_send_level_; // 1：高级；2：中级；3：低级
  int edm_cast_type_; //1：人群定向；2：ID定向
  bool campaign_status_; // 所属campaign的状态
  bool member_table_status_; // 所属member的状态
  set<Ice::Long> creative_ids_;
  map<Ice::Long,Ice::Long> creative2widget_;
  set<AdCreativePtr> creative_seq_;

  bool point_valid_;//是否在排期内

};
typedef IceUtil::Handle<AdGroup> AdGroupPtr;
typedef vector<AdGroupPtr> AdGroupSeq;

/*********************************************************************
 * 最终选择广告信息
 * *******************************************************************/
class SelectedAdGroup : public IceUtil::Shared {
public:
  SelectedAdGroup(AdGroupPtr p) : group_(p), cost_(0),
                                  ecpm_(0){

  }

  bool operator<(const SelectedAdGroup& right) const {
    return group_ < right.group_;
  }
  bool operator==(const SelectedAdGroup& right) const {
    return group_ < right.group_;
  }
  bool operator >(const SelectedAdGroup& right) const {
    return group_ > right.group_;
  }
  AdGroupPtr group() {
    return group_;
  }
  string widget() {
    return group_->widget();
  }
  Ice::Long group_id() {
    return group_->group_id();
  }
  double cost(){
    return cost_;
  }
  void set_cost(double cost){
    cost_ = cost;
  }
  void set_send_type (SendType send_type){
    send_type_ = send_type;
  }
  SendType send_type() {return send_type_;}
private:
  AdGroupPtr group_;
  double cost_; //cost_改成double类型，否则cpm扣费可能有问题
  double ecpm_;
  string append_info_;
  string extra_;
  SendType send_type_;

};

typedef IceUtil::Handle<SelectedAdGroup> SelectedAdGroupPtr;
typedef vector<SelectedAdGroupPtr> SelectedAdGroupSeq;

} // end namespace ad
} // end namespace xce



#endif /* EDMSTRUCT_H_ */
