#include "FriendSuggestUtil.h"

using namespace com::xiaonei::friendsuggest;

void FriendSuggestUtil::generateNameMap(vector<int>& fids, map<int,string>& id2Name, int maxSize) {
    int size = fids.size();
    try{
      if(size < maxSize) {
        map<int, UserCachePtr> users=UserCacheAdapter::instance().GetUserCache(fids);
        for(map<int, UserCachePtr>::iterator it = users.begin();it != users.end();++it) {
          string name = it->second->name();
          transform(name.begin(), name.end(), name.begin(), ::tolower);
          id2Name[it->first] = name;
        }
      } else {
        vector<int>::iterator itS = fids.begin();
        vector<int>::iterator itE = itS + maxSize;
        while(itS != fids.end()){
          vector<int> part_ids(itE-itS);
          copy(itS,itE,part_ids.begin());
          itS = itE;
          itE += maxSize;
          if(itE > fids.end())
            itE = fids.end();
          map<int, UserCachePtr> part_users = UserCacheAdapter::instance().GetUserCache(part_ids);
          for(map<int, UserCachePtr>::iterator it = part_users.begin();it != part_users.end();++it){
            string name = it->second->name();
            transform(name.begin(), name.end(), name.begin(), ::tolower);
            id2Name[it->first] = name;
          }
        }
      }
    }catch(...){
      MCE_WARN("Buddy||User throw Exception");
    }
}

void FriendSuggestUtil::buildIndex(RelationLoaderAdapter& adapter_, int id_, map<int,string>& id2Name, QuerySuggestion *& qs,int tag, int maxSize,int dataNum) {
  map<int, map<int,short> > res;
  try {
    if(tag == 1) {
      res = adapter_.GetRelationV2(id_,1,"FriendInput");//一度好友，返回key=1的一条记录
    } else if(tag == 2) {
      res = adapter_.GetRelationV2(id_,2,"SecondDegreeFriend");//二度好友，返回key=2的一条记录
    }
  } catch(...) {
    MCE_WARN("test zk. error,adapter.GetRelateionV2 error~");
    return;
  }

  map<int, map<int,short> >::iterator it = res.begin();
  if(res.size() != 1 || it->first != tag) {
    MCE_WARN("get result from adapter.GetRelationV2 is wrong~");
    return;
  }
  if((res[tag].size() == 0)) {
    MCE_WARN("id:"<<id_<<" has none friend~");
    return;
  }

  vector<int> fids;
  int count = 0;
  for(map<int,short>::iterator iter = res[tag].begin();iter != res[tag].end()&&count<dataNum;iter++) {
    fids.push_back(iter->first);
    count++;
  }
  generateNameMap(fids, id2Name, maxSize);
 
  int uid;
  string name;
  short commonNum = 0;
  map<int,short>::iterator iter;
  for(map<int,string>::iterator it = id2Name.begin();it != id2Name.end();it++) {
    uid = it->first;
    name = it->second;
    iter = res[tag].find(it->first);
    if(iter != res[tag].end()) {
      commonNum = iter->second;
    }
    qs->addData(uid,name,commonNum);
  }
}

int FriendSuggestUtil::getStrLen(const string& condition) {
  int size = 0;
  for(vector<string>::size_type i = 0;i < condition.size();++i) {
    if((condition[i]&0x80) != 0) {
      ++size;
      i += 2;
    } else {
      ++size;
    }   
  }
  return size;
}

//===============================================================
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
  dict[name].push_back(pinyin);
  return true;
}

