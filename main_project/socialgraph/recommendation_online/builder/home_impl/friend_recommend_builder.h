#ifndef FRIEND_RECOMMEND_BUILDER_H_
#define FRIEND_RECOMMEND_BUILDER_H_

#include <boost/shared_ptr.hpp>

#include "RecommendationManager.h"

#include "../base_builder.h"
#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace socialgraph {

typedef std::vector<unsigned char> BinaryData;

//---------------------------------------
/*
@brief specific builder inheriting from base_builder
*/
class FriendRecommendBuilder :  public BaseBuilder, public MyUtil::Singleton<FriendRecommendBuilder> {
 public:
	FriendRecommendBuilder() { }
	~FriendRecommendBuilder() { }	

	void Preload();
	RecommendItemSeq Prepare();
	void Fliter(RecommendItemSeq& item_list);
	void Rank(RecommendItemSeq& item_list);
	void Explanation(RecommendItemSeq& item_list);

 protected:

 private:
	/*
	@author zhangnan@renren-inc.com
	@brief Get friendsrecommend from rcd cache
	*/
	RecommendItemSeq GetRcdFriendRecommend();	
	RecommendItemSeq GetFFucrFriendRecommend();
	RecommendItemSeq GetFinalFriendRecommend();
	RecommendItemSeq GetFFucrHSUNFriendRecommend();
};

typedef boost::shared_ptr<FriendRecommendBuilder> FriendRecommendBuilderPtr;	//define FriendRecommendBuilderPtr 

}}

#endif
