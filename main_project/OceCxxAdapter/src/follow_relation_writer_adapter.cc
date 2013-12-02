#include "follow_relation_writer_adapter.h"

namespace xce {
namespace buddy {
namespace adapter {

void FollowRelationWriterAdapter::AddFollowRelation(const MyUtil::GlobalIdPtr& gid, ::Ice::Int uid) {
  getProxy(gid->id)->add(gid, uid);
}

void FollowRelationWriterAdapter::RemoveFollowRelation(const MyUtil::GlobalIdPtr& gid, ::Ice::Int uid) {
  getProxy(gid->id)->remove(gid, uid);
}
   
}
}
}


