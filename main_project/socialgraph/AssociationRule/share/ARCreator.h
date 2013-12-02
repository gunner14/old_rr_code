/*************************************************************************
    > File Name: CacheManager.h
    > Author: zunwen.you
    > Mail: zunwen.you@renren-inc.com 
    > Created Time: 2012年09月25日 星期二 11时43分20秒
 ************************************************************************/

#ifndef __AR_CREATOR_H__
#define __AR_CREATOR_H__

#include <ext/hash_map>
#include <AssociationRule.h>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <UtilCxx/src/Singleton.h>
#include "ARCacheUtil.h"

typedef std::map<long, int> IndexCountMap;


namespace xce {
namespace socialgraph {

  class ARCreator : public MyUtil::Singleton<ARCreator> {
    public:
      friendDataSeq create(int userId, int applyId, int limit);
      RcdInfoSeq createRcd(int userId, int applyId, int limit);

      friend class MyUtil::Singleton<ARCreator>;

    private:
      static const int _ruleLimit = 20000;
      static const int _friendLimit = 1000;
      static const std::string _ARtype;
     
      IndexRuleMap _indexRuleMap;
      UserIndexSeqMap _userIndexSeqMap;
      MyUtil::IntSeq _friendList;
      MyUtil::IntSeq _applyList;
      
      MyUtil::IntSeq getSelectedFriends(int userId, int applyId);
      void collectRuleRatio(int userId, std::vector<Rule*>& rulePtrSeq);
      void collectRuleRatio(int applyId, RuleSeq& applyRules, std::vector<Rule*>& rulePtrSeq);
      void rankUsers(const std::vector<Rule*>& rulePtrSeq, int applyId, friendDataSeq& rcdSeq);
      void rankOnlyApplyId(const std::vector<Rule*>& rulePtrSeq, int applyId, friendDataSeq& rcdSeq);
      void rankOnlyApplyId(const std::vector<Rule*>& rulePtrSeq, int applyId, RcdInfoSeq& rcdSeq);
      void addOtherRcd(RcdInfoSeq& oriRcdSeq, const RcdInfoSeq& newRcdSeq);
      void filterList(int userId, int applyId, friendDataSeq& rcdSeq);
      void filterList(int userId, int applyId, RcdInfoSeq& rcdSeq);
 };

}
}

#endif
