/*
 * =====================================================================================
 *
 *       Filename:  TriggerInterface.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年04月23日 16时05分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "IndexServiceReplicaAdapter.h"
#include "FeedMemcProxy/client/user_profile_client.h"

namespace xce{
  namespace ad{

    class TriggerInterface : public MyUtil::Singleton<TriggerInterface>{
      public:
        int trigger(const UserProfile &profile, std::vector<long> &zones, GroupDict& groups);
    };
	}
}
