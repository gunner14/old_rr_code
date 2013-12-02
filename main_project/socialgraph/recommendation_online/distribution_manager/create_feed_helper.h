#ifndef CREATE_FEED_HELPER_H_
#define CREATE_FEED_HELPER_H_

#include <iostream>

#include "UtilCxx/src/Singleton.h"
#include "RecommendationManager.h"
#include "album_util.h"

namespace xce {
namespace socialgraph {

class CreateFeedHelper {
 public:
	static bool CreatePageFeedRecommendation(int userid, const RecommendItemSeq& item_seq);	
	static void Print(const std::map<std::string, std::string>& source);
};

}
}

#endif
