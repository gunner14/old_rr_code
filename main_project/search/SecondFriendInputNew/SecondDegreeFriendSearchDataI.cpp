#include "BuddyByIdReplicaAdapter.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "SecondDegreeFriendSearchDataI.h"

using namespace xce::serverstate;
using namespace com::renren::secondfrienddata;
using namespace xce::buddy::adapter;

//===========SecondDegreeFriendSearchDataI============
SecondDegreeFriendSearchDataI::SecondDegreeFriendSearchDataI(int id,std::tr1::unordered_map<string,vector<string> >& dict_,
        RelationLoaderAdapter& adapter):id_(id),adapter_(adapter),maxSize(300) {
  qs = new QuerySuggestion(dict_,false);
}

SecondDegreeFriendSearchDataI::~SecondDegreeFriendSearchDataI() {
  delete qs;
}

void SecondDegreeFriendSearchDataI::buildIndex(int dataNum) {
  FriendSuggestUtil::buildIndex(adapter_, id_, id2Name, qs, 2, maxSize,dataNum);
  old_ = time(NULL);
}

MyUtil::IntSeq SecondDegreeFriendSearchDataI::search(const string& query,int limit)
{
  map<long,float> dynamicScore;
  string noQuery = query;
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

time_t SecondDegreeFriendSearchDataI::getTime() {
  return old_; 
}
