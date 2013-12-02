/*
 * =====================================================================================
 *
 *       Filename:  freshGroup.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月03日 10时05分38秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef __INDEX_SERVICE_FRESH_GROUP__
#define __INDEX_SERVICE_FRESH_GROUP__

#include "AdDemoQuality.h"
#include "AdDemoQualityAdapter.h"
#include <IceUtil/Shared.h>

namespace xce{
	namespace ad{

		class FreshAdGroupsCache: public MyUtil::Singleton<FreshAdGroupsCache> {
			public:
				FreshAdQualityMap GetFreshGroups();

				string GetFreshKey(Ice::Long zone_id, int pos); 

				void getFreshGroupsByAdzone(long adzone, int pos, std::map<long, double> &gids);

				void Load(); 
			private:
				FreshAdQualityMap fresh_groups_;
				IceUtil::RWRecMutex mutex_;
		};

	}
}

#endif
