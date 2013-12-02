#include "SocialGraphBuddyByIdAdapter.h"

using namespace xce::socialgraph;

Ice::Int SocialGraphBuddyByIdAdapter::getFriendCount(Ice::Int hostId) {
  return getProxy(hostId)->getFriendCount(hostId);
}

MyUtil::Int2IntMap SocialGraphBuddyByIdAdapter::getFriendCounts(const MyUtil::IntSeq& hostIds) {
  if(hostIds.empty()) {
    return MyUtil::Int2IntMap();
  }  else {
    try {
      return getProxy(hostIds.at(0))->getFriendCounts(hostIds);
    } catch(...) {
      return MyUtil::Int2IntMap();
    }
  } 
}

MyUtil::IntSeq SocialGraphBuddyByIdAdapter::getFriendList(Ice::Int hostId, Ice::Int limit) {
  return getProxy(hostId)->getFriendList(hostId, limit);
}

MyUtil::IntSeq SocialGraphBuddyByIdAdapter::getFriendListN(Ice::Int hostId, Ice::Int begin, Ice::Int limit) {
  return getProxy(hostId)->getFriendListN(hostId, begin, limit);
}

MyUtil::Int2IntSeqMap SocialGraphBuddyByIdAdapter::getFriendLists(const MyUtil::IntSeq& hostIds) {
  if(hostIds.empty()) {
    return MyUtil::Int2IntSeqMap();
  } else {
    try {
      return getProxy(hostIds.at(0))->getFriendLists(hostIds);
    } catch(...) {
      return MyUtil::Int2IntSeqMap();
    }
  }
}

void SocialGraphBuddyByIdAdapter::AddFriend(Ice::Int hostId, Ice::Int guestId) {
  getProxy(hostId)->AddFriend(hostId, guestId);
}

void SocialGraphBuddyByIdAdapter::RemoveFriend(Ice::Int hostId, Ice::Int guestId) {
  getProxy(hostId)->RemoveFriend(hostId, guestId);
}

