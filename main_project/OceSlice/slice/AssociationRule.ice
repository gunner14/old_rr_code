#ifndef __ASSOCIATION_RULE_H__ 
#define __ASSOCIATION_RULE_H__

#include <Util.ice>

module xce {
  module socialgraph {
  
    struct Rule {
      int index;
      int recommendId;
      int confidence;
      MyUtil::IntSeq aprioriIds;
    };
    sequence<Rule> RuleSeq;
  
    struct FriendData {
      int userID;
      float weight;
    };
    sequence<FriendData> friendDataSeq;
    
    interface AssociationRule {
      friendDataSeq getRecommendByAssociationRule(int userId, int applyId, int limit);
      //MyUtil::IntSeq apply(int userId, int applyId);
    };
  
  };
};

#endif



