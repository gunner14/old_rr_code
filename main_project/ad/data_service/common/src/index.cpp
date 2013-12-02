/*
 * =====================================================================================
 *
 *       Filename:  index.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年04月11日 11时10分04秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */


#include <boost/tokenizer.hpp>
#include <bitset>
#include "Date.h"
#include "index.h"
#include "db.h"
#include "indexConfig.h"

using namespace xce::ad;

//typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

bool PointsChecker::check(const std::string &days, const std::string & hours)
{
	return checkDay(days) && checkHour(hours);
}

bool PointsChecker::checkHour(const std::string &hours)
{
	if (hours.empty()) 
	{
		return true;
	}

	std::bitset < 24 > points;
	try 
	{
		points = boost::lexical_cast<int> (hours);
	} 
	catch (...) 
	{
		MCE_WARN("PointsChecker::checkHour error: "  << hours);	
		return true;
	}

	MyUtil::Date date = MyUtil::Date(time(NULL));
	int hour = date.hour();

	return points.test(hour);
}

bool PointsChecker::checkDay(const std::string &days)
{
	if(days.empty())
	{
		return true;
	}

	boost::tokenizer<> tok(days);
	boost::tokenizer<>::iterator it = tok.begin();

	int year = boost::lexical_cast<int> ((*it).substr(0, 4));
	int month = boost::lexical_cast<int> ((*(it++)).substr(4, 2));

	std::map<int, bitset<32> > yearmonth_day;
	for (; it != tok.end(); ++it)
	{
		if (*it != "0")
		{
			yearmonth_day[year * 100 + month] = boost::lexical_cast<int> (*it);
		}

		if (month == 12)
		{
			++year;
			month = 1;
		}
		else
		{
			++month;
		}
	}

	MyUtil::Date date = MyUtil::Date(time(NULL));
	int this_day = date.day();
	int this_month = date.month();
	int this_year = date.year();
	int yearmonth = this_year * 100 + this_month;

	std::map<int, bitset<32> >::iterator mit = yearmonth_day.find(yearmonth);
	if (mit != yearmonth_day.end())
	{
		return  mit->second.test(this_day - 1);
	}
	else
	{
		return false;
	}

}

AdGroupPtr AdPool::getAdGroup(long id)
{
	IceUtil::RWRecMutex::RLock lock(lock_);

	__gnu_cxx::hash_map<long, AdGroupPtr>::iterator it = group_pool_.find(id);
	if (it != group_pool_.end())
	{
		return it->second;
	}

	return NULL;
}

AdCampaignPtr AdPool::getAdCampaign(long id)
{
	IceUtil::RWRecMutex::RLock lock(lock_);

  __gnu_cxx::hash_map<long, AdCampaignPtr>::const_iterator it = campaign_pool_.find(id);
	if (it != campaign_pool_.end()) 
	{
		return  it->second;
	} 

	return NULL;
}

vector<Ice::Long> AdPool::getAdCampaignIDList()
{
	IceUtil::RWRecMutex::RLock lock(lock_);

	vector<Ice::Long> id_vec;
  __gnu_cxx::hash_map<long, AdCampaignPtr>::const_iterator it = campaign_pool_.begin();
	for (; it != campaign_pool_.end(); ++it)
	{
		id_vec.push_back(it->first);
	}

	return id_vec;
}

vector<Ice::Long> AdPool::getAdMemberIDList()
{
	IceUtil::RWRecMutex::RLock lock(lock_);
	
	vector<Ice::Long> id_vec;
  __gnu_cxx::hash_map<long, AdMemberPtr>::const_iterator it = member_pool_.begin();
	for (; it != member_pool_.end(); ++it)
	{
		id_vec.push_back(it->first);
	}       
	return id_vec;
}

AdMemberPtr AdPool::getAdMember(long id)
{
	IceUtil::RWRecMutex::RLock lock(lock_);

  __gnu_cxx::hash_map<Ice::Long, AdMemberPtr>::iterator it = member_pool_.find(id);
	if (it != member_pool_.end()) 
	{
		return it->second;
	} 

	return NULL;
}

void AdPool::AddAdGroup(AdGroupPtr group){
  __gnu_cxx::hash_map<long, AdGroupPtr>::iterator it = group_pool_.find(group->group_id_); 
  if(it != group_pool_.end()){
    group_pool_.erase(it);
  }
  group_pool_.insert(make_pair(group->group_id_, group));
}

int AdPool::load()
{
	int ret = 0;

//	MCE_DEBUG("BEGIN AdPool::loadAdMembers");
	ret = loadAdMembers();
//	MCE_DEBUG("BEGIN AdPool::loadAdMembers");
	if(ret < 0)
		return ret;

//	MCE_DEBUG("BEGIN AdPool::loadAdCampaigns");
	ret = loadAdCampaigns();
//	MCE_DEBUG("BEGIN AdPool::loadAdCampaigns");
	if(ret < 0)
		return ret;

	
//	MCE_DEBUG("BEGIN AdPool::loadAdGroup");
	ret = loadAdGroups();
//	MCE_DEBUG("BEGIN AdPool::loadAdGroup");
	if(ret < 0)
		return ret;

	return 0;
}

int AdPool::reload()
{
	MCE_INFO("-------------AdPool begin reload-------------");
	load();
	MCE_INFO("-------------AdPool end reload-------------");
	return 0;
}

int AdPool::loadAdGroups()
{


	__gnu_cxx::hash_map<long, AdGroupPtr> pool;
	int ret = DBSelector::instance().loadAdGroups(pool);
	if(ret < 0)
	{
		MCE_WARN("AdGroupPool::load failed");
		return -1;
	}

	{
		IceUtil::RWRecMutex::WLock wlock(lock_);
		group_pool_.swap(pool);
	}

	ret = DBSelector::instance().loadAdCreatives(group_pool_);
	if(ret < 0)
	{
		MCE_WARN("AdGroupPool::load loadAdCreatives failed");
		return -1;
	}

	ret = DBSelector::instance().loadAdZoneRef(group_pool_);
	if(ret < 0)
	{
		MCE_WARN("AdGroupPool::load loadAdZoneRef failed");
		return -1;
	}

	return 0;
}

int AdPool::loadAdCampaigns()
{
  __gnu_cxx::hash_map<long, AdCampaignPtr> pool;
	int ret = DBSelector::instance().loadAdCampaigns(pool);
	if(ret < 0)
	{
		MCE_WARN("AdCampaignPool::load failed");
		return -1;
	}

	IceUtil::RWRecMutex::WLock wlock(lock_);
	campaign_pool_.swap(pool);
	return 0;
}

int AdPool::loadAdMembers()
{
  __gnu_cxx::hash_map<long, AdMemberPtr> pool;
	int ret = DBSelector::instance().loadAdMembers(pool);
	if(ret < 0)
	{
		MCE_WARN("AdMemberPool::load failed");
		return -1;
	}

	IceUtil::RWRecMutex::WLock wlock(lock_);
	member_pool_.swap(pool);
	return 0;
}

void AdPool::UpdatePool(__gnu_cxx::hash_map<long, AdGroupPtr>& adgroup,
						__gnu_cxx::hash_map<long, AdCampaignPtr>& campaign_pool,
						__gnu_cxx::hash_map<long, AdMemberPtr>& member_pool){
		IceUtil::RWRecMutex::WLock wlock(lock_);
		group_pool_.swap(adgroup);
		campaign_pool_.swap(campaign_pool);
		member_pool_.swap(member_pool);
}

void IndexPoolTimer::process()
{
	AdPool *adPool = (AdPool*)instance_;
	if(adPool == NULL)
	{
		MCE_WARN("IndexPoolTimer::process get a NULL adPool");
		return ;
	}
	adPool->reload();
}

int IndexPoolTimer::interval() const
{
	return 60;
}
