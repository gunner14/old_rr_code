#ifndef _FRIENDRANK_CACHE_NEW_ADAPTER_H_
#define _FRIENDRANK_CACHE_NEW_ADAPTER_H_

#include <IceUtil/RWRecMutex.h>
#include "TopicI.h"
#include "AdapterI.h"
#include "Singleton.h"
#include "TaskManager.h"
#include "FriendRankNew.h"

namespace xce {
	namespace socialgraph {

/*		class  RSscore{
		public:
  			int fid;
  			float staticScore;
  			float activeScore;
  			RSscore& operator=(const RSscore& s)
  			{
    				this->activeScore = s.activeScore;
    				this->fid = s.fid;
    				this->staticScore = s.staticScore;
    				return *this; 
  			}
		};*/
		typedef RankDataNew RSscore;
		//class FriendRankCacheAdapter: virtual public MyUtil::AdapterI, 
		//	virtual public MyUtil::AdapterISingleton<MyUtil::OceChannel, FriendRankCacheAdapter>
		// HA化的服务的Adapter，须继承自ReplicatedClusterAdapterI
		class FriendRankCacheAdapter: 
			virtual public MyUtil::ReplicatedClusterAdapterI<FriendRankCacheNewPrx>,
			virtual public MyUtil::Singleton<FriendRankCacheAdapter>
		{
			public:
				FriendRankCacheAdapter() :
					MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::FriendRankCacheNewPrx>(
							"ControllerFriendRankCacheNew", 120, 1000, new MyUtil::SocialGraphChannel()) {
						_managers.resize(cluster() > 0 ? cluster() : 1);
						//MCE_INIT("./log", "INFO");
				//		MCE_INFO("i am here. test.");
					}
				virtual ~FriendRankCacheAdapter();

				RankSeqIntNew GetRankWithFactor(int userId, float factor);
				RankSeqNew GetRank(int userId);
				RankMapNew GetRevRank(int userId);
				void SetRank(int userId, const FriendRankDataNewPtr& list);
				void Load(const map<int,vector<RSscore> >& rankdatas);
			protected:
				bool isValid(const Ice::ObjectPrx& proxy);
				friend class MyUtil::Singleton<FriendRankCacheAdapter>;

			private:
				vector<FriendRankCacheNewPrx> _managers;

				virtual string name()
				{
					return "FriendRankCacheNew";
				}
				virtual string endpoints()
				{
					return "@FriendRankCacheNew";
				}
				virtual size_t cluster()
				{
					return 10;  // 部署服务的份数，必须对应，因为服务命名规则使用了这个数字
				}
		};
	}
}

#endif

