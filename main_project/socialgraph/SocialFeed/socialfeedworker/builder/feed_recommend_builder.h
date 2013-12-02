#ifndef  FEED_RECOMMEND_BUILDER_SF_H_
#define  FEED_RECOMMEND_BUILDER_SF_H_

#include <boost/shared_ptr.hpp>

#include "SocialFeedRecommendationManager.h"

#include "base_builder.h"
#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace socialgraph {

typedef   std::vector<unsigned char> BinaryData;

//---------------------------------------
/*
@brief specific builder inheriting from base_builder
*/
class FeedRecommendBuilder :  public BaseBuilder, public MyUtil::Singleton<FeedRecommendBuilder> {
 public:
	FeedRecommendBuilder() { }
	~FeedRecommendBuilder() { }	

	void Preload();
	RecommendItemSeq Prepare();
	void Fliter(RecommendItemSeq& item_list);
	void Rank(RecommendItemSeq& item_list);
	void Explanation(RecommendItemSeq& item_list);

 protected:

 private:
	RecommendItemSeq  GetFeedRecommend();
        // the strategy of how many feeds to send
        int   SendCountStrategy(int time_stamp, int count_remain);
};

typedef boost::shared_ptr<FeedRecommendBuilder> FeedRecommendBuilderPtr;	//define FeedRecommendBuilderPtr 

}}

#endif
