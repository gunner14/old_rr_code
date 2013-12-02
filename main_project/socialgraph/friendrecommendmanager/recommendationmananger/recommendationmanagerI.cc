#include "recommendationmanagerI.h"

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/random.hpp>

#include "RecommendationExperimentIAdapter.h"
#include "RecommendationExperimentIIAdapter.h"
#include "RecommendationExperimentIIIAdapter.h"

#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/InvokeMonitor.h"
#include "util/cpp/TimeCost.h"
#include "Monitor/client/service/ServiceMonitor.h"

#include "socialgraph/friendrecommendmanager/builder/friend_recommend_builder.h"
#include "socialgraph/friendrecommendmanager/common/config_parser.h"
#include "socialgraph/friendrecommendmanager/common/recommendation_helper.h"

void MyUtil::initialize() {
  using namespace xce::serverstate;
  using namespace MyUtil;
  using namespace com::xiaonei::xce;
  using namespace xce::socialgraph;

  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&RecommendationManagerI::instance(), service.createIdentity("RM", ""));

  ConnectionPoolManager::instance().initialize();

  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("RecommendationManager.Mod");
  int sInterval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("RecommendationManager.Interval", 5);

  //向controller注册
  ServerStateSubscriber::instance().initialize("ControllerRecommendationManager", 
					&RecommendationManagerI::instance(), mod, sInterval, new MyUtil::SocialGraphChannel());

  ServerStateSubscriber::instance().setStatus(1);

	ConfigParser::instance().Initialize("/data/xce/XSocialGraph/sg_conf/people_recommend.conf");
}

//----------------------------------------------------

namespace xce {
namespace socialgraph {

RecommendItemSeq RecommendationManagerI::GetFriendRecommendByHome(int userid, 
																																  int limit, const Ice::Current& current) {
	// 1. get experiment tag of this request
	// 2. get recommendations according to experiment tag
	// 3. add tag
  using namespace com::xiaonei::xce;

	std::ostringstream oss;
	oss << "RecommendationManagerI::GetFriendRecommendByHome userid:" << userid << " limit:" << limit;

  MyUtil::TimeCost tc = MyUtil::TimeCost::create(oss.str(), MyUtil::TimeCost::LOG_LEVEL_INFO);		//time cost

	//PrepareAllExp();		//fill in experiments
	std::string r_exp = GetExperiment(userid);		//get tag of experiment
  MCE_INFO("[RecommendationManagerI] GetFriendRecommendByHome userid:" << userid 
						<< " limit:" << limit << " exp:" << r_exp);
	
	RecommendItemSeq result = GetResult(userid, limit, r_exp);		//raw recommend_item_list returned
	RecommendationHelper::AddExperimentTag(r_exp, result);		//add tag of experiment
	return result;
}

std::string RecommendationManagerI::GetExperiment(int userid) {
	std::string r_exp = DEFAULT;		//use default
	std::vector<std::string> onlines;
	std::vector<std::string> experiments;

	//online
	std::string o = ConfigParser::instance().Get(Online, "online", "rule_1");
	onlines.push_back(o);

	//experiment
	experiments = InitAllExperiments();
	//std::string e1 = ConfigParser::instance().Get(Online, "experiment", "rule_1");
	//std::string e3 = ConfigParser::instance().Get(Online, "experiment", "rule_3");
	//experiments.push_back(e1);
	//experiments.push_back(e3);

	std::string online_part = ConfigParser::instance().Get(Online, "online", "proportion");
	std::string experiment_part = ConfigParser::instance().Get(Online, "experiment", "proportion");
	
	float online_proportion = 0;
	float experiment_proportion = 0;
	try {
		online_proportion = boost::lexical_cast<float>(online_part);
		experiment_proportion = boost::lexical_cast<float>(experiment_part);
	} catch (boost::bad_lexical_cast& e) {
		MCE_DEBUG("[RecommendationManagerI]::GetExperiment " << e.what());
		return r_exp;
	}

	//boost::rand48 rng;
	//float r = rng() * 1.0 / std::numeric_limits<boost::rand48::result_type>::max();
	srand(time(NULL));
	float r = (float)rand() / RAND_MAX;

	MCE_DEBUG("RecommendationManagerI::GetExperiment r:" << r);
	try {
		if (r <= online_proportion) {
			int index = rand() % onlines.size();
			r_exp = onlines.at(index);
		} else {
			int index = rand() % experiments.size();
			r_exp = experiments.at(index);
		}
	} catch (...) {
		MCE_DEBUG("RecommendationManagerI::experiments unknown error");
	}

	return r_exp;
}

std::vector<std::string> RecommendationManagerI::InitAllExperiments() {
	std::vector<std::string> experiments;
	std::string prefix = "rule_";
	for (int i = 1; i <= 20; ++i) {			//support 20 experiments, and experiment index begin from 1 
		try {
			std::string key = prefix + boost::lexical_cast<std::string>(i);
			std::string e = ConfigParser::instance().Get(Online, "experiment", key);		//get experiment
			if (!e.empty()) {	
				experiments.push_back(e);
			}
		} catch (boost::bad_lexical_cast& e) {
			MCE_DEBUG("[RecommendationManagerI]::InitAllExperiments " << e.what());
		} catch (...) {
			MCE_DEBUG("[RecommendationManagerI]::InitAllExperiments unknown exception");
		}
	}
	return experiments;
}

//void RecommendationManagerI::PrepareAllExp() {
//	std::vector<std::string> tmp_online;
//	std::vector<std::string> tmp_experiment;
//
//	//online
//	std::string o = ConfigParser::instance().Get(Online, "online", "rule_1");
//	tmp_online.push_back(o);
//
//	//experiment
//	std::string e1 = ConfigParser::instance().Get(Online, "experiment", "rule_1");
//	//std::string e3 = ConfigParser::instance().Get(Online, "experiment", "rule_2");
//	//std::string e4 = ConfigParser::instance().Get(Online, "experiment", "rule_3");
//	tmp_experiment.push_back(e1);
//	//experiments_.push_back(e3);
//	//experiments_.push_back(e4);
//	{
//		boost::mutex::scoped_lock lock(mutex_);
//
//		onlines_.swap(tmp_online);
//		experiments_.swap(tmp_experiment);
//	}
//	MCE_DEBUG("[RecommendationManagerI] PrepareAllExp onlines_:" << onlines_.size() 
//			<< " experiments_:" << experiments_.size());
//}

RecommendItemSeq RecommendationManagerI::GetResult(int userid, int limit, const std::string& exp) {
	bool finish = true;
	RecommendItemSeq result;		//recommend_item_list returned

	if (DEFAULT == exp) {			//use local server
		result = FriendRecommendBuilder::instance().GetRecommendData(userid, limit);
	} else if (EXPERIMENTI == exp) {
		try {
			result = RecommendationExperimentIAdapter::instance().GetFriendRecommendByHome(userid, limit);
		} catch (Ice::Exception& e) {
			MCE_DEBUG("RecommendationManagerI::GetFriendRecommendByHome ExperimentI : " << e.what());
			finish = false;
		} catch (...) {
			MCE_DEBUG("RecommendationManagerI::GetFriendRecommendByHome ExperimentI unknown exception");
			finish = false;
		}
	} else if (EXPERIMENTII == exp) {
		try {
			result = RecommendationExperimentIIAdapter::instance().GetFriendRecommendByHome(userid, limit);
		} catch (Ice::Exception& e) {
			MCE_DEBUG("RecommendationManagerI::GetFriendRecommendByHome ExperimentII : " << e.what());
			finish = false;
		} catch (...) {
			MCE_DEBUG("RecommendationManagerI::GetFriendRecommendByHome ExperimentII unknown exception");
			finish = false;
		}
	} else if (EXPERIMENTIII == exp) {
		try {
			result = RecommendationExperimentIIIAdapter::instance().GetFriendRecommendByHome(userid, limit);
		} catch (Ice::Exception& e) {
			MCE_DEBUG("RecommendationManagerI::GetFriendRecommendByHome ExperimentIII : " << e.what());
			finish = false;
		} catch (...) {
			MCE_DEBUG("RecommendationManagerI::GetFriendRecommendByHome ExperimentIII unknown exception");
			finish = false;
		}
	}

	if (!finish) {
		result = FriendRecommendBuilder::instance().GetRecommendData(userid, limit);
	}
	return result;
}

}}
