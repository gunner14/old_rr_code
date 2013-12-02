#ifndef GUIDE_RECOMMEND_CONTENT_I_H_
#define GUIDE_RECOMMEND_CONTENT_I_H_

#include "GuideRecommendContent.h"
#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace socialgraph {

class GuideRecommendContentI : virtual public GuideRecommendContent,
		virtual public MyUtil::Singleton<GuideRecommendContentI> {
 public:
	virtual void TriggerContentNewsFeed(int, int, const Ice::Current&);

 private:
	void Print(const std::map<int, std::string>& obj);

	friend class MyUtil::Singleton<GuideRecommendContentI>;
};

}}

#endif
