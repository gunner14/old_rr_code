/*************************************************************************
    > File Name: AssociationRuleUtil.cc
    > Author: zunwen.you
    > Mail: zunwen.you@renren-inc.com 
    > Created Time: 2012年09月25日 星期二 18时33分03秒
 ************************************************************************/
#include "AssociationRuleUtil.h" 

#include <boost/foreach.hpp>
#include <AssociationRule.h>
#include <BuddyByIdReplicaAdapter.h>
#include <FriendCountCacheReplicaAdapter.h>
#include <FriendFinderReplicaAdapter.h>
#include <FriendFinderInfoCacheReplicaAdapter.h>
#include <FriendFinderByIPReplicaAdapter.h>
#include <PhotoStateFilterAdapter.h>
#include <OnlineStateFilterAdapter.h>
#include <StatusCacheAdapter.h>
#include <UserStateFilterAdapter.h>
#include <FriendRankCacheNewAdapter.h>
#include <BuddyByApplicantAdapter.h>
#include <BuddyCoreAdapter.h>
#include <SocialGraphRcdTripodAdapter.h>

#include "item.pb.h"
#include "tripod2/cc/core/tripod_client.h"

using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace com::xiaonei::service;
using namespace xce::friendfinder;
using namespace xce::buddy;
using namespace xce::buddy::adapter;
using namespace xce::friendfinder::adapter;
using namespace xce::photostatefilter::adapter;
using namespace xce::onlinestatefilter::adapter;
using namespace xce::userstatefilter::adapter;
using namespace xce::statuscache;
using namespace xce::socialgraph;

friendDataSeq AssociationRuleUtil::getFoFRecommend(int userId, int limit) {
  friendDataSeq ret;
  try {
    string key = boost::lexical_cast<string>(userId);
    string value;
    SocialGraphRcdTripodAdapter::instance().Get(key, &value);

    vector<char> fof_raw_data(value.begin(), value.end());
    Items fof_raw_items;
    if(fof_raw_items.ParseFromArray(fof_raw_data.data(), fof_raw_data.size())) {
      BOOST_FOREACH(const Item &it, fof_raw_items.items()) {
        if(it.message() == "FOF") {
          FriendData rcd;
          rcd.userID = it.id();
          rcd.weight = boost::lexical_cast<float>(it.messages(0));
          ret.push_back(rcd);
        }
      }
    }
  }
	catch(Ice::Exception& e)
	{
		MCE_WARN("[AssociationRuleUtil::getFoFRecommend] userId: "<< userId << " getFoFComm IceException" << e);
	}
	catch(std::exception& e)
	{
		MCE_WARN("[AssociationRuleUtil::getFoFRecommend] userId: "<< userId << "getFoFComm StdException" << e.what());
	}
	catch(...)
	{
		MCE_WARN("[AssociationRuleUtil::getFoFRecommend] userId: "<< userId << "getFoFComm unknown exception");
	}
  if (ret.size() > limit) {
    ret.resize(limit);
  }
  return ret;
}

RcdInfoSeq AssociationRuleUtil::getFoFRcd(int userId, int limit) {
  RcdInfoSeq ret;
  try {
    string key = boost::lexical_cast<string>(userId);
    string value;
    SocialGraphRcdTripodAdapter::instance().Get(key, &value);

    vector<char> fof_raw_data(value.begin(), value.end());
    Items fof_raw_items;
    if(fof_raw_items.ParseFromArray(fof_raw_data.data(), fof_raw_data.size())) {
      BOOST_FOREACH(const Item &it, fof_raw_items.items()) {
        if(it.message() == "FOF") {
          RcdInfo rcd;
          rcd.userID = it.id();
          rcd.weight = boost::lexical_cast<float>(it.messages(0));
          rcd.type = it.message();
          ret.push_back(rcd);
        }
      }
    }
  }
	catch(Ice::Exception& e)
	{
		MCE_WARN("[AssociationRuleUtil::getFoFRcd] userId: "<< userId << " getFoFComm IceException" << e);
	}
	catch(std::exception& e)
	{
		MCE_WARN("[AssociationRuleUtil::getFoFRcd] userId: "<< userId << "getFoFComm StdException" << e.what());
	}
	catch(...)
	{
		MCE_WARN("[AssociationRuleUtil::getFoFRcd] userId: "<< userId << "getFoFComm unknown exception");
	}
  if (ret.size() > limit) {
    ret.resize(limit);
  }
  return ret;
}


MyUtil::IntSeq AssociationRuleUtil::getFriendSeq(int userId) {
        MyUtil::IntSeq buddies;
        try {
                buddies = BuddyByIdCacheAdapter::instance().getFriendListAsc( userId, -1 );
        } catch( Ice::Exception e ){
                MCE_WARN("[AssociationRuleUtil::getFriendSeq] userId=" << userId << " found Ice::Exception : " << e.what() );
        } catch( ... ){
                MCE_WARN("[AssociationRuleUtil::getFriendSeq] userId=" << userId << " found Unknown::Exception" );
        }
	return buddies;
}

MyUtil::Int2IntSeqMap AssociationRuleUtil::getFriendSeqBatch(const MyUtil::IntSeq& ids) {
        Int2IntSeqMap result;
        if (ids.empty()) {
                return result;
        }
        return BuddyByIdCacheAdapter::instance().getFriendLists(ids);
}

MyUtil::IntSeq AssociationRuleUtil::getBlockSeqByCache(int userId) {
  MyUtil::IntSeq blockSeq;
  try {
    blockSeq = com::xiaonei::service::buddy::BuddyCoreAdapter::instance().getBlockSeq(userId, 0, -1);
  } catch( Ice::Exception e ){
    MCE_WARN("[AssociationRuleUtil::getBlockSeqByCache] userId=" << userId << " found Ice::Exception : " << e.what() );
  } catch( ... ){
    MCE_WARN("[AssociationRuleUtil::getBlockSeqByCache] userId=" << userId << " found Unknown::Exception" );
  }
  return blockSeq;
}

MyUtil::IntSeq AssociationRuleUtil::getBlockSeq(int id) {
        IntSeq result;
        if (id<=0) {
                return result;
        }
        Statement oss;
        oss << "SELECT guest FROM relation_block  WHERE host = " << id;
        BlockResultHandler rsh(result, "guest");
        QueryRunner("friendfinder_block", CDbRServer).query(oss, rsh);
        return result;
}

MyUtil::IntSeq AssociationRuleUtil::getApplySeq(int userId) {
	IntSeq result;
	if( userId <= 0 ) {
		return result;
	}

	ostringstream tableName;
	tableName << "buddyapply_applicant_" << (userId % 100);	

	Statement oss;
	oss << "SELECT accepter FROM " << tableName.str() << " WHERE applicant=" << userId << " limit 100";
	QueryRunner("offerfriends_apply", CDbRServer).query( oss, ApplyResultHandler(result) );
	return result;
}

MyUtil::IntSeq AssociationRuleUtil::getApplySeqByCache(int userId) {
	IntSeq result;
	if( userId <= 0 ) {
		return result;
	}
  int limit = 1000;
  BuddyByApplicantDataPtr app = BuddyByApplicantCacheAdapter::instance()
    .getAccepters(userId, 0, limit); 
  BOOST_FOREACH (AccepterInfo one, app->accepters) {
    result.push_back(one.accepter);
  }
	return result;
}

MyUtil::IntSeq AssociationRuleUtil::getFriendRank(int userId) {
  IntSeq result;
	if( userId <= 0 ) {
		return result;
	}
  float factor = 0.7 / 0.3;
  RankSeqIntNew friendRank = FriendRankCacheAdapter::instance().GetRankWithFactor(userId, factor);  
  BOOST_FOREACH(RankDataInt i, friendRank) {
    result.push_back(i.fid);
  }
  return result;
}

map<int,bool> AssociationRuleUtil::getPhotoStateBatch(const MyUtil::IntSeq& ids){
        try {
                return PhotoStateFilterAdapter::instance().getPhotoStateBatch( ids );
        } catch( Ice::Exception e ){
                MCE_WARN("[AssociationRuleUtil::getPhotoStateBatch] ids.size()=" << ids.size() << " found Ice::Exception : " << e.what() );
        } catch( ... ){
                MCE_WARN("[AssociationRuleUtil::getPhotoStateBatch] ids.size()=" << ids.size() << " found unknown exception" );
        }
	map<int,bool> res;
	for( MyUtil::IntSeq::const_iterator it = ids.begin(); it != ids.end(); ++it ){
		res[*it] = true;
	}
	return res;
}

map<int,bool> AssociationRuleUtil::getOnlineStateBatch(const MyUtil::IntSeq& ids){
        try {
                return OnlineStateFilterAdapter::instance().getOnlineStateBatch( ids );
        } catch( Ice::Exception e ){
                MCE_WARN("[AssociationRuleUtil::getOnlineStateBatch] ids.size()=" << ids.size() << " found Ice::Exception : " << e.what() );
        } catch( ... ){
                MCE_WARN("[AssociationRuleUtil::getOnlineStateBatch] ids.size()=" << ids.size() << " found unknown exception" );
        }
	map<int,bool> res;
	for( MyUtil::IntSeq::const_iterator it = ids.begin(); it != ids.end(); ++it ){
		res[*it] = false;
	}
	return res;
}

map<int,bool> AssociationRuleUtil::getStatusBatch(const MyUtil::IntSeq& ids){
	try {
  	return StatusCacheAdapter::instance().getStatusBatch(ids );
	} catch( Ice::Exception e ){
	  MCE_WARN("[AssociationRuleUtil::getStatusBatch] ids.size()=" << ids.size() << " found Ice::Exception : " << e.what() );
	} catch( ... ){
	  MCE_WARN("[AssociationRuleUtil::getStatusBatch] ids.size()=" << ids.size() << " found unknown exception" );
	}
	map<int, bool> res;
	for (MyUtil::IntSeq::const_iterator it = ids.begin(); 
			it != ids.end(); ++it ){
		res[*it] = false;
	}
	return res;
}

map<int,bool> AssociationRuleUtil::getStateStarBatch(const MyUtil::IntSeq& ids){
        try {
                return UserStateFilterAdapter::instance().getStateStarBatch( ids );
        } catch( Ice::Exception e ){
                MCE_WARN("[AssociationRuleUtil::getStateStarBatch] ids.size()=" << ids.size() << " found Ice::Exception : " << e.what() );
        } catch( ... ){
                MCE_WARN("[AssociationRuleUtil::getStateStarBatch] ids.size()=" << ids.size() << " found unknown exception" );
        }
	map<int,bool> res;
	for( MyUtil::IntSeq::const_iterator it = ids.begin(); it != ids.end(); ++it ){
		res[*it] = true;
	}
	return res;
}

ApplyResultHandler::~ApplyResultHandler() {
}

bool ApplyResultHandler::handle(mysqlpp::Row& row) const {
	int friendId = (int)row["accepter"];
	_result.push_back( friendId );
	return true;
} 

BlockResultHandler::BlockResultHandler(MyUtil::IntSeq& results, const std::string& toField) :
        _results(results), _toField(toField) {
}

BlockResultHandler::~BlockResultHandler() {
}

bool BlockResultHandler::handle(mysqlpp::Row& row) const {
        int to = (int)row[_toField.c_str()];
        _results.push_back(to);
        return true;
}

