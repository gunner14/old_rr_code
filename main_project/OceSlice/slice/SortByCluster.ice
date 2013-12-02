#ifndef _SORT_BY_CLUSTER_H_ 
#define _SORT_BY_CLUSTER_H_

#include <Util.ice>

module xce {
module socialgraph {

interface SortByCluster {
	MyUtil::IntSeq GetRecommendByCluster(int userid, MyUtil::IntSeq idlist, int limit);
	MyUtil::IntSeq GetFinalRecommendByCluster(int userid, MyUtil::IntSeq idlist, int limit);
};

};
};

#endif


