#ifndef RECOMMENDATION_HELPER_H_
#define RECOMMENDATION_HELPER_H_

#include "RecommendationManager.h"

namespace xce {
namespace socialgraph {
/*
@brief RecommendationHelper
*/
class RecommendationHelper {
 public:
	/*
	@brief Add tag of experiment into recommenditem list
	@param exp tag of experiment
	@param item_list result of recommendation
	*/
	static void AddExperimentTag(const std::string& exp, RecommendItemSeq& item_list);

 protected:

 private:

};

}}

#endif
