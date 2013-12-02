#include "gtest/gtest.h"
#include <iostream>
#include "ad/mob/UT/code/Engine.h"
#include "ad/mob/UT/code/CachePool.h"
#include "ad/mob/UT/code/Struct.h"
#include "ad/mob/UT/code/InvertedIndex.h"
#include "ad/mob/UT/code/Config.h"

#include "ParaCheckHandlerTest.h"
#include "SelectorTest.h"

using namespace std;
using namespace xce::ad;

const string DbInstance_Main = "ads_db";
const string DbInstance_Ads = "ads_db";
const string DbInstance_Wr = "ads_db";

class AdMobUTEnvironment : public testing::Environment {
	public:
		virtual void SetUp() {
			srand(time(NULL));

			InvertedIndex::instance().SetDB(DbInstance_Ads);
			EngineConfig::instance().Initialize(0);

			PlatformMap::instance().Initialize(DbInstance_Main, ONE_MINUTE);
			Brand3GMap::instance().Initialize(DbInstance_Main, ONE_MINUTE);
			ResolutionMap::instance().Initialize(DbInstance_Main, ONE_MINUTE);
			ResolutionMatcher::instance().Initialize(DbInstance_Main, ONE_MINUTE);

			MemberPool::instance().Initialize(DbInstance_Main, ONE_MINUTE);
			CampaignPool::instance().Initialize(DbInstance_Ads, ONE_MINUTE);
			GroupPool::instance().Initialize(DbInstance_Ads, ONE_MINUTE);
			ZonePool::instance().Initialize(DbInstance_Main, ONE_MINUTE);

			AdMobEngine::instance().initOK();
		}
		virtual void TearDown() {
		}
};


int main(int argc, char** argv){
	testing::AddGlobalTestEnvironment(new AdMobUTEnvironment);
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

