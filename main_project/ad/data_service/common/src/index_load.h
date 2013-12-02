/*
 * =====================================================================================
 *
 *       Filename:  index_load.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/20/12 17:21:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "ad/indexService/common/src/index.h"
#include "ad/indexService/common/src/IndexLoader.h"
#include "IndexItem.h"

namespace xce{
namespace ad{
	
struct BidUnit{
	long bid_id_;
	long group_id_;
	int gender_;
	int stage_;	
	string grade_;
	string age_;
	string school_;
	string area_;
	string company_;
	string interest_; 
};

class IndexServiceLoader : public IndexLoader{
	public:
		IndexServiceLoader(){}
		IndexServiceLoader(__gnu_cxx::hash_map<long, AdGroupPtr>& group_pool,
				__gnu_cxx::hash_map<long, AdCampaignPtr>& campaign_pool,
				__gnu_cxx::hash_map<long, AdMemberPtr>& member_pool):
			group_pool_(group_pool), campaign_pool_(campaign_pool), member_pool_(member_pool){
			}

		virtual int LoadBase(const std::string &table, const std::vector<IndexItem> &base);
		virtual int LoadInc(const std::vector<IndexItem> &inc);
		virtual void UpdateData();
		virtual void CreateIndex();
		virtual ~IndexServiceLoader(){}


		void LoadAdMembers(const std::vector<IndexItem> &items);
		void LoadAdCampaigns(const std::vector<IndexItem> &items);
		void LoadAdGroups(const std::vector<IndexItem> &items);
		void LoadAdCreatives(const std::vector<IndexItem> &items);
		void LoadAdZoneRef(const std::vector<IndexItem> &items);
		void LoadBidUnits(const std::vector<IndexItem> &items);

		void LoadIncItemToMember(const IndexItem & item);
		void LoadIncItemToGroups(const IndexItem & item);
		void LoadIncItemToCampain(const IndexItem & item);
		void LoadIncItemToCreatives(const IndexItem & item);
		void LoadIncItemToBidUnit(const IndexItem & item);
		void LoadIncItemToZone(const IndexItem & item);

	private:
		void ShowVectorContent(const std::string& title,const IndexItem& item,int type);

	private:
		__gnu_cxx::hash_map<long, AdGroupPtr> group_pool_;
		__gnu_cxx::hash_map<long, AdCampaignPtr> campaign_pool_;
		__gnu_cxx::hash_map<long, AdMemberPtr> member_pool_;
		__gnu_cxx::hash_map<long, BidUnit> bidunit_pool_;
		bool bidunit_changed_;
		IceUtil::RWRecMutex lock_;
};
}
}
