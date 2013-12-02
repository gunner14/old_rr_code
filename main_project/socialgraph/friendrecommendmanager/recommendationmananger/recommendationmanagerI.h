#ifndef RECOMMENDATION_MANAGER_H_ 
#define RECOMMENDATION_MANAGER_H_

#include <boost/thread/mutex.hpp>

#include "RecommendationManager.h"

#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace socialgraph {

const static std::string DEFAULT = "O";
const static std::string EXPERIMENTI = "E1";
const static std::string EXPERIMENTII = "E2";
const static std::string EXPERIMENTIII = "E3";

//---------------------------------------------------

class RecommendationManagerI: virtual public RecommendationManager,
		virtual public MyUtil::Singleton<RecommendationManagerI> {
 public:
	RecommendationManagerI() {}
	
	virtual RecommendItemSeq GetFriendRecommendByHome(int, int, const Ice::Current&);

 private:
	friend class MyUtil::Singleton<RecommendationManagerI>;

	std::string GetExperiment(int userid);
	//void PrepareAllExp();
	RecommendItemSeq GetResult(int userid, int limit, const std::string& exp);
	std::vector<std::string> InitAllExperiments();

	std::vector<std::string> onlines_;
	std::vector<std::string> experiments_;
};

}}

#endif
