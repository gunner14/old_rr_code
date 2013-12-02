#include "SecondDegreeFriendsI.h"
#include <cmath>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include "BuddyByIdReplicaAdapter.h"
#include <vector>
#include <map>
#include <set>
#include <queue>

using namespace std;
using namespace MyUtil;
using namespace xce::sdfriends;
using namespace xce::buddy::adapter;
using namespace xce::serverstate;

MyUtil::Int2IntMap SecondDegreeData::getSecondDegreeFriendsKMerge(Ice::Int userId, Ice::Int limit) {
  map<int, int> result;
  ostringstream os1;
  os1 << "KMerge:" << userId;
  TimeCost tc = TimeCost::create(os1.str(), 1);
  MCE_INFO("[SecondDegreeData::KMerge] userId:" << userId << " limit:" << limit);
  vector<int> friendIds = BuddyByIdCacheAdapter::instance().getFriendListAsc(userId, limit);
  tc.step("1st.Get friends of user");
  map<int, vector<int> > friendOfFriendIds = BuddyByIdCacheAdapter::instance().getFriendLists(friendIds);
  MCE_INFO("[SecondDegreeData::KMerge] friendIds.size:" << friendIds.size() << " friendOfFriendIds.size:"   << friendOfFriendIds.size());
  tc.step("2nd.Get second degree friends of user");
  _blockSortList.push_back(userId);
  _blockSortList.insert(_blockSortList.end(), friendIds.begin(), friendIds.end());
  sort(_blockSortList.begin(), _blockSortList.end());
  _findIter = _blockSortList.begin();

  merge(friendOfFriendIds, 10);
  tc.step("merge");
  if ((int)_bucket.size() != BUCKET_SIZE) {
    return result;
  }

  for (int i = BUCKET_SIZE - 1; i >= 0; --i) {
    if (!_bucket[i].empty()) {
      for (vector<CommonFriend>::iterator iter = _bucket[i].begin(); iter != _bucket[i].end(); ++iter) {
        if (result.size() >= BUCKET_SIZE) {
          break;
        } else {
          result[(*iter).userId] = (*iter).commonNum;
        }
      }
    }
  }

  ostringstream os2;
  os2 << "result.size:" << result.size();
  tc.step(os2.str());
  return result;

}

void SecondDegreeData::merge(const map<int, vector<int> >& friendOfFriendIds, int weight) {
  _bucket.resize(BUCKET_SIZE);
  priority_queue<HeapItem, vector<HeapItem>, IdAscComp> fliterHeap;

  for (map<int, vector<int> >::const_iterator iter = friendOfFriendIds.begin(); iter != friendOfFriendIds.end(); ++iter) {
    if (iter->second.empty()) {
      continue;
    }
    HeapItem item;
    item.hostId = iter->first;
    item.weight = weight;
    item.beginIter = iter->second.begin();
    item.endIter = iter->second.end();
    fliterHeap.push(item);
  }
  vector<HeapItem> resultList;
  while (!fliterHeap.empty()) {
    HeapItem item = fliterHeap.top();

    if (resultList.empty()) {
      resultList.push_back(item);
    } else {
      if (*resultList.back().beginIter == *item.beginIter) {
        resultList.push_back(item);
      } else {
        flushSortVector(resultList);
        resultList.push_back(item);
      }
    }
    fliterHeap.pop();

    if (++item.beginIter != item.endIter) {
      fliterHeap.push(item);
    }
  }

  if (!resultList.empty()) {
    flushSortVector(resultList);
  }
}

void SecondDegreeData::flushSortVector(std::vector<HeapItem>& sortVec) {
  if (sortVec.empty() || isBlock(*(sortVec.front().beginIter))) {
    sortVec.clear();
    return;
  }

  CommonFriend comm;
  vector<int> tmpVec;
  for (vector<HeapItem>::iterator iter = sortVec.begin();
      iter != sortVec.end(); ++iter) {
    comm.userId = *iter->beginIter;
    comm.weight += iter->weight;
    tmpVec.push_back(iter->hostId);
  }
  comm.commonNum= (int)tmpVec.size();
  insertBucket(comm);
  sortVec.clear();
}

void SecondDegreeData::insertBucket(const CommonFriend& comm) {
  int index = comm.weight / STEP;
  if (index < 0) {
    return;
  }
  if (index > BUCKET_SIZE) {
    _bucket[BUCKET_SIZE - 1].push_back(comm);
  } else {
    _bucket[index].push_back(comm);
  }
}

bool SecondDegreeData::isBlock(int userid) {
  if (_blockSortList.empty()) {
    return false;
  }

  while (_findIter != _blockSortList.end()) {
    if (*_findIter < userid) {
      ++_findIter;
    } else if (*_findIter == userid) {
      return true;
    } else {
      break;
    }
  }

  return false;
}

