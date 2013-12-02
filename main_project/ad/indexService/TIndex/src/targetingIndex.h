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

#ifndef __INDEX_SERVICE_TARGETING_INDEX_H__
#define __INDEX_SERVICE_TARGETING_INDEX_H__

#include <set>
#include <vector>
#include <string>
#include <Ice/Config.h>
#include <IceUtil/Shared.h>
#include "LRUCache.h"
#include "ad/indexService/common/src/index.h"

namespace xce{
	namespace ad{

		template<typename T>
			class InnerIndex :public IceUtil::Shared
		{
			public:
				void  output(){ 
					ostringstream   sout; 
					sout << hex;
					for(typename std::map<T, std::set<AdGroupPtr> >::iterator it = index_.begin(); it != index_.end(); ++it){
						sout << hex << it->first << ":" << dec;    
						for (std::set<AdGroupPtr>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2){ 
							sout << (*it2)->group_id_ << ",";     
						}           
						sout << "\n";   
					}      
					MCE_DEBUG(sout.str());    
				}

				void add(const std::vector<T> &keys, AdGroupPtr value)
				{
					IceUtil::RWRecMutex::WLock wlock(lock_);

					for(typename std::vector<T>::const_iterator it = keys.begin(); it != keys.end(); ++it)
						index_[(*it)].insert(value);
				}

				void del(const std::vector<T> &keys, AdGroupPtr value)
				{
					IceUtil::RWRecMutex::WLock wlock(lock_);

					for(typename std::vector<T>::const_iterator it = keys.begin(); it != keys.end(); ++it)
					{
						index_[*it].erase(value);	
					}
				}

				void virtual get(const std::vector<T> &keys, std::set<AdGroupPtr> &result)
				{
					IceUtil::RWRecMutex::RLock rlock(lock_);

					for(typename std::vector<T>::const_iterator it = keys.begin(); it != keys.end(); ++it)
					{
						typename map<T, std::set<AdGroupPtr> >::const_iterator iter = index_.find(*it);	
						if(iter != index_.end())
						{
							const std::set<AdGroupPtr>& s = iter->second;
							result.insert(s.begin(), s.end());
						}
					}

				}

        string GetInfo() {
          ostringstream os;
          IceUtil::RWRecMutex::RLock rlock(lock_);
          typename map<T, std::set<AdGroupPtr> >::iterator it = index_.begin();
          os << "[";
          for (; it != index_.end(); ++it) {
            os << "\"" << it->first << ":";
            set<AdGroupPtr>::iterator it_g = it->second.begin();
            for (; it_g != it->second.end(); ++it_g) {
              os << (*it_g)->group_id_ << ",";
            }
            os << "\",";
          }
          os << "]";
          return os.str();
        }

			protected:
				std::map<T, std::set<AdGroupPtr> > index_;
				IceUtil::RWRecMutex lock_;
		};

		class AreaIndex: public InnerIndex<uint64_t>
		{
			public:
				virtual void get(const uint64_t &key, std::set<AdGroupPtr> &result)
				{
          /*
					for(std::map<uint64_t, std::set<AdGroupPtr> >::iterator iit = index_.begin(); iit != index_.end(); ++iit)
					{
						MCE_DEBUG("ipArea: " << iit->first << " has groups: ");
						for(std::set<AdGroupPtr>::iterator sit = iit->second.begin(); sit != iit->second.end(); ++sit)
						{
							AdGroupPtr g = *sit;
							MCE_DEBUG("group: " << g->group_id_);
						}
					}
          */

					uint64_t province = (key/10000000000)*10000000000;
					const uint64_t COUNTRY = 86000000000000;

					IceUtil::RWRecMutex::RLock rlock(lock_);

					std::map<uint64_t, std::set<AdGroupPtr> >::iterator it = index_.find(key);
					if(it != index_.end())
					{
						const std::set<AdGroupPtr>& s =it->second;
						result.insert(s.begin(), s.end());
					}

					it = index_.find(0);
					if(it != index_.end())
					{
						const std::set<AdGroupPtr>& s =it->second;
						result.insert(s.begin(), s.end());
					}


					if(province != key) 
					{
						it = index_.find(province);
						if(it != index_.end())
						{
							const std::set<AdGroupPtr>& s =it->second;
							result.insert(s.begin(), s.end());
						}
					}

					if(province != COUNTRY)
					{
						it = index_.find(COUNTRY);
						if(it != index_.end()) 
						{
							const std::set<AdGroupPtr>& s = it->second;
							result.insert(s.begin(), s.end());
						}
					}

				}
		};

		typedef InnerIndex<short> GenderIndex;
		typedef InnerIndex<short> AgeIndex;
		typedef InnerIndex<short> StageIndex;
		typedef InnerIndex<short> GradeIndex;

		typedef InnerIndex<uint64_t> SchoolIndex;
		typedef InnerIndex<uint64_t> SchoolAreaIndex;
    typedef InnerIndex<uint64_t> CompanyIndex;
                typedef InnerIndex<uint64_t> InterestIndex;  //add 2012.07.05

		typedef IceUtil::Handle<GenderIndex> GenderIndexPtr;
		typedef IceUtil::Handle<AgeIndex> AgeIndexPtr;
		typedef IceUtil::Handle<StageIndex> StageIndexPtr;
		typedef IceUtil::Handle<GradeIndex> GradeIndexPtr;
		typedef IceUtil::Handle<SchoolIndex> SchoolIndexPtr;
		//typedef IceUtil::Handle<SchoolAreaIndex> SchoolAreaIndexPtr;
		typedef IceUtil::Handle<AreaIndex> AreaIndexPtr;
    typedef IceUtil::Handle<CompanyIndex> CompanyIndexPtr;
                typedef IceUtil::Handle<InterestIndex> InterestIndexPtr;   //add 2012.07.05

		class TargetingIndex :public Index, public MyUtil::Singleton<TargetingIndex>
		{
			public:

				virtual int init(const std::string &name, int type);
				virtual int load();
				virtual int reload();

				virtual int trigger(const TriggerParameter &input, long zone_id,
						std::set<AdGroupPtr> &result_set);
 

				
				struct FeatureIndex: public IceUtil::Shared{
          FeatureIndex(){
					  gender_index_ = new GenderIndex;
					  stage_index_ = new StageIndex;
					  grade_index_ = new GradeIndex;
					  age_index_ = new AgeIndex;
					  area_index_ = new AreaIndex;
					  school_index_ = new SchoolIndex;
					  school_area_index_ = new AreaIndex;
            company_index_ = new CompanyIndex;
                                          interest_index_ = new InterestIndex;  //add 2012.07.05
          }

				  GenderIndexPtr gender_index_;
				  StageIndexPtr stage_index_;
				  GradeIndexPtr grade_index_;
				  AgeIndexPtr age_index_;
				  AreaIndexPtr area_index_;
				  SchoolIndexPtr school_index_;
				  AreaIndexPtr school_area_index_;
          CompanyIndexPtr company_index_;
                                  InterestIndexPtr interest_index_;      //add 2012.07.05
        };

				typedef IceUtil::Handle<FeatureIndex> FeatureIndexPtr; 
        string PrintAll();
        string Print(long zoneId);
			private:

				int parseConfigure(const std::string &filename);
				int loadBidUnits();
                                LRUCache lru_cache_;  //add 2012.07.05
				std::map<long, FeatureIndexPtr> zone_index_;
				IceUtil::RWRecMutex lock_;
		};

		class TargetingTimer : public IndexTimer
		{
			public:
				TargetingTimer(void *instance): IndexTimer(instance){}
				virtual void process();
				virtual int interval() const;
		};


	}
}
#endif

