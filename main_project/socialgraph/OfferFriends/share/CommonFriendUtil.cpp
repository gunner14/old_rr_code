#include "CommonFriendUtil.h"
#include "OceCxxAdapter/src/SocialGraphRcdTripodAdapter.h"
#include "item.pb.h"
#include "tripod2/cc/core/tripod_client.h"

#include <boost/lexical_cast.hpp>
#include <limits>

using namespace std;
using namespace MyUtil;
using namespace ::com::xiaonei::xce;
using namespace ::com::xiaonei::service;
using namespace ::xce::offerfriends;
using namespace ::xce::tripod;
using namespace ::xce::socialgraph;

void CommonFriendCalculator::buildWeight(const MyUtil::Int2IntSeqMap& users, int weight) {
	try
	{
		for(MyUtil::Int2IntSeqMap::const_iterator i=users.begin();
				i!=users.end();
				++i)
		{
			for(MyUtil::IntSeq::const_iterator ui=i->second.begin();ui!=i->second.end();++ui)
			{
				_weightCount[*ui] += weight;
				MyUtil::Int2IntSeqMap::iterator sIt = _shareCount.find(*ui);
				if(sIt == _shareCount.end())
				{
					MyUtil::IntSeq firstShares;
					firstShares.push_back( i->first );
					_shareCount.insert(make_pair(*ui, firstShares));
				}
				else
				{
					sIt->second.push_back( i->first );
				}
			}
		}
	}
	catch(Ice::Exception& e)
	{
		MCE_WARN("buildUser IceException " << _userId << e.what());
	}
	catch(std::exception& e)
	{
		MCE_WARN("buildUser stdException " << _userId << e.what());
	}
	catch(...)
	{
		MCE_WARN("buildUser exception " << _userId);
	}
}

void CommonFriendCalculator::fetchData() {
	// initializing
	if (_friendList.empty() && _applyList.empty()){
		return;
	}

	// remove myself
	_weightCount[_userId]=numeric_limits<int>::min();

	// build weight
	buildWeight(_ff, 100);
	buildWeight(_af, 70);

	// remove blocks
	for (MyUtil::IntSeq::const_iterator i=_blockList.begin(); i!=_blockList.end(); ++i) {
		_weightCount[*i]=numeric_limits<int>::min();
	}
	for (MyUtil::IntSeq::const_iterator i=_commBlockList.begin(); i!=_commBlockList.end(); ++i) {
		_weightCount[*i]=numeric_limits<int>::min();
	}
	//将已经是该用户的好友对应的权值设置为最小
	for(MyUtil::IntSeq::const_iterator i=_friendList.begin(); i!=_friendList.end(); ++i){
		_weightCount[(*i)] = numeric_limits<int>::min();
	}
	//将该用户已经发过申请的人权值设置为最小
	for(MyUtil::IntSeq::const_iterator i=_applyList.begin(); i!=_applyList.end(); ++i){
		_weightCount[(*i)] = numeric_limits<int>::min();
	}
}

CommonFriendSeq CommonFriendCalculator::getComm(int limit) {
	fetchData();
	try
	{
		//std::vector<CommonFriend> ret;
		CommonFriend cf;
		for(CommonFriendCalculator::weightIter i=_weightCount.begin();i!=_weightCount.end();++i)
		{
			if(i->second<0)
			{
				continue;
			}
			cf.userId=i->first;
			cf.weight=i->second;
			MyUtil::Int2IntSeqMap::iterator shareIt = _shareCount.find(i->first);
			if(shareIt != _shareCount.end())
			{
				MyUtil::IntSeq(shareIt->second).swap(cf.shares);
			}
			else
			{
				cf.shares = MyUtil::IntSeq();
			}
			_ret.push_back(cf);
		}
	}
	catch(Ice::Exception& e)
	{
		MCE_WARN("getComm IceException" << e);
	}
	catch(std::exception& e)
	{
		MCE_WARN("getComm StdException" << e.what());
	}
	catch(...)
	{
		MCE_WARN("getComm unknown exception");
	}

	sort(_ret.begin(), _ret.end(), CommonFriendWeight);
        return ( _ret.end() - _ret.begin() > limit ) ? CommonFriendSeq( _ret.begin(), _ret.begin()+limit ) : CommonFriendSeq( _ret );
}


CommonFriendSeq CommonFriendCalculator::getFoFComm(int limit) {
    
try {
  string key = boost::lexical_cast<string>(_userId);
  string value;
  SocialGraphRcdTripodAdapter::instance().Get(key, &value);

  vector<char> fof_raw_data(value.begin(), value.end());
  Items fof_raw_items;
  if(fof_raw_items.ParseFromArray(fof_raw_data.data(), fof_raw_data.size())) {
    BOOST_FOREACH(const Item &it, fof_raw_items.items()) {
      if(it.message() == "FOF") {
	      CommonFriend comFriend;
        comFriend.userId = it.id();
        comFriend.weight = (int)boost::lexical_cast<float>(it.messages(0));
        BOOST_FOREACH(int i, it.fields()) {
          comFriend.shares.push_back(i);
        }
        _ret.push_back(comFriend);
      }
    }
  }
}
	catch(Ice::Exception& e)
	{
		MCE_WARN("getFoFComm IceException" << e);
	}
	catch(std::exception& e)
	{
		MCE_WARN("getFoFComm StdException" << e.what());
	}
	catch(...)
	{
		MCE_WARN("getFoFComm unknown exception");
	}

  return ( _ret.end() - _ret.begin() > limit ) ? CommonFriendSeq( _ret.begin(), _ret.begin()+limit ) : CommonFriendSeq( _ret );
}

