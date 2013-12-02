#include "FriendInputDataI.h"
#include "BuddyByIdReplicaAdapter.h"
#include <cmath>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

using namespace xce::serverstate;
using namespace xce::buddy::adapter;
using namespace com::renren::finputdata;

//==========FriendInputDataI=================
FriendInputDataI::FriendInputDataI(int id,std::tr1::unordered_map<string,vector<string> >& dict_, 
    RelationLoaderAdapter& adapter):id_(id),adapter_(adapter),maxSize(300) {
  qs = new QuerySuggestion(dict_,false);
}

FriendInputDataI::~FriendInputDataI() {
  delete qs;
}

void FriendInputDataI::buildIndex() {
  FriendSuggestUtil::buildIndex(adapter_, id_, id2Name, qs, 1, maxSize, INT_MAX);
  old_ = time(NULL);
}


MyUtil::IntSeq FriendInputDataI::search(const string& query,Ice::Int limit) {
  map<long,float> dynamicScore;
  string noQuery(query);
  vector<long> res = qs->search(noQuery,limit,false,dynamicScore);

  vector<int> resInt;
  if(res.size() == 0) {
    return resInt;
  }
  for(vector<long>::iterator it = res.begin();it != res.end();it++) {
    resInt.push_back((int)*it);
  }
  return resInt;
}

time_t FriendInputDataI::getTime() {
  return old_;
}
