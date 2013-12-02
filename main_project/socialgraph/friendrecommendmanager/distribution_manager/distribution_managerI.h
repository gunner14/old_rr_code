#ifndef DISTRIBUTION_MANAGER_H_
#define DISTRIBUTION_MANAGER_H_

#include "DistributionManager.h"

#include "UtilCxx/src/Singleton.h"

#include "socialgraph/friendrecommendmanager/distribution_manager/selector.h"

namespace xce {
namespace socialgraph {

const static std::string ONLINE = "O";
const static std::string EXPERIMENTI = "E1";
const static std::string EXPERIMENTII = "E2";
const static std::string EXPERIMENTIII = "E3";
const static std::string EXPERIMENTIV = "E4";
const static std::string EXPERIMENTV = "E5";
const static std::string EXPERIMENTVI = "E6";

//-------------------------------------------

class DistributionManagerI: virtual public DistributionManager,
		virtual public MyUtil::Singleton<DistributionManagerI> {
 public:
	DistributionManagerI() {}
	
	virtual RecommendItemSeq GetFriendRecommendByHome(int, int, const Ice::Current&);
	virtual void AddRecommendationFeedAsync(int, int, const Ice::Current&);
	
	virtual void ReloadBlackAndWhiteFile(int, const Ice::Current&);

 private:
	bool ConfirmTag(int userid, const std::string& tag, SelectorPtr tmp_ptr);

	RecommendItemSeq GetResult(int userid, int limit, std::string& exp);
};

}
}

#endif
