#ifndef __SECOND_DEGREE_FRIEND_SEARCH_DATA_H__
#define __SECOND_DEGREE_FRIEND_SEARCH_DATA_H__ 

#include "search/GeneralSuggestion2/util/SearchDataI.h"

namespace xce {
  namespace suggestiondata {

    //const static int SECOND_DEGREE_FRIEND_DATA_TIMEOUT = 60 * 60;
    const static int SECOND_DEGREE_FRIEND_DATA_TIMEOUT = 60 * 60 * 12;
    const static string SECOND_DEGREE_FRIEND = "second_degree_friend_name";

    class SecondDegreeFriendSearchDataI : public SearchDataI {
      public:
        SecondDegreeFriendSearchDataI() : _maxSize(300), _limit(2000) {}
        void load(int);
        void load();
        SearchDataIPtr clone() {
          return new SecondDegreeFriendSearchDataI;
        }
      private:
        size_t _maxSize; //分次取好友姓名时，每次取的最大值
        Ice::Int _limit; //取二度好友时的个数最大限制
        void getFriendsName(vector<int>& sdFriendIds, map<int,string>& sdFriendLists);
    };

    class SecondDegreeFriendSearchDataIUtil {
      public:
        static SearchDataIPtr getSearchDataIPtr(string type) {
          SearchDataIPtr p = NULL;
          if (type == SECOND_DEGREE_FRIEND) {
            p = new SecondDegreeFriendSearchDataI;
          }
          return p;
        }

        static bool isDynamicData(string type) {
          return (SECOND_DEGREE_FRIEND == type);
        }

        static string getKey(string type, int userid) {
          string key = type;
          if (SECOND_DEGREE_FRIEND == type) {
            ostringstream oss;
            oss<< userid << "_" << type;
            key = oss.str();
          }
          return key;
        }

    };
  }
}



#endif
