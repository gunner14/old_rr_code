#include "SortByClusterAdapter.h"

namespace xce {
namespace socialgraph {

MyUtil::IntSeq SortByClusterAdapter::GetRecommendByCluster(int userid, MyUtil::IntSeq idlist, int limit) {
	MyUtil::IntSeq result_list;
  try {
    result_list = getProxy(userid)->GetRecommendByCluster(userid, idlist, limit);
  } catch (...) {
    //Nothing to do
  }
	
	return result_list;
}

MyUtil::IntSeq SortByClusterAdapter::GetFinalRecommendByCluster(int userid, MyUtil::IntSeq idlist, int limit) {
	MyUtil::IntSeq result_list;
  try {
    result_list = getProxy(userid)->GetFinalRecommendByCluster(userid, idlist, limit);
  } catch (...) {
    //Nothing to do
  }
	
	return result_list;
}

}
}
