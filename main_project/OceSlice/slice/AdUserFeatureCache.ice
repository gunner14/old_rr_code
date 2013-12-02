#ifndef __AD_USER_FEATURE_CACHE_ICE__
#define __AD_USER_FEATURE_CACHE_ICE__


#include <Util.ice>

module xce {
  module ad{

    interface AdUserFeatureCache{
      string GetFeature(long userID);
      string GetNowFile();
      bool Rollback(string version);
    };

};
};

#endif

