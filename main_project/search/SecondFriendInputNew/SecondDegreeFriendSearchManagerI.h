#ifndef __SECONDDEGREE_MANAGER_H__
#define __SECONDDEGREE_MANAGER_H__

#include "SecondDegreeFriendSearchDataI.h"

namespace xce {
  namespace seconddegreefriendsearch {

    using namespace com::renren::secondfrienddata;

    class SecondDegreeFriendSearchManagerI : public SearchManager, public MyUtil::Singleton<SecondDegreeFriendSearchManagerI> {		
      public:
        SecondDegreeFriendSearchManagerI();
        virtual ~SecondDegreeFriendSearchManagerI(){MCE_INFO("~SecondDegreeFriendSearchManagerI");};
        virtual SuggestionResultPtr search(int,const string&,int begin, int limit, const xce::generalsuggestion::StrSeq&, const Ice::Current& crt=Ice::Current());
        virtual void load(int,const IndexItemSeq&, const MyUtil::Str2IntMap& config,const Ice::Current& crt=Ice::Current());//兼容老接口
        void init(string& zk,string& loaderpath,int dataNum);

       const int SECONDFRIENDSUGGEST_DATA;
      private:
        void initComNum(int uid);
        void createObject(int hostID,SecondDegreeFriendSearchDataIPtr&);

        const string SECOND_DEGREE_FRIEND;
        const string DB_FRIENDINPUT_PINYIN;
        const int TIMEOUT_CACHE;
        RelationLoaderAdapter adapter;
        std::tr1::unordered_map<string,vector<string> > dict_;
        string zk_;               
        string loaderpath_;
        int dataNum_;
        map<int,short> id2ComNum;

        friend class MyUtil::Singleton<SecondDegreeFriendSearchManagerI>;
    };
    typedef IceUtil::Handle<SecondDegreeFriendSearchManagerI> SecondDegreeFriendSearchManagerIPtr;

  };
};
#endif
