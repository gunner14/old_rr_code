//#include "search/GeneralSuggestion/seconddegreefriends/SecondDegreeFriendSearchDataI.h"
#include <cmath>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include "BuddyByIdReplicaAdapter.h"
#include "UserCacheAdapter.h"
#include "SecondDegreeFriendSearchDataI.h"
#include "SecondDegreeFriendsI.h"

using namespace xce::sdfriends;
using namespace xce::suggestiondata;
using namespace std;
using namespace MyUtil;
using namespace xce::serverstate;
using namespace com::xiaonei::xce;
using namespace xce::search;
using namespace xce::buddy::adapter;
using namespace xce::usercache;

void SecondDegreeFriendSearchDataI::load() {}

void SecondDegreeFriendSearchDataI::load(int id) {
  MCE_INFO("[SecondDegreeFriendSearchDataI::load] id:" << id);
  if (id < 0) return;
  ostringstream in;
  in << "Load SecondDegreeFriends: id_" << id;
  TimeCost tc = TimeCost::create(in.str(), 1);
  //获取带有共同好友数的二度好友id列表
  map<int,int> sdFriendListWithCommonNum;
  SecondDegreeData data;
  sdFriendListWithCommonNum = data.getSecondDegreeFriendsKMerge(id,_limit);
  MCE_INFO("[SecondDegreeFriendSearchDataI::load] sdFriendListWithCommonNum.size:" << sdFriendListWithCommonNum.size());
  //获取二度好友id列表
  vector<int> sdFriendIds;
  for (map<int,int>::iterator iter = sdFriendListWithCommonNum.begin(); iter != sdFriendListWithCommonNum.end(); ++iter) {
    sdFriendIds.push_back(iter->first);
  }
  //获取二度好友姓名
  map<int,string> sdFriendList;
  getFriendsName(sdFriendIds,sdFriendList);
  MCE_INFO("[SecondDegreeFriendSearchDataI::load] sdFriendList.size:" << sdFriendList.size());
  //存储结果
  vector< map<string,string> > docs;
  for (map<int,string>::iterator iter = sdFriendList.begin(); iter != sdFriendList.end(); ++iter) {
    int id = iter->first;
    string name = iter->second;
    int commonNum = sdFriendListWithCommonNum[id];
    map<string,string> doc;
    doc["id"] = boost::lexical_cast<string>(id);
    doc["name"] = name;
    doc["commonNum"] = boost::lexical_cast<string>(commonNum);
    docs.push_back(doc);
  }
  //MCE_INFO("==========SecondDegreeFriendSearchDataI::load 1111111111111");

  map<string, int> config;
  // 只存储
  config["id"] = 4;
  // 索引存储
  config["name"] = 7;
  config["commonNum"] = 7;
  add(docs, config, SECOND_DEGREE_FRIEND_DATA_TIMEOUT);
  ostringstream os;
  os << "SecondDegreeFriend Count: " << docs.size() << " Time Usage:";
  tc.step(os.str());
  MCE_INFO("[SecondDegreeFriendSearchDataI::load] User's secondDegreeFriend load finished");
}

void SecondDegreeFriendSearchDataI::getFriendsName(vector<int>& sdFriendIds, map<int,string>& sdFriendLists) {
  try{
    //获取二度好友姓名
    if(sdFriendIds.size() <= _maxSize){
      map<int, UserCachePtr> users = UserCacheAdapter::instance().GetUserCache(sdFriendIds);
      for(map<int, UserCachePtr>::iterator iter = users.begin(); iter != users.end(); ++iter) {
        string name = iter->second->name();
            transform(name.begin(),name.end(),name.begin(),::tolower);
            sdFriendLists[iter->first] = name;
      }
    }else{
    //若好友数较多，则分多次取ID对应的名字
      vector<int>::iterator iterS = sdFriendIds.begin();
      vector<int>::iterator iterE = iterS + _maxSize;
      while(iterS != sdFriendIds.end()){
        vector<int> part_ids(iterE - iterS);
            copy(iterS,iterE,part_ids.begin());
            iterS = iterE;
            iterE += _maxSize;
            if(iterE > sdFriendIds.end())
              iterE = sdFriendIds.end();
            map<int,UserCachePtr> part_users = UserCacheAdapter::instance().GetUserCache(part_ids);
            for(map<int,UserCachePtr>::iterator iter = part_users.begin(); iter != part_users.end(); ++iter){
              string name = iter->second->name();
              transform(name.begin(), name.end(), name.begin(), ::tolower);
              sdFriendLists[iter->first] = name;
            }
      }
    }
  }catch(...){
    MCE_WARN("Buddy||User throw Exception");
  }
}
