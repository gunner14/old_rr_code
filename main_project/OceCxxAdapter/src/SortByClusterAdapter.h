#ifndef _SORT_BY_CLUSTER_ADAPTER_H_
#define _SORT_BY_CLUSTER_ADAPTER_H_

#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"
#include "SortByCluster.h"

namespace xce {
namespace socialgraph {

class SortByClusterAdapter : public MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::SortByClusterPrx>, 
		public MyUtil::Singleton<SortByClusterAdapter> {
 public:
  SortByClusterAdapter() :
		MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::SortByClusterPrx>("ControllerSortByCluster", 120, 1000, 
																																						new MyUtil::SocialGraphChannel()) { }
  
		MyUtil::IntSeq GetRecommendByCluster(int userid, MyUtil::IntSeq idlist, int limit); 
		MyUtil::IntSeq GetFinalRecommendByCluster(int userid, MyUtil::IntSeq idlist, int limit); 
};


}}

#endif

