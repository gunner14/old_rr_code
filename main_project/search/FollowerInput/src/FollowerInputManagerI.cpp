#include "UserCacheAdapter.h"
#include "BuddyByIdReplicaAdapter.h"
#include "FollowerInputManagerI.h"
#include <cmath>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "ad/gj_cache/src/UserCacheClientPlus.h"
#include <vector>
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace com::xiaonei::services::FollowerInput;
using namespace xce::buddy::adapter;
using namespace xce::usercache;
using namespace xce::serverstate;
using namespace xce::ad;

void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  service.registerObjectCache(FOLLOWERINPUT_DATA,"FOLLOWERINPUT",new FollowerInputFactoryI,false);
  service.getAdapter()->add(&FollowerInputManagerI::instance(), service.createIdentity("Manager",""));
  ServerStateSubscriber::instance().initialize("ControllerFollowerInput",&FollowerInputManagerI::instance(),10,5, new MyUtil:: GadgetsChannel);
}

Ice::ObjectPtr FollowerInputFactoryI::create(Ice::Int id) {
  ostringstream oss;
  oss<<"CreateIndex : "<< id ;
  TimeCost tc = TimeCost::create(oss.str(),1);
  MCE_INFO("[FollowerInputFactoryI::create] create for:" << id );
  FollowerInputDataIPtr obj=new FollowerInputDataI(id);
  std::map<int, std::string> mm;
  //获取id对应的好友的名字, 存入mm中
  generateNameMap(id,mm);
  obj->add(mm);
  return obj;
}

void FollowerInputFactoryI::generateNameMap(Ice::Int uid, map<int, string>& follower2Name) {
  ostringstream oss;
  oss<<"GenerateNameMap : "<< uid ;
  TimeCost tc = TimeCost::create(oss.str(),1);
  MCE_INFO("[FollowerInputFactoryI::generateNameMap] for user:"<<uid);
  try{
    //获取用户好友id
    vector<int> friendsIds=BuddyByIdCacheAdapter::instance().getFriendListAsc(uid,20000);
    tc.step("Get FriendsIds");
    //获取用户关注的人的id
    //
    vector<long> follower;
    vector<int> followerIds;
    UserCacheClientPlus::instance().GetFollower(follower,(long)uid, 2000);
    MCE_INFO("[FollowerInputFactoryI::generateNameMap] uid : " << uid << " friendsIds : " << friendsIds.size() << " follower : " << follower.size());
    tc.step("Get FollowerIds");
    followerIds = complement_vec(follower, friendsIds);
    tc.step("Get follower - friendsIds");
    MCE_INFO("[FollowerInputFactoryI::generateNameMap] uid : " << uid << " after complement followerIds : " << followerIds.size());
  
    map<int, UserCachePtr> users=UserCacheAdapter::instance().GetUserCache(followerIds);
    for(map<int, UserCachePtr>::iterator it=users.begin();it!=users.end();++it) {
      string name = it->second->name();
      transform(name.begin(), name.end(), name.begin(), ::tolower);
      follower2Name[it->first] = name;
    }
  }catch(...){
    MCE_WARN("Buddy||User throw Exception");
  }
  MCE_INFO("[FollowerInputFactoryI::generateNameMap] Add uid:"<<uid<<" follower2Name :"<<follower2Name.size());
}

bool PinYinResultHandler::handle(mysqlpp::Row& res) const {
#ifndef NEWARCH
  string name=res["hanzi"].get_string();
  string pinyin=res["pinyin"].get_string();
#else
  string name=res["hanzi"].c_str();
  string pinyin=res["pinyin"].c_str();
#endif
  if(pinyin=="")
    return false;
  dict_[name].push_back(pinyin);
  return true;
}

FollowerInputManagerI::FollowerInputManagerI() {
  Statement sql;
  sql << "SELECT hanzi, pinyin FROM pinyin";
  PinYinResultHandler handler(newSet, dict_);
  QueryRunner(DB_FRIENDINPUT_PINYIN, CDbRServer).query(sql, handler);
}

MyUtil::IntSeq FollowerInputManagerI::getFollowerInput(Ice::Int hostID,const string& condition,Ice::Int limit,const Ice::Current& crt) {
  MCE_INFO("[FollowerInputManagerI::getFollowerInput] hostID= "<<hostID<<" condition="<<condition<<" limit="<<limit);
  if(condition.size()>15||condition.size()==0)
    return vector<int>();
  else  {
    FollowerInputDataIPtr server = ServiceI::instance().findObject<FollowerInputDataIPtr>(FOLLOWERINPUT_DATA, (long)hostID);
    if(server!=NULL){
      time_t old = server->getTime();
      int duration = time(NULL) - old;
      MCE_INFO("duration is:"<<duration);
      if(duration >= TIMEOUT_CACHE)  {
        ServiceI::instance().removeObject(FOLLOWERINPUT_DATA, (long)hostID);
      }
    }
    return ServiceI::instance().locateObject<FollowerInputDataIPtr>(FOLLOWERINPUT_DATA, (long)hostID)->search(condition,limit);
  }
};

void FollowerInputManagerI::reLoadFollower(Ice::Int hostID,const Ice::Current& crt) {
  MCE_INFO("[FollowerInputManagerI::reLoadFollower] hostID="<<hostID);
  FollowerInputDataIPtr obj = ServiceI::instance().findObject<FollowerInputDataIPtr>(FOLLOWERINPUT_DATA, (long)hostID);
  if(!obj){
    ServiceI::instance().addObject(obj, FOLLOWERINPUT_DATA, (long)hostID);
  }
};

vector<string> FollowerInputManagerI::dict(const string& word) {
  map<string,vector<string> >::const_iterator it = dict_.find(word);
  if(it != dict_.end()){
    return it->second;
  }
  return vector<string>();
}

void FollowerInputDataI::splitWord(const string& value, vector<string>& words)
{
  for(vector<string>::size_type i=0;i<value.size();++i) {
    //是否为汉字
    if((value[i]&0x80)!=0) {
      words.push_back(value.substr(i,3));
      i+=2;//累计已加3,即一个UTF8中文编码字符长度
    }else {
      if(value[i]!=' ')
        words.push_back(string(1,value[i]));
    }
  }
}

void FollowerInputDataI::add(std::map<int,std::string>& result)
{
  MCE_INFO("[FollowerInputDataI::add] for "<<id_);
  ostringstream  oss;
  oss<<"AddNameMap : "<< id_;
  TimeCost tc = TimeCost::create(oss.str(),1);
  followerName.swap(result);
  string name;
  unsigned int id;
  for(map<int,string>::iterator it=followerName.begin();it!=followerName.end();++it) {
    name=it->second;
    id=it->first;
    vector<string> words;
    splitWord(name,words);
    for(unsigned int i=0;i<words.size();++i) {
      if(words[i].size()==3) {
        vector<string> pinyins=FollowerInputManagerI::instance().dict(words[i]);
        for(vector<string>::iterator it_vec=pinyins.begin();it_vec!=pinyins.end();++it_vec) {
          postingsPY[(*it_vec)].push_back(id);
        }
      }
      postingsHZ[words[i]].push_back(id);
    }
  }
  old_ = time(NULL);
}

MyUtil::IntSeq FollowerInputDataI::search(const string& query,Ice::Int limit) {
  string transformed_query(query);
  transform(transformed_query.begin(), transformed_query.end(), transformed_query.begin(), ::tolower);
  ostringstream oss;
  oss<<"Search : "<< id_<<","<<query;
  TimeCost tc = TimeCost::create(oss.str(),1);
  MCE_DEBUG("begin search " << id_ <<"."<< transformed_query);
  map<int,int> value;
  vector<string> words;
  splitWord(transformed_query, words);
  if(isPinYin(words)) {
    vector<string> subStr(transformed_query.size());
    splitPY(transformed_query,0,0,subStr, value);
  }
  if(limit < 0 || value.size() < limit)
    searchHZ(words,value);
  return select(value,limit);
}

vector<int> FollowerInputDataI::select(const map<int,int>& value,unsigned int limit) {
  vector<pair<int,int> > res;
  res.reserve(value.size());
  for(map<int,int>::const_iterator it=value.begin();it!=value.end();++it){
    res.push_back(make_pair<int>(it->second,it->first));
  }
  MCE_INFO("getFriends number : "<<value.size());

  sort(res.begin(),res.end());

  size_t count = 0;
  vector<int> result;
  for(vector<pair<int,int> >::iterator it=res.begin();it!=res.end() && count<limit;++it,++count){
    result.push_back(it->second);
    MCE_INFO("select "<<it->second<<" "<<it->first);
  }

  return result;
}

bool FollowerInputDataI::reChoose(int doc,const vector<std::string>& strs,const int& len,int& first) {
  int inc=0;
  first = len;
  vector<string> words;
  splitWord(followerName[doc], words);
  for(unsigned int i=0;i<words.size()&&inc<len;++i) {
    if(words[i].size()==3) {
      vector<string> pinyins=FollowerInputManagerI::instance().dict(words[i]);
      if(!pinyins.empty()) {
        for(vector<string>::iterator it=pinyins.begin();it!=pinyins.end();++it) {
          if(startsWith(*(it),strs[inc])) {
            if(inc == 0){
              first = i;
            }
            ++inc;
            break;
          }
        }
      }
    }
  }
  if(inc==len)
    return true;
  else
    return false;
}

bool FollowerInputDataI::reChooseHZ(int doc,const vector<string>& str,int& first){
  string name = followerName[doc];
  size_t pos = 0;
  first =  str.size();
  for(size_t i=0;i<str.size();++i){
    pos = name.find(str[i],pos);
    if(pos == string::npos){
      break;
    }else if(i == 0){
      first = pos;
    }
    pos += str[i].size();
  }
  if(pos!=string::npos){
    MCE_INFO("find hz doc "<<doc<<" "<<name);
  }
  return pos!=string::npos;
}

void FollowerInputDataI::splitPY(const string& pinyin,unsigned int start,int num,vector<string> subStr,map<int,int>& value) {
  if(start>=pinyin.size()) {
    vector<int> temp=getPrefix(subStr[0]);
    for(int i=1;i<num;i++) {
      intersect_vec(temp,getPrefix(subStr[i]));
      if(temp.empty())
        return;
    }
    for(vector<int>::iterator it=temp.begin();it!=temp.end();++it) {
      int pos;
      int id = *it;
      if(reChoose(id,subStr,num,pos)) {
        ++pos;
        if(value[id] == 0 || value[id] > pos)
          value[id] = pos;
      }
    }
    temp.clear();
    return;
  }else {
    for(unsigned int end=start+1;end<=pinyin.size();++end) {
      string temp=pinyin.substr(start,end-start);
      map<string,vector<int> >::iterator it=postingsPY.lower_bound(temp);
      if(it!=postingsPY.end()&&startsWith(it->first,temp)) {
        subStr[num]=temp;
        splitPY(pinyin,end,num+1,subStr,value);
      }
    }
  }
}

void FollowerInputDataI::searchHZ(const vector<string>& words,map<int,int>& value) {
  if (words.empty()) {
    return;
  }
  map<string,vector<int> >::iterator it = postingsHZ.find(words[0]);
  vector<int> results;
  if(it!=postingsHZ.end()){
    results = it->second;
  }else{
    return;
  }
  for(size_t i=1;i<words.size() && !results.empty();++i)  {
    it = postingsHZ.find(words[i]);
    if(it!=postingsHZ.end()){
      intersect_vec(results,it->second);
    }
  }
  MCE_INFO("get hz result "<<results.size());
  for(size_t i=0;i<results.size();++i){
    int pos;
    int id = results[i];
    if(reChooseHZ(id,words,pos)){
      ++pos;
      if(value[id] == 0 || value[id] > pos)
        value[id] = pos;
    }
  }
}

