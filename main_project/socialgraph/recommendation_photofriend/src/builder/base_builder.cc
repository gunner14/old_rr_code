#include "base_builder.h"

namespace xce {
namespace socialgraph {

RecommendItemSeq BaseBuilder::GetRecommendData(int userid, int limit) {
	boost::mutex::scoped_lock alock(mutex_);		//lock local circle

	userid_ = userid;
	limit_ = limit;

	Preload();						//preload config or data needed later
	RecommendItemSeq item_list = Prepare();					//get raw data from source
	Fliter(item_list);							//fliter bad case in raw data
	Rank(item_list);							//rank item list
	Explanation(item_list);				//explanation of item

	return item_list;
}

}}
