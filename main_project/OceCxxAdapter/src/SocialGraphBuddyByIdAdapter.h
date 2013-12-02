#ifndef _SOCIALGRAPH_BUDDYBYID_H_
#define _SOCIALGRAPH_BUDDYBYID_H_

#include "SocialGraphBuddyById.h"
#include "Channel.h"
#include "AdapterI.h"
#include "Singleton.h"

namespace xce {
namespace socialgraph {

class SocialGraphBuddyByIdAdapter : public MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::SocialGraphBuddyByIdPrx>,
    public MyUtil::Singleton<SocialGraphBuddyByIdAdapter> {
 public: 
  SocialGraphBuddyByIdAdapter():
    MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::SocialGraphBuddyByIdPrx>("ControllerSocialGraphBuddyById", 
										120, 5000, new MyUtil::SocialGraphChannel()) {
  }

  Ice::Int getFriendCount(Ice::Int hostId);
  MyUtil::Int2IntMap getFriendCounts(const MyUtil::IntSeq& hostIds);

  MyUtil::IntSeq getFriendList(Ice::Int hostId, Ice::Int limit);
  MyUtil::IntSeq getFriendListN(Ice::Int hostId, Ice::Int begin, Ice::Int limit);
  MyUtil::Int2IntSeqMap getFriendLists(const MyUtil::IntSeq& hostIds);

  void AddFriend(Ice::Int hostId, Ice::Int guestId);
  void RemoveFriend(Ice::Int hostId, Ice::Int guestId);
};

};
};

#endif
