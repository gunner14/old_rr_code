/*
 * =====================================================================================
 *
 *       Filename:  config.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月04日 14时01分12秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "indexConfig.h"
#include "db.h"
#include <iostream>

using namespace xce::ad;


bool IndexConfig::lookupValue(const std::string &key, int &val)
{
	LOOKUP_NO_EXCEPTIONS(key, int, val);
}

bool IndexConfig::lookupValue(const std::string &key, uint64_t &val)
{
	LOOKUP_NO_EXCEPTIONS(key, uint64_t, val);
}

bool IndexConfig::lookupValue(const std::string &key, bool &val)
{
	LOOKUP_NO_EXCEPTIONS(key, bool, val);
}

bool IndexConfig::lookupValue(const std::string &key, double &val)
{
	LOOKUP_NO_EXCEPTIONS(key, double, val);
}

bool IndexConfig::lookupValue(const std::string &key, std::string &val)
{
	LOOKUP_NO_EXCEPTIONS(key, std::string, val);
}

void IndexConfig::setServerIndex(int idx)
{
	server_index_ = idx;
}

int IndexConfig::getServerIndex()
{
	return server_index_; 
}

int IndexConfig::getGameTotalAdNum(long zone)
{
  IceUtil::RWRecMutex::RLock rlock(lock_);
  std::map<long, int>::iterator it = game_ad_total_num_.find(zone);
  if(it != game_ad_total_num_.end())
  {
    return it->second;
  }
  else
  {
    return game_ad_total_num_[0];
  }
}

int IndexConfig::getGameMemberAdNum(long zone)
{
  IceUtil::RWRecMutex::RLock rlock(lock_);
  std::map<long, int>::iterator it = game_member_ad_num_.find(zone);
  if(it != game_member_ad_num_.end())
  {
    return it->second;
  }
  else
  {
    return game_member_ad_num_[0];
  }
}

void IndexConfig::getAdNumByAdzone(long adzone, unsigned &total_num, unsigned &new_num)
{
	IceUtil::RWRecMutex::RLock rlock(lock_);

	std::map<long, std::pair<unsigned, unsigned> >::iterator it = adzone_ad_num_.find(adzone);
	if(it != adzone_ad_num_.end())
	{
		new_num = it->second.second;	
    total_num = it->second.first + new_num;
	}
	else
	{
		new_num = adzone_ad_num_[0].second;
		total_num = adzone_ad_num_[0].first + new_num;
	}
}


std::map<std::string, int> IndexConfig::getTriggerInfo()
{
	return trigger_info_;
}

int IndexConfig::loadConfig()
{
	MCE_INFO("BEGIN IndexConfig::loadConfig");
	int ret = load();
	MCE_INFO("END IndexConfig::loadConfig");

  return ret;
}

int IndexConfig::reloadConfig()
{
  MCE_INFO("BEGIN IndexConfig::reloadConfig");
	int ret = load();
  MCE_INFO("END IndexConfig::reloadConfig");

  return ret;
}

int IndexConfig::load()
{
	std::map<long, std::pair<unsigned, unsigned> > ad_num_config;
	std::map<std::string, int> trigger_info;
  std::map<long, int> game_ad_total_num;
  std::map<long, int> game_member_ad_num;

  std::map<std::string, std::string> common_config;
	int ret = DBSelector::instance().loadCommonConfig(common_config);
  if(ret < 0)
  {
    return -1;
  }

  for(std::map<std::string, std::string>::iterator it = common_config.begin();
      it != common_config.end(); ++it)
  {
    std::pair<std::string, std::string> res = split(it->first, "_");

    const std::string prefix = res.first;
    const std::string &key = res.second;
    const std::string &value = it->second;

    if(prefix == "adzone_ad_num_old" || prefix == "adzone_ad_num_new")
    {
      long adzone = boost::lexical_cast<long>(key);
      int num = boost::lexical_cast<int>(value);
      std::map<long, std::pair<unsigned, unsigned> >::iterator nit = ad_num_config.find(adzone);
      if(nit != ad_num_config.end())
      {
        prefix == "adzone_ad_num_old" ? nit->second.first = num : nit->second.second = num;
      }
      else
      {
        int old_ad_num = 0, new_ad_num = 0;
        prefix == "adzone_ad_num_old" ? old_ad_num = num : new_ad_num = num;
        std::pair<unsigned, unsigned> nums = make_pair(old_ad_num, new_ad_num);
        ad_num_config[adzone] = nums;
      }
    }
    else if(prefix == "trigger_type")
    {
      trigger_info[key] = boost::lexical_cast<int>(value);
    }
    else if(prefix == "adzone_game_ad_total_num")
    {
      long adzone = boost::lexical_cast<long>(key);
      int num = boost::lexical_cast<int>(value);
      game_ad_total_num[adzone] = num;
    }else if(prefix == "adzone_game_member_ad_num"){
      long adzone = boost::lexical_cast<long>(key);
      int num = boost::lexical_cast<int>(value);
      game_member_ad_num[adzone] = num;
    }
    else if(prefix == "ad_close_expired_minute"){
      ad_close_expired_minute_ = boost::lexical_cast<int>(value);
    } 
    else if (prefix == "ad_send_speed_lower"){
      lower_db_source_ = value;
    }
    else
    {
      MCE_WARN("LoadConfig get an unknown config:  " << it->first << ", prefix:" << prefix);
    }
  }


  if(ad_num_config[0].first == 0 || ad_num_config[0].second == 0 || 
      game_ad_total_num[0] == 0 || game_member_ad_num[0] == 0)
  {
    MCE_WARN("default config is not set(adzone_ad_num_ or game_ad_total_num_ or game_member_ad_num_)");
  }
  else if(ad_close_expired_minute_ <= 24 * 60){
    MCE_WARN("ad_close_expired_minute_ is abnormal: " << ad_close_expired_minute_);
  }
  
	{
		IceUtil::RWRecMutex::WLock wlock(lock_);
		adzone_ad_num_.swap(ad_num_config);
		trigger_info_.swap(trigger_info);
    game_ad_total_num_.swap(game_ad_total_num);
    game_member_ad_num_.swap(game_member_ad_num);
	}

  outputConfig();
	return 0;
}

void IndexConfig::outputConfig()
{
	MCE_INFO("                                                                   ");
	MCE_INFO("######################### BEGIN Adzone_ad_num Config ####################");
	std::map<long, std::pair<unsigned, unsigned> >::iterator it = adzone_ad_num_.begin();
	for(; it != adzone_ad_num_.end(); ++it)
	{
		unsigned old_ad_num = it->second.first;
		unsigned new_ad_num = it->second.second;
		long adzone = it->first;
		MCE_INFO("adzone: " << adzone << ", old_ad_num: " << old_ad_num << 
				",new_ad_num: " << new_ad_num);
	}
	MCE_INFO("######################### END Adzone_ad_num Config ########################");
	MCE_INFO("                                                                   ");

	MCE_INFO("######################### BEGIN TriggerInfo Config ########################");
	std::map<std::string, int>::iterator tit = trigger_info_.begin();
	for(; tit != trigger_info_.end(); ++tit)
	{
		MCE_INFO("trigger_name: " << tit->first << ", trigger_type: " << tit->second);
	}
	MCE_INFO("######################## END TriggerInfo Config ############################");
	MCE_INFO("                                                                   ");

  MCE_INFO("######################### BEGIN GameTotalAdNum Config ########################");
	std::map<long, int>::iterator git = game_ad_total_num_.begin();
	for(; git != game_ad_total_num_.end(); ++git)
	{
		MCE_INFO("adzone: " << git->first << ", game_total_ad_num: " << git->second);
	}
	MCE_INFO("######################## END GameTotalAdNum Config ############################");
	MCE_INFO("                                                                   ");

  MCE_INFO("######################### BEGIN GameMemberAdNum Config ########################");
	for(git = game_member_ad_num_.begin(); git != game_member_ad_num_.end(); ++git)
	{
		MCE_INFO("adzone: " << git->first << ", game_member_ad_num: " << git->second);
	}
	MCE_INFO("######################## END GameMemberAdNum Config ############################");
	MCE_INFO("                                                                   ");

  MCE_INFO("######################### BEGIN AdClose Config ########################");
	MCE_INFO("ad_close_expired_minute: " << ad_close_expired_minute_);
	MCE_INFO("######################## END AdClose Config ############################");
	MCE_INFO("                                                                   ");


}

std::pair<std::string, std::string> IndexConfig::split(const std::string &s, const std::string &spliter)
{
  std::pair<std::string, std::string> res;
  std::string::size_type pos = s.rfind(spliter);

  if(pos != std::string::npos)
  {
    res.first = s.substr(0, pos);
    res.second = s.substr(pos+1);
  }
  return res;
}


void ConfigTimer::process()
{
	IndexConfig *config = (IndexConfig*)instance_;
	if(config == NULL)
	{
		MCE_WARN("ConfigTimer::process get a NULL pointer");
		return ;
	}
	config->reloadConfig();
}

int ConfigTimer::interval() const
{
	return 60;
}

template<typename T1, typename T2>
ostream& xce::ad::operator <<(ostream& os, const std::pair<T1, T2>& sp){
  os<<sp.first<<","<<sp.second;
  return os;
}

template<typename KEY, typename VALUE>
ostream& xce::ad::operator <<(ostream& os, const map<KEY, VALUE >& mp) {
  typename map<KEY, VALUE >::const_iterator it = mp.begin();
  for (int i=1; it != mp.end(); ++it, ++i) {
    os << "[" << i << "]" << it->first << ":" << it->second << ";";
  }
  return os;
}

/*
 * Func:IndexConfig::PrintZoneAdNum
 * Desc:广告位上的广告数
 */
string IndexConfig::PrintZoneAdNum() {
  IceUtil::RWRecMutex::RLock lock(lock_);
  ostringstream os;
  os << adzone_ad_num_;
  return os.str();
}

/*
 * Func:IndexConfig::PrintTriggerInfo
 * Desc:trigger info
 */
string IndexConfig::PrintTriggerInfo() {
  IceUtil::RWRecMutex::RLock lock(lock_);
  ostringstream os;
  os << trigger_info_;
  return os.str();
}

/*
 * Func:IndexConfig::PringGameAdNum
 * Desc:广告位游戏数
 */
string IndexConfig::PrintGameAdNum() {
  IceUtil::RWRecMutex::RLock lock(lock_);
  ostringstream os;
  os << game_ad_total_num_;
  return os.str();
}

/*
 * Func:IndexConfig::PrintGameMemNum
 * Desc:广告位游戏商数
 */
string IndexConfig::PrintGameMemNum() {
  IceUtil::RWRecMutex::RLock lock(lock_);
  ostringstream os;
  os << game_member_ad_num_;
  return os.str();
}

