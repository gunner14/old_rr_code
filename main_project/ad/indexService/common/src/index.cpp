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
#include <string>
#include <iostream>
#include <algorithm>
#include "Date.h"
#include "index.h"
#include "db.h"
#include "indexConfig.h"

using namespace xce::ad;
using namespace std;
//typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

string AdPool::Print(const int& type) {
  IceUtil::RWRecMutex::RLock lock(lock_);
  std::ostringstream os;
  switch (type) {
    case MEMBERTYPE:
      {
        os << "[AdMember pool]:\n";
        std::map<long, AdMemberPtr>::iterator itm = member_pool_.begin();
        for(int i=1; itm != member_pool_.end(); ++itm,++i) {
          os << "[" << i << "]" << itm->first << ";";
          if ( i % 5 == 0 ) {
            os << "\n";
          }
        }
      }
      break;
    case CAMTYPE:
      {
        os << "[AdCampaign pool]:\n";
        std::map<long, AdCampaignPtr>::iterator itc = campaign_pool_.begin();
        for(int i=1; itc != campaign_pool_.end(); ++itc,++i) {
          os << "[" << i << "]" << itc->first << ";";
          if ( i % 5 == 0 ) {
            os << "\n";
          }
        }
      }
      break;
    case GROUPTYPE:
      {
        os << "[AdGroup pool]:\n";
        __gnu_cxx::hash_map<long, AdGroupPtr>::iterator itg = group_pool_.begin();      
        for(int i=1; itg != group_pool_.end(); ++itg,++i) {
          os << "[" << i << "]" << itg->first << ";";
          if ( i % 5 == 0 ) {
            os << "\n";
          }
        }
      }
      break;
    default:
      break;
  }
  return os.str();
}

/*
 * Func:AdPool::PrintAllZonePrice
 * Desc:所有广告组的出价
 */
string AdPool::PrintAllZonePrice() {
  IceUtil::RWRecMutex::RLock lock(lock_);
  ostringstream os;
  __gnu_cxx::hash_map<long, AdGroupPtr>::iterator it = group_pool_.begin();
  for(; it != group_pool_.end(); ++it) {
    os << "adgroup:" << it->first << "\n";
    os << PrintZonePrice(it->first) << "\n";
  }
  return os.str();
}

/*
 * Func:AdPool::PrintZonePrice
 * Desc:广告组在广告位上的出价
 */
string AdPool::PrintZonePrice(long group_id){
  ostringstream os;
  IceUtil::RWRecMutex::RLock lock(lock_);
  AdGroupPtr group_ptr = getAdGroup(group_id);
  if (!group_ptr) {
    return "";
  }
  map<long, double>::iterator it = group_ptr->zone_price_.begin();
  for (int i=1; it != group_ptr->zone_price_.end(); ++it, ++i) {
    os << "[" << i << "]" << it->first << ":" << it->second << ";";
    if (i%5 == 0) {
      os << "\n";
    }
  }
  return os.str();
}

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

	std::map<long, AdCampaignPtr>::const_iterator it = campaign_pool_.find(id);
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
	std::map<long, AdCampaignPtr>::const_iterator it = campaign_pool_.begin();
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
	std::map<long, AdMemberPtr>::const_iterator it = member_pool_.begin();
	for (; it != member_pool_.end(); ++it)
	{
		id_vec.push_back(it->first);
	}       
	return id_vec;
}

AdMemberPtr AdPool::getAdMember(long id)
{
	IceUtil::RWRecMutex::RLock lock(lock_);

	std::map<Ice::Long, AdMemberPtr>::iterator it = member_pool_.find(id);
	if (it != member_pool_.end()) 
	{
		return it->second;
	} 

	return NULL;
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
	std::map<long, AdCampaignPtr> pool;
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
	std::map<long, AdMemberPtr> pool;
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
