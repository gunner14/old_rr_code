#ifndef BASE_BUILDER_H_
#define BASE_BUILDER_H_

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>

#include "RecommendationManager.h"

namespace xce {
namespace socialgraph {

/*
@brief base buider offer process of building recommendation
*/
class BaseBuilder {
 public:
	BaseBuilder() { }
	virtual ~BaseBuilder() { }

	RecommendItemSeq GetRecommendData(int userid, int limit);

	/*
	@brief load configuration or data
	*/
	virtual void Preload() = 0;

	/*
	@brief	prepare raw data
	*/
	virtual RecommendItemSeq Prepare() = 0;

	/*
	@brief fliter item_list by block 
	*/
	virtual void Fliter(RecommendItemSeq& item_list) = 0;

	/*
	@brief online rank
	*/
	virtual void Rank(RecommendItemSeq& item_list) = 0;

	/*
	@brief add explanation in each recommenditem
	*/
	virtual void Explanation(RecommendItemSeq& item_list) = 0;

 protected:
	int userid_;			//userid
	int limit_;				//limit
	
	boost::mutex mutex_;		//mutex
};

typedef boost::shared_ptr<BaseBuilder> BaseBuilderPtr;		//define BaseBuilderPtr

}}

#endif
