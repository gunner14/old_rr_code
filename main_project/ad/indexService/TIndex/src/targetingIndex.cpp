#include "targetingIndex.h"
#include "ad/indexService/common/src/db.h"
#include "ad/indexService/common/src/indexConfig.h"
#include <boost/algorithm/string/regex.hpp>
#include "ad/Degrade/src/Degrade.h"
#include <algorithm>
#include <iterator>
using namespace xce::ad;

int TargetingIndex::loadBidUnits()
{
	std::map<long, FeatureIndexPtr> zone_index;
		

	int ret = DBSelector::instance().loadBidUnits(zone_index);
	if(ret < 0)
	{
		MCE_WARN("TargetingIndex::loadBidUnits FAILED");
		return -1;
	}

	IceUtil::RWRecMutex::WLock wlock(lock_);
	zone_index_.swap(zone_index);
	//MCE_DEBUG("zone_index_: " << &zone_index_ << ", TargetingIndex: " << this);

	return 0;
}

int TargetingIndex::load()
{
	return loadBidUnits();
}

int TargetingIndex::reload()
{
	MCE_INFO("-------------TargetingIndex begin reload-------------");
	load();
	MCE_INFO("-------------TargetingIndex end reload-------------");
	return 0;
}

int TargetingIndex::init(const std::string &name, int type)
{
	name_ = name;
	type_ = type;
	return 0;
}

template <typename T>
void trigger_output(const std::string &name, const T &con)
{
	ostringstream s;
	s << name << " :";

	for(typename T::const_iterator it = con.begin(); it != con.end(); ++it)
	{
		AdGroupPtr g = *it;
		s << g->group_id_ << ",";
	}

	MCE_DEBUG(s.str());
}



int TargetingIndex::trigger(const TriggerParameter & input, long zone_id, std::set<AdGroupPtr> &result_set)
{
  const int STAGE_WORK = 4;
  const uint64_t OTHER_MD5 = 0xba7790b1708b71cb;
	std::vector<AdGroupPtr> result;
	int user_stage = input.stage_; 

	FeatureIndexPtr fidx = NULL;
	{
		IceUtil::RWRecMutex::RLock lock(lock_);
		map<long, FeatureIndexPtr>::const_iterator it = zone_index_.find(zone_id);
		if (it != zone_index_.end()) {
			fidx = it->second;
		}
	}

	if (!fidx) 
	{
		//MCE_WARN("TargetingIndex::trigger failed, cannot find adzone: " << zone_id);
		return -1;
	}

	try
	{
		std::vector<short> genders;
		genders.push_back(input.gender_);
		std::set<AdGroupPtr> gender_set; 
		fidx->gender_index_->get(genders, gender_set);
		//MCE_INFO("trigger gender_set size: " << gender_set.size());
		//trigger_output("gender", gender_set);

		std::vector<short> stages;
		stages.push_back(user_stage);
		std::set<AdGroupPtr> stage_set;
		fidx->stage_index_->get(stages, stage_set);
		//MCE_INFO("trigger stage_set size: " << stage_set.size());
		//trigger_output("stage", stage_set);

		result.resize(min(gender_set.size(), stage_set.size()), NULL);

		std::vector<AdGroupPtr>::iterator it = std::set_intersection(gender_set.begin(), 
				gender_set.end(), stage_set.begin(), stage_set.end(), result.begin());
		result_set.insert(result.begin(), it);


		std::vector<short> ages;
		ages.push_back(input.age_);
		ages.push_back(-1);
		std::set<AdGroupPtr> age_set;
		fidx->age_index_->get(ages, age_set);
		//MCE_INFO("trigger age_set size: " << age_set.size());
		//trigger_output("age", age_set);
		it = std::set_intersection(result_set.begin(), result_set.end(), age_set.begin(), age_set.end(), result.begin());

		result_set.clear();
		result_set.insert(result.begin(), it);
		
		if (input.useNew_ == true){
			std::set<AdGroupPtr>  companyresults;  
			std::vector<uint64_t>  companys;
			companys.push_back(OTHER_MD5); 

			std::copy(input.companys_.begin(), input.companys_.end(), std::back_inserter(companys));
			
		  ostringstream  tempout1;
                        tempout1 << "uid = " << input.uid_;
			tempout1 << "zone id = " << zone_id;
			tempout1 << "company used for trigger:"<< hex;
			copy(companys.begin(), companys.end(), ostream_iterator<uint64_t>(tempout1, ","));
			MCE_DEBUG(tempout1.str());
		
//			fidx->company_index_->output();	
			fidx->company_index_->get(companys, companyresults);   
			
//			MCE_INFO("trigger company_set size: " << companyresults.size());
//		  trigger_output("company", companyresults);
			//std::set_intersection(result_set.begin(), result_set.end(), companyresults.begin(), companyresults.end(), back_inserter(result));  
			it = std::set_intersection(result_set.begin(), result_set.end(), companyresults.begin(), companyresults.end(), result.begin());  
			result_set.clear();     
			//result_set.insert(result.begin(), result.end());
			result_set.insert(result.begin(), it);
		

                       	std::set<AdGroupPtr>  interestresults;  
			std::vector<uint64_t>  interests;
                        std::set<uint64_t> interests_cache;
			interests.push_back(OTHER_MD5); 
                        if(DegradeManager::instance().IsEnable("interest") && DegradeManager::instance().IsGray(input.uid_%100)){
                        bool bret = lru_cache_.GetData(input.uid_, interests_cache);
                        if(bret == false){
                          lru_cache_.UpdateData(input.uid_);
                        }
			//companys.push_back(OTHER_MD5); 
			/*vector<string>  temp;
			boost::algorithm::split_regex(temp, input.companys_, boost::regex(","));
			for (vector<string>::iterator it = temp.begin(); it != temp.end(); ++it){
				char*  chTemp = NULL;
				companys.push_back(strtoul(it->c_str(), &chTemp, 16));
				MCE_WARN("user's companys:" << strtoul(it->c_str(), &chTemp, 16));
			}*/
			std::copy(interests_cache.begin(), interests_cache.end(), std::back_inserter(interests));
                        } else {
                        }
			
		        ostringstream  tempout2;
                        tempout2 << "uid = " << input.uid_;
			tempout2 << "zone id = " << zone_id;
			tempout2 << "interest used for trigger:"<< hex;
			copy(interests.begin(), interests.end(), ostream_iterator<uint64_t>(tempout2, ","));
			MCE_DEBUG(tempout2.str());
		
//			fidx->interest_index_->output();	
			fidx->interest_index_->get(interests, interestresults);   
			
//			MCE_INFO("trigger interest_set size: " << interestresults.size());
//		        trigger_output("interest", interestresults);
			//std::set_intersection(result_set.begin(), result_set.end(), interestresults.begin(), interestresults.end(), back_inserter(result));  
			it = std::set_intersection(result_set.begin(), result_set.end(), interestresults.begin(), interestresults.end(), result.begin());  
			result_set.clear();     
			//result_set.insert(result.begin(), result.end());
			result_set.insert(result.begin(), it);
		        //trigger_output("interest and company", result_set);
                        
		}
   
		if (user_stage == STAGE_WORK) 
		{
			uint64_t ip_area = boost::lexical_cast<uint64_t> (input.ipArea_);
			std::set<AdGroupPtr> ip_area_set; 
			fidx->area_index_->get(ip_area, ip_area_set);
			//MCE_INFO("trigger ip_area_set size: " << ip_area_set.size());
			//trigger_output("ip_area", ip_area_set);
			it = std::set_intersection(result_set.begin(), result_set.end(), 
					ip_area_set.begin(), ip_area_set.end(), result.begin());
			result_set.clear();
			result_set.insert(result.begin(), it);
		} 
		else 
		{
			std::vector<short> grades;
			grades.push_back(input.grade_);
			grades.push_back(-1);
			std::set<AdGroupPtr> grade_set;  
			fidx->grade_index_->get(grades, grade_set);
			//MCE_INFO("trigger grade_set size: " << grade_set.size());
			//trigger_output("grade", grade_set);
			it = std::set_intersection(result_set.begin(), result_set.end(), grade_set.begin(), grade_set.end(), result.begin());
			result_set.clear();
			result_set.insert(result.begin(), it);

			uint64_t school_id = boost::lexical_cast<uint64_t>(input.school_);
			std::vector<uint64_t> school_ids;
			school_ids.push_back(school_id);
			school_ids.push_back(0);
			std::set<AdGroupPtr> school_set; 
			fidx->school_index_->get(school_ids, school_set);
			//trigger_output("school", school_set);
			//MCE_INFO("trigger school_set size: " << school_set.size());

			uint64_t school_scope = boost::lexical_cast<uint64_t> (input.currentArea_);
			std::set<AdGroupPtr> school_area_set; 
			fidx->school_area_index_->get(school_scope, school_area_set);
			//MCE_INFO("trigger school_area_set size: " << school_area_set.size());
			//trigger_output("school_area", school_area_set);


			uint64_t ip_area = boost::lexical_cast<uint64_t> (input.ipArea_);
			std::set<AdGroupPtr> ip_area_set; 
			fidx->area_index_->get(ip_area, ip_area_set);
			//MCE_INFO("trigger ip_area_set size: " << ip_area_set.size());
			//trigger_output("ip_area2", ip_area_set);

			school_set.insert(school_area_set.begin(), school_area_set.end());
			school_set.insert(ip_area_set.begin(), ip_area_set.end());

			it = std::set_intersection(result_set.begin(), result_set.end(), school_set.begin(), school_set.end(), result.begin());
			result_set.clear();
			result_set.insert(result.begin(), it);
		}
	}
	catch (std::exception e)
	{
		MCE_WARN("TargetingIndex::trigger catch exception: " << e.what());
		return -1;
	}
	catch (...)
	{
		MCE_WARN("TargetingIndex::trigger catch unkwown exception");
		return -1;
	}

  /*
	if (result_set.empty()) 
	{
		MCE_DEBUG("trigger Get no ad: uid=" << input.uid << " gender=" << input.gender << " age=" << input.age << " stage=" 
				<< input.stage << " school=" << input.school <<  " current_area=" 
				<< input.currentArea << " ip_area=" << input.ipArea);
	}
	else
	{
		MCE_DEBUG("trigger result_set has " << result_set.size() << " groups");
		for(std::set<AdGroupPtr>::iterator it = result_set.begin(); it != result_set.end(); ++it)
		{
			MCE_DEBUG("result_set has: " << (*it)->group_id_ << ",uid: " << input.uid);
		}
	}
  */

	return 0;

}

/*
 * Func:TargetingIndex::PrintAll
 * Desc:打印所有广告位上的索引
 */
string TargetingIndex::PrintAll() {
  IceUtil::RWRecMutex::RLock lock(lock_);
  ostringstream os;
  map<long, FeatureIndexPtr>::iterator it = zone_index_.begin();
  os << "[";
  for (; it != zone_index_.end(); ++it) {
    os << Print(it->first);
    os << ",";
  }
  os << "]";
  return os.str();
}

/*
 * Func:TargetingIndex::Print
 * Desc:特定广告位上的信息
 */
string TargetingIndex::Print(long zoneId) {
  IceUtil::RWRecMutex::RLock lock(lock_);
  ostringstream os;
  map<long, FeatureIndexPtr>::iterator it = zone_index_.find(zoneId);
  if (it != zone_index_.end()) {
    os << "{\"adzoneId\":" << it->first << ",";
    os << "\"GenderIndex\":";
    os << it->second->gender_index_->GetInfo() << ",";
    os << "\"StageIndex\":";
    os << it->second->stage_index_->GetInfo() << ",";
    os << "\"GradeIndex\":";
    os << it->second->grade_index_->GetInfo() << ",";
    os << "\"AgeIndex\":";
    os << it->second->age_index_->GetInfo() << ",";
    os << "\"AreaIndex\":";
    os << it->second->area_index_->GetInfo() << ",";
    os << "\"SchoolIndex\":";
    os << it->second->school_index_->GetInfo() << ",";
    os << "\"SchoolAreaIndex\":";
    os << it->second->school_area_index_->GetInfo() << ",";
    os << "\"CompanyIndex\":";
    os << it->second->company_index_->GetInfo() << ",";
    os << "\"InterestIndex\":";
    os << it->second->interest_index_->GetInfo() << "}";
  }
  return os.str();
}
 
void TargetingTimer::process()
{
	TargetingIndex *tidx = (TargetingIndex*)instance_;
	if(tidx == NULL)
	{
		MCE_WARN("TargetingTimer::process get a NULL tidx");
		return ;
	}
	tidx->reload();
}
int TargetingTimer::interval() const
{
	return 60;
}

