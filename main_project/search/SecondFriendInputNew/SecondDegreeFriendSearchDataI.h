#ifndef __SECONDFRIEND_SUGGEST_H__
#define __SECONDFRIEND_SUGGEST_H__

#include <GeneralSuggestion.h>
#include "UtilCxx/src/TaskManager.h"
#include <IceUtil/IceUtil.h>
#include "../FriendSuggestUtil/FriendSuggestUtil.h"

namespace com {
namespace renren {
namespace secondfrienddata {

  using namespace std;
  using namespace xce::generalsuggestion;
  using namespace com::xiaonei::friendsuggest;

  class SecondDegreeFriendSearchDataI : virtual public SecondDegreeFriendSearchData  {
    public:
      SecondDegreeFriendSearchDataI(int id,std::tr1::unordered_map<string,vector<string> >&,RelationLoaderAdapter&);
      virtual ~SecondDegreeFriendSearchDataI();
      void buildIndex(int dataNum);
      std::vector<int> search(const string& query,int limit);
      time_t getTime();
    
    private:
      const size_t maxSize;
      RelationLoaderAdapter adapter_;
      QuerySuggestion *qs;
      map<int,string> id2Name;
      int id_;
      time_t old_;
};
typedef IceUtil::Handle<SecondDegreeFriendSearchDataI> SecondDegreeFriendSearchDataIPtr;
};
};
};
#endif
