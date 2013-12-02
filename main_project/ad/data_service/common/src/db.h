/*
 * =====================================================================================
 *
 *       Filename:  db.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年04月18日 09时42分22秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef __INDEX_SERVICE_DB_H__
#define __INDEX_SERVICE_DB_H__

#include <QueryRunner.h>
#include <DbDescriptor.h>
#include "index.h"
#include "Close.h"
#include "ad/indexService/TIndex/src/targetingIndex.h"
#include "sql.h"

namespace xce{
	namespace ad{

		class DBSelector : public MyUtil::Singleton<DBSelector>
		{
			public:
				DBSelector():config_db_source_("ad_st"){}
				~DBSelector();
				//int init();

        void setConfigDBSource(const std::string &config_db_source){
          config_db_source_ = config_db_source;
        }

				void setAdDBSource(const std::string &ad_db_source)
				{
					ad_db_source_ = ad_db_source;	
				}

				void outputDBConfig();

				int loadAdMembers(__gnu_cxx::hash_map<long, AdMemberPtr> &member_pool);
				int loadAdCampaigns(__gnu_cxx::hash_map<long, AdCampaignPtr> &campaign_pool);
				int loadAdGroups(__gnu_cxx::hash_map<long, AdGroupPtr> &group_pool);
				int loadAdCreatives(__gnu_cxx::hash_map<long, AdGroupPtr> &group_pool);
				int loadAdZoneRef(const __gnu_cxx::hash_map<long, AdGroupPtr> &group_pool);
				int loadBidUnits(std::map<long, TargetingIndex::FeatureIndexPtr> &zone_index);
				int loadTriggerConfig(std::map<std::string, int > &triggerInfo); 
				int loadAdNumConfig(std::map<long, std::pair<unsigned, unsigned> > &adzone_ad_num); 
        int loadCommonConfig(std::map<std::string, std::string> &para_map);
        int LoadClosedAd(__gnu_cxx::hash_map<int, std::set<CloseItem> > &closed_ad_pool, int days, long &max_close_id);
        int ReloadClosedAd(std::map<int, std::list<CloseItem> > &closed_items, long &max_close_id);
        int InsertClosedAd(const CloseItem &item);

			private:
				int excuteQuery(const com::xiaonei::xce::Statement &sql, 
						const std::string &table,
						mysqlpp::StoreQueryResult & res,
            const std::string &db_source);

				mysqlpp::Connection conn_;

				std::string ad_db_source_;
        std::string config_db_source_;

		};

	}
}

#endif

