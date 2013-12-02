#ifndef __SECOND_DEGREE_FRIENDSI_H__
#define __SECOND_DEGREE_FRIENDSI_H__

#include "IceExt/src/ServiceI.h"
#include "util/cpp/TimeCost.h"
#include "UtilCxx/src/TaskManager.h"
#include "DbCxxPool/src/QueryRunner.h"
#include <IceUtil/IceUtil.h>
namespace xce {
	namespace sdfriends {
		static const int BUCKET_SIZE = 2000;
		static const int STEP = 20;
		class SDFriend;
		typedef IceUtil::Handle<SDFriend> SDFriendPtr;

		struct CommonFriend {
			CommonFriend() {
				userId = 0;
				weight = 0;
				commonNum = 0;
			}

			int userId;
			int weight;
			int commonNum;
		};

		struct HeapItem {
			int hostId;
			int weight;
			std::vector<int>::const_iterator beginIter;
			std::vector<int>::const_iterator endIter;
		};

		struct IdAscComp {
			bool operator() (const HeapItem& lhs, const HeapItem& rhs) const {
				return *lhs.beginIter > *rhs.beginIter;
			}
		};

		class SecondDegreeData {
			public:

				MyUtil::Int2IntMap getSecondDegreeFriendsKMerge(Ice::Int userId, Ice::Int limit);
				void merge(const std::map<int, std::vector<int> >& friendOfFriendIds, int weight);
				void flushSortVector(std::vector<HeapItem>& sortVec);
				void insertBucket(const CommonFriend& comm);
				bool isBlock(int userid);
			private:
				vector<int> _blockSortList;
				std::vector<int>::iterator _findIter;
				std::vector<std::vector<CommonFriend> > _bucket;
		};
	}
}

#endif
