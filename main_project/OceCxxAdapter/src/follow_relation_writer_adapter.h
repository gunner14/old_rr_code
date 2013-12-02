#ifndef __FOLLOW_RELATION_WRITER_ADAPTER_H__
#define __FOLLOW_RELATION_WRITER_ADAPTER_H__

#include "AdapterI.h"
#include "Singleton.h"
#include "TaskManager.h"
#include "FollowRelation.h"
#include "UtilCxx/src/GlobalId.h"

namespace xce {
namespace buddy {
namespace adapter {

class FollowRelationWriterAdapter:
        public MyUtil::ReplicatedClusterAdapterI<FollowRelationWriterPrx>,
        public MyUtil::Singleton<FollowRelationWriterAdapter>  {
 public:
  FollowRelationWriterAdapter():
    MyUtil::ReplicatedClusterAdapterI<FollowRelationWriterPrx>("ControllerFollowRelationWriter", 120, 300) {}

  virtual void AddFollowRelation(const MyUtil::GlobalIdPtr& gid, ::Ice::Int uid);
  virtual void RemoveFollowRelation(const MyUtil::GlobalIdPtr& gid, ::Ice::Int uid);

 private:
  friend class MyUtil::Singleton<FollowRelationWriterAdapter>;
};//class FollowRelationWriterAdapter

}//namespace adapter
}//namespace buddy
}//namespace xce

#endif


