#include "buddy_relation_client.h"

#include "OceCxxAdapter/src/BuddyRelationReplicaAdapter.h"

namespace xce {
namespace client {

using xce::buddy::adapter::BuddyRelationCacheAdapter;
using namespace xce::buddy;

bool BuddyRelationClient::IsFriend(int host, int guest) {
  Relationship relation = {host, guest};
  BuddyDesc result = BuddyRelationCacheAdapter::instance().getRelation(relation);
  return result == Friend;
}

}} //namespace xce::client
