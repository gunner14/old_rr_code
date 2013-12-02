/*
 * =====================================================================================
 *
 *       Filename:  MiniGroupCommon.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月14日 17时34分50秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  xiaoguang.yang (), xiaoguang.yang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#include "MiniGroupCommon.h"

using namespace xce::mngp;
using namespace com::xiaonei::xce;

MiniGroupInfo::MiniGroupInfo(long pid, const string& pname, const string& picon) {
  this->id = pid;
  this->name = pname;
  this->icon = picon;
}

MyUtil::Str2StrMap MiniGroupInfo::getExtraInfo() {
  IceUtil::Mutex::Lock lock(*this);
  return this->extraInfo;
}

MiniGroupInfo::MiniGroupInfo(const MiniGroupPtr& ptr) : MiniGroup(ptr->id, ptr->name, ptr->icon, ptr->extraInfo) {
}

void MiniGroupInfo::addToExtraInfo(const string& key, const string& value) {
  IceUtil::Mutex::Lock lock(*this);
  this->extraInfo.insert(make_pair(key, value));
}

MiniGroupInfo::MiniGroupInfo(long id, const string& name, const string& icon, 
    const MyUtil::Str2StrMap& extraInfo) : MiniGroup(id, name, icon, extraInfo) {
}

void MiniGroupInfo::setId (long id) {
  IceUtil::Mutex::Lock lock(*this);
  this->id = id;
}

void MiniGroupInfo::setName(const string& name) {
  IceUtil::Mutex::Lock lock(*this);
  this->name = name;
}

void MiniGroupInfo::setIcon(const string& icon) {
  IceUtil::Mutex::Lock lock(*this);
  this->icon = icon;
}

long MiniGroupInfo::getId() {
  IceUtil::Mutex::Lock lock(*this);
  return this->id;
}

string MiniGroupInfo::getName() {
  IceUtil::Mutex::Lock lock(*this);
  return this->name;
}

string MiniGroupInfo::getIcon() {
  IceUtil::Mutex::Lock lock(*this);
  return this->icon;
}

string MiniGroupInfo::serialize() {
  string resultString;
  xce::distcache::minigroup::GroupData groupData;
  {
    IceUtil::Mutex::Lock lock(*this);
    groupData.set_id(this->id);
    groupData.set_name(this->name);
    groupData.set_icon(this->icon);
  }
  groupData.SerializeToString(&resultString);
  return resultString;
}

User2MiniGroupInfo::User2MiniGroupInfo(int user_id, const MyUtil::LongSeq& minigroup_ids) : User2MiniGroup(user_id, minigroup_ids) {
}

User2MiniGroupInfo::User2MiniGroupInfo(const User2MiniGroupPtr& ptr) : User2MiniGroup(ptr->userId, ptr->miniGroupIds) {
}

void User2MiniGroupInfo::User2MiniGroupInfo::setJoinedMiniGroups(const MyUtil::LongSeq& minigroup_ids) {
  IceUtil::Mutex::Lock lock(*this);
  this->miniGroupIds = minigroup_ids;
}

void User2MiniGroupInfo::addJoinedMiniGroups(const MyUtil::LongSeq& minigroup_ids, bool addDirectly) {
  IceUtil::Mutex::Lock lock(*this);
  if (addDirectly) {
    this->miniGroupIds.insert(this->miniGroupIds.end(), minigroup_ids.begin(), minigroup_ids.end());
    return;
  }
  for (MyUtil::LongSeq::const_iterator it = minigroup_ids.begin(); it != minigroup_ids.end(); ++it) {
    MyUtil::LongSeq::iterator found_it = find(this->miniGroupIds.begin(), this->miniGroupIds.end(), *it);
    if (found_it == this->miniGroupIds.end()) {
      this->miniGroupIds.push_back(*it);
    }
  }
}

void User2MiniGroupInfo::removeJoinedMiniGroups(const MyUtil::LongSeq& minigroup_ids) {
  IceUtil::Mutex::Lock lock(*this);
  for (MyUtil::LongSeq::const_iterator it = minigroup_ids.begin(); it != minigroup_ids.end(); ++it) {
    MyUtil::LongSeq::iterator found_it = find(this->miniGroupIds.begin(), this->miniGroupIds.end(), *it);
    if (found_it != this->miniGroupIds.end()) {
      this->miniGroupIds.erase(found_it);
    }
  }
}

void User2MiniGroupInfo::clearJoinedMiniGroups() {
  IceUtil::Mutex::Lock lock(*this);
  this->miniGroupIds.clear();
}


int User2MiniGroupInfo::getUserId() {
  IceUtil::Mutex::Lock lock(*this);
  return this->userId;  
}

string User2MiniGroupInfo::serialize() {
  xce::distcache::user2minigroup::User2MiniGroupData user2MiniGroupData;
  {
    IceUtil::Mutex::Lock lock(*this);
    user2MiniGroupData.set_userid(userId);
    vector<long>::const_iterator it;
    for(it = this->miniGroupIds.begin(); it != this->miniGroupIds.end(); ++it){
      user2MiniGroupData.add_groupids(*it);
    }
  }
  string resultString;
  user2MiniGroupData.SerializeToString(&resultString);
  return resultString;
}

MyUtil::LongSeq User2MiniGroupInfo::getJoinedMiniGroupIds() {
  IceUtil::Mutex::Lock lock(*this);
  return this->miniGroupIds;
}

UserIdsProducerHandlerI::UserIdsProducerHandlerI(vector<int>& ids, int& id) : result_ids_(ids), max_id_(id) {
}

bool UserIdsProducerHandlerI::handle(mysqlpp::Row& row) const {
  int id = (int) row[TABLE_MEMBER_MINIGROUP_MEMBER_ID.c_str()];
  result_ids_.push_back(id);
  if (id > max_id_) {
    max_id_ = id;
  }
  return true;
}

MiniGroupInfoResultHandlerI::MiniGroupInfoResultHandlerI(vector<MiniGroupInfoPtr>& mnGroupInfoPtrs) : minigroupinfo_ptrs_(mnGroupInfoPtrs) {
}

bool MiniGroupInfoResultHandlerI::handle(mysqlpp::Row& row) const {
  long id = (long)row[TABLE_MINIGROUP_ID.c_str()];
  string name = (string)row[TABLE_MINIGROUP_NAME.c_str()];
  string icon = (string)row[TABLE_MINIGROUP_ICON.c_str()];
  MiniGroupInfoPtr ptr = new MiniGroupInfo(id, name, icon);
  //MCE_INFO("[MiniGroupInfoResultHandlerI::handle] id = " << id << ", name = " << name << ", icon = " << icon);
  minigroupinfo_ptrs_.push_back(ptr);
  return true;
}

User2MiniGroupInfoResultHandlerI::User2MiniGroupInfoResultHandlerI(vector<User2MiniGroupInfoPtr>& user2ptrs) : user2minigroupinfo_ptrs_(user2ptrs) {
}

bool User2MiniGroupInfoResultHandlerI::handle(mysqlpp::Row& row) const {
  long minigroup_id = (long)row[TABLE_MEMBER_MINIGROUP_MNGP_ID.c_str()];
  int memberId = (int)row[TABLE_MEMBER_MINIGROUP_MEMBER_ID.c_str()];
  //MCE_INFO("[User2MiniGroupResultHandlerI::handle] minigroup_id = " << minigroup_id << " memberId = " << memberId);
  MyUtil::LongSeq ids;
  ids.push_back(minigroup_id);
  //MCE_INFO("[User2MiniGroupResultHandlerI::handle] minigroupId = " << minigroup_id << " memberId = " << memberId);
  //要这样做必须保证查询memeber_group表的sql语句中一定要包括group by member_id, 否则下面的逻辑就是不对的
  if (user2minigroupinfo_ptrs_.empty() || (*(user2minigroupinfo_ptrs_.end() - 1))->userId != memberId) {
    User2MiniGroupInfoPtr cur_ptr = new User2MiniGroupInfo(memberId, ids);
    user2minigroupinfo_ptrs_.push_back(cur_ptr);
  } else {
    (*(user2minigroupinfo_ptrs_.end() - 1))->addJoinedMiniGroups(ids, true);
  }
  //MCE_INFO("[User2MiniGroupResultHandlerI::handle] user_id = " << (*(user2minigroupinfo_ptrs_.end() - 1))->userId 
  //    << " joined size = " << (*(user2minigroupinfo_ptrs_.end() - 1))->miniGroupIds.size());
  //MCE_INFO("[User2MiniGroupResultHandlerI::handle] user2MiniGroupPtrs.size = " << user2minigroupinfo_ptrs_.size());
  return true;
}

template<typename V>
template<typename T>
string DBUtil<V>::getTableName(T id, T mod, const string& prefix) {
  return prefix + "_" + boost::lexical_cast<string>(id % mod);
}

template<typename V>
template<typename T>
string DBUtil<V>::getIdsStr(const vector<T>& ids) {
  ostringstream result;
  result << "(";
  if(ids.size() > 0) {
    result << boost::lexical_cast<string>(ids[0]);
    for(size_t i = 1; i < ids.size(); ++i) {
      result << ", " << boost::lexical_cast<string>(ids[i]);
    }
  }
  result << ")";
  return result.str();
}

template<typename V>
template<typename T>
map<T, vector<T> > DBUtil<V>::seperateIdsByMod(int mod, const vector<T>& ids) {
  map<T, vector<T> > result;
  for(size_t i = 0; i < ids.size(); ++i) {
    typename map<T, vector<T> >::iterator foundIt = result.find(ids[i] % mod);
    if(foundIt == result.end()) {
      vector<T> modVector;
      modVector.push_back(ids[i]);
      result.insert(make_pair<T, vector<T> >(ids[i] % mod, modVector));
    } else {
      foundIt->second.push_back(ids[i]);
    }
  }
  return result;
}

void initDBUtil() {
  DBUtil<int>::getTableName<int>(0, 1, "test");
  vector<long> longIds;
  DBUtil<int>::getIdsStr(longIds);
  vector<int> intIds;
  DBUtil<int>::seperateIdsByMod<int>(100, intIds);
  DBUtil<int>::seperateIdsByMod<long>(100, longIds);
  DBUtil<int>::getIdsStr(intIds);
}

template<typename T>
BatchIdResultHandler<T>::BatchIdResultHandler(const string& idColumnName, vector<T>& ids) : _idColumnName(idColumnName), _ids(ids) {
} 

template<typename T>
bool BatchIdResultHandler<T>::handle(mysqlpp::Row& row) const {
  T id = (T)row[_idColumnName.c_str()];
  _ids.push_back(id);
  return true;
}

void initBatchResultHander() {
  vector<int> results;
  BatchIdResultHandler<int> handler("id", results);
}

template<typename T, typename TPtr, typename SPtr>
MyUtil::ObjectResultPtr xce::mngp::parse(const MyUtil::ObjectResultPtr& sourceData) {
  MyUtil::ObjectResultPtr result = new MyUtil::ObjectResult;
  for (map<long, Ice::ObjectPtr>::const_iterator it = sourceData->data.begin(); it != sourceData->data.end(); ++it) {
    result->data.insert(make_pair<long, TPtr>((*it).first, new T(SPtr::dynamicCast((*it).second))));
  }
  return result;
}

void initParse() {
  parse<MiniGroupInfo, MiniGroupInfoPtr, MiniGroupPtr>(0);
  parse<User2MiniGroupInfo, User2MiniGroupInfoPtr, User2MiniGroupPtr>(0);
}

MyUtil::ObjectResultPtr xce::mngp::parse(const MiniGroupSeq& miniGroups) {
  MyUtil::ObjectResultPtr result = new MyUtil::ObjectResult;
  for (vector<MiniGroupPtr>::const_iterator it = miniGroups.begin(); it != miniGroups.end(); ++it) {
    result->data.insert(make_pair<long, MiniGroupInfoPtr>((*it)->id, new MiniGroupInfo(*it)));
  }
  return result;
}

