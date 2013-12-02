#ifndef __ADUSERCACHELOADER_REPLICA_ADAPTER_H__
#define __ADUSERCACHELOADER_REPLICA_ADAPTER_H__

#include "AdapterI.h"
#include "AdUserCache.h"

namespace xce{
  namespace ad{
    using namespace MyUtil;

    class AdUserCacheLoaderReplicaAdapter: public MyUtil::ReplicatedClusterAdapterI<AdUserCacheLoaderPrx>,
    public MyUtil::Singleton<AdUserCacheLoaderReplicaAdapter> {
      public:
        AdUserCacheLoaderReplicaAdapter(): MyUtil::ReplicatedClusterAdapterI <AdUserCacheLoaderPrx> ("ControllerAdUserCacheLoaderR", 120, 150, new XceFeedControllerChannel()) {}

        string GetById(int userid);
    
      //offline load interface
        int LoadFromAduserinfoDBByIdSeq(MyUtil::IntSeq& userIds);
        int LoadFromAduserinfoDBById(int userId);
        int LoadFromSourceDBByIdSeq(MyUtil::IntSeq& userIds);
        int LoadFromSourceDBById(int userId);
    
      //offline debug interface 
        int Set(UserInfo& info,int& flag);
        int Reset(int userId);
        int GetDebuggerList(userseq& infoseq);    
        int GetByIdOffline(int userId,int& flag,UserInfo& info);
      //offline inference interface
        int OfflineInference(int userId);
        int SetOfflineGender(int userId, int gender);

      protected:
        AdUserCacheLoaderPrx getManagerOneway(int id);
        AdUserCacheLoaderPrx getManager(int id);

    };

  };
};
#endif
