#include "feed_recommend_builder.h"

#include "commondbhelper.h"
#include "item.pb.h"     
#include "socialgraph/socialgraphutil/mc_adapter.h"

#include "Singleton.h"
#include "tripod2/cc/core/tripod_client.h"
#include <boost/lexical_cast.hpp>
#include "OceCxxAdapter/src/SocialGraphRcdTripodAdapter.h"

namespace xce {
namespace socialgraph {

using namespace xce::tripod;
void FeedRecommendBuilder::Preload() {
	//to-do	
	//prepare common data
}

RecommendItemSeq FeedRecommendBuilder::Prepare() {
	return RecommendItemSeq();
}

void FeedRecommendBuilder::Fliter(RecommendItemSeq& item_list) {
	//to-do
	//fliter bad case
}

void FeedRecommendBuilder::Rank(RecommendItemSeq& item_list) {
	////random recommenditem list

}

void FeedRecommendBuilder::Explanation(RecommendItemSeq& item_list) {
	//to-do
	//add explanation
}

}}
