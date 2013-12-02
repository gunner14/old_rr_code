/*************************************************************************
    > File Name: CacheManager.cc
    > Author: zunwen.you
    > Mail: zunwen.you@renren-inc.com 
    > Created Time: 2012年09月25日 星期二 12时55分40秒
 ************************************************************************/
#include "ARCacheUtil.h"

#include <boost/lexical_cast.hpp>
#include "item.pb.h"     
#include "Singleton.h"
#include "tripod2/cc/core/tripod_client.h"
#include "OceCxxAdapter/src/SocialGraphCommonTripodAdapter.h"
#include "socialgraph/socialgraphutil/mc_adapter.h"


using namespace std;
using namespace MyUtil;
using namespace xce::socialgraph;

string ARCacheUtil::ruleSuffix = "_AR";
string ARCacheUtil::indexSuffix = "_II";

/*
 * @brief: use rule index to get rule for tripod cache 
 * key: rule index
 * rule: return value
*/
bool ARCacheUtil::getRule(int userId, RuleSeq& ruleSeq) {
  string myKey;
  try {
    myKey = boost::lexical_cast<string>(userId);
  } catch (boost::bad_lexical_cast& e) {
    MCE_WARN("[CacheManager::getIndexList] bad lexcical cast from " << userId << "to string");
    return false;
  }
	
  string value;
  SocialGraphCommonTripodAdapter::instance().Get(myKey + ruleSuffix, &value);

  bool succ = parseRule(value, ruleSeq);
  return succ;
}

/*
 * @brief: use rule index to multi get rule for tripod cache 
 * keyList: rule index list
 * indexRuleMap: return value
*/
map<int, bool> ARCacheUtil::multiGetRule(const IntSeq& userList
    , UserIndexSeqMap& userIndexSeqMap, IndexRuleMap& indexRuleMap) {

  map<int, bool> userSuccMap;
  xce::tripod::StringListType keySeq;
  BOOST_FOREACH(const int index, userList) {
    userSuccMap.insert(pair<int, bool>(index, false));
    string myKey;
    try {
      myKey = boost::lexical_cast<string>(index);
    } catch (boost::bad_lexical_cast& e) {
      MCE_WARN("[CacheManager::multiGetRule] bad lexcical cast from " << myKey << "to string");
      continue;
    }
    keySeq.push_back(myKey + ruleSuffix);
  }
  xce::tripod::StringMapType keyValueMap;
  
  SocialGraphCommonTripodAdapter::instance().MultiGet(keySeq, &keyValueMap);
  
  map<string, string>::iterator itrKeyValue = keyValueMap.begin(); 
  for( ; itrKeyValue != keyValueMap.end(); itrKeyValue++ ) {
    //parse userId from the string with indexSuffix 
 
    string tmpKey = itrKeyValue->first;
    size_t pos = tmpKey.find(ruleSuffix);
    int idx = boost::lexical_cast<int>(tmpKey.substr(0, pos - 0));
    string value = itrKeyValue->second;

    IndexSeq indexSeq;
    bool succ = parseRule(value, indexSeq, indexRuleMap);
    userIndexSeqMap[idx] = indexSeq;
    userSuccMap[idx] = succ;
  }
  
  return userSuccMap;
}

map<int, bool> ARCacheUtil::getRulesByMultiThread(const IntSeq& userList
    , UserIndexSeqMap& userIndexSeqMap, IndexRuleMap& indexRuleMap){
  map<int, bool> userSuccMap;
  BOOST_FOREACH (int userId, userList) {
    TaskManager::instance().execute(new GetRuleTask(userId, userIndexSeqMap, indexRuleMap, userSuccMap));
  }
  while (TaskManager::instance().size(TASK_LEVEL_GETRULE) > 0 
      || TaskManager::instance().runningThreadSize(TASK_LEVEL_GETRULE) > 0) {
    usleep(5000);
  }
  return userSuccMap;
}

/*
 * @brief: use userId to get rule index list for tripod cache 
 * userId: user Id  in renren.com
 * indexList: return rule index list
*/
bool ARCacheUtil::getIndexList(int userId, MyUtil::LongSeq& indexList) {
  string value;
  string key;
  try {
     key = boost::lexical_cast<string>(userId);
  } catch (boost::bad_lexical_cast& e) {
    MCE_WARN("[CacheManager::getIndexList] bad lexcical cast from " << key << "to string");
    return false;
  }
  SocialGraphCommonTripodAdapter::instance().Get(key + indexSuffix, &value);
  return parseIndexList(value, indexList);
}

/*
 * @brief: use userId to multi get rule index list for tripod cache 
 * userId: user Id list in renren.com
 * indexList: return value 
*/
map<int, bool> ARCacheUtil::multiGetIndexList(const MyUtil::IntSeq& userIdSeq, Int2LongSeqMap& userIndexListMap) {
  map<int, bool> userSuccMap;
  xce::tripod::StringListType keySeq;
  BOOST_FOREACH(const int userId, userIdSeq) {
    userSuccMap.insert(pair<int, bool>(userId, false));
    string myKey;
    try {
      myKey = boost::lexical_cast<string>(userId);
    } catch (boost::bad_lexical_cast& e) {
      MCE_WARN("[CacheManager::multiGetIndexList] bad lexcical cast from " << myKey << "to string");
      continue;
    }
    keySeq.push_back(myKey + indexSuffix);
  }
  xce::tripod::StringMapType keyValueMap;
  SocialGraphCommonTripodAdapter::instance().MultiGet(keySeq, &keyValueMap);

  map<string, string>::iterator itrKeyValue = keyValueMap.begin(); 
  int succCount = 0;
  for( ; itrKeyValue != keyValueMap.end(); itrKeyValue++ ) {
    //parse userId from the string with indexSuffix 
    string tmpKey = itrKeyValue->first;
    size_t pos = tmpKey.find(indexSuffix);
    int userId = boost::lexical_cast<int>(tmpKey.substr(0, pos - 0));
    string value = itrKeyValue->second;
    MyUtil::LongSeq indexList;
    int succ = parseIndexList(value, indexList);
    userIndexListMap.insert(pair<int, MyUtil::LongSeq>(userId, indexList));        
    userSuccMap[userId] = succ;
    succCount++;
  }
  return userSuccMap;
}

/* prase rule from value obtained from tripod cache */
bool ARCacheUtil::parseRule(const string& value, IndexSeq& indexSeq, IndexRuleMap& indexRuleMap) {
  if (value.empty()) {
    return false;
  }
  BinaryData rawRule(value.begin(), value.end());
  Items rawItems;
  bool parseSucc = rawItems.ParseFromArray(rawRule.data(), rawRule.size());
  if (!parseSucc) {
    return false;
  }
  BOOST_FOREACH(const Item& rawItem, rawItems.items()) {
    long index;
    try {
      index = boost::lexical_cast<long>(rawItem.message());
    } catch(boost::bad_lexical_cast& e) {
      MCE_WARN("[ARCacheUtil::parseRule] bad lexcical cast from Item.message to long");
      return false;
    }
    if (indexRuleMap.find(index) != indexRuleMap.end()) {
      continue;
    }
    Rule rule;
    rule.index = index;
    rule.support = rawItem.id();
    rule.confidence = rawItem.fields(0);
    int apriSize = rawItem.fields(1);
    ::google::protobuf::RepeatedField< ::google::protobuf::int32 >::const_iterator itrFields;
    itrFields= rawItem.fields().begin();
    rule.aprioriIds.assign(itrFields + 2, itrFields + apriSize + 2);
    rule.consequenceIds.assign(itrFields + apriSize + 2, itrFields + rawItem.fields().size());
    rule.hit = 0;
    rule.ratio = 0.0;
    indexSeq.push_back(rule.index);
    indexRuleMap[rule.index] = rule;
  }
  return true;
}

bool ARCacheUtil::parseRule(const string& value, RuleSeq& ruleSeq) {
  if (value.empty()) {
    return false;
  }
  BinaryData rawRule(value.begin(), value.end());
  Items rawItems;
  bool parseSucc = rawItems.ParseFromArray(rawRule.data(), rawRule.size());
  if (!parseSucc) {
    return false;
  }
  BOOST_FOREACH(const Item& rawItem, rawItems.items()) {
    Rule rule;
    try {
      rule.index = boost::lexical_cast<long>(rawItem.message());
    } catch(boost::bad_lexical_cast& e) {
      MCE_WARN("[ARCacheUtil::parseRule] bad lexcical cast from Item.message to long");
      return false;
    }
    rule.support = rawItem.id();
    rule.confidence = rawItem.fields(0);
    int apriSize = rawItem.fields(1);
    ::google::protobuf::RepeatedField< ::google::protobuf::int32 >::const_iterator itrFields;
    itrFields= rawItem.fields().begin();
    rule.aprioriIds.assign(itrFields + 2, itrFields + apriSize + 2);
    rule.consequenceIds.assign(itrFields + apriSize + 2, itrFields + rawItem.fields().size());
    rule.hit = 0;
    rule.ratio = 0.0;
    ruleSeq.push_back(rule);
  }
  return true;
}


/* prase index list from value obtained from tripod cache */
bool ARCacheUtil::parseIndexList(const string& value, LongSeq& indexList) {
  if (value.empty()) {
    return false;
  }
  BinaryData rawIndeces(value.begin(), value.end());
  Items rawItems;
  if (rawItems.ParseFromArray(rawIndeces.data(), rawIndeces.size())) {
    BOOST_FOREACH(Item rawItem, rawItems.items()) {
      BOOST_FOREACH(string ms, rawItem.messages()) {
        long index;
	   		try {
          index = boost::lexical_cast<long>(ms);
	   		} catch (boost::bad_lexical_cast& e) {
	   			MCE_WARN("boost::lexical_cast error: " << e.what() << "\n"); 
	   		}
        indexList.push_back(index);
      }
    }
    return true;
  }
  return false;
}

// ---------------------------------------------------------------
/*
GetRuleTask::GetRuleTask(const int& userId, map<int, RuleSeq>& userRulesMap, ::Ice::Int level) {
  _userId = userId;
  _userRulesMap = userRulesMap;
  MyUtil::Task(level);
}
*/
IceUtil::Mutex GetRuleTask::_mutex;

void GetRuleTask::handle() {
  RuleSeq ruleSeq;
  bool succ = ARCacheUtil::getRule(_userId, ruleSeq);
  IndexSeq indexSeq;
  BOOST_FOREACH(Rule& rule, ruleSeq) {
    indexSeq.push_back(rule.index);
    IceUtil::Mutex::Lock lock(_mutex);
    {
      _indexRuleMap[rule.index] = rule;
    }
  }
  IceUtil::Mutex::Lock lock(_mutex);
  {
    _userIndexSeqMap.insert(pair<int, IndexSeq>(_userId, indexSeq));
    _userSuccMap.insert(pair<int, bool>(_userId, succ));
  }
}

