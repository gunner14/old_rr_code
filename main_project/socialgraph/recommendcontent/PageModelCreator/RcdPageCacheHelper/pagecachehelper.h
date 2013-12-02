/*
  Author: yize.tan@renren-inc.com
  Date: 2012-11-27
  Description:build page recommendation data into cache
*/

#include <set>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <sstream>
#include "OceCxxAdapter/src/BuddyByIdReplicaAdapter.h"
#include "base/logging.h"
#include "base/ptime.h"
#include "socialgraph/recommendcontent/PageModelCreator/RcdPageDbHelper/rcdpagedbhelper.h"
#include "socialgraph/socialgraphproto/item.pb.h"
#include "OceCxxAdapter/src/SocialGraphRcdPageCfTripodAdapter.h"
#include "QueryRunner.h"
#include <boost/lexical_cast.hpp>
#include "TaskManager.h"
#include "socialgraph/socialgraphutil/mcclient.h"
#include "socialgraph/socialgraphutil/algostuff.h"
#include "socialgraph/socialgraphutil/clogging.h"

namespace xce {
namespace socialgraph {

class RcdPageCacheHelper {
public:
  static bool setRcdPageIntoCache(int user_id, std::set<int>& seq);
  static void getUserFriendPageList(int user_id, std::vector<int>& user_friend, std::map<int, std::set<int> >& friend_page);
  static void filterRcdPage(int user_id, std::set<int>& seq);
  static bool getRcdPageFromCache(int user_id, std::vector<int>& seq);
};

};
};
