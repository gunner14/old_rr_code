#include "sgmessage_handler.h"

#include "item.pb.h"
#include "socialgraph/socialgraphutil/mc_adapter.h"
#include "OceCxxAdapter/src/SocialGraphRcdTripodAdapter.h"
#include "OceCxxAdapter/src/SocialGraphRcdPageCfTripodAdapter.h"
#include "OceCxxAdapter/src/SocialGraphGuideUGCAdapter.h"
#include "OceCxxAdapter/src/DistributionManagerAdapter.h"
#include "socialgraph/socialgraphutil/algostuff.h"
#include "UserStateAdapter.h"

namespace xce {
namespace socialgraph {

void RemoveRcdCacheHandler::Update(const SGMessage& sgm) {
	long source = sgm.source;
	long target = sgm.target;
	MCE_INFO("[RemoveRcdCacheHandler] Update source:" << source << " target:" << target);

	std::string value;
	std::string key;
	try {
		key = boost::lexical_cast<std::string>(source);
	} catch (boost::bad_lexical_cast& e) {
		MCE_ERROR("[RemoveRcdCacheHandler] Update " << e.what());
		return;
	} catch (...) {
		return;
	}
	BinaryData bin_data;
	SocialGraphRcdTripodAdapter::instance().Get(key, &value);
	if (value.empty()) {
		return;
	}
	bin_data = BinaryData(value.begin(), value.end());	

	Items old_items;
	if (!old_items.ParseFromArray(bin_data.data(), bin_data.size())) {
  	MCE_ERROR("RemoveRcdCacheHandler ParseFromArray Failed : " << source);
		return;
	}

	Items new_items;
	BOOST_FOREACH(const Item& it, old_items.items()) {
		if (it.id() != target) {
			Item* item = new_items.add_items();
			*item = it;
		}
	}
	BinaryData new_bin_data;
	int size = new_items.ByteSize();
	new_bin_data.resize(size);
	new_items.SerializeToArray(new_bin_data.data(), size);

	SocialGraphRcdTripodAdapter::instance().Set(key, std::string(new_bin_data.begin(), new_bin_data.end()), 0);
}

//-------------------------------------------

void RemoveRedisCacheHandler::Update(const SGMessage& sgm) {

}

//-------------------------------------------

void InsertSGBlockHandler::Update(const SGMessage& sgm) {

}

//-------------------------------------------

void RemoveFriendClusterHandler::Update(const SGMessage& sgm) {
	long source = sgm.source;
	long target = sgm.target;
	MCE_INFO("[RemoveFriendClusterHandler] Update source:" << source << " target:" << target);

	int64_t codeid = source + 100000000000000;
	const std::string biz = "FFUCR";

	BinaryData raw_data;
	bool fetch = BusinessCacheAdapter::GetInstance().Get(biz, codeid, raw_data);

	Items raw_items;
  if (!raw_items.ParseFromArray(raw_data.data(), raw_data.size())) {
  	MCE_ERROR("RemoveFriendClusterHandler ParseFromArray Failed : " << source);
		return;
  }

	Items new_items;
	BOOST_FOREACH(const Item& it, raw_items.items()) {
		if (it.id() != target) {
			Item* item = new_items.add_items();
			*item = it;
		}	
	}

	BinaryData new_bin_data;
	int size = new_items.ByteSize();
	std::string s;
	new_items.SerializeToString(&s);
	new_bin_data.reserve(size);
	new_bin_data.assign(s.data(), s.data() + size);
	BusinessCacheAdapter::GetInstance().Set(biz, source, new_bin_data);
}

//-------------------------------------------

void RemoveRcdPageCfTripodHandler::Update(const SGMessage& sgm) {
	MCE_INFO("[RemoveRcdPageCfTripodHandler] Update source:" << sgm.source << " target:" << sgm.target);
	std::string key;
	std::string value;

	try {
		//key = boost::lexical_cast<std::string>(sgm.source);
		key = xce::Comb("RCDPAGECF_", sgm.source);
		SocialGraphRcdPageCfTripodAdapter::instance().Get(key, &value);
		if (!value.empty()) {
			BinaryData page_data(value.begin(), value.end());
			Items page_items;
			if (page_items.ParseFromArray(page_data.data(), page_data.size())) {
				Items new_items;
				BOOST_FOREACH(const Item& it, page_items.items()) {
					if (it.id() != sgm.source) {
						Item* item = new_items.add_items();
						*item = it;
					}	
				}
				BinaryData new_bin_data;
				int size = new_items.ByteSize();
				std::string s;
				new_items.SerializeToString(&s);
				new_bin_data.reserve(size);
				new_bin_data.assign(s.data(), s.data() + size);
				if (new_bin_data.empty()) {
					SocialGraphRcdPageCfTripodAdapter::instance().Remove(key);
				} else {
					SocialGraphRcdPageCfTripodAdapter::instance().Set(key, 
							std::string(new_bin_data.begin(), new_bin_data.end()), 0);
				}
			}
		}
	} catch (boost::bad_lexical_cast& e) {
		MCE_WARN("[RemoveRcdPageCfTripodHandler] Update boost::Exception: " << e.what());
	} catch (Ice::Exception& e) {
		MCE_WARN("[RemoveRcdPageCfTripodHandler] Update Ice::Exception: " << e.what());
	} catch (...) {
		MCE_WARN("[RemoveRcdPageCfTripodHandler] Update UnknownException");
	}
}

//-------------------------------------------

void NotifyFeedDistributionHandler::Update(const SGMessage& sgm) {
	using namespace xce::adapter::userstate;
	MCE_INFO("[NotifyFeedDistributionHandler] Update message.id:" << sgm.id);
	int userid = sgm.id;
	int limit = -1;
	UserStateInfoPtr ptr = UserStateAdapter::instance().getUserState(userid);
	if (ptr->isGuideDone()) {
	  DistributionManagerPrx prx = DistributionManagerAdapter::instance().getProxy(userid);
		MCE_INFO("[NotifyFeedDistributionHandler] Update home userid:" << userid << " prx:" << prx);
		prx->AddRecommendationFeedAsync(sgm.id, limit);
		//DistributionManagerAdapter::instance().AddRecommendationFeedAsync(sgm.id, limit);	
	} else {
		MCE_INFO("[NotifyFeedDistributionHandler] Update guide userid:" << userid);
	}
}

//-------------------------------------------

void NotifyGuideUGCServerHandler::Update(const SGMessage& sgm) {
	using namespace xce::adapter::userstate;
	MCE_INFO("[NotifyGuideUGCServerHandler] Update message.id:" << sgm.id);
	int userid = sgm.id;
	UserStateInfoPtr ptr = UserStateAdapter::instance().getUserState(userid);
	if (ptr->isGuideDone()) {
		MCE_INFO("[NotifyGuideUGCServerHandler] Update home userid:" << userid);
	} else {
		MCE_INFO("[NotifyGuideUGCServerHandler] Update guide userid:" << userid);
		SocialGraphGuideUGCAdapter::instance().NotifyUserLogin(userid);
	}
}

//-------------------------------------------

HandlerPtr HandlerFactory::Create(const std::string& behaviour) {
	if (RemoveRcdCache == behaviour) {
		return HandlerPtr(new RemoveRcdCacheHandler());

	} else if (InsertSGBlock == behaviour) {
		return HandlerPtr(new InsertSGBlockHandler());

	} else if (RemoveRedisCache == behaviour) {
		return HandlerPtr(new RemoveRedisCacheHandler());

	} else if (NotifyFeedDistribution == behaviour) {
		return HandlerPtr(new NotifyFeedDistributionHandler());

	} else if (RemoveFriendCluster == behaviour) {
		return HandlerPtr(new RemoveFriendClusterHandler());

	} else if (RemoveRcdPageCfTripod == behaviour) {
		return HandlerPtr(new RemoveRcdPageCfTripodHandler());

	} else if (NotifyGuideUGCServer == behaviour) {
		return HandlerPtr(new NotifyGuideUGCServerHandler());

	}

	return HandlerPtr();
}

}
}
