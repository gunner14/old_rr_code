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

#ifndef __INDEX_SERVICE_INDEX_H__
#define __INDEX_SERVICE_INDEX_H__

#include <set>
#include <string>
#include <map>
#include <ext/hash_map>
#include "IndexService.h"

#include <IceUtil/RWRecMutex.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>
#include <Ice/Config.h>

#include <QueryRunner.h>
#include <DbDescriptor.h>

#include "indexTimer.h"

namespace xce{
	namespace ad{

		struct AdGroup :public IceUtil::Shared
		{
      AdGroup():creative_id_(0),bid_unit_id_(0),widget_id_(0),trans_type_(0),member_category_(0),member_industry_(0){}

			bool operator >(const AdGroup& group) const 
			{
				return group_id_ > group.group_id_;
			}
			bool operator ==(const AdGroup& group) const 
			{
				return group_id_ == group.group_id_;
			}
			bool operator <(const AdGroup& group) const 
			{
				return group_id_ < group.group_id_;
			}

			long group_id_;
			long member_id_;
			long campaign_id_;
			long creative_id_;
			long bid_unit_id_;
			long widget_id_;
			int trans_type_;
			int member_category_;
			int member_industry_;
			std::map<long,double> zone_price_;
		};

		struct AdCampaign :public IceUtil::Shared
		{

			long campaign_id_;
			long member_id_;
		};

		struct AdMember:public IceUtil::Shared
		{
			int category_;
      int industry_;
			long member_id_;
		};

		typedef IceUtil::Handle<AdGroup> AdGroupPtr;
		typedef IceUtil::Handle<AdCampaign> AdCampaignPtr;
		typedef IceUtil::Handle<AdMember> AdMemberPtr;
    
		class AdPool : public MyUtil::Singleton<AdPool>
		{
			public:

				int load();
				int reload();

				AdGroupPtr getAdGroup(long id);
				AdCampaignPtr getAdCampaign(long id);
				AdMemberPtr getAdMember(long id);
 				
				std::vector<Ice::Long> getAdMemberIDList();
				std::vector<Ice::Long> getAdCampaignIDList();
        string Print(const int& type);
        string PrintAllZonePrice();
        string PrintZonePrice(long group_id);
			private:
				int loadAdGroups();
				int loadAdCampaigns();
				int loadAdMembers();
        enum PRINTTYPE{MEMBERTYPE, CAMTYPE, GROUPTYPE};
				__gnu_cxx::hash_map<long, AdGroupPtr> group_pool_;
				std::map<long, AdCampaignPtr> campaign_pool_;
				std::map<long, AdMemberPtr> member_pool_;
				IceUtil::RWRecMutex lock_;
		};

		class IndexPoolTimer : public IndexTimer
		{
			public:
				IndexPoolTimer(void *instance): IndexTimer(instance){}
				virtual void process();
				virtual int interval() const;
		};

		/*  
		class AdGroupPool : public MyUtil::Singleton<AdGroupPool>
		{
			public:
				int init();
				int load();
				int reload();
				AdGroupPtr getAdGroup(long id);

			private:	
				__gnu_cxx::hash_map<long, AdGroupPtr> group_pool_;
				IceUtil::RWRecMutex lock_;

		};

		class AdCampaignPool : public MyUtil::Singleton<AdCampaignPool>
		{
			public:
				int init();
				int load();
				int reload();
				AdCampaignPtr getAdCampaign(long id);

			private:	
				std::map<long, AdCampaignPtr> campaign_pool_;
				IceUtil::RWRecMutex lock_;

		};

		class AdMemberPool : public MyUtil::Singleton<AdMemberPool>
		{
			public:
				int init();
				int load();
				int reload();
				AdMemberPtr getAdMember(long id);

			private:	
				std::map<long, AdMemberPtr> member_pool_;
				IceUtil::RWRecMutex lock_;

		};
		*/

		class PointsChecker : public MyUtil::Singleton<PointsChecker> 
		{
			public:
				bool check(const std::string &days, const std::string & hours);
			private:
				bool checkHour(const std::string &days);
				bool checkDay(const std::string &hours);
		};


    struct TriggerParameter{
      short age_;
      short gender_;
      short stage_;
      short grade_;
      int uid_;
      string school_;
      string ipArea_;
      string currentArea_;
      vector<uint64_t> companys_;
      vector<long> zones_;
      bool  useNew_;   //过度变量，兼容老版本
    };


		class Index :public IceUtil::Shared
		{
			public:

				Index(){}

				virtual int init(const std::string &name, int type) = 0;
				virtual int load() = 0;
				virtual int reload() = 0;
				virtual int trigger(const TriggerParameter &input, long zone_id, std::set<AdGroupPtr> &result_set) = 0;
    
				std::string getName()
				{
					return name_;
				}
				int getType()
				{
					return type_;
				}
			protected:
				int type_;
				std::string name_;

		};

		typedef IceUtil::Handle<Index> IndexPtr;

	}
}

#endif

