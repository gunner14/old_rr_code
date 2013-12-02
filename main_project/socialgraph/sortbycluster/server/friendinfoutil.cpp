#include "friendinfoutil.h"
#include <BuddyByIdReplicaAdapter.h>
#include <FriendFinderReplicaAdapter.h>
#include <FriendFinderInfoCacheReplicaAdapter.h>
#include <FriendFinderByIPReplicaAdapter.h>
#include <PhotoStateFilterAdapter.h>
#include <OnlineStateFilterAdapter.h>
#include <StatusCacheAdapter.h>
#include <UserStateFilterAdapter.h>
#include <sstream>

using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::xce;

using namespace com::xiaonei::service;

using namespace xce::buddy::adapter;

using namespace xce::photostatefilter::adapter;
using namespace xce::onlinestatefilter::adapter;
using namespace xce::userstatefilter::adapter;
using namespace xce::statuscache;
using namespace xce::socialgraph;


map<int,bool> FriendInfoUtil::getPhotoStateBatch(const MyUtil::IntSeq& ids){
        try {
                return PhotoStateFilterAdapter::instance().getPhotoStateBatch( ids );
        } catch( Ice::Exception e ){
                MCE_WARN("[FriendInfoUtil::getPhotoStateBatch] ids.size()=" << ids.size() << " found Ice::Exception : " << e.what() );
        } catch( ... ){
                MCE_WARN("[FriendInfoUtil::getPhotoStateBatch] ids.size()=" << ids.size() << " found unknown exception" );
        }
	map<int,bool> res;
	for( MyUtil::IntSeq::const_iterator it = ids.begin(); it != ids.end(); ++it ){
		res[*it] = true;
	}
	return res;
}

map<int,bool> FriendInfoUtil::getOnlineStateBatch(const MyUtil::IntSeq& ids){
        try {
                return OnlineStateFilterAdapter::instance().getOnlineStateBatch( ids );
        } catch( Ice::Exception e ){
                MCE_WARN("[FriendInfoUtil::getOnlineStateBatch] ids.size()=" << ids.size() << " found Ice::Exception : " << e.what() );
        } catch( ... ){
                MCE_WARN("[FriendInfoUtil::getOnlineStateBatch] ids.size()=" << ids.size() << " found unknown exception" );
        }
	map<int,bool> res;
	for( MyUtil::IntSeq::const_iterator it = ids.begin(); it != ids.end(); ++it ){
		res[*it] = false;
	}
	return res;
}

map<int,bool> FriendInfoUtil::getStateStarBatch(const MyUtil::IntSeq& ids){
        try {
                return UserStateFilterAdapter::instance().getStateStarBatch( ids );
        } catch( Ice::Exception e ){
                MCE_WARN("[FriendInfoUtil::getStateStarBatch] ids.size()=" << ids.size() << " found Ice::Exception : " << e.what() );
        } catch( ... ){
                MCE_WARN("[FriendInfoUtil::getStateStarBatch] ids.size()=" << ids.size() << " found unknown exception" );
        }
	map<int,bool> res;
	for( MyUtil::IntSeq::const_iterator it = ids.begin(); it != ids.end(); ++it ){
		res[*it] = true;
	}
	return res;
}

map<int,string> FriendInfoUtil::getLoginDayState(const MyUtil::IntSeq& ids) {

  map<int, string> userLoginDaysMap;
  if(ids.size() == 0) {
    return userLoginDaysMap;
  }

  Statement sql;
  sql << "SELECT user_id, login_record FROM login_record_count WHERE user_id in (";
  
  for(MyUtil::IntSeq::const_iterator iter = ids.begin(); iter != ids.end(); iter++) {
    sql << *iter;
    if (iter != ids.end() - 1) {
      sql << ", ";
    }
  }
  sql <<")";
  
  try {
    QueryRunner("reg_invite", CDbRServer).query(sql, UserLoginDaysHandler(userLoginDaysMap));
  } catch (Ice::Exception& e) {
    MCE_WARN("[FriendInfoUtil::getLoginDayState] ids.size()=" << ids.size() << " found Ice::Exception : " << e.what() );
  } catch ( ... ) {
    MCE_WARN("[FriendInfoUtil::getLoginDayState] ids.size()=" << ids.size() << " found unknown exception" );
  }
  return userLoginDaysMap;
}

/********************************************************************/


bool UserLoginDaysHandler::handle(mysqlpp::Row& row) const {

    int userId = (int)row["user_id"];
    userLoginDaysMap_[userId] = string(row["login_record"]);
    return true;
}
 
