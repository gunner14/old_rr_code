#include "recommendation_helper.h"

#include <boost/foreach.hpp>

namespace xce {
namespace socialgraph {

void RecommendationHelper::AddExperimentTag(const std::string& exp, RecommendItemSeq& item_list) {
	BOOST_FOREACH(RecommendItem& item, item_list) {
		item.experiment = exp;		//add tag
	}	
}

}}
