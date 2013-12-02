/*
 * FeedQueueI.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: yejingwei
 */

#include "ServiceI.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "FeedNaiveBayesI.h"

using namespace std;
using namespace boost;
using namespace MyUtil;
using namespace xce::feed;
using namespace com::xiaonei::xce;

//注意Bayes服务 老的HA是在XF下面的  原来的registask用的是xceFeed，
// static  xce::serverstate::ServerStateSubscriber _subscriber1;
// static  xce::serverstate::ServerStateSubscriber _subscriber2;
void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&FeedNaiveBayesI::instance(), service.createIdentity("M", ""));

  //WhiteListHandler::instance().LoadWhitelist();
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("FeedNaiveBayes.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FeedNaiveBayes.Interval", 5);
  xce::serverstate::ServerStateSubscriber::instance().initialize(
          "ControllerFeedNaiveBayesR", &FeedNaiveBayesI::instance(), mod, interval,  new XceFeedControllerChannel());//TODO  //change to XceFeedControllerChannel
  // register to 2 controllers
  // _subscriber1.initialize("ControllerFeedNaiveBayesR", &FeedNaiveBayesI::instance(),
          // mod, interval, new XceFeedChannel());
  // _subscriber2.initialize("ControllerFeedNaiveBayesR", &FeedNaiveBayesI::instance(),
          // mod, interval, new XceFeedControllerChannel());
  MCE_INFO("FeedNaiveBayesI::RegisterControllerR. mod:" << mod << " interval:" << interval);
  //初始化
  TaskManager::instance().execute(new ReloadTask());
}


//FeedDispatcher::GetScoreDict( feed->actor, target, stype)
map<int,int>  FeedNaiveBayesI::GetScoreDict(int fromid, const vector<int> & toids, int stype, const Ice::Current& ) {  
	TimeStat ts;
	map<int,int> dict;
	if (!ScoreCache::instance().ready()) {
		MCE_INFO("FeedNaiveBayesI::GetScoreDict. not ready. fromid:" << fromid  );
		for(unsigned i= 0; i != toids.size(); ++i){
			dict[toids.at(i)] = 15;
		}
		return dict;// use the default value
	}

	CaculateHelper caculater(fromid, false);
	int combined;
	BOOST_FOREACH(int toid, toids) {
		combined = caculater.Caculate(fromid, toid, stype);
		dict[toid] = combined;
	}
	MCE_INFO("FeedNaiveBayesI::GetScoreDict. -->  toids.size:" << toids.size()
			<< " fromid:" << fromid << " stype:" << stype
			<< " dictsize:" << dict.size()
			<< " cost:" << ts.getTime()
			<< caculater.ShowInfo();

	);
	return dict;
}

map<int,int>  FeedNaiveBayesI::GetScoreDictReverse(int toid, const vector<int> & fromids, int stype, const Ice::Current& ){
	TimeStat ts;
	map<int,int> dict;
	if (!ScoreCache::instance().ready()) {
		MCE_INFO("FeedNaiveBayesI::GetScoreDictReverse. not ready. toid:" << toid  );
		for(unsigned i= 0; i != fromids.size(); ++i){
			dict[fromids.at(i)] = 15;
		}
		return dict;
	}

	CaculateHelper caculater(toid, true);
	int combined;
	BOOST_FOREACH(int fromid, fromids){
		combined = caculater.Caculate(fromid, toid, stype);
		dict[fromid] = combined;
	}
	MCE_INFO("FeedNaiveBayesI::GetScoreDictReverse. --> fromids.size:" << fromids.size()
			<< " toid:" << toid << " stype:" << stype
			<< " dictsize:" << dict.size()
			<< " cost:" << ts.getTime()
			<<	caculater.ShowInfo()
	);
	return dict;
}

int  FeedNaiveBayesI::GetSendFrequency(int fromid, const Ice::Current&){
	int res = FrequencyCache::instance().GetFrequency(fromid);
	MCE_INFO("FeedNaiveBayesI::GetSendFrequency.  --> fromid:" << fromid << " res:" << res);
	return res;
}

void FeedNaiveBayesI::ReloadCache(const Ice::Current&) {
  MCE_INFO("FeedNaiveBayesI::ReloadCache. --> begin to reload score file!");
  TaskManager::instance().execute(new ReloadTask());
}

int FeedNaiveBayesI::UpdateScoreBatch(const IntSeq & userids, const IntSeq & featureids, const FloatSeq & scores, const Ice::Current& ) {
  ScoreCache::instance().UpdateScoreBatch(userids, featureids, scores);  
  return 1;
}

void FeedNaiveBayesI::MergeNew(const string & new_score,const string & dump,const Ice::Current&){
	//ScoreCache::instance().MergeNew(new_score, dump);
}

void FeedNaiveBayesI::RegisterController(int status) {
  try {
	  xce::serverstate::ServerStateSubscriber::instance().setStatus(status);
	  MCE_INFO("FeedNaiveBayesI::RegisterController.  Set Register Status sucess --> status:" << status);
	}catch (Ice::Exception& e) {
		MCE_WARN("FeedNaiveBayesI::RegisterController. --> status:" << status  << " error:  "<< e);
	}catch(...){
		MCE_WARN("FeedNaiveBayesI::RegisterController. --> status:" << status << " unknown error" );
	}
}






