#include <connection.h>
#include <query.h>
#include <boost/algorithm/string/predicate.hpp>
#include <cstdlib>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/regex.hpp>
#include "QueryRunner.h"
#include "UrlMaping.h"
#include "AdMatchManager.h"
#include "Notify/util/url_encode.h"
#include "Notify/util/json_escape.h"
#include "ServiceI.h"
#include "AdMatchNAdapter.h"
#include "ad_creative_cache.h"
using namespace xce::ad;
using namespace std;
using namespace com::xiaonei::xce;
using namespace MyUtil;
using namespace mysqlpp;

static const char * kJsonTplFileS = "../etc/ad_ebpn_tpl/json_gates.tpl";
static const char * kJsonTplFile = "../etc/ad_ebpn_tpl/json_gaten.tpl";

AdCreativeCache::AdCreativeCache() : json_gates_template_(0), json_gaten_template_(0) {
}

AdCreativeCache::~AdCreativeCache() {
  delete json_gates_template_;
  delete json_gaten_template_;
}

bool AdCreativeCache::Reload() {
  // TODO : template表很大的时候，只加载最新更新的template
  AdCache ad_cache;
  map<Ice::Long, vector<Ice::Long> > default_ads;
  map<Ice::Long, set<Ice::Long> > tmp_adzone_adgroup_map;

  set<long> active_members;
  set<long> active_campaigns;
  set<long> active_groups;
  map<long,long> bid_groups_widget;

  MCE_INFO("AdCreativeCache::Reload() --> db_instance = " << db_instance);

  {
    Statement sql;
    sql << "SELECT member_id FROM member WHERE is_formal = 1 AND validity = 1 AND "
       "have_money = 1 AND no_exceed_max = 1 AND am_online_status = 1 AND ma_online_status = 1 AND delete_flag = 1";

    try {
      mysqlpp::StoreQueryResult res = QueryRunner(db_instance, CDbRServer).store(sql);
      if (!res) {
        MCE_WARN("select table ad_member error.");
        return false;
      }

      for (size_t i = 0; i < res.num_rows(); ++i) {
        mysqlpp::Row row = res.at(i);
        active_members.insert((long)row["member_id"]);
      }
    } catch (std::exception& e) {
      MCE_WARN("load db err : " << e.what());
      return false;
    } catch (...) {
      MCE_WARN("load db unknown err.");
      return false;
    }
  }
  if (active_members.empty()) {
    MCE_WARN("no active members !");
    return false;
  }
  MCE_INFO("active member size " << active_members.size());

  {
    Statement sql;
    sql << "SELECT campaign_id FROM campaign WHERE begin_time < now() AND (end_time>now() OR end_time is NULL) AND sys_status = 1 AND "
       "member_status = 1 AND am_status = 1 AND audit_status = 1 AND delete_flag = 1";

    try {
      mysqlpp::StoreQueryResult res = QueryRunner(db_instance, CDbRServer).store(sql);
      
      if (!res) {
        MCE_WARN("select table ad_campaign error.");
        return false;
      }

      for (size_t i = 0; i < res.num_rows(); ++i) {
        mysqlpp::Row row = res.at(i);
        active_campaigns.insert((long)row["campaign_id"]);
      }
    } catch (std::exception& e) {
      MCE_WARN("load db err : " << e.what());
      return false;
    } catch (...) {
      MCE_WARN("load db unknown err.");
      return false;
    }
  }
  if (active_campaigns.empty()) {
    MCE_WARN("no active campaign !");
    return false;
  }
  MCE_INFO("active campaign_id size " << active_campaigns.size());

  {
    Statement sql;
    sql << "SELECT adgroup_id FROM adgroup WHERE member_status = 1 AND am_status = 1 AND audit_status = 1 AND delete_flag = 1 "
      "AND daily_sys_status = 1 AND total_sys_status = 1";

    try {
      mysqlpp::StoreQueryResult res = QueryRunner(db_instance, CDbRServer).store(sql);
      
      if (!res) {
        MCE_WARN("select table ad_group error.");
        return false;
      }

      for (size_t i = 0; i < res.num_rows(); ++i) {
        mysqlpp::Row row = res.at(i);
        active_groups.insert((long)row["adgroup_id"]);
      }
    } catch (std::exception& e) {
      MCE_WARN("load db err : " << e.what());
      return false;
    } catch (...) {
      MCE_WARN("load db unknown err.");
      return false;
    }
  }

//bidunit
  {
    Statement sql;
    sql << "SELECT adgroup_id,campaign_id,member_id FROM bid_unit WHERE delete_flag = 1 AND cast_area=\"0086000000000000\""
      "AND cast_stage = 6 AND cast_gender = 2";

    try {
      mysqlpp::StoreQueryResult res = QueryRunner(db_instance, CDbRServer).store(sql);
      
      if (!res) {
        MCE_WARN("select table bid_unit error.");
        return false;
      }

      for (size_t i = 0; i < res.num_rows(); ++i) {
        mysqlpp::Row row = res.at(i);
        long group_id = (long)row["adgroup_id"];
        long campaign_id = (long)row["campaign_id"];
        long member_id = (long)row["member_id"];
        map<long,long> group_creative;
        if (active_members.find(member_id) == active_members.end() || 999 == member_id) {
          continue;
        }
        if (active_campaigns.find(campaign_id) == active_campaigns.end()) {
          continue;
        }
        if (active_groups.find(group_id) == active_groups.end()) {
          continue;
        }
        bid_groups_widget[group_id]=0;
      }
    } catch (std::exception& e) {
      MCE_WARN("load db err : " << e.what());
      return false;
    } catch (...) {
      MCE_WARN("load db unknown err.");
      return false;
    }

  }
  if (bid_groups_widget.empty()) {
    MCE_WARN("no bid group!");
    return false;
  }
  MCE_INFO("bid_group size " << bid_groups_widget.size());

  map<long, set<long> > default_group_zones;
  {
    // 加载 默认 ad group �?zone 信息
    Statement sql;
    sql << "SELECT adzone_id, adgroup_id FROM adgroup_adzone_ref WHERE member_id=999 AND member_status=1 AND am_status=1 AND delete_flag=1";
    try {
      mysqlpp::StoreQueryResult res = QueryRunner(db_instance, CDbRServer).store(sql);
      if (res) {
        for (size_t i = 0; i < res.num_rows(); ++i) {
          mysqlpp::Row row = res.at(i);
          long zone_id = (long)row["adzone_id"];
          long group_id = (long)row["adgroup_id"];
          default_group_zones[group_id].insert(zone_id);
        }
      }
    } catch (std::exception& e) {
      MCE_WARN("load table adgroup_adzone_ref err : " << e.what());
      return false;
    } catch (...) {
      MCE_WARN("load table adgroup_adzone_ref unknown err.");
      return false;
    }
  }

  {
    // 加载 creative 信息
    Statement sql;

    sql << "SELECT creative_id, adgroup_id, campaign_id, member_id, widget, "
        "widget_id, widget_version, creative_name, content, click_url, media_uri, "
        "last_update_time FROM creative where member_status = 1 and am_status = 1 "
        "and audit_status = 1 and delete_flag = 1";

    try {
      mysqlpp::StoreQueryResult res = QueryRunner(db_instance, CDbRServer).store(sql);
      
      if (!res) {
        MCE_WARN("select table creative error.");
        return false;
      }

      MCE_DEBUG("loaded ad size : " << res.num_rows());

      for (size_t i = 0; i < res.num_rows(); ++i) {
        mysqlpp::Row row = res.at(i);
        long id = (long)row["creative_id"];
        long member_id = (long)row["member_id"];
        long group_id = (long)row["adgroup_id"];
        long widget_id = (long)row["widget_id"];
        if (member_id == 999) {
          map<long, set<long> >::iterator it = default_group_zones.find(group_id);
          if (it != default_group_zones.end()) {
            // 一个默�?group 可能在多�?zone 上展�? zone �?group 是多对多�?
            for(set<long>::iterator zone_it = it->second.begin(); zone_it != it->second.end(); ++ zone_it) {
              default_ads[*zone_it].push_back(id);
            }
          }
        }
        if (bid_groups_widget.find(group_id) == bid_groups_widget.end()) {
          continue;
        } else {
          bid_groups_widget[group_id] = widget_id;
        }
        {
          AdCreativePtr  ad = new AdCreative();
          ad->id = id;
          ad->widget = row["widget"].c_str();
          ad->widget_id = widget_id; 
          ad->widget_version = row["widget_version"].c_str();
          ad->group_id = group_id;
          ad->member_id = member_id;

          ad->title = row["creative_name"].c_str();

          ad->content = row["content"].c_str();
          ad->img_url = row["media_uri"].c_str();
          if (ad->img_url.compare("NULL") == 0) {
            ad->img_url = "";
          }
          ad->dest_url = xce::notify::UrlEncode(row["click_url"].c_str());
          ad->update_time = (time_t)((mysqlpp::DateTime)row["last_update_time"]);
          ad_cache[id] = ad;

        }
      }
    } catch (std::exception& e) {
      MCE_WARN("load db err : " << e.what());
      return false;
    } catch (...) {
      MCE_WARN("load db unknown err.");
      return false;
    }
  }
  {
    Statement sql;

    sql << "SELECT adgroup_id, adzone_id FROM adgroup_adzone_ref where member_status = 1 and am_status = 1 "
        "and show_flag = 1 and delete_flag = 1";

    try {
      mysqlpp::StoreQueryResult res = QueryRunner(db_instance, CDbRServer).store(sql);

      if (!res) {
        MCE_WARN("select table adgroup_adzone_ref error.");
        return false;
      }
      for (size_t i = 0; i < res.num_rows(); ++i) {
        mysqlpp::Row row = res.at(i);
        Ice::Long zone_id = boost::lexical_cast<Ice::Long>(row["adzone_id"]);
        Ice::Long group_id = boost::lexical_cast<Ice::Long>(row["adgroup_id"]);
        if (bid_groups_widget.find(group_id) == bid_groups_widget.end()) {
          continue;
        }
        int widget_id = bid_groups_widget[group_id];
        if(100000000065 == zone_id || 100000000093 == zone_id || 100000000070 == zone_id || 100000000072 == zone_id || 100000000075 == zone_id) {
          //TODO DONE?
          if(2 != Widget2TypeCache::instance().GetWidgetType(widget_id)) {
            continue;
          }
        } else if(100000000063 == zone_id) {
          if(1 != Widget2TypeCache::instance().GetWidgetType(widget_id)) {
            continue;
          }
        }
        map< Ice::Long, set<Ice::Long> >::iterator it1 = tmp_adzone_adgroup_map.find(zone_id);
        if(it1 != tmp_adzone_adgroup_map.end()) {
          it1->second.insert(group_id);
        } else {
          set<Ice::Long> adgroups;
          adgroups.insert(group_id);
          tmp_adzone_adgroup_map[zone_id] = adgroups;
        }
      }
    } catch (std::exception& e) {
      MCE_WARN("load db err : " << e.what());
      return false;
    } catch (...) {
      MCE_WARN("load db unknown err.");
      return false;
    }

  }
  if(!tmp_adzone_adgroup_map.empty()) {
    IceUtil::RWRecMutex::WLock lock(group_mutex_);
    adzone_adgroup_map_.swap(tmp_adzone_adgroup_map);
    /*map< Ice::Long, set<Ice::Long> >::iterator itzone2group = adzone_adgroup_map_.begin();
    for(; itzone2group != adzone_adgroup_map_.end(); ++itzone2group) {
      set<Ice::Long> adgroups = itzone2group->second;
      ostringstream groupsstream;
      set<Ice::Long>::iterator itgroups = adgroups.begin();
      for(;itgroups != adgroups.end(); ++ itgroups) {
        groupsstream << *itgroups << "^";
      }
      MCE_DEBUG("adzone:" << itzone2group->first << ";groups:" << groupsstream.str());
    }*/
  } else {
    MCE_WARN("AdCreativeCache load error.");
    return false;
  }

  if(!ad_cache.empty()) {
    ad_cache_.swap(ad_cache);
  } else {
    MCE_WARN("ad_cache_ load error.");
    return false;
  }
  if(!default_ads.empty()) {
    IceUtil::RWRecMutex::WLock lock(group_mutex_);
    default_ads_.swap(default_ads);
  } else {
    MCE_WARN("Default ads load error.");
    return false;
  }
  MCE_INFO("AdCreativeCache load success.");
  return true;
}
bool AdCreativeCache::LoadTemplate(){
  {
    if (json_gates_template_) {
      IceUtil::RWRecMutex::WLock lock(tpl_s_mutex_);
      if (json_gates_template_->ReloadIfChanged()) {
        MCE_INFO("template " << kJsonTplFileS << " reloaded successfully.");
      }
    } else {
      json_gates_template_= ctemplate::Template::GetTemplate(kJsonTplFileS, ctemplate::DO_NOT_STRIP);
    }
    if (!json_gates_template_) {
      MCE_WARN("load template " << kJsonTplFileS << " fail.");
      return false;
    }
  }
  {
    if (json_gaten_template_) {
      IceUtil::RWRecMutex::WLock lock(tpl_n_mutex_);
      if (json_gaten_template_->ReloadIfChanged()) {
        MCE_INFO("template " << kJsonTplFile << " reloaded successfully.");
      }
    } else {
      json_gaten_template_= ctemplate::Template::GetTemplate(kJsonTplFile, ctemplate::DO_NOT_STRIP);
    }
    if (!json_gaten_template_) {
      MCE_WARN("load template " << kJsonTplFile << " fail.");
      return false;
    }
  }
  return true;
}

AdCreativePtr AdCreativeCache::GetCreativeidByGroupid(long group_id) const {
  MCE_DEBUG("GetCreativeidByGroupid");
  IceUtil::RWRecMutex::RLock lock(mutex_);
  for(AdCache::const_iterator it = ad_cache_.begin(); it != ad_cache_.end(); ++it) {
    if(it->second->group_id == group_id) {
      return it->second;
    }
  }
  return NULL;
}

void AdCreativeCache::GetAdsByZoneInfo(const set<Ice::Long> & zoneids, AdZone2Creatives & ads) const {
  MCE_DEBUG("GetAdsByZoneInfo");
  int abs_pos;
  int num;
  vector<Ice::Long> tmp;
  set<Ice::Long> groupid_exist;
  IceUtil::RWRecMutex::RLock lock(group_mutex_);
  for(set<Ice::Long>::iterator zone_it = zoneids.begin(); zone_it != zoneids.end(); ++zone_it) {
    Ice::Long zone_id = *zone_it;
    map< Ice::Long, set<Ice::Long> >::const_iterator it_zone2group = adzone_adgroup_map_.find(zone_id);
    if (it_zone2group == adzone_adgroup_map_.end()) {
      continue;
    }
    AdCreativeSeq ad_creative_seq;
    abs_pos = 0;
    tmp.clear();
    for(set<Ice::Long>::iterator p = it_zone2group->second.begin(); p != it_zone2group->second.end(); ++p) {
      tmp.push_back(*p);
    }
    if(tmp.size() != 0) {
      srand((unsigned)time(NULL));
      random_shuffle(tmp.begin(), tmp.end());
      if(zone_id == 100000000001 || zone_id == 100000000063) {
        num = 3;
      } else {
        num = 1;
      }
      vector<Ice::Long>::iterator tmp_p = tmp.begin();
      for(int i = 0; i < num && tmp_p != tmp.end(); ++tmp_p) {
        AdCreativeInfo ci;
        AdCreativePtr cp;
        if(groupid_exist.find(*tmp_p) == groupid_exist.end()) {
          ci.groupid = *tmp_p;
          cp = GetCreativeidByGroupid(ci.groupid);
          if(!cp) {
            continue;
          }
          ci.creativeid = cp->id;
          ci.widgetid = cp->widget_id;
          ci.position = ++abs_pos;
          ci.text = "";
          ci.extra = "";
          ad_creative_seq.push_back(ci);
          groupid_exist.insert(ci.groupid);
          ++i;
        }
      }
      int default_num = num - ad_creative_seq.size();
      if(default_num > 0) {
        MCE_DEBUG("GetAdsByZoneInfo--->push default;defalut_num = " << default_num);
        map<Ice::Long, vector<Ice::Long> >::const_iterator defaultit = default_ads_.find(zone_id);
        if(defaultit != default_ads_.end()) {
          tmp = defaultit->second;
          random_shuffle(tmp.begin(), tmp.end());
          tmp_p = tmp.begin();
          for(int j = 0; j < default_num && tmp_p != tmp.end(); ++tmp_p) {
            Ice::Long creative_id = *tmp_p;
            AdCache::const_iterator adcacheit = ad_cache_.find(creative_id);
            if(adcacheit != ad_cache_.end()) {
              AdCreativeInfo ci;
              AdCreativePtr cp;
              cp = adcacheit->second;
              ci.groupid = cp->group_id;
              ci.creativeid = cp->id;
              ci.widgetid = cp->widget_id;
              ci.position = ++abs_pos;
              ci.text = "";
              ci.extra = "";
              ad_creative_seq.push_back(ci);
              ++j;
            }
          }
        }
      }
      ads[zone_id] = ad_creative_seq;
    }
  }
}

void AdCreativeCache::GetJson(const AdZone2Creatives & zone_ads_map, string * html) const {
  ctemplate::TemplateDictionary dict("json");
  MyUtil::Date date(time(NULL));
  string pv_time = date.toDateTime();
  dict.SetIntValue("UPDATE_INTERVAL", 100);
  dict.SetIntValue("MAX_UPDATE_COUNT", 0);
  dict.SetIntValue("REFRESH_SOURCE", 0);
  dict.SetIntValue("REFRESH_IDX", 0);
  dict.SetValue("REFERER", "");

  {
    IceUtil::RWRecMutex::RLock lock(mutex_);
    bool item_sep = false;
    for(AdZone2Creatives::const_iterator map_it = zone_ads_map.begin(); map_it != zone_ads_map.end(); ++ map_it) {
      const AdCreativeSeq & ads = map_it->second;
      long zone_id = map_it->first;

      ctemplate::TemplateDictionary* ad_zone_dict = dict.AddSectionDictionary("SEC_AD_ZONES");
      ad_zone_dict->SetIntValue("AD_ZONE_ID", zone_id);
      if (map_it != zone_ads_map.begin()) {
        ad_zone_dict->SetValue("AD_ZONE_SEP", ",");
      }

      ad_zone_dict->SetIntValue("ENGINE_TYPE", 7);

      item_sep = false;
      for(size_t i = 0; i < ads.size(); ++i) {
        AdCache::const_iterator itc = ad_cache_.find(ads[i].creativeid);
        AdCreativePtr ad;
        if (itc != ad_cache_.end()) {
          ad = itc->second;
        }
        if (!ad) {
          MCE_INFO("ad " << ads[i].creativeid << " not found in memory.");
          continue;
        }
        ctemplate::TemplateDictionary* ad_item_dict = ad_zone_dict->AddSectionDictionary("SEC_AD_ITEMS");
        if (item_sep) {
          ad_item_dict->SetValue("AD_ITEM_SEP", ",");
        }
        item_sep = true;
        ad_item_dict->SetValue("ENGINE","-1");
        MCE_DEBUG("ad->widget_id = " << ad->widget_id);
        ad_item_dict->SetIntValue("AD_WIDGET_ID", ad->widget_id);
        ad_item_dict->SetValue("AD_WIDGET_VERSION", ad->widget_version);
        ad_item_dict->SetValue("AD_WIDGET", xce::notify::JsonEscape(ad->widget));
        ad_item_dict->SetValue("AD_DEST_URL", ad->dest_url);
        ad_item_dict->SetValue("AD_MATCHER_COOKIE", xce::notify::UrlEncode(ads[i].text));
        ad_item_dict->SetIntValue("AD_CREATIVE_ID", ads[i].creativeid);
        ad_item_dict->SetValue("AD_TITLE", xce::notify::JsonEscape(ad->title));
        ad_item_dict->SetValue("AD_CONTENT", xce::notify::JsonEscape(ad->content));
        ad_item_dict->SetValue("AD_PVTIME", pv_time);
        ad_item_dict->SetValue("AD_MEDIA_URI", xce::notify::UrlEncode(ad->img_url));
      }
    }
  }

  {
    IceUtil::RWRecMutex::RLock lock(tpl_n_mutex_);
    if (json_gaten_template_) {
      json_gaten_template_->Expand(html, &dict);
    }
  }
}

void AdCreativeCache::GetJsonS(const AdZone2Creatives& zone_ads_map, string * html) const {
  ctemplate::TemplateDictionary root_dict("json");

  MCE_DEBUG("AdCreativeCache::GetJson --> enter zone_ads_map.size() = " << zone_ads_map.size());
  IceUtil::RWRecMutex::RLock lock(mutex_);
  bool item_sep = false;
  for(AdZone2Creatives::const_iterator map_it = zone_ads_map.begin(); map_it != zone_ads_map.end(); ++ map_it) {
    const AdCreativeSeq& ads = map_it->second;
    long zone_id = map_it->first;
    ctemplate::TemplateDictionary* ad_zone_dict = root_dict.AddSectionDictionary("SEC_AD_ZONES");
    ad_zone_dict->SetIntValue("AD_ZONE_ID", zone_id);
    ad_zone_dict->SetIntValue("NEXT_LOAD_TIME", 0);
    if (map_it != zone_ads_map.begin()) {
      ad_zone_dict->SetValue("AD_ZONE_SEP", ",");
    }
    ad_zone_dict->SetValue("REFERER", "");
    item_sep = false;
    for(size_t i = 0; i < ads.size(); ++i) {
      AdCache::const_iterator itc = ad_cache_.find(ads[i].creativeid);
      AdCreativePtr ad;
      if (itc != ad_cache_.end()) {
        ad = itc->second;
      }
      if (!ad) {
        MCE_INFO("ad " << ads[i].creativeid << " not found in memory.");
        continue;
      }

      ctemplate::TemplateDictionary* ad_item_dict = ad_zone_dict->AddSectionDictionary("SEC_AD_ITEMS");

      if (item_sep) {
        ad_item_dict->SetValue("AD_ITEM_SEP", ",");
      }
      item_sep = true;

      MCE_DEBUG("AdCreativeCache::GetJson --> creative_id = " << ads[i].creativeid << ", widget_id = " << ads[i].widgetid);
      if(ads[i].widgetid) {
        ad_item_dict->SetIntValue("AD_WIDGET_ID", ads[i].widgetid);
      } else {
        ad_item_dict->SetIntValue("AD_WIDGET_ID", ad->widget_id);
      }
      ad_item_dict->SetValue("AD_WIDGET_VERSION", ad->widget_version);
      ad_item_dict->SetValue("AD_WIDGET", xce::notify::JsonEscape(ad->widget));
      ad_item_dict->SetValue("AD_DEST_URL", ad->dest_url);
      ad_item_dict->SetValue("AD_MATCHER_COOKIE", xce::notify::UrlEncode(ads[i].text));
      ad_item_dict->SetIntValue("AD_CREATIVE_ID", ads[i].creativeid);
      ad_item_dict->SetValue("AD_TITLE", xce::notify::JsonEscape(ad->title));
      ad_item_dict->SetValue("AD_CONTENT", xce::notify::JsonEscape(ad->content));
      ad_item_dict->SetValue("AD_MEDIA_URI", ad->img_url);
    }
  }

  {
    IceUtil::RWRecMutex::RLock lock(tpl_s_mutex_);
    if (json_gates_template_) {
      json_gates_template_->Expand(html, &root_dict);
    }
  }
}

void Widget2TypeCache::Load() {
  MCE_INFO("[Widget2Type]Widget2TypeCache::Load start");
  try {
    const char* db_host = "statdb.db.jebe.d.xiaonei.com";
    const char* db_name = "jebe_main";
    Connection conn(db_name,db_host,"ad","adjb###");
    stringstream sql; 
    sql << "select * from widget_type";
    MCE_INFO("Widget2TypeCache::Load try to excute " << sql.str());
    string s= sql.str();
    Query q(&conn,true,s.c_str());
    mysqlpp::StoreQueryResult res = q.store();
    if (!res) {
      MCE_WARN("Widget2TypeCache::Load Widget2TypeCache Error");
      return;
    }    
    MCE_INFO("Widget2TypeCache::Load Widget2TypeCache, Widget2TypeCache Size=: " << res.num_rows());
    map<Ice::Long, int> pool;
    for (size_t i = 0; i < res.num_rows(); ++i) {
      mysqlpp::Row row = res.at(i);
      Ice::Long widget_id = (Ice::Long) row["widget_id"];
      int type = (int)row["type"];
      pool[widget_id] = type;
    }    
    IceUtil::RWRecMutex::WLock lock(mutex_);
    if(!pool.empty()) {
      widget2Type_map_.swap(pool);
      MCE_INFO("End: Widget2TypeCache::Load size = " << widget2Type_map_.size());
    } else {
      MCE_WARN("Widget2TypeCache::Load err");
    }

  }catch (Ice::Exception& e) { 
    MCE_WARN("Widget2TypeCache::load --> call Database ERR  line:"
        << __LINE__ << " err:" << e);
  } catch (std::exception& e) { 
    MCE_WARN("Widget2TypeCache::load --> call Database ERR  line:"
        << __LINE__ << " err:" << e.what());
  } catch (...) {
    MCE_WARN("Widget2TypeCache::LoadDiscountCache error!");
  }
}
