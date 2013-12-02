#ifndef __FRIEND_SUGGEST_UTIL_H__
#define __FRIEND_SUGGEST_UTIL_H__
#include "IceExt/src/ServiceI.h"
#include "DbCxxPool/src/QueryRunner.h"
#include "UserCacheAdapter.h"
#include "search/GeneralInputSuggestion/trunk/src/server-src/QuerySuggestion.h"
#include "../SearchRelation/SearchRelationCxxAdapter/src/LoaderAdapter.h"

using namespace MyUtil;
using namespace xce::usercache;
using namespace com::xiaonei::xce;
using namespace search::relation;

namespace com {
namespace xiaonei {
namespace friendsuggest {

  class FriendSuggestUtil {
    public:
      static void generateNameMap(vector<int>& fids, map<int,string>& id2Name, int maxSize); 
      static void buildIndex(RelationLoaderAdapter& adapter_, int id_, map<int,string>& id2Name, QuerySuggestion *& qs, int tag, int maxSize,int dataNum);
      static int getStrLen(const string& query);
  };

  class PinYinResultHandler : virtual public com::xiaonei::xce::ResultHandler,
  public MyUtil::Singleton<PinYinResultHandler> {
    public:
      PinYinResultHandler(std::tr1::unordered_map<string,vector<string> >& v_dict):dict(v_dict){}
    protected:
      virtual bool handle(mysqlpp::Row& res) const;
    private:
      std::tr1::unordered_map<string,vector<string> >& dict;
  }; 

};
};
};
#endif
