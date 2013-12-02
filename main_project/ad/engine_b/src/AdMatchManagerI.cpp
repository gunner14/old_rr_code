#include <curl/curl.h>
#include "AdMatchManagerI.h"
#include "AdLogReplicaAdapter.h"
#include "AdStruct.h"
#include "ServiceI.h"
#include "Date.h"
#include <QueryRunner.h>
#include <DbDescriptor.h>
#include "client/linux/handler/exception_handler.h"
#include "AdMonitorI.h"
#include "../../util/src/AdCommonUtil.h"
#include "AdConfig.h"
#include "MatchHandlers.h"
#include "UserRankHandler.h"
#include "OnlyFirstHandler.h"
#include "AdEngineDebugI.h"
#include "../../util/src/UrlMaping.h"
#include "NielsonFilter.h"
#include "engineTimer.h"
#include "UserDemographicCache.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "ad/Degrade/src/Degrade.h"

using namespace com::xiaonei::xce;
using namespace xce::ad;
using namespace boost;
using namespace ::xce::serverstate;

bool MyCallback(const char *dump_path, const char *minidump_id, void *context, bool succeeded) {
  printf("got crash %s %s\n", dump_path, minidump_id);
  return true;
}


void MyUtil::initialize() {
  srand(time(NULL));
  static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
  ServiceI& service = ServiceI::instance();
  int index = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("ServerIndex", -1);
  string source_db_ads = service.getCommunicator()->getProperties()->getPropertyWithDefault("DbInstance_Ads", "jebe_ads_load");
  string source_db_main = service.getCommunicator()->getProperties()->getPropertyWithDefault("DbInstance_Main", "jebe_main_load");
  EngineConfig::instance().SetDBSource_Ads(source_db_ads);
  EngineConfig::instance().SetDBSource_Main(source_db_main);
  xce::ad::EngineConfig::instance().set_server_object(service.getCommunicator()->getProperties()->getPropertyWithDefault("ServerObject", "brand"));
  xce::ad::EngineConfig::instance().set_server_name(index);
  service.getAdapter()->add(&AdMatchManagerI::instance(), service.createIdentity("M",""));
  service.getAdapter()->add(&AdMonitorI::instance(), service.createIdentity("B",""));
  service.getAdapter()->add(&AdEngineDebugI::instance(), service.createIdentity("D",""));
  service.getAdapter()->add(&FirstFlag::instance(), service.createIdentity("F",""));
  xce::ad::NielsonFilter::instance();
  TaskManager::instance().execute(new InitTask());
  TaskManager::instance().schedule(new xce::ad::AdUrlMapingLoadTimer(xce::ad::EngineConfig::instance().GetDBSource_Main(), 0));
  // TaskManager::instance().scheduleRepeated(new ReloadTimer());
  // TaskManager::instance().scheduleRepeated(&AdQualityCache::instance());
  int idx = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("ServerIndex", -1);
	EngineConfig::instance().setServerIndex(idx);

	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("AdEngineB.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("AdEngineB.Interval",5);
	xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerAdEngineB",&AdMatchManagerI::instance(),mod,interval, new EdmChannel());
  MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);
  //TaskManager::instance().schedule(new xce::ad::AdUrlMapingLoadTimer(xce::ad::EngineConfig::instance().DB(), 0));
	EngineTimerPtr userDemoCacheTimer = new UserDemographicCacheTimer(&(UserDemographicCache::instance()));  
	EngineTimerPtr freshCreativeLoadTimer = new FreshCreativeLoadTimer(&(UserDemographicCache::instance()));
	EngineTimerManager::instance().add(userDemoCacheTimer);
	EngineTimerManager::instance().add(freshCreativeLoadTimer);
	EngineTimerManager::instance().start().detach();
	DegradeManager::instance().SetDBSource("ad_st");
	DegradeManager::instance().Register("EngineBAISort");
	DegradeManager::instance().Register("EngineBDCO");
	if(DegradeManager::instance().Init() !=0 ){
		MCE_WARN("DegradeManager::Init failed");
	}
}

/***********************************************************************************************************************/
string LogHelper::MakeText(const LogParas& para){
  ostringstream ans;
  string spliter = "^C";
  ans << "0" << spliter << "0" << spliter << para.creative_id_ << spliter << para.bid_unit_id_ << spliter
      << para.cost_ << spliter << para.pos_ << spliter << para.max_price_
      << spliter << para.pv_time_ << spliter << MakeUbc(para.user_profile_)
      << spliter << para.zone_id_ << spliter << "0" << spliter
      << para.next_load_time_ << spliter << para.refresh_idx_ << spliter << para.append_info_;
  return ans.str();
}

LogParas LogHelper::ParseText(const string& text){
  LogParas ans;
  if(!text.empty()){
    vector<string> para_strs;
    boost::algorithm::split_regex( para_strs, text,regex( "\\^C" ) ) ;
    if(para_strs.size() >= 14){
      try{
        ans.creative_id_ = boost::lexical_cast<Ice::Long>(para_strs[2]);
        ans.bid_unit_id_ = boost::lexical_cast<Ice::Long>(para_strs[3]);
        ans.cost_ = boost::lexical_cast<int>(para_strs[4]);
        ans.pos_ = boost::lexical_cast<int>(para_strs[5]);
        ans.max_price_ = boost::lexical_cast<int>(para_strs[6]);
        ans.pv_time_ = boost::lexical_cast<time_t>(para_strs[7]);
        ans.ubc_ = para_strs[8];
        ans.zone_id_ = boost::lexical_cast<Ice::Long>(para_strs[9]);
        ans.next_load_time_ = para_strs[11];
        ans.refresh_idx_ = para_strs[12];
        ans.append_info_ = para_strs[13];
      }
      catch(...){
        MCE_WARN("LogHelper::ParseText --> cast error ");
      }
    }
  }

  if (!ans.ubc_.empty()) {
//    UserProfile profile;
    vector<string> profile_items;
    boost::algorithm::split_regex(profile_items, ans.ubc_,regex( "\\|" ) ) ;
    if (profile_items.size() >= 11) {
      try {
        vector<string> id_items;
        boost::algorithm::split_regex(id_items, profile_items[0], regex( "\\_" ) ) ;
        if (id_items.size() >= 2) {
          ans.user_profile_.set_id(boost::lexical_cast<int>(id_items[1]));
        }

        ans.user_profile_.set_gender(boost::lexical_cast<int>(profile_items[1]));
        ans.user_profile_.set_age(boost::lexical_cast<int>(profile_items[3]));
        ans.user_profile_.set_stage(boost::lexical_cast<int>(profile_items[4]));

        vector<string> school_area_items;
        boost::algorithm::split_regex(school_area_items, profile_items[6], regex( "\\_" ) ) ;
        if (school_area_items.size() >= 2) {
          ans.user_profile_.set_school(boost::lexical_cast<Ice::Long>(school_area_items[0]));
          ans.user_profile_.set_current_area(school_area_items[1]);
        }

        ans.user_profile_.set_grade(boost::lexical_cast<int>(profile_items[7]));
        ans.user_profile_.set_ip_area(profile_items[10]);
      } catch (...) {
        MCE_WARN("LogHelper::ParseText --> cast ubc error ");
      }
    }
  }
  return ans;
}

void LogHelper::Process(string& str){
  CURL *handle = curl_easy_init();
  char *encodedURL = curl_easy_escape(handle, str.c_str(), 0);
  if (encodedURL){
    //MCE_DEBUG("Encoded URL: " << encodedURL);
    str = encodedURL;
    curl_free(encodedURL);
  } else {
    MCE_INFO("URL encoding failed");
  }
  curl_easy_cleanup(handle);

  str = "\"" + str + "\"";
}

string LogHelper::MakeUbc(const UserProfile& upf){
  ostringstream ubc_ios;

  int birth_year = Date(time(NULL)).year() - upf.age();
  int user_stage = upf.stage();
  string valid_area = "";
  if(4 == user_stage){
    valid_area = upf.ip_area();
  }
  else{
    valid_area = upf.current_area();
  }
  if (valid_area.empty()) {
    valid_area = "-";
  }

  string current_area = (upf.current_area().empty()) ? "-" : upf.current_area();
  string ip_area = (upf.ip_area().empty()) ? "-" : upf.ip_area();

  ubc_ios << "1000000_" << upf.id() << "|" << upf.gender() << "|" << birth_year
      << "-01-01" << "|" << upf.age() << "|" << upf.stage() << "|" << valid_area << "|"
      << upf.school() << "_" << current_area << "|" << upf.grade() << "|0|0|" << ip_area;
  return ubc_ios.str();
}

string LogHelper::MakeHttp(const LogParas& para){
  ostringstream http_ios;
  http_ios << "/show?userid=" << para.user_profile_.id() << "&tt=0&type=.js&adzoneid="
          << para.zone_id_ << "&count=" << para.ad_count_ << "&next_load_time="
          << para.next_load_time_ << "&refresh_idx=" << para.refresh_idx_<< "&rowoffset=0&ip="
          << MakeIp(para.user_profile_.ip()) << "&jbc=cookie|a|b&ubc=" << MakeUbc(para.user_profile_);
  return http_ios.str();
}

string LogHelper::MakeIp(int ip){
  int a, b, c, d;
  unsigned ip_u = (unsigned) ip;
  a = ip_u / 16777216;
  b = (ip_u - a * 16777216) / 65536;
  c = (ip_u - a * 16777216 - b * 65536) / 256;
  d = (ip_u - a * 16777216 - b * 65536 - c * 256);
  ostringstream ios2;
  ios2 << a << "." << b << "." << c << "." << d;
  return ios2.str();
}

string LogHelper::FormatedClickTime(time_t t)
{
  struct tm lt;
  localtime_r(&t, &lt);

  const int MAX_LEN = 24;
  char res[MAX_LEN];
  res[MAX_LEN - 1] = 0;
  snprintf(res, MAX_LEN - 1, "%d-%02d-%02d %02d:%02d:%02d", lt.tm_year + 1900,
      lt.tm_mon + 1, lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec);
  return res;
}

string LogHelper::GetAppId(string refer) {
  string result = "";

  int begin = refer.find("com");
  int end = 0;
  if (begin == -1) {
    result = "-";
  } else if ((refer.size() - 1) >= (unsigned) (begin + 4)) {
    string sub_refer = refer.substr(begin + 4);

    int index1 = sub_refer.find_first_of("?");
    int index2 = sub_refer.find_first_of("/");
    if (index1 == -1 && index2 != -1) {
      end = index2;
    } else if (index2 == -1 && index1 != -1) {
      end = index1;
    } else {
      end = min(index1, index2);
    }
    result = sub_refer.substr(0, end);
  }
  return result.empty() ? "-" : result;
}

/***********************************************************************************************************************/
AdMatchManagerI::AdMatchManagerI() {
  if (EngineConfig::instance().IsForBrand()) {
    ad_match_handler_list_.push_back(new OnlyFirstHandler());
    ad_match_handler_list_.push_back(new RotationHandler());
  }
  ad_match_handler_list_.push_back(new UserRankHandler());
  ad_match_handler_list_.push_back(new DefaultHandler());
  ad_match_handler_list_.push_back(new LogHandler());

  init_ok_ = false;
  
  MCE_INFO("AdMatchNManagerI::Create --> The Engine started.");
}

void AdMatchManagerI::FilterBindUser(AdZoneDict& zone_dict, AdResultMap& ans, int userid){
  IceUtil::RWRecMutex::WLock wlock(mutex_);
  for(AdZoneDict::iterator it = zone_dict.begin(); it != zone_dict.end();) {
    UserBindPtr user_bind = UserBindPool::instance().GetUserBind(userid, it->second->id());
    if(user_bind){
      //只有大客户截屏功能，而大客户只有一个展示位置，
      //小客户截屏功能
      AdInfoSeq ad_info_seq;
      AdInfo ad_info;
      ad_info.creativeid = user_bind->creative_id();
      ad_info.groupid = 0;
      ad_info.updateTime = 0;
      ad_info.text = "";
      ad_info_seq.push_back(ad_info);
      ans[user_bind->adzone_id()] = ad_info_seq;
      zone_dict.erase(it++);
    } else {
      ++it;
    }
  }
}

void CookieEmptyCheck(UserProfile& upf) {
  MCE_DEBUG("id=" << upf.id() << ",iparea=" << upf.ip_area() << ",stage=" << upf.stage() << ",current_area=" << upf.current_area());
  if (!upf.id()) {
    upf.set_id(0);
  }
  if (upf.ip_area().empty() || "0000000000000000" == upf.ip_area()) {
    upf.set_ip_area("0086110000000000"); // 北京
  }
  if (!upf.stage()) {
    upf.set_stage(4);
  }
  if (!upf.ip()) {
    upf.set_ip(975044608); // 北京的一个随机IP
  }
  if (upf.current_area().empty()) {
    upf.set_current_area("0086110000000000"); // 北京
  }
  //测试代码，设置ip 10.2.17.35 （彭贺庭）和10.2.19.216（邢永涛）为国外
  /*if (upf.ip() == (10*256*256*256+2*256*256+17*256+35) || upf.ip() == (10*256*256*256+2*256*256+19*256+216)) {
    MCE_DEBUG("10.2.17.35 set to 0087110000000000, uid = " << upf.id());
    upf.set_current_area("0087110000000000");
    upf.set_ip_area("0087110000000000"); // 北京
  }*/
}

AdResultMap AdMatchManagerI::GetAds(const string& userProfile, const string& append_para, int rotate_index, const Ice::Current& current){
  AdResultMap ans;
  UserProfile profile;
  UserProfileClient::instance().Deserialize(userProfile, &profile);
  if(!init_ok_){
    MCE_INFO("AdMatchNManagerI::GetAds() --> initing.......");
    return ans;
  }
  bool hide = false;
  Ice::Context::const_iterator i = current.ctx.find("hide");
  if (i != current.ctx.end() && i->second == "true") {
    hide = true;
  }

  CookieEmptyCheck(profile);

  MatchHandlerParamter para(profile, append_para);

  MCE_DEBUG("AdMatchNManagerI::GetAds() --> referer(para.referer_) = " << para.referer_ << ", append_para = " << append_para);
  AdZoneDict zone_dict = AdZonePool::instance().GetAdZoneOrdered(para.referer_, profile, hide);
  //AdZoneDict zone_dict = AdZonePool::instance().GetAdZoneOrdered(para.referer_, profile.stage());
  if(zone_dict.empty()){
    // MCE_INFO("GetAdZoneOrdered() empty. referer=" << para.referer_);
    return ans;
  }
  //for(AdZoneDict::iterator zit = zone_dict.begin(); zit != zone_dict.end(); ++zit) {
  //    MCE_DEBUG("user = " << profile.id() << ", zone = " << zit->second->id());
  //}
  MCE_DEBUG("[MAIN_PROCESS] AdMatchNManagerI::GetAds --> AdZoneDict->size = " << zone_dict.size() << ", referer=" << para.referer_);
  FilterBindUser(zone_dict, ans, profile.id());
  MCE_DEBUG("[MAIN_PROCESS] AdMatchNManagerI::GetAds --> after FilterBindUser AdZoneDict->size = " << zone_dict.size() << ", referer=" << para.referer_);

  //bool hide_flag = false;
  //按优先级遍历广告位，priority越小，优先级越高
  for(AdZoneDict::iterator it = zone_dict.begin(); it != zone_dict.end(); ++it){
    para.adzone_ = it->second;
    /*if(100000000073 == para.adzone_->id() && hide_flag) {
      continue;
    }*/
    int rotate_size = it->second->GetRotateSize();
    if (rotate_size) {
      para.rotate_index_ = rotate_index % rotate_size;
    }
    para.zone_selected_groups_.clear();

    for(size_t i = 0; i < ad_match_handler_list_.size(); ++i) {
      if (!ad_match_handler_list_[i]->handle(para, ans)) {
        //如果是相册页wb1(72)展示自助广告，则不展示wb2广告位(73)
        /*if((100000000072 == para.adzone_->id()) && (!ans[100000000072].empty())) {
          hide_flag = true;
        }*/
        break;
      }
    }
  }
  MCE_DEBUG("[REMAIN] AdMatchNManagerI::GetAds() --> log size = " << para.pv_log_seq_.size() << " + " << para.default_log72_.size());
  try {
    if(!para.pv_log_seq_.empty() && !para.default_log72_.empty()) {
      para.pv_log_seq_.insert(para.pv_log_seq_.begin(), para.default_log72_.begin(), para.default_log72_.end());
      AdLogReplicaAdapter::instance().PvBySeq(para.pv_log_seq_);
    } else if(!para.pv_log_seq_.empty()) {
      AdLogReplicaAdapter::instance().PvBySeq(para.pv_log_seq_);
    } else if(!para.default_log72_.empty()) {
      AdLogReplicaAdapter::instance().PvBySeq(para.default_log72_);
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("AdMatchManagerI::GetAds() --> AdLogReplicaAdapter.PvBySeq ERR line:"
        << __LINE__ << " err:" << e);
  } catch (std::exception& e) {
    MCE_WARN("AdMatchManagerI::GetAds() --> AdLogReplicaAdapter.PvBySeq ERR line:"
        << __LINE__ << " err:" << e.what());
  }

  MCE_DEBUG("[REMAIN] AdMatchNManagerI::GetAds() --> cpm_log size = " << para.cpm_pv_log_seq_.size() << " + " << para.cpm_default_log72_.size());
  try {
    if(!para.cpm_pv_log_seq_.empty() && !para.cpm_default_log72_.empty()) {
      para.cpm_pv_log_seq_.insert(para.cpm_pv_log_seq_.begin(), para.cpm_default_log72_.begin(), para.cpm_default_log72_.end());
      AdLogReplicaAdapter::instance().PvForCharge(para.cpm_pv_log_seq_);
    } else if(!para.cpm_pv_log_seq_.empty()) {
      AdLogReplicaAdapter::instance().PvForCharge(para.cpm_pv_log_seq_);
    } else if(!para.cpm_default_log72_.empty()) {
      AdLogReplicaAdapter::instance().PvForCharge(para.cpm_default_log72_);
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("AdMatchManagerI::GetAds() --> AdLogReplicaAdapter.PvForCharge ERR line:"
        << __LINE__ << " err:" << e);
  } catch (std::exception& e) {
    MCE_WARN("AdMatchManagerI::GetAds() --> AdLogReplicaAdapter.PvForCharge ERR line:"
        << __LINE__ << " err:" << e.what());
  }
  return ans;
}

void AdMatchManagerI::Click(int uid, const string& ip, const string& text,const string& referer ,const string& click_url ,const Ice::Current& current) {
  TimeStat ts;
  TimeStat subts;
  MCE_INFO("AdMatchManagerI::Click --> text = " << text);

  ostringstream clicklog, chargelog;
  ostringstream engine_click_log;
  LogParas para = LogHelper::instance().ParseText(text);
  UserProfile profile = para.user_profile_;

  if (!para.ubc_.empty()) {
    string deal_referer = referer;
    LogHelper::instance().Process(deal_referer);

    /*char sep = '&';
    if (click_url.find_first_of('?') == string::npos) {
      sep = '?';
    }
    ostringstream log_url;
    log_url << click_url << sep << "next_load_time=" << para.next_load_time_
        << "&refresh_idx=" << para.refresh_idx_;*/
    string deal_click_url = click_url;//log_url.str();
    LogHelper::instance().Process(deal_click_url);

    Ice::Long group_id = para.creative_id_ / 100000;
    AdGroupPtr group_ptr = AdGroupPool::instance().GetAdGroup(group_id);

    if (group_ptr) {
      MyUtil::Date date(time(NULL));

      MCE_INFO("[MAIN_PROCESS] AdMatchManagerI::Click --> userid = " << uid << " creativeid = " << para.creative_id_ << " text = " << text);

      MyUtil::Date pv_date(boost::lexical_cast<time_t>(para.pv_time_));
      if(para.cost_ > group_ptr->max_price()){
        MCE_INFO("AdMatchManagerI::Click --> highest_bid  ==> " << para.cost_ << " > " << group_ptr->max_price());
        para.cost_ = group_ptr->max_price();
      }
      double cost = para.cost_;
      double max_price = group_ptr->max_price();
      if(2 == group_ptr->display_type()) {
        cost = 0;
        max_price = 0;
      } else {
        chargelog << para.creative_id_ << ',' << group_id << ','
          << group_ptr->campaign_id() << ',' << group_ptr->member_id() << ',' << para.zone_id_ << ','
          << /*group_ptr->site_id*/'0' << ',' << /*group_ptr->site_id*/'0' << ',' << para.zone_id_ << ','
          << para.cost_ << ',' << LogHelper::instance().FormatedClickTime(time(NULL)) << ',' << para.cost_ << ','
          << para.cost_ << ',' << "1" << ',' << "lm" << ',' << EngineConfig::instance().sys_min_price()
          << "," <<  group_ptr->bid_unit_id() << ',' << ip << ',' << uid << ',' << '0';
        MCE_DEBUG("AdMatchManagerI::Click --> chargelog: " << chargelog.str());
      }

      //display_type:1=轮播, 2= cpm ,3=余量, 4= cpc, 5=产品
      clicklog << para.creative_id_ << "," << date.toDateTime() << "," << group_ptr->bid_unit_id()<< "," << cost << ","
          << ip << "," << para.zone_id_ << ",1000000_" << uid << "," << para.ubc_ << "," << deal_click_url << ","
          << para.pos_ << ",-," << group_ptr->member_id() << "," << "," << max_price << "," << "2,"<< deal_referer <<",,"
          << pv_date.toDateTime() <<"," << uid << ",2,1,1," << ip << ",,," << max_price << ",1,lm," << EngineConfig::instance().sys_min_price() << ','
          << group_ptr->bid_unit_id() << ",0,1," << group_ptr->display_type();
      if((!para.append_info_.empty()) && (para.append_info_.compare("-") != 0)) {
          clicklog << ',' << para.append_info_;
      }
      MCE_DEBUG("AdMatchManagerI::Click --> clicklog: " << clicklog.str());

      try{
        AdLogReplicaAdapter::instance().ClickForBp(clicklog.str());
      } catch(Ice::Exception& e){
        MCE_WARN("AdMatchManagerI::Click --> call AdLogReplicaAdapter.ClickForBp ERR  line:"<<__LINE__<<" err:"<<e);
      } catch(std::exception& e){
        MCE_WARN("AdMatchManagerI::Click --> call AdLogReplicaAdapter.ClickForBp ERR  line:"<<__LINE__<<" err:"<<e.what());
      }

      try{
        AdLogReplicaAdapter::instance().ClickForCharge(chargelog.str());
      } catch(Ice::Exception& e){
        MCE_WARN("AdMatchManagerI::Click --> call AdLogReplicaAdapter.ClickForCharge ERR  line:"<<__LINE__<<" err:"<<e);
      } catch(std::exception& e){
        MCE_WARN("AdMatchManagerI::Click --> call AdLogReplicaAdapter.ClickForCharge ERR  line:"<<__LINE__<<" err:"<<e.what());
      }

    } else {
      MCE_INFO("[MAIN_PROCESS] group NOT FOUND: group_id = " << group_id);
    }
  } else {
    MCE_INFO("[MAIN_PROCESS] NO ubc, text = " << text);
  }
}

void AdMatchManagerI::Init(){
  TimeStat ts;
  MCE_INFO("AdMatchManagerI::Init --> begin init :" << Date(time(NULL)).toDateTime());
  UserBindPool::instance().Init();
  MCE_INFO("AdMatchManagerI::Init --> UserBindPool::instance().Init(); DONE");
  AdMemberPool::instance().Init();
  MCE_INFO("AdMatchManagerI::Init --> AdMemberPool::instance().Init(); DONE");
  AdCampaignPool::instance().Init();
  MCE_INFO("AdMatchManagerI::Init -->  AdCampaignPool::instance().Init(); DONE");
  AdGroupPool::instance().Init();
  MCE_INFO("AdMatchManagerI::Init -->  AdGroupPool::instance().Init(); DONE");
  AdZonePool::instance().Init();
  MCE_INFO("AdMatchManagerI::Init -->  AdZonePool::instance().Init(); DONE");
  CompeteRef::instance().Init();
  MCE_INFO("AdMatchManagerI::Init -->  CompeteRef::instance().Init(); DONE");
}

void InitTask::handle(){
  SchoolPool::instance().Init();
  MCE_INFO("AdMatchManagerI::Init --> SchoolPool::instance().Init(); DONE");
  //UserBindPool::instance().Init();
  //MCE_INFO("AdMatchManagerI::Init --> UserBindPool::instance().Init(); DONE");
  AdMatchManagerI::instance().Init();
  MCE_INFO("AdMatchManagerI::Init --> AdMatchManagerI::instance().Init(); DONE");
  AdMatchManagerI::instance().InitOk();
  
  usleep(60 * 1000 * 1000);
  TaskManager::instance().execute(new ReloadTask());
}

/*void ReloadTimer::handle(){
  MCE_INFO("ReloadTimer::handle --> BEGIN RELOAD");
  AdMatchManagerI::instance().Init();
  MCE_INFO("ReloadTimer::handle --> END RELOAD");

}*/

void ReloadTask::handle(){
  while(true) {
    try {
      MCE_INFO("ReloadTask::handle --> BEGIN RELOAD");
      AdMatchManagerI::instance().Init();
      MCE_INFO("ReloadTask::handle --> END RELOAD");
      usleep(60 * 1000 * 1000);
    } catch (std::exception& e) {
      MCE_WARN("ReloadTask--> reload ERR  line:" << __LINE__ << " err:" << e.what());
    } catch (...) {
      MCE_WARN("ReloadTask--> reload ERR  line:" << __LINE__);
    }
  }
}

bool Filter::IsCompete(const Ice::Long& gid1, const Ice::Long& gid2){
  AdGroupPtr group1 = AdGroupPool::instance().GetAdGroup(gid1);
  AdGroupPtr group2 = AdGroupPool::instance().GetAdGroup(gid2);
  if(group1 == false || group2 == false){
		return false;
	}

	if (group1->member_id() == group2->member_id()){
    return true;
  }
  vector<Ice::Long> tmp = CompeteRef::instance().Get(group1->campaign_id());
  if (find(tmp.begin(), tmp.end(), group2->campaign_id()) != tmp.end()){
      return true;    
  }
  return false;
}


void Filter::FilterCompete(int uid, set<AdGroupPtr> & groups, const set<SelectedAdGroupPtr> & selected) {
  if (selected.empty()) {
    return;
  }

  set<Ice::Long> compete_campaigns;
  set<Ice::Long> selected_members;

  //将selected_gids的竟品和相同加入compete_gids,selected_campaign;
  for (set<SelectedAdGroupPtr>::iterator sit = selected.begin(); sit != selected.end(); ++sit) {
    vector < Ice::Long > tmp = CompeteRef::instance().Get((*sit)->group_->campaign_id());
    for (vector<Ice::Long>::iterator cit = tmp.begin(); cit != tmp.end(); ++cit) {
      compete_campaigns.insert(*cit);
    }
    selected_members.insert((*sit)->group_->member_id());
  }

  vector<AdGroupPtr> group_seq;
  for (set<AdGroupPtr>::iterator sit = groups.begin(); sit != groups.end();) {
    if (selected_members.count((*sit)->member_id())) { // 如果已经选择过这个member
      groups.erase(sit++);
      continue;
    }
    if (compete_campaigns.count((*sit)->campaign_id())) { // 如果已经被选择过
      groups.erase(sit++);
      continue;
    }
    group_seq.push_back(*sit);
    //groups.erase(sit++);
    sit++;
  }

  /*for(size_t i = 0; i < group_seq.size() && groups.size() < 4; ++i) {
	  groups.insert(group_seq.at(i));
  }*/ 
}

AdResultMap AdMatchManagerI::GetAdsTest(::Ice::Int uid, ::Ice::Int stage,
    ::Ice::Int gender, ::Ice::Int age, const ::std::string& currentArea,
    const ::std::string& ipArea, ::Ice::Long school, ::Ice::Int grade,
    int count, const string & referer, int rotate_index, const Ice::Current& current) {
  AdInfoSeq out;
  
  string serialized;
  if (uid > 0) {
    bool b = UserProfileClient::instance().GetSerialized(uid, &serialized);

    if (!b) {
      MCE_INFO("profile not found in memcache. id=" << uid); 
      /*try {
        serialized = AdUserCacheLoaderAdapter::instance().GetById(uid);
      } catch(Ice::Exception& e) {
        MCE_WARN("AdUserCacheLoaderAdapter GetById error : " << e << " uid=" << _login_uid);
      } catch(std::exception& e) {
        MCE_WARN("AdUserCacheLoaderAdapter GetById error : " << e.what() << " uid=" << _login_uid);
      }*/
    }
  }

  UserProfile profile;
  profile.ParseFromString(serialized);


  /*UserProfile profile;
  profile.set_age(age);
  profile.set_id(uid);
  profile.set_gender(gender);
  profile.set_stage(stage);
  profile.set_current_area(currentArea);
  profile.set_ip_area(ipArea);
  profile.set_school(school);
  profile.set_grade(grade);*/

  profile.set_ip(1896477159);

  string tmp;
  UserProfileClient::instance().Serialize(profile, &tmp);

  string tmpref = referer;
  return AdMatchManagerI::GetAds(tmp, tmpref, rotate_index);

}
xce::ad::AdZone2Creatives AdMatchManagerI::GetAdsByZone(const ::std::string&, const ::std::string&, ::Ice::Int, const ::Ice::Current&) {
  return AdZone2Creatives();
}
/***********************************************************/

