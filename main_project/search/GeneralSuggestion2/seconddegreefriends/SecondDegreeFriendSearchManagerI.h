#ifndef __SEARCH_MANAGER_H__
#define __SEARCH_MANAGER_H__

#include <GeneralSuggestion.h>
#include "IceExt/src/ServiceI.h"
#include "util/cpp/TimeCost.h"
#include "UtilCxx/src/TaskManager.h"
#include "DbCxxPool/src/QueryRunner.h"
#include <IceUtil/IceUtil.h>
#include <algorithm>
#include "search/GeneralSuggestion2/util/SearchUtil.h"
#include "SecondDegreeFriendSearchDataI.h"

namespace xce {
  namespace seconddegreefriendsearch {

    using namespace MyUtil;
    using namespace xce::search;
    using namespace std;
    using namespace xce::suggestiondata;
    using namespace xce::generalsuggestion;

    const size_t maxSize = 300;

    class SecondDegreeFriendSearchManagerI : public SearchManager, public MyUtil::Singleton<SecondDegreeFriendSearchManagerI> {		
      public:
        SecondDegreeFriendSearchManagerI() : dynamic_cache_(maxSize) {};
        virtual ~SecondDegreeFriendSearchManagerI(){};
        virtual SuggestionResultPtr search(int,const string&,int begin, int limit, const StrSeq&, const Ice::Current& crt=Ice::Current());
        virtual void load(int,const IndexItemSeq&, const MyUtil::Str2IntMap& config,const Ice::Current& crt=Ice::Current());
        void load(string type, int id);
        void reload(string type, int id);
        set<string> newSet;
        void initialize();

        SearchDataIPtr getCachedData(string key) {
          return dynamic_cache_.locate(key, 0);
        }

        void addDynamicData(string key, SearchDataIPtr sp) {
          dynamic_cache_.add(sp, key);
        }

      private:
        SizedEvictor<string,SearchDataIPtr> dynamic_cache_;
        friend class MyUtil::Singleton<SecondDegreeFriendSearchManagerI>;
        map<string,vector<string> > dict_;

    };
    typedef IceUtil::Handle<SecondDegreeFriendSearchManagerI> SecondDegreeFriendSearchManagerIPtr;

    class SecondDegreeFriendLoadThread : public virtual IceUtil::Thread {
      private:
        int id_;
        string type_;
      public:
        SecondDegreeFriendLoadThread(string type, int id) : id_(id), type_(type) {
        }

        void run() {
          ostringstream os;
          os << "Update " << type_ << " ID: " << id_;
          TimeCost tc = TimeCost::create(os.str(), 1);
          //string key = StrUtil::getKey(type_, id_);
          string key = SecondDegreeFriendSearchDataIUtil::getKey(type_, id_);

          SearchDataIPtr sp = NULL;
          SearchDataIPtr oldSp = SecondDegreeFriendSearchManagerI::instance().getCachedData(key);

          // 动态数据有可能被逐出
          if (oldSp) {
            sp = oldSp->clone();
          } else if (SecondDegreeFriendSearchDataIUtil::isDynamicData(type_)) {
            sp = SecondDegreeFriendSearchDataIUtil::getSearchDataIPtr(type_);
          } else {
            MCE_INFO("Object Not Exist In Cache For " << type_);
            return;
          }
          sp->load(id_);
          if (SecondDegreeFriendSearchDataIUtil::isDynamicData(type_)) {
            SecondDegreeFriendSearchManagerI::instance().addDynamicData(key, sp);
          }
          tc.step("Time Usage: ");
          return;
        }
    };
    typedef IceUtil::Handle<SecondDegreeFriendLoadThread> SecondDegreeFriendLoadThreadPtr;
  };
};
#endif
