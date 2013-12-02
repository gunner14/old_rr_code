#ifndef __FRIENDINPUTDATAI_H__
#define __FRIENDINPUTDATAI_H__

#include <FriendInput.h>
#include "UtilCxx/src/TaskManager.h"
#include <IceUtil/IceUtil.h>
#include "../FriendSuggestUtil/FriendSuggestUtil.h"

namespace com {
namespace renren {
namespace finputdata {

  using namespace std;
  using namespace com::xiaonei::friendsuggest;

class FriendInputDataI : virtual public FriendInputData {
  public:
    FriendInputDataI(int id,std::tr1::unordered_map<string,vector<string> >&,RelationLoaderAdapter&);
    virtual ~FriendInputDataI();
    void buildIndex();
    vector<int> search(const string& query,int limit);
    time_t getTime();//FriendInputManagerI中调用

  private:
    QuerySuggestion *qs;
    map<int,string> id2Name;
    int id_;
    time_t old_;
    RelationLoaderAdapter adapter_;
    const size_t maxSize;
};
typedef IceUtil::Handle<FriendInputDataI> FriendInputDataIPtr;

};
};
};

#endif
