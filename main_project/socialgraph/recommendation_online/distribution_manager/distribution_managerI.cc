#include "distribution_managerI.h"

#include <queue>
#include <deque>

#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/InvokeMonitor.h"
#include "util/cpp/TimeCost.h"
#include "Monitor/client/service/ServiceMonitor.h"

#include "RecommendationExperimentIAdapter.h"
#include "RecommendationExperimentIIAdapter.h"
#include "RecommendationExperimentIIIAdapter.h"
#include "RecommendationExperimentIVAdapter.h"
#include "RecommendationExperimentVAdapter.h"
#include "RecommendationExperimentVIAdapter.h"
#include "RecommendationExperimentVIIAdapter.h"
#include "RecommendationExperimentVIIIAdapter.h"
#include "RecommendationExperimentVIIIIAdapter.h"
#include "RecommendationExperimentVVAdapter.h"
#include "RecommendationOnlineAdapter.h"

#include "RecommendationFeedOnlineAdapter.h"
#include "RecommendationFeedExperimentIAdapter.h"
#include "create_feed_helper.h"

#include "socialgraph/recommendation_online/builder/home_impl/friend_recommend_builder.h"
#include "socialgraph/recommendation_online/builder/feed_impl/feed_recommend_builder.h"
#include "socialgraph/recommendation_online/common/config_parser2.h"
#include "socialgraph/recommendation_online/common/id_filter.h"
#include "socialgraph/recommendation_online/common/recommendation_helper.h"

void MyUtil::initialize() {
  using namespace xce::serverstate;
  using namespace MyUtil;
  using namespace com::xiaonei::xce;
  using namespace xce::socialgraph;
 
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&DistributionManagerI::instance(), service.createIdentity("DM", ""));
  
  ConnectionPoolManager::instance().initialize();
  
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("DistributionManager.Mod");
  int sInterval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("DistributionManager.Interval", 5);
  
  //向controller注册
  ServerStateSubscriber::instance().initialize("ControllerDistributionManager",
          &DistributionManagerI::instance(), mod, sInterval, new MyUtil::SocialGraphChannel());

  ServerStateSubscriber::instance().setStatus(1);

  //ConfigParser::instance().Initialize("/data/xce/XSocialGraph/sg_conf/recommend2.conf");
  ConfigParser::instance().Initialize("/data/xce/XSocialGraph/sg_conf/recommendation_distribution.conf");
	IdFilter::instance().Initialize("/data/xce/XSocialGraph/sg_data");
}

//----------------------------------------------------

namespace xce {
namespace socialgraph {

RecommendItemSeq DistributionManagerI::GetFriendRecommendByHome(int userid, 
																																int limit, 
																																const Ice::Current& current) {
  std::ostringstream oss;
  oss << "[DistributionManagerI] GetFriendRecommendByHome userid:" << userid << " limit:" << limit;

	MyUtil::TimeCost tc = MyUtil::TimeCost::create(oss.str(), MyUtil::TimeCost::LOG_LEVEL_INFO);    //time cost

	std::string r_exp = "O";			//use "O" as default
	StrItemSeq str_seq = ConfigParser::instance().GetPriorityAsStrItemSeq(ID_PRIORITY, "priority");
	

	SelectorPtr random_ptr = SelectorFactory::Create(ID_RANDOM, HOME);		//random only initialize once
	for (StrItemSeq::iterator iter = str_seq.begin();
			iter != str_seq.end(); ++iter) {
		//MCE_DEBUG("Tag Confirm : " << *iter);
		if (ConfirmTag(userid, *iter, random_ptr)) {
			r_exp = *iter;
			break;
		}	
	}
	MCE_INFO("[DistributionManagerI] GetFriendRecommendByHome userid:" 
				<< userid << " limit:" << limit << " r_exp:" << r_exp);

  RecommendItemSeq result = GetResult(userid, limit, r_exp);    //raw recommend_item_list returned
  RecommendationHelper::AddExperimentTag(r_exp, result);    //add tag of experiment
  return result;
}

void DistributionManagerI::AddRecommendationFeedAsync(int userid, int limit, const Ice::Current& current) {
  std::ostringstream oss;
  oss << "[DistributionManagerI] AddRecommendationFeedAsync userid:" << userid << " limit:" << limit;

	MyUtil::TimeCost tc = MyUtil::TimeCost::create(oss.str(), MyUtil::TimeCost::LOG_LEVEL_INFO);    //time cost

	std::string r_exp = "F_O";			//use "F_O" as default
	StrItemSeq str_seq = ConfigParser::instance().GetPriorityAsStrItemSeq(ID_PRIORITY, "feed_priority");

	SelectorPtr random_ptr = SelectorFactory::Create(ID_RANDOM, FEED);		//random only initialize once

	for (StrItemSeq::iterator iter = str_seq.begin();
			iter != str_seq.end(); ++iter) {
		//MCE_INFO("Tag Confirm : " << *iter);
		if (ConfirmTag(userid, *iter, random_ptr)) {
			r_exp = *iter;
			break;
		}	
	}
	MCE_INFO("[DistributionManagerI] AddRecommendationFeedAsync userid:" 
				<< userid << " limit:" << limit << " r_exp:" << r_exp);
	RecommendItemSeq result = GetFeedRecommendationResult(userid, limit, r_exp);	//raw recommend_item_list returned
  RecommendationHelper::AddExperimentTag(r_exp, result);    //add tag of experiment

	if (!result.empty()) {
		CreateFeedHelper::CreatePageFeedRecommendation(userid, result);
	}
}
	
void DistributionManagerI::ReloadBlackAndWhiteFile(int userid, const Ice::Current& current) {
	MCE_DEBUG("[DistributionManagerI] ReloadBlackAndWhiteFile userid:" << userid);

	IdFilter::instance().Initialize("/data/xce/XSocialGraph/sg_data");
}

bool DistributionManagerI::ConfirmTag(int userid, const std::string& tag, SelectorPtr tmp_ptr) {
	std::string exp_tag = ConfigParser::instance().GetRuleAsString(ID_RULE, tag);
	//MCE_INFO("[DistributionManagerI] ConfirmTag userid:" << userid << " tag:" << tag << " exp_tag:" << exp_tag);
	if (exp_tag == DEFAULT)			//default as common strategy got from config
		return true;

	std::vector<std::string> str_list;
	boost::split(str_list, exp_tag, boost::is_any_of(" "));
	std::deque<std::string> sel_q;			//store with selector
	std::deque<std::string> cha_q;			//store with character

	for (int index = 0; index < str_list.size(); ++index) {
		if (index % 2 == 0) {							//"random:r_all && whitelist:w_1"
			sel_q.push_back(str_list.at(index));
		} else {
			cha_q.push_back(str_list.at(index));
		}
	}
	//MCE_DEBUG("sel_q size : " << sel_q.size());
	//while (!sel_q.empty()) {
	//	MCE_DEBUG(sel_q.front());
	//	sel_q.pop();
	//}
	//MCE_DEBUG("cha_q size : " << cha_q.size());
	//while (!cha_q.empty()) {
	//	MCE_DEBUG(cha_q.front());
	//	cha_q.pop();
	//}
	bool result = false;

	while (!sel_q.empty()) {
		std::string item = sel_q.front();
		sel_q.pop_front();
		std::vector<std::string> type_id;
		boost::split(type_id, item, boost::is_any_of(":"));					//"whitelist:w_1"

		//for (std::vector<std::string>::iterator iter = type_id.begin();
		//		iter != type_id.end(); ++iter) {
		//	MCE_DEBUG("type_id : " << *iter);
		//}

		if (type_id.size() != 2) {
			cha_q.pop_front();						//pop invalid character
			continue;
		}

		SelectorPtr ptr;
		if (ID_RANDOM == type_id[0]) {
			ptr = tmp_ptr;
		} else {
			ptr = SelectorFactory::Create(type_id[0], type_id[1]);		//ex:  whitelist w_1
		}

		if (NULL == ptr) {
			cha_q.pop_front();
			continue;
		}
		result = ptr->Access(userid, tag);					//condition

		std::string character = "";
		if (!cha_q.empty()) {
			character = cha_q.front();
			cha_q.pop_front();
		}

		if ("||" == character) {
			if (true == result) {
				if (!sel_q.empty()) {
					sel_q.pop_front();		//remove next selector
					sel_q.push_front("trueselector:trueselector");
				}
			} else {
				continue;
			}
		} else if ("&&" == character) {
			if (false == result) {
				if (!sel_q.empty()) {
					sel_q.pop_front();			//remove next selector
					sel_q.push_front("falseselector:falseselector");
				}
			} else {
				continue;
			}
		}
	}

	return result;
}

RecommendItemSeq DistributionManagerI::GetResult(int userid, int limit, std::string& exp) {
	MCE_INFO("DistributionManagerI::GetResult userid:" << userid << " limit:" << limit << " exp:" << exp);
	bool finish = true;
	RecommendItemSeq result;		//recommend_item_list returned

	if (ONLINE == exp) {			//use local server
		try {
			result = RecommendationOnlineAdapter::instance().GetFriendRecommendByHome(userid, limit);
		} catch (Ice::Exception& e) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome Online : " << e.what());
			finish = false;
		} catch (...) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome Online unknown exception");
			finish = false;
		}
	} else if (EXPERIMENTI == exp) {
		try {
			result = RecommendationExperimentIAdapter::instance().GetFriendRecommendByHome(userid, limit);
		} catch (Ice::Exception& e) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome ExperimentI : " << e.what());
			finish = false;
		} catch (...) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome ExperimentI unknown exception");
			finish = false;
		}
	} else if (EXPERIMENTII == exp) {
		try {
			result = RecommendationExperimentIIAdapter::instance().GetFriendRecommendByHome(userid, limit);
		} catch (Ice::Exception& e) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome ExperimentII : " << e.what());
			finish = false;
		} catch (...) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome ExperimentII unknown exception");
			finish = false;
		}
	} else if (EXPERIMENTIII == exp) {
		try {
			result = RecommendationExperimentIIIAdapter::instance().GetFriendRecommendByHome(userid, limit);
		} catch (Ice::Exception& e) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome ExperimentIII : " << e.what());
			finish = false;
		} catch (...) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome ExperimentIII unknown exception");
			finish = false;
		}
	} else if (EXPERIMENTIV == exp) {
		try {
			result = RecommendationExperimentIVAdapter::instance().GetFriendRecommendByHome(userid, limit);
		} catch (Ice::Exception& e) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome ExperimentIV : " << e.what());
			finish = false;
		} catch (...) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome ExperimentIV unknown exception");
			finish = false;
		}
	} else if (EXPERIMENTV == exp) {
		try {
			result = RecommendationExperimentVAdapter::instance().GetFriendRecommendByHome(userid, limit);
		} catch (Ice::Exception& e) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome ExperimentV : " << e.what());
			finish = false;
		} catch (...) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome ExperimentV unknown exception");
			finish = false;
		}
	} else if (EXPERIMENTVI == exp) {
		try {
			result = RecommendationExperimentVIAdapter::instance().GetFriendRecommendByHome(userid, limit);
		} catch (Ice::Exception& e) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome ExperimentVI : " << e.what());
			finish = false;
		} catch (...) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome ExperimentVI unknown exception");
			finish = false;
		}
	} else if (EXPERIMENTVII == exp) {
		try {
			result = RecommendationExperimentVIIAdapter::instance().GetFriendRecommendByHome(userid, limit);
		} catch (Ice::Exception& e) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome ExperimentVII : " << e.what());
			finish = false;
		} catch (...) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome ExperimentVII unknown exception");
			finish = false;
		}
	} else if (EXPERIMENTVIII == exp) {
		try {
			result = RecommendationExperimentVIIIAdapter::instance().GetFriendRecommendByHome(userid, limit);
		} catch (Ice::Exception& e) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome ExperimentVIII : " << e.what());
			finish = false;
		} catch (...) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome ExperimentVIII unknown exception");
			finish = false;
		}
	} else if (EXPERIMENTVIIII == exp) {
		try {
			result = RecommendationExperimentVIIIIAdapter::instance().GetFriendRecommendByHome(userid, limit);
		} catch (Ice::Exception& e) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome ExperimentVIIII : " << e.what());
			finish = false;
		} catch (...) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome ExperimentVIIII unknown exception");
			finish = false;
		}
	} else if (EXPERIMENTVV == exp) {
		try {
			result = RecommendationExperimentVVAdapter::instance().GetFriendRecommendByHome(userid, limit);
		} catch (Ice::Exception& e) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome ExperimentVV : " << e.what());
			finish = false;
		} catch (...) {
			MCE_WARN("DistributionManagerI::GetFriendRecommendByHome ExperimentVV unknown exception");
			finish = false;
		}
	}

	if (!finish) {
		result = FriendRecommendBuilder::instance().GetRecommendData(userid, limit);
		exp = "O";			//use "O" as default
	}
	return result;
}

RecommendItemSeq DistributionManagerI::GetFeedRecommendationResult(int userid, int limit, std::string& exp) {
	MCE_INFO("DistributionManagerI::GetFeedRecommendationResult userid:" << userid << " limit:" << limit << " exp:" << exp);
	bool finish = true;
	RecommendItemSeq result;		//recommend_item_list returned

	if (FEED_ONLINE == exp) {			//use local server
		try {
			result = RecommendationFeedOnlineAdapter::instance().GetRecommendationByFeed(userid, limit);
		} catch (Ice::Exception& e) {
			MCE_WARN("DistributionManagerI::GetFeedRecommendationResult Online : " << e.what());
			finish = false;
		} catch (...) {
			MCE_WARN("DistributionManagerI::GetFeedRecommendationResult Online unknown exception");
			finish = false;
		}
	} else if (FEED_EXPERIMENTI == exp) {
		try {
			result = RecommendationFeedExperimentIAdapter::instance().GetRecommendationByFeed(userid, limit);
		} catch (Ice::Exception& e) {
			MCE_WARN("DistributionManagerI::GetFeedRecommendationResult ExperimentI : " << e.what());
			finish = false;
		} catch (...) {
			MCE_WARN("DistributionManagerI::GetFeedRecommendationResult ExperimentI unknown exception");
			finish = false;
		}
	}

	if (!finish) {
		result = FeedRecommendBuilder::instance().GetRecommendData(userid, limit);
		exp = FEED_ONLINE;			//use "F_O" as default
	}
	return result;
}

}
}
