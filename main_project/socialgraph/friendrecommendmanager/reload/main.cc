#include <iostream>

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include "FriendFinderFFWAdapter.h"

#include "RecommendationExperimentIAdapter.h"
#include "RecommendationExperimentIIAdapter.h"
#include "RecommendationExperimentIIIAdapter.h"
#include "RecommendationOnlineAdapter.h"
#include "RecommendationManangerAdapter.h"
#include "DistributionManagerAdapter.h"

#include "socialgraph/friendrecommendmanager/builder/base_builder.h"
#include "socialgraph/friendrecommendmanager/builder/friend_recommend_builder.h"
#include "DbCxxPool/src/QueryRunner.h"

using namespace xce::socialgraph;

static void show_help() {
	puts(
		"Usage: ./ReloadUtil All or ./ReloadUtil Tailnum"
	);
}

int main(int argc, char* argv[]) {
	using namespace xce::socialgraph;
	using namespace com::xiaonei::xce;
	using namespace MyUtil;

	ConnectionPoolManager::instance().initialize();

	if (argc != 2) {
		std::cerr << "Error parameter!" << std::endl;
		show_help();
		return 0;
	}

	std::cout << "raw argv:" << argv[0] << "  " << argv[1] << std::endl;
	if (strcmp(argv[1], "All") == 0) {
		for (int i = 0; i < 10; ++i) {
			try {
				DistributionManagerAdapter::instance().ReloadBlackAndWhiteFile(i);
			} catch (...) {
				std::cerr << "Exception : " << i << std::endl;
			}
		}
	} else {
		try {
			int num = boost::lexical_cast<int>(argv[1]);
			DistributionManagerAdapter::instance().ReloadBlackAndWhiteFile(num);
		} catch (boost::bad_lexical_cast& e) {
			std::cerr << e.what() << std::endl;
		} catch (...) {
			std::cerr << "unknown exception." << std::endl;
		}
	}
}
