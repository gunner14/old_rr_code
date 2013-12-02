#include <algorithm>
#include "../common/constant.h"
#include "../include/ad_send_speed_filter.h"

using namespace std;
using namespace xce::ad;
using namespace com::xiaonei::xce;

void AdSendSpeedLower::Init(const string& db){
  MCE_INFO("AdSendSpeedLower::Init --> db source: " << db);
  DBQuotaLoader::instance().SetDB(db);
  LowerConfig::instance().SetDB(db); 
}

bool AdSendSpeedLower::SelectByRand(double ratio){
  int mod_value = rand() % UNIT;
  return 1.0 * mod_value / UNIT > ratio;
}

double AdSendSpeedLower::GetValue(map<Ice::Long, DummyRecordPtr>& ratio, const Ice::Long id, const string& default_value_name){
  {
    IceUtil::RWRecMutex::RLock lock(mutex_);
    map<Ice::Long, DummyRecordPtr>::iterator m_iter = ratio.find(id);
    if (m_iter != ratio.end()){
      return m_iter->second->value_;
    }
  }

  DummyRecordPtr record_ptr = DummyRecordPtr(new DummyRecord());
  if (init_){
    record_ptr->value_ = LowerConfig::instance().GetValue(default_value_name);
  } else {
    record_ptr->value_ = 1.0;
  }

  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    ratio[id] = record_ptr;
  }
  return record_ptr->value_;
}

void AdSendSpeedLower::InitCampaignIDList(vector<Ice::Long>& campaign_id_vec){
  vector<Ice::Long>::const_iterator v_iter = campaign_id_vec.begin();
  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    for (; v_iter != campaign_id_vec.end(); ++v_iter){
      DummyRecordPtr record_ptr = DummyRecordPtr(new DummyRecord());
      if (!init_){
	record_ptr->value_ = 1.0;
      } else {
        record_ptr->value_ = LowerConfig::instance().GetValue(CAMPAIGN_INIT_RATIO);
      }
      campaign_ratio_[*v_iter] = record_ptr;
    }
  }
}

void AdSendSpeedLower::InitMemberIDList(vector<Ice::Long>& member_id_vec){
  vector<Ice::Long>::const_iterator v_iter = member_id_vec.begin(); 
  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    for (; v_iter != member_id_vec.end(); ++v_iter){
      DummyRecordPtr record_ptr = DummyRecordPtr(new DummyRecord());
      if (!init_){
	record_ptr->value_ = 1.0;
      } else {
        record_ptr->value_ = LowerConfig::instance().GetValue(MEMBER_INIT_RATIO);
      }
      member_ratio_[*v_iter] = record_ptr;
    }
  }
}

bool AdSendSpeedLower::IsOffline(AdGroupPtr adgroup_ptr){
  double campaign_ratio = 0.0, member_ratio = 0.0;
  campaign_ratio = GetValue(campaign_ratio_, adgroup_ptr->campaign_id_, CAMPAIGN_DEFAULT_RATIO);
  member_ratio = GetValue(member_ratio_, adgroup_ptr->member_id_, MEMBER_DEFAULT_RATIO);

  double ratio = min(campaign_ratio, member_ratio);
  
  if (init_){
    ratio = max(ratio, LowerConfig::instance().GetValue(LEAST_RATIO));
  } else {
    ratio = 1.0;
  }

  //MCE_DEBUG("AdSendSpeedLower::IsOffline --> campaignId: " << adgroup_ptr->campaign_id_ << " memberID: " << adgroup_ptr->member_id_<< " ratio: " << ratio << " campaign ratio: " << campaign_ratio << " member ratio: " << member_ratio);
  return SelectByRand(ratio);
}

void AdSendSpeedLower::Copy(const map<Ice::Long, DummyRecordPtr>& record, map<Ice::Long, DummyRecordPtr>& temp_record){
  IceUtil::RWRecMutex::RLock lock(mutex_);
  ostringstream os;
  for (map<Ice::Long, DummyRecordPtr>::const_iterator m_iter = record.begin(); 
       m_iter != record.end(); ++m_iter){
    DummyRecordPtr record_ptr = DummyRecordPtr(new DummyRecord());
    record_ptr->value_ = m_iter->second->value_;
    record_ptr->update_time_ = m_iter->second->update_time_;
    temp_record[m_iter->first] = record_ptr;
    os << m_iter->first << "&";
  }
  //MCE_DEBUG("AdSendeSpeedLower::Copy--> Size: " << record.size() << "After Size: " << temp_record.size() << "IDS: " << os.str() );
}

void AdSendSpeedLower::UpdateData(){
  //time_t now = time(NULL);
  //MCE_DEBUG("AdSendSpeedLower::UpdateData: Begin Time: " << now);
  if (Clear() || (!init_)){
    //MCE_DEBUG("AdSendSpeedLower::UpdateData() init value: " << init_ );
    MCE_INFO("AdSendSpeedLower::UpdataData --> Start getAdCampaignIDList");
    vector<Ice::Long> id_list = AdPool::instance().getAdCampaignIDList();
    MCE_INFO("AdSendSpeedLower::UpdataData --> End getAdCampaignIDList");
    InitCampaignIDList(id_list);
    MCE_INFO("AdSendSpeedLower::UpdataData --> End InitCampaignIDList");

    id_list.clear();
    MCE_INFO("AdSendSpeedLower::UpdataData --> Start getAdMemberIDList");
    id_list = AdPool::instance().getAdMemberIDList();
    MCE_INFO("AdSendSpeedLower::UpdataData --> End getAdMemberIDList");
    InitMemberIDList(id_list); 
    MCE_INFO("AdSendSpeedLower::UpdataData --> End InitMemberIDList");
  }

  //now = time(NULL);
  //MCE_WARN("AdSendSpeedLower::UpdateData: LowerConfig DB Begin Time: " << now);
  LowerConfig::instance().UpdateData();
  init_ = true;
  //now = time(NULL);
  //MCE_WARN("AdSendSpeedLower::UpdateData: LowerConfig DB End Time: " << now);

  //now = time(NULL);
  //MCE_DEBUG("AdSendSpeedLower::UpdateData: ID Init Time: " << now);
  Statement sql1;
  sql1 << " SELECT * FROM ad_campaign_balance "
      << " WHERE update_time >= " << campaign_last_update_time_;
  //MCE_DEBUG("AdSendSpeedLower::UpdateData --> campaign_last_update_time " << campaign_last_update_time_);

  Statement sql2;
  sql2 << " SELECT * FROM ad_member_balance "
      << " WHERE update_time >= " << member_last_update_time_;
  //MCE_DEBUG("AdSendSpeedLower::UpdateData --> member last update time " << member_last_update_time_);

  map<Ice::Long, DummyRecordPtr> temp_ratio;
  Copy(campaign_ratio_, temp_ratio);
  if (!DBQuotaLoader::instance().Load(temp_ratio, CAMPAIGN_VEC, sql1, new CampaignComputeRatio(), campaign_last_update_time_, AD_CAMPAIGN_BALANCE_TABLE_NAME)){
    MCE_WARN("AdSendSpeedLower::UpdateData() --> Campaign DB Update Error!");
  }
   
  //now = time(NULL);
  //MCE_DEBUG("AdSendSpeedLower::UpdateData: Campaign DB End Time: " << now);
  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    campaign_ratio_.swap(temp_ratio);
  }
  //now = time(NULL);
  //MCE_DEBUG("AdSendSpeedLower::UpdateData: Campaign Swap Time: " << now);

  /*ostringstream os3;
  map<Ice::Long, DummyRecordPtr>::iterator m_iter = campaign_ratio_.begin();
  for (; m_iter != campaign_ratio_.end(); ++m_iter){
    os3 << m_iter->first << " " << m_iter->second->value_ << " " << m_iter->second->update_time_ << " : "; 
  }
  MCE_DEBUG("AdSendSpeedLower::UpdateData() --> " << os3.str());*/

  temp_ratio.clear();
  Copy(member_ratio_, temp_ratio);
  if (!DBQuotaLoader::instance().Load(temp_ratio, MEMBER_VEC, sql2, new MemberComputeRatio(), member_last_update_time_, AD_MEMBER_BALANCE_TABLE_NAME)){
    MCE_WARN("AdSendSpeedLower::UpdateData() --> Member DB Update Error!");  
  }
  
  //now = time(NULL);
  //MCE_DEBUG("AdSendSpeedLower::UpdateData: Member DB End Time: " << now);
  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    member_ratio_.swap(temp_ratio);
  }
  ///now = time(NULL);
  //MCE_DEBUG("AdSendSpeedLower::UpdateData: Member Swap Time: " << now);

  /*ostringstream os4;
  m_iter = member_ratio_.begin();
  for (; m_iter != member_ratio_.end(); ++m_iter){
    os4 << m_iter->first << " " << m_iter->second->value_ << " " << m_iter->second->update_time_ << " : ";
  }
  MCE_DEBUG("AdSendSpeedLower::UpdateData() --> Member: " << os4.str());*/
}

bool AdSendSpeedLower::Clear(){
  time_t now = time(NULL);
  if (now < (last_clear_time_ + ONE_DAY)){
    return false;
  }

  struct tm time;
  localtime_r(&now, &time);
  time.tm_hour = THREE_CLOCK;
  time.tm_min = ZERO;
  time.tm_sec = ZERO;
  last_clear_time_ = mktime(&time);
  MCE_INFO("AdSendSpeedLower::Clear() --> last_clear_time: " << last_clear_time_);

  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    member_ratio_.clear();
    campaign_ratio_.clear();
  }
  MCE_INFO("AdSendSpeedLower::Clear --> clear over!");
  return true;
}

