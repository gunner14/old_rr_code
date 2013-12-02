/*************************************************************************
    > File Name: CacheManager.h
    > Author: zunwen.you
    > Mail: zunwen.you@renren-inc.com 
    > Created Time: 2012年09月25日 星期二 11时43分20秒
 ************************************************************************/

#ifndef __AR_CACHE_UTIL_H__
#define __AR_CACHE_UTIL_H__

#include <AssociationRule.h>
#include "OceCxxAdapter/src/SocialGraphCommonTripodAdapter.h"

#include "TaskManager.h"

typedef vector<char> BinaryData;
typedef std::map< int, std::vector<long> > Int2LongSeqMap;
typedef std::map< long, std::vector<int> > Long2IntSeqMap;

typedef std::vector<long> IndexSeq;
typedef std::map<int, IndexSeq> UserIndexSeqMap;
typedef std::map<long, xce::socialgraph::Rule> IndexRuleMap;

static const int TASK_LEVEL_GETRULE = 1;
static const int TASK_LEVEL_PARSERULE = 2;


namespace xce {
namespace socialgraph {

  class ARCacheUtil {
    public:
      static bool getRule(int userId, RuleSeq& ruleSeq);
      static std::map<int, bool> multiGetRule(const MyUtil::IntSeq& userList, UserIndexSeqMap& userIndexSeqMap, IndexRuleMap& indexRuleMap);
      static std::map<int, bool> getRulesByMultiThread(const MyUtil::IntSeq& userList, UserIndexSeqMap& userIndexSeqMap, IndexRuleMap& indexRuleMap);

      static bool getIndexList(int userId, MyUtil::LongSeq& indexList);
      static std::map<int, bool> multiGetIndexList(const MyUtil::IntSeq& userIdList, Int2LongSeqMap& userIndexListMap);

    private:
      static std::string ruleSuffix;
      static std::string indexSuffix;
      static bool parseRule(const string& raw, RuleSeq& ruleSeq);
      static bool parseRule(const string& raw, IndexSeq& indexSeq, IndexRuleMap& indexRuleMap);
      static bool parseIndexList(const string& rule, MyUtil::LongSeq& indexList);

  };

// -----------task class-----------------------------------------------------------
  class GetRuleTask : public MyUtil::Task {
    public:
      GetRuleTask(const int& userId, UserIndexSeqMap& userIndexSeqMap, IndexRuleMap& indexRuleMap  
          , map<int, bool>& userSuccMap, ::Ice::Int level = TASK_LEVEL_GETRULE):
        _userId(userId), _userIndexSeqMap(userIndexSeqMap), _indexRuleMap(indexRuleMap), _userSuccMap(userSuccMap), MyUtil::Task(level){
       }
      virtual void handle();

    private:
      int _userId;
      
      UserIndexSeqMap& _userIndexSeqMap;
      IndexRuleMap& _indexRuleMap;
      std::map<int, bool>& _userSuccMap;
      static IceUtil::Mutex _mutex;
  };

}
}


#endif
