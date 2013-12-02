#ifndef SORT_BY_CLUSTER_I_H_
#define SORT_BY_CLUSTER_I_H_

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include "SortByCluster.h"
#include "IceExt/src/ServiceI.h"

namespace xce {
namespace socialgraph {

class SortByClusterI: virtual public SortByCluster,
		virtual public MyUtil::Singleton<SortByClusterI> {
 public:
	virtual MyUtil::IntSeq GetRecommendByCluster(int userid, const MyUtil::IntSeq& idlist, 
																								int limit, const Ice::Current& current);
	virtual MyUtil::IntSeq GetFinalRecommendByCluster(int userid, const MyUtil::IntSeq& idlist, 
																								int limit, const Ice::Current& current);
  
};

}
}

#endif
