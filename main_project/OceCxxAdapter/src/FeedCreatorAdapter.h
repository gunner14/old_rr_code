#ifndef FEED_CREATOR_ADAPTER_H
#define FEED_CREATOR_ADAPTER_H

#include "FeedCreator.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce{
namespace feed{

using namespace MyUtil;

class FeedCreatorAdapter : public AdapterI,
	public AdapterISingleton<MyUtil::XceFeedChannel, FeedCreatorAdapter>
{

protected:
	virtual string name() {
		return "FeedAssistant";
	}
	virtual string endpoints() {
		return "@FeedAssistant";
	}
	virtual size_t cluster() {
		return 1;
	}

public:
	FeedCreatorAdapter();
	~FeedCreatorAdapter();

	bool Create(int stype, int version, const MyUtil::Str2StrMap& props);        
	bool CreateEDM(int stype, int version, const MyUtil::Str2StrMap& props);        
  
private:

	FeedCreatorPrx getManager();
	FeedCreatorPrx getManagerOneway();

	vector<FeedCreatorPrx> _managersOneway;
	vector<FeedCreatorPrx> _managers;
};


};
};
#endif
