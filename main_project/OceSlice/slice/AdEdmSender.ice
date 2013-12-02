#ifndef __AD_EDM_SENDER__
#define __AD_EDM_SENDER__

#include <Util.ice>

module xce {
  module ad {

		sequence<long> groups;
    interface AdEdmSender {
              void SendEdm(int userId, MyUtil::Str2StrMap params);
 	      void Expose(int userId, long adgroupId, int index);
              void SendEdmTest(int userId, MyUtil::Str2StrMap params);
	      void ClearLoginCacheByUser(int userid);
				groups GetCandidateGroups(int userid);
    };

  };
};
#endif
